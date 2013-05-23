#ifndef _MP4_DOWNLOAD_TASK_H_
#define _MP4_DOWNLOAD_TASK_H_

#include <atlstr.h>
#include <vector>

typedef enum tagEMediaDataRequestSource
{
	Invalid_RequestSource = -1,
	P2P_Local_RequestSource,
	CDN_Remote_RequestSource,
	Last_RequestSource
}EMediaDataRequestSource;

//��Ƶ�汾
enum VIDEO_VERSION
{
	VER_ERROR	 = -1,
	VER_HIGH	 = 1,     //�����
	VER_NORMAL   = 2,     //�����
	VER_SUPER	 = 21 ,   //�����
	VER_ORIGINAL = 31     //ԭ����
};

using namespace std;
using namespace ATL;
//������Ϣ
typedef struct tagSHVideoAspect
{
	int				time;	//ʱ��
	CString	desc;	//����
}SHVideoAspect;

//��ƵƬ����Ϣ
typedef struct tagSHVideoSection
{
	double			duration;//ʱ��
	int				size;	 //��С
	CString	url;	 //��Ƶ��Ƭ���ļ���ַ
	CString	hashId;		 //hashId
	CString	key;		 //��Ƶ���ܴ�, ������
	CString	newAddress;	 //�¼ܹ���ƵƬ�ε�ַ
	CString	cachePath;	 //���ػ����ַ
}SHVideoSection;

//��Ƶ��Ϣ
typedef struct tagSHVideoInfo
{
	int				p2pflag;	//�����������Ĳ���
	int				vid;		//��Ƶid
	int				norVid;		//��Ӧ������VID
	int				highVid;	//��Ӧ������VID
	int				superVid;	//��Ӧ�ĳ���VID
	int             oriVid;     //ԭ��vid    
	bool			longVideo;	//�Ƿ��ǳ���Ƶ
	int				tn;			//�������ʱת�������ȷ�����(������:cdn)
	int				status;		//��Ƶ��Ϣ״̬, 1Ϊ����
	bool			play;		//1Ϊ�������� 0Ϊ����
	bool			fms;		//�Ƿ�ΪFMS��ƵԴ
	bool			fee;		//�Ƿ�Ϊ������Ƶ
	int				pid;
	int				cid;
	int				playerListId;
	int				fps;		//��Ƶ����
	int				version;	//��Ƶ�汾, 1Ϊ���� 2Ϊ����
	int				num;		//����Ƶ��ר���е�λ��
	int				st;			//Ƭͷʱ��, ��Ƭͷʹ��
	int				et;			//Ƭβʱ��, ��Ƭβʹ��
	int             systype;    // 0:vrs���� 1��bms����
	int				width;		//��Ƶ��
	int				height;		//��Ƶ��
	CString			name;		//��Ƶ����
	CString			ch;			//��Ƶ����Ƶ��
	CString			allot;		//���ȷ�������ַ
	CString			reserveIp;	//���õ��ȵ�ַ����;�ֿ�����:220.181.61.229;115.25.217.132
	CString			url;		//��Ƶ���ղ���ҳ
	CString            coverImg;   //��Ƶ����ͼ
	vector<SHVideoAspect>	aspects; //����
	vector<SHVideoSection>	sections;//��Ƶ����Ϣ
	CString			cachePath;
	CString            catcode; //vrs�е��·������
	//
	int				size;		 //������Ƶ�Ĵ�С
	bool			complete;	 //�Ƿ��������
	int				index;		 //��ǰ���ض�
	int				downloadPos; //��ǰ����λ��
	int				vidFromWeb;	 //��ҳ�洫������vid
	CString	uid;
	bool			preload;
	bool			downloadEnable;//�Ƿ���������
	bool            isMy;
	CString    source;

	bool IsPiracy() const
	{
		return  !source.IsEmpty() && (source.GetAt(0) == TCHAR('x') || source.GetAt(0) == TCHAR('X'));
	}

	CString GetUrl() const 
	{
		CString playUrl;
		if(!IsPiracy())
		{
			if(isMy)
				playUrl.Format(_T("http://tv.sohu.com/ifoxplay/?ismytv=1&vid=%d"),vid);
			else
				playUrl.Format(_T("http://tv.sohu.com/ifoxplay/?pid=%d&vid=%d"),pid,vid);
		}
		else
		{
			if(isMy)
				playUrl.Format(_T("http://ent1.3322.org/videoplay.html?ismytv=1&vid=%d"),vid);
			else
				playUrl.Format(_T("http://ent1.3322.org/videoplay.html?pid=%d&vid=%d"),pid,vid);
		}
		return playUrl;
	}

	tagSHVideoInfo()
	{
		p2pflag		= 0;
		vid			= 0;
		norVid		= 0;
		highVid		= 0;
		superVid	= 0;
		oriVid      = 0;
		longVideo	= false;
		tn			= 0;
		status		= 0;
		play		= false;
		fms			= false;
		fee			= false;
		pid			= 0;
		fps			= 0;
		version		= 0;
		num			= 0;
		st			= 0;
		et			= 0;
		size		= 0;
		complete	= false;
		index		= 0;
		downloadPos	= 0;
		cid			= 0;
		playerListId= 0;
		vidFromWeb	= 0;
		width		= 0;
		height		= 0;
		preload		= false;
		downloadEnable = true;
		isMy        = false;
	}
}SHVideoInfo;

class CMP4DownloadTask : public PLT_ThreadTask
{
public:
	
	CMP4DownloadTask(const char* url);
	virtual ~CMP4DownloadTask();

protected:

	virtual void DoRun();


protected:
	NPT_String m_Url;
	PLT_Downloader* m_Downloader;
};

#endif