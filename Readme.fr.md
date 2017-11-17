# DESCRIPTION
Tout d'abord il s'agit de ma toutes première source en C++ / C++ CLI, pas en tant que source posté sur ce réseau mais étant la première fois que j'utilise ce langage disons de manière "normal" 

Qu'est ce que c'est : Ce code est une dll C++ CLI pouvant être chargé dans XChat en tant que plugin. 
Pourquoi cette source : Actuellement XChat ne gère pas les plugin de type .Net 

J'aurais pu faire une source banale en C++ CLI présentant les points d'entrés requis pour XChat et montrer qu'il est possible de faire des appels .Net avec. 

J'ai été un petit peu plus loin dans le developpement de cette source : 
Ce code permet lui même de chargé des assembly .net et fait l'interop entre Xchat et "n" plugins managés 

Ce code inclus 3 dll :
La dll d'interop entre XChat et les appel managé disons => "XChatInterop.dll"

celle ci a une commande "/DotNetManager" permettant d'ouvrir un formulaire de gestion des plugins .Net
Une dll d'interfacage entre cette dites dll d'interop et les plugins => "PluginInterface.dll" contenant les interfaces, type, delegué ect ... decrivant les "standards" de communication
Une derniere dll => "PluginXchatDotNet.dll" celle ci m'a servis de banc d'essais et sert en qq sorte a illustrer le fonctionnement basique du projet


Je n'ai pas encore testé dans son integralité ou essayé de poussé les plugin pour voir ce que cela pouvait supporté pour le moment 

Ce code se sert des AppDomain de .net permettant de charger et decharger a chaud les assembly en mémoire cela peux expliquer un code qui semble etre lourd, mais qui au final ne l'est pas tant que ca. 

(voir la conclusion pour l'utilisation de ce projet) 


```
// Il s'agit du fichier DLL principal.
#include "stdafx.h"
#include "xchat-plugin.h"
#include "XChatInterop.h"
#include "frmManager.h"
#include "DomainInformation.h"
#include "AppDomainUpdater.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#define NAME "DotNet Plugins Manager"
#define DESC "Admit you to Load/Unload DotNet Plugins"
#define VERSION "1.0"

using namespace System;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;
using namespace XChatInterop;

xchat_hook * cmdDotNetManager; /* Hook de la commande "DotNetManager" pour ouvrir "frmManager" */
static xchat_plugin *ph;   /* plugin handle */

//Déclaration des fonctions du fichier
extern "C" __declspec( dllexport ) int xchat_plugin_init(xchat_plugin *plugin_handle, char **plugin_name, char **plugin_desc, char **plugin_version,char *arg);
extern "C" __declspec( dllexport ) int xchat_plugin_deinit(void);
static int DotNetManager(char *word[], char *word_eol[], void *userdata);

//Point d'entré du plugin
int xchat_plugin_init(xchat_plugin *plugin_handle, char **plugin_name, char **plugin_desc, char **plugin_version,char *arg)
{
	ph = plugin_handle;

	
plugin_name= NAME;
	
plugin_desc=DESC;
	
plugin_version=VERSION;


	//Change le "BaseDirectory" de l'AppDomain pour qu'il correspondent a celui ou est la dll de ce plugin
	AppDomainUpdater^ CurrentAppDomainUpdater = gcnew AppDomainUpdater(AppDomain::CurrentDomain);
	CurrentAppDomainUpdater->BaseDirectory = Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location);

	//Met en place le hook pour la commande "DotNetManager"
	cmdDotNetManager = xchat_hook_command(ph, "DotNetManager", XCHAT_PRI_NORM, DotNetManager, "Usage: DOTNETMANAGER, Show the window manager.", 0);

	//Le chargement c'est apparement bien passé
	xchat_print(ph, "DotNet Plugins Manager loaded succefully\n");

	return 1;
}

//Appelé au déchargement
int xchat_plugin_deinit()
{
	//Enlève le hook de la commande "DotNetManager"
	xchat_unhook(ph, cmdDotNetManager);

	DomainInformation::UnloadAll();

	//Déchargement finis
	xchat_print(ph, "DotNet Plugins Manager unloaded succefully!\n");

	return 1;
}
//Appelé quand qqun execute la commande "/DotNetManager"
static int DotNetManager(char *word[], char *word_eol[], void *userdata)
{
	frmManager^ Manager = gcnew frmManager();
	//Ouvre le manager en specifiant le "Process::GetCurrentProcess()->MainWindowHandle" c'est a dire le handle de Xchat comme parent
	//et en le wrappant avec la classe "MainWindowWrapper" afin de pouvoir le caster en "IWin32Window" requis pour la methode "Show"
	Manager->Show((IWin32Window^)(gcnew MainWindowWrapper(Process::GetCurrentProcess()->MainWindowHandle)));

	//Propre a ce plugin
	return XCHAT_EAT_ALL;
}

//Assesceur de "ph"
xchat_plugin * GetPh()
{
	return ph;
}
```

# CONCLUSION :

Si il y a des erreur dans la programmation que ca soit dans le "standard" de la syntaxe ou des mauvaise utilisation ou autre n'hesiter pas a m'en faire par en commentaire ici. 

### Documentation supplémentaire : 
http://www.xchat.org/docs/plugin20.html 

### Utilisation :
Installer VS.Net (avec au moins la prise en charge C++.net et pourquoi pas C#, VB.net)
Installer Xchat
Créer un projet .net avec "PluginInterface" dans les Références (mais pas besoin de "XChatInterop") cela est volontaire
Créer une classe qui herite de "MarshalByRefObject" et qui implémente "PluginInterface.IManagedPlugin"
Implementer toutes les methode requise bien entendu
Implémenter "PluginInterface.IOptions" (Facultatif) : cela permet d'activé le bouton "Options" de la fenêtre de config
Pour le debug il faut aller dans les options du projet et indiquer Xchat.exe en tant que programme a lancer, pas besoin d'argument supplementaire

en toute logique cela va simplement lancer Xchat rien de plus
les point d'arret que vous aurez mis seront marquer comme ne pouvant etre déclenché cela est parfaitement normal il vous suffira d'aller dans le menu "Fenetre > Greffons et scripts" puis charger "XChatInterop.dll"
Tapper "/DotNetManager" une fois le chargement fini et enfin charger votre dll .net


### ATTENTION il ne faut pas oublier :
Que les dll soient absolument toutes dans le même repertoire pour des question de typage entre assembly (ou alros les enregistrer dans le GAC et changer legerement le code)
et ne pas oublier d'overrider :

```
//C# : 
//Durée de vie du service infini 
public override object InitializeLifetimeService() 
{ 
return null; 
} 

//C++.Net : 
virtual Object^ InitializeLifetimeService() override 
{ 
return nullptr; 
} 
```

La valeur "null" specifie que la durée du service est infini, pour plus de detail se reporter dans la doc MSDN a la fonction : "InitializeLifetimeService" de la classe "MarshalByRefObject"
