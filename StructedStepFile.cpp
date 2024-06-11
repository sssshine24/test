#include "stdafx.h"
#include "pch.h"
#include "StructedStepFileDataItem.h"
#include "StringUtility.h"
#include "StructedStepFile.h"



StructedStepFile::StructedStepFile() :
	m_Datas()
{

}

StructedStepFile::~StructedStepFile()
{
	Clear();
}

void StructedStepFile::LoadInitialize()
{
	Clear();
}
bool StructedStepFile::Load(const String& p_Path)
{

	if (0 == p_Path.size())
	{
		return false;
	}

	IFStream inFile;
	inFile.open(p_Path);

	if (false == inFile.is_open())
	{
		assert(false);
		return false;
	}

	LoadInitialize();

	bool isParseCorrect = false;

	try
	{
		isParseCorrect = LoadParse(inFile);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		assert(false);
	}
	catch (...)
	{
		assert(false);
	}

	inFile.close();

	return isParseCorrect;
}

bool StructedStepFile::LoadParse(IFStream& p_InFile)
{
	assert(0 == m_Datas.size());

	String line;
	bool isInDataBlock = false;
	//bool isFirstDataItem = true;
	bool isMultiLineDataItem = false;

	size_t expectedItemId = 0;

	//将数据一次性读入内存，并转换成宽字符串
	std::stringstream buffer;
	buffer << p_InFile.rdbuf();
	std::string contents(buffer.str());
	std::wstring fileDatas = string2wstring(contents);

	//对fileDatas中的数据进行处理
	std::vector<std::wstring> result;

	result = splitWs(fileDatas, L'\n');

	for (int i = 0; i < result.size(); ++i)
	{
		std::wstring line = result[i];

		if (false == isInDataBlock)
		{
			isInDataBlock = IsDataSectionBegin(line);
			continue;
		}

		if (true == isInDataBlock && false == IsSectionEnd(line))
		{
			expectedItemId++;

			if (false == IsSingleLineDataItem(line))
			{
				assert(true == IsMultiLineDataItemBegin(line));

				int j = i + 1;
				for (; j < result.size(); ++j)
				{
					std::wstring lineInMultiLine = result[j];
					//判断最后一个字符是否为逗号,如果不是逗号并且不是分号，则加上逗号
					if (isInsertComma(line)) {
						line += _T(",") + lineInMultiLine;
					}
					else {
						line += lineInMultiLine;
					}

					if (true == IsMultiLineDataItemEnd(lineInMultiLine))
					{
						break;
					}
				}
				i = j;
			}

			assert(true == IsSingleLineDataItem(line));

			auto itemInfo = NormalizeDataItem(line);

			//如果 读取的 id 不等于 预期 id 则需要补全
			if (expectedItemId < itemInfo.Id)
			{
				FillNonExistDataItem(itemInfo.Id);
				expectedItemId = itemInfo.Id;
			}

			assert(expectedItemId >= itemInfo.Id); // 大于情况出现在 中性文件中 id 是无序的

			//解析 line 的内容
			StructedStepFileDataItem* pNewDataItem = ParseStructedDataItem(itemInfo, line);
			assert(nullptr != pNewDataItem);

			//保存解析的结果
			
			if (expectedItemId == itemInfo.Id)
			{
				m_Datas.push_back(pNewDataItem);
			}
			else
			{
				auto& pDataItem = m_Datas.at(itemInfo.Id - 1); //auto = StructedStepFileDataItem* 这里简写主要是强调是获得的引用
				assert(nullptr != pDataItem);
				assert(true == pDataItem->IsNull());
				delete pDataItem;
				pDataItem = pNewDataItem;

				expectedItemId--;
			}

			//找到ManifoldSolidBrep的索引值 或 ShellBasedSurfaceModel 的索引值
			if (pNewDataItem->m_StepType == _T("MANIFOLD_SOLID_BREP"))
			{
				m_ManifoldSolidBrepIndexs.push_back(pNewDataItem->m_StepId - 1);
			}
			if (pNewDataItem->m_StepType == _T("SHELL_BASED_SURFACE_MODEL"))
			{
				m_ShellBasedSurfaceModelIndexs.push_back(pNewDataItem->m_StepId - 1);
			}

			//将解析结果另外保存到一个一维数组和并行处理数组中，为并行处理及处理其他除拓扑几何外的数据做准备
			/*size_t itemId = -1;
			if (m_TypeToNum.find(pNewDataItem->m_StepType) != m_TypeToNum.end())
			{
				itemId= m_TypeToNum.at(pNewDataItem->m_StepType);
			}
			else {
				m_OtherDatas.push_back(pNewDataItem);
			}

			switch (itemId)
			{
			case 0:
				m_ParallelDatas[0].push_back(pNewDataItem);
				break;
			case 1:
				m_ParallelDatas[1].push_back(pNewDataItem);
				break;
			case 2:
				m_ParallelDatas[2].push_back(pNewDataItem);
				break;
			case 3:
				m_ParallelDatas[3].push_back(pNewDataItem);
				break;
			case 4:
				m_ParallelDatas[4].push_back(pNewDataItem);
				break;
			case 5:
				m_ParallelDatas[5].push_back(pNewDataItem);
				break;
			case 6:
				m_ParallelDatas[6].push_back(pNewDataItem);
				break;
			case 7:
				m_ParallelDatas[7].push_back(pNewDataItem);
				break;
			case 8:
				m_ParallelDatas[8].push_back(pNewDataItem);
				break;
			case 9:

				m_ParallelDatas[9].push_back(pNewDataItem);
				break;
			default:
				assert(true);

			}*/

			continue;
		}

		break;
	}

	return true;
}

