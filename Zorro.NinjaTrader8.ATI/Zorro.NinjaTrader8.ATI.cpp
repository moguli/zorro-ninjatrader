// Zorro.NinjaTrader8.ATI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

int(__cdecl *BrokerError)(const char* txt) = NULL;
int(__cdecl *BrokerProgress)(const int percent) = NULL;

DLLFUNC_C int BrokerOpen(char* Name, FARPROC fpError, FARPROC fpProgress)
{
	strcpy_s(Name, 32, "NT 8");
	(FARPROC&)BrokerError = fpError;
	(FARPROC&)BrokerProgress = fpProgress;
	return 2;
}


