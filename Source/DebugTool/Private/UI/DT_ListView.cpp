// FireProject by likapu company


#include "UI/DT_ListView.h"

#include "DebugTool/DT_Logger.h"

void UDT_ListView::AddItemAt(UObject* Item, int32 Index)
{
    if (Item == nullptr)
    {
        DT_ERROR_NO_LOGGER("{0}", "Cannot add null item into ListView");
        return;
    }

    if (ListItems.Contains(Item))
    {
        DT_ERROR_NO_LOGGER("{0}", "Cannot add duplicate item into ListView");
        return;
    }

    ListItems.Insert(Item, Index);

    const TArray<UObject*> Added = { Item };
    const TArray<UObject*> Removed;
    OnItemsChanged(Added, Removed);

    RequestRefresh();
}
