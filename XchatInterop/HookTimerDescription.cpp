#include "StdAfx.h"
#include "HookTimerDescription.h"

using namespace System;
using namespace PluginInterface;
using namespace PluginInterface::RemotingType;

namespace XChatInterop
{
	//Classe decrivante un hook de Timer
	//herite de "HookDescription"

	HookTimerDescription::HookTimerDescription(int UniqueId, int UniqueIdTimer, int TimeOut, Hook_Timer_CallBack^ CallBack, Object^ UserData, IntPtr HookInfoPtr)
		:HookDescription(UniqueId, UserData, HookInfoPtr)
	{
		this->callBack = CallBack;
		this->uniqueIdTimer = UniqueIdTimer;
	}

	int HookTimerDescription::GetUniqueTimerId()
	{
		HookTimerDescription::_LastUniqueIdTimer++;
		return HookTimerDescription::_LastUniqueIdTimer - 1;
	}

	Hook_Timer_CallBack^ HookTimerDescription::CallBack::get()
	{
		return this->callBack;
	}

	int HookTimerDescription::UniqueIdTimer::get()
	{
		return this->uniqueIdTimer;
	}
}