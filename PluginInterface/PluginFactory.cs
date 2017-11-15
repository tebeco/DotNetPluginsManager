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
        //Contient le le chemin de la dll charg� dans l'appdomain
        private String AssemblyPath;

        //Doit avoir un constructeur par defaut
        public PluginFactory(){}

        public Boolean CreateInstance(PluginDescription PlugDesc)
        {
            try
            {
                //return (IManagedPlugin)Activator.CreateInstanceFrom(assembly, typename, false, bfi, null, args, null, null, null).Unwrap();

                //Cr�� une instance de la dll/classe sp�cifi� par le descripteur dans l'assembly cr�� auparevent
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

        //Enumere les plugins de la dll dont le chemin est pass� en param�tre
        public List<PluginDescription> EnumeratePlugins(String assembly)
        {
            //Collection String qui servira a list� les NameSpace complet des classes pouvant correspondre a des plugins
            List<PluginDescription> pluginDesc = new List<PluginDescription>();
            //Sert a la description des plugins dans la boucle
            PluginDescription tmpPlugDesc;

            AssemblyPath = assembly;
            //Charge l'assembly dont le chemin correspondant est pass� en param�tre
            Assembly loadedAssembly = Assembly.LoadFrom(AssemblyPath);

            //Enum�re les differents types dedans
            foreach (Type type in loadedAssembly.GetTypes())
            {
                if (!type.IsMarshalByRef)
                    //Si le type n'herite pas de "MarshalByRefObject" le type est rejet�
                    continue;
                if (type.IsAbstract)
                    //Si il s'agit d'une classe Abstraite le type est rejet�
                    continue;
                if (Array.IndexOf(type.GetInterfaces(), typeof(IManagedPlugin)) < 0)
                    //Si le type n'impl�mente pas "IManagedPlugin" le type est rejet�
                    continue;

                //Si le code est arriv� l�, alors le type peut un plugin potentiellement chargeable
                //On cr�� alors une nouvelle structure pour decrire ce type
                tmpPlugDesc = new PluginDescription();
                tmpPlugDesc.AssemblyName = assembly;
                //Indique que le plugins a pas �t� charg�
                tmpPlugDesc.Loaded = false;
                //Indique le fait qu'il soit demand� au chargement
                tmpPlugDesc.Requested = true;
                //Le plugin n'existe pas encore donc :
                tmpPlugDesc.Plugin = null;
                tmpPlugDesc.Exception = null;
                //Remplis le NameSpace
                tmpPlugDesc.FullName = type.FullName;
                //Met a true "HasOptions" si le type impl�mente "IOptions", sinon � false
                //Superieur ou egal car les Index commence a 0 et IndexOf renvoi l'Index
                //Renvoi -1 si non trouv�
                tmpPlugDesc.HasOptions = (Array.IndexOf(type.GetInterfaces(), typeof(IOptions)) >= 0);
                //on rajoute donc a la liste cette description du plugin
                pluginDesc.Add(tmpPlugDesc);
            }

            //Une fois termin� on retourne la collection
            return pluginDesc;
        }

        //Dur�e de vie du service infini
        public override object InitializeLifetimeService()
        {
            return null;
        }
    }
}