#include "JsonWriter.h"


JsonWriter::JsonWriter(std::string filePath)
{
	m_filePath = filePath;
	m_awaitingCloseObject = false;
	m_awaitingCloseArray = false;

	m_numOpenArrays = 0;
	m_numOpenObjects = 0;

	p_mJsonWriter = 
		new rapidjson::PrettyWriter<rapidjson::StringBuffer>(m_jsonStringBuff);
}

JsonWriter::~JsonWriter()
{
	delete p_mJsonWriter;
}


bool JsonWriter::DumpToFile(bool &err)
{
	bool successFlag = false;

	std::ofstream outFile (m_filePath);

	if (outFile.is_open())
	{
		outFile << GetStringRepresentation(successFlag);
		outFile.close();
	}

	return successFlag;
}

std::string JsonWriter::GetStringRepresentation(bool &err)
{
	if (p_mJsonWriter->IsComplete())
	{
		err = false;
		return m_jsonStringBuff.GetString();

	} else
	{
		err = true;
		return "";
	}
	
}

void JsonWriter::StartObjectBlock()
{
	p_mJsonWriter->StartObject();
	m_awaitingCloseObject = true;
	m_numOpenObjects++;
}

void JsonWriter::StartArrayBlock(const std::string blockName)
{
	if (blockName.length() > 0 && m_awaitingCloseObject)
	{
		p_mJsonWriter-> String(blockName.c_str());
		p_mJsonWriter->StartArray();
		m_awaitingCloseArray = true;
		m_numOpenArrays++;
	}
	
}

void JsonWriter::EndObjectBlock()
{
	p_mJsonWriter->EndObject();
	m_numOpenObjects--;

	if (m_numOpenObjects == 0)
	{
		m_awaitingCloseObject = false;
	}
}

void JsonWriter::EndArrayBlock()
{
	p_mJsonWriter->EndArray();
	m_numOpenArrays--;

	if (m_numOpenArrays == 0)
	{
		m_awaitingCloseArray = false;
	}
}

void JsonWriter::InsertBoolValue(const std::string valueName, const bool theValue)
{
	if (valueName.length() > 0 && OkayToInsertKeyPair())
	{ 
		p_mJsonWriter->String(valueName.c_str());
		p_mJsonWriter->Bool(theValue);
	}
}

void JsonWriter::InsertStringValue(const std::string valueName, const std::string theValue)
{
	if (valueName.length() > 0 && OkayToInsertKeyPair())
	{
		p_mJsonWriter->String(valueName.c_str());
		p_mJsonWriter->String(theValue.c_str());
	}
}

void JsonWriter::InsertUnsignedIntValue(const std::string valueName, const uint32_t theValue)
{
	if (valueName.length() > 0 && OkayToInsertKeyPair())
	{
		p_mJsonWriter->String(valueName.c_str());
		p_mJsonWriter->Uint(theValue);
	}
	
}

void JsonWriter::InsertDoubleValue(const std::string valueName, const double theValue)
{
	if (valueName.length() > 0 && OkayToInsertKeyPair())
	{
		p_mJsonWriter->String(valueName.c_str());
		p_mJsonWriter->Double(theValue);
	}
	
}

void JsonWriter::InsertNullValue(const std::string valueName)
{
	if (valueName.length() > 0 && OkayToInsertKeyPair())
	{
		p_mJsonWriter->String(valueName.c_str());
		p_mJsonWriter->Null();
	}
	
}

void JsonWriter::InsertIntValue(const std::string valueName, const int theValue)
{
	if (valueName.length() > 0 && OkayToInsertKeyPair())
	{
		p_mJsonWriter->String(valueName.c_str());
		p_mJsonWriter->Int(theValue);
	}
	
}

void JsonWriter::InsertBoolValue(const bool theValue)
{
	if (OkayToInsertSingleValue())
	{
		p_mJsonWriter->Bool(theValue);
	}
}

void JsonWriter::InsertStringValue(const std::string theValue)
{
	if (OkayToInsertSingleValue())
	{
		p_mJsonWriter->String(theValue.c_str());
	}

}

void JsonWriter::InsertUnsignedIntValue(const uint32_t theValue)
{
	if (OkayToInsertSingleValue())
	{
		p_mJsonWriter->Uint(theValue);
	}
}

void JsonWriter::InsertDoubleValue(const double theValue)
{
	if (OkayToInsertSingleValue())
	{
		p_mJsonWriter->Double(theValue);
	}
}

void JsonWriter::InsertNullValue()
{
	if (OkayToInsertSingleValue())
	{
		p_mJsonWriter->Null();
	}
}

void JsonWriter::InsertIntValue(const int theValue)
{
	if (OkayToInsertSingleValue())
	{
		p_mJsonWriter->Int(theValue);
	}
}

bool JsonWriter::OkayToInsertKeyPair()
{
	bool flag = false;

	if (m_awaitingCloseObject && !m_awaitingCloseArray)
	{
		flag = true;
	}

	return flag;
}

bool JsonWriter::OkayToInsertSingleValue()
{
	bool flag = false;

	if (m_awaitingCloseObject && m_awaitingCloseArray)
	{
		flag = true;
	}

	return flag;
}
