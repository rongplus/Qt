﻿#include "wirelessmonitor.h"

#include <QSplitter>
#include <QTimer>

BOOL bWait = true;
BOOL bIsWindowsXPorLater;
BOOL bIsWindowsVistaorLater;
BOOL bIsWindows7Later;
BOOL bIsWindows8Later;
bool bIsWindows81Later;
bool bIsWindows10Later;

//GUID and handle list
#include <vector>
#include <algorithm>
struct GUIDHandle {
   WCHAR*  id;
   HANDLE hClientHandle;
};
vector<struct GUIDHandle> vGUIDHandle;

//OID createfile handle
HANDLE hClientHandle;
TCHAR CurrentMACAddressStr[256]; 

//WinINet
BOOL checkUpdate()
{
	DWORD nErrorNo;
	HINTERNET hOpen, hURLFile;
	LPCWSTR NameProgram = L"WlanQuery";             //      LPCWSTR == Long Pointer to Const Wide String 
	LPCWSTR Website = L"https://github.com/coolshou/WlanQuery/releases/latest";
	//CString MyHttpServer=L"";
	hOpen = InternetOpen(NameProgram, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
	if ( hOpen == NULL ) {
		DWORD nErrorNo = GetLastError(); // 
		LPCWSTR lpBuffer;    
		FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER  |
		    FORMAT_MESSAGE_IGNORE_INSERTS  |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			nErrorNo, // ， GetLastError()
			LANG_NEUTRAL,
			(LPTSTR) & lpBuffer,
			0 ,
			NULL );
		//cerr << "Error in opening internet" << endl;
		wprintf(L"Error in opening internet: %s\n", lpBuffer);
		//InternetCloseHandle(hOpen);
		//  Free the buffer.
		//LocalFree (lpBuffer);
		return false;
    }
	//https://msdn.microsoft.com/en-us/library/windows/desktop/aa385098%28v=vs.85%29.aspx
	//TODO: async open url or call back function
	//TODO: error handle
	hURLFile = InternetOpenUrl( 
		hOpen, 								//The handle to the current Internet session. The handle must have been returned by a previous call to InternetOpen.
		Website, 							//A pointer to a null-terminated string variable that specifies the URL to begin reading. Only URLs beginning with ftp:, http:, https: are supported.	
		NULL, 								//A pointer to a null-terminated string that specifies the headers to be sent to the HTTP server
		NULL, 								//The size of the additional headers, in TCHARs
		INTERNET_FLAG_SECURE | 				//flag: SSL,
		INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE,	// RELOAD, NO CACHE
		NULL 								//A pointer to a variable that specifies the application-defined value that is passed, along with the returned handle, to any callback functions.
	);            //Need to open the URL    
	if(!hURLFile) {
		//TODO: error on network can not access
		nErrorNo = GetLastError(); // 
		if(nErrorNo!=0)
		{
			if(nErrorNo==ERROR_INTERNET_EXTENDED_ERROR)
			{
				wstring wstrBuffer;
				DWORD bufferLength=0;
				InternetGetLastResponseInfo(&nErrorNo,NULL,&bufferLength);
				wstrBuffer.resize( bufferLength + 1 );
				InternetGetLastResponseInfo(&nErrorNo,&wstrBuffer[0],&bufferLength);
			} else {
				LPWSTR lpBuffer; 
				FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER  |
					FORMAT_MESSAGE_IGNORE_INSERTS  |
					FORMAT_MESSAGE_FROM_SYSTEM,
					NULL,
					nErrorNo, //  GetLastError()
					LANG_NEUTRAL,
					(LPTSTR) & lpBuffer,
					0 ,
					NULL );
				wprintf(L"Error in opening Website: %s\n %s\n", Website, lpBuffer);
				LocalFree (lpBuffer);
			}
			
		}
		//InternetCloseHandle(hURLFile);
		InternetCloseHandle(hOpen);
		//  Free the buffer.
		return false;
	}
	//TODO: buffer size handle?
	/* output is ?????
	//Pointer to dynamic buffer.
    //char *data = 0;
    //Dynamic data size.
	DWORD dwBytesRead = 0 ;
    //DWORD dataSize = 0;
	std::string output;
	do {
		char buffer[2000];
        InternetReadFile(hURLFile, (LPVOID) buffer, _countof(buffer), &dwBytesRead);
        //Allocate more space.
        //char *tempData = new char[dataSize + dwBytesRead];
        //Copy the already-fetched data into the new buffer.
        //memcpy(tempData, data, dataSize);
        //Now copy the new chunk of data.
        //memcpy(tempData + dataSize, buffer, dwBytesRead);
        //Now update the permanent variables
        //delete[] data;
        //data = tempData;
        //dataSize += dwBytesRead;
		output.append(buffer, dwBytesRead);
	} while (dwBytesRead);
	//TODO: parser data
	wprintf(L"TODO: parser data: %s\n", output);
	*/
	//dump content to file 
	
	DWORD dwBytesRead;
    VOID * szTemp[25];
	FILE * pFile ;
	CHAR *szFileName = "tmp.html";
	if  ( !(pFile = fopen (szFileName, "wb" ) ) )
	{
		cerr << "Error !" << endl;
		return FALSE;
	}
	do {
		// Keep coping in 25 bytes chunks, while file has any data left.
        // Note: bigger buffer will greatly improve performance.
        if (!InternetReadFile (hURLFile, szTemp, 100,  &dwBytesRead) )
        {
            fclose (pFile);
            cerr << "Error !" << endl;
			return FALSE;
        }
        if (!dwBytesRead)
            break;  // Condition of dwSize=0 indicate EOF. Stop.
        else
            fwrite(szTemp, sizeof (char), dwBytesRead , pFile);
	}while (TRUE);
	fflush (pFile);
    fclose (pFile);
	
	//end file

	/*
	InternetReadFile(hURLFile, fileBuffer, 100, &dwBytesRead);
	while (dwBytesRead == 100)
	{
		InternetReadFile(hURLFile, fileBuffer, 100, &dwBytesRead);
		fileBuffer[dwBytesRead] = '\0';
		cout << fileBuffer;
	}
	*/
	cout << endl;
	InternetCloseHandle(hURLFile);
	InternetCloseHandle(hOpen);
	return true;

}

/*get file*/
/*
inet_service = create inet
data = create u_inetresult
inet_service.geturl("http://test.com/a.pdf",data)

li_fln = FileOpen(as_FileName, StreamMode!, Write!, LockReadWrite!,
Replace!)
If li_fln < 0 Then Return -1 // Can't Open File to Modify

ll_StrLen = Len(data)

If ll_StrLen > 32765 Then
 If Mod(ll_StrLen, 32765) = 0 Then
  li_return = ll_StrLen / 32765
 Else
  li_return = (ll_StrLen / 32765) + 1
 End if
Else
 li_return = 1
End if

ll_CurrentPos = 1

For li_Cnt = 1 To li_return
 ls_record = Mid(ls_file, ll_CurrentPos, 32765)
 ll_CurrentPos += 32765
 If FileWrite(li_fln, ls_record) = -1 Then
  Return // Can't write
 End if
Next

FileClose(li_fln)
*/

/*
No matter if you use GetVersionEx() or RtlGetVersion(). Both will return the WRONG OS version if the user has right clicked your EXE and in 'Properties' -> 'Compatibility Mode' has chosen an older operating system.
*/
// RTL_OSVERSIONINFOEXW is defined in winnt.h
BOOL GetOsVersion(RTL_OSVERSIONINFOEXW* pk_OsVer)
{
	LONG Status = false;
	typedef LONG(WINAPI* tRtlGetVersion)(RTL_OSVERSIONINFOEXW*);

	memset(pk_OsVer, 0, sizeof(RTL_OSVERSIONINFOEXW));
	pk_OsVer->dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);

	HMODULE h_NtDll = GetModuleHandleW(L"ntdll.dll");
	if (h_NtDll > 0) {
		tRtlGetVersion f_RtlGetVersion = (tRtlGetVersion)GetProcAddress(h_NtDll, "RtlGetVersion");

		if (!f_RtlGetVersion)
			return FALSE; // This will never happen (all processes load ntdll.dll)

		LONG Status = f_RtlGetVersion(pk_OsVer);
	}
	return Status == 0; // STATUS_SUCCESS;
}

