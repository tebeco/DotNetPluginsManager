#include "StdAfx.h"
#include "XChatInterop.h"
#include "RemoteOperations.h"
#include "HookCommandDescription.h"
#include "HookServerDescription.h"
#include "HookPrintDescription.h"
#include "HookTimerDescription.h"
#include "xchat-plugin.h"
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace PluginInterface;
using namespace PluginInterface::RemotingType;

namespace XChatInterop
{
#pragma managed(push, off)
	//Fonctions natives avec de l'asm inline mettant sur la pile les elements un a un du tableau de chaine passer en parametre
	//Cela afin de pouvoir appelé les fonction correspondante gerant "n" parametre et "n" etant inconue a la compilation
	void Mon_XChat_Commandf(char * Format, long count,char ** arg, void (* Commandf) (xchat_plugin *phTmp, const char *format, ...))
	{
		__asm
		{
			PUSH EAX
			PUSH ECX
			PUSH EDX
			MOV ECX, count
			MOV EAX, ECX
			SHL EAX, 2
			MOV ESI, EAX
			ADD ESI, arg
			debut:
			SUB ESI, 4
			PUSH [ESI]
			DEC ECX
			JNZ debut
		}
		(*Commandf)(GetPh(), Format);
		__asm
		{
			MOV EAX, count
			SHL EAX, 2
			ADD ESP, EAX
			POP EDX
			POP ECX
			POP EAX
		}
	}

	int Mon_XChat_Emit_Print(char * Event_Name, long count,char ** arg, int (* EmitPrint) (xchat_plugin *phTmp, const char *event_name, ...))
	{
		int res;
		__asm
		{
			PUSH EAX
			PUSH ECX
			PUSH EDX
			MOV ECX, count
			MOV EAX, ECX
			SHL EAX, 2
			MOV ESI, EAX
			ADD ESI, arg
			debut:
			SUB ESI, 4
			PUSH [ESI]
			DEC ECX
			JNZ debut
		}
		res = (*EmitPrint)(GetPh(), Event_Name);
		__asm
		{
			MOV EAX, count
			SHL EAX, 2
			ADD ESP, EAX
			POP EDX
			POP ECX
			POP EAX
		}
		return res;
	}

	void Mon_XChat_Printf(char * Format, long count,char ** arg, void (* XChatPrintf) (xchat_plugin *phTmp, const char *format, ...))
	{
		__asm
		{
			PUSH EAX
			PUSH ECX
			PUSH EDX
			MOV ECX, count
			MOV EAX, ECX
			SHL EAX, 2
			MOV ESI, EAX
			ADD ESI, arg
			debut:
			SUB ESI, 4
			PUSH [ESI]
			DEC ECX
			JNZ debut
		}
		(*XChatPrintf)(GetPh(), Format);
		__asm
		{
			MOV EAX, count
			SHL EAX, 2
			ADD ESP, EAX
			POP EDX
			POP ECX
			POP EAX
		}
	}

#pragma managed(pop)

