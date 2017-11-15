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
	*plugin_name= NAME;
	*plugin_desc=DESC;
	*plugin_version=VERSION;

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