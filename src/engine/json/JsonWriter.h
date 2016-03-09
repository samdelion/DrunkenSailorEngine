#pragma once
#include <string>
#include <fstream>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"



/**
 * A JSON writer using RapidJSON. 
 * Simplifies the JSON writing process and provides
 * a simpler interface to construct JSON strings
 * and JSON files. 
 * 
 *
 * @author	Luke Sharland
 * @date	9/03/2016
 */

class JsonWriter
{

public:

	/**
	 * Constructor. Takes in a file name/path that will later be
	 * used to dump the JSON output. If only a filename and ext,
	 * then it will be stored in the same folder as the exe.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	fileName	Filename/path of the file.
	 */

	JsonWriter(std::string fileName);

	/**
	 * Dumps the generated JSON to file, using the file name
	 * specified in the constructor.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param [in,out]	err	The error flag.
	 *
	 * @return	true if it succeeds, false if it fails.
	 */

	bool DumpToFile(bool &err);

	/**
	 * Gets the final JSON string that has been generated. This
	 * requires that the JSON object has been created and all objects
	 * and JSON arrays are closed.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param [in,out]	err	The error flag.
	 *
	 * @return	The JSON string.
	 */

	std::string GetStringRepresentation(bool &err);

	/**
	 * Starts an object block. This encapsulates arrays
	 * and individual key-pair values.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 */

	void StartObjectBlock();

	/**
	 * Starts array block. This encapsulates key-values. Will
	 * not create if blockName is empty.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	blockName	Name of the block.
	 */

	void StartArrayBlock(const std::string blockName);

	/**
	 * Ends object block. This must be called to terminate
	 * an object. Failure to terminate will result in no
	 * JSON string being generated. 
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 */

	void EndObjectBlock();

	/**
	 * Ends array block. Failure to call this will
	 * result in no JSON string being generated.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 */

	void EndArrayBlock();

	/**
	 * Inserts a bool value into an array or object. This inserts the value
	 * with a required value name as the key. The bool being the value. Will
	 * not insert if valueName is empty.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	valueName	Name of the value.
	 * @param	theValue 	the value.
	 */

	void InsertBoolValue(const std::string valueName, const bool theValue);

	/**
	 * Inserts a string value into an array or object. This inserts the value
	 * with a required value name as the key. Will
	 * not insert if valueName is empty.
	 * 
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	valueName	Name of the value.
	 * @param	theValue 	the value.
	 */

	void InsertStringValue(const std::string valueName, const std::string theValue);

	/**
	 * Inserts an unsigned int value into an array or object. This inserts the value
	 * with a required value name as the key. Will
	 * not insert if valueName is empty.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	valueName	Name of the value.
	 * @param	theValue 	the value.
	 */

	void InsertUnsignedIntValue(const std::string valueName, const uint32_t theValue);

	/**
	 * Inserts a double value into an array or object. This inserts the value
	 * with a required value name as the key. Will
	 * not insert if valueName is empty.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	valueName	Name of the value.
	 * @param	theValue 	the value.
	 */

	void InsertDoubleValue(const std::string valueName, const double theValue);

	/**
	 * Inserts a null value described by valueName. Will
	 * not insert if valueName is empty.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	valueName	Name of the value.
	 */

	void InsertNullValue(const std::string valueName);

	/**
	 * Inserts an int value into an array or object. This inserts the value
	 * with a required value name as the key. Will
	 * not insert if valueName is empty.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	valueName	Name of the value.
	 * @param	theValue 	the value.
	 */

	void InsertIntValue(const std::string valueName, const int theValue);


	/**
	 * Inserts a bool value. Requires that an array be
	 * opened beforehand.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	theValue	the value.
	 */

	void InsertBoolValue(const bool theValue);

	/**
	 * Inserts a string value. Requires that an array be
	 * opened beforehand.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	theValue	the value.
	 */

	void InsertStringValue(const std::string theValue);

	/**
	 * Inserts an unsigned int value. Requires that an array be
	 * opened beforehand.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	theValue	the value.
	 */

	void InsertUnsignedIntValue(const uint32_t theValue);

	/**
	 * Inserts a double value. Requires that an array be
	 * opened beforehand.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	theValue	the value.
	 */

	void InsertDoubleValue(const double theValue);


	/**
	 * Inserts a null value. Requires that an array be
	 * opened beforehand.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 */

	void InsertNullValue();


	/**
	 * Inserts an int value. Requires that an array be
	 * opened beforehand.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	theValue	the value.
	 */

	void InsertIntValue(const int theValue);


	/**
	 * Destructor.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 */

	~JsonWriter();

private:

	/** if objects need closing */
	bool m_awaitingCloseObject;

	/** Number of open objects. */
	int m_numOpenObjects;

	/** if arrays need closing */
	bool m_awaitingCloseArray;

	/** Number of open arrays. */
	int m_numOpenArrays;
	
	/** Full pathname of the file. */
	std::string m_filePath;

	/** Buffer for JSON string data. */
	rapidjson::StringBuffer m_jsonStringBuff;
	
	/** The JSON writer. */
	rapidjson::PrettyWriter<rapidjson::StringBuffer> * p_mJsonWriter;

	/**
	* Determines if okay to insert key pair.
	* (open object, no array open)
	*
	* @author	Luke Sharland
	* @date	9/03/2016
	*
	* @return	true if ok, false if not.
	*/

	bool OkayToInsertKeyPair();

	/**
	* Determines if okay to insert single value.
	* (open object and array)
	*
	* @author	Luke Sharland
	* @date	9/03/2016
	*
	* @return	true if ok, false if not.
	*/

	bool OkayToInsertSingleValue();

};

