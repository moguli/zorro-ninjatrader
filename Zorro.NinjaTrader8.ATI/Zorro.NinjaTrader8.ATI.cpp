// Zorro.NinjaTrader8.ATI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <string>
#include <sstream>
#include <vector>
#include <algorithm> // transform

BOOL dllLoaded = 0;

int(__cdecl *BrokerError)(const char* txt) = NULL;
int(__cdecl *BrokerProgress)(const int percent) = NULL;

HINSTANCE hinstNtDirect = NULL;

typedef INT(CALLBACK* NT8CONNECTED)(INT);
NT8CONNECTED nt8Connected;

typedef INT(CALLBACK* NT8TEARDOWN)();
NT8TEARDOWN nt8TearDown;

typedef double(CALLBACK* NT8CASHVALUE)(string);
NT8CASHVALUE nt8CashValue;

typedef double(CALLBACK* NT8REALIZEDPNL)(string);
NT8REALIZEDPNL nt8RealizedPnL;

typedef INT(CALLBACK* NT8SUBSCRIBEMARKETDATA)(string);
NT8SUBSCRIBEMARKETDATA nt8SubscribeMarketData;

typedef double(CALLBACK* NT8MARKETDATA)(string, INT);
NT8MARKETDATA nt8MarketData;

typedef INT(CALLBACK* NT8UNSUBSCRIBEMARKETDATA)(string);
NT8UNSUBSCRIBEMARKETDATA nt8UnsubscribeMarketData;

typedef INT(CALLBACK* NT8COMMAND)(string, string, string, string, int, string, double, double, string, string, string, string, string);
NT8COMMAND nt8Command;

bool isCaseInsensitveMatch(std::string a, std::string b)
{
	std::transform(a.begin(), a.end(), a.begin(), toupper);
	std::transform(b.begin(), b.end(), b.begin(), toupper);
	if (a == b)return true;
	else return false;
}


class AssetList
{
private:
	class Entry
	{
	public:
		std::string asset = "";
		double price = 0.;
		double spread = 0.;
	};

	std::vector<Entry> entries;
	bool isInitialized = false;

	void initialize()
	{
		if (isInitialized) return;
		entries.reserve(50);
		isInitialized = true;
	}


public:

	void updateEntry(std::string Asset, double pPrice, double pSpread)
	{
		initialize();
		// is asset in the list?
		for (auto & this_entry : entries)
		{
			if (!isCaseInsensitveMatch(this_entry.asset, Asset)) continue;	// skip if case insensitve match fails, i.e. MSFT/msft matches

			this_entry.price = pPrice;
			this_entry.spread = pSpread;
			return;
		}
		//not found, push back
		Entry new_entry;
		new_entry.asset = Asset;
		new_entry.price = pPrice;
		new_entry.spread = pSpread;
		entries.push_back(new_entry);

		return;
	}


	bool CanGetValues(std::string Asset, double *outputPrice, double *outputSpread)
	{
		// is asset in the list?
		for (auto & this_entry : entries)
		{
			if (!isCaseInsensitveMatch(this_entry.asset, Asset)) continue;	// skip if case insensitve match fails, i.e. MSFT/msft matches

			*outputPrice = this_entry.price;
			*outputSpread = this_entry.spread;
			return true;
		}

		return false;
	}

	bool DoesAssetExist(std::string find_this_asset)
	{
		// is asset in the list?
		for (auto & this_entry : entries)
		{
			if (!isCaseInsensitveMatch(this_entry.asset, find_this_asset)) continue;	// skip if case insensitve match fails, i.e. MSFT/msft matches

			return true;
		}
		//not found
		return false;
	}

	int DeleteAsset(std::string inputAsset)
	{
		if (inputAsset == "")return false; // nothing was deleted.
		unsigned int i = 0; int entries_deleted = 0;
		while (i < entries.size())
		{
			if (!isCaseInsensitveMatch(entries[i].asset, inputAsset)) { i++;  continue; }  // skip if case insensitve match fails, i.e. MSFT/msft matches
			else
			{
				entries.erase(entries.begin() + i);
				entries_deleted++;
			}
		}

		return entries_deleted;
	}

	std::vector<std::string> GetAssets()
	{
		std::vector<std::string> out;

		for (auto & this_entry : entries)
		{
			out.push_back(this_entry.asset);
		}

		return out;
	}

	void ClearAssets()
	{
		entries.clear();
	}


	std::string GetAssetCSV()
	{
		std::ostringstream oss; int i = 0;
		for (auto & this_entry : entries)
		{
			oss << this_entry.asset << ",";
			i++;
		}

		if (i == 0) return("");
		std::string csv = oss.str();
		return(csv.substr(0, csv.length() - 1));
	}

};

AssetList assetList;
string accountName;

