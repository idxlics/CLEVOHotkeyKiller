// CLEVOHotkeyKiller.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "CLEVOHotkeyKiller.h"
#include <TlHelp32.h>


void KillProcess(LPTSTR Filename)
{
	LPTSTR filename = new TCHAR[lstrlen(Filename) + 1];
	wcscpy_s(filename, lstrlen(Filename) + 1, Filename);
	_wcslwr_s(filename, lstrlen(filename) + 1);
	TCHAR ExeFile[MAX_PATH];
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapshot, &pEntry);
	while (hRes)
	{
		wcscpy_s(ExeFile, pEntry.szExeFile);
		_wcslwr_s(pEntry.szExeFile);
		if (wcsstr(filename, pEntry.szExeFile) != NULL)
		{
			wprintf(ExeFile);
			wprintf(L"...");
			HANDLE hProcess = OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, 0, pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 1);
				if (WaitForSingleObject(hProcess, 10000) == 0)
				{
					wprintf(L"Killed\n");
				}
				else
				{
					wprintf(L"Timed out\n");
				}
				CloseHandle(hProcess);
			}
			else
			{
				wprintf(L"Failed to open process\n");
			}
		}
		hRes = Process32Next(hSnapshot, &pEntry);
	}
	CloseHandle(hSnapshot);
	delete[]filename;
}

/*void ListProcess()
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapshot, &pEntry);
	while (hRes)
	{
		wprintf(pEntry.szExeFile);
		wprintf(L"\n");
		hRes = Process32Next(hSnapshot, &pEntry);
	}
	CloseHandle(hSnapshot);
}*/

void PreReturn(void)
{
	TCHAR dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);
	wprintf(dir);
	wprintf(L">");
	FreeConsole();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{

	if (AttachConsole(ATTACH_PARENT_PROCESS))
	{
		FILE *stream;
		_wfreopen_s(&stream, L"CONOUT$", L"w+t", stdout);
		wprintf(L"\n");
	}

	LPTSTR seps = L" ";
	LPTSTR token = NULL;
	LPTSTR next_token = NULL;
	token = wcstok_s(lpCmdLine, seps, &next_token);
	if (token != NULL)
	{
		if (_wcsicmp(token, L"/r") == 0)
		{
			STARTUPINFO si = { sizeof(si) };
			PROCESS_INFORMATION pi;
			TCHAR lpAppName[MAX_PATH] = { L'\0' };
			GetEnvironmentVariable(L"ProgramFiles(x86)", lpAppName, MAX_PATH);  //%ProgramFiles(x86)%
			wcscat_s(lpAppName, L"\\Hotkey\\HkeyTray.exe");
			wprintf(L"Run HkeyTray.exe...");
			if (CreateProcess(lpAppName, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi))
			{
				wprintf(L"Process created\n");
			}
			else
			{
				wprintf(L"Failed\n");
			}
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			token = wcstok_s(NULL, seps, &next_token);
			if (token != NULL)
			{
				Sleep(1000 * wcstol(token, NULL, 10));
			}
			else
			{
				Sleep(15000);
			}
		}
		else if (_wcsicmp(token, L"/d") == 0)
		{
			token = wcstok_s(NULL, seps, &next_token);
			if (token != NULL)
			{
				Sleep(1000 * wcstol(token, NULL, 10));
			}
			else
			{
				Sleep(15000);
			}
		}
		else if (_wcsicmp(token, L"/?") == 0 || _wcsicmp(token, L"/help") == 0)
		{
			wprintf(L"Kill CLEVO Hotkey processes including ComboKeyTray.exe, ControlCenter.exe, HKClipSvc.exe, HkeyTray.exe, hkysound.exe, HotkeyService.exe\n\n");
			wprintf(L"Usage:\nCLEVOHotkeyKiller [/r time | /d time | /? | /help]\n\n");
			wprintf(L"Options: (case-insensitive)\n");
			wprintf(L"No options	Kill the processes immediately\n");
			wprintf(L"/r time		Run HkeyTray.exe, delay <time> seconds, then kill the processes\n");
			wprintf(L"/d time		Delay <time> seconds, then kill the processes\n");
			wprintf(L"/? | /help	Display help message\n\n");
			wprintf(L"Remark: The default value of <time> is 15\n\n");
			PreReturn();
			return 0;
		}
		
	}

	KillProcess(L"ComboKeyTray.exe\\ControlCenter.exe\\HKClipSvc.exe\\HkeyTray.exe\\hkysound.exe\\HotkeyService.exe");
	wprintf(L"\n");

	//MSG msg;
	//while (GetMessage(&msg, NULL, 0, 0)); //use while(1) will take up 1 CPU thread
	//while (1);	

	PreReturn();
	return 0;
}

