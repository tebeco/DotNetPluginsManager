using System;
using System.Collections.Generic;
using System.Text;
using PluginInterface.RemotingType;

namespace PluginInterface
{
    namespace RemotingType
    {
        //Délégué de CallBack correspond aux même signature que ceux de Xchat
        public delegate XChatEat Hook_Command_CallBack(String[] Word, String[] Word_Eol, Object User_Data);
        public delegate XChatEat Hook_Server_CallBack(String[] Word, String[] Word_Eol, Object User_Data);
        public delegate XChatEat Hook_Print_CallBack(String[] Word, Object User_Data);
        public delegate XChatTimer Hook_Timer_CallBack(Object User_Data);
        public delegate XChatEat Hook_Fd_CallBack(int Fd, XChatFd Flag, Object User_Data);

        //Enums utilisées pour l'interop
        //Prioritées des Hook
        public enum XChatPriority
        {
            Highest	= 127,
            High = 64,
            Normal = 0,
            Low = -64,
            Lowest = -128
        };
        //File Descriptor
        public enum XChatFd
        {
            Read = 1,
            Write = 2,
            Exception = 4,
            NotSocket = 8,
        };
        //Valeurs renvoyées par les Hook
        public enum XChatEat 
        {
            None = 0,
            Xchat = 1,
            Plugin = 2,
            All = 3
         };
        //A renvoyer lors des CallBack des Timers
        public enum XChatTimer
        {
            Stop = 0,
            Continue = 1
        }
        //Specifie le type du hook
        public enum XChatHook
        {
            Command,
            Server,
            Print,
            Timer,
            Fd
        }
    }

    public interface IRemoting
    {
        //Diverse fonction de hook
        IntPtr XChat_Hook_Command(String Name, XChatPriority Priority, Hook_Command_CallBack CallBack, String HelpText, Object UserData);
        IntPtr XChat_Hook_Server(String Name, XChatPriority Priority, Hook_Server_CallBack CallBack, Object UserData);
        IntPtr XChat_Hook_Print(String Name, XChatPriority Priority, Hook_Print_CallBack CallBack, Object UserData);
        IntPtr XChat_Hook_Timer(int TimeOut, Hook_Timer_CallBack CallBack, Object UserData);
        //Les File Descriptor ne sont pas gerer pour le moment je ne sais pas si je le completerais ou non
        //IntPtr XChat_Hook_Fd(int Fd, XChatFd Flags, Hook_Fd_CallBack CallBack, Object UserData);
        
        //Fonction pour supprimer des Hook préallablement posés
        Object XChat_Unhook(IntPtr Hook, XChatHook HookType);
        void UnHookAll();

        void XChat_Command(String Command);
        void XChat_Commandf(String Format, params String[] Args); 
        void XChat_Print(String Text);
        void XChat_Printf(String Format, params String[] Args); 
        int XChat_Emit_Print(String Event_Name, params String[] Args);
        void XChat_Send_Modes(String[] Targets, int NTargets, int Modes_Per_Line, Char Sign, Char Mode);

        IntPtr XChat_Find_Context(String Servname, String Channel);
        IntPtr XChat_Get_Context();
        String XChat_Get_Info(String Id);
        int XChat_Get_Prefs(String Name, ref String @String, ref int Integer);
        int XChat_Set_Context(IntPtr Ctx);

        int XChat_Nickcmp(String s1, String s2);
        String XChat_Strip(String Str, int Len, int Flags);

        //Manipulation des listes de Xchat
        IntPtr XChat_List_Get(String Name);
        void XChat_List_Free(IntPtr xList);
        String[] XChat_List_Fields(String Name);
        int XChat_List_Next(IntPtr xList);
        String XChat_List_Str(IntPtr xList, String Name);
        int XChat_List_Int(IntPtr xList, String Name);
        DateTime XChat_List_Time(IntPtr xList, String Name);

        String XChat_GetText(String msgid);

        IntPtr XChat_Plugingui_Add(String FileName, String Name, String Desc, String Version, ref String reserved);
        void XChat_Plugingui_Remove(IntPtr Handle);

    }

}