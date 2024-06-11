#include "stdafx.h"
#include "pch.h"
#include "StringUtility.h"

namespace seago
{
	bool IsZero(real p_Value, real p_Tolerance)
	{
		assert(p_Tolerance > 0);
		return fabs(p_Value) <= p_Tolerance;
	}

	bool IsNotZero(real p_Value, real p_Tolerance)
	{
		return !IsZero(p_Value, p_Tolerance);
	}

	bool IsLess(real p_Value, real p_Target, real p_Tolerance)
	{
		assert(p_Tolerance > 0);
		return p_Value < (p_Target - p_Tolerance);
	}

	string IntToString(int int_num)
	{
		return to_string(int_num);
	}

	//暂时默认有6位小数
	string DoubleToString(double double_num)
	{
		return to_string(double_num);
	}
	//	String ToString(const char* p_String)
	//	{
	//		assert(nullptr != p_String);
	//
	//#ifdef  UNICODE                     // r_winnt
	//		wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	//		return converter.from_bytes(p_String);
	//#else   /* UNICODE */               // r_winnt
	//		return String(p_String);
	//#endif /* UNICODE */
	//	}


	string ToCharString(const wstring& p_Value)
	{
		return string(p_Value.begin(), p_Value.end());
	}

	//String FormatString(const Char* p_Format, ...)
	//{
	//	String l_Return;
	//
	//	va_list l_Marker = NULL;
	//	va_start(l_Marker, p_Format);
	//
	//#ifdef  UNICODE    
	//	size_t l_FormatSize = _vscwprintf(p_Format, l_Marker);
	//#else
	//	size_t l_FormatSize = _vscprintf(p_Format, l_Marker);
	//#endif
	//
	//
	//	l_Return.resize(l_FormatSize + 1);
	//
	//
	//#ifdef  UNICODE    
	//	vswprintf_s((Char*)l_Return.data(), l_Return.capacity(), p_Format, l_Marker);
	//#else
	//	vsprintf_s((Char*)l_Return.data(), l_Return.capacity(), p_Format, l_Marker);
	//#endif
	//
	//	va_end(l_Marker);
	//
	//	l_Return.pop_back();
	//
	//	return l_Return;
	//}
	
	//vector<String> SplitBy(const String& p_String, Char p_Split)
	//{
	//	String delimiter = ToString(p_Split); // 将 Char 转换为 String
	//	return SplitBy(p_String, delimiter);//返回一个SplitBy的重载
	//}

	/*StringArray SplitBy(const String& p_String, const vector<Char>& p_Splits)
	{
		vector<String> splitStrings(p_Splits.size(), _T(""));

		for (const auto& splitChar : p_Splits)
		{
			splitStrings.push_back(ToString(splitChar));
		}

		return SplitBy(p_String, splitStrings);
	}*/

	vector<String> SplitBy(const String& p_String, const String& p_Split)
	{
		vector<String> l_Result;

		assert(p_String.size() > 0 && p_Split.size() == 1);

		String l_CopyString = p_String;

		size_t l_Position;

		size_t copyStringsize = l_CopyString.size();

		if (l_CopyString.rfind(p_Split) != copyStringsize - 1)
		{
			l_CopyString += p_Split;
		}

		for (size_t i = 0, size = l_CopyString.size(); i < size; i++)
		{
			l_Position = l_CopyString.find(p_Split, i);

			if (l_Position < size && l_Position != 0)
			{
				String l_SplitString = l_CopyString.substr(i, l_Position - i);
				l_Result.push_back(l_SplitString);
				i = int(l_Position + p_Split.size() - 1);
			}
		}
		return l_Result;
	}
	vector<String> SplitBy(const String& p_String, Char p_Split)
	{
		String delimiter = ToString(p_Split); // 将 Char 转换为 String
		return SplitBy(p_String, delimiter);//返回一个SplitBy的重载
	}


	//////////////////////////////////////////////////////////////////////////
	vector<String> SplitBy(const String& p_String, const vector<String>& p_Splits)
	{
		vector<String> r_Result;

		assert(p_String.size() > 0 && p_Splits.size() > 0);

		String copyString = p_String;

		size_t position = 0;
		size_t splitIndex = 0;

		copyString += p_Splits.at(0);

		for (size_t i = 0; i < copyString.size(); i++)
		{
			position = 0;
			splitIndex = 0;

			for (size_t j = 0, splitSize = p_Splits.size(); j < splitSize; j++)
			{
				const String& split = p_Splits.at(j);

				size_t p = copyString.find(split, i);

				if (0 == position || p < position)
				{
					position = p;
					splitIndex = j;
				}
			}

			if (position < copyString.size())
			{
				String l_SplitString = copyString.substr(i, position - i);
				r_Result.push_back(l_SplitString);
				i = int(position + p_Splits.at(splitIndex).size() - 1);
			}
		}

		return r_Result;
	}