	int Hook_Commands_CallBack(char *word[], char *word_eol[], void *userdata)
	{
		//CallBack des Hook de type "Command"

		//Recupere la structure native d'identification
		HookCommandInfos hookInfos = *((HookCommandInfos*)userdata);
		HookCommandDescription^ tmpHookCommand;
		bool bFound = false;
		//Parmis les element de la liste des hook de type "Command"
		for(int nIndex = 0; nIndex < RemoteOperations::lstHookCommandDescription->Count; nIndex++)
		{
			tmpHookCommand = RemoteOperations::lstHookCommandDescription[nIndex];
			String^ name = gcnew String((const char*)hookInfos.Name);
			//Cherche si il existe une correspondance
			if((tmpHookCommand->UniqueId == hookInfos.UniqueId) && (tmpHookCommand->Name == name))
			{
				bFound = true;
				break;
			}
		}

		if(bFound)
		{
			//Si oui on convetir les tableau de chaine natifs en managé
			array<String^>^ Word = RemoteOperations::Convert(word);
			array<String^>^ Word_Eol = RemoteOperations::Convert(word_eol);

			//Et on invoke le délégué de callback avec ainsi qu'avec le UserData spécifié
			XChatEat res = tmpHookCommand->CallBack->Invoke(Word, Word_Eol, tmpHookCommand->UserData);

			//Et on renvoi la valeur renvoyé par le delegué
			return (int)res;
		}
		else
		{
			//Si non on renvoi "none"
			return (int)XChatEat::None;
		}
	}
	int Hook_Timers_CallBack(void *userdata)
	{
		//CallBack des Hook de type "Timer"
		//Cf "Hook_Commands_CallBack" pour le detail du code

		HookTimerInfos hookInfos = *((HookTimerInfos*)userdata);
		HookTimerDescription^ tmpHookTimer;
		bool bFound = false;
		for(int nIndex = 0; nIndex < RemoteOperations::lstHookTimerDescription->Count; nIndex++)
		{
			tmpHookTimer = RemoteOperations::lstHookTimerDescription[nIndex];
			if((tmpHookTimer->UniqueId == hookInfos.UniqueId) && (tmpHookTimer->UniqueIdTimer == hookInfos.UniqueIdTimer))
			{
				bFound = true;
				break;
			}
		}

		if(bFound)
		{
			XChatTimer res = tmpHookTimer->CallBack->Invoke(tmpHookTimer->UserData);
			return (int)res;
		}
		else
		{
			return (int)XChatTimer::Stop;
		}
	}
	int Hook_Servers_CallBack(char *word[], char *word_eol[], void *userdata)
	{
		//CallBack des Hook de type "Server"
		//Cf "Hook_Commands_CallBack" pour le detail du code

		HookServerInfos hookInfos = *((HookServerInfos*)userdata);
		HookServerDescription^ tmpHookServer;
		bool bFound = false;
		for(int nIndex = 0; nIndex < RemoteOperations::lstHookServerDescription->Count; nIndex++)
		{
			tmpHookServer = RemoteOperations::lstHookServerDescription[nIndex];
			String^ name = gcnew String((const char*)hookInfos.Name);
			if((tmpHookServer->UniqueId == hookInfos.UniqueId) && (tmpHookServer->Name == name))
			{
				bFound = true;
				break;
			}
		}

		if(bFound)
		{
			array<String^>^ Word = RemoteOperations::Convert(word);
			array<String^>^ Word_Eol = RemoteOperations::Convert(word_eol);
			XChatEat res = tmpHookServer->CallBack->Invoke(Word, Word_Eol, tmpHookServer->UserData);
			return (int)res;
		}
		else
		{
			return (int)XChatEat::None;
		}
	}
	int Hook_Prints_CallBack(char *word[], void *userdata)
	{
		//CallBack des Hook de type "Print"
		//Cf "Hook_Commands_CallBack" pour le detail du code


		HookPrintInfos hookInfos = *((HookPrintInfos*)userdata);
		HookPrintDescription^ tmpHookPrint;
		bool bFound = false;
		for(int nIndex = 0; nIndex < RemoteOperations::lstHookPrintDescription->Count; nIndex++)
		{
			tmpHookPrint = RemoteOperations::lstHookPrintDescription[nIndex];
			String^ name = gcnew String((const char*)hookInfos.Name);
			if((tmpHookPrint->UniqueId == hookInfos.UniqueId) && (tmpHookPrint->Name == name))
			{
				bFound = true;
				break;
			}
		}

		if(bFound)
		{
			array<String^>^ Word = RemoteOperations::Convert(word);
			XChatEat res = tmpHookPrint->CallBack->Invoke(Word, tmpHookPrint->UserData);
			return (int)res;
		}
		else
		{
			return (int)XChatEat::None;
		}
	}


	
	RemoteOperations::RemoteOperations()
	{
		//Recupère la valeur du plugin handle il est NECESSAIRE que cette variable s'apelle "ph"
		ph = GetPh();
		//Genere un nouvelle ID
		this->UniqueId = RemoteOperations::LastUniqueId;
		//Et incrémente le compteur
		LastUniqueId++;
	}
	
	IntPtr RemoteOperations::XChat_Hook_Command(String^ Name, XChatPriority Priority, Hook_Command_CallBack^ CallBack, String^ HelpText, Object^ UserData)
	{
		//Hook un evenemant de type "Command"
		//Cf "XChat_Hook_Timer" pour le detail du code

		char * tmpName = (char *)Marshal::StringToHGlobalAnsi(Name).ToPointer();
		char * tmpHelpText = (char *)Marshal::StringToHGlobalAnsi(HelpText).ToPointer();
		HookCommandInfos * hookInfos = new HookCommandInfos();//(HookCommandInfos *)malloc(sizeof(int));
		hookInfos->UniqueId = this->UniqueId;
		hookInfos->Name = strdup(tmpName);

		HookCommandDescription^ HookCommand = gcnew HookCommandDescription(this->UniqueId, Name, CallBack, HelpText, UserData, Priority, (IntPtr)hookInfos);

		xchat_hook * res = xchat_hook_command(ph, (const char *)tmpName, (int)Priority, Hook_Commands_CallBack, (const char*)tmpHelpText, hookInfos);

		HookCommand->XChatHookPtr = (IntPtr)res;
		lstHookCommandDescription->Add(HookCommand);

		return ((IntPtr)res);
	}

