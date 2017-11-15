#include "StdAfx.h"
#include "HookCommandDescription.h"

using namespace System;
using namespace PluginInterface;
using namespace PluginInterface::RemotingType;

namespace XChatInterop
{
	//Classe decrivante un hook de Command
	//herite de "HookDescription"

	HookCommandDescription::HookCommandDescription(int UniqueId, String^ Name, Hook_Command_CallBack^ CallBack, String^ HelpText, Object^ UserData, XChatPriority Priority, IntPtr HookInfoPtr)
		:HookDescription(UniqueId, UserData, HookInfoPtr)
	{
		this->name = Name;
		this->callBack = CallBack;
		this->helpText = HelpText;
		this->priority = Priority;
	}

	String^ HookCommandDescription::Name::get()
	{
		return this->name;
	}

	Hook_Command_CallBack^ HookCommandDescription::CallBack::get()
	{
		return this->callBack;
	}

	String^ HookCommandDescription::HelpText::get()
	{
		return this->helpText;
	}

	XChatPriority HookCommandDescription::Priority::get()
	{
		return this->priority;
	}
}
