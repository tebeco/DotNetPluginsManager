#include "StdAfx.h"
#include "frmManager.h"
#include "DomainInformation.h"
#include "FileLock.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace PluginInterface;

namespace XChatInterop {
	System::Void frmManager::frmManager_Load(System::Object^  sender, System::EventArgs^  e)
	{
		//Appel la procédure de mise à jour de la liste car des pluggin on peut etre été chargé et ce formulaire refermé
		this->UpdateList();
	}
	System::Void frmManager::frmManager_Resize(System::Object^  sender, System::EventArgs^  e)
	{
		int tmpWidth;

		//Redimensionne la premiere colonne en fonction de son entête
		//Colonne : Name
		lsvLoadedPlugins->AutoResizeColumn(0, System::Windows::Forms::ColumnHeaderAutoResizeStyle::HeaderSize);
		tmpWidth = lsvLoadedPlugins->Columns[0]->Width; //Mémorise la largeur
		//Redimensionne la premiere colonne en fonction de son contenu
		lsvLoadedPlugins->AutoResizeColumn(0, System::Windows::Forms::ColumnHeaderAutoResizeStyle::ColumnContent);
		if(tmpWidth > lsvLoadedPlugins->Columns[0]->Width)
		{
			//Adapte la largeur de la colonne a la plus grande valeure des deux
			lsvLoadedPlugins->Columns[0]->Width = tmpWidth;
		}

		//Idem pour la deuxieme colonne
		//Colonne : Version
		lsvLoadedPlugins->AutoResizeColumn(1, System::Windows::Forms::ColumnHeaderAutoResizeStyle::ColumnContent);
		tmpWidth = lsvLoadedPlugins->Columns[1]->Width;
		lsvLoadedPlugins->AutoResizeColumn(1, System::Windows::Forms::ColumnHeaderAutoResizeStyle::HeaderSize);
		if(tmpWidth > lsvLoadedPlugins->Columns[1]->Width)
		{
			lsvLoadedPlugins->Columns[1]->Width = tmpWidth;
		}

		//Idem pour la troisieme colonne
		//Colonne : File
		lsvLoadedPlugins->AutoResizeColumn(2, System::Windows::Forms::ColumnHeaderAutoResizeStyle::HeaderSize);
		tmpWidth = lsvLoadedPlugins->Columns[2]->Width; //Mémorise la largeur
		lsvLoadedPlugins->AutoResizeColumn(2, System::Windows::Forms::ColumnHeaderAutoResizeStyle::ColumnContent);
		if(tmpWidth > lsvLoadedPlugins->Columns[2]->Width)
		{
			lsvLoadedPlugins->Columns[2]->Width = tmpWidth;
		}

		//Idem pour la quatrieme colonne
		//Colonne : Namespace
		lsvLoadedPlugins->AutoResizeColumn(3, System::Windows::Forms::ColumnHeaderAutoResizeStyle::HeaderSize);
		tmpWidth = lsvLoadedPlugins->Columns[3]->Width; //Mémorise la largeur
		lsvLoadedPlugins->AutoResizeColumn(3, System::Windows::Forms::ColumnHeaderAutoResizeStyle::ColumnContent);
		if(tmpWidth > lsvLoadedPlugins->Columns[3]->Width)
		{
			lsvLoadedPlugins->Columns[3]->Width = tmpWidth;
		}

		//Idem pour la cinquieme colonne
		//Colonne : Description
		lsvLoadedPlugins->AutoResizeColumn(4, System::Windows::Forms::ColumnHeaderAutoResizeStyle::HeaderSize);
		tmpWidth = lsvLoadedPlugins->Columns[4]->Width; //Mémorise la largeur
		lsvLoadedPlugins->AutoResizeColumn(4, System::Windows::Forms::ColumnHeaderAutoResizeStyle::ColumnContent);
		if(tmpWidth > lsvLoadedPlugins->Columns[4]->Width)
		{
			lsvLoadedPlugins->Columns[4]->Width = tmpWidth;
		}

		//puis adapte la derniere colonne a la taille du formulaire
		if( ( lsvLoadedPlugins->Columns[0]->Width
			+ lsvLoadedPlugins->Columns[1]->Width
			+ lsvLoadedPlugins->Columns[2]->Width
			+ lsvLoadedPlugins->Columns[3]->Width
			+ lsvLoadedPlugins->Columns[4]->Width) < lsvLoadedPlugins->ClientSize.Width)
		{
			lsvLoadedPlugins->Columns[4]->Width = lsvLoadedPlugins->ClientSize.Width - 
												  ( lsvLoadedPlugins->Columns[0]->Width
												  + lsvLoadedPlugins->Columns[1]->Width
												  + lsvLoadedPlugins->Columns[2]->Width
												  + lsvLoadedPlugins->Columns[3]->Width);
		}
	}

