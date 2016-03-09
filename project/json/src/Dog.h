#pragma once
#include "engine/json/IJerialize.h"
#include "engine/json/JsonWriter.h"

#include <iostream>

class Dog : public IJerialize
{
public:
	void StoreJSONRepresentation() 
	{
		// set file details.
		JsonWriter ourWriter("dog.json");

		ourWriter.StartObjectBlock();
			ourWriter.InsertBoolValue("IsCat", false);
		ourWriter.EndObjectBlock();

		auto shouldEndTrue = false;
		ourWriter.DumpToFile(shouldEndTrue);
		std::cout << "Dog saved: " << shouldEndTrue << std::endl;
	}
};
