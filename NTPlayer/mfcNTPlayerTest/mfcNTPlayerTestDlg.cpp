
// mfcNTPlayerTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mfcNTPlayerTest.h"
#include "CodecsInfoDlg.h"
#include "mfcNTPlayerTestDlg.h"
#include "NTPlayer.h"

#ifdef _DEBUG
#define new new
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CmfcNTPlayerTestDlg �Ի���




CmfcNTPlayerTestDlg::CmfcNTPlayerTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CmfcNTPlayerTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmfcNTPlayerTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CmfcNTPlayerTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_OPENFILE, &CmfcNTPlayerTestDlg::OnBnClickedBtnOpenfile)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_CODECSINFO, &CmfcNTPlayerTestDlg::OnBnClickedBtnCodecsinfo)
    ON_WM_SIZE()
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CmfcNTPlayerTestDlg)
    EASYSIZE(IDC_BTN_CODECSINFO, ES_KEEPSIZE, ES_BORDER, ES_BORDER, ES_KEEPSIZE, 0)
END_EASYSIZE_MAP


// CmfcNTPlayerTestDlg ��Ϣ�������

BOOL CmfcNTPlayerTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_CodecsInfoDlg = NULL;
    ntplayer_init();

    INIT_EASYSIZE;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CmfcNTPlayerTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CmfcNTPlayerTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CmfcNTPlayerTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CmfcNTPlayerTestDlg::OnBnClickedBtnOpenfile()
{
    // TODO: Add your control notification handler code here
    ntplayer_open_url("D:\\Media\\һ·��BD1280��������[www.spp54.com].rmvb");

}

void CmfcNTPlayerTestDlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: Add your message handler code here
    if (m_CodecsInfoDlg != NULL)
    {
        m_CodecsInfoDlg->DestroyWindow();
        delete m_CodecsInfoDlg;
    }
    ntplayer_uninit();
}

void CmfcNTPlayerTestDlg::OnBnClickedBtnCodecsinfo()
{
    // TODO: Add your control notification handler code here
    const char* info = NULL;
    ntplayer_get_codecs_info_string(&info);

    CStringA straInfo(info);
    CString strInfo(straInfo);

    if (m_CodecsInfoDlg == NULL)
    {
        m_CodecsInfoDlg = new CCodecsInfoDlg;
        m_CodecsInfoDlg->Create(IDD_DIALOG_CODECSINFO, this);
        m_CodecsInfoDlg->ShowWindow(SW_SHOW);
        m_CodecsInfoDlg->SetDlgItemText(IDC_EDIT_CODECSINFO, strInfo);
    }
    else
    {
        // �ж϶Ի���ɼ�״̬���ɼ���������أ����������ɼ�
        BOOL bVisible = m_CodecsInfoDlg->IsWindowVisible();
        int nCmdShow = bVisible ? SW_HIDE : SW_SHOW;
        m_CodecsInfoDlg->ShowWindow(nCmdShow);
    }
}

void CmfcNTPlayerTestDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    UPDATE_EASYSIZE;
}
