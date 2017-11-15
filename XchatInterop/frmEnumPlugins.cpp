#include "StdAfx.h"
#include "frmEnumPlugins.h"

using namespace PluginInterface;

namespace XChatInterop
{
	List<PluginDescription^>^ frmEnumPlugins::PluginsDescription::get()
	{
		//Remplis les valeurs "Requested" de la liste des "PluginDescription" si ceux ci sont coché ou non
		for(int nIndex = 0; nIndex < cklPlugins->Items->Count; nIndex++)
		{
			switch(cklPlugins->GetItemCheckState(nIndex))
			{
			case CheckState::Checked:
				_PluginsDesc[nIndex]->Requested = true;
				break;
			case CheckState::Unchecked:
			case CheckState::Indeterminate:
				_PluginsDesc[nIndex]->Requested = false;
				break;
			}
		}
		return _PluginsDesc;
	}

	void frmEnumPlugins::PluginsDescription::set(List<PluginDescription^>^ value)
	{
		//Attribut une liste de "PluginDescription" et coche les case en correspondance avec la valeur du champs "Requested"
		_PluginsDesc = value;
		cklPlugins->Items->Clear();
		for each(PluginDescription^ tmpPlugDesc in _PluginsDesc)
		{
			cklPlugins->Items->Add(tmpPlugDesc->FullName, tmpPlugDesc->Requested);
		}
	}
};