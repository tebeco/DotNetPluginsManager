using System;
using System.Collections.Generic;
using System.Text;

namespace PluginInterface
{
    //Classe décrivant chaque Plugin "eligible" dans les assembly
    public class PluginDescription : MarshalByRefObject
    {
        //Nom de la dll conteneur
        public String AssemblyName;
        //Nom complet de la classe depuis le namespace racine
        public String FullName;
        //Indique si le plugin implémente "IOptions"
        public Boolean HasOptions;
        //Indique si le plugin a été chargé dans l'appdomain
        //ATTENTION NE PAS CONFONDRE "Charger" et "Démarrer"
        //Ici il s'agit juste de l'appel a "CreateInstance"
        public Boolean Loaded;
        //Indique si l'utilisateur a coché ce plugin dans la CheckBoxList
        public Boolean Requested;
        //Proxy Transparent vers le plugin
        public IManagedPlugin Plugin;
        //Utilisé pour les erreurs potentiel a l'instanciation
        public Exception Exception;
        //Permet de lié un plugin a sa classe de remoting
        public IRemoting RemontingProxy;

        //Durée de vie du service infini
        public override object InitializeLifetimeService()
        {
            return null;
        }
    }
}