	IntPtr RemoteOperations::XChat_Hook_Server(String^ Name, XChatPriority Priority, Hook_Server_CallBack^ CallBack, Object^ UserData)
	{
		//Hook un evenemant de type "Server"
		//Cf "XChat_Hook_Timer" pour le detail du code

		char * tmpName = (char *)Marshal::StringToHGlobalAnsi(Name).ToPointer();
		HookServerInfos * hookInfos = new HookServerInfos();
		hookInfos->UniqueId = this->UniqueId;
		hookInfos->Name = strdup(tmpName);

		HookServerDescription^ HookServer = gcnew HookServerDescription(this->UniqueId, Name, CallBack, UserData, Priority, (IntPtr)hookInfos);

		xchat_hook * res = xchat_hook_server(ph, (const char *)tmpName, (int)Priority, Hook_Servers_CallBack, hookInfos);

		HookServer->XChatHookPtr = (IntPtr)res;
		lstHookServerDescription->Add(HookServer);

		return ((IntPtr)res);
	}
	IntPtr RemoteOperations::XChat_Hook_Print(String^ Name, XChatPriority Priority, Hook_Print_CallBack^ CallBack, Object^ UserData)
	{
		//Hook un evenemant de type "Print"
		//Cf "XChat_Hook_Timer" pour le detail du code

		char * tmpName = (char *)Marshal::StringToHGlobalAnsi(Name).ToPointer();
		HookPrintInfos * hookInfos = new HookPrintInfos ();
		hookInfos->UniqueId = this->UniqueId;
		hookInfos->Name = strdup(tmpName);

		HookPrintDescription^ HookPrint = gcnew HookPrintDescription(this->UniqueId, Name, CallBack, UserData, Priority, (IntPtr)hookInfos);

		xchat_hook * res = xchat_hook_print(ph, (const char *)tmpName, (int)Priority, Hook_Prints_CallBack, hookInfos);

		HookPrint->XChatHookPtr = (IntPtr)res;
		lstHookPrintDescription->Add(HookPrint);

		return ((IntPtr)res);
	}
	IntPtr RemoteOperations::XChat_Hook_Timer(int TimeOut, Hook_Timer_CallBack^ CallBack, Object^ UserData)
	{
		//Permet de créé un hook sur un timer en specifiant
		//l'intervale, le delegué et callback et une valeur qqonque utile a l'appelant

		//Créé une structure permettant d'identifier de maniere unique ce hook parmis les autre
		HookTimerInfos * hookInfos = new HookTimerInfos ();
		hookInfos->UniqueId = this->UniqueId;	//A partir de l'Id du plugin
		hookInfos->UniqueIdTimer = HookTimerDescription::GetUniqueTimerId();	//Et de l'Id du timer

		//Créé une nouvelle instance de la classe "HookTimerDescription" qui sera listée pour le callback et le unhook
		HookTimerDescription^ HookTimer = gcnew HookTimerDescription(this->UniqueId, hookInfos->UniqueIdTimer, TimeOut, CallBack, UserData, (IntPtr)hookInfos);

		//Lance le hook en specifiant en userdata le fameuse structure unique
		xchat_hook * res = xchat_hook_timer(ph, TimeOut, Hook_Timers_CallBack, hookInfos);

		//Remplis la classe de description avec le hook renvoyé par Xchat
		//cela servira lors du UnHook
		HookTimer->XChatHookPtr = (IntPtr)res;
		//Rajoute le hook a la liste
		lstHookTimerDescription->Add(HookTimer);

		//Renvoi le pointeur sur le hook
		return ((IntPtr)res);
	}
	
