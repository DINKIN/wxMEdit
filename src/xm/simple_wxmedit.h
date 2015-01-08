///////////////////////////////////////////////////////////////////////////////
// vim:         ts=4 sw=4
// Name:        xm/simple_wxmedit.h
// Description: Simple wxMEdit Implementations
// Copyright:   2015  JiaYanwei   <wxmedit@gmail.com>
// Licence:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _SIMPLE_WXMEDIT_H_
#define _SIMPLE_WXMEDIT_H_

#include "../wxmedit/wxmedit.h"

namespace wxm
{

struct DummyConfigWriter: public ConfigWriter
{
	virtual void Record(const wxString& key, long val) {}
	virtual void Record(const wxString& key, double val) {}
	virtual void Record(const wxString& key, const wxString& val) {}

	virtual void SetConfig(wxConfigBase* cfg) {}
};

struct SimpleWXMEdit: public MadEdit
{
	SimpleWXMEdit(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSIMPLE_BORDER | wxWANTS_CHARS)
		: MadEdit(new DummyConfigWriter(),  parent, id, pos, size, style)
	{
	}
};

struct InvisibleWXMEdit: public SimpleWXMEdit
{
	InvisibleWXMEdit(wxWindow* parent)
		: SimpleWXMEdit(parent, wxID_ANY, wxPoint(-1024, -1024))
	{}
};

struct SearchingWXMEdit: public InvisibleWXMEdit
{
	SearchingWXMEdit(wxWindow* parent, bool bSearchWholeWord)
		: InvisibleWXMEdit(parent)
	{
		StopRepaint();
		SetFixedWidthMode(false);
		SetWordWrapMode(wwmNoWrap);
		SetSearchOptions(true, bSearchWholeWord);
	}
};

struct HexPrintingWXMEdit: public InvisibleWXMEdit
{
	HexPrintingWXMEdit(wxWindow* parent, const wxFont* font);
};

} //namespace wxm

#endif //_SIMPLE_WXMEDIT_H_