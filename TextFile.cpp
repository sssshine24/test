
#include "stdafx.h"
#include "pch.h"
#include "TextFile.h"

namespace seago
{

	bool TextFile::Load(const String& p_Path)
	{
		if (0 == p_Path.size())
		{
			return false;
		}

		InFileStream inFile;
		inFile.open(p_Path);

		if (false == inFile.is_open())
		{
			assert(false);
			return false;
		}

		LoadInitialize();

		bool isParseCorrect = LoadParse(inFile);

		inFile.close();

		return isParseCorrect;
	}

	void TextFile::LoadInitialize()
	{

	}

	bool TextFile::LoadParse(InFileStream& p_InFile)
	{
		assert(false);
		return false;
	}
}