	Object^ RemoteOperations::XChat_Unhook(IntPtr Hook, XChatHook HookType)
	{
		//Retire le hook passer en pointeur en fonction de son type de hook

		HookDescription^ tmpHook;
		List<HookDescription^>^ lstHook;
		bool bFound = false;
		Object^ res;

		//Recherche le hook dans la liste correspondante a son type
		switch(HookType)
		{
		case XChatHook::Command:
			for(int nIndex = 0; nIndex < lstHookCommandDescription->Count; nIndex++)
			{
				tmpHook = lstHookCommandDescription[nIndex];
				//Si le hook en cour correspond au pointeur passer alors on stop la boucle
				if(Hook.ToInt64() == tmpHook->XChatHookPtr.ToInt64())
				{
					bFound = true;
					break;
				}
			}

			break;
		case XChatHook::Fd:
			//non gerer pour le moment
			break;
		case XChatHook::Print:
			for(int nIndex = 0; nIndex < lstHookPrintDescription->Count; nIndex++)
			{
				tmpHook = lstHookPrintDescription[nIndex];
				if(Hook.ToInt64() == tmpHook->XChatHookPtr.ToInt64())
				{
					bFound = true;
					break;
				}
			}

			break;
		case XChatHook::Server:
			for(int nIndex = 0; nIndex < lstHookServerDescription->Count; nIndex++)
			{
				tmpHook = lstHookServerDescription[nIndex];
				if(Hook.ToInt64() == tmpHook->XChatHookPtr.ToInt64())
				{
					bFound = true;
					break;
				}
			}

			break;
		case XChatHook::Timer:
			for(int nIndex = 0; nIndex < lstHookTimerDescription->Count; nIndex++)
			{
				tmpHook = lstHookTimerDescription[nIndex];
				if(Hook.ToInt64() == tmpHook->XChatHookPtr.ToInt64())
				{
					bFound = true;
					break;
				}
			}

			break;
		}
		
		if(bFound)
		{
			//Si celui ci a été trouve on recupere le "UserData" et on dehook
			res = tmpHook->UserData;
			xchat_unhook(ph, (xchat_hook*)(void*) Hook);
			
			IntPtr xchatHook = tmpHook->HookInfosPtr;

			//Puis on le retire de la liste correspondante
			HookCommandInfos * hookCommandInfos = nullptr;
			HookPrintInfos * hookPrintInfos = nullptr;
			HookServerInfos * hookServerInfos = nullptr;
			HookTimerInfos * hookTimerInfos = nullptr;
			switch(HookType)
			{
			case XChatHook::Command:
				lstHookCommandDescription->Remove((HookCommandDescription^)tmpHook);
				hookCommandInfos = ((HookCommandInfos *)(void *)xchatHook);
				delete hookCommandInfos;
				break;
			case XChatHook::Fd:
				//non gerer pour le moment
				break;
			case XChatHook::Print:
				lstHookPrintDescription->Remove((HookPrintDescription^)tmpHook);
				hookPrintInfos = ((HookPrintInfos *)(void *)xchatHook);
				delete hookPrintInfos;
				break;
			case XChatHook::Server:
				lstHookServerDescription->Remove((HookServerDescription^)tmpHook);
				hookServerInfos = ((HookServerInfos *)(void *)xchatHook);
				delete hookServerInfos;
				break;
			case XChatHook::Timer:
				lstHookTimerDescription->Remove((HookTimerDescription^)tmpHook);
				hookTimerInfos = ((HookTimerInfos *)(void *)xchatHook);
				delete hookTimerInfos;
				break;
			}
			//On retourne enfin le "UserData"
			return res;
		}
		else
		{
			//Si il n'a pas été trouver on retourne "null"
			return nullptr;
		}
	}
	void RemoteOperations::UnHookAll()
	{
		//Supprime tous les hook en cour a partir de chaqunes des liste en bouclant sur element une par une
		HookDescription^ tmpHook;
		int nIndex;
		for(nIndex = lstHookCommandDescription->Count; nIndex > 0; nIndex--)
		{
			tmpHook = lstHookCommandDescription[nIndex-1];
			//On s'assure tout de meme qu'il s'agis d'un hook du plugin en cour vu que les listes sont static
			if(tmpHook->UniqueId == this->UniqueId)
			{
				//Retire le hook depuis son pointeur
				xchat_unhook(ph, (xchat_hook*)(void*) tmpHook->XChatHookPtr);
				//puis on les supprime de la liste en question
				lstHookCommandDescription->Remove((HookCommandDescription^)tmpHook);
			}
		}

		for(nIndex = lstHookPrintDescription->Count; nIndex > 0; nIndex--)
		{
			tmpHook = lstHookPrintDescription[nIndex-1];
			if(tmpHook->UniqueId == this->UniqueId)
			{
				xchat_unhook(ph, (xchat_hook*)(void*) tmpHook->XChatHookPtr);
				lstHookPrintDescription->Remove((HookPrintDescription^)tmpHook);
			}
		}

		for(nIndex = lstHookServerDescription->Count; nIndex > 0; nIndex--)
		{
			tmpHook = lstHookServerDescription[nIndex-1];
			if(tmpHook->UniqueId == this->UniqueId)
			{
				xchat_unhook(ph, (xchat_hook*)(void*) tmpHook->XChatHookPtr);
				lstHookServerDescription->Remove((HookServerDescription^)tmpHook);
			}
		}

		for(nIndex = lstHookTimerDescription->Count; nIndex > 0; nIndex--)
		{
			tmpHook = lstHookTimerDescription[nIndex-1];
			if(tmpHook->UniqueId == this->UniqueId)
			{
				xchat_unhook(ph, (xchat_hook*)(void*) tmpHook->XChatHookPtr);
				lstHookTimerDescription->Remove((HookTimerDescription^)tmpHook);
			}
		}
	}
	
