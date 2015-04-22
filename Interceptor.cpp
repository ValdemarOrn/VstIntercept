#include <windows.h>
#include <stdio.h>
#include <string>
#include "public.sdk/source/vst2.x/audioeffectx.h"

typedef AEffect* (*PluginEntryProc) (audioMasterCallback audioMaster);
void* hInstance;

HINSTANCE GetMyModuleHandle()
{
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery(GetMyModuleHandle, &mbi, sizeof(mbi));
	return (HINSTANCE)(mbi.AllocationBase);
}

struct PluginLoader
{
	void* module;

	PluginLoader() : module(0)
	{
	}

	~PluginLoader()
	{
		if (module)
		{
			// Sloppy, no cleanup after loading
			//FreeLibrary((HMODULE)module);
		}
	}

	bool loadLibrary(const char* fileName)
	{
		module = LoadLibrary(fileName);
		return module != 0;
	}

	PluginEntryProc getMainEntry()
	{
		PluginEntryProc mainProc = 0;
		mainProc = (PluginEntryProc)GetProcAddress((HMODULE)module, "VSTPluginMain");
		if (!mainProc)
			mainProc = (PluginEntryProc)GetProcAddress((HMODULE)module, "main");
		return mainProc;
	}
};

//-------------------------------------------------------------------------------------------------------
extern "C"
{
	__declspec(dllexport) AEffect* VSTPluginMain(audioMasterCallback audioMaster)
	{
		char dllName[512];
		char underlyingDllName[512];
		char vstIdIntString[16];

		HINSTANCE thismodule = GetMyModuleHandle();
		GetModuleFileName(thismodule, dllName, 512);

		int pluginNameLen = strlen(dllName) - 24;
		
		// determine underlying dll name
		strncpy(underlyingDllName, dllName, pluginNameLen);
		strcpy(underlyingDllName + pluginNameLen, "dll");
		const char* fullFilePath = underlyingDllName;

		// determine the vstid from the name
		strncpy(vstIdIntString, dllName + pluginNameLen + 10, 10);
		vstIdIntString[10] = 0;
		const int vstId = atoi(vstIdIntString);

		PluginLoader loader;
		if (!loader.loadLibrary(fullFilePath))
		{
			printf("Failed to load VST Plugin library!\n");
			return 0;
		}

		PluginEntryProc mainEntry = loader.getMainEntry();
		if (!mainEntry)
		{
			printf("VST Plugin main entry not found!\n");
			return 0;
		}

		printf("Create effect...\n");
		AEffect* effect = mainEntry(audioMaster);
		if (!effect)
		{
			printf("Failed to create effect instance!\n");
			return 0;
		}

		unsigned int uid = effect->uniqueID;
		effect->uniqueID = vstId;
		return effect;
	}

}

extern "C" 
{
	BOOL WINAPI DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved)
	{
		hInstance = hInst;
		return 1;
	}
}
