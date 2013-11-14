#pragma once

#include "PlayerBaseStream.h"

class __declspec(uuid("2E03B28F-EA09-464e-9EDA-52FF59C13E13"))
PlayerAsyncReader : public CAsyncReader
                  , public IFileSourceFilter
{
public:
    PlayerAsyncReader(HRESULT* phr);
    ~PlayerAsyncReader();

    DECLARE_IUNKNOWN
    STDMETHODIMP		NonDelegatingQueryInterface(REFIID riid, void **ppv);

    // IFileSourceFilter
    STDMETHODIMP		Load(LPCOLESTR pszFileName, const AM_MEDIA_TYPE *pmt);
    STDMETHODIMP		GetCurFile(LPOLESTR *ppszFileName, AM_MEDIA_TYPE *pmt);

protected:
    static BOOL CheckBytes();

protected:
    PlayerAsyncStream   m_stream;
    CStringW            m_fn;
};