#include "stdafx.h"
#include "AppDomainUpdater.h"

using namespace System;
using namespace System::Reflection;

namespace XChatInterop {

	AppDomainUpdater::AppDomainUpdater(AppDomain^ appDomain) {
		this->_appDomain = appDomain;
	}

	String^ AppDomainUpdater::BaseDirectory::get()
	{
		return this->_appDomain->BaseDirectory;
	}

	void AppDomainUpdater::BaseDirectory::set(String^ value)
	{
		/*
		 * Permet de modifier la propriété "BaseDirectory" du domain d'application passer au constructeur
		 * Celle ci est normalement en lecture seul et vu que l'AppDomain n'est pas créé par nous 
		 * on ne peux pas le modifié a sa création
		*/

		IntPtr fusionContext = (IntPtr)this->CallInstanceMethod(_appDomain, "GetFusionContext", nullptr);
		Object^ APPBASEKEY = this->GetStaticProperty(AppDomainSetup::typeid, "ApplicationBaseKey");
		this->CallStaticMethod(AppDomainSetup::typeid, "UpdateContextProperty", gcnew array<Object^>(3){fusionContext, APPBASEKEY, value});

		AppDomainSetup^ adSetup = ((AppDomainSetup^)(this->ReadInstanceField(_appDomain, "_FusionStore")));
		this->SetInstanceProperty(adSetup, "ApplicationBase", value);
		this->WriteInstanceField(adSetup, "_AppBase", value);
	}

	Object^ AppDomainUpdater::CallInstanceMethod(Object^ obj, String^ methodName, array<Object^>^ args )
	{
		MethodInfo^ mi = obj->GetType()->GetMethod(methodName, BindingFlags::Public | BindingFlags::NonPublic | BindingFlags::Instance);
		return mi->Invoke(obj, args);
	}

	Object^ AppDomainUpdater::CallStaticMethod(Type^ t, String^ methodName, array<Object^>^ args)
	{
		MethodInfo^ mi = t->GetMethod(methodName, BindingFlags::Public | BindingFlags::NonPublic | BindingFlags::Static);
        return mi->Invoke(nullptr, args);
	}

	Object^ AppDomainUpdater::GetStaticProperty(Type^ t, String^ propName)
	{
		PropertyInfo^ pi = t->GetProperty(propName, BindingFlags::Public | BindingFlags::NonPublic | BindingFlags::Static);
        return pi->GetValue(nullptr, nullptr);
	}
	Object^ AppDomainUpdater::ReadInstanceField(Object^ obj, String^ fieldName)
	{
		FieldInfo^ fi = obj->GetType()->GetField(fieldName, BindingFlags::Public | BindingFlags::NonPublic | BindingFlags::Instance);
		return fi->GetValue(obj);

	}
	void AppDomainUpdater::SetInstanceProperty(Object^ obj, String^ propName, Object^ newValue)
	{
		PropertyInfo^ pi = obj->GetType()->GetProperty(propName, BindingFlags::Public | BindingFlags::NonPublic | BindingFlags::Instance);
		pi->SetValue(obj, newValue, nullptr);
	}
	void AppDomainUpdater::WriteInstanceField(Object^ obj, String^ fieldName, Object^ newValue)
	{
		FieldInfo^ fi = obj->GetType()->GetField(fieldName, BindingFlags::Public | BindingFlags::NonPublic | BindingFlags::Instance);
		fi->SetValue(obj, newValue);
	}
}