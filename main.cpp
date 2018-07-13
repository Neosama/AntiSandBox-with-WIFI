
#include <Windows.h>
#include <iostream>
#include <wlanapi.h>

#pragma comment(lib, "wlanapi.lib")


VOID WlanNotification(WLAN_NOTIFICATION_DATA *wlanNotifData, VOID *p)
{
	if (wlanNotifData->NotificationCode == wlan_notification_acm_scan_complete)
	{
		bool bWait = false;
	}
	else if (wlanNotifData->NotificationCode == wlan_notification_acm_scan_fail)
	{
		bool bWait = false;
	}
}

bool WIFI_here()
{
	HANDLE hWlan = NULL;

	DWORD dwError = 0;
	DWORD dwSupportedVersion = 0;
	DWORD dwClientVersion = (TRUE ? 2 : 1);

	GUID guidInterface; ZeroMemory(&guidInterface, sizeof(GUID));

	WLAN_INTERFACE_INFO_LIST *wlanInterfaceList = (WLAN_INTERFACE_INFO_LIST*)WlanAllocateMemory(sizeof(WLAN_INTERFACE_INFO_LIST));
	ZeroMemory(wlanInterfaceList, sizeof(WLAN_INTERFACE_INFO_LIST));

	WLAN_AVAILABLE_NETWORK_LIST *wlanNetworkList = (WLAN_AVAILABLE_NETWORK_LIST*)WlanAllocateMemory(sizeof(WLAN_AVAILABLE_NETWORK_LIST));
	ZeroMemory(wlanNetworkList, sizeof(WLAN_AVAILABLE_NETWORK_LIST));

	int xxx = 0;

	try
	{
		if (dwError = WlanOpenHandle(dwClientVersion, NULL, &dwSupportedVersion, &hWlan) != ERROR_SUCCESS)
			return FALSE;

		if (dwError = WlanEnumInterfaces(hWlan, NULL, &wlanInterfaceList) != ERROR_SUCCESS)
			return FALSE;

		if (dwError = wlanInterfaceList->InterfaceInfo[0].isState != wlan_interface_state_not_ready)
		{
			if (wlanInterfaceList->dwNumberOfItems > 1)
			{
				guidInterface = wlanInterfaceList->InterfaceInfo[0].InterfaceGuid;
			}
			else
			{
				guidInterface = wlanInterfaceList->InterfaceInfo[0].InterfaceGuid;
			}
		}
		else
			return FALSE;

		DWORD dwPrevNotif = 0;
		bool bWait = true;

		// Scan takes awhile so we need to register a callback
		if (dwError = WlanRegisterNotification(hWlan, WLAN_NOTIFICATION_SOURCE_ACM, TRUE,
			(WLAN_NOTIFICATION_CALLBACK)WlanNotification, NULL, NULL, &dwPrevNotif) != ERROR_SUCCESS)
			return FALSE;

		if (dwError = WlanScan(hWlan, &guidInterface, NULL, NULL, NULL) != ERROR_SUCCESS)
			return FALSE;

		Sleep(100);

		// Unregister callback, don't care if it succeeds or not
		WlanRegisterNotification(hWlan, WLAN_NOTIFICATION_SOURCE_NONE, TRUE, NULL, NULL, NULL, &dwPrevNotif);

		if (dwError = WlanGetAvailableNetworkList(hWlan, &guidInterface, NULL, NULL, &wlanNetworkList) != ERROR_SUCCESS)
			return FALSE;

		for (unsigned int i = 0; i < wlanNetworkList->dwNumberOfItems; i++)
		{
			xxx++;
		}
	}
	catch (char *szError)
	{
		return FALSE;
	}

	if (wlanNetworkList)
		WlanFreeMemory(wlanNetworkList);
	if (wlanInterfaceList)
		WlanFreeMemory(wlanInterfaceList);
	if (hWlan)
		WlanCloseHandle(hWlan, NULL);

	if (xxx > 0)
		return TRUE;
}

int main(int argc, char *argv[])
{
	printf("\nWIFI HERE : %s", WIFI_here() ? "TRUE" : "FALSE");
	
	getchar();
	return 0;
}
