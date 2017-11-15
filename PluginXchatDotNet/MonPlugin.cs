using System;
using System.Collections.Generic;
using System.Text;
using PluginInterface;
using PluginInterface.RemotingType;

namespace PluginXchatDotNet
{
    class MonPlugin : MarshalByRefObject, IManagedPlugin, IOptions
    {
        IRemoting remoteClient;
        Hook_Command_CallBack Hook_Command_Handler;
        Hook_Print_CallBack Hook_Print_Handler;
        Hook_Server_CallBack Hook_Server_Handler;
        IntPtr CurrentContext2;
        IntPtr HookCommandMANAGED;

        //Durée de vie du service infini
        public override object InitializeLifetimeService()
        {
            return null;
        }

        #region IManagedPlugin Membres

        public void Load(IRemoting RemoteClient)
        {
            remoteClient = RemoteClient;

            remoteClient.XChat_Print("MonPlugin\tLoaded");
                        
            remoteClient.XChat_Command("PRIVMSG #a Test de XChat_Command");
            remoteClient.XChat_Commandf("PRIVMSG %s Test de %s", "#a", "XChat_Commandf");

            remoteClient.XChat_Emit_Print("Channel Message", "MonPlugin", "Test de XChat_Emit_Print", "@", null);

            IntPtr CurrentContext = remoteClient.XChat_Find_Context(null, null);
            remoteClient.XChat_Print("Current Context (XChat_Find_Context): " + CurrentContext.ToString());

            CurrentContext2 = remoteClient.XChat_Get_Context();
            remoteClient.XChat_Print("Current Context (XChat_Get_Context): " + CurrentContext2.ToString());

            String ServerName = remoteClient.XChat_Get_Info("server");
            remoteClient.XChat_Print("Server Name (XChat_Get_Info): " + ServerName);

            String str="";
            Int32 i = 0;
            if (remoteClient.XChat_Get_Prefs("irc_nick1", ref str, ref i) == 1)
                remoteClient.XChat_Printf("XChat_Get_Prefs : Current irc_nick1 setting (String): %s\n", str);
            else
                remoteClient.XChat_Print("XChat_Get_Prefs Failed : irc_nick1");

            if (remoteClient.XChat_Get_Prefs("tab_new_to_front", ref str, ref i) == 2)
                remoteClient.XChat_Printf("XChat_Get_Prefs : Current tab_new_to_front setting (Interger): %s\n", i.ToString());
            else
                remoteClient.XChat_Print("XChat_Get_Prefs Failed : tab_new_to_front");

            if (remoteClient.XChat_Get_Prefs("text_wordwrap", ref str, ref i) == 3)
                remoteClient.XChat_Printf("XChat_Get_Prefs : Current text_wordwrap setting (Boolean): %s\n", i.ToString());
            else
                remoteClient.XChat_Print("XChat_Get_Prefs Failed : text_wordwrap");

            //Fonction non documentée
            remoteClient.XChat_Print("Fonction non documenté (XChat_GetText) : " + remoteClient.XChat_GetText("a"));
            
            Hook_Command_Handler = new Hook_Command_CallBack(Command_CallBack);
            HookCommandMANAGED = remoteClient.XChat_Hook_Command("HookCommandMANAGED", XChatPriority.Normal, Hook_Command_Handler, "helpText HookCommandMANAGED helpText", null);

            
            Hook_Print_Handler = new Hook_Print_CallBack(Print_CallBack);
            remoteClient.XChat_Hook_Print("You Kicked", XChatPriority.Normal, Hook_Print_Handler, null);
            
            Hook_Server_Handler = new Hook_Server_CallBack(Server_CallBack);
            remoteClient.XChat_Hook_Server("KICK", XChatPriority.Normal, Hook_Server_Handler, null);


            //See the other plugin for timers
            //remoteClient.XChat_Hook_Timer();

            
            String[] Listes = remoteClient.XChat_List_Fields("lists");

            /*
dcc         static const char * const dcc_fields[] =
            {
                "iaddress32","icps",		"sdestfile","sfile",		"snick",	"iport",
                "ipos", "iresume", "isize", "isizehigh", "istatus", "itype", NULL
            };
            
channels    static const char * const channels_fields[] =
            {
                "schannel",	"schantypes", "pcontext", "iflags", "iid", "ilag", "imaxmodes",
                "snetwork", "snickmodes", "snickprefixes", "iqueue", "sserver", "itype", "iusers",
                NULL
            };
            
ignore      static const char * const ignore_fields[] =
            {
                "iflags", "smask", NULL
            };

notify      static const char * const notify_fields[] =
            {
                "iflags", "snetworks", "snick", "toff", "ton", "tseen", NULL
            };

users       static const char * const users_fields[] =
            {
                "iaway", "shost", "tlasttalk", "snick", "sprefix", "iselected", NULL
            };

lists       static const char * const list_of_lists[] =
            {
                "channels",	"dcc", "ignore", "notify", "users", NULL
            };
            */
            //Listes[4] ==> "users"
            //Cf : plugin.c
            IntPtr list = remoteClient.XChat_List_Get(Listes[4]);

            while (remoteClient.XChat_List_Next(list) != 0)
            {
                int away = remoteClient.XChat_List_Int(list, "away");
                DateTime lasttalk = remoteClient.XChat_List_Time(list, "lasttalk");
                String nick = remoteClient.XChat_List_Str(list, "nick");
                String host = remoteClient.XChat_List_Str(list, "host");
                String prefix =remoteClient.XChat_List_Str(list, "prefix");
                int selected = remoteClient.XChat_List_Int(list, "selected");
                remoteClient.XChat_Printf("%s %s %s %s %s %s\n",away.ToString(), lasttalk.ToShortTimeString(), nick, host, prefix, selected.ToString());
                    
            }

            remoteClient.XChat_List_Free(list);

            int res = remoteClient.XChat_Nickcmp("TeBeCo", "TeBeCo_");
            remoteClient.XChat_Print("MonPlugin : XChat_Nickcmp : " + res.ToString());

            /*
             * Weird results
            */
            String sReserved = "";
            IntPtr DnsFakePlug = remoteClient.XChat_Plugingui_Add("xcdns.dll", "DNS", "dns resolver", "1.0", ref sReserved);
            remoteClient.XChat_Plugingui_Remove(DnsFakePlug);
            
            remoteClient.XChat_Print("Test de XChat_Print");
            remoteClient.XChat_Printf("Test de %s", "XChat_Printf");
            
            remoteClient.XChat_Send_Modes(new String[] {"TeBeCo"}, 1, 1, '+', 'v');

            String sCouleur = Convert.ToChar(3) + "12Blue" + Convert.ToChar(3) + " " + Convert.ToChar(2) + "Bold!" + Convert.ToChar(3);
            String sStriped = remoteClient.XChat_Strip(sCouleur, -1, 1 | 2);
            remoteClient.XChat_Printf("Unstriped : %s\n", sCouleur);
            remoteClient.XChat_Printf("Striped : %s\n", sStriped);
        }

