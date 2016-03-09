#include "BatchJSON.h"


void BatchJSON::BatchSerialize(std::vector<IJerialize*> itemsToSerialize)
{
	for (auto iItem = 0; iItem < itemsToSerialize.size(); iItem++)
	{
		itemsToSerialize[iItem]->StoreJSONRepresentation();
	}
}

void BatchJSON::SingleSerialize(IJerialize *itemToSerialize)
{
	itemToSerialize->StoreJSONRepresentation();
}
