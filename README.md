# NinjaTrader 8 (and 7?) plugin for Zorro

NinjaTrader is a software platform that provides charting and trading capabilities with connectivity to multiple brokers. The software can trade futures, forex, and equities depending on the capabilities of the user's brokers.

This plugin was written in C++ with Visual Studio 2017.

I use NinjaTrader 8, so I can't verify if the plugin works for NinjaTrader 7. It should in theory, as the exported interface has not changed between versions. Feel free to try it and let me know.

## Installation Instructions

To install the plugin, simply place the Zorro.NinjaTrader8.ATI.dll file in the Plugin folder where Zorro is installed.

## Login Instructions
1 Make sure NinjaTrader is running and that the ATI system is enabled in Tools->Options->Automated Trading Interface.
2 Choose NT8 from the plugin dropdown.
3 User should be set to the NT8 account name you wish to trade in. Testing can be done in the default Sim101 account.
4 Password is unused.

## Connectivity Method
This plugin uses the native C++ NinjaTrader ATI library, NtDirect.dll. This DLL is normally installed in %WinDir%\System32 and is found and loaded automatically by the plugin.

## Plugin Capabilities
The ATI connection method is simple but is not full-featured. Historical data, account parameters, and asset information outside of price and spread are all unavailable.

At some point I may attempt to write a full-featured bridge in the same vein as Zorro's MT4/5 bridge, but it probably won't happen anytime soon unless someone wants to pay me to do it! That would be a bear of a project.

## Functions Implemented
* BrokerOpen
* BrokerLogin
* BrokerAsset