	System::Void frmManager::btnLoad_Click(System::Object^  sender, System::EventArgs^  e)
	{
		//Si qqun clique sur le bouton "Load"
		System::Windows::Forms::DialogResult result;

		//Ouvre un selecteur de fichier
		result = ofdPlugin->ShowDialog();
		if(result != ::DialogResult::OK)
		{
			//Si il n'appuis pas sur "OK" on sort
			return;
		}
		//Sinon on appelle la procedure de chargement avec le nom du fichier complet
		this->LoadAssembly(ofdPlugin->FileName);
	}
	System::Void frmManager::btnUnload_Click(System::Object^  sender, System::EventArgs^  e)
	{
		//Recupere les Info de l'item
		ItemDescription^ itemTag = (ItemDescription^)(lsvLoadedPlugins->SelectedItems[0]->Tag);
		//Récupère la description du plugin
		PluginDescription^ tmpPlugDesc = itemTag->PlugDesc;
		//Récupère le gestionnaire de l'AppDomain associé au plugin
		DomainInformation^ tmpDomainInfo = itemTag->DomainInfo;

		//Décharge le plugin selectionné
		if(tmpDomainInfo->UnloadPlugin(tmpPlugDesc))
		{
			//Si le DomainInformation est vide suite à cela alors on le detruit
			delete tmpDomainInfo;
		}
		//Met à jour la liste
		this->UpdateList();
	}
	System::Void frmManager::btnOptions_Click(System::Object^  sender, System::EventArgs^  e)
	{
			 /*
			 *
			 *A REVOIR : Du code à changer ?
			 *
			 */
		//Recupere les Info de l'item
		ItemDescription^ itemTag = (ItemDescription^)(lsvLoadedPlugins->SelectedItems[0]->Tag);
		//Récupère la description du plugin
		PluginDescription^ tmpPlugDesc = itemTag->PlugDesc;
		//Récupère le gestionnaire de l'AppDomain associé au plugin
		DomainInformation^ tmpDomainInfo = itemTag->DomainInfo;

		((IOptions^)(tmpPlugDesc->Plugin))->ShowDialog();

	}
	System::Void frmManager::btnClose_Click(System::Object^  sender, System::EventArgs^  e)
	{
		//Ferme le formulaire
		 this->Close();
	}
	System::Void frmManager::lsvLoadedPlugins_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		//Lorsque la ligne de selection dans la liste change
		if(lsvLoadedPlugins->SelectedItems->Count > 0)
		{
			//Si au moins 1 item est sélectionné alors
			//On activé le bouton "Unload" qui permet de le dechargé
			this->btnUnload->Enabled = true;
			//Et on activé le bouton "Options" si la classe implémente l'interface "IOptions"
			//On en est informé via le biais du membre "HasOptions" issue de la description du plugin stocké dans le "Tag"
			this->btnOptions->Enabled = ((ItemDescription^)(lsvLoadedPlugins->SelectedItems[0]->Tag))->PlugDesc->HasOptions;
		}
		else
		{
			//Si rien est selectionné on desactive les 2 boutons
			this->btnUnload->Enabled = false;
			this->btnOptions->Enabled = false;
		}
	}
	System::Void frmManager::LoadAssembly(String^ assembly)
	{
		//Instancie un gestionnaire d'AppDomain en specifiant le chemin de la dll a chargé
		DomainInformation^ domain = gcnew DomainInformation(assembly);
		if(domain->Empty == (Boolean)true)
		{
			//Si le domain resultant est vide on le detruit et on quitte la procédure
			delete domain;
			return;
		}
		//Quand un plugin s'est chargé on met a jour la listebox
		this->UpdateList();
	}

	System::Void frmManager::UpdateList()
	{
		List<int>^ lstRemoveRequested;
		//Reset le contenu de la liste
		lsvLoadedPlugins->Items->Clear();
		//Récupère la liste des domaines créés
		List<DomainInformation^>^ domains = DomainInformation::GetDomains();
		if (domains != nullptr)
		{
			//On instancie la liste que si il existe des classes sinon ce n'est pas la peine
			lstRemoveRequested = gcnew List<int>();
			//Si elle est pas vide alors on boucle dessus et on les prend un par un
			//Pour chaque domaine on enumère les plugins à l'interieur

			for each(DomainInformation^ domain in domains)
			{
				if(!(domain->Empty))
				{
					//Si le domain n'est pas vide alors
					for each(PluginDescription^ plugDesc in domain->PluginsDescription)
					{
						if(plugDesc->Loaded)
						{
							//et pour chaqun d'eux, et à condition qu'ils soient chargés
							//On créé une structure contenant les info utile lorsque l'utilisateur selectionne une ligne
							ItemDescription^ itemTag = gcnew ItemDescription();
							itemTag->DomainInfo = domain;		//On indique le domain conteneur
							itemTag->PlugDesc = plugDesc;		//Et la description du plugin

							//on créé une ligne en spécifiant
							ListViewItem^ item = gcnew ListViewItem();

							item->SubItems[0]->Text = plugDesc->Plugin->Name;				//Son Nom
							item->SubItems->Add(plugDesc->Plugin->Version);					//Sa Version
							item->SubItems->Add(Path::GetFileName(plugDesc->AssemblyName));	//Son fichier dll correspondant
							item->SubItems->Add(plugDesc->FullName);						//Son NameSpace
							item->SubItems->Add(plugDesc->Plugin->Description);				//Sa description
							item->Tag = itemTag;											//Puis on fait correspondre au "Tag" les infos du plugin
							lsvLoadedPlugins->Items->Add(item);								//On rajoute enfin cette ligne a la liste
						}
					}
				}
				else 
				{
					//On releve le numéro de l'index de l'item vide pour une suppresion ulterieur car on ne peux pas faire de remove pendant un for each()
					lstRemoveRequested->Add(domains->IndexOf(domain));
				}
			}

			//Puis ont detruit tous les items listé comme étant vides
			for each(int nIndex in lstRemoveRequested)
			{
				try {
					delete domains[nIndex];
				}
				catch (Exception^ ex)
				{
					//Erreur lors de l'a suppression d'un domaine vide
					//A il deja été enlevé de la liste ?
					throw ex;
				}
			}
			//et on detruit la liste
			lstRemoveRequested->Clear();
			delete lstRemoveRequested;

			//Ensuite on redimensionne les colonnes pour que cela soit lisible
			frmManager_Resize(this, EventArgs::Empty);
		}
		Application::DoEvents();
	}
};