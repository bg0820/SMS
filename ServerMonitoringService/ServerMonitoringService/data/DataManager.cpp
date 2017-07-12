#include "DataManager.hpp"

void DataManager::Update()
{
	ProcessList processList = ProcessList();
	processList.Update();
}