CString getVersion()
{
	/*
	CString current_version(_T(""));
	current_version = CGlobalFunctions::GetFileVersionX();
	/*
	wprintf( L"FileVersion: %s\nProductVersion: %s\nMyPrivateInfo: %s", 
             CGlobalFunctions::GetFileVersionX(), 
             CGlobalFunctions::GetProductVersionX(),
             CGlobalFunctions::GetVersionInfo(NULL, "MyPrivateInfo"));
			*/
	return "0";
} 

BOOL checkVistaAbove()
{

	OSVERSIONINFO osvi;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);
	wprintf(L"System version(Major,Minor): %d , %d\n", osvi.dwMajorVersion, osvi.dwMinorVersion);
	bIsWindowsXPorLater =
		((osvi.dwMajorVersion > 5) ||
		((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion >= 1)));

    bIsWindowsVistaorLater = 
       ( (osvi.dwMajorVersion > 6) ||
       ( (osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion >= 0) ));
	bIsWindows7Later =
		((osvi.dwMajorVersion > 6) ||
		((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion >= 1)));
	bIsWindows8Later =
		((osvi.dwMajorVersion > 6) ||
		((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion >= 2)));
	bIsWindows81Later =
		((osvi.dwMajorVersion > 6) ||
		((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion >= 3)));
	bIsWindows10Later =
		((osvi.dwMajorVersion > 10) ||
		((osvi.dwMajorVersion == 10) && (osvi.dwMinorVersion >= 0)));

	/*
	Applications not manifested for Windows 8.1 or Windows 10 will return the Windows 8 OS version value (6.2). Once an application is manifested for a given operating system version, GetVersionEx will always return the version that the application is manifested for in future releases.
	use Version Helper functions
	https://msdn.microsoft.com/zh-tw/library/windows/desktop/dn424972%28v=vs.85%29.aspx
	These APIs are defined by versionhelpers.h, which is included in the Windows 8.1 software development kit (SDK)
	if (bIsWindows8Later) {
		//win8.1 sdk
		bIsWindows81Later = IsWindows8Point1OrGreater();
		//win10 sdk
		//bIsWindows10Later = IsWindows10OrGreater;
	}
	*/
    return true;
}

/*
//TODO
ULONG getChannel(HANDLE *hClient, const GUID *pInterfaceGuid) 
{
	DWORD dwResult = 0;
	ULONG dwRetVal = 0;
	
	DWORD channelSize= sizeof(ULONG);
	ULONG channel=0;
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_query_only;
	
	dwResult = WlanQueryInterface(hClient,
								  pInterfaceGuid,
								  wlan_intf_opcode_channel_number,
								  NULL,
								  &channelSize,
								  (PVOID *) &channel, 
								  &opCode);

	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"    WlanQueryInterface channel failed with error: %u\n", dwResult);
		dwRetVal = 0;
		// You can use FormatMessage to find out why the function failed
	} else {
		wprintf(L"    channel:", channel);
		dwRetVal = channel;
	}
	return dwRetVal;
}
*/
//TODO
ULONG getRSSI(HANDLE* hClient, const GUID *pInterfaceGuid) 
{
	DWORD dwResult = 0;
	ULONG dwRetVal = 0;
	
	LONG rssi=0;
	DWORD dwSizeRssi = sizeof(rssi);
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_query_only;
	
	dwResult = WlanQueryInterface(hClient,
								  pInterfaceGuid,
								  wlan_intf_opcode_rssi,
								  NULL,
								  &dwSizeRssi,
								  (PVOID *) &rssi, 
								  &opCode);

	if (dwResult != ERROR_SUCCESS) {
		DWORD nErrorNo = GetLastError(); // 
		LPWSTR lpBuffer;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			nErrorNo, // ， GetLastError()
			LANG_NEUTRAL,
			(LPTSTR)& lpBuffer,
			0, NULL);
		wprintf(L"    WlanQueryInterface rssi failed with error(%u):(%i) %s\n", dwResult, nErrorNo, lpBuffer);

		//wprintf(L"    WlanQueryInterface rssi failed with error: %u\n", dwResult);
		dwRetVal = 0;
		// You can use FormatMessage to find out why the function failed
	} else {
		wprintf(L"    rssi: %d", rssi);
		dwRetVal = rssi;
	}
	return dwRetVal;
}

//convert frequency to channel
USHORT freqToChannel(ULONG freq)
{
	USHORT ch;
	ch =0;
	//TODO: 60GHz
	if (freq > 5000000) {
		//5GHz
		ch = ((freq - 5150000)/1000)/5 + 30;
	} else {
		//2.4GHz
		ch = ((freq - 2412000)/1000)/5 + 1;
	}
	return ch;
}

//add GUID handle
HANDLE AddGUIDHandle (WCHAR* wsNICGUIDinString)
{
	char	sNICIDFullPath[256];
	HANDLE hCHandle;
	// open handle
	memset(sNICIDFullPath, 0, 256);				
	sprintf_s(sNICIDFullPath, "\\\\.\\%S", wsNICGUIDinString);
	//TODO: dwFlagsAndAttributes with FILE_FLAG_OVERLAPPED for asynchronous I/O
	hCHandle = CreateFileA(sNICIDFullPath, GENERIC_READ | GENERIC_WRITE, 
											FILE_SHARE_READ | FILE_SHARE_WRITE,
											NULL, OPEN_EXISTING,
											0, NULL) ;

	if(hClientHandle == INVALID_HANDLE_VALUE)
	{
		wprintf(L"Error in CreateFileA\n");
		return NULL;
	}
	//TODO: how to improve when struct is big!!
	struct GUIDHandle sGH;
	sGH.id = wsNICGUIDinString;
	sGH.hClientHandle = hCHandle;
	vGUIDHandle.push_back(sGH);
	return hCHandle;
}

//check exist of GUIDHandle
HANDLE IsExistGUIDHandle (WCHAR* wsNICGUIDinString)
{
	WCHAR *t;
	HANDLE hCHandle = NULL;
	//check exist
	vector<struct GUIDHandle>::iterator iter = vGUIDHandle.begin();
   for(iter; iter != vGUIDHandle.end(); ++iter) {
	   t=iter->id;
	   if ( t == wsNICGUIDinString ) {
		   hCHandle = iter->hClientHandle;
		   break;
	   }
   }
   return hCHandle;
}

//Get GUID's Handle, when not exist, open a new one
HANDLE GetGUIDHandle (WCHAR* wsNICGUIDinString)
{
	HANDLE hCHandle = NULL;

	//check exist
   hCHandle = IsExistGUIDHandle(wsNICGUIDinString);
   if (hCHandle == NULL) {
	  //create new one
	  hCHandle = AddGUIDHandle(wsNICGUIDinString);
   }
   return hCHandle;
}

//Close GUIDHandle 
//return: true: close OK
//		 false: close fail
bool CloseGUIDHandle (WCHAR* wsNICGUIDinString)
{
	bool rs = false;
	WCHAR *t;
	//int i=0;
	HANDLE hCHandle = NULL;
	//hCHandle = NULL;
	//check exist
	vector<struct GUIDHandle>::iterator iter = vGUIDHandle.begin();
	for(iter; iter != vGUIDHandle.end(); ++iter) {
		t=iter->id;
		if ( t == wsNICGUIDinString ) {
			hCHandle = iter->hClientHandle;
			// close handle
			if (hCHandle != NULL) {
				rs = CloseHandle(hCHandle);
			}
			if (rs) {
				//remove from vector
				vGUIDHandle.erase(iter);
			}
			break;
		}
//		i++;
	}
	return rs;
}
//Close all open GUID Handle
bool CloseAllGUIDHandle ()
{
	bool rs = false;
	HANDLE hCHandle = NULL;
	vector<struct GUIDHandle>::iterator iter = vGUIDHandle.begin();
	for(iter; iter != vGUIDHandle.end(); ++iter) {
		hCHandle = iter->hClientHandle;
		if (hCHandle != NULL) {
				rs = CloseHandle(hCHandle);
		}
		if (rs) {
			//remove from vector
			vGUIDHandle.erase(iter);
		}
	}
	return rs;
}