	void RemoteOperations::XChat_Command(String^ Command)
	{
		//Execute une commande irc
		//Exemple : "PRIVMSG #a toto"
		//cela va envoyer le message "toto" sur le cannal "#a"
		char * tmpCommand = (char*)Marshal::StringToHGlobalAnsi(Command).ToPointer();
		xchat_command(ph, tmpCommand);
		Marshal::FreeHGlobal((IntPtr)tmpCommand);
	}

	void RemoteOperations::XChat_Commandf(String^ Format,... array<String^>^ Args)
	{
		//Meme chose que "XChat_Command" mais avec la prise en charge du formattage
		char * tmpFormat = (char*)Marshal::StringToHGlobalAnsi(Format).ToPointer();
		char ** tmpArgs = new char *[Args->GetLength(0)];
		for(int nIndex=0; nIndex < Args->GetLength(0); nIndex++)
		{
			tmpArgs[nIndex] = (char*)Marshal::StringToHGlobalAnsi(Args[nIndex]).ToPointer();
		}

		//Appelle une fonction native avec de l'asm inline qui va manipuler la pile avant d'appeler la fonction de xchat
		Mon_XChat_Commandf(tmpFormat, Args->GetLength(0), tmpArgs, xchat_commandf);

		//Libère les ressource
		for(int nIndex = 0; nIndex < Args->GetLength(0); nIndex++)
		{
			Marshal::FreeHGlobal((IntPtr)((char*)(*tmpArgs[nIndex])));
		}
		delete[] tmpArgs;
		Marshal::FreeHGlobal((IntPtr)tmpFormat);
	}
	void RemoteOperations::XChat_Print(String^ Text)
	{
		//Ecrit une chaine sur la fenetre correspondant au context comme la commande "Echo"

		char * tmpText = (char*)Marshal::StringToHGlobalAnsi(Text).ToPointer();
		xchat_print(ph, tmpText);
		Marshal::FreeHGlobal((IntPtr)tmpText);
	}
	int RemoteOperations::XChat_Emit_Print(String^ Event_Name,... array<String^>^ Args)
	{
		//Simule la generation d'un evenement de type "Print" a partir de son nom et des arguments

		char * tmpEventName = (char*)Marshal::StringToHGlobalAnsi(Event_Name).ToPointer();
		char ** tmpArgs = new char *[Args->GetLength(0)];
		for(int nIndex=0; nIndex < Args->GetLength(0); nIndex++)
		{
			tmpArgs[nIndex] = (char*)Marshal::StringToHGlobalAnsi(Args[nIndex]).ToPointer();
		}

		//Appelle une fonction native avec de l'asm inline qui va manipuler la pile avant d'appeler la fonction de xchat
		int res = Mon_XChat_Emit_Print(tmpEventName, Args->GetLength(0), tmpArgs, xchat_emit_print);

		//Libère les ressource
		for(int nIndex = 0; nIndex < Args->GetLength(0) - 1; nIndex++)
		{
			Marshal::FreeHGlobal((IntPtr)((char*)(*tmpArgs[nIndex])));
		}
		delete[] tmpArgs;
		Marshal::FreeHGlobal((IntPtr)tmpEventName);

		//Renvoi le resultat
		return res;
	}

