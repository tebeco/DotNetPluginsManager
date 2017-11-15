#pragma once

using namespace System;

namespace XChatInterop
{
	ref class HookDescription
	{
	public:
		HookDescription(int UniqueId, Object^ UserData, IntPtr HookInfosPtr);

		property int UniqueId
		{
			int get();
		}
		property Object^ UserData
		{
			Object^ get();
		}

		property IntPtr HookInfosPtr	
		{
			IntPtr get();
		}
		property IntPtr XChatHookPtr
		{
			IntPtr get();
			void set(IntPtr value);
		}

	private:
		//Valeur commune a tous les hook
		int uniqueId;						//UniqueId du plugin auquel le hook appartient
		Object^ userData;					//Object passé au callback Managé
		IntPtr hookInfosPtr;				//Object passé au callback natif
		IntPtr _XChatHookPtr;				//Pointeur du hook reel de Xchat
	};
}