DWORD DoWlanScan(WCHAR* wsNICGUIDinString)
{
	HANDLE hCHandle = NULL;
	INT	rtn;
	ULONG* test;
	test = (ULONG*)VirtualAlloc(NULL, sizeof(ULONG) * 1000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	hCHandle = GetGUIDHandle(wsNICGUIDinString);
	if (hCHandle != NULL) {
		ULONG	ulBytesReturned;
		ULONG	ulOIDCode;
		ulOIDCode = OID_802_11_BSSID_LIST_SCAN ;//action to perform , ndis 5.1
		//use: OID_DOT11_SCAN_REQUEST ( DOT11_SCAN_REQUEST_V2)

		//TODO: dwFlagsAndAttributes with FILE_FLAG_OVERLAPPED for asynchronous I/O
		//lpOverlapped 
		rtn = DeviceIoControl(
				hCHandle, IOCTL_NDIS_QUERY_GLOBAL_STATS,
				&ulOIDCode, sizeof(ulOIDCode),
				(ULONG *)test, 0,
//				(ULONG *)NULL, 0,
                &ulBytesReturned,
                NULL);
		//Notice: it will need some time to do the scan
		if(rtn == 0)
		{
			DWORD nErrorNo = GetLastError(); // 
			LPWSTR lpBuffer;    
			FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER  |
				FORMAT_MESSAGE_IGNORE_INSERTS  |
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				nErrorNo, // ， GetLastError()
				LANG_NEUTRAL,
				(LPTSTR) & lpBuffer,
				0 ,
				NULL );
			wprintf(L"Error in DoWlanScan DeviceIoControl:(%i) %s\n",nErrorNo, lpBuffer);
			CloseGUIDHandle(wsNICGUIDinString);
			return 1;
		}

	}
	return 0;
}
/**
* Show the connected AP info using the NDIS 802.11 IOCTLs.
*/
INT ShowConnectedAPwithNDIS(WCHAR* wsNICGUIDinString )
{
	INT	rtn;
	//TCHAR szCurrentESSID[256];
	//WCHAR szCurrentESSID[256];
	TCHAR szCurrentMACAddressStr[256]; 
	INT iCurrentRSSI;
	INT iCurrentLinkQuality;
	NDIS_802_11_RSSI ndisRSSI;
	NDIS_802_11_SSID ndisESSID;
	NDIS_802_11_MAC_ADDRESS ndisMACAddress;
//	WCHAR	wsNICGUIDinString[256];
//	char	sNICIDFullPath[256];
	WCHAR	sNICIDFullPath[256];
	DWORD	dwMemSize;
	ULONG	ulBytesReturned;
	ULONG	ulOIDCode;

	// open handle
	hClientHandle = GetGUIDHandle(wsNICGUIDinString);
	if (hClientHandle == NULL) {
		wprintf(L"Error in GetGUIDHandle: %s\n", wsNICGUIDinString);
		return 0;
	}

	// call to get connected ESSID
	dwMemSize = sizeof(NDIS_802_11_SSID);
	ulOIDCode = OID_802_11_SSID;
	ulBytesReturned = 0;
	rtn = DeviceIoControl(hClientHandle, IOCTL_NDIS_QUERY_GLOBAL_STATS,
							&ulOIDCode, sizeof(ulOIDCode), (ULONG *) &ndisESSID,
							dwMemSize, &ulBytesReturned, NULL);
	/*

	*/
	
	if(rtn == 0)
	{
		wprintf(L"Error in DeviceIoControl\n");
		//CloseHandle(hClientHandle);
		CloseGUIDHandle(wsNICGUIDinString);
		return rtn;
	}
	//memset(szCurrentESSID, 0, 256);
	//memcpy(szCurrentESSID, ndisESSID.Ssid, ndisESSID.SsidLength);


	// call to get connected MAC Address
	dwMemSize = sizeof(NDIS_802_11_MAC_ADDRESS);
	ulOIDCode = OID_802_11_BSSID;
	ulBytesReturned = 0;
	rtn = DeviceIoControl(hClientHandle, IOCTL_NDIS_QUERY_GLOBAL_STATS,
							&ulOIDCode, sizeof(ulOIDCode), (ULONG *) &ndisMACAddress,
							dwMemSize, &ulBytesReturned, NULL);
	if(rtn == 0)
	{
		wprintf(L"Error in DeviceIoControl\n");
		//CloseHandle(hClientHandle);
		CloseGUIDHandle(wsNICGUIDinString);
		return rtn;
	}
	memset(szCurrentMACAddressStr, 0, 256);
	_stprintf_s(szCurrentMACAddressStr, 256, L"%02x:%02x:%02x:%02x:%02x:%02x", 
		ndisMACAddress[0],
		ndisMACAddress[1],
		ndisMACAddress[2],
		ndisMACAddress[3],
		ndisMACAddress[4],
		ndisMACAddress[5]);
	//global
	memset(CurrentMACAddressStr, 0, 256);
	_stprintf_s(CurrentMACAddressStr, 256, L"%02x:%02x:%02x:%02x:%02x:%02x", 
		ndisMACAddress[0],
		ndisMACAddress[1],
		ndisMACAddress[2],
		ndisMACAddress[3],
		ndisMACAddress[4],
		ndisMACAddress[5]);	
		
	// call to get RSSI
	ndisRSSI = 0;
	dwMemSize = sizeof(NDIS_802_11_RSSI);
	ulOIDCode = OID_802_11_RSSI;
	ulBytesReturned = 0;
	rtn = DeviceIoControl(hClientHandle, IOCTL_NDIS_QUERY_GLOBAL_STATS,
							&ulOIDCode, sizeof(ulOIDCode), (ULONG *) &ndisRSSI,
							dwMemSize, &ulBytesReturned, NULL);
	if(rtn == 0)
	{
		wprintf(L"Error in DeviceIoControl\n");
		//CloseHandle(hClientHandle);
		CloseGUIDHandle(wsNICGUIDinString);
		return rtn;
	}

	// close handle
	//CloseHandle(hClientHandle);
	//CloseGUIDHandle(wsNICGUIDinString);

	iCurrentRSSI = ndisRSSI;
	// MS computes link quality as follows;  
	// range 1 to 100, maps rssi from -100 to -50, linear interpolation
	iCurrentLinkQuality = (ndisRSSI + 100) * 2;
	if(iCurrentLinkQuality < 0)
	{
		iCurrentLinkQuality = 0;
	}
	else if(iCurrentLinkQuality > 100)
	{
		iCurrentLinkQuality = 100;
	}

	//wprintf(L"Network Name (Profile Name): %s\n", szCurrentESSID);
	wprintf(L"    AP MAC Address : %s\t\t \n", szCurrentMACAddressStr);
	wprintf(L"    RSSI           : %d\t\t \n", iCurrentRSSI);
	wprintf(L"    Link Quality   : %d\t\t \n", iCurrentLinkQuality);
	//channel 
	
	return rtn;
}

VOID WlanNotification(WLAN_NOTIFICATION_DATA *wlanNotifData,VOID *p)
{
	if(wlanNotifData->NotificationCode == wlan_notification_acm_scan_complete)
	{
	    bWait = false;
	}
	else if(wlanNotifData->NotificationCode == wlan_notification_acm_scan_fail)
	{
	    printf("Scanning failed with error: %p\n", wlanNotifData->pData);
	    bWait = false;
	}
}

void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime) 
{
	printf("TimerProc\n");
	bWait = false;
}

