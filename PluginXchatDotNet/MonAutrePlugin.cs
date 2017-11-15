using System;
using System.Collections.Generic;
using System.Text;
using PluginInterface;
using PluginInterface.RemotingType;

namespace PluginXchatDotNet
{
    class MonAutrePlugin : MarshalByRefObject, IManagedPlugin
    {
        IRemoting remoteClient;
        Hook_Timer_CallBack Hook_Timer_Handler;
        IntPtr test;
        Hook_Timer_CallBack Hook_Timer_Handler2;
        IntPtr test2;

        //Durée de vie du service infini
        public override object InitializeLifetimeService()
        {
            return null;
        }

        #region IManagedPlugin Membres

        public void Load(IRemoting RemoteClient)
        {
            remoteClient = RemoteClient;
            remoteClient.XChat_Print("MonAutrePlugin\tLoaded");

            Hook_Timer_Handler = new Hook_Timer_CallBack(Timer_CallBack);
            Hook_Timer_Handler2 = new Hook_Timer_CallBack(Timer_CallBack2);
            test = remoteClient.XChat_Hook_Timer(5000, Hook_Timer_Handler, "timer 1");
            test2 = remoteClient.XChat_Hook_Timer(2000, Hook_Timer_Handler2, "timer 2");
        }

        public void Unload()
        {

        }

        public string Name
        {
            get
            {
                return "Mon Autre Plugin";
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
                return "Mon Autre Description";
            }
        }

        #endregion

        #region IDisposable Membres

        public void Dispose()
        {

        }

        #endregion

        #region "CallBacks"

        XChatTimer Timer_CallBack(Object User_Data)
        {
            remoteClient.XChat_Print("MonAutrePlugin\t" + User_Data.ToString() + " tick every 5 sec");
            return XChatTimer.Continue;
        }

        XChatTimer Timer_CallBack2(Object User_Data)
        {
            remoteClient.XChat_Print("MonAutrePlugin\t" + User_Data.ToString() + " tick every 2 sec");
            return XChatTimer.Continue;
        }

        #endregion
    }
}
