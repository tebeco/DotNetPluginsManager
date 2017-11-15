#pragma once
#include "HookDescription.h"

using namespace System;
using namespace PluginInterface;
using namespace PluginInterface::RemotingType;

typedef struct _HookPrintInfos
{
	int UniqueId;
	char * Name;
} HookPrintInfos;

namespace XChatInterop
{
	ref class HookPrintDescription : HookDescription
	{
	public:
		HookPrintDescription(int UniqueId, String^ Name, Hook_Print_CallBack^ CallBack, Object^ UserData, XChatPriority Priority, IntPtr HookInfoPtr);
		
		property String^ Name
		{
			String^ get();
		}
		property Hook_Print_CallBack^ CallBack
		{
			Hook_Print_CallBack^ get();
		}
		property XChatPriority Priority	
		{
			XChatPriority get();
		}

	private:
		String^ name;						//Nom du "print" qui est hooké
		Hook_Print_CallBack^ callBack;		//Delegué de la fonction de Callback
		XChatPriority priority;				//Priorité (Xchat)
	};
}