DWORD WirelessMonitor::doConsoleScan()
{
	// Declare and initialize variables.
	
	//WLAN API Client version for Windows Vista and Windows Server 2008
	//1: for Client version for Windows XP with SP3 and Wireless LAN API for Windows XP with SP2.
	DWORD dwMaxClient = 2;
	//The version of the WLAN API that will be used in this session
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;
	DWORD dwRetVal = 0;
	int iRet = 0;
	TCHAR MACAddressStr[256];
	bool bIsCurrentConnected = false;
	WCHAR GuidString[39] = { 0 };
	unsigned int i, j, k, l, r;

	WLAN_RATE_SET rateSet;
	USHORT rate_in_mbps;
	USHORT maxRate_in_mbps = 0;
	// variables used for WlanEnumInterfaces
	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
	PWLAN_INTERFACE_INFO pIfInfo = NULL;

	PWLAN_AVAILABLE_NETWORK_LIST pNetworkList = NULL;
	PWLAN_AVAILABLE_NETWORK pNetworkEntry = NULL;

	

	// variables used for WlanQueryInterfaces for opcode = wlan_intf_opcode_current_connection
	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
	DWORD connectInfoSize = sizeof(WLAN_CONNECTION_ATTRIBUTES);
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;

	if(hClient == NULL)dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);

	if (dwResult != ERROR_SUCCESS) {
		//DWORD nErrorNo = GetLastError(); // 
		LPWSTR lpBuffer;
		//TCHAR lpBuffer;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_IGNORE_INSERTS |	FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			dwResult, //  GetLastError()
			LANG_NEUTRAL,
			(LPTSTR)& lpBuffer,
			0, NULL);
		wprintf(L"WlanOpenHandle failed with error(%u): %s\n", dwResult, L"");
		//wprintf(L"WlanOpenHandle failed with error(%u): %s\n", dwResult, L"test");
		return 1;
		// You can use FormatMessage here to find out why the function failed
	}

	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if (dwResult != ERROR_SUCCESS) {
		//DWORD nErrorNo = GetLastError(); // 
		LPWSTR lpBuffer;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_IGNORE_INSERTS |	FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			dwResult, //  GetLastError()
			LANG_NEUTRAL,
			(LPTSTR)& lpBuffer,
			0,
			NULL);
		wprintf(L"WlanEnumInterfaces failed with error(%u): %s\n", dwResult, lpBuffer);
		return 1;
		// You can use FormatMessage here to find out why the function failed
	}
	else {
		wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems);
		wprintf(L"Current Index: %lu\n", pIfList->dwIndex);
		for (i = 0; i < (int)pIfList->dwNumberOfItems; i++) {
			pIfInfo = (WLAN_INTERFACE_INFO *)& pIfList->InterfaceInfo[i];
			wprintf(L"  Interface Index[%u]:\t %lu\n", i, i);
			iRet =
				StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR)& GuidString,
				sizeof(GuidString) / sizeof(*GuidString));
			// For c rather than C++ source code, the above line needs to be
			// iRet = StringFromGUID2(&pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 
			//     sizeof(GuidString)/sizeof(*GuidString)); 
			if (iRet == 0)
				wprintf(L"StringFromGUID2 failed\n");
			else {
				wprintf(L"  InterfaceGUID[%d]:\t %ws\n", i, GuidString);
			}
			wprintf(L"  Interface Description[%d]: %ws", i, pIfInfo->strInterfaceDescription);
			wprintf(L"\n");
			wprintf(L"  Interface State[%d]:\t ", i);
			switch (pIfInfo->isState) {
			case wlan_interface_state_not_ready:
				wprintf(L"Not ready\n");
				break;
			case wlan_interface_state_connected:
				wprintf(L"Connected\n");
				break;
			case wlan_interface_state_ad_hoc_network_formed:
				wprintf(L"First node in a ad hoc network\n");
				break;
			case wlan_interface_state_disconnecting:
				wprintf(L"Disconnecting\n");
				break;
			case wlan_interface_state_disconnected:
				wprintf(L"Not connected\n");
				break;
			case wlan_interface_state_associating:
				wprintf(L"Attempting to associate with a network\n");
				break;
			case wlan_interface_state_discovering:
				wprintf(L"Auto configuration is discovering settings for the network\n");
				break;
			case wlan_interface_state_authenticating:
				wprintf(L"In process of authenticating\n");
				break;
			default:
				wprintf(L"Unknown state %ld\n", pIfInfo->isState);
				break;
			}
			wprintf(L"\n");

			// If interface state is connected, call WlanQueryInterface
			// to get current connection attributes
			if (pIfInfo->isState == wlan_interface_state_connected) {
				dwResult = WlanQueryInterface(hClient,
					&pIfInfo->InterfaceGuid,
					wlan_intf_opcode_current_connection,
					NULL,
					&connectInfoSize,
					(PVOID *)&pConnectInfo,
					&opCode);

				if (dwResult != ERROR_SUCCESS) {
					wprintf(L"WlanQueryInterface WLAN_CONNECTION_ATTRIBUTES failed with error: %u\n", dwResult);
					dwRetVal = 1;
					// You can use FormatMessage to find out why the function failed
				}
				else {
					wprintf(L"  WLAN_CONNECTION_ATTRIBUTES for this interface\n");

					wprintf(L"  Interface State:\t ");
					switch (pConnectInfo->isState) {
					case wlan_interface_state_not_ready:
						wprintf(L"Not ready\n");
						break;
					case wlan_interface_state_connected:
						wprintf(L"Connected\n");
						break;
					case wlan_interface_state_ad_hoc_network_formed:
						wprintf(L"First node in a ad hoc network\n");
						break;
					case wlan_interface_state_disconnecting:
						wprintf(L"Disconnecting\n");
						break;
					case wlan_interface_state_disconnected:
						wprintf(L"Not connected\n");
						break;
					case wlan_interface_state_associating:
						wprintf(L"Attempting to associate with a network\n");
						break;
					case wlan_interface_state_discovering:
						wprintf
							(L"Auto configuration is discovering settings for the network\n");
						break;
					case wlan_interface_state_authenticating:
						wprintf(L"In process of authenticating\n");
						break;
					default:
						wprintf(L"Unknown state %ld\n", pIfInfo->isState);
						break;
					}

					wprintf(L"  Connection Mode:\t ");
					switch (pConnectInfo->wlanConnectionMode) {
					case wlan_connection_mode_profile:
						wprintf(L"A profile is used to make the connection\n");
						break;
					case wlan_connection_mode_temporary_profile:
						wprintf(L"A temporary profile is used to make the connection\n");
						break;
					case wlan_connection_mode_discovery_secure:
						wprintf(L"Secure discovery is used to make the connection\n");
						break;
					case wlan_connection_mode_discovery_unsecure:
						wprintf(L"Unsecure discovery is used to make the connection\n");
						break;
					case wlan_connection_mode_auto:
						wprintf
							(L"connection initiated by wireless service automatically using a persistent profile\n");
						break;
					case wlan_connection_mode_invalid:
						wprintf(L"Invalid connection mode\n");
						break;
					default:
						wprintf(L"Unknown connection mode %ld\n",
							pConnectInfo->wlanConnectionMode);
						break;
					}

					wprintf(L"  Profile name used:\t %ws\n", pConnectInfo->strProfileName);

					wprintf(L"  Association Attributes for this connection\n");
					wprintf(L"    SSID:\t\t ");
					if (pConnectInfo->wlanAssociationAttributes.dot11Ssid.uSSIDLength == 0)
						wprintf(L"\n");
					else {
						for (k = 0;
							k < pConnectInfo->wlanAssociationAttributes.dot11Ssid.uSSIDLength;
							k++) {
							wprintf(L"%c",
								(int)pConnectInfo->wlanAssociationAttributes.dot11Ssid.ucSSID[k]);
						}
						wprintf(L"\n");
					}

					wprintf(L"    BSS Network type:\t ");
					switch (pConnectInfo->wlanAssociationAttributes.dot11BssType) {
					case dot11_BSS_type_infrastructure:
						wprintf(L"Infrastructure\n");
						break;
					case dot11_BSS_type_independent:
						wprintf(L"independent BSS\n");
						break;
					default:
						wprintf(L"Other = %lu\n",
							pConnectInfo->wlanAssociationAttributes.dot11BssType);
						break;
					}
					if (!bIsWindowsVistaorLater) {
						ShowConnectedAPwithNDIS((LPOLESTR)&GuidString);
					}
					else {
						wprintf(L"    AP MAC address:\t ");
						//global
						memset(CurrentMACAddressStr, 0, 256);
						_stprintf_s(CurrentMACAddressStr, 256, L"%02x:%02x:%02x:%02x:%02x:%02x",
							pConnectInfo->wlanAssociationAttributes.dot11Bssid[0],
							pConnectInfo->wlanAssociationAttributes.dot11Bssid[1],
							pConnectInfo->wlanAssociationAttributes.dot11Bssid[2],
							pConnectInfo->wlanAssociationAttributes.dot11Bssid[3],
							pConnectInfo->wlanAssociationAttributes.dot11Bssid[4],
							pConnectInfo->wlanAssociationAttributes.dot11Bssid[5]);
						wprintf(L"%s\n", CurrentMACAddressStr);

						//following Available in Windows Vista and later
						wprintf(L"    PHY network type:\t ");
						switch (pConnectInfo->wlanAssociationAttributes.dot11PhyType) {
						case dot11_phy_type_unknown:
							wprintf(L"Unknown");
							break;
						case dot11_phy_type_fhss:
							wprintf(L"Frequency-hopping spread-spectrum (FHSS)\n");
							break;
						case dot11_phy_type_dsss:
							wprintf(L"Direct sequence spread spectrum (DSSS)\n");
							break;
						case dot11_phy_type_irbaseband:
							wprintf(L"Infrared (IR) baseband\n");
							break;
						case dot11_phy_type_ofdm:
							wprintf(L"Orthogonal frequency division multiplexing (OFDM)\n");
							break;
						case dot11_phy_type_hrdsss:
							wprintf(L"High-rate DSSS (HRDSSS) = \n");
							break;
						case dot11_phy_type_erp:
							wprintf(L"Extended rate PHY type\n");
							break;
						case dot11_phy_type_ht:
							wprintf(L"802.11n PHY type\n");
							break;
						//case dot11_phy_type_vht: //sdk8.1 dot11_phy_type_vht:8
						//	wprintf(L"802.11ac PHY type\n");
						//	break;
						case dot11_phy_type_IHV_start:
							wprintf(L"The start of the range independent hardware vendor (IHV)\n");
							break;
						case dot11_phy_type_IHV_end:
							wprintf(L"The end of the range independent hardware vendor (IHV)\n");
							break;
						default:
							wprintf(L"Unknown = %lu\n",
								pConnectInfo->wlanAssociationAttributes.dot11PhyType);
							break;
						}

						wprintf(L"    PHY index:\t\t %u\n",
							pConnectInfo->wlanAssociationAttributes.uDot11PhyIndex);

						//TODO: RSSI
						//getRSSI(&hClient,&pIfInfo->InterfaceGuid);

						wprintf(L"    Signal Quality:\t %d\n",
							pConnectInfo->wlanAssociationAttributes.wlanSignalQuality);

						wprintf(L"    Receiving Rate:\t %ld\n",
							pConnectInfo->wlanAssociationAttributes.ulRxRate);

						wprintf(L"    Transmission Rate:\t %ld\n",
							pConnectInfo->wlanAssociationAttributes.ulTxRate);
						//TODO: channel
						//getChannel(hClient,&pIfInfo->InterfaceGuid);
						wprintf(L"\n");
					}

					wprintf(L"  Security Attributes for this connection\n");

					wprintf(L"    Security enabled:\t ");
					if (pConnectInfo->wlanSecurityAttributes.bSecurityEnabled == 0)
						wprintf(L"No\n");
					else
						wprintf(L"Yes\n");

					wprintf(L"    802.1X enabled:\t ");
					if (pConnectInfo->wlanSecurityAttributes.bOneXEnabled == 0)
						wprintf(L"No\n");
					else
						wprintf(L"Yes\n");

					wprintf(L"    Authentication Algorithm: ");
					switch (pConnectInfo->wlanSecurityAttributes.dot11AuthAlgorithm) {
					case DOT11_AUTH_ALGO_80211_OPEN:
						wprintf(L"802.11 Open\n");
						break;
					case DOT11_AUTH_ALGO_80211_SHARED_KEY:
						wprintf(L"802.11 Shared\n");
						break;
					case DOT11_AUTH_ALGO_WPA:
						wprintf(L"WPA\n");
						break;
					case DOT11_AUTH_ALGO_WPA_PSK:
						wprintf(L"WPA-PSK\n");
						break;
					case DOT11_AUTH_ALGO_WPA_NONE:
						wprintf(L"WPA-None\n");
						break;
					case DOT11_AUTH_ALGO_RSNA:
						wprintf(L"RSNA\n");
						break;
					case DOT11_AUTH_ALGO_RSNA_PSK:
						wprintf(L"RSNA with PSK\n");
						break;
					default:
						wprintf(L"Other (%lu)\n", pConnectInfo->wlanSecurityAttributes.dot11AuthAlgorithm);
						break;
					}

					wprintf(L"    Cipher Algorithm:\t ");
					switch (pConnectInfo->wlanSecurityAttributes.dot11CipherAlgorithm) {
					case DOT11_CIPHER_ALGO_NONE:
						wprintf(L"None\n");
						break;
					case DOT11_CIPHER_ALGO_WEP40:
						wprintf(L"WEP-40\n");
						break;
					case DOT11_CIPHER_ALGO_TKIP:
						wprintf(L"TKIP\n");
						break;
					case DOT11_CIPHER_ALGO_CCMP:
						wprintf(L"CCMP\n");
						break;
					case DOT11_CIPHER_ALGO_WEP104:
						wprintf(L"WEP-104\n");
						break;
					case DOT11_CIPHER_ALGO_WEP:
						wprintf(L"WEP\n");
						break;
					default:
						wprintf(L"Other (0x%x)\n", pConnectInfo->wlanSecurityAttributes.dot11CipherAlgorithm);
						break;
					}
					wprintf(L"\n");
				}
			}
			//scan first
			//test? passible cause disconnect?
			if (1) {
				DWORD dwPrevNotif = 0;
				// Scan takes awhile so we need to register a callback
				//Windows XP with SP3 and Wireless LAN API for Windows XP with SP2:  Only the wlan_notification_acm_connection_complete and wlan_notification_acm_disconnected notifications are available.
				if (dwResult = WlanRegisterNotification(hClient, WLAN_NOTIFICATION_SOURCE_ACM, TRUE,
					(WLAN_NOTIFICATION_CALLBACK)WlanNotification, NULL, NULL, &dwPrevNotif) != ERROR_SUCCESS) {
					throw("[x] Unable to register for notifications");
				}

				printf("[%d] Scanning for nearby networks...\n", i);
				//The WlanScan function returns immediately and does not provide a notification when the scan is complete on Windows XP with SP3 or the Wireless LAN API for Windows XP with SP2. (timeout after 4sec)
				//TODO: does WlanScan (OID_802_11_BSSID_LIST_SCAN) is 
				//This list includes BSSIDs for all BSSs responding on frequency channels that are permitted in the region in which the device is operating. The driver will return the contents of this list when queried by OID_802_11_BSSID_LIST.
				//use OID_DOT11_SCAN_REQUEST
				if (1) {
					if (dwResult = WlanScan(hClient, &pIfInfo->InterfaceGuid, NULL, NULL, NULL) != ERROR_SUCCESS) {
						throw("[x] Scan failed, check adapter is enabled");
					}
				}
				else {
					//not work!!
					if (DoWlanScan((LPOLESTR)&GuidString) != ERROR_SUCCESS) {
						throw("[x] Scan failed, check adapter is enabled");
					}
				}
				// 
				UINT nRet;
				MSG msg;
				if (!bIsWindowsVistaorLater) {
					printf("XP and below no support wlan_notification_acm_scan_complete\n");
					//XP just wait 5 sec
					//nRet = SetTimer(NULL, 0, 5000, &TimerProc);
				}
				int iWait = 0;
				bWait = true;
				while (bWait & (iWait <= 50)) {
					Sleep(100);
					wprintf(L".");
					//TODO: following will not let while keep out put "."
					if (!bIsWindowsVistaorLater) {
						//for SetTimer with NULL handle
						GetMessage(&msg, NULL, 0, 0);
						DispatchMessage(&msg);
					}
					iWait++;
				}

				wprintf(L"\n");
				// Unregister callback, don't care if it succeeds or not
				WlanRegisterNotification(hClient, WLAN_NOTIFICATION_SOURCE_NONE, TRUE, NULL, NULL, NULL, &dwPrevNotif);
				if (!bIsWindowsVistaorLater) {
					KillTimer(NULL, nRet);
				}
			}

			//WlanGetAvailableNetworkList
			/*
			TODO: Get profile list for this interface?
			dwResult = WlanGetAvailableNetworkList(hClient,
			&pIfInfo->InterfaceGuid,
			0, NULL, &pBssList);
			if (dwResult != ERROR_SUCCESS) {
			RETAILMSG(OUTPUT_LOGMSG,(L"WlanGetAvailableNetworkList failed with error: %u\r\n",
			dwResult));
			dwRetVal = 1;
			// You can use FormatMessage to find out why the function failed
			} else {
			RETAILMSG(OUTPUT_LOGMSG, (L"WLAN_AVAILABLE_NETWORK_LIST for this interface\r\n"));
			RETAILMSG(OUTPUT_LOGMSG,(L"Num Entries: %d\r\n", pBssList->dwNumberOfItems));

			for (j = 0; j < pBssList->dwNumberOfItems; j++)
			{
			pBssEntry = (WLAN_AVAILABLE_NETWORK *)&pBssList->Network[j];
			RETAILMSG(OUTPUT_LOGMSG,(L"  Profile Name[%u]:  %s\r\n", j, &pBssEntry->strProfileName));
			RETAILMSG(OUTPUT_LOGMSG,(L"  SSID[%u]:\t\t ", j));
			if (pBssEntry->dot11Ssid.uSSIDLength == 0)
			RETAILMSG(OUTPUT_LOGMSG,(L"\r\n"));
			else
			{
			CString str = _T("");
			str = pBssEntry->dot11Ssid.ucSSID;


			RETAILMSG(OUTPUT_LOGMSG,(L"%s\r\n", str));
			RETAILMSG(OUTPUT_LOGMSG,(L"%s\r\n", &pBssEntry->dot11Ssid.ucSSID));
			}

			RETAILMSG(OUTPUT_LOGMSG,(L"BSS Network type[%u]:\t ", j));
			switch (pBssEntry->dot11BssType)
			{
			case dot11_BSS_type_infrastructure   :
			RETAILMSG(OUTPUT_LOGMSG,(L"Infrastructure (%u)\r\n", pBssEntry->dot11BssType));
			break;
			case dot11_BSS_type_independent:
			RETAILMSG(OUTPUT_LOGMSG,(L"Infrastructure (%u)\r\n", pBssEntry->dot11BssType));
			break;
			default:
			RETAILMSG(OUTPUT_LOGMSG,(L"Other (%lu)\r\n", pBssEntry->dot11BssType));
			break;
			}

			RETAILMSG(OUTPUT_LOGMSG,(L"  Number of BSSIDs[%u]:\t %u\r\n", j, pBssEntry->uNumberOfBssids));
			RETAILMSG(OUTPUT_LOGMSG,(L"  Connectable[%u]:\t ", j));
			if (pBssEntry->bNetworkConnectable)
			RETAILMSG(OUTPUT_LOGMSG,(L"Yes\r\n"));
			else
			{
			RETAILMSG(OUTPUT_LOGMSG,(L"No\r\n"));
			RETAILMSG(OUTPUT_LOGMSG,(L" Not connectable WLAN_REASON_CODE value[%u]:\t %u\r\n", j,
			pBssEntry->wlanNotConnectableReason));
			}

			RETAILMSG(OUTPUT_LOGMSG,(L"Number of PHY types supported[%u]:\t %u\r\n", j, pBssEntry->uNumberOfPhyTypes));

			if (pBssEntry->wlanSignalQuality == 0)
			iRSSI = -100;
			else if (pBssEntry->wlanSignalQuality == 100)
			iRSSI = -50;
			else
			iRSSI = -100 + (pBssEntry->wlanSignalQuality/2);

			RETAILMSG(OUTPUT_LOGMSG,(L"  Signal Quality[%u]:\t %u (RSSI: %i dBm)\r\n", j,
			pBssEntry->wlanSignalQuality, iRSSI));

			RETAILMSG(OUTPUT_LOGMSG,(L"  Security Enabled[%u]:\t ", j));
			if (pBssEntry->bSecurityEnabled)
			RETAILMSG(OUTPUT_LOGMSG,(L"Yes\r\n"));
			else
			RETAILMSG(OUTPUT_LOGMSG,(L"No\r\n"));
			//身份驗證類型
			RETAILMSG(OUTPUT_LOGMSG,(L"  Default AuthAlgorithm[%u]: ", j));
			switch (pBssEntry->dot11DefaultAuthAlgorithm)
			{
			case DOT11_AUTH_ALGO_80211_OPEN:
			RETAILMSG(OUTPUT_LOGMSG,(L"802.11 Open (%u)\r\n", pBssEntry->dot11DefaultAuthAlgorithm));
			break;
			case DOT11_AUTH_ALGO_80211_SHARED_KEY:
			RETAILMSG(OUTPUT_LOGMSG,(L"802.11 Shared (%u)\r\n", pBssEntry->dot11DefaultAuthAlgorithm));
			break;
			case DOT11_AUTH_ALGO_WPA:
			RETAILMSG(OUTPUT_LOGMSG,(L"WPA (%u)\r\n", pBssEntry->dot11DefaultAuthAlgorithm));
			break;
			case DOT11_AUTH_ALGO_WPA_PSK:
			RETAILMSG(OUTPUT_LOGMSG,(L"WPA-PSK (%u)\r\n", pBssEntry->dot11DefaultAuthAlgorithm));
			break;
			case DOT11_AUTH_ALGO_WPA_NONE:
			RETAILMSG(OUTPUT_LOGMSG,(L"WPA-None (%u)\r\n", pBssEntry->dot11DefaultAuthAlgorithm));
			break;
			case DOT11_AUTH_ALGO_RSNA:
			RETAILMSG(OUTPUT_LOGMSG,(L"RSNA (%u)\r\n", pBssEntry->dot11DefaultAuthAlgorithm));
			break;
			case DOT11_AUTH_ALGO_RSNA_PSK:
			RETAILMSG(OUTPUT_LOGMSG,(L"RSNA with PSK(%u)\r\n", pBssEntry->dot11DefaultAuthAlgorithm));
			break;
			default:
			RETAILMSG(OUTPUT_LOGMSG,(L"Other (%lu)\r\n", pBssEntry->dot11DefaultAuthAlgorithm));
			break;
			}

			//加密類型
			RETAILMSG(OUTPUT_LOGMSG,(L"  Default CipherAlgorithm[%u]: ", j));
			switch (pBssEntry->dot11DefaultCipherAlgorithm)
			{
			case DOT11_CIPHER_ALGO_NONE:
			RETAILMSG(OUTPUT_LOGMSG,(L"None (0x%x)\r\n", pBssEntry->dot11DefaultCipherAlgorithm));
			break;
			case DOT11_CIPHER_ALGO_WEP40:
			RETAILMSG(OUTPUT_LOGMSG,(L"WEP-40 (0x%x)\r\n", pBssEntry->dot11DefaultCipherAlgorithm));
			break;
			case DOT11_CIPHER_ALGO_TKIP:
			RETAILMSG(OUTPUT_LOGMSG,(L"TKIP (0x%x)\r\n", pBssEntry->dot11DefaultCipherAlgorithm));
			break;
			case DOT11_CIPHER_ALGO_CCMP:
			RETAILMSG(OUTPUT_LOGMSG,(L"CCMP (0x%x)\r\n", pBssEntry->dot11DefaultCipherAlgorithm));
			break;
			case DOT11_CIPHER_ALGO_WEP104:
			RETAILMSG(OUTPUT_LOGMSG,(L"WEP-104 (0x%x)\r\n", pBssEntry->dot11DefaultCipherAlgorithm));
			break;
			case DOT11_CIPHER_ALGO_WEP:
			RETAILMSG(OUTPUT_LOGMSG,(L"WEP (0x%x)\r\n", pBssEntry->dot11DefaultCipherAlgorithm));
			break;
			default:
			RETAILMSG(OUTPUT_LOGMSG,(L"Other (0x%x)\r\n", pBssEntry->dot11DefaultCipherAlgorithm));
			break;
			}

			RETAILMSG(OUTPUT_LOGMSG,(L"  Flags[%u]:\t 0x%x", j, pBssEntry->dwFlags));
			if (pBssEntry->dwFlags)
			{
			if (pBssEntry->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED)
			RETAILMSG(OUTPUT_LOGMSG,(L" - Currently connected"));
			if (pBssEntry->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED)
			RETAILMSG(OUTPUT_LOGMSG,(L" - Has profile"));
			}
			RETAILMSG(OUTPUT_LOGMSG,(L"\r\n"));

			RETAILMSG(OUTPUT_LOGMSG,(L"\r\n"));
			}//for (j = 0; j < pBssList->dwNumberOfItems; j++)
			*/

			//WlanGetNetworkBssList, Vista above
			GetBList(hClient, pIfInfo );
			//TODO
			/*
			if (pBssList != NULL) {
			WlanFreeMemory(pBssList);
			pBssList = NULL;
			}
			*/
		}
	}
	if (pConnectInfo != NULL) {
		WlanFreeMemory(pConnectInfo);
		pConnectInfo = NULL;
	}

	if (pIfList != NULL) {
		WlanFreeMemory(pIfList);
		pIfList = NULL;
	}
	dwRetVal = CloseAllGUIDHandle();
	return dwRetVal;
}

