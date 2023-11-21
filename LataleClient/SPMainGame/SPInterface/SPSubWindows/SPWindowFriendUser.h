// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-8-30    19:30 
//***************************************************************************

#pragma once

#define FRIENDUSER_BG_COUNT					3

class SPWindow;
class SPWindowStatic;

//struct _FRIEND_INFO;

class SPWindowFriendUser : public SPWindow
{
public:
	SPWindowFriendUser( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual	~SPWindowFriendUser();

	virtual	void		Init				();
	virtual	void		Clean				();
	virtual void		Show				(bool bWithChild = true);
	virtual void		Hide				(bool bSendServer = true);

	virtual	void		Process				( float fTime );
	virtual	void		Render				( float fTime );
	virtual void		RefreshRelationCoord();

	bool				IsFriendInfo		();
	void				SetFriendInfo		( FRIEND_INFO* pFriendInfo );
	void				ShowToolTip			( bool bShowToolTip );
	const char*			GetName				();

	void				SetIconImage		( const char* szImage , int iSrcX , int iSrcY );
	void				SetIconImagePos		( int iXPos , int iYPos );
	void				SetIconImageSize	( int iXSize , int iYSize );

	void				SetTextPos			( int iXPos , int iYPos );
	void				SetTextSize			( int iXSize , int iYSize );
	void				SetTextColorOnLine	( D3DXCOLOR cColor );
	void				SetTextColorOffLine	( D3DXCOLOR cColor );

	void				SetWndState			( WND_STATE eWndState );

private:
	void				ReposOwnImage();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int					OnCursorEnter		( WPARAM wParam, LPARAM lParam );
	int					OnCursorOut			( WPARAM wParam, LPARAM lParam );
	int					OnCursorMove		( WPARAM wParam, LPARAM lParam );

	int					OnLButtonUp			( WPARAM wParam, LPARAM lParam );

	int					OnWheelUp			( WPARAM wParam, LPARAM lParam );
	int					OnWheelDown			( WPARAM wParam, LPARAM lParam );

	int					OnCursorEnter		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnCursorOut			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnCursorMove		( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int					OnLButtonUp			( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int					OnWheelUp			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnWheelDown			( unsigned int iID, WPARAM wParam , LPARAM lParam );

	// }}

	void				SetName				( const char* pstrName );
	void				SetLevel			( UINT8 uiLevel );
	void				SetOnline			( BOOL bOnline );

	//////////////////////////////////////////////////////////////////////////
	//
	//	������
	//

	SPTexture*			m_pBGTexture;
	RECT				m_rcBGDest[ FRIENDUSER_BG_COUNT ];
	RECT				m_rcBGSrc[ FRIENDUSER_BG_COUNT ];

	SPWindowStatic*		m_pIcon;
	SPWindowStatic*		m_pName;

	INSTANCE_ID			m_iStartID;
	INSTANCE_ID			m_iEndID;

	D3DXCOLOR			m_cOnLineColor;
	D3DXCOLOR			m_cOffLineColor;

	bool				m_bShowToolTip;
	FRIEND_INFO*		m_pFriendInfo;

};
