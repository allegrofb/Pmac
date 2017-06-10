// Pmac.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "PmacDeviceWnd.h"


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int main()
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(nullptr);

	if (hModule != nullptr)
	{
		// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
		if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
		{
			// TODO: ���Ĵ�������Է���������Ҫ
			wprintf(L"����: MFC ��ʼ��ʧ��\n");
			nRetCode = 1;
		}
		else
		{
			// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
			CPmacDeviceWnd pmac;
			pmac.run();

			fprintf(stderr, "press x key to exit\n");
			char key = std::getchar();

			while (key != 'x')
			{
				switch (key)
				{
				case 'q':
					fprintf(stderr, "I'm a test");
					break;
				case 's':
					pmac.start();
					break;
				case 't':
					pmac.stop();
					break;
				case 'd':
					pmac.get();
					break;
				default:
					break;
				}

				key = std::getchar();
			}

			pmac.quit();
		}
	}
	else
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		wprintf(L"����: GetModuleHandle ʧ��\n");
		nRetCode = 1;
	}

	return nRetCode;
}




