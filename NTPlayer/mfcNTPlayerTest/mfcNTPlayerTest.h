
// mfcNTPlayerTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CmfcNTPlayerTestApp:
// �йش����ʵ�֣������ mfcNTPlayerTest.cpp
//

class CmfcNTPlayerTestApp : public CWinAppEx
{
public:
	CmfcNTPlayerTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CmfcNTPlayerTestApp theApp;