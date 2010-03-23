
#pragma once

#include <WebView.h>

//==================================================================
// CFrameWnd
//
/// Main window frame
/**

*/
//==================================================================
class CFrameWnd : public wxFrame
{
public:

	enum
	{
		WXIDM_Exit = wxID_HIGHEST + 1,
		WXIDM_Open
	};


public:

	/// Destructor
	virtual ~CFrameWnd();

	//==============================================================
	// CFrameWnd()
	//==============================================================
	/// Constructor
	/**
		\param [in] x_sTitle	-	Window title
		\param [in] x_ptWin		-	Default window position
		\param [in] x_sizeWin	-	Default window size

		\return

		\see
	*/
    CFrameWnd( const wxString &x_sTitle, const wxPoint &x_ptWin, const wxSize &x_sizeWin );

	void OnExit( wxCommandEvent &x_wxCe );

	void OnOpen( wxCommandEvent &x_wxCe );

    DECLARE_EVENT_TABLE()

private:

//	wxWebView 		*m_pWebView;

};