DLLFUNC_C int BrokerOpen(char* Name, FARPROC fpError, FARPROC fpProgress)
{
	strcpy_s(Name, 4, "NT8");
	(FARPROC&)BrokerError = fpError;
	(FARPROC&)BrokerProgress = fpProgress;

	hinstNtDirect = LoadLibrary(TEXT("NtDirect.dll"));

	if (hinstNtDirect != NULL)
	{
		dllLoaded = TRUE;

		nt8Connected = (NT8CONNECTED)GetProcAddress(hinstNtDirect, "Connected");
		nt8TearDown = (NT8TEARDOWN)GetProcAddress(hinstNtDirect, "TearDown");
		nt8CashValue = (NT8CASHVALUE)GetProcAddress(hinstNtDirect, "CashValue");
		nt8RealizedPnL = (NT8REALIZEDPNL)GetProcAddress(hinstNtDirect, "RealizedPnL");
		nt8SubscribeMarketData = (NT8SUBSCRIBEMARKETDATA)GetProcAddress(hinstNtDirect, "SubscribeMarketData");
		nt8MarketData = (NT8MARKETDATA)GetProcAddress(hinstNtDirect, "MarketData");
		nt8UnsubscribeMarketData = (NT8UNSUBSCRIBEMARKETDATA)GetProcAddress(hinstNtDirect, "UnsubscribeMarketData");

		if (NULL == nt8Connected)
		{
			BrokerError("Cannot Find NT8 Connected function!");
			FreeLibrary(hinstNtDirect);
			dllLoaded = FALSE;
		}

		if (NULL == nt8TearDown)
		{
			BrokerError("Cannot Find NT8 TearDown function!");
			FreeLibrary(hinstNtDirect);
			dllLoaded = FALSE;
		}

		if (NULL == nt8CashValue)
		{
			BrokerError("Cannot Find NT8 CashValue function!");
			FreeLibrary(hinstNtDirect);
			dllLoaded = FALSE;
		}

		if (NULL == nt8RealizedPnL)
		{
			BrokerError("Cannot Find NT8 RealizedPnL function!");
			FreeLibrary(hinstNtDirect);
			dllLoaded = FALSE;
		}

		if (NULL == nt8SubscribeMarketData)
		{
			BrokerError("Cannot Find NT8 SubscribeMarketData function!");
			FreeLibrary(hinstNtDirect);
			dllLoaded = FALSE;
		}

		if (NULL == nt8MarketData)
		{
			BrokerError("Cannot Find NT8 MarketData function!");
			FreeLibrary(hinstNtDirect);
			dllLoaded = FALSE;
		}

		if (NULL == nt8UnsubscribeMarketData)
		{
			BrokerError("Cannot Find NT8 UnsubscribeMarketData function!");
			FreeLibrary(hinstNtDirect);
			dllLoaded = FALSE;
		}
	}
	else
	{
		BrokerError("Cannot Find NtDirect.dll!");
		dllLoaded = FALSE;
	}

	return 2;
}

DLLFUNC_C int BrokerLogin(char* User, char* Pwd, char* Type, char* Accounts)
{
	if (dllLoaded == FALSE)
	{
		BrokerError("NtDirect.dll is not loaded, check previous errors!");
		return 0;
	}

	if (User != NULL) // Log in
	{
		if (nt8Connected(0) == 0)
		{
			accountName = User;
			return 1;
		}
		else
		{
			BrokerError("NT8 ATI is not responding. Ensure: \n - NinjaTrader 8 is running \n - Tools->Options->Automated Trading Interface->AT Interface is checked \n - Server Port is 36973");
			return 0;
		}
	}
	else // Log out
	{
		for (auto & this_asset : assetList.GetAssets())
		{
			nt8UnsubscribeMarketData((char *)this_asset.c_str());
		}

		assetList.ClearAssets();

		if (nt8TearDown() == 0)
		{
			return 0;
		}
		else
		{
			BrokerError("Error tearing down NT8 ATI connection");
			return 0;
		}
	}	
		
}

DLLFUNC_C int BrokerAsset(char* Asset, double *pPrice, double *pSpread, double *pVolume, double *pPip, double *pPipCost, double *pLotAmount, double *pMarginCost, double *pRollLong, double *pRollShort)
{
	if (!pPrice) // Subscribe to asset
	{
		if (!assetList.DoesAssetExist(Asset))
		{
			int result = nt8SubscribeMarketData(Asset);

			if (result == 0)
			{
				double last = nt8MarketData(Asset, 0);
				double bid = nt8MarketData(Asset, 1);
				double ask = nt8MarketData(Asset, 2);

				assetList.updateEntry(Asset, last, bid - ask);

				return 1;
			}

			return 0;
		}
		else
		{
			return 1;
		}
	}
	else // Return data about asset
	{
		double last = nt8MarketData(Asset, 0);
		double bid = nt8MarketData(Asset, 1);
		double ask = nt8MarketData(Asset, 2);

		*pPrice = last;
		*pSpread = bid - ask;
		return 1;
	}
}

DLLFUNC_C int BrokerAccount(char* Account, double *pBalance, double *pTradeVal, double *pMarginVal)
{
	if (Account)
	{
		BrokerError("Multiple accounts not supported!");
		return 0;
	}

	*pBalance = nt8CashValue(accountName);
	// auto c = nt8RealizedPnL(accountName);

	return 1;
}

//DLLFUNC_C int BrokerBuy(char* Asset, int nAmount, double dStopDist, double *pPrice)
//{
//
//}


