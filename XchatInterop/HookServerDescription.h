#pragma once
#include "HookDescription.h"

using namespace System;
using namespace PluginInterface;
using namespace PluginInterface::RemotingType;

typedef struct _HookServerInfos
{
	int UniqueId;
	char * Name;
} HookServerInfos;

namespace XChatInterop
{
	ref class HookServerDescription : HookDescription
	{
	public:
		HookServerDescription(int UniqueId, String^ Name, Hook_Server_CallBack^ CallBack, Object^ UserData, XChatPriority Priority, IntPtr HookInfoPtr);

		property String^ Name
		{
			String^ get();
		}
		property Hook_Server_CallBack^ CallBack
		{
			Hook_Server_CallBack^ get();
		}
		property XChatPriority Priority	
		{
			XChatPriority get();
		}
	private:
		String^ name;						//Nom de l'evenement server hooké
		Hook_Server_CallBack^ callBack;		//Délégué de la fonction de CallBack
		XChatPriority priority;				//Priorité
	};
}
