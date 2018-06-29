// Zorro.NinjaTrader8.ATI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

int(__cdecl *BrokerError)(const char* txt) = NULL;
int(__cdecl *BrokerProgress)(const int percent) = NULL;

HINSTANCE hinstNtDirect = NULL;

typedef INT(CALLBACK* NT8CONNECTED)(INT);
NT8CONNECTED nt8Connected;

typedef double(CALLBACK* NT8CASHVALUE)(double);
NT8CASHVALUE nt8CashValue;

typedef double(CALLBACK* NT8SUBSCRIBEMARKETDATA)(string);
NT8SUBSCRIBEMARKETDATA nt8SubscribeMarketData;

typedef double(CALLBACK* NT8MARKETDATA)(string, INT);
NT8MARKETDATA nt8MarketData;

typedef double(CALLBACK* NT8UNSUBSCRIBEMARKETDATA)(string);
NT8UNSUBSCRIBEMARKETDATA nt8UnsubscribeMarketData;

DLLFUNC_C int BrokerOpen(char* Name, FARPROC fpError, FARPROC fpProgress)
{
	strcpy_s(Name, 32, "NT8");
	(FARPROC&)BrokerError = fpError;
	(FARPROC&)BrokerProgress = fpProgress;

	hinstNtDirect = LoadLibrary(TEXT("NtDirect.dll"));

	if (hinstNtDirect != NULL)
	{
		nt8Connected = (NT8CONNECTED)GetProcAddress(hinstNtDirect, "Connected");
		nt8CashValue = (NT8CASHVALUE)GetProcAddress(hinstNtDirect, "CashValue");
		nt8SubscribeMarketData = (NT8SUBSCRIBEMARKETDATA)GetProcAddress(hinstNtDirect, "SubscribeMarketData");
		nt8MarketData = (NT8MARKETDATA)GetProcAddress(hinstNtDirect, "MarketData");
		nt8UnsubscribeMarketData = (NT8UNSUBSCRIBEMARKETDATA)GetProcAddress(hinstNtDirect, "UnsubscribeMarketData");

		if (NULL == nt8Connected)
		{
			BrokerError("Cannot Find NT8 Connected function!");
			FreeLibrary(hinstNtDirect);
		}

		if (NULL == nt8CashValue)
		{
			BrokerError("Cannot Find NT8 CashValue function!");
			FreeLibrary(hinstNtDirect);
		}

		if (NULL == nt8SubscribeMarketData)
		{
			BrokerError("Cannot Find NT8 SubscribeMarketData function!");
			FreeLibrary(hinstNtDirect);
		}

		if (NULL == nt8MarketData)
		{
			BrokerError("Cannot Find NT8 MarketData function!");
			FreeLibrary(hinstNtDirect);
		}

		if (NULL == nt8UnsubscribeMarketData)
		{
			BrokerError("Cannot Find NT8 UnsubscribeMarketData function!");
			FreeLibrary(hinstNtDirect);
		}
	}
	else
	{
		BrokerError("Cannot Find NtDirect.dll!");
	}

	return 2;
}

DLLFUNC_C int BrokerLogin(char* User, char* Pwd, char* Type, char* Accounts)
{
	if (hinstNtDirect == NULL)
	{
		BrokerError("NtDirect.dll is not loaded, check previous errors!");
		return 0;
	}
	
	if (nt8Connected(0) == 0)
	{
		return 1;
	}
	else
	{
		BrokerError("NT8 ATI is not responding. Ensure: \n - NinjaTrader 8 is running \n - Tools->Options->Automated Trading Interface->AT Interface is checked \n - Server Port is 36973");
		return 0;
	}
		
}


