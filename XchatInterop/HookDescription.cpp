#include "StdAfx.h"
#include "HookDescription.h"

namespace XChatInterop
{
	//Classe mere des différents hook :
	//Constructeur + Assesceurs aux valeurs communes
	HookDescription::HookDescription(int UniqueId, Object^ UserData, IntPtr HookInfoPtr)
	{
		this->uniqueId = UniqueId;
		this->userData = UserData;
		this->hookInfosPtr = HookInfoPtr;
	}

	int HookDescription::UniqueId::get()
	{
		return this->uniqueId;
	}

	Object^ HookDescription::UserData::get()
	{
		return this->userData;
	}

	IntPtr HookDescription::HookInfosPtr::get()
	{
		return this->hookInfosPtr;
	}

	IntPtr HookDescription::XChatHookPtr::get()
	{
		return this->_XChatHookPtr;
	}

	void HookDescription::XChatHookPtr::set(IntPtr value)
	{
		this->_XChatHookPtr = value;
	}

}