bool StructedStepFile::isInsertComma(String& lineInMultiLine)
{
	char character = lineInMultiLine[lineInMultiLine.length() - 1];

	//一行字符串的最后一个字符是空格的情况
	if (character == ' ')
	{
		if (lineInMultiLine[lineInMultiLine.length() - 2] == _T('('))
		{
			return false;
		}
		return true;
	}

	if (character != ',' && character != ';' && character != '(')
		return true;


	return false;
}
const StructedStepFileDataItem& StructedStepFile::operator[](size_t p_Index) const
{
	return *(m_Datas.at(p_Index));
}

void StructedStepFile::Clear()
{
	for (auto* pDataItem : m_Datas)
	{
		if (nullptr != pDataItem)
		{
			delete pDataItem;
		}
	}

	m_Datas.clear();
}

bool StructedStepFile::IsDataSectionBegin(const String& p_Line)
{
	//共 2 个条件来确定是 数据头， 等更多例子调试
	//1. 非空
	//2. 起始字符串为 _T("DATA;") //这里暂时不要求绝对等于， 主要防止有空格等不标准表示

	return (false == p_Line.empty()) && (0 == p_Line.compare(_T("DATA;")));
}

bool StructedStepFile::IsSingleLineDataItem(const String& p_Line)
{
	//共 3 个条件来确定是 单行 数据项， 等更多例子调试
	//1. 非空 //结合后 2 个条件， 用 2 <= p_Line.size() 替代非空
	//2. 起始字符为 #
	//3. 中止符为 ;

	return (2 <= p_Line.size()) && ('#' == p_Line.front()) && (';' == p_Line.back());
}

bool StructedStepFile::IsSectionEnd(const String& p_Line)
{
	//共 2 个条件来确定是 SectionEnd， 等更多例子调试
	//1. 非空
	//2. 起始字符串为 _T("ENDSEC;") //这里暂时不要求绝对等于， 主要防止有空格等不标准表示
	return (false == p_Line.empty()) && (0 == p_Line.compare(_T("ENDSEC;")));
}

bool StructedStepFile::IsMultiLineDataItemBegin(const String& p_Line)
{
	//共 3 个条件来确定是 多行数据项的 Begin ， 等更多例子调试
	//1. 非空 //结合后 1 个条件， 用 1 <= p_Line.size() 替代非空
	//2. 起始字符为 #
	//3. 中止符 不 为 ;

	return (1 <= p_Line.size()) && ('#' == p_Line.front()) && (';' != p_Line.back());
}

