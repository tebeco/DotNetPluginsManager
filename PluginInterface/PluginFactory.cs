using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;

namespace PluginInterface
{
    public class PluginFactory : MarshalByRefObject
    {
        //Utile pour l'appel de membre par Reflexion
        private const BindingFlags bfi = BindingFlags.Instance | BindingFlags.Public | BindingFlags.CreateInstance;
        //Contient le le chemin de la dll chargé dans l'appdomain
        private String AssemblyPath;

        //Doit avoir un constructeur par defaut
        public PluginFactory(){}

        public Boolean CreateInstance(PluginDescription PlugDesc)
        {
            try
            {
                //return (IManagedPlugin)Activator.CreateInstanceFrom(assembly, typename, false, bfi, null, args, null, null, null).Unwrap();

                //Créé une instance de la dll/classe spécifié par le descripteur dans l'assembly créé auparevent
                IManagedPlugin tmpIPlug = (IManagedPlugin)Activator.CreateInstanceFrom(PlugDesc.AssemblyName, PlugDesc.FullName, false, bfi, null, null, null, null, null).Unwrap();
                //Remplis le proxy transparent si le chargement a eu lieu correctement et renvoi "true"
                PlugDesc.Plugin = tmpIPlug;
                return true;
            }
            catch(Exception ex)
            {
                //Si une erreur se produit alors on indique via le descripteur l'erreur et on retourne "false"
                PlugDesc.Exception = ex;
                return false;
            }
        }

        //Enumere les plugins de la dll dont le chemin est passé en paramètre
        public List<PluginDescription> EnumeratePlugins(String assembly)
        {
            //Collection String qui servira a listé les NameSpace complet des classes pouvant correspondre a des plugins
            List<PluginDescription> pluginDesc = new List<PluginDescription>();
            //Sert a la description des plugins dans la boucle
            PluginDescription tmpPlugDesc;

            AssemblyPath = assembly;
            //Charge l'assembly dont le chemin correspondant est passé en paramètre
            Assembly loadedAssembly = Assembly.LoadFrom(AssemblyPath);

            //Enumère les differents types dedans
            foreach (Type type in loadedAssembly.GetTypes())
            {
                if (!type.IsMarshalByRef)
                    //Si le type n'herite pas de "MarshalByRefObject" le type est rejeté
                    continue;
                if (type.IsAbstract)
                    //Si il s'agit d'une classe Abstraite le type est rejeté
                    continue;
                if (Array.IndexOf(type.GetInterfaces(), typeof(IManagedPlugin)) < 0)
                    //Si le type n'implémente pas "IManagedPlugin" le type est rejeté
                    continue;

                //Si le code est arrivé là, alors le type peut un plugin potentiellement chargeable
                //On créé alors une nouvelle structure pour decrire ce type
                tmpPlugDesc = new PluginDescription();
                tmpPlugDesc.AssemblyName = assembly;
                //Indique que le plugins a pas été chargé
                tmpPlugDesc.Loaded = false;
                //Indique le fait qu'il soit demandé au chargement
                tmpPlugDesc.Requested = true;
                //Le plugin n'existe pas encore donc :
                tmpPlugDesc.Plugin = null;
                tmpPlugDesc.Exception = null;
                //Remplis le NameSpace
                tmpPlugDesc.FullName = type.FullName;
                //Met a true "HasOptions" si le type implémente "IOptions", sinon à false
                //Superieur ou egal car les Index commence a 0 et IndexOf renvoi l'Index
                //Renvoi -1 si non trouvé
                tmpPlugDesc.HasOptions = (Array.IndexOf(type.GetInterfaces(), typeof(IOptions)) >= 0);
                //on rajoute donc a la liste cette description du plugin
                pluginDesc.Add(tmpPlugDesc);
            }

            //Une fois terminé on retourne la collection
            return pluginDesc;
        }

        //Durée de vie du service infini
        public override object InitializeLifetimeService()
        {
            return null;
        }
    }
}