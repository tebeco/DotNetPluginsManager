#include "StdAfx.h"
#include "DomainInformation.h"
#include "RemoteOperations.h"
#include "frmEnumPlugins.h"

using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;
using namespace System::Threading;
using namespace PluginInterface;

namespace XChatInterop {

	DomainInformation::DomainInformation(String^ assembly)
	{
		//Initialize la liste de description des potentiels plugins comme étant vide
		_pluginsDesc = gcnew List<PluginDescription^>();
		this->bEmpty = true;		//Initialise la classe comme étant vide

		if (DomainInformation::domains == nullptr) 
		{
			//Si la collection listant les Instance de gestion des AppDomain n'a pas été instancié elle même
			//On l'instancie
			DomainInformation::domains = gcnew List<DomainInformation^>();
		}
		//Rajoute cette instance dans la collection générale (pour ne pas oublier de la detruire)
		DomainInformation::domains->Add(this);

		//On mémorise le nom de l'assembly chargé
		CurrentAssembly = assembly;

		//On verouille le fichier
		_fileLock = gcnew FileLock(CurrentAssembly);
		if(_fileLock->Lock() == false)
		{
			return;
		}

		//Créé un nouveau domain d'application avec comme nom celui de la dll sélectionnée
		domain = AppDomain::CreateDomain(Path::GetFileName(CurrentAssembly));
		//Charge une classe dans l'AppDomain a partir de la dll "PluginInterface.dll", la classe chargé depuis son namespace "PluginInterface.PluginFactory"
		factory = (PluginFactory^)domain->CreateInstanceFromAndUnwrap(L"PluginInterface.dll", L"PluginInterface.PluginFactory");
		//Récupère la liste des "potentiels" plugins dans la dll
		_pluginsDesc = factory->EnumeratePlugins(CurrentAssembly);

		if(_pluginsDesc->Count == 0) 
		{
			//La dll ne contient pas de plugins
			MessageBox::Show("The assembly file does not contains any available plugins.\nMake sure the class implements the following objects :\n   * System.MarshalByRefObject\n   * PluginInterface.IManagedPlugin","Xchat DotNet Managed Manager", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		//Si il y a un ou plusieur plugins a charger alors :
		//On affiche un formulaire permerttant un le choix du ou des plugins à charger
		frmEnumPlugins^ EnumUI = gcnew frmEnumPlugins();
		EnumUI->PluginsDescription = _pluginsDesc;
		EnumUI->ShowDialog();
		_pluginsDesc = EnumUI->PluginsDescription;
		
		//Pour chaqun des plugin éligible :
		for (int nIndex = 0; nIndex < _pluginsDesc->Count;nIndex++)
		{
			//Recupere l'item
			PluginDescription^ tmpPlugDesc = _pluginsDesc[nIndex];
			if(!tmpPlugDesc->Requested)
			{
				//Si la case n'as pas été coché alors on ne le charge pas et on passe au suivant
				continue;
			}
			//On instancie la classe en question
			if( !(factory->CreateInstance(tmpPlugDesc)) )
			//Si la fonction renvoi false
			//Une erreur c'est produite à la tentative d'instanciation de la classe servant de plugin
			{
				if (tmpPlugDesc->Exception->GetType()->Name == "MethodAccessException")
				{
					//MethodAccessException L'appelant n'a pas l'autorisation d'appeler ce constructeur.
					MessageBox::Show("The caller does not have permission to call this constructor : " + tmpPlugDesc->FullName,"Xchat DotNet Managed Manager", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				if (tmpPlugDesc->Exception->GetType()->Name == "SecurityException")
				{
					//SecurityException L'appelant a les FileIOPermission nécessaires. 
					MessageBox::Show("The caller does have the required File In/Out Permission.","Xchat DotNet Managed Manager", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				if (tmpPlugDesc->Exception->GetType()->Name == "MissingMethodException")
				{
					//MissingMethodException Aucun constructeur correspondant n'a été trouvé.
					MessageBox::Show("No matching constructor was found. Please make sure you have a default constructor in the class :" + tmpPlugDesc->FullName,"Xchat DotNet Managed Manager", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				if (tmpPlugDesc->Exception->GetType()->Name == "TargetInvocationException")
				{
					//TargetInvocationException Le constructeur, qui a été appelé par la réflexion, a levé une exception. 
					MessageBox::Show("The constructor, which was invoked through reflection, threw an exception.","Xchat DotNet Managed Manager", MessageBoxButtons::OK, MessageBoxIcon::Error);
				}
				//Apres presentation de l'erreur on quitte la procédure de chargement de ce plugin.
				continue;
			}
			InitLoad(tmpPlugDesc);
		}
	}

	DomainInformation::~DomainInformation()
	{
		//Detruit la classe de chargement
		delete factory;
		_fileLock->UnLock();
		delete _fileLock;

		if(_pluginsDesc != nullptr)
		{
			//Si la liste des description des plugins contient qqchs alors pour chaque éléments
			for(int nIndex = (this->_pluginsDesc->Count - 1); nIndex >=0; nIndex--)
			{
				PluginDescription^ pluginDesc = _pluginsDesc[nIndex];
				//si le plugin a été chargé correctement
				if(pluginDesc->Loaded)
				{
					//Indique que le plugin n'est plus chargé
					pluginDesc->Loaded = false;
					_pluginsDesc->Remove(pluginDesc);
					try
					{
						//On décharge le plugin associé 
						this->UnloadPlugin(pluginDesc);
						//Et on appel son desctructeur
						delete pluginDesc->Plugin;
					}
					catch (Exception^ ex)
					{
						//Lorsqu'une erreur se produit durant l'appel a la methode "Unload"
						MessageBox::Show("An Unhandled exception occured during the execution of the ""Unload"" or ""Dispose"" subroutine.\nThe plugin has been unloaded anyway.\nPlease report to your associated code in order to handle it.\n\nMore Informations :\nMessage :" + ex->Message,"Xchat DotNet Managed Manager",MessageBoxButtons::OK, MessageBoxIcon::Error);
					}
				}
				//On detruit l'exception (si toute fois il y en a une)
				if (pluginDesc->Exception != nullptr)
				{
					delete pluginDesc->Exception;
				}
				delete pluginDesc;
			}
			//Une fois que tous les items ont été détruit
			//On vide la liste, puis on la détruit
			_pluginsDesc->Clear();
			delete _pluginsDesc;
			//On déchargé l'AppDomain, puis on le détruit
			AppDomain::Unload(domain);
			//Enfin on supprime Cette instance de la collection
			domains->Remove(this);
		}
	}

	Boolean DomainInformation::UnloadPlugin(PluginDescription^ PluginDesc)
	{
		//Indique que le plugin n'est plus chargé
		PluginDesc->Loaded = false;
		_pluginsDesc->Remove(PluginDesc);
		try
		{
			//Enleve tous les hook
			PluginDesc->RemontingProxy->UnHookAll();
			//On décharge le plugin associé 
			PluginDesc->Plugin->Unload();
			//Et on appel son desctructeur
			delete PluginDesc->Plugin;
		}
		catch (Exception^ ex)
		{
			//Lorsqu'une erreur se produit durant l'appel a la methode "Unload"
			MessageBox::Show("An Unhandled exception occured during the execution of the ""Unload"" or ""Dispose"" subroutine.\nThe plugin has been unloaded anyway.\nPlease report to your associated code in order to handle it.\n\nMore Informations :\nMessage : " + ex->Message,"Xchat DotNet Managed Manager",MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		//On detruit l'exception (si toute fois il y en a une)
		if (PluginDesc->Exception != nullptr)
		{
			delete PluginDesc->Exception;
		}
		//On detruit aussi la structure de description
		delete PluginDesc;

		if(_pluginsDesc->Count == 0)
		{
			//Si la liste des plugins est alors vide, on le signal afin de permettre sa desctruction
			bEmpty = true;
		}
		return bEmpty;
	}
	Void DomainInformation::UnloadAll()
	{
		if(domains != nullptr)
		{
			//Si la liste n'est pas vide alors on prend les instance une à une
			int nIndex;
			for(nIndex = domains->Count - 1 ; nIndex >=0; nIndex--)
			{
				if(domains[nIndex] != nullptr)
				{
					//et si chaque domain existe alors on le detruis
					delete domains[nIndex];
				}
			}
			//On vide la liste
			domains->Clear();
		}
	}
	List<DomainInformation^>^ DomainInformation::GetDomains()
	{
		//Assesceur de la liste des Classe "DomainInformation"
		//Permet à tout instant d'avoir une énumeration complète de ce qui a été fait
		return domains;
	}

	List<PluginDescription^>^ DomainInformation::PluginsDescription::get()
	{
		return this->_pluginsDesc;
	}
	String^ DomainInformation::AssemblyFile::get()
	{
		//Renvoi le chemin complet de la dll chargé dans le Domain
		return Path::GetFileName(CurrentAssembly);
	}
	Boolean DomainInformation::Empty::get()
	{
		//Informe les autre classe si ce Domain est vide ou non
		//Auquel cas il peux etre non traité et/ou détruit
		return this->bEmpty;
	}

	Void DomainInformation::InitLoad(PluginDescription^ PluginDesc)
	{
		LoadState^ state = gcnew LoadState();
		//Instancie l'objet "AutoResetEvent" qui permet de controler la durée d'execution
		LoadAutoEvent = gcnew AutoResetEvent(false);
		state->AutoEvent = LoadAutoEvent;	//Objet de surveillance du TimeOut
		state->PluginDesc = PluginDesc;		//Description du plugin
		state->ExceptionOccured = true;		//Par default on se met dans un cas ou il y a un probleme

		//Ajoute a la pile la tache décrite par le "WaitCallBack" (methode ciblé) et lui passe en objet l'AutoReset
		ThreadPool::QueueUserWorkItem(gcnew WaitCallback(this, &DomainInformation::LoadCallBack), state);

		//Execute le prochain item sur la pile avec 60 secondes de delai d'execution
		if (LoadAutoEvent->WaitOne(60000, false))
		{
			//Si le travail a été réalisé en moins de 60 secondes alors
			if(state->ExceptionOccured == true)
			{
				//Si une erreur s'est produite
				//Lorsqu'une erreur se produit durant l'appel a la methode "Load"
				MessageBox::Show("An Unhandled exception occured during the execution of the EntryPoint code.\nThe plugin has not been unloaded an could keep working.\nSee your code in ""Load"" function.\nYou should add a ""try catch"" section to handle it.","Xchat DotNet Managed Manager",MessageBoxButtons::OK, MessageBoxIcon::Error);
				//Décharge le plugin selectionné
				this->UnloadPlugin(PluginDesc);
			}
			else
			{
				//Si il n'y a pas eu d'erreur
				//Indique via la description que ce plugin a été chargé
				PluginDesc->Loaded = true;
				//A partir de là, la classe de gestion de l'AppDomain "n'est plus vide" <=> "contient au moins un plugin chargé"
				this->bEmpty = false;
			}
		}
		else
		{
			//Si la procédure "Load" a mit plus de 60 secondes a s'executer alors
			//Décharge le plugin selectionné
			this->UnloadPlugin(PluginDesc);
		}
	}

	Void DomainInformation::LoadCallBack(Object^ StateObject)
	{
		//Récupère l'objet passé au travers de la structure
		LoadState^ state = (LoadState^)StateObject;
		try
		{
			//Tente d'appeler la méthode "Load" du plugin au travers du proxy
			//en lui passant en parametre la classe de remoting
			//A REVOIR
			RemoteOperations^ _remoteOperations = gcnew RemoteOperations();
			state->PluginDesc->RemontingProxy = (IRemoting^)_remoteOperations;
			state->PluginDesc->Plugin->Load((IRemoting^)_remoteOperations);
			//Pas eu d'erreur
			state->ExceptionOccured =  false;
			//Indique a l'objet de TimeOut que le traitement est fini et qu'il n'est pas la peine de signaler le Timeout
			state->AutoEvent->Set();
		}
		catch (Exception^ ex)
		{
			//Signalement de l'erreur
			state->ExceptionOccured =  true;
			state->PluginDesc->Exception = ex;
			//Indique a l'objet de TimeOut qu'il n'a plus besoin d'attendre on sait que ca s'est planté
			state->AutoEvent->Set();
		}
	}
};