bool StructedStepFile::IsMultiLineDataItemEnd(const String& p_Line)
{
	//共 2 个条件来确定是 多行数据项的 End， 等更多例子调试
	//1. 非空 //结合后 2 个条件， 用 2 <= p_Line.size() 替代非空
	//2. 中止符为 ;

	return (1 <= p_Line.size()) && (';' == p_Line.back());
}

StructedStepFile::DataItemInfo StructedStepFile::NormalizeDataItem(String& p_Line)
{
	assert(true == IsSingleLineDataItem(p_Line));

	//DataItem 的核心表示形式为 index = value 例如
	//#1 = CARTESIAN_POINT ( 'NONE',  ( 9.9, 0.0, 30.0 ) ) ;

	//标准化 DataItem 主要完成如下工作:
	//1. 移除 结束符号 ';' //结束符号 ';' 直接 pop 移除， 起始符号 '#' 由于没有 pop_front，所以为了效率，不在 标准化 工作中完成
	//2. 移除 ' '（空格）、'#' 、'\''（单引号） //暂时这 3 种符号， 后续随着具体实例扩展
	//3. 对 DataItem 的 value 部分，基于 () 的嵌套进行层级划分，暂时采用这种方式， 后续可能会扩展为树进行表示，具体为
	//		第 0 层级 分割符 为 ' ' //不会出现这种分割，为了代码方便， 默认已经在 0 层级
	//		第 1 层级 分割符 为 ','
	//		第 2 层级 分割符 为 '$'
	//		第 3 层级 分割符 为 '%'
	//		第 4 层级 分割符 为 '@' 
	//4. 判断 DataItem 是否为 无类型，判定标准为 '=' 号后直接跟 '(', 为暂时标准，后续随着具体实例扩展，例如
	//		#5 =( NAMED_UNIT ( * ) SI_UNIT ( $, .STERADIAN. ) SOLID_ANGLE_UNIT ( ) );
	//5. 提取 DataItem id（id >= 1） 


	DataItemInfo r_Info;

	size_t& equalMarkPos = r_Info.EqualMarkPos;

	size_t& firstLeftBracketPos = r_Info.FirstLeftBracketPos;
	size_t& lastRightBracketPos = r_Info.LastRightBracketPos;

	ushort bracketLevel = 0; //用于执行功能工作 3 默认为 0 层级

	String spliteCases = _T(" ,$%@");

	p_Line.pop_back(); //1. 移除 结束符号 ';'

	for (size_t i = 1; i < p_Line.size();) //i = 1 开始不判断对 起始的 '#' 处理
	{
		const Char& c = p_Line.at(i);

		switch (c)
		{
		case ' ':
		case '#':
		case '\'': //单引号字符 
			p_Line.erase(i, 1); //删除 空格 # 和 . '
			continue; //避免连续出现需要删除的部分，所以 continue 不执行++ 
			assert(false); //确保正确 continue 防止移除代码后错误

		case '=':
			assert(0 == equalMarkPos); //确保 DataItem 中只有 1 个等号
			equalMarkPos = i;
			break;

		case '(':
			if (0 == firstLeftBracketPos) //第一个左括号作为 value 起始的位置
			{
				firstLeftBracketPos = i;
			}

			bracketLevel++;
			break;

		case ')':
			assert(0 < bracketLevel);
			lastRightBracketPos = i;
			bracketLevel--;
			break;

		case ',':
			if (1 < bracketLevel) // 1 == bracketLevel 时，是 Step 文件标准中默认的 ',' 分割， 所以不处理
			{
				assert(spliteCases.size() > bracketLevel); //防止层级没有定义的情况
				p_Line.at(i) = spliteCases.at(bracketLevel);
			}
			break;

		default:
			break;
		}

		i++;
	}

	assert(2 <= equalMarkPos);
	assert(equalMarkPos < firstLeftBracketPos&& firstLeftBracketPos < lastRightBracketPos);

	r_Info.IsUnknowType = (equalMarkPos + 1 == firstLeftBracketPos);

	String idStr = p_Line.substr(1, equalMarkPos - 1);
	r_Info.Id = stoul(idStr);
	assert(r_Info.Id >= 1);

	return r_Info;
}

