// XChatInterop.h

#pragma once
#include "xchat-plugin.h"

using namespace System;
using namespace System::Windows::Forms;

//Assesceur permettant l'acces a la variable "ph" qui contient le handle du plugin pour Xchat
xchat_plugin * GetPh();

namespace XChatInterop {
	//Classe permettant de recupérer le handle de la fentre de xchat pour ouvrir "frmManager" avec Xchat comme parent
	public ref class MainWindowWrapper : public IWin32Window
	{
	public:
		// TODO : ajoutez ici vos méthodes pour cette classe.
		//Constructeur de la classe initialisant le handle de la fenetre de xchat
		MainWindowWrapper(IntPtr handle)
		{
			_hwnd = handle;
		}
		//Implémente la propriété "Handle" de "IWin32Window" permettant de spécifie le "owner" dans l'appel a ".Show(owner)" 
		virtual property IntPtr Handle {
			IntPtr get ()
			{
				return _hwnd;
			}
		}

	private:
		//Handle de la fenetre de Xchat
		IntPtr _hwnd;
	};

}

