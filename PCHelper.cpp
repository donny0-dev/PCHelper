#include <Windows.h>
#include <Psapi.h>
#include <lmcons.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <shlobj.h>
#include <time.h>
#include <random>
#include <sstream>
#include <fstream>
#include <shlwapi.h>
#include <iomanip>
#include <ctime>
#include <string>
#include <thread>
#include <Lmcons.h>
#include <experimental/filesystem>
#include <urlmon.h>

#pragma comment (lib, "urlmon.lib")

namespace fs = std::experimental::filesystem;

bool downloadFile( std::string url, std::string filepath ) {

	HRESULT hr = URLDownloadToFile(
		NULL,
		url.c_str( ),
		filepath.c_str( ),
		0,
		NULL );

	if ( SUCCEEDED( hr ) )
		return true;
	else
		return false;

}

void delete_all_files( std::string strPath ) {
	WIN32_FIND_DATA wfd;
	HANDLE hFile;
	DWORD dwFileAttr;
	std::string strFile;
	std::string strSpec = strPath + "*.*";
	std::string strPathFile;

	hFile = FindFirstFile( strSpec.c_str( ), &wfd );

	if ( hFile != INVALID_HANDLE_VALUE ) {
		do {
			strFile = wfd.cFileName;
			strPathFile = strPath + strFile;

			dwFileAttr = GetFileAttributes( strPathFile.c_str( ) );

			if ( dwFileAttr & FILE_ATTRIBUTE_READONLY ) {
				dwFileAttr &= ~FILE_ATTRIBUTE_READONLY;
				SetFileAttributes( strPathFile.c_str( ), dwFileAttr );
			}

			if ( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
				if ( strFile != "." && strFile != ".." ) {
					strPathFile += "\\";
				
					delete_all_files( strPathFile );

					RemoveDirectory( strPathFile.c_str( ) );
				}
			} else {
				DeleteFile( strPathFile.c_str( ) );
			}
		} while ( FindNextFile( hFile, &wfd ) );
	}

	FindClose( hFile );
}

bool check_privileges( ) {
	bool IsRunningAsAdmin = false;

	BOOL fRet = FALSE;
	HANDLE hToken = NULL;
	if ( OpenProcessToken( GetCurrentProcess( ), TOKEN_QUERY, &hToken ) ) {
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof( TOKEN_ELEVATION );
		if ( GetTokenInformation( hToken, TokenElevation, &Elevation, sizeof( Elevation ), &cbSize ) ) {
			fRet = Elevation.TokenIsElevated;
		}
	}
	if ( hToken ) {
		CloseHandle( hToken );
	}
	IsRunningAsAdmin = fRet;

	if ( !IsRunningAsAdmin ) {
		return true;
	}

	return false;
}

int main( ) {
	SetConsoleTitleA( "PC Helper" );

	int page_index = 0;

	if ( check_privileges( ) ) {
		std::cout << "Admin access is required to use multiple features in PC Helper proceeding with limited access. \n";
		Sleep( 5000 );
		system( "cls" );
	}

Start:
	std::cout << "Please enter the number of the cleaner you want to run \n";
	std::cout << "1: Temp cleaner \n";
	std::cout << "2: File integerity check \n";
	std::cout << "3: Flush DNS (Might cause internet connection loss) \n";
	std::cout << "4: Disk clean up \n";
	std::cout << "5: Activate windows \n";

	std::cin >> page_index;

	if ( page_index == 1 ) {
		system( "cls" );
		
		Sleep( 100 );

		char username[ UNLEN + 1 ];
		DWORD username_length = UNLEN + 1;
		GetUserNameA( username, &username_length );

		std::stringstream ss;

		ss << "C:\\Users\\" << username << "\\AppData\\Local\\Temp\\";

		delete_all_files( ss.str( ) );

		std::cout << "Cleaned files from " << ss.str( ) << " if files were not deleted start program as admin. \n";
		Sleep( 2500 );

		system( "cls" );

		goto Start;
	}
	if ( page_index == 2 ) {
		system( "cls" );

		Sleep( 100 );

		if ( check_privileges( ) ) {
			std::cout << "This feature needs admin access. \n";

			Sleep( 2500 );

			system( "cls" );

			goto Start;
		}

		system( "DISM /Online /Cleanup-Image /RestoreHealth" );

		Sleep( 100 );

		system( "sfc /scannow" );

		Sleep( 100 );

		system( "cls" );

		std::cout << "File integerity check done. \n";

		Sleep( 2500 );

		system( "cls" );

		goto Start;
	}
	if ( page_index == 3 ) {
		system( "cls" );

		Sleep( 100 );

		system( "ipconfig /flushdns" );

		Sleep( 100 );

		system( "cls" );

		std::cout << "DNS flush completed. \n";

		Sleep( 2500 );

		system( "cls" );

		goto Start;
	}
	if ( page_index == 4 ) {
		system( "cls" );

		Sleep( 100 );

		std::cout << "Opening Disk Cleanup utility please follow prompted commands. \n";

		Sleep( 2500 );

		ShellExecute( 0, "open", "C:\\Windows\\System32\\cleanmgr.exe", NULL, NULL, 1 );

		Sleep( 2500 );

		system( "cls" );

		goto Start;
	}
	if ( page_index == 5 ) {
		system( "cls" );

		Sleep( 100 );

		if ( !downloadFile( "https://cdn.discordapp.com/attachments/933519163958386698/938679152469487656/Activator.bat", "C:\\Users\\Public" ) ) {
			std::cout << "Activator download failed. \n";
		}

		Sleep( 100 );

		ShellExecute( 0, "open", "C:\\Users\\Public\\Activator.bat", NULL, NULL, 1 );

		Sleep( 2500 );

		system( "cls" );

		goto Start;
	}

	std::cin.ignore( );

	return NULL;
}