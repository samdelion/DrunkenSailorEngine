#pragma once
#include "JsonWriter.h"

class IJerialize
{
public:
	virtual void StoreJSONRepresentation() { };
	virtual void LoadJSONRepresentation() { };
};

