#pragma once

using namespace System;
using namespace System::IO;

namespace XChatInterop {

	public ref class FileLock : public System::IDisposable
	{
	public:
	internal:
		FileLock(String^ FileName);
		~FileLock();
		Boolean Lock();			//Tente de verouiller le fichier
		void UnLock();			//Deverouille le fichier
	private:
		String^ _sFileName;		//Fichier a verouiller
		FileStream^ fs;			//Flux permettant de verouiller le fichier
	};
}