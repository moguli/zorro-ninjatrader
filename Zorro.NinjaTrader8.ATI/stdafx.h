// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// TODO: reference additional headers your program requires here
typedef double DATE;
#include "include\zorro\trading.h"

#define DLLFUNC extern __declspec(dllexport)
#define DLLFUNC_C extern "C" __declspec(dllexport)

// zorro functions
DLLFUNC_C int BrokerOpen(char* Name, FARPROC fpError, FARPROC fpProgress);
DLLFUNC_C int BrokerLogin(char* User, char* Pwd, char* Type, char* Account);
DLLFUNC_C int BrokerTime(DATE *pTimeGMT);
DLLFUNC_C int BrokerAccount(char* Account, double *pBalance, double *pTradeVal, double *pMarginVal);
DLLFUNC_C int BrokerAsset(char* Asset, double *pPrice, double *pSpread, double *pVolume, double *pPip, double *pPipCost, double *pLotAmount, double *pMarginCost, double *pRollLong, double *pRollShort);
DLLFUNC_C int BrokerBuy(char* Asset, int nAmount, double dStopDist, double *pPrice);
DLLFUNC_C double BrokerCommand(int Command, DWORD dwParameter);