using System;
using System.Collections.Generic;
using System.Text;

namespace PluginInterface
{
    public interface IManagedPlugin : IDisposable
    {
        //Point d'entré du plugin
        void Load(IRemoting RemoteClient);
        //Lancer au dechargement
        void Unload();

        //Asseceur permettant de se renseigner sur les infos tel que le nom, la description et la version du plugin
        String Name
        {
            get;
        }
        String Version
        {
            get;
        }
        String Description
        {
            get;
        }

        //Destructeur
        new void Dispose();
    }
}
