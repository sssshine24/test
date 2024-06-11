#pragma once
#include "TextFile.h"

class StructedStepFileDataItem;

class SG_DECLSPEC StructedStepFile //: public TextFile
{

protected:
	std::vector<StructedStepFileDataItem*> m_Datas; //使用指针来减少 vector 申请空间的消耗， 后续需要使用 索引访问， 所以无法使用 list	

	std::vector<StructedStepFileDataItem*> m_OtherEntities; // 存放除拓扑、几何之外的数据 


//暂时性的
protected:
	std::vector<size_t>  m_ManifoldSolidBrepIndexs; //得到ManifoldSolidBrep的索引值数组
	std::vector<size_t>  m_ShellBasedSurfaceModelIndexs; //ShellBasedSurfaceModel 的索引值	

public:
	std::vector<size_t> GetManifoldSolidBrepIndexs() const;
	std::vector<size_t> GetShellBasedSurfaceModelIndexs() const;


public:
	StructedStepFile();
	~StructedStepFile();

public:
	bool Load(const String& p_Path);

protected://override		
	void LoadInitialize();
	bool LoadParse(IFStream& p_InFile);

public:
	//const String& GetDataExpress(size_t p_Index) const;
	size_t GetDataCount() const { return m_Datas.size(); }
	//size_t GetDataItemBeginIndex() const { return m_BeginIndex; }

public:
	const StructedStepFileDataItem& operator[](size_t p_Index) const;

private:
	class DataItemInfo
	{
	public:
		bool IsUnknowType;
		size_t EqualMarkPos;

		size_t FirstLeftBracketPos;
		size_t LastRightBracketPos;

		size_t Id;

		DataItemInfo() :
			IsUnknowType(false),
			EqualMarkPos(0),
			FirstLeftBracketPos(0), LastRightBracketPos(0),
			Id(0)
		{

		}
	};

private:
	void Clear();

	bool IsDataSectionBegin(const String& p_Line);
	bool IsSingleLineDataItem(const String& p_Line);
	bool IsSectionEnd(const String& p_Line);

	bool IsMultiLineDataItemBegin(const String& p_Line);
	bool IsMultiLineDataItemEnd(const String& p_Line);

	DataItemInfo NormalizeDataItem(String& p_Line);
	StructedStepFileDataItem* ParseStructedDataItem(const DataItemInfo& p_Info, const String& p_DataItem);

	void FillNonExistDataItem(size_t p_FirstDataItemId);

	bool isInsertComma(String& lineInMultiLine);


	String GetB_SplineCurveValue(String& stepValueStr);
	String GetB_SplineSurfaceValue(String& stepValueStr);

};
