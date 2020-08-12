#include "MemoryManager.h"
HANDLE MemoryManager::hProc;
MemoryManager::MemoryManager()
{
    this->hProc = NULL;
    this->pID = NULL;
    try
    {
        while (!attachProc())
        {
            attachProc();
            Sleep(100);
        }

         this->eDll = getModule(pID, "hw.dll");
		 this->cDll = getModule(pID, "client.dll");
    }
    catch (...)
    {
        MessageBoxA(NULL, "Cant access memory", 0, MB_ICONSTOP | MB_OK);
    }
}

MemoryManager::~MemoryManager()
{
    CloseHandle(this->hProc);
}
