/* 
 *	Copyright (C) 2003-2006 Gabest
 *	http://www.gabest.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "stdafx.h"
#include "DirectVobSub.h"
#include "VSFilter.h"

CDirectVobSub::CDirectVobSub()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BYTE* pData = NULL;
	UINT nSize = 0;

    m_supported_filter_verion = theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_SUPPORTED_VERSION), 0);
    m_config_info_version = theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_VERSION), 0);

    m_iSelectedLanguage = 0;
    m_fHideSubtitles = !!theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_HIDE), 0);
	m_fDoPreBuffering = !!theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_DOPREBUFFERING), 1);
    
    m_colorSpace = GetCompatibleProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_COLOR_SPACE), CDirectVobSub::YuvMatrix_AUTO);
    if( m_colorSpace!=CDirectVobSub::YuvMatrix_AUTO && 
        m_colorSpace!=CDirectVobSub::BT_601 && 
        m_colorSpace!=CDirectVobSub::BT_709 &&
        m_colorSpace!=CDirectVobSub::GUESS)
    {
        m_colorSpace = CDirectVobSub::YuvMatrix_AUTO;
    }
    m_yuvRange = GetCompatibleProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_YUV_RANGE), CDirectVobSub::YuvRange_Auto);
    if( m_yuvRange!=CDirectVobSub::YuvRange_Auto && 
        m_yuvRange!=CDirectVobSub::YuvRange_PC && 
        m_yuvRange!=CDirectVobSub::YuvRange_TV )
    {
        m_yuvRange = CDirectVobSub::YuvRange_Auto;
    }

    m_bt601Width = theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_BT601_WIDTH), 1024);
    m_bt601Height = theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_BT601_HEIGHT), 600);

	m_fOverridePlacement = !!theApp.GetProfileInt(ResStr(IDS_R_TEXT), ResStr(IDS_RT_OVERRIDEPLACEMENT), 0);
	m_PlacementXperc = theApp.GetProfileInt(ResStr(IDS_R_TEXT), ResStr(IDS_RT_XPERC), 50);
	m_PlacementYperc = theApp.GetProfileInt(ResStr(IDS_R_TEXT), ResStr(IDS_RT_YPERC), 90);
	m_fBufferVobSub = !!theApp.GetProfileInt(ResStr(IDS_R_VOBSUB), ResStr(IDS_RV_BUFFER), 1);
	m_fOnlyShowForcedVobSubs = !!theApp.GetProfileInt(ResStr(IDS_R_VOBSUB), ResStr(IDS_RV_ONLYSHOWFORCEDSUBS), 0);
	m_fPolygonize = !!theApp.GetProfileInt(ResStr(IDS_R_VOBSUB), ResStr(IDS_RV_POLYGONIZE), 0);
	m_defStyle <<= theApp.GetProfileString(ResStr(IDS_R_TEXT), ResStr(IDS_RT_STYLE), _T(""));
	m_fFlipPicture = !!theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_FLIPPICTURE), 0);
	m_fFlipSubtitles = !!theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_FLIPSUBTITLES), 0);
	m_fOSD = !!theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_SHOWOSDSTATS), 0);
	m_fSaveFullPath = !!theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_SAVEFULLPATH), 0);
	m_nReloaderDisableCount = !!theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_DISABLERELOADER), 0) ? 1 : 0;
	m_SubtitleDelay = theApp.GetProfileInt(ResStr(IDS_R_TIMING), ResStr(IDS_RTM_SUBTITLEDELAY), 0);
	m_SubtitleSpeedMul = theApp.GetProfileInt(ResStr(IDS_R_TIMING), ResStr(IDS_RTM_SUBTITLESPEEDMUL), 1000);
	m_SubtitleSpeedDiv = theApp.GetProfileInt(ResStr(IDS_R_TIMING), ResStr(IDS_RTM_SUBTITLESPEEDDIV), 1000);
	m_fMediaFPSEnabled = !!theApp.GetProfileInt(ResStr(IDS_R_TIMING), ResStr(IDS_RTM_MEDIAFPSENABLED), 0);
	m_ePARCompensationType = static_cast<CSimpleTextSubtitle::EPARCompensationType>(theApp.GetProfileInt(ResStr(IDS_R_TEXT), ResStr(IDS_RT_AUTOPARCOMPENSATION), 0));
    m_fHideTrayIcon =  !!theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_HIDE_TRAY_ICON), 0);

    m_overlay_cache_max_item_num = theApp.GetProfileInt(ResStr(IDS_R_PERFORMANCE), ResStr(IDS_RP_OVERLAY_CACHE_MAX_ITEM_NUM)
        , CacheManager::OVERLAY_CACHE_ITEM_NUM);
    if(m_overlay_cache_max_item_num<0) m_overlay_cache_max_item_num = 0;

    m_overlay_no_blur_cache_max_item_num = theApp.GetProfileInt(ResStr(IDS_R_PERFORMANCE), ResStr(IDS_RP_OVERLAY_NO_BLUR_CACHE_MAX_ITEM_NUM)
        , CacheManager::OVERLAY_NO_BLUR_CACHE_ITEM_NUM);
    if(m_overlay_no_blur_cache_max_item_num<0) m_overlay_no_blur_cache_max_item_num = 0;

    m_scan_line_data_cache_max_item_num = theApp.GetProfileInt(ResStr(IDS_R_PERFORMANCE), ResStr(IDS_RP_SCAN_LINE_DATA_CACHE_MAX_ITEM_NUM)
        , CacheManager::SCAN_LINE_DATA_CACHE_ITEM_NUM);
	if(m_scan_line_data_cache_max_item_num<0) m_scan_line_data_cache_max_item_num = 0;
    
    m_path_data_cache_max_item_num = theApp.GetProfileInt(ResStr(IDS_R_PERFORMANCE), ResStr(IDS_RP_PATH_DATA_CACHE_MAX_ITEM_NUM)
        , CacheManager::PATH_CACHE_ITEM_NUM);
    if(m_path_data_cache_max_item_num<0) m_path_data_cache_max_item_num = 0;

    m_subpixel_pos_level = theApp.GetProfileInt(ResStr(IDS_R_PERFORMANCE), ResStr(IDS_RP_SUBPIXEL_POS_LEVEL), SubpixelPositionControler::EIGHT_X_EIGHT);
    if(m_subpixel_pos_level<0) m_subpixel_pos_level=0;
    else if(m_subpixel_pos_level>=SubpixelPositionControler::MAX_COUNT) m_subpixel_pos_level=SubpixelPositionControler::EIGHT_X_EIGHT;

    m_fFollowUpstreamPreferredOrder = !!theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_USE_UPSTREAM_PREFERRED_ORDER), true);
    // get output colorspace config
    if(pData)
    {
        delete [] pData;
        pData = NULL;
    }
    if(theApp.GetProfileBinary(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_OUTPUT_COLORFORMATS), &pData, &nSize)
        && pData && nSize == 2*GetOutputColorSpaceNumber())
    {
        for (unsigned i=0;i<nSize/2;i++)
        {
            m_outputColorSpace[i] = static_cast<ColorSpaceId>(pData[2*i]);
            m_selectedOutputColorSpace[i] = !!pData[2*i+1];
        }        
    }
    else
    {
        for (unsigned i=0;i<GetOutputColorSpaceNumber();i++)
        {
            m_outputColorSpace[i] = static_cast<ColorSpaceId>(i);
            m_selectedOutputColorSpace[i] = true;
        }
    }

    // get input colorspace config
    if(pData)
    {
        delete [] pData;
        pData = NULL;
    }
    if(theApp.GetProfileBinary(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_INPUT_COLORFORMATS), &pData, &nSize)
        && pData && nSize == 2*GetInputColorSpaceNumber())
    {
        for (unsigned i=0;i<nSize/2;i++)
        {
            m_inputColorSpace[i] = static_cast<ColorSpaceId>(pData[2*i]);
            m_selectedInputColorSpace[i] = !!pData[2*i+1];
        }        
    }
    else
    {
        for (unsigned i=0;i<GetOutputColorSpaceNumber();i++)
        {
            m_inputColorSpace[i] = static_cast<ColorSpaceId>(i);
            m_selectedInputColorSpace[i] = true;
        }
    }

    //
    if(pData)
    {
        delete [] pData;
        pData = NULL;
    }
	if(theApp.GetProfileBinary(ResStr(IDS_R_TIMING), ResStr(IDS_RTM_MEDIAFPS), &pData, &nSize) && pData)
	{
		if(nSize != sizeof(m_MediaFPS)) m_MediaFPS = 25.0;
		else memcpy(&m_MediaFPS, pData, sizeof(m_MediaFPS));
	}
	m_ZoomRect.left = m_ZoomRect.top = 0;
	m_ZoomRect.right = m_ZoomRect.bottom = 1;

	m_fForced = false;
    if(pData)
    {
        delete [] pData;
        pData = NULL;
    }
}

CDirectVobSub::~CDirectVobSub()
{
	CAutoLock cAutoLock(&m_propsLock);
}

STDMETHODIMP CDirectVobSub::get_FileName(WCHAR* fn)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(!fn) return E_POINTER;

#ifdef UNICODE
	wcscpy(fn, m_FileName);
#else
	mbstowcs(fn, m_FileName, m_FileName.GetLength()+1);
#endif

    return S_OK;
}

STDMETHODIMP CDirectVobSub::put_FileName(WCHAR* fn)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(!fn) return E_POINTER;

	CString tmp = fn;
	if(!m_FileName.Left(m_FileName.ReverseFind('.')+1).CompareNoCase(tmp.Left(tmp.ReverseFind('.')+1))) return S_FALSE;

#ifdef UNICODE
	m_FileName = fn;
#else
	CHARSETINFO cs={0};
	::TranslateCharsetInfo((DWORD *)DEFAULT_CHARSET, &cs, TCI_SRCCHARSET);
	CHAR* buff = m_FileName.GetBuffer(MAX_PATH*2);
	int len = WideCharToMultiByte(cs.ciACP/*CP_OEMCP*/, NULL, fn, -1, buff, MAX_PATH*2, NULL, NULL);
	m_FileName.ReleaseBuffer(len+1);
