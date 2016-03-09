#pragma once
#include "engine/json/JsonWriter.h"
#include "engine/json/BatchJSON.h"
#include "engine/json/IJerialize.h"
#include "Cat.h"
#include "Dog.h"
#include <iostream>
#include <vector>


int main(void)
{
	
	// Initial Setup, feed in the output file.
	JsonWriter ourWriter("exampleOne.json");

	/* Indenting just for example readability. First
		create an object. Consider this our parent.*/
	ourWriter.StartObjectBlock();
	
		/* Insert some key-pair values. */
		ourWriter.InsertBoolValue("ExampleBoolKey", true);
		ourWriter.InsertDoubleValue("ExampleDoubleKey", 2.33);
		ourWriter.InsertStringValue("ExampleStringKey", "All your base are");
		
		ourWriter.InsertIntValue("ExampleIntVal", -1);
		ourWriter.InsertUnsignedIntValue("ExampleUnsigned", 1);

		/* Start array. */
		ourWriter.StartArrayBlock("ExampleArray");
		
			/* Insert single values, no key-pair */
			ourWriter.InsertBoolValue(false);
			ourWriter.InsertBoolValue(true);
			ourWriter.InsertBoolValue(false);
			
		/* Make sure array is closed. */
		ourWriter.EndArrayBlock();

		/* This demonstrates that nothing will be generated
			if both arrays and objects are not closed first. */
		auto flag = true;
		ourWriter.DumpToFile(flag); 
		std::cout << "This should be false. Object not closed!! Result: " << flag << std::endl;
		std::cout << "made it here" << std::endl;
		
	/* Now properly close the object. */
	ourWriter.EndObjectBlock();

	
	/* Now let's properly dump the JSON this time. */
	auto shouldEndTrue = false;

	ourWriter.DumpToFile(shouldEndTrue);
	std::cout << "This should be true. Object closed Result: " << shouldEndTrue << std::endl;
	std::cout << "End Result: " << std::endl << ourWriter.GetStringRepresentation(shouldEndTrue) << std::endl;
	std::cout << "Finished? " << shouldEndTrue << std::endl;


	// ---- OPTIONAL ----- //
	
	
	/* Both implement IJerialize save functions.*/
	Cat aCat;
	Dog aDog;
	
	IJerialize *meow = &aCat;
	IJerialize *woof = &aDog;
	
	std::vector<IJerialize*> animals;
	animals.push_back(meow);
	animals.push_back(woof);

	// Save all.
	BatchJSON::BatchSerialize(animals);

}