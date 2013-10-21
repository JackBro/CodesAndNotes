
// mfcNTPlayerTestDlg.h : ͷ�ļ�
//

#pragma once

class CCodecsInfoDlg;

// CmfcNTPlayerTestDlg �Ի���
class CmfcNTPlayerTestDlg : public CDialog
{
    DECLARE_EASYSIZE
// ����
public:
	CmfcNTPlayerTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCNTPLAYERTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnOpenfile();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedBtnCodecsinfo();

private:
    CCodecsInfoDlg* m_CodecsInfoDlg;
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
