#include "StdAfx.h"
#include "HookPrintDescription.h"

using namespace System;
using namespace PluginInterface;
using namespace PluginInterface::RemotingType;

namespace XChatInterop
{
	//Classe decrivante un hook de Print
	//herite de "HookDescription"

	HookPrintDescription::HookPrintDescription(int UniqueId, String^ Name, Hook_Print_CallBack^ CallBack, Object^ UserData, XChatPriority Priority, IntPtr HookInfoPtr)
		:HookDescription(UniqueId, UserData, HookInfoPtr)
	{
		this->name = Name;
		this->callBack = CallBack;
		this->priority = Priority;
	}

	String^ HookPrintDescription::Name::get()
	{
		return this->name;
	}

	Hook_Print_CallBack^ HookPrintDescription::CallBack::get()
	{
		return this->callBack;
	}

	XChatPriority HookPrintDescription::Priority::get()
	{
		return this->priority;
	}

}