StructedStepFileDataItem* StructedStepFile::ParseStructedDataItem(const DataItemInfo& p_Info, const String& p_DataItem)
{
	//解析 DataItem 主要工作包括：

	if (true == p_Info.IsUnknowType) //是无类型的
	{

		String stepTypeStr = _T("unknow");
		String stepValueStr = p_DataItem;

		size_t leftIdx = p_Info.FirstLeftBracketPos;

		//曲线的一种特殊情况
		String str = p_DataItem.substr(leftIdx + 1, 13);
		if (str == _T("BOUNDED_CURVE"))
		{
			stepTypeStr = _T("RATIONAL_B_SPLINE_CURVE");
			//得到属性值字符串
			size_t stepValueStrSize = p_Info.LastRightBracketPos - p_Info.FirstLeftBracketPos - 1;
			String valueStr = p_DataItem.substr(p_Info.FirstLeftBracketPos + 1, stepValueStrSize);

			stepValueStr = GetB_SplineCurveValue(valueStr); //对属性值字符串进一步处理，将其中包含的字母等不必要的内容去掉
		}

		//曲面的一种特殊情况 
		String str1 = p_DataItem.substr(leftIdx + 1, 15);
		if (str1 == _T("BOUNDED_SURFACE"))
		{
			stepTypeStr = _T("RATIONAL_B_SPLINE_SURFACE");
			//得到属性值字符串
			size_t stepValueStrSize = p_Info.LastRightBracketPos - p_Info.FirstLeftBracketPos - 1;
			String valueStr = p_DataItem.substr(p_Info.FirstLeftBracketPos + 1, stepValueStrSize);

			stepValueStr = GetB_SplineSurfaceValue(valueStr);
		}

		auto* pNewDataItem = new StructedStepFileDataItem(p_Info.Id, stepTypeStr, stepValueStr);
		assert(nullptr != pNewDataItem);
		return pNewDataItem;
	}
	else
	{
		size_t stepTypeStrSize = p_Info.FirstLeftBracketPos - p_Info.EqualMarkPos - 1;
		assert(stepTypeStrSize > 0);
		//得到step类型
		String stepTypeStr = p_DataItem.substr(p_Info.EqualMarkPos + 1, stepTypeStrSize);

		String stepValueStr = _T("");

		if (p_Info.LastRightBracketPos > p_Info.FirstLeftBracketPos + 1)
		{
			size_t stepValueStrSize = p_Info.LastRightBracketPos - p_Info.FirstLeftBracketPos - 1; //不包括 左右 括号

			if (',' == p_DataItem.at(p_Info.FirstLeftBracketPos + 1))
			{
				//name为“”时，将name属性设为“null”
				stepValueStr = _T("null") + p_DataItem.substr(p_Info.FirstLeftBracketPos + 1, stepValueStrSize);
			}
			else
			{
				stepValueStr = p_DataItem.substr(p_Info.FirstLeftBracketPos + 1, stepValueStrSize);
			}
		}

		auto* pNewDataItem = new StructedStepFileDataItem(p_Info.Id, stepTypeStr, stepValueStr);
		assert(nullptr != pNewDataItem);
		return pNewDataItem;
	}

	assert(false);
	return nullptr;
}

