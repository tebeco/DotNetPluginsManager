#pragma once
#include "HookDescription.h"

using namespace System;
using namespace PluginInterface;
using namespace PluginInterface::RemotingType;

typedef struct _HookTimerInfos
{
	int UniqueId;
	int UniqueIdTimer;
} HookTimerInfos;

namespace XChatInterop
{
	ref class HookTimerDescription : HookDescription
	{
	public:
		HookTimerDescription(int UniqueId, int UniqueIdTimer, int TimeOut, Hook_Timer_CallBack^ CallBack, Object^ UserData, IntPtr HookInfoPtr);

		property Hook_Timer_CallBack^ CallBack
		{
			Hook_Timer_CallBack^ get();
		}
		property int UniqueIdTimer
		{
			int get();
		}
	internal:
		static int GetUniqueTimerId();
	private:
		Hook_Timer_CallBack^ callBack;		//Délégué de la fonction de CallBack
		int uniqueIdTimer;					//Identifiant du timer (permet de differencié les timers les un des autre)

		static int _LastUniqueIdTimer = 0;	//Compteur pour les ID des timer
	};
}