//following will create a new cmd window, not good!!
/*
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nCmdShow)
{
#ifdef WITH_CONSOLE
	RedirectIOToConsole();
#endif
	//test console
	int iVar;

	// test stdio
	fprintf(stdout, "Test output to stdout\n");
	fprintf(stderr, "Test output to stderr\n");
	fprintf(stdout, "Enter an integer to test stdin: ");
	scanf("%d", &iVar);
	printf("You entered %d\n", iVar);

	//test iostreams
	cout << "Test output to cout" << endl;
	cerr << "Test output to cerr" << endl;
	clog << "Test output to clog" << endl;
	cout << "Enter an integer to test cin: ";
	cin >> iVar;
	cout << "You entered " << iVar << endl;

#ifndef _USE_OLD_IOSTREAMS
	// test wide iostreams
	wcout << L"Test output to wcout" << endl;
	wcerr << L"Test output to wcerr" << endl;
	wclog << L"Test output to wclog" << endl;
	wcout << L"Enter an integer to test wcin: ";
	wcin >> iVar;
	wcout << L"You entered " << iVar << endl;
#endif

}
*/


void WirelessMonitor::GetBList(HANDLE hClient, PWLAN_INTERFACE_INFO pIfInfo )
{
	DWORD dwResult;
	DWORD dwRetVal;
	PWLAN_BSS_LIST		pBssList = NULL;
	PWLAN_BSS_ENTRY		pBssEntry = NULL;
	bool bIsCurrentConnected = false;
	unsigned int i, j, k, l, r;
	TCHAR MACAddressStr[256];
	WLAN_RATE_SET rateSet;
	USHORT rate_in_mbps;
	USHORT maxRate_in_mbps = 0;

	char *pReserved = NULL;
			DOT11_BSS_TYPE dot11BssType = dot11_BSS_type_any;
			//TODO: NDIS_802_11_BSSID_LIST_EX ? https://msdn.microsoft.com/en-us/library/windows/hardware/ff564113%28v=vs.85%29.aspx
			dwResult = WlanGetNetworkBssList(hClient, &pIfInfo->InterfaceGuid,
				NULL, dot11BssType, NULL, pReserved, &pBssList);

			if (dwResult != ERROR_SUCCESS) {
				wprintf(L"WlanGetNetworkBssList failed with error: %u\n",
					dwResult);
				dwRetVal = 1;
				// You can use FormatMessage to find out why the function failed
			}
			else {
				wprintf(L"PWLAN_BSS_LIST for this interface\n");
				wprintf(L" %32ls  %16ls %4ls %ls %ls %ls\n", L"SSID", L"MAC", L"RSSI", L"RATE", L"TYPE", L"FREQUENCY");

				string result_ssid;
				string result_mac;

				
				RSSIs.resize(pBssList->dwNumberOfItems);
				SSIDs.resize(pBssList->dwNumberOfItems);
				Channels.resize(pBssList->dwNumberOfItems);
				ui.tableWidget->setRowCount(pBssList->dwNumberOfItems);

				for (j = 0; j < pBssList->dwNumberOfItems; j++) {

					

					bIsCurrentConnected = false;
					pBssEntry =
						(WLAN_BSS_ENTRY *)& pBssList->wlanBssEntries[j];
					//ssid
					//add empty char at begin of ssid
					for (l = 0; l <= (32 - pBssEntry->dot11Ssid.uSSIDLength); l++) {
						wprintf(L"%c", 0x20);
					}
					for (k = 0; k < pBssEntry->dot11Ssid.uSSIDLength; k++) {
						wprintf(L"%c", (int)pBssEntry->dot11Ssid.ucSSID[k]);
					}
					wprintf(L" ");
					//mac
					memset(MACAddressStr, 0, 256);
					_stprintf_s(MACAddressStr, 256, L"%02x:%02x:%02x:%02x:%02x:%02x",
						pBssEntry->dot11Bssid[0], pBssEntry->dot11Bssid[1],
						pBssEntry->dot11Bssid[2], pBssEntry->dot11Bssid[3],
						pBssEntry->dot11Bssid[4], pBssEntry->dot11Bssid[5]);
					wprintf(L"%s ", MACAddressStr);

					QString text;
					text.sprintf("%s",pBssEntry->dot11Ssid.ucSSID);
					SSIDs[j] = text;
					insertText(text, j, 1);
					
					//rssi
					RSSIs[j] = pBssEntry->lRssi;
					text.sprintf("%4ld ", pBssEntry->lRssi);
					insertText(text, j, 2);
					//rate
					rateSet = pBssEntry->wlanRateSet;
					maxRate_in_mbps = 0;
					for (r = 0; r < rateSet.uRateSetLength; r++) {
						//Mbps
						//basic rate
						rate_in_mbps = (rateSet.usRateSet[r] & 0x7FFF) * 0.5;
						if (rate_in_mbps > maxRate_in_mbps) {
							maxRate_in_mbps = rate_in_mbps;
						}
					}

					text.sprintf("%4ld ", maxRate_in_mbps);
					insertText(text, j, 3);

					//type
					switch (pBssEntry->dot11BssPhyType) {
					case dot11_phy_type_ofdm:
						text.sprintf("%ls  ", L"11a ");
						break;
					case dot11_phy_type_hrdsss:
						text.sprintf("%ls  ", L"11b ");
						break;
					case dot11_phy_type_erp:
						text.sprintf("%ls  ", L"11g ");
						break;
					case dot11_phy_type_ht:
						text.sprintf("%ls  ", L"11n ");
						break;
					//case dot11_phy_type_vht://11ac vht, sdk8.1, dot11_phy_type_vht:8
					//	wprintf(L"%ls  ", L"11ac");
					//	break;
					default:
						text.sprintf("%ls(%d)", L"N/A", pBssEntry->dot11BssPhyType);
						break;
					}

					insertText(text, j, 4);
					
					//channel
					Channels[j] = freqToChannel(pBssEntry->ulChCenterFrequency);
					text.sprintf("%4ld (%3d )", (pBssEntry->ulChCenterFrequency) / 1000, freqToChannel(pBssEntry->ulChCenterFrequency));
					insertText(text, j, 5);
					
					//current connected 
					if (!_tcscmp(MACAddressStr, CurrentMACAddressStr)) {
						wprintf(L" *");
					}
					wprintf(L"\n");
				}
				wprintf(L"*Num Entries: %lu\n", pBssList->dwNumberOfItems);
			}
}