#endif

	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_LanguageCount(int* nLangs)
{
	CAutoLock cAutoLock(&m_propsLock);

	return nLangs ? *nLangs = 0, S_OK : E_POINTER;
}

STDMETHODIMP CDirectVobSub::get_LanguageName(int iLanguage, WCHAR** ppName)
{
	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_SelectedLanguage(int* iSelected)
{
	CAutoLock cAutoLock(&m_propsLock);

	return iSelected ? *iSelected = m_iSelectedLanguage, S_OK : E_POINTER;
}

STDMETHODIMP CDirectVobSub::put_SelectedLanguage(int iSelected)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(m_iSelectedLanguage == iSelected) return S_FALSE;

	int nCount;
	if(FAILED(get_LanguageCount(&nCount))
	|| iSelected < 0 
	|| iSelected >= nCount) 
		return E_FAIL;

	m_iSelectedLanguage = iSelected;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_HideSubtitles(bool* fHideSubtitles)
{
	CAutoLock cAutoLock(&m_propsLock);

	return fHideSubtitles ? *fHideSubtitles = m_fHideSubtitles, S_OK : E_POINTER;
}

STDMETHODIMP CDirectVobSub::put_HideSubtitles(bool fHideSubtitles)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(m_fHideSubtitles == fHideSubtitles) return S_FALSE;

	m_fHideSubtitles = fHideSubtitles;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_PreBuffering(bool* fDoPreBuffering)
{
	CAutoLock cAutoLock(&m_propsLock);

	//return fDoPreBuffering ? *fDoPreBuffering = m_fDoPreBuffering, S_OK : E_POINTER;
    return fDoPreBuffering ? *fDoPreBuffering = false, S_OK : E_POINTER;
}

STDMETHODIMP CDirectVobSub::put_PreBuffering(bool fDoPreBuffering)
{
    return E_NOTIMPL;
	//CAutoLock cAutoLock(&m_propsLock);

	//if(m_fDoPreBuffering == fDoPreBuffering) return S_FALSE;

	//m_fDoPreBuffering = fDoPreBuffering;

	//return S_OK;
}

STDMETHODIMP CDirectVobSub::get_ColorSpace(int* colorSpace)
{
    CAutoLock cAutoLock(&m_propsLock);

    return colorSpace ? *colorSpace = m_colorSpace, S_OK : E_POINTER;
}

STDMETHODIMP CDirectVobSub::put_ColorSpace(int colorSpace)
{
    CAutoLock cAutoLock(&m_propsLock);

    if(m_colorSpace == colorSpace) return S_FALSE;

    m_colorSpace = colorSpace;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::get_YuvRange(int* yuvRange)
{
    CAutoLock cAutoLock(&m_propsLock);

    return yuvRange ? *yuvRange = m_yuvRange, S_OK : E_POINTER;
}

STDMETHODIMP CDirectVobSub::put_YuvRange(int yuvRange)
{
    CAutoLock cAutoLock(&m_propsLock);

    if(m_yuvRange == yuvRange) return S_FALSE;

    m_yuvRange = yuvRange;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::get_Placement(bool* fOverridePlacement, int* xperc, int* yperc)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(fOverridePlacement) *fOverridePlacement = m_fOverridePlacement;
	if(xperc) *xperc = m_PlacementXperc;
	if(yperc) *yperc = m_PlacementYperc;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::put_Placement(bool fOverridePlacement, int xperc, int yperc)
{
	DbgLog((LOG_TRACE, 3, "%s(%d): %s", __FILE__, __LINE__, __FUNCTION__));	
	CAutoLock cAutoLock(&m_propsLock);

	DbgLog((LOG_TRACE, 3, "\tfOverridePlacement:%d xperc:%d yperc:%d", (int)fOverridePlacement, xperc, yperc));
	DbgLog((LOG_TRACE, 3, "\tm_fOverridePlacement:%d m_PlacementXperc:%d m_PlacementYperc:%d", (int)m_fOverridePlacement, m_PlacementXperc, m_PlacementYperc));
	if(m_fOverridePlacement == fOverridePlacement && m_PlacementXperc == xperc && m_PlacementYperc == yperc) return S_FALSE;

	m_fOverridePlacement = fOverridePlacement;
	m_PlacementXperc = xperc;
	m_PlacementYperc = yperc;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_VobSubSettings(bool* fBuffer, bool* fOnlyShowForcedSubs, bool* fPolygonize)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(fBuffer) *fBuffer = m_fBufferVobSub;
	if(fOnlyShowForcedSubs) *fOnlyShowForcedSubs = m_fOnlyShowForcedVobSubs;
	if(fPolygonize) *fPolygonize = m_fPolygonize;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::put_VobSubSettings(bool fBuffer, bool fOnlyShowForcedSubs, bool fPolygonize)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(m_fBufferVobSub == fBuffer && m_fOnlyShowForcedVobSubs == fOnlyShowForcedSubs && m_fPolygonize == fPolygonize) return S_FALSE;

	m_fBufferVobSub = fBuffer;
	m_fOnlyShowForcedVobSubs = fOnlyShowForcedSubs;
	m_fPolygonize = fPolygonize;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_TextSettings(void* lf, int lflen, COLORREF* color, bool* fShadow, bool* fOutline, bool* fAdvancedRenderer)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(lf)
	{
		if(lflen == sizeof(LOGFONTA))
			strncpy_s(((LOGFONTA*)lf)->lfFaceName, LF_FACESIZE, CStringA(m_defStyle.fontName), _TRUNCATE);
		else if(lflen == sizeof(LOGFONTW))
			wcsncpy_s(((LOGFONTW*)lf)->lfFaceName, LF_FACESIZE, CStringW(m_defStyle.fontName), _TRUNCATE);
		else
			return E_INVALIDARG;

		((LOGFONT*)lf)->lfCharSet = m_defStyle.charSet;
		((LOGFONT*)lf)->lfItalic = m_defStyle.fItalic;
		((LOGFONT*)lf)->lfHeight = m_defStyle.fontSize;
		((LOGFONT*)lf)->lfWeight = m_defStyle.fontWeight;
		((LOGFONT*)lf)->lfStrikeOut = m_defStyle.fStrikeOut;
		((LOGFONT*)lf)->lfUnderline = m_defStyle.fUnderline;
	}

	if(color) *color = m_defStyle.colors[0];
	if(fShadow) *fShadow = (m_defStyle.shadowDepthX>0) || (m_defStyle.shadowDepthY>0);
	if(fOutline) *fOutline = (m_defStyle.outlineWidthX>0) || (m_defStyle.outlineWidthY>0);
	if(fAdvancedRenderer) *fAdvancedRenderer = m_fAdvancedRenderer;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::put_TextSettings(void* lf, int lflen, COLORREF color, bool fShadow, bool fOutline, bool fAdvancedRenderer)
{
	CAutoLock cAutoLock(&m_propsLock);
    STSStyle tmp = m_defStyle;
	if(lf)
	{
		if(lflen == sizeof(LOGFONTA))
			tmp.fontName = ((LOGFONTA*)lf)->lfFaceName;
		else if(lflen == sizeof(LOGFONTW))
			tmp.fontName = ((LOGFONTW*)lf)->lfFaceName;
		else
			return E_INVALIDARG;

		tmp.charSet = ((LOGFONT*)lf)->lfCharSet;
		tmp.fItalic = !!((LOGFONT*)lf)->lfItalic;
		tmp.fontSize = ((LOGFONT*)lf)->lfHeight;
		tmp.fontWeight = ((LOGFONT*)lf)->lfWeight;
		tmp.fStrikeOut = !!((LOGFONT*)lf)->lfStrikeOut;
		tmp.fUnderline = !!((LOGFONT*)lf)->lfUnderline;

		if(tmp.fontSize < 0)
		{
			HDC hdc = ::GetDC(0);
			m_defStyle.fontSize = -m_defStyle.fontSize * 72 / GetDeviceCaps(hdc, LOGPIXELSY);
			::ReleaseDC(0, hdc);
		}

	}

	tmp.colors[0] = color;
	tmp.shadowDepthX = tmp.shadowDepthY = fShadow?2:0;
	tmp.outlineWidthX = tmp.outlineWidthY = fOutline?2:0;

    if(tmp==m_defStyle) 
    {
        return S_FALSE;//Important! Avoid unnecessary deinit
    }
    else 
    {
        m_defStyle = tmp;
        return S_OK;
    }

}

STDMETHODIMP CDirectVobSub::get_Flip(bool* fPicture, bool* fSubtitles)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(fPicture) *fPicture = m_fFlipPicture;
	if(fSubtitles) *fSubtitles = m_fFlipSubtitles;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::put_Flip(bool fPicture, bool fSubtitles)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(m_fFlipPicture == fPicture && m_fFlipSubtitles == fSubtitles) return S_FALSE;

	m_fFlipPicture = fPicture;
	m_fFlipSubtitles = fSubtitles;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_OSD(bool* fOSD)
{
	CAutoLock cAutoLock(&m_propsLock);

	return fOSD ? *fOSD = m_fOSD, S_OK : E_POINTER;
}

STDMETHODIMP CDirectVobSub::put_OSD(bool fOSD)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(m_fOSD == fOSD) return S_FALSE;

	m_fOSD = fOSD;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_SaveFullPath(bool* fSaveFullPath)
{
	CAutoLock cAutoLock(&m_propsLock);

	return fSaveFullPath ? *fSaveFullPath = m_fSaveFullPath, S_OK : E_POINTER;
}

STDMETHODIMP CDirectVobSub::put_SaveFullPath(bool fSaveFullPath)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(m_fSaveFullPath == fSaveFullPath) return S_FALSE;

	m_fSaveFullPath = fSaveFullPath;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_SubtitleTiming(int* delay, int* speedmul, int* speeddiv)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(delay) *delay = m_SubtitleDelay;
	if(speedmul) *speedmul = m_SubtitleSpeedMul;
	if(speeddiv) *speeddiv = m_SubtitleSpeedDiv;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::put_SubtitleTiming(int delay, int speedmul, int speeddiv)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(m_SubtitleDelay == delay && m_SubtitleSpeedMul == speedmul && m_SubtitleSpeedDiv == speeddiv) return S_FALSE;

	m_SubtitleDelay = delay;
	m_SubtitleSpeedMul = speedmul;
	if(speeddiv > 0) m_SubtitleSpeedDiv = speeddiv;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_MediaFPS(bool* fEnabled, double* fps)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(fEnabled) *fEnabled = m_fMediaFPSEnabled;
	if(fps) *fps = m_MediaFPS;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::put_MediaFPS(bool fEnabled, double fps)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(m_fMediaFPSEnabled == fEnabled && m_MediaFPS == fps) return S_FALSE;

	m_fMediaFPSEnabled = fEnabled;
	if(fps > 0) m_MediaFPS = fps;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_ZoomRect(NORMALIZEDRECT* rect)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(!rect) return E_POINTER;

	*rect = m_ZoomRect;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::put_ZoomRect(NORMALIZEDRECT* rect)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(!rect) return E_POINTER;

	if(!memcmp(&m_ZoomRect, rect, sizeof(m_ZoomRect))) return S_FALSE;

	m_ZoomRect = *rect;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_OutputColorFormat(ColorSpaceId* preferredOrder, bool* fSelected, UINT* count)
{
    CAutoLock cAutoLock(&m_propsLock);
    if(count)
    {
        *count = GetOutputColorSpaceNumber();
    }
    if(preferredOrder)
    {
        memcpy(preferredOrder, m_outputColorSpace, GetOutputColorSpaceNumber()*sizeof(*preferredOrder));
    }
    if(fSelected)
    {
        memcpy(fSelected, m_selectedOutputColorSpace, GetOutputColorSpaceNumber()*sizeof(*fSelected));
    }

    return S_OK;
}

STDMETHODIMP CDirectVobSub::put_OutputColorFormat( const ColorSpaceId* preferredOrder, const bool* fSelected, UINT count )
{
    CAutoLock cAutoLock(&m_propsLock);

    if( count!=GetOutputColorSpaceNumber() )
        return S_FALSE;
    if( (preferredOrder==NULL || !memcmp(preferredOrder, m_outputColorSpace, count*sizeof(*preferredOrder)))
        && (fSelected==NULL || !memcmp(fSelected, m_selectedOutputColorSpace, count*sizeof(*fSelected))) )
        return S_FALSE;
    if(preferredOrder)
        memcpy(m_outputColorSpace, preferredOrder, count*sizeof(*preferredOrder));
    if(fSelected)
        memcpy(m_selectedOutputColorSpace, fSelected, count*sizeof(*fSelected));

    return S_OK;
}

STDMETHODIMP CDirectVobSub::get_InputColorFormat( ColorSpaceId* preferredOrder, bool* fSelected, UINT* count )
{
    CAutoLock cAutoLock(&m_propsLock);
    if(count)
    {
        *count = GetInputColorSpaceNumber();
    }
    if(preferredOrder)
    {
        memcpy(preferredOrder, m_inputColorSpace, GetOutputColorSpaceNumber()*sizeof(*preferredOrder));
    }
    if(fSelected)
    {
        memcpy(fSelected, m_selectedInputColorSpace, GetOutputColorSpaceNumber()*sizeof(*fSelected));
    }

    return S_OK;
}

STDMETHODIMP CDirectVobSub::put_InputColorFormat( const ColorSpaceId* preferredOrder, const bool* fSelected, UINT count )
{
    CAutoLock cAutoLock(&m_propsLock);

    if( count!=GetInputColorSpaceNumber() )
        return S_FALSE;
    if( (preferredOrder==NULL || !memcmp(preferredOrder, m_inputColorSpace, count*sizeof(*preferredOrder)))
        && (fSelected==NULL || !memcmp(fSelected, m_selectedInputColorSpace, count*sizeof(*fSelected))) )
        return S_FALSE;
    if(preferredOrder)
        memcpy(m_inputColorSpace, preferredOrder, count*sizeof(*preferredOrder));
    if(fSelected)
        memcpy(m_selectedInputColorSpace, fSelected, count*sizeof(*fSelected));

    return S_OK;
}

STDMETHODIMP CDirectVobSub::get_OverlayCacheMaxItemNum(int* overlay_cache_max_item_num)
{
    CAutoLock cAutoLock(&m_propsLock);

    if(overlay_cache_max_item_num) *overlay_cache_max_item_num = m_overlay_cache_max_item_num;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::put_OverlayCacheMaxItemNum(int overlay_cache_max_item_num)
{
    CAutoLock cAutoLock(&m_propsLock);

    if(m_overlay_cache_max_item_num == overlay_cache_max_item_num || overlay_cache_max_item_num<0) return S_FALSE;

    m_overlay_cache_max_item_num = overlay_cache_max_item_num;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::get_ScanLineDataCacheMaxItemNum(int* scan_line_data_cache_max_item_num)
{
    CAutoLock cAutoLock(&m_propsLock);

    if(scan_line_data_cache_max_item_num) *scan_line_data_cache_max_item_num = m_scan_line_data_cache_max_item_num;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::put_ScanLineDataCacheMaxItemNum(int scan_line_data_cache_max_item_num)
{
    CAutoLock cAutoLock(&m_propsLock);

    if(m_scan_line_data_cache_max_item_num == scan_line_data_cache_max_item_num || scan_line_data_cache_max_item_num<0) return S_FALSE;
    m_scan_line_data_cache_max_item_num = scan_line_data_cache_max_item_num;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::get_PathDataCacheMaxItemNum(int* path_data_cache_max_item_num)
{
    CAutoLock cAutoLock(&m_propsLock);

    if(path_data_cache_max_item_num) *path_data_cache_max_item_num = m_path_data_cache_max_item_num;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::put_PathDataCacheMaxItemNum(int path_data_cache_max_item_num)
{
    CAutoLock cAutoLock(&m_propsLock);

    if(m_path_data_cache_max_item_num == path_data_cache_max_item_num || path_data_cache_max_item_num<0) return S_FALSE;
    m_path_data_cache_max_item_num = path_data_cache_max_item_num;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::get_OverlayNoBlurCacheMaxItemNum(int* overlay_no_blur_cache_max_item_num)
{
    CAutoLock cAutoLock(&m_propsLock);

    if(overlay_no_blur_cache_max_item_num) *overlay_no_blur_cache_max_item_num = m_overlay_no_blur_cache_max_item_num;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::put_OverlayNoBlurCacheMaxItemNum(int overlay_no_blur_cache_max_item_num)
{
    CAutoLock cAutoLock(&m_propsLock);

    if(m_overlay_no_blur_cache_max_item_num == overlay_no_blur_cache_max_item_num || overlay_no_blur_cache_max_item_num<0) return S_FALSE;
    m_overlay_no_blur_cache_max_item_num = overlay_no_blur_cache_max_item_num;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::get_CachesInfo(CachesInfo* caches_info)
{
    CAutoLock cAutoLock(&m_propsLock);
    if(caches_info)
    {
        caches_info->path_cache_cur_item_num    = 0;
        caches_info->path_cache_hit_count       = 0;
        caches_info->path_cache_query_count     = 0;
        caches_info->scanline_cache_cur_item_num= 0;
        caches_info->scanline_cache_hit_count   = 0;
        caches_info->scanline_cache_query_count = 0;
        caches_info->non_blur_cache_cur_item_num= 0;
        caches_info->non_blur_cache_hit_count   = 0;
        caches_info->non_blur_cache_query_count = 0;
        caches_info->overlay_cache_cur_item_num = 0;
        caches_info->overlay_cache_hit_count    = 0;
        caches_info->overlay_cache_query_count  = 0;
        caches_info->interpolate_cache_cur_item_num = 0;
        caches_info->interpolate_cache_query_count  = 0;
        caches_info->interpolate_cache_hit_count    = 0;
        caches_info->text_info_cache_cur_item_num   = 0;
        caches_info->text_info_cache_query_count    = 0;
        caches_info->text_info_cache_hit_count      = 0;
        caches_info->word_info_cache_cur_item_num   = 0;
        caches_info->word_info_cache_query_count    = 0;
        caches_info->word_info_cache_hit_count      = 0;
        return S_OK;
    }
    else 
    {
        return S_FALSE;
    }
}

STDMETHODIMP CDirectVobSub::get_SubpixelPositionLevel(int* subpixel_pos_level)
{
    CAutoLock cAutoLock(&m_propsLock);

    if(subpixel_pos_level) *subpixel_pos_level = m_subpixel_pos_level;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::put_SubpixelPositionLevel(int subpixel_pos_level)
{
    CAutoLock cAutoLock(&m_propsLock);

    if(m_subpixel_pos_level == subpixel_pos_level || subpixel_pos_level<0 || subpixel_pos_level>=SubpixelPositionControler::MAX_COUNT) return S_FALSE;
    m_subpixel_pos_level = subpixel_pos_level;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::get_FollowUpstreamPreferredOrder( bool *fFollowUpstreamPreferredOrder )
{
    CAutoLock cAutoLock(&m_propsLock);

    if(fFollowUpstreamPreferredOrder) *fFollowUpstreamPreferredOrder=m_fFollowUpstreamPreferredOrder;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::put_FollowUpstreamPreferredOrder( bool fFollowUpstreamPreferredOrder )
{
    CAutoLock cAutoLock(&m_propsLock);

    if(m_fFollowUpstreamPreferredOrder == fFollowUpstreamPreferredOrder) return S_FALSE;
    m_fFollowUpstreamPreferredOrder = fFollowUpstreamPreferredOrder;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::get_HideTrayIcon( bool *fHideTrayIcon )
{
    CAutoLock cAutoLock(&m_propsLock);

    if(fHideTrayIcon) *fHideTrayIcon=m_fHideTrayIcon;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::put_HideTrayIcon( bool fHideTrayIcon )
{
    CAutoLock cAutoLock(&m_propsLock);

    if(m_fHideTrayIcon == fHideTrayIcon) return S_FALSE;
    m_fHideTrayIcon = fHideTrayIcon;

    return S_OK;
}

STDMETHODIMP CDirectVobSub::UpdateRegistry()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAutoLock cAutoLock(&m_propsLock);

	theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_HIDE), m_fHideSubtitles);
	theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_DOPREBUFFERING), m_fDoPreBuffering);

    theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_YUV_RANGE), m_yuvRange);

    theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_COLOR_SPACE), m_colorSpace);
    theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_BT601_WIDTH), 1024);
    theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_BT601_HEIGHT), 600);

	theApp.WriteProfileInt(ResStr(IDS_R_TEXT), ResStr(IDS_RT_OVERRIDEPLACEMENT), m_fOverridePlacement);
	theApp.WriteProfileInt(ResStr(IDS_R_TEXT), ResStr(IDS_RT_XPERC), m_PlacementXperc);
	theApp.WriteProfileInt(ResStr(IDS_R_TEXT), ResStr(IDS_RT_YPERC), m_PlacementYperc);
	theApp.WriteProfileInt(ResStr(IDS_R_VOBSUB), ResStr(IDS_RV_BUFFER), m_fBufferVobSub);
	theApp.WriteProfileInt(ResStr(IDS_R_VOBSUB), ResStr(IDS_RV_ONLYSHOWFORCEDSUBS), m_fOnlyShowForcedVobSubs);
	theApp.WriteProfileInt(ResStr(IDS_R_VOBSUB), ResStr(IDS_RV_POLYGONIZE), m_fPolygonize);
	CString style;
	theApp.WriteProfileString(ResStr(IDS_R_TEXT), ResStr(IDS_RT_STYLE), style <<= m_defStyle);
	theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_FLIPPICTURE), m_fFlipPicture);
	theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_FLIPSUBTITLES), m_fFlipSubtitles);
	theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_SHOWOSDSTATS), m_fOSD);
	theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_SAVEFULLPATH), m_fSaveFullPath);
	theApp.WriteProfileInt(ResStr(IDS_R_TIMING), ResStr(IDS_RTM_SUBTITLEDELAY), m_SubtitleDelay);
	theApp.WriteProfileInt(ResStr(IDS_R_TIMING), ResStr(IDS_RTM_SUBTITLESPEEDMUL), m_SubtitleSpeedMul);
	theApp.WriteProfileInt(ResStr(IDS_R_TIMING), ResStr(IDS_RTM_SUBTITLESPEEDDIV), m_SubtitleSpeedDiv);
	theApp.WriteProfileInt(ResStr(IDS_R_TIMING), ResStr(IDS_RTM_MEDIAFPSENABLED), m_fMediaFPSEnabled);
	theApp.WriteProfileBinary(ResStr(IDS_R_TIMING), ResStr(IDS_RTM_MEDIAFPS), (BYTE*)&m_MediaFPS, sizeof(m_MediaFPS));
	theApp.WriteProfileInt(ResStr(IDS_R_TEXT), ResStr(IDS_RT_AUTOPARCOMPENSATION), m_ePARCompensationType);
    theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_HIDE_TRAY_ICON), m_fHideTrayIcon);

    theApp.WriteProfileInt(ResStr(IDS_R_PERFORMANCE), ResStr(IDS_RP_OVERLAY_CACHE_MAX_ITEM_NUM), m_overlay_cache_max_item_num);
    theApp.WriteProfileInt(ResStr(IDS_R_PERFORMANCE), ResStr(IDS_RP_OVERLAY_NO_BLUR_CACHE_MAX_ITEM_NUM), m_overlay_no_blur_cache_max_item_num);
    theApp.WriteProfileInt(ResStr(IDS_R_PERFORMANCE), ResStr(IDS_RP_SCAN_LINE_DATA_CACHE_MAX_ITEM_NUM), m_scan_line_data_cache_max_item_num);
    theApp.WriteProfileInt(ResStr(IDS_R_PERFORMANCE), ResStr(IDS_RP_PATH_DATA_CACHE_MAX_ITEM_NUM), m_path_data_cache_max_item_num);
    theApp.WriteProfileInt(ResStr(IDS_R_PERFORMANCE), ResStr(IDS_RP_SUBPIXEL_POS_LEVEL), m_subpixel_pos_level);
    theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_USE_UPSTREAM_PREFERRED_ORDER), m_fFollowUpstreamPreferredOrder);

    //save output color config
    {
        int count = GetOutputColorSpaceNumber();
        BYTE* pData = new BYTE[2*count];
        for(int i = 0; i < count; i++)
        {
            pData[2*i] = static_cast<BYTE>(m_outputColorSpace[i]);
            pData[2*i+1] = static_cast<BYTE>(m_selectedOutputColorSpace[i]);
        }
        theApp.WriteProfileBinary(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_OUTPUT_COLORFORMATS), pData, 2*count);

        delete [] pData;
    }

    //save input color config
    {
        int count = GetInputColorSpaceNumber();
        BYTE* pData = new BYTE[2*count];
        for(int i = 0; i < count; i++)
        {
            pData[2*i] = static_cast<BYTE>(m_inputColorSpace[i]);
            pData[2*i+1] = static_cast<BYTE>(m_selectedInputColorSpace[i]);
        }
        theApp.WriteProfileBinary(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_INPUT_COLORFORMATS), pData, 2*count);

        delete [] pData;
    }

    theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_SUPPORTED_VERSION), CUR_SUPPORTED_FILTER_VERSION);
    theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_VERSION), XY_VSFILTER_VERSION_COMMIT);
    //
	return S_OK;
}

