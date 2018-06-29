// Zorro.NinjaTrader8.ATI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

int(__cdecl *BrokerError)(const char* txt) = NULL;
int(__cdecl *BrokerProgress)(const int percent) = NULL;

int(__cdecl *Nt8Connected)(const int showMessage) = NULL;


HINSTANCE hinstNtDirect = NULL;

DLLFUNC_C int BrokerOpen(char* Name, FARPROC fpError, FARPROC fpProgress)
{
	strcpy_s(Name, 32, "NT8");
	(FARPROC&)BrokerError = fpError;
	(FARPROC&)BrokerProgress = fpProgress;

	hinstNtDirect = LoadLibrary(TEXT("NtDirect.dll"));

	if (hinstNtDirect != NULL)
	{
		(FARPROC&)Nt8Connected = GetProcAddress(hinstNtDirect, "Connected");

		if (NULL == Nt8Connected)
		{
			MessageBox(NULL, L"Cannot Find Connected function!", L"Error", 0);
		}
	}
	else
	{
		MessageBox(NULL, L"Cannot Load NtDirect.dll!", L"Error", 0);
	}

	return 2;
}

DLLFUNC_C int BrokerLogin(char* User, char* Pwd, char* Type, char* Accounts)
{
	if (Nt8Connected(0) == 0)
		return 1;
	else
		return 0;
}