	void RemoteOperations::XChat_Printf(String^ Format, ... array<String^>^ Args)
	{
		//Meme chose que le "Print" mais avec la prise en charge du format 

		//Chaine contenant le format
		char * tmpFormat = (char*)Marshal::StringToHGlobalAnsi(Format).ToPointer();
		//Tableau de chaine correspondant au paramètre
		char ** tmpArgs = new char *[Args->GetLength(0)];
		for(int nIndex=0; nIndex < Args->GetLength(0); nIndex++)
		{
			tmpArgs[nIndex] = (char*)Marshal::StringToHGlobalAnsi(Args[nIndex]).ToPointer();
		}

		//Appelle une fonction native avec de l'asm inline qui va manipuler la pile avant d'appeler la fonction de xchat
		Mon_XChat_Printf(tmpFormat, Args->GetLength(0), tmpArgs, xchat_printf);

		//Libère les ressource
		for(int nIndex = 0; nIndex < Args->GetLength(0); nIndex++)
		{
			Marshal::FreeHGlobal((IntPtr)((char*)(*tmpArgs[nIndex])));
		}
		delete[] tmpArgs;
		Marshal::FreeHGlobal((IntPtr)tmpFormat);
	}
	void RemoteOperations::XChat_Send_Modes (array<String^>^ Targets, int NTargets, int Modes_Per_Line, Char Sign, Char Mode)
	{
		//Envoi au cannal courant une commande d'execution de changement de modes:
		//Target : liste des pseudo
		//NTargets : nombre de target
		//Modes_Per_Line : nombre maximum de mode par ligne
		//Sign : '+' ou '-' (pour activer / desactiver un mode)
		//Mode : le mode en question ('o' pour Op, 'v' pour voice ect ...)

		const char ** tbTargets = new const char *[NTargets];
		for(int nIndex = 0; nIndex < NTargets; nIndex++)
		{
			tbTargets[nIndex] = (char*)Marshal::StringToHGlobalAnsi(Targets[nIndex]).ToPointer();
		}

		//Execute la commande
		xchat_send_modes(ph, tbTargets, NTargets, Modes_Per_Line, Sign, Mode);

		//Libère les ressources du tableau case par case
		for(int nIndex = 0; nIndex < NTargets; nIndex++)
		{
			Marshal::FreeHGlobal((IntPtr)((char*)(*tbTargets[nIndex])));
		}
		//detruit le tableau
		delete[] tbTargets;
	}
	
	IntPtr RemoteOperations::XChat_Find_Context(String^ Servname, String^ Channel)
	{
		//Récupere un pointeur sur un contexte depuis le nom du serveur et le nom du cannal specifié

		//Convertis en natif les noms du serveur/cannal
		char * tmpServname = (char*)Marshal::StringToHGlobalAnsi(Servname).ToPointer();
		char * tmpChannel = (char*)Marshal::StringToHGlobalAnsi(Channel).ToPointer();

		//Recupere le pointeur vers le contexte ou "null" si introuvé
		IntPtr res = (IntPtr) xchat_find_context(ph, tmpServname, tmpChannel);

		//Libère les ressources temporaires
		Marshal::FreeHGlobal((IntPtr)tmpServname);
		Marshal::FreeHGlobal((IntPtr)tmpChannel);

		//Renvois le pointeur
		return res;
	}
	IntPtr RemoteOperations::XChat_Get_Context()
	{
		//Retourne le pointeur du context courant
		return (IntPtr) xchat_get_context(ph);
	}
	String^ RemoteOperations::XChat_Get_Info(String^ Id)
	{
		//Retourne des information concernant le contexte courrant a partir d'un "String^"

		String^ res;
		char * tmpId = (char*)Marshal::StringToHGlobalAnsi(Id).ToPointer();

		//Cette chaine ne doit pas etre "freed"
		char * xchat_res = (char *)xchat_get_info(ph, tmpId);
		//Convertis la chaine en managé
		res = gcnew String(xchat_res);
		
		Marshal::FreeHGlobal((IntPtr)tmpId);
		
		//Renvoi la valeur
		return res;
	}
	int RemoteOperations::XChat_Get_Prefs(String^ Name, String^% string, int% Integer)
	{
		//Permet de récupérer la valeur d'une varaible interne a Xchat
		//La liste est visible en tappant "/set" dans une fenetre

		char * tmpName = (char*)Marshal::StringToHGlobalAnsi(Name).ToPointer();
		const char *tbString;
		int i;

		//Recupère "res" a partir de la fonction, cela permet de savoir si ca a echoué,
		//et si ca reussi si il s'agit d'un int, boolean ou String
		int res = xchat_get_prefs(ph, tmpName, &tbString, &i);
		//Remplis la valeur de la chaine passé par reference (au cas ou il s'agisse d'une chaine)
		string = gcnew String((const char*)((void *)tbString));
		//Remplis la valeur de l'int passé par reference (au cas ou il s'agisse d'un int ou boolean)
		Integer = i;

		Marshal::FreeHGlobal((IntPtr)tmpName);

		return res;
	}
	int RemoteOperations::XChat_Set_Context(IntPtr Ctx)
	{
		//Force le context courant a partir du pointeur donné
		return xchat_set_context(ph, (xchat_context* )(void*)Ctx);
	}
	