STDMETHODIMP CDirectVobSub::HasConfigDialog(int iSelected)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDirectVobSub::ShowConfigDialog(int iSelected, HWND hWndParent)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDirectVobSub::IsSubtitleReloaderLocked(bool* fLocked)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(!fLocked) return E_POINTER; 

	bool fDisabled;
	get_SubtitleReloader(&fDisabled);

	*fLocked = fDisabled || m_nReloaderDisableCount > 0;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::LockSubtitleReloader(bool fLock)
{
	CAutoLock cAutoLock(&m_propsLock);

	if(fLock) m_nReloaderDisableCount++;
	else m_nReloaderDisableCount--;

	ASSERT(m_nReloaderDisableCount >= 0);
	if(m_nReloaderDisableCount < 0) m_nReloaderDisableCount = 0;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_SubtitleReloader(bool* fDisabled)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAutoLock cAutoLock(&m_propsLock);

	if(fDisabled) *fDisabled = !!theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_DISABLERELOADER), 0);

	return S_OK;
}

STDMETHODIMP CDirectVobSub::put_SubtitleReloader(bool fDisable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAutoLock cAutoLock(&m_propsLock);

	bool b;
	get_SubtitleReloader(&b);
	if(b == fDisable) return S_FALSE;

	theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_DISABLERELOADER), fDisable);

	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_ExtendPicture(int* horizontal, int* vertical, int* resx2, int* resx2minw, int* resx2minh)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAutoLock cAutoLock(&m_propsLock);

	if(horizontal) *horizontal = theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_MOD32FIX), 0) & 1;
	if(vertical) *vertical = theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_EXTPIC), 0);
	//if(resx2) *resx2 = theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_RESX2), 2) & 3;
    if(resx2) *resx2 = 0;
	if(resx2minw) *resx2minw = theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_RESX2MINW), 384);
	if(resx2minh) *resx2minh = theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_RESX2MINH), 288);

	return S_OK;
}

