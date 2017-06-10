// PmacDeviceWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "PmacDeviceWnd.h"

IMPLEMENT_DYNAMIC(CPmacDeviceWnd, CWnd)
CPmacDeviceWnd::CPmacDeviceWnd()
{
	EnableAutomation();
}

CPmacDeviceWnd::~CPmacDeviceWnd()
{
}

void CPmacDeviceWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CPmacDeviceWnd, CWnd)
	ON_MESSAGE(WM_PMAC_USER_MSG, OnRecvUsermsg)
	ON_MESSAGE(WM_QUIT, OnRecvUsermsg)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPmacDeviceWnd, CWnd)
	DISP_FUNCTION_ID(CPmacDeviceWnd, "Message", 1, OnMessage, VT_EMPTY, VTS_I4 VTS_BSTR VTS_BOOL)
	DISP_FUNCTION_ID(CPmacDeviceWnd, "Progress", 2, OnProgress, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CPmacDeviceWnd, "Error", 3, OnError, VT_EMPTY, VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CPmacDeviceWnd, "Interrupt", 4, OnInterrupt, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CPmacDeviceWnd, "UnSolicited", 5, OnUnSolicited, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CPmacDeviceWnd, "DataReady", 6, OnDataReady, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

// Note: we add support for IID_IPmacDeviceEvents to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {25C3A48B-AD8D-41E6-9D97-6969085AE3EC}
/* Comment out Not Needed --- static const IID IID_IPmacDeviceEvents =
{ 0x25C3A48B, 0xAD8D, 0x41E6, { 0x9D, 0x97, 0x69, 0x69, 0x8, 0x5A, 0xE3, 0xEC } };
*/

static const IID IID_IPmacDeviceEvents =
{ 0xd1c5c7c8, 0x5e0c, 0x495e,{ 0x9a, 0xaf, 0x85, 0xb1, 0x46, 0xda, 0x5f, 0x13 } };
BEGIN_INTERFACE_MAP(CPmacDeviceWnd, CWnd)
	INTERFACE_PART(CPmacDeviceWnd, IID_IPmacDeviceEvents, Dispatch)
END_INTERFACE_MAP()

void CPmacDeviceWnd::OnMessage(long lDevice, LPCTSTR Msg, BOOL bNewLine)
{
	fprintf(stderr,"OnMessage %ws", Msg);

}

void CPmacDeviceWnd::OnProgress(long lDevice, long dwPercent)
{
	fprintf(stderr, "OnProgress %d", dwPercent);

}

void CPmacDeviceWnd::OnError(long lDevice, LPCTSTR FileName, long ErrorID, long LineNum, LPCTSTR Line)
{
	fprintf(stderr, "OnError %ws", FileName);
}

void CPmacDeviceWnd::OnInterrupt(long lDevice, long dwLevel)
{
	fprintf(stderr, "OnInterrupt %d", dwLevel);

	LPCTSTR Msg;
	switch (dwLevel)
	{
	//case 0: Msg = "Interrupt>> In Position"; break;
	//case 1: Msg = "Interrupt>> Buffer Request"; break;
	//case 2: Msg = "Interrupt>> Fatal Following Error"; break;
	//case 3: Msg = "Interrupt>> Warning Following Error"; break;
	//case 4: Msg = "Interrupt>> Host Request"; break;
	//case 5: Msg = "Interrupt>> Custom IR5"; break;
	//case 6: Msg = "Interrupt>> Custom IR6"; break;
	//case 7: Msg = "Interrupt>> Custom IR7"; break;
	//default: Msg = "Interrupt>> Unknown"; break;
	}
}

void CPmacDeviceWnd::OnUnSolicited(long lDevice, LPCTSTR Msg)
{
	fprintf(stderr, "OnUnSolicited %ws", Msg);
}

void CPmacDeviceWnd::OnDataReady(long lDevice)
{
	fprintf(stderr, "OnDataReady %d", lDevice);
}

void CPmacDeviceWnd::run()
{
	_hThread = CreateThread(NULL, 0, ThreadFunc, this, 0, NULL);
}

DWORD WINAPI CPmacDeviceWnd::ThreadFunc(LPVOID lpParam)
{
	fprintf(stderr, "thread start\n");

	CPmacDeviceWnd *thisPtr = (CPmacDeviceWnd*)lpParam;

	thisPtr->_dwDevice = 0;

	VARIANT_BOOL bSuccess;
	BSTR szTest1 = ::SysAllocString(L"");;
	long status;

	CoInitialize(NULL);

	CString wnd_class_name = ::AfxRegisterWndClass(NULL);

	//CString wnd_class_name = AfxRegisterWndClass(
	//	CS_VREDRAW | CS_HREDRAW,
	//	::LoadCursor(NULL, IDC_ARROW),
	//	(HBRUSH) ::GetStockObject(WHITE_BRUSH),
	//	::LoadIcon(NULL, IDI_APPLICATION));

	BOOL created = thisPtr->CreateEx(0, wnd_class_name,
		CString("CMyMessageOnlyWindowClass"), 0
		, 0
		, 0
		, 0
		, 0
		, HWND_MESSAGE, 0);

	if (!created)
	{
		CoUninitialize();
		fprintf(stderr, "create windows fail\n");
		return 0;
	}

	thisPtr->_pmacDeivce.CreateInstance(__uuidof(PmacDevice));

	//spPmacDeivce->SelectDevice(NULL, &dwDevice, &bSuccess);
	//if (dwDevice < 0 || dwDevice >  7)
	//{
	//	fprintf(stderr, "Bad device number\n");
	//	spPmacDeivce.Release();
	//	CoUninitialize();
	//	return 0;
	//}

	thisPtr->_pmacDeivce->Open(thisPtr->_dwDevice, &bSuccess);
	if (!bSuccess)
	{
		fprintf(stderr, "Pmac could not be opened\n");
		thisPtr->_pmacDeivce.Release();
		CoUninitialize();
		return 0;
	}

	DWORD dwCookie;
	IID iid = IID_IUnknown;
	LPDISPATCH pUnkSink = thisPtr->GetIDispatch(FALSE);

	if (!AfxConnectionAdvise(thisPtr->_pmacDeivce, IID_IPmacDeviceEvents, pUnkSink, FALSE, &dwCookie))
	{
		fprintf(stderr, "AfxConnectionAdvise failed\n");
		return 0;
	}

	thisPtr->_pmacDeivce->put_InterruptMask(thisPtr->_dwDevice, 0); // Mask all interrupts
	thisPtr->_pmacDeivce->put_InterruptsEnabled(thisPtr->_dwDevice, true); // Enable Interrupts

	MSG msg;
	while (::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	thisPtr->_pmacDeivce->put_InterruptsEnabled(thisPtr->_dwDevice, false);
	AfxConnectionUnadvise(thisPtr->_pmacDeivce, IID_IPmacDeviceEvents, pUnkSink, FALSE, dwCookie);

	thisPtr->DestroyWindow();


	thisPtr->_pmacDeivce->Close(thisPtr->_dwDevice);
	thisPtr->_pmacDeivce.Release();
	CoUninitialize();

	fprintf(stderr, "thread exit\n");

	return 0;
}

void CPmacDeviceWnd::quit()
{
	::PostThreadMessage(GetThreadId(this->_hThread),WM_QUIT,0,0);
	WaitForSingleObject(this->_hThread, INFINITE);
}

void CPmacDeviceWnd::start()
{
	::PostMessage(this->m_hWnd, WM_PMAC_USER_MSG, PMAC_USER_START, 0);
}

void CPmacDeviceWnd::stop()
{
	::PostMessage(this->m_hWnd, WM_PMAC_USER_MSG, PMAC_USER_STOP, 0);
}

void CPmacDeviceWnd::get()
{
	::PostMessage(this->m_hWnd, WM_PMAC_USER_MSG, PMAC_USER_GET, 0);
}

LRESULT CPmacDeviceWnd::OnRecvUsermsg(WPARAM wParam, LPARAM lParam)
{
	fprintf(stderr, "OnRecvUsermsg %u\n", unsigned int(wParam));

	if (wParam == PMAC_USER_START)
	{
		BSTR szTest1 = ::SysAllocString(L"");;
		long status;

		this->_pmacDeivce->GetResponseEx(this->_dwDevice, _T("m3334=1"), 0, &szTest1, &status);
		if (IS_COMM_ANYERROR(status))
		{
			fprintf(stderr, "Fatal Error %d", status);
		}
		else
		{
			fprintf(stderr, "start cmd success\n");
		}

		return 0;
	}
	else if (wParam == PMAC_USER_STOP)
	{
		BSTR szTest1 = ::SysAllocString(L"");;
		long status;

		this->_pmacDeivce->GetResponseEx(this->_dwDevice, _T("m3334=0"), 0, &szTest1, &status);
		if (IS_COMM_ANYERROR(status))
		{
			fprintf(stderr, "Fatal Error %d", status);
		}
		else
		{
			fprintf(stderr, "stop cmd success\n");
		}
		return 0;
	}
	else if (wParam == PMAC_USER_GET)
	{
		BSTR szTest1 = ::SysAllocString(L"");;
		long status;

		this->_pmacDeivce->GetResponseEx(this->_dwDevice, _T("M103"), 0, &szTest1, &status);
		if (IS_COMM_ANYERROR(status))
		{
			fprintf(stderr, "Fatal Error %d", status);
		}
		else
		{
			fprintf(stderr, "get cmd success\n");
		}

	}

	return 0;
}


