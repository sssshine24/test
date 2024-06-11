#pragma once


// Action_Render dialog

class Action_Render : public CPropertyPage
{
	DECLARE_DYNAMIC(Action_Render)

public:
	Action_Render();
	virtual ~Action_Render();

// Dialog Data
	enum { IDD = IDD_ActionRender };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