STDMETHODIMP CDirectVobSub::put_ExtendPicture(int horizontal, int vertical, int resx2, int resx2minw, int resx2minh)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAutoLock cAutoLock(&m_propsLock);
    if(resx2==0)
    {
	    int i[5];
	    get_ExtendPicture(i, i+1, i+2, i+3, i+4);
	    if(i[0] == horizontal && i[1] == vertical && i[2] == resx2 && i[3] == resx2minw && i[4] == resx2minh) return S_FALSE;

	    theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_MOD32FIX), horizontal & 1);
	    theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_EXTPIC), vertical);
	    theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_RESX2), resx2 & 3);
	    theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_RESX2MINW), resx2minw);
	    theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_RESX2MINH), resx2minh);

	    return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}

STDMETHODIMP CDirectVobSub::get_LoadSettings(int* level, bool* fExternalLoad, bool* fWebLoad, bool* fEmbeddedLoad)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAutoLock cAutoLock(&m_propsLock);

	if(level) *level = theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_LOADLEVEL), 0) & 3;
	if(fExternalLoad) *fExternalLoad = !!theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_EXTERNALLOAD), 1);
	if(fWebLoad) *fWebLoad = !!theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_WEBLOAD), 0);
	if(fEmbeddedLoad) *fEmbeddedLoad = !!theApp.GetProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_EMBEDDEDLOAD), 1);

	return S_OK;
}

