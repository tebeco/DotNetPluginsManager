#pragma once
#include "XChatInterop.h"
#include "DomainInformation.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Reflection;
using namespace PluginInterface;

namespace XChatInterop {

	/// <summary>
	/// Description résumée de frmManager
	///
	/// AVERTISSEMENT : si vous modifiez le nom de cette classe, vous devrez modifier la
	///          propriété 'Nom du fichier de ressources' de l'outil de compilation de ressource managée
	///          pour tous les fichiers .resx dont dépend cette classe. Dans le cas contraire,
	///          les concepteurs ne pourront pas interagir correctement avec les ressources
	///          localisées associées à ce formulaire.
	/// </summary>
	public ref class frmManager : public System::Windows::Forms::Form
	{
	public:
		frmManager(void)
		{
			InitializeComponent();
			//
			//TODO : ajoutez ici le code du constructeur
			//
		}

	protected:
		/// <summary>
		/// Nettoyage des ressources utilisées.
		/// </summary>
		~frmManager()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 
	private: System::Windows::Forms::Button^  btnLoad;
	private: System::Windows::Forms::Button^  btnUnload;
	private: System::Windows::Forms::Button^  btnOptions;
	private: System::Windows::Forms::Button^  btnClose;
	private: System::Windows::Forms::ListView^  lsvLoadedPlugins;
	private: System::Windows::Forms::ColumnHeader^  clhName;
	private: System::Windows::Forms::ColumnHeader^  clhVersion;
	private: System::Windows::Forms::ColumnHeader^  clhFile;
	private: System::Windows::Forms::ColumnHeader^  clhDescription;
	private: System::Windows::Forms::OpenFileDialog^  ofdPlugin;
	private: System::Windows::Forms::ColumnHeader^  clhNameSpace;

	private:
		/// <summary>
		/// Variable nécessaire au concepteur.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
		/// le contenu de cette méthode avec l'éditeur de code.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnLoad = (gcnew System::Windows::Forms::Button());
			this->btnUnload = (gcnew System::Windows::Forms::Button());
			this->btnOptions = (gcnew System::Windows::Forms::Button());
			this->btnClose = (gcnew System::Windows::Forms::Button());
			this->lsvLoadedPlugins = (gcnew System::Windows::Forms::ListView());
			this->clhName = (gcnew System::Windows::Forms::ColumnHeader());
			this->clhVersion = (gcnew System::Windows::Forms::ColumnHeader());
			this->clhFile = (gcnew System::Windows::Forms::ColumnHeader());
			this->clhNameSpace = (gcnew System::Windows::Forms::ColumnHeader());
			this->clhDescription = (gcnew System::Windows::Forms::ColumnHeader());
			this->ofdPlugin = (gcnew System::Windows::Forms::OpenFileDialog());
			this->SuspendLayout();
			// 
			// btnLoad
			// 
			this->btnLoad->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->btnLoad->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btnLoad->Location = System::Drawing::Point(120, 146);
			this->btnLoad->Name = L"btnLoad";
			this->btnLoad->Size = System::Drawing::Size(75, 23);
			this->btnLoad->TabIndex = 0;
			this->btnLoad->Text = L"&Load";
			this->btnLoad->UseVisualStyleBackColor = true;
			this->btnLoad->Click += gcnew System::EventHandler(this, &frmManager::btnLoad_Click);
			// 
			// btnUnload
			// 
			this->btnUnload->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->btnUnload->Enabled = false;
			this->btnUnload->Location = System::Drawing::Point(201, 146);
			this->btnUnload->Name = L"btnUnload";
			this->btnUnload->Size = System::Drawing::Size(75, 23);
			this->btnUnload->TabIndex = 1;
			this->btnUnload->Text = L"&Unload";
			this->btnUnload->UseVisualStyleBackColor = true;
			this->btnUnload->Click += gcnew System::EventHandler(this, &frmManager::btnUnload_Click);
			// 
			// btnOptions
			// 
			this->btnOptions->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->btnOptions->Enabled = false;
			this->btnOptions->Location = System::Drawing::Point(282, 146);
			this->btnOptions->Name = L"btnOptions";
			this->btnOptions->Size = System::Drawing::Size(75, 23);
			this->btnOptions->TabIndex = 2;
			this->btnOptions->Text = L"&Options";
			this->btnOptions->UseVisualStyleBackColor = true;
			this->btnOptions->Click += gcnew System::EventHandler(this, &frmManager::btnOptions_Click);
			// 
			// btnClose
			// 
			this->btnClose->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->btnClose->Location = System::Drawing::Point(363, 146);
			this->btnClose->Name = L"btnClose";
			this->btnClose->Size = System::Drawing::Size(75, 23);
			this->btnClose->TabIndex = 3;
			this->btnClose->Text = L"&Close";
			this->btnClose->UseVisualStyleBackColor = true;
			this->btnClose->Click += gcnew System::EventHandler(this, &frmManager::btnClose_Click);
			// 
			// lsvLoadedPlugins
			// 
			this->lsvLoadedPlugins->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->lsvLoadedPlugins->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(5) {this->clhName, this->clhVersion, 
				this->clhFile, this->clhNameSpace, this->clhDescription});
			this->lsvLoadedPlugins->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->lsvLoadedPlugins->FullRowSelect = true;
			this->lsvLoadedPlugins->GridLines = true;
			this->lsvLoadedPlugins->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->lsvLoadedPlugins->Location = System::Drawing::Point(0, 0);
			this->lsvLoadedPlugins->MultiSelect = false;
			this->lsvLoadedPlugins->Name = L"lsvLoadedPlugins";
			this->lsvLoadedPlugins->Size = System::Drawing::Size(559, 140);
			this->lsvLoadedPlugins->TabIndex = 4;
			this->lsvLoadedPlugins->UseCompatibleStateImageBehavior = false;
			this->lsvLoadedPlugins->View = System::Windows::Forms::View::Details;
			this->lsvLoadedPlugins->SelectedIndexChanged += gcnew System::EventHandler(this, &frmManager::lsvLoadedPlugins_SelectedIndexChanged);
			// 
			// clhName
			// 
			this->clhName->Text = L"Name";
			this->clhName->Width = 69;
			// 
			// clhVersion
			// 
			this->clhVersion->Text = L"Version";
			this->clhVersion->Width = 51;
			// 
			// clhFile
			// 
			this->clhFile->Text = L"File";
			// 
			// clhNameSpace
			// 
			this->clhNameSpace->Text = L"Namespace";
			this->clhNameSpace->Width = 137;
			// 
			// clhDescription
			// 
			this->clhDescription->Text = L"Description";
			this->clhDescription->Width = 238;
			// 
			// ofdPlugin
			// 
			this->ofdPlugin->Filter = L"Assembly file (*.dll)|*.dll";
			this->ofdPlugin->Title = L"Select a plugin to load";
			// 
			// frmManager
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(559, 176);
			this->Controls->Add(this->lsvLoadedPlugins);
			this->Controls->Add(this->btnClose);
			this->Controls->Add(this->btnOptions);
			this->Controls->Add(this->btnUnload);
			this->Controls->Add(this->btnLoad);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->Name = L"frmManager";
			this->ShowInTaskbar = false;
			this->Text = L"DoNet Plugin Manager";
			this->Load += gcnew System::EventHandler(this, &frmManager::frmManager_Load);
			this->Resize += gcnew System::EventHandler(this, &frmManager::frmManager_Resize);
			this->ResumeLayout(false);

		}
#pragma endregion

private: 
		System::Void frmManager_Load(System::Object^  sender, System::EventArgs^  e);
		System::Void frmManager_Resize(System::Object^  sender, System::EventArgs^  e);
		System::Void btnLoad_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btnUnload_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void btnClose_Click(System::Object^  sender, System::EventArgs^  e);
		System::Void lsvLoadedPlugins_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
		System::Void LoadAssembly(System::String^ Assembly);
		System::Void UpdateList();
		System::Void btnOptions_Click(System::Object^  sender, System::EventArgs^  e);

		ref struct ItemDescription
		{
		public:
			DomainInformation^ DomainInfo;
			PluginDescription^ PlugDesc;
		};

};
}