WirelessMonitor::WirelessMonitor(QWidget *parent)
	: QWidget(parent)
{

	hClient = NULL;
	
	ui.setupUi(this);
	sp = new QSplitter(ui.widget_contain);
	sp->setOrientation(Qt::Horizontal);
	

	sp->addWidget(ui.tableWidget);

	bg = new BarGraph(this);

	
	//bg->setYlabel("RSSI");
	bg->setShow_tooltip(true);

	bg->setYlabel("RSSi [dBm]");


	QSplitter* right_sp = new QSplitter(this);
	right_sp->setOrientation(Qt::Vertical);
	sp->addWidget(right_sp);
	right_sp->addWidget(bg);
	zb_bg = new BarGraph(this);
	right_sp->addWidget(zb_bg);

	zb_bg->set_min_value(0);
	zb_bg->set_max_value(255);
	zb_bg->set_zb(true);
	zb_bg->setYlabel("Energy [0-255]");

	//sp->setSizes();
	//sp->setGeometry(0,20,500,500);

	ui.tableWidget->clear();
	ui.tableWidget->setRowCount(0);

	ui.tableWidget->setColumnCount(6);
	ui.tableWidget->setHorizontalHeaderLabels(QStringList() << "" <<"SSID" << "RSSI" << "Rate Mbps" << "Type" << "Channel" );
	//ui.tableWidget->setColumnWidth(0,200);
	//ui.tableWidget->setColumnWidth(1,50);


	ui.gridLayout_2->addWidget(sp,1,0,3,2);
	
	//doConsoleScan();

	//bg->setData(Channels, RSSIs, SSIDs, false);
	
	//bg->update();

	ui.tableWidget->resizeColumnsToContents();


	wifiLedIndicator = new LedIndicator(); //creates new instance whih off state
	ui.gridLayout_4->addWidget(wifiLedIndicator,0,1); //add it to the layout

	zigbeeLedIndicator = new LedIndicator(); //creates new instance whih off state
	ui.gridLayout_3->addWidget(zigbeeLedIndicator,0,2); //add it to the layout

	//wifiLedIndicator->setLedSize(30); //make a big led indicator;
	

	timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGraph()));

	zbtimer = new QTimer(this);
    connect(zbtimer, SIGNAL(timeout()), this, SLOT(startEneryScan()));

	ui.comboBox->clear();
	foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts())
	{
		if (port.isValid() && !port.isBusy())
			ui.comboBox->addItem(port.portName() + " (" + port.description() + ")", port.portName());
	}

	index = 0;
	total = 10;

	m_serial = nullptr;

	//onScan(true);

}