	int RemoteOperations::XChat_Nickcmp(String^ s1, String^ s2)
	{
		//Effectue une comparaison de 2 chaine comme si il s'agissait de 2 pseudo
		//Algo interne a XCaht pour le "classement"

		char * tmpS1 = (char*)Marshal::StringToHGlobalAnsi(s1).ToPointer();
		char * tmpS2 = (char*)Marshal::StringToHGlobalAnsi(s2).ToPointer();

		int res = xchat_nickcmp(ph, tmpS1, tmpS2);

		Marshal::FreeHGlobal((IntPtr)tmpS1);
		Marshal::FreeHGlobal((IntPtr)tmpS2);

		return res;
	}
	String^ RemoteOperations::XChat_Strip(String^ Str, int Len, int Flags)
	{
		//Fonction permettant de supprimer les mise en forme du texte specifié

		//Convertis en natif la chaine
		char * tmpStr = (char*)Marshal::StringToHGlobalAnsi(Str).ToPointer();
		//Recupere le texte en natif
		char * resNatif = xchat_strip(ph, tmpStr, Len, Flags);
		//Le convertis en managé
		String^ res = gcnew String((const char*)resNatif);
		//detruis la chaine native
		xchat_free(ph, resNatif);

		//Libere les ressource temporaires
		Marshal::FreeHGlobal((IntPtr)tmpStr);
		//Retourne le texte dégarnis de la mise en forme demandé
		return res;
	}
	
