// SHDLNAPlayerTestDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "SHDLNAPlayer.h"
#include "afxcmn.h"
#include <map>
#include <string>

#define VOLUME_MIN		0
#define VOLUME_MAX		100

enum {
	PLAY_STATE_NONE,
	PLAY_STATE_PLAYING,
	PLAY_STATE_PAUSE,
	PLAY_STATE_STOP,
};

typedef std::map<std::string, std::string>					StringMap;
typedef std::map<std::string, std::string>::iterator		StringMapIterator;

#define WM_SHDLNAPLAYER_MSG_BASE				WM_USER + 0x100

typedef enum {
	WM_SH_DLNAPLAYER_UI_MESSAGE_NONE = WM_SHDLNAPLAYER_MSG_BASE,
	WM_SH_DLNAPLAYER_UI_MESSAGE_OPEN_MEDIA_SUCCEEDED,						// ��ý��ɹ�
	WM_SH_DLNAPLAYER_UI_MESSAGE_OPEN_MEDIA_FAILED,							// ��ý��ʧ��

	WM_SH_DLNAPLAYER_UI_MESSAGE_SEEK_SUCCEEDED,							// seek�ɹ�
	WM_SH_DLNAPLAYER_UI_MESSAGE_SEEK_FAILED,								// seekʧ��

	WM_SH_DLNAPLAYER_UI_MESSAGE_GET_MEDIA_INFO_SUCCEEDED,					// ��ȡý����Ϣ�ɹ�
	WM_SH_DLNAPLAYER_UI_MESSAGE_GET_MEDIA_INFO_FAILED,						// ��ȡý����Ϣʧ��

	WM_SH_DLNAPLAYER_UI_MESSAGE_MEDIA_TOTAL_DURATION,						// ������Ƶʱ�� ms��wParam����ʱ��(long)
	WM_SH_DLNAPLAYER_UI_MESSAGE_MEDIA_CURRENT_POS,							// ��ǰ����λ�� ms��wParam���ص�ǰλ��(long)

	WM_SH_DLNAPLAYER_UI_MESSAGE_DEVICE_LIST_UPDATED,						// �豸�б���£�wParam�����豸�б�(SH_DLNAPlayer_DeviceList*)

	WM_SH_DLNAPLAYER_UI_MESSAGE_DEVICE_CURRENT_VOLUME,						// �豸��ǰ����������wParam���ص�ǰ������int)

	WM_SH_DLNAPLAYER_UI_MESSAGE_CURRENT_DEVICE_DISCONNECT,					// ��ǰ�豸ʧȥ����

	WM_SH_DLNAPLAYER_UI_MESSAGE_NO_DEVICE_CHOOSEN,							// û��ѡ��Ҫ���ӵ��豸

	WM_SH_DLNAPLAYER_UI_MESSAGE_DEVICE_VOLUME_CHANGED,						// �豸�����ı�, wParam���ص�ǰ����(int)



} WM_SHDLNAPlayerMessage;

// CSHDLNAPlayerTestDlg dialog
class CSHDLNAPlayerTestDlg : public CDialog
{
// Construction
public:
	CSHDLNAPlayerTestDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CSHDLNAPlayerTestDlg();

// Dialog Data
	enum { IDD = IDD_SHDLNAPLAYERTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	static CSHDLNAPlayerTestDlg*	m_spThis;
	SH_DLNAPlayer_DeviceList		m_DLNADeviceList;
	CString							m_FilePath;

public:
	static void SHDLNAPlayerMessageNotifyUI(int msg, void* wParam, void* lParam);
	void OnDLNAPlayerOpenMediaSucceeded();
	void OnDLNAPlayerOpenMediaFailed();
	void OnDLNAPlayerSeekSucceeded();
	void OnDLNAPlayerSeekFailed();
	void OnDLNAPlayerGetMediaDuration(void* wParam);
	void OnDLNAPlayerGetCurrentPlayPos(void* wParam);
	void OnDLNAPlayerDeviceListUpdated(void* wParam);
	void OnDLNAPlayerGetVolume(void* wParam);
	void OnDLNAPlayerCurrentDeviceDisconnect();
	void OnDLNAPlayerNoDeviceChoosen();
	void OnDLNAPlayerDeviceVolumeChanged(void* wParam);

	afx_msg void OnBnClickedCheckEnableDlna();
	afx_msg void OnBnClickedBtnOpenLocalMedia();
	afx_msg void OnBnClickedBtnOpenOnlineMedia();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

protected:
	CListBox						m_ctrlDeviceList;
	CListBox						m_ctrlOnlineMedia;
	CSliderCtrl						m_ctrlPlayProgress;
	CString							m_strCurPlayPos;
	CString							m_strMediaDuration;
	CSliderCtrl						m_ctrlVolume;
	CButton							m_btnEnableDLNA;
	
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

protected:
	BOOL							m_bMediaDurationGot;
	long							m_MediaDuration;
	int								m_PlayState;

	StringMap						m_mapOnlineVideos;

	int								m_nVolume;
public:
	afx_msg void OnBnClickedBtnChoosedevice();

	afx_msg LRESULT OnDlnaPlayerMsgDeviceVolumeChanged(WPARAM wParam, LPARAM lParam);
};