WirelessMonitor::~WirelessMonitor()
{
	bg = nullptr;
	zb_bg = nullptr;

	timer->stop();
	zbtimer->stop();

	delete timer;
    timer = nullptr;

	delete zbtimer;
    zbtimer = nullptr;

	if(m_serial)
	{
		m_serial->close();
		delete m_serial;
		m_serial = nullptr;
	}
}

void WirelessMonitor::set_port(QSerialPort * port)
{
	Q_ASSERT(port);
	m_serial = port;
	bool check = true;
	check = check &	m_serial->setBaudRate(QSerialPort::Baud38400);
	//check = check & m_serialPort->setDataBits(QSerialPort::Data8);
	//check = check & m_serialPort->setStopBits(QSerialPort::OneStop);
	//check = check & m_serialPort->setParity(QSerialPort::NoParity);
	check = check & m_serial->setFlowControl(QSerialPort::NoFlowControl);

	is_new_command = true;
	ui.comboBox->setCurrentText(port->portName());
	connect(m_serial, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	ui.zbScan->setChecked(true);
}

void WirelessMonitor::resizeEvent(QResizeEvent * event)
{
	QWidget::resizeEvent(event);
	int w = width();
	sp->setSizes(QList<int>() <<  w*0.3 << w*0.7);
	
}
void WirelessMonitor::insertText(QString text, int i, int j)
{
		QTableWidgetItem *pCell = ui.tableWidget->item(i, j);
		if(!pCell)
		{
			pCell = new QTableWidgetItem;
			ui.tableWidget->setItem(i, j, pCell);
		}
		pCell->setText(text);
}


void WirelessMonitor::onScan(bool enable)
{	
	if(enable)
	{
		timer->start(500);
	}
	else
	{
		timer->stop();
		wifiLedIndicator->setState(false);
	}
	
}

void WirelessMonitor::onConnect() 
{
	auto currentPortName = ui.comboBox->currentData().toString();
	auto serial = new QSerialPort(currentPortName,this);
	
	if (!serial->open(QIODevice::ReadWrite))
	{
		QMessageBox::critical(this, tr("Error"), serial->errorString());
		delete serial;				
		return;
	}
	m_serial = serial;

	bool check = true;		
	check = check &	m_serial->setBaudRate(QSerialPort::Baud38400);
	//check = check & m_serialPort->setDataBits(QSerialPort::Data8);
	//check = check & m_serialPort->setStopBits(QSerialPort::OneStop);
	//check = check & m_serialPort->setParity(QSerialPort::NoParity);
	check = check & m_serial->setFlowControl(QSerialPort::NoFlowControl);

	is_new_command = true;

	connect(m_serial, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	
}


void WirelessMonitor::onReadyRead()
{
	QByteArray arr = m_serial->readAll();
	

	QString str_comm;
	int m;
	for (int n=0; n< arr.length(); n++)
	{
		//str_comm += QString::number(arr[n],16) + " ";
		m = arr[n];
		zb_data.raw[index] = m;
		//str_comm = toHex(m);

		if(is_new_command && m == SYNC)
		{
			//data_received.clear();
			index  = 0;	
			//ui.terminal->insertPlainText("RX: " + str_comm );
			//data_received +=  str_comm;	
			is_new_command = false;
		}
		else
		{
			//data_received +=  str_comm;

			if(index == 3) {
				total =  (unsigned char )arr[n];
				total +=4;
				
			}else if(index == total)
			{
				index = 0;
				total = 10;
				if(zb_data.packet.OG ==0x97 && 
					zb_data.packet.OC ==0x55 && 
					zb_data.raw[4] ==0x00)
				{
					zbRSSIs.resize(zb_data.raw[6]);
					zbSSIDs.resize(zb_data.raw[6]);
					zbChannels.resize(zb_data.raw[6]);

					for(int i=0 ;i<zb_data.raw[6]; i++)
					{
						zbRSSIs[i] = (unsigned char)zb_data.raw[11+i];
						zbSSIDs[i] = "ZB-"+QString::number(i+11);
						zbChannels[i] = i+11;

					}

					if(zbRSSIs.size()>0)
						updateZBGraph();
					//AddCommItem( data_received , zb_data);
				}

				is_new_command = true;
			}
		}


		index++;

	}

	
	
}

void WirelessMonitor::onzbScan(bool enable)
{	
	if(enable)
	{
		if(m_serial ==nullptr)onConnect();		
		zbtimer->start(500);
	}
	else
	{
		zbtimer->stop();
		zigbeeLedIndicator->setState(false);
		if(m_serial)
		{
			m_serial->close();
			delete m_serial;
			m_serial = nullptr;
		}
	}

}


void WirelessMonitor::updateGraph()
{

	//ui.tableWidget->clear();
	//ui.tableWidget->setRowCount(0);
	if(bg !=nullptr)
	{
		doConsoleScan();

		bg->setData(Channels, RSSIs, SSIDs, false);
	
		bg->update();

		ui.tableWidget->resizeColumnsToContents();

		wifiLedIndicator->toggle(); //make it on/off

		QHash<QString, QColor> ColHash = bg->getColors();

		for (int i=0; i< SSIDs.size(); i++)
		{
			QTableWidgetItem *pCell = ui.tableWidget->item(i, 0);

			if(!pCell)
			{
				pCell = new QTableWidgetItem;
				ui.tableWidget->setItem(i, 0, pCell);
			}

			pCell->setBackground(ColHash[SSIDs[i]]);
		}
	}


}

void WirelessMonitor::startEneryScan()
{
	char data[10]= {0x02, 0x96, 0x54, 0x05, 0x00, 0xF8, 0xFF, 0x07, 0x02, 0xC5};
	if (m_serial != nullptr)m_serial->write(data,10);
}

void WirelessMonitor::updateZBGraph()
{
	zb_bg->setData(zbChannels, zbRSSIs, zbSSIDs, true);	
	zb_bg->update();
	ui.tableWidget->resizeColumnsToContents();
	zigbeeLedIndicator->toggle(); //make it on/off
}