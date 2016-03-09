#pragma once
#include <vector>
#include <iostream>
#include "IJerialize.h"

/**
 * Handles multiple serializable objects and calls
 * the storing functions.
 *
 * @author	Luke Sharland
 * @date	9/03/2016
 */

class BatchJSON
{
public:

	/**
	 * Batch serialize. This takes in a vector of objects that inherit from
	 * IJerialize and calls the required functions to store the JSON
	 * representation.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	itemsToSerialize	The items to serialize.
	 */

	static void BatchSerialize(std::vector<IJerialize*> itemsToSerialize);

	/**
	 * Single serialize an object inheriting IJerialize.
	 *
	 * @author	Luke Sharland
	 * @date	9/03/2016
	 *
	 * @param	itemToSerialize	The item to serialize.
	 */

	static void SingleSerialize(IJerialize *itemToSerialize);
};

