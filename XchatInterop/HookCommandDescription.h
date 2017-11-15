#pragma once
#include "HookDescription.h"

using namespace System;
using namespace PluginInterface;
using namespace PluginInterface::RemotingType;

//Structure natif Permettant de faire le lien entre le hook de xchat et celui appellé par le code managé
//via des identifiants uniques entre eux
typedef struct _HookCommandInfos
{
	int UniqueId;	//ID unique de la classe de remoting (donc du plugin)
	char * Name;	//Nom de la commande hooké
} HookCommandInfos;

namespace XChatInterop
{
	ref class HookCommandDescription : HookDescription
	{
	public:
		HookCommandDescription(int UniqueId, String^ Name, Hook_Command_CallBack^ CallBack, String^ HelpText, Object^ UserData, XChatPriority Priority, IntPtr HookInfoPtr);

		property String^ Name
		{
			String^ get();
		}
		property Hook_Command_CallBack^ CallBack
		{
			Hook_Command_CallBack^ get();
		}
		property String^ HelpText
		{
			String^ get();
		}
		property XChatPriority Priority	
		{
			XChatPriority get();
		}
	private:
		String^ name;						//Nom de la commande hookée
		Hook_Command_CallBack^ callBack;	//Delegué pointant sur la fonction de callback
		String^ helpText;					//Texte d'aide pour la commande
		XChatPriority priority;				//Priorité du hook
	};
}