/*
函数作用：
主要用来处理一种特殊曲线的属性值
BOUNDED_CURVE()
B_SPLINE_CURVE(3$(58743%123242%93817%29258)$.UNSPECIFIED.$.F.$.T.),B_SPLINE_CURVE_WITH_KNOTS((4%4)$(4.814598377544293800%4.930555136883981500)$.UNSPECIFIED.),
CURVE()GEOMETRIC_REPRESENTATION_ITEM()RATIONAL_B_SPLINE_CURVE((1.000000000000000000%0.9988798163404487600%0.9988798163404487600%1.000000000000000000)),REPRESENTATION_ITEM()

处理成：
3$(58743%123242%93817%29258)$.UNSPECIFIED.$.F.$.T.,(4%4)$(4.814598377544293800%4.930555136883981500)$.UNSPECIFIED.,(1.000000000000000000%0.9988798163404487600%0.9988798163404487600%1.000000000000000000)

首先需要遍历字符串，获取三对括号：
分别B_SPLINE_CURVE、B_SPLINE_CURVE_WITH_KNOTS、RATIONAL_B_SPLINE_CURVE后紧跟的三对括号
然后截取每队括号中间的字符串

获取括号的方法：
左括号进行计数：
记录第2个，第 4 个和第 9 个左括号的位置
右括号：第3个，第 6 个和第 10 个左括号的位置

*/
String StructedStepFile::GetB_SplineCurveValue(String& stepValueStr)
{
	String str = _T("");

	std::vector<size_t> BracketPos; //定义一个二维数组，用来记录括号的位置

	std::stack<size_t> bracket;

	int bracketNum = 0;

	//遍历字符串，获取三对括号的位置
	for (size_t i = 0; i < stepValueStr.size(); ++i)
	{
		const Char& c = stepValueStr.at(i);

		switch (c)
		{
		case '(':
			bracket.push(i);
			break;
		case ')':
			if (bracket.size() == 1)
			{
				bracketNum++;
				if (bracketNum == 2 || bracketNum == 3 || bracketNum == 6)
				{
					size_t idx = bracket.top();
					BracketPos.emplace_back(idx);
					BracketPos.emplace_back(i);
				}
			}
			bracket.pop();
			break;
		default:
			break;
		}
	}

	//获取B_SPLINE_CURVE的属性值
	str += stepValueStr.substr(BracketPos[0] + 1, BracketPos[1] - BracketPos[0] - 1) + _T(",");

	//获取B_SPLINE_CURVE_WITH_KNOTS的属性值
	str += stepValueStr.substr(BracketPos[2] + 1, BracketPos[3] - BracketPos[2] - 1) + _T(",");

	//RATIONAL_B_SPLINE_CURVE的属性值
	str += stepValueStr.substr(BracketPos[4] + 1, BracketPos[5] - BracketPos[4] - 1);

	return str;
}

/*
首先需要遍历字符串，获取三对括号：
分别B_SPLINE_SURFACE、B_SPLINE_SURFACE_WITH_KNOTS、RATIONAL_B_SPLINE_SURFACE后紧跟的三对括号
然后截取每队括号中间的字符串

获取括号的方法：利用括号匹配
*/
String StructedStepFile::GetB_SplineSurfaceValue(String& stepValueStr)
{
	String str = _T("");

	std::vector<size_t> BracketPos; //定义一个二维数组，用来记录括号的位置

	std::stack<size_t> bracket;

	int bracketNum = 0;

	//遍历字符串，获取三对括号的位置
	for (size_t i = 0; i < stepValueStr.size(); ++i)
	{
		const Char& c = stepValueStr.at(i);

		switch (c)
		{
		case '(':
			bracket.push(i);
			break;
		case ')':
			if (bracket.size() == 1)
			{
				bracketNum++;
				if (bracketNum == 2 || bracketNum == 3 || bracketNum == 5)
				{
					size_t idx = bracket.top();
					BracketPos.emplace_back(idx);
					BracketPos.emplace_back(i);
				}
			}
			bracket.pop();
			break;
		default:
			break;
		}
	}

	//首先获取B_SPLINE_SURFACE的属性值
	str += stepValueStr.substr(BracketPos[0] + 1, BracketPos[1] - BracketPos[0] - 1) + _T(",");

	//获取B_SPLINE_SURFACE_WITH_KNOTS的属性值
	str += stepValueStr.substr(BracketPos[2] + 1, BracketPos[3] - BracketPos[2] - 1) + _T(",");

	//RATIONAL_B_SPLINE_SURFACE的属性值
	str += stepValueStr.substr(BracketPos[4] + 1, BracketPos[5] - BracketPos[4] - 1);

	return str;
}

