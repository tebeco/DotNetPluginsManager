#pragma once
#include "XChatInterop.h"
#include "RemoteOperations.h"
#include "FileLock.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Reflection;
using namespace System::Threading;
using namespace PluginInterface;

namespace XChatInterop {

	public ref class DomainInformation : public System::IDisposable
	{
	public :
		DomainInformation(String^);
		~DomainInformation();
		static List<DomainInformation^>^ GetDomains();
	internal:
		property List<PluginDescription^>^ PluginsDescription
		{
			List<PluginDescription^>^ get();
		}
		property String^ AssemblyFile
		{
			String^ get();
		}
		property Boolean Empty
		{
			Boolean get();
		}	
		Boolean UnloadPlugin(PluginDescription^ PluginDesc);//Decharge le plugin specifié
		static Void UnloadAll();							//Decharge tous les plugin du domaine
		Void InitLoad(PluginDescription^ PluginDesc);		//Prepare le chargement du plugin
		Void LoadCallBack(Object^ StateObject);				//Sert a chargé le plugin
		ref struct LoadState				//Objet à passer lors de la tentative de "Load" car il stocke a la fois
		{
		public :
			AutoResetEvent^ AutoEvent;		//Le Reset afin de pouvoir signaler que le traitement est fini
			PluginDescription^ PluginDesc;	//Et la description du plugin pour les opérations
			Boolean ExceptionOccured;		//Un erreur s'est produite pendant le "Load"
		};
	private:
		static List<DomainInformation^>^ domains = nullptr;	//Liste statique listant tous les "DomainInfo" en cour
		List<PluginDescription^>^ _pluginsDesc;				//Liste de description de chaque plugin eligible
		AppDomain^ domain;									//AppDomain cible dans laquelle va etre chargé la dll
		AutoResetEvent^ LoadAutoEvent;						//Utiliser pour controler le temps d'execution du "Load" du plugin
		PluginFactory^ factory;								//Classe de chargement des plugin dans l'AppDomain
		String^ CurrentAssembly;							//Chaine contenant le nom de la dll chargé
		Boolean bEmpty;										//Indique ci ce descripteur d'AppDomain est vide ou non
		FileLock^ _fileLock;								//Classe permettant de blocké le fichier
	};
}
