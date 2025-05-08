#include "../Items/CInventoryItem.h"

void UCInventoryItem::Init(const FItemData& InItemData)
{
	ItemData = InItemData;
	Quantity = 1;
}
