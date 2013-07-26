///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        wxmedit/wxm_encoding.h
// Description: define the Encodings which are supported by wxMEdit
// Author:      wxmedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_ENCODING_H_
#define _WXM_ENCODING_H_

#include "../wxmedit/ucs4_t.h"

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
#endif

#include <boost/utility.hpp>
#include <string>
#include <vector>
#include <map>

namespace wxm
{

enum WXMEncodingType
{ etSingleByte, etDoubleByte, etUTF8, etUTF16LE, etUTF16BE, etUTF32LE, etUTF32BE };


struct WXMEncoding;
struct WXMEncodingCreator: private boost::noncopyable
{
	static WXMEncodingCreator& Instance()
	{
		static WXMEncodingCreator creator;
		return creator;
	}

	void InitEncodings()
	{
		if (!m_initialized)
			DoInit();
		m_initialized = true;
	}
	void FreeEncodings();

	WXMEncoding* CreateWxmEncoding(ssize_t idx);
	WXMEncoding* CreateWxmEncoding(wxFontEncoding enc);
	WXMEncoding* CreateWxmEncoding(const wxString &name);

	size_t GetEncodingsCount();
	wxString GetEncodingName(ssize_t idx);
	wxString GetEncodingDescription(ssize_t idx);
	wxString GetEncodingFontName(ssize_t idx);
	wxString EncodingToName(wxFontEncoding enc);
	wxFontEncoding NameToEncoding(const wxString &name);
	WXMEncoding* GetSystemEncoding();

	WXMEncodingType GetIdxEncType(ssize_t idx);

private:
	ssize_t AdjustIndex(ssize_t idx);
	wxFontEncoding IdxToEncoding(ssize_t idx)
	{
		return NameToEncoding(GetEncodingName(idx));
	}

	void DoInit();

	WXMEncodingCreator()
	: m_initialized(false), ms_sysenc_idx(-1), ms_sysenc(NULL)
	{
		DoInit();
		m_initialized = true;
	}

	bool m_initialized;
	ssize_t ms_sysenc_idx;
	WXMEncoding *ms_sysenc;

	typedef std::map<std::string, wxFontEncoding> WXEncMap;
	WXEncMap ms_wxenc_map;

	std::vector<wxString> ms_wxenc_list;
	std::vector<std::string> ms_enc_list;

	typedef std::map<wxString, wxFontEncoding> WXNameEncMap;
	typedef std::map<wxFontEncoding, wxString> WXEncNameMap;
	typedef std::map<wxFontEncoding, WXMEncodingType> WXEncTypeMap;
	typedef std::map<wxFontEncoding, wxString> WXEncFontMap;
	WXNameEncMap ms_wxnameenc_map;
	WXEncNameMap ms_wxencname_map;
	WXEncTypeMap ms_wxenctype_map;
	WXEncFontMap ms_wxencfont_map;

	typedef std::map<ssize_t, WXMEncoding*> WXEncInstMap;
	WXEncInstMap ms_inst_map;
};

struct WXMEncoding: private boost::noncopyable
{
protected:
	wxString m_name;
	wxString m_desc;
	wxString m_fontname;
	wxFontEncoding m_enc;
	WXMEncodingType m_type;
	ssize_t m_idx;

	virtual void Create(ssize_t idx);

protected:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	friend void WXMEncodingCreator::FreeEncodings();
	WXMEncoding(): m_idx(-1)
	{
	}
	virtual ~WXMEncoding()
	{
	}

public:
	// return the converted length of buf
	virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf) = 0;

	virtual ucs4_t MultiBytetoUCS4(wxByte* buf)
	{
		return 0xFFFD;
	}

	virtual bool IsLeadByte(wxByte byte)
	{
		return false;
	}

	wxString GetName() { return m_name; }
	wxString GetDescription() { return m_desc; }
	WXMEncodingType GetType() { return m_type; }
	wxString GetFontName() { return m_fontname; }
	wxFontEncoding GetEncoding() { return m_enc; }
};

struct WXMEncodingMultiByte: public WXMEncoding
{
    virtual void MultiByteInit() = 0;
    virtual ucs4_t MultiBytetoUCS4(wxByte* buf) = 0;
	virtual void Create(ssize_t idx);

protected:
    wxCSConv        *m_CSConv;
    ucs2_t          *m_MBtoWC_Table;    // MultiByte To WideChar table
    wxWord          *m_WCtoMB_Table;    // WideChar To MultiByte table

	WXMEncodingMultiByte(): m_CSConv(NULL), m_MBtoWC_Table(NULL), m_WCtoMB_Table(NULL)
	{
	}
	~WXMEncodingMultiByte()
	{
		delete m_CSConv; m_CSConv = NULL;
		delete m_MBtoWC_Table; m_MBtoWC_Table = NULL;
		delete m_WCtoMB_Table; m_WCtoMB_Table = NULL;
	}
};

struct WXMEncodingSingleByte: public WXMEncodingMultiByte
{
    virtual void MultiByteInit();
    virtual ucs4_t MultiBytetoUCS4(wxByte* buf);
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingSingleByte(){}
	~WXMEncodingSingleByte(){}
};

struct WXMEncodingDoubleByte: public WXMEncodingMultiByte
{
    virtual void MultiByteInit();
    virtual bool IsLeadByte(wxByte byte);
    virtual ucs4_t MultiBytetoUCS4(wxByte* buf);
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
    wxByte          *m_LeadByte_Table;  // DBCS Lead-Byte table, 0:unset, 1:IsLeadByte, 0xFF:NotLeadByte

	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingDoubleByte(): m_LeadByte_Table(NULL)
	{
	}
	~WXMEncodingDoubleByte()
	{
		delete m_LeadByte_Table; m_LeadByte_Table = NULL;
	}
};

struct WXMEncodingUTF8: public WXMEncoding
{
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF8(){}
	~WXMEncodingUTF8(){}
};

struct WXMEncodingUTF16LE: public WXMEncoding
{
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF16LE(){}
	~WXMEncodingUTF16LE(){}
};

struct WXMEncodingUTF16BE: public WXMEncoding
{
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF16BE(){}
	~WXMEncodingUTF16BE(){}
};

struct WXMEncodingUTF32LE: public WXMEncoding
{
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF32LE(){}
	~WXMEncodingUTF32LE(){}
};

struct WXMEncodingUTF32BE: public WXMEncoding
{
    virtual size_t UCS4toMultiByte(ucs4_t ucs4, wxByte* buf);
private:
	friend WXMEncoding* WXMEncodingCreator::CreateWxmEncoding(ssize_t idx);
	WXMEncodingUTF32BE(){}
	~WXMEncodingUTF32BE(){}
};

size_t UCS4toUTF16LE_U10000(ucs4_t ucs4, wxByte* buf);

};// namespace wxm

#endif