        public void Unload()
        {
            remoteClient.UnHookAll();
        }

        public string Name
        {
            get
            {
                return "Mon Plugin";
            }
        }

        public string Version
        {
            get
            {
                return "0.1";
            }
        }

        public string Description
        {
            get
            {
                return "Ma Description";
            }
        }

        #endregion

        #region IDisposable Membres

        public void Dispose()
        {

        }

        #endregion

        #region IOptions Membres

        public void ShowDialog()
        {
            ;
        }

        #endregion

        #region "CallBacks"

        XChatEat Command_CallBack(String[] Word, String[] Word_Eol, Object User_Data)
        {
            remoteClient.XChat_Print("MonPlugin : XChat_Hook_Command " + String.Join(" ", Word));

            remoteClient.XChat_Print("MonPlugin : Before XChat_Set_Context");
            remoteClient.XChat_Set_Context(CurrentContext2);
            remoteClient.XChat_Print("MonPlugin : After XChat_Set_Context");

            remoteClient.XChat_Unhook(HookCommandMANAGED, XChatHook.Command);
            remoteClient.XChat_Print("MonPlugin : XChat_Unhook => \"/HookCommandMANAGED\" Won't work anymore");
            return XChatEat.All;
        }

        XChatEat Print_CallBack(String[] Word, Object User_Data)
        {
            remoteClient.XChat_Print("MonPlugin : XChat_Hook_Print \"You Kicked\" Event" + String.Join(" ", Word));
            return XChatEat.None;
        }

        XChatEat Server_CallBack(String[] Word, String[] Word_Eol, Object User_Data)
        {
            remoteClient.XChat_Print("MonPlugin : XChat_Hook_Server \"KICK\" : " + String.Join(" ", Word));
            return XChatEat.None;
        }

        #endregion
    }
}
