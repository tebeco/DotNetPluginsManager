#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace PluginInterface;

namespace XChatInterop
{
	/// <summary>
	/// Summary for frmEnumPlugins
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class frmEnumPlugins : public System::Windows::Forms::Form
	{
	public:
		frmEnumPlugins(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmEnumPlugins()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::CheckedListBox^  cklPlugins;
	protected: 

	private: System::Windows::Forms::Button^  btnOk;



	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->cklPlugins = (gcnew System::Windows::Forms::CheckedListBox());
			this->btnOk = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// cklPlugins
			// 
			this->cklPlugins->Dock = System::Windows::Forms::DockStyle::Top;
			this->cklPlugins->FormattingEnabled = true;
			this->cklPlugins->Location = System::Drawing::Point(0, 0);
			this->cklPlugins->Name = L"cklPlugins";
			this->cklPlugins->Size = System::Drawing::Size(346, 109);
			this->cklPlugins->TabIndex = 1;
			// 
			// btnOk
			// 
			this->btnOk->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btnOk->Location = System::Drawing::Point(259, 115);
			this->btnOk->Name = L"btnOk";
			this->btnOk->Size = System::Drawing::Size(75, 23);
			this->btnOk->TabIndex = 0;
			this->btnOk->Text = L"&Ok";
			this->btnOk->UseVisualStyleBackColor = true;
			// 
			// frmEnumPlugins
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(346, 142);
			this->Controls->Add(this->btnOk);
			this->Controls->Add(this->cklPlugins);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"frmEnumPlugins";
			this->Text = L"List of available pluigns for this file";
			this->ResumeLayout(false);

		}
#pragma endregion

	internal:
		property List<PluginDescription^>^ PluginsDescription
		{
			List<PluginDescription^>^ get();
			void set(List<PluginDescription^>^ value);
		}
	private:
		List<PluginDescription^>^ _PluginsDesc;
};
}