	IntPtr RemoteOperations::XChat_List_Get(String^ Name)
	{
		//Permet de récupérer un list a partir de son nom

		char * tmpName = (char *)Marshal::StringToHGlobalAnsi(Name).ToPointer();

		IntPtr res = (IntPtr)xchat_list_get(ph, tmpName);
		
		Marshal::FreeHGlobal((IntPtr)tmpName);

		//Renvoi le pointeur sur la liste demandée
		return res;
	}
	void RemoteOperations::XChat_List_Free(IntPtr xList)
	{
		//Libère les ressource d'une liste depuis son pointeur
		xchat_list_free(ph, (xchat_list*)(void*) xList);
	}
	array<String^>^ RemoteOperations::XChat_List_Fields(String^ Name)
	{
		//Fonction permettant d'obtenir la liste des champs d'une liste
		//Convertis la chaine en natif
		char * tmpName = (char*)Marshal::StringToHGlobalAnsi(Name).ToPointer();
		int nIndex = 0;

		//Récupère le tableau de chaine au format natif depuis le nom de la liste demandé
		const char* const* res = xchat_list_fields(ph, tmpName);

		//Instancie un tableau managé avec la meme taille que le natif
		while(res[nIndex] != (char*) nullptr)
		{
			nIndex++;
		}
		array<String^>^ tbRes= gcnew array<String^>(nIndex);

		nIndex = 0;
		while(res[nIndex] != (char*) nullptr)
		{
			//Remplis les case une a une du tableu managé depuis leurs valeurs
			tbRes[nIndex] = gcnew String((const char*)res[nIndex]);
			nIndex++;
		}

		//Libère les ressource allouées
		Marshal::FreeHGlobal((IntPtr)tmpName);

		//Renvois le tableau natif
		return tbRes;
	}
	int RemoteOperations::XChat_List_Next(IntPtr xList)
	{
		//Permet de passer au prochain item de la list a partir du pointeur de cette list
		//Cela sert d'enumerateur
		//Tant que la valeur renvoyé differe de "0" alors il reste des elements a lire
		//Renvoi donc la valeurs en question
		return xchat_list_next(ph, (xchat_list*)(void*)xList);
	}
	String^ RemoteOperations::XChat_List_Str(IntPtr xList, String^ Name)
	{
		//Convertis les "String^" managé en "char *" natif
		char * tmpName = (char*)Marshal::StringToHGlobalAnsi(Name).ToPointer();

		//Apelle la fonction xchat_list_str en spécifiant le pointeur sur la liste ainsi que le nom du champs en question
		const char * res = xchat_list_str(ph, (xchat_list*)(void*)xList, tmpName);

		//Detruis les pointeurs alloués
		Marshal::FreeHGlobal((IntPtr)tmpName);
		//Retourne la valeur convertis en "String^" managé depuis la valeur "char*" native
		return gcnew String(res);
	}
	int RemoteOperations::XChat_List_Int(IntPtr xList, String^ Name)
	{
		//Convertis les "String^" managé en "char *" natif
		char * tmpName = (char*)Marshal::StringToHGlobalAnsi(Name).ToPointer();

		//Apelle la fonction xchat_list_int en spécifiant le pointeur sur la liste ainsi que le nom du champs en question
		int res = xchat_list_int(ph, (xchat_list*)(void*)xList, tmpName);

		//Detruis les pointeurs alloués
		Marshal::FreeHGlobal((IntPtr)tmpName);
		//Retourne directement la valeur
		return res;
	}
	DateTime RemoteOperations::XChat_List_Time(IntPtr xList, String^ Name)
	{
		//Permet de recupere une varaible au Format "time_t" d'une des listes de Xchat
		//Convertis les "String^" managé en "char *" natif
		char * tmpName = (char*)Marshal::StringToHGlobalAnsi(Name).ToPointer();

		//time_t est definis en tant que "__int64" dans les header
		//Apelle la fonction xchat_list_time en spécifiant le pointeur sur la liste ainsi que le nom du champs en question
		__int64 res = xchat_list_time(ph, (xchat_list*)(void*)xList, tmpName);

		//Convertis les "String^" managé en "char *" natif
		Marshal::FreeHGlobal((IntPtr)tmpName);

		//retourne la valeur convertis sous forme de "DateTime" managé
		return DateTime::FromBinary(res);
	}


	
	String^ RemoteOperations::XChat_GetText(String^ MsgId)
	{
		//Fonction non documentée

		//Convertis les "String^" managé en "char *" natif
		char * tmpMsgId = (char *)Marshal::StringToHGlobalAnsi(MsgId).ToPointer();

		//Apelle Xchat avec les paramètre convertis et recupère la valeur dans le meme temps
		String ^res = gcnew String((const char*)xchat_gettext(ph, (const char *)tmpMsgId));
		//Detruis les pointeurs alloués
		Marshal::FreeHGlobal((IntPtr)tmpMsgId);

		return res;
	}

	
	IntPtr RemoteOperations::XChat_Plugingui_Add (String^ FileName, String^ Name, String^ Desc, String^ Version, String^% reserved)
	{
		//Fonction non documentée

		//Convertis les "String^" managé en "char *" natif
		char * tmpFileName = (char*)Marshal::StringToHGlobalAnsi(FileName).ToPointer();
		char * tmpName = (char*)Marshal::StringToHGlobalAnsi(Name).ToPointer();
		char * tmpDesc = (char*)Marshal::StringToHGlobalAnsi(Desc).ToPointer();
		char * tmpVersion = (char*)Marshal::StringToHGlobalAnsi(Version).ToPointer();
		char * tmpReserved = nullptr;

		//Appelle la fonction Xchat avec les paramètre passé
		IntPtr res = (IntPtr)xchat_plugingui_add(ph, tmpFileName, tmpName, tmpDesc, tmpVersion, tmpReserved);
		//Récupère la valeur du champs passé par référence
		reserved = gcnew String((const char*)tmpReserved);

		//Detruis les pointeur alloués
		Marshal::FreeHGlobal((IntPtr)tmpFileName);
		Marshal::FreeHGlobal((IntPtr)tmpName);
		Marshal::FreeHGlobal((IntPtr)tmpDesc);
		Marshal::FreeHGlobal((IntPtr)tmpVersion);

		//Renvoi le resultat
		return res;
	}
    void RemoteOperations::XChat_Plugingui_Remove (IntPtr Handle)
	{
		//Fonction non documentée

		//Cast le pointeur en "void *" pour pouvoir le passer a XChat puis appelle "xchat_plugingui_remove"
		xchat_plugingui_remove(ph, (void*) Handle);
	}


	
	array<System::String^>^ RemoteOperations::Convert(char** from)
	{
		//Créé un nouveau tableau de 32 case
		//Pourquoi 32 ? car cette fonction ne me sert que pour les varaible "Word" et "Word_eol"
		//qui font tous les 2 32 case d'apres la documentation
		array<String^>^ toReturn = gcnew array<String^>(32);

		//La premiere case est reservé et ne dois pas etre lu dans les registre Xchat
		//Je la met donc a "String::Empty" pour conserver les 32 case
		toReturn[0] = String::Empty;
		for(int i=1;i<32;i++)
		{
			//Remplis chaque case avec la valeur correspondante depuis sa valeur
			toReturn[i] = gcnew String((const char*)from[i]);
		}
		return toReturn;
	}
}