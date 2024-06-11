#pragma once


// MobiusTransTab dialog

class MobiusTransTab : public CPropertyPage
{
	DECLARE_DYNAMIC(MobiusTransTab)

public:
	MobiusTransTab();
	virtual ~MobiusTransTab();

// Dialog Data
	enum { IDD = IDD_DIALOG12 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