STDMETHODIMP CDirectVobSub::put_LoadSettings(int level, bool fExternalLoad, bool fWebLoad, bool fEmbeddedLoad)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAutoLock cAutoLock(&m_propsLock);

	int i;
	bool b[3];
	get_LoadSettings(&i, b, b+1, b+2);
	if(i == level && b[0] == fExternalLoad && b[1] == fWebLoad && b[2] == fEmbeddedLoad) return S_FALSE;

	theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_LOADLEVEL), level & 3);
	theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_EXTERNALLOAD), fExternalLoad);
	theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_WEBLOAD), fWebLoad);
	theApp.WriteProfileInt(ResStr(IDS_R_GENERAL), ResStr(IDS_RG_EMBEDDEDLOAD), fEmbeddedLoad);

	return S_OK;
}

// IDirectVobSub2

STDMETHODIMP CDirectVobSub::AdviseSubClock(ISubClock* pSubClock)
{
	m_pSubClock = pSubClock;
	return S_OK;
}

STDMETHODIMP_(bool) CDirectVobSub::get_Forced()
{
	return m_fForced;
}

STDMETHODIMP CDirectVobSub::put_Forced(bool fForced)
{
    if(m_fForced==fForced)
        return S_FALSE;

	m_fForced = fForced;
	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_TextSettings(STSStyle* pDefStyle)
{
	CheckPointer(pDefStyle, E_POINTER);

	CAutoLock cAutoLock(&m_propsLock);

	*pDefStyle = m_defStyle;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::put_TextSettings(STSStyle* pDefStyle)
{
	CheckPointer(pDefStyle, E_POINTER);

	CAutoLock cAutoLock(&m_propsLock);

    if(m_defStyle==*pDefStyle)
        return S_FALSE;
	if(!memcmp(&m_defStyle, pDefStyle, sizeof(m_defStyle)))
		return S_FALSE;

	m_defStyle = *pDefStyle;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::get_AspectRatioSettings(CSimpleTextSubtitle::EPARCompensationType* ePARCompensationType)
{
	CAutoLock cAutoLock(&m_propsLock);

	*ePARCompensationType = m_ePARCompensationType;

	return S_OK;
}

STDMETHODIMP CDirectVobSub::put_AspectRatioSettings(CSimpleTextSubtitle::EPARCompensationType* ePARCompensationType)
{
	CAutoLock cAutoLock(&m_propsLock);
	
	if(m_ePARCompensationType==*ePARCompensationType)
		return S_FALSE;

	m_ePARCompensationType = *ePARCompensationType;

	return S_OK;
}

// IFilterVersion

STDMETHODIMP_(DWORD) CDirectVobSub::GetFilterVersion()
{
	return 0x0234;
}

bool CDirectVobSub::is_compatible()
{
    bool compatible = false;
    if ( m_config_info_version>XY_VSFILTER_VERSION_COMMIT )
    {
        compatible = m_supported_filter_verion<=XY_VSFILTER_VERSION_COMMIT;
    }
    else
    {
        compatible = REQUIRED_CONFIG_VERSION<=m_config_info_version;
    }
    return compatible;
}

UINT CDirectVobSub::GetCompatibleProfileInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault )
{
    UINT result = nDefault;
    if (is_compatible())
    {
        result = theApp.GetProfileInt(lpszSection, lpszEntry, nDefault);
    }
    return result;
}
