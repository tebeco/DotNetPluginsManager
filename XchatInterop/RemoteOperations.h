#pragma once
#include "xchat-plugin.h"
#include "HookCommandDescription.h"
#include "HookServerDescription.h"
#include "HookPrintDescription.h"
#include "HookTimerDescription.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace PluginInterface;
using namespace PluginInterface::RemotingType;

namespace XChatInterop
{
	//Toutes les fonction d'interop entre XChat et le plugin (dans le sens plugins => XChat) sont ici
	//Description faites dans le .cpp

#pragma managed(push, off)
	void Mon_XChat_Commandf(char * Format, long count,char ** arg, void (* Commandf) (xchat_plugin *phTmp, const char *format, ...)); 
	int Mon_XChat_Emit_Print(char * Event_Name, long count,char ** arg, int (* Commandf) (xchat_plugin *phTmp, const char *event_name, ...));
	void Mon_XChat_Printf(char * Format, long count,char ** arg, void (* XChatPrintf) (xchat_plugin *phTmp, const char *format, ...));
#pragma managed(pop)

	int Hook_Commands_CallBack(char *word[], char *word_eol[], void *userdata);
	int Hook_Servers_CallBack(char *word[], char *word_eol[], void *userdata);
	int Hook_Prints_CallBack(char *word[], void *userdata);
	int Hook_Timers_CallBack(void *userdata);

	public ref class RemoteOperations : IRemoting, MarshalByRefObject
	{
	public:
		RemoteOperations();

		virtual IntPtr XChat_Hook_Command(String^ Name, XChatPriority Priority, Hook_Command_CallBack^ CallBack, String^ HelpText, Object^ UserData);
		virtual IntPtr XChat_Hook_Server(String^ Name, XChatPriority Priority, Hook_Server_CallBack^ CallBack, Object^ UserData);
		virtual IntPtr XChat_Hook_Print(String^ Name, XChatPriority Priority, Hook_Print_CallBack^ CallBack, Object^ UserData);
		virtual IntPtr XChat_Hook_Timer(int TimeOut, Hook_Timer_CallBack^ CallBack, Object^ UserData);
		
		virtual Object^ XChat_Unhook(IntPtr Hook, XChatHook HookType);
		virtual void UnHookAll();

		virtual void XChat_Command(String^ Command);
		virtual void XChat_Commandf(String^ Format,... array<String^>^ Args);
		virtual void XChat_Print(String^ Text);
		virtual void XChat_Printf(String^ Format, ... array<String^>^ Args);
		virtual int XChat_Emit_Print(String^ Event_Name,... array<String^>^ Args);
        virtual void XChat_Send_Modes (array<String^>^ Targets, int NTargets, int Modes_Per_Line, Char Sign, Char Mode);

        virtual IntPtr XChat_Find_Context(String^ Servname, String^ Channel);
        virtual IntPtr XChat_Get_Context();
		virtual String^ XChat_Get_Info(String^ Id);
        virtual int XChat_Get_Prefs(String^ Name, String^% String, int% Integer);
        virtual int XChat_Set_Context(IntPtr Ctx);

		virtual int XChat_Nickcmp(String^ s1, String^ s2);
		virtual String^ XChat_Strip(String^ Str, int Len, int Flags);

		virtual IntPtr XChat_List_Get(String^ Name);
		virtual void XChat_List_Free(IntPtr xList);
        virtual array<String^>^ XChat_List_Fields(String^ Name);
        virtual int XChat_List_Next(IntPtr xList);
        virtual String^ XChat_List_Str(IntPtr xList, String^ Name);
        virtual int XChat_List_Int(IntPtr xList, String^ Name);
        virtual DateTime XChat_List_Time(IntPtr xList, String^ Name);

		virtual String^ XChat_GetText(String^ msgid);

		virtual IntPtr XChat_Plugingui_Add(String^ FileName, String^ Name, String^ Desc, String^ Version, String^% reserved);
        virtual void XChat_Plugingui_Remove(IntPtr Handle);

		//Durée de vie du service infini
        virtual Object^ InitializeLifetimeService() override
        {
            return nullptr;
        }
	internal:
		static List<HookCommandDescription^>^ lstHookCommandDescription = gcnew List<HookCommandDescription^>();
		static List<HookServerDescription^>^ lstHookServerDescription = gcnew List<HookServerDescription^>();
		static List<HookPrintDescription^>^ lstHookPrintDescription = gcnew List<HookPrintDescription^>();
		static List<HookTimerDescription^>^ lstHookTimerDescription = gcnew List<HookTimerDescription^>();

		static array<System::String^>^ Convert(char** from);
	private:
		xchat_plugin * ph;				//Pointeur sur le plugin_handle du plugin xchat
		int UniqueId;					//ID de la classe en cour
		static int LastUniqueId = 0;	//Compteur pour les UniqueId des Plugins pour differencié les hook entre eux
	};
}