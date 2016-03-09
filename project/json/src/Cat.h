#pragma once
#include "engine/json/IJerialize.h"
#include "engine/json/JsonWriter.h"

#include <iostream>

class Cat : public IJerialize
{
public:
	void StoreJSONRepresentation() 
	{
		// set file details.
		JsonWriter ourWriter("cat.json");
		ourWriter.StartObjectBlock();
			ourWriter.InsertBoolValue("IsCat", true);
		ourWriter.EndObjectBlock();

		auto shouldEndTrue = false;
		ourWriter.DumpToFile(shouldEndTrue);
		std::cout << "Cat saved: " << shouldEndTrue << std::endl;
	}
};