void StructedStepFile::FillNonExistDataItem(size_t p_NonExistDataItemCount)
{
	//临时用于补全中性文件中出现序号不连贯的情况，以保证整体框架可运行

	assert(p_NonExistDataItemCount > 1); // == 1 的情况就不需要补充

	size_t currentDataItemCount = m_Datas.size();
	assert(p_NonExistDataItemCount > (currentDataItemCount + 1)); //由于无符号， 所以需要先判断而不做运算
	size_t nonExistDataItemCount = p_NonExistDataItemCount - currentDataItemCount - 1;

	std::vector<StructedStepFileDataItem*>nonExistDataItems(nonExistDataItemCount);

	std::generate(nonExistDataItems.begin(), nonExistDataItems.end(), []() { return new StructedStepFileDataItem(-1, _T(""), _T("")); });

	m_Datas.insert(m_Datas.end(), nonExistDataItems.begin(), nonExistDataItems.end());
}




std::vector<size_t> StructedStepFile::GetManifoldSolidBrepIndexs() const
{
	return m_ManifoldSolidBrepIndexs;
}
std::vector<size_t> StructedStepFile::GetShellBasedSurfaceModelIndexs() const
{
	return m_ShellBasedSurfaceModelIndexs;
}

//unordered_map<String, size_t> StructedStepFile::initialMap()
//{
//	unordered_map<String, size_t> mmap;

//	//所有的几何点和direction都放到第一层
//	mmap.insert(make_pair(_T("CARTESIAN_POINT"), 0));
//	mmap.insert(make_pair(_T("DIRECTION"), 0));

//	//所有的vertex和placement（AXIS2_PLACEMENT_3D），vector在第二层
//	mmap.insert(make_pair(_T("AXIS2_PLACEMENT_3D"), 1));
//	mmap.insert(make_pair(_T("VERTEX_POINT"), 1));
//	mmap.insert(make_pair(_T("VECTOR"), 1));

//	//所有的几何曲线 和 几何曲面 在第三层
//	mmap.insert(make_pair(_T("CIRCLE"), 2));
//	mmap.insert(make_pair(_T("ELLIPSE"), 2));
//	mmap.insert(make_pair(_T("LINE"), 2));
//	mmap.insert(make_pair(_T("B_SPLINE_CURVE_WITH_KNOTS"), 2));

//	mmap.insert(make_pair(_T("PLANE"), 2));
//	mmap.insert(make_pair(_T("CYLINDRICAL_SURFACE"), 2));
//	mmap.insert(make_pair(_T("SPHERICAL_SURFACE"), 2));
//	mmap.insert(make_pair(_T("CONICAL_SURFACE"), 2));
//	mmap.insert(make_pair(_T("TOROIDAL_SURFACE"), 2));
//	mmap.insert(make_pair(_T("B_SPLINE_SURFACE_WITH_KNOTS"), 2));

//	//拓扑边放在第四层
//	mmap.insert(make_pair(_T("EDGE_CURVE"), 3));

//	//半边放在第五层
//	mmap.insert(make_pair(_T("ORIENTED_EDGE"), 4));

//	//Loop放在第六层
//	mmap.insert(make_pair(_T("EDGE_LOOP"), 5));

//	//面边界face_bound放在第七层
//	mmap.insert(make_pair(_T("FACE_BOUND"), 6));
//	mmap.insert(make_pair(_T("FACE_OUTER_BOUND"), 6));

//	//拓扑面face 放在第八层
//	mmap.insert(make_pair(_T("ADVANCED_FACE"), 7));

//	//壳放在第9层
//	mmap.insert(make_pair(_T("CLOSED_SHELL"), 8));

//	//ManifoldSolidBrep,ShellBasedSurfaceModel 放在第10层
//	mmap.insert(make_pair(_T("MANIFOLD_SOLID_BREP"), 9));
//	mmap.insert(make_pair(_T("SHELL_BASED_SURFACE_MODEL"), 9));

//	return mmap;
//}