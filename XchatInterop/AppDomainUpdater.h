#pragma once

using namespace System;
using namespace System::Reflection;

namespace XChatInterop {
	public ref class AppDomainUpdater
	{
	private:
		AppDomain^ _appDomain;
	internal:
		AppDomainUpdater(AppDomain^ appDomain);

		property String^ BaseDirectory
		{
			String^ get();
			void set(String^ value);
		}
		Object^ CallInstanceMethod(Object^ obj, String^ methodName, array<Object^>^ args);
		Object^ CallStaticMethod(Type^ t, String^ methodName, array<Object^>^ args);
		Object^ AppDomainUpdater::GetStaticProperty(Type^ t, String^ propName);
		Object^ ReadInstanceField(Object^ obj, String^ fieldName);
		void SetInstanceProperty(Object^ obj, String^ propName, Object^ newValue);
		void WriteInstanceField(Object^ obj, String^ fieldName, Object^ newValue);
	};

}