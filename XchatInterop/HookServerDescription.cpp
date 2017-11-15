#include "StdAfx.h"
#include "HookServerDescription.h"

using namespace System;
using namespace PluginInterface;
using namespace PluginInterface::RemotingType;

namespace XChatInterop
{
	//Classe decrivante un hook de Server
	//herite de "HookDescription"

	HookServerDescription::HookServerDescription(int UniqueId, String^ Name, Hook_Server_CallBack^ CallBack, Object^ UserData, XChatPriority Priority, IntPtr HookInfoPtr)
		:HookDescription(UniqueId, UserData, HookInfoPtr)
	{
		this->name = Name;
		this->callBack = CallBack;
		this->priority = Priority;
	}

	String^ HookServerDescription::Name::get()
	{
		return this->name;
	}

	Hook_Server_CallBack^ HookServerDescription::CallBack::get()
	{
		return this->callBack;
	}

	XChatPriority HookServerDescription::Priority::get()
	{
		return this->priority;
	}
}