	StringArray SplitBy(const String& p_String, const vector<size_t>& p_SplitPosition)
	{
		if (p_String.empty() || p_SplitPosition.empty())
		{
			assert(false);
			throw invalid_argument(" __FUNCSIG__ 输入参数为空 !");
		}

		StringArray r_Result;
		{

			size_t begin_pos = 0;
			size_t end_pos = 0;

			for (size_t i = 0, size = p_SplitPosition.size(); i < size; i++)
			{
				end_pos = p_SplitPosition.at(i);

				if (end_pos <= begin_pos)
				{
					assert(false);
					throw invalid_argument(" __FUNCSIG__ 输入参数错误 !");
				}

				//end_pos - begin_pos 不加 1 ，因为不需要包含分隔符
				r_Result.push_back(p_String.substr(begin_pos, end_pos - begin_pos));

				begin_pos = end_pos + 1;
			}

			r_Result.push_back(p_String.substr(begin_pos)); //加入最后一段
		}

		return r_Result;
	}

	String RemoveSuccessive(const String& p_String, Char p_Char)
	{
		assert(false == p_String.empty());

		String result;

		size_t stringSize = p_String.size();
		result.reserve(stringSize);

		Char last = p_String.at(0);
		result.push_back(last);

		for (auto itr = p_String.begin() + 1; itr != p_String.end(); itr++)
		{
			const Char& current = *itr;

			if (current != p_Char || last != current)
			{
				result.push_back(current);
				last = current;
			}
		}

		return result;
	}


	bool IsBeginWith(const String& p_String, const String& p_Begin)
	{
		if (true == p_String.empty() || p_Begin.empty())
		{
			assert(false);
			throw invalid_argument(" __FUNCSIG__ 输入参数为空 !");
		}

		size_t begin_size = p_Begin.size();

		//如果 p_Begin 的长度 大于 p_String 长度，则不能是 p_String 的开头，返回 false
		//否则 取 p_String 的开头字符串 和 p_Begin 对比
		return p_String.size() < begin_size ? false : (p_String.substr(0, begin_size) == p_Begin);
	}

	vector<size_t> FindNotInTagRange(const String& p_String, const array2<Char>& p_Tag, Char p_Char)
	{
		if (true == p_String.empty())
		{
			assert(false);
			throw invalid_argument(" __FUNCSIG__ 输入参数 p_String 为空 !");
		}

		const Char& begin_tag = p_Tag.at(0); //起始 Tag 例如 '('
		const Char& end_tag = p_Tag.at(1); //终止 Tag 例如 ')'

		//该参数用于记录在 Tag 中的层级， 例如 "((test))" 在 "test"范围内 tag_level = 2
		//tag_level = 0 代表不在任何 Tag 中
		int tag_level = 0;

		vector<size_t> r_Result; //通过遍历字符串 p_String 求出不在 Tag 范围内的 p_Char 字符的位置
		{

			for (size_t pos = 0, size = p_String.size(); pos < size; pos++)
			{
				const Char& c = p_String.at(pos);

				if (c == begin_tag)
				{
					tag_level++;
					continue;
				}

				if (c == end_tag)
				{
					if (tag_level == 0) {
						assert(false);
						throw invalid_argument(" __FUNCSIG__ 输入参数 p_String 中的 Tag 标记不是配对出现!");
					}
					tag_level--;
					continue;
				}

				if (c == p_Char && 0 == tag_level)
				{
					r_Result.push_back(pos);
					continue;
				}
			}

			assert(tag_level >= 0);
			if (tag_level > 0)
			{
				assert(false);
				throw invalid_argument(" __FUNCSIG__ 输入参数 p_String 中的 Tag 标记不是配对出现!");
			}
		}

		return r_Result;
	}

	
	/*vector<String> splitWs(const wstring& s, Char delim) {
		vector<String> elems;
		splitw(s, delim, elems);
		return elems;
	}
	void splitw(const wstring& s, Char delim, vector<String>& elems) {
		StringStream ss;
		ss.str(s);
		String item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
	}*/

	/*String string2wstring(const string& s)
	{
		setlocale(LC_ALL, "chs");

		const char* _Source = s.c_str();
		size_t _Dsize = s.size() + 1;
		Char* _Dest = new Char[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, _Dest, _Dsize, _Source, _TRUNCATE);
		String result = _Dest;
		delete[]_Dest;

		setlocale(LC_ALL, "C");

		return result;
	}*/

}
vector<String> splitWs(const wstring& s, Char delim) {
	vector<String> elems;
	splitw(s, delim, elems);
	return elems;
}
void splitw(const wstring& s, Char delim, vector<String>& elems) {
	StringStream ss;
	ss.str(s);
	String item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}
String string2wstring(const string& s)
{
	setlocale(LC_ALL, "chs");

	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	Char* _Dest = new Char[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, _Dest, _Dsize, _Source, _TRUNCATE);
	String result = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, "C");

	return result;
}