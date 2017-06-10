#pragma once

#include "stdafx.h"


#define COMM_EOT 0x80000000
#define COMM_TIMEOUT 0xC0000000
#define COMM_BADCKSUM 0xD0000000
#define COMM_ERROR 0xE0000000
#define COMM_FAIL 0xF0000000
#define COMM_ANYERR 0x70000000
#define COMM_UNSOLICITED 0x10000000
#define COMM_CHARS(c) (c & 0x0FFFFFFF) // Returns the number of characters
#define COMM_STATUS(c) (c & 0xF0000000) // Returns the status byte
#define IS_COMM_MORE(c) ((c & COMM_FAIL) == 0)
#define IS_COMM_EOT(c) ((c & COMM_FAIL) == COMM_EOT)
#define IS_COMM_TIMEOUT(c) ((c & COMM_FAIL) == COMM_TIMEOUT)
#define IS_COMM_BADCKSUM(c) ((c & COMM_FAIL) == COMM_BADCKSUM)
#define IS_COMM_ERROR(c) ((c & COMM_FAIL) == COMM_ERROR)
#define IS_COMM_FAIL(c) ((c & COMM_FAIL) == COMM_FAIL)
#define IS_COMM_ANYERROR(c) ((c & COMM_ANYERR) > 0)
#define IS_COMM_UNSOLICITED(c) ((c & 0xF0000000) == COMM_UNSOLICITED)


#define WM_PMAC_USER_MSG WM_USER + 100
#define PMAC_USER_START 0
#define PMAC_USER_STOP 1
#define PMAC_USER_GET 2


class CPmacDeviceWnd : public CWnd
{
	DECLARE_DYNAMIC(CPmacDeviceWnd)

public:
	CPmacDeviceWnd();
	virtual ~CPmacDeviceWnd();
	virtual void OnFinalRelease();

public:
	void run();
	static DWORD WINAPI ThreadFunc(LPVOID);

public:
	void quit();
	void start();
	void stop();
	void get();

private:
	IPmacDevicePtr _pmacDeivce;
	HANDLE _hThread;
	long _dwDevice;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnProgress(long Device, long dwPercent);
	afx_msg void OnMessage(long Device, LPCTSTR Msg, BOOL bNewLine);
	afx_msg void OnError(long Device, LPCTSTR FileName, long ErrorID, long LineNum, LPCTSTR Line);
	afx_msg void OnInterrupt(long Device, long dwLevel);
	afx_msg void OnUnSolicited(long lDevice, LPCTSTR Msg);
	afx_msg void OnDataReady(long lDevice);
	afx_msg LRESULT OnRecvUsermsg(WPARAM wParam, LPARAM lParam);
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

