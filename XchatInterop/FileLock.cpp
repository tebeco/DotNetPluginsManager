#include "StdAfx.h"
#include "FileLock.h"

using namespace System;
using namespace System::IO;
using namespace System::Security;
using namespace System::Windows::Forms;

namespace XChatInterop {

	FileLock::FileLock(String^ FileName)
	{
		//Stocke le nil du fichier a verouillé
		_sFileName = FileName;
	}

	FileLock::~FileLock()
	{
		//Appelle la methode poure deverouillé le fichier
		this->UnLock();
		delete _sFileName;	
	}

	Boolean FileLock::Lock()
	{
		//Si le fichier existe
		if(File::Exists(_sFileName)) 
		{
			try
			{
				//On tente de créé un stream sur le fichier en autorisant que la lecture depuis les autre application
				//Ca a donc pour effet de verouillé tte modification renomage ect ...
				fs = gcnew FileStream(_sFileName, FileMode::Open, FileAccess::Read, FileShare::Read);
				//Operation réussi
				return true;
			}
			catch (ArgumentException^ ex)
			{
				//ArgumentException path refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in an NTFS environment.
				MessageBox::Show("The file :" + Environment::NewLine + _sFileName + Environment::NewLine + "refers to a non-file device and can't be used.","Xchat DotNet Managed Manager", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			catch (NotSupportedException^ ex)
			{
				//NotSupportedException path refers to a non-file device, such as "con:", "com1:", "lpt1:", etc. in a non-NTFS environment.
				MessageBox::Show("The file :" + Environment::NewLine + _sFileName + Environment::NewLine + "refers to a non-file device and can't be used.","Xchat DotNet Managed Manager", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			catch (FileNotFoundException^ ex)
			{
				//FileNotFoundException The file cannot be found, such as when mode is FileMode.Truncate or FileMode.Open, and the file specified by path does not exist. The file must already exist in these modes. 
				MessageBox::Show("The following file cannot be found :" + Environment::NewLine + _sFileName + Environment::NewLine + "Please check that your specified file path is correct.","Xchat DotNet Managed Manager", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			catch (DirectoryNotFoundException^ ex)
			{
				//DirectoryNotFoundException The specified path is invalid, such as being on an unmapped drive. 
				MessageBox::Show("The specified path is invalid, such as being on an unmapped drive." + Environment::NewLine + "File path :" + Environment::NewLine + _sFileName,"Xchat DotNet Managed Manager", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			catch (PathTooLongException^ ex)
			{
				MessageBox::Show("The specified path, file name, or both exceed the system-defined maximum length." + Environment::NewLine + "For example, on Windows-based platforms, paths must be less than 248 characters, and file names must be less than 260 characters.","Xchat DotNet Managed Manager", MessageBoxButtons::OK, MessageBoxIcon::Error);
				//PathTooLongException The specified path, file name, or both exceed the system-defined maximum length. For example, on Windows-based platforms, paths must be less than 248 characters, and file names must be less than 260 characters. 
			}
			catch (IOException^ ex)
			{
				//IOException An I/O error occurs, such as specifying FileMode.CreateNew and the file specified by path already exists. 
				MessageBox::Show("An In/Out error occurs while loading the file. Please try again. If the problem persists please try with another file.","Xchat DotNet Managed Manager", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			catch (SecurityException^ ex)
			{
				//SecurityException The caller does not have the required permission. 
				MessageBox::Show("You don't have the required permission to do this.","Xchat DotNet Managed Manager", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			catch (UnauthorizedAccessException^ ex)
			{
				MessageBox::Show("The access requested is not permitted by the operating system for the specified path :" + Environment::NewLine + _sFileName,"Xchat DotNet Managed Manager", MessageBoxButtons::OK, MessageBoxIcon::Error);
				//UnauthorizedAccessException The access requested is not permitted by the operating system for the specified path, such as when access is Write or ReadWrite and the file or directory is set for read-only access. 
			}
			//Une erreur c'est produite on le signal
			return false;
		}
		else
		{
			//Le fichire n'existe pas
			return false;
		}
	}
	void FileLock::UnLock()
	{
		if(fs != nullptr)
		{
			try
			{
				//Si le FileStream existe alors on tente de le fermer
				fs->Close();
				//et on le detruit
				delete fs;
			}
			catch(Exception^ ex)
			{
				throw ex;
			}
		}
	}
}