/**
	@file : SPWindowBeautyShop.cpp
	@auth : ��ȭ�� ( duragon@actoz.com )
	@date : 2006/09/11
	@update 
		20060912 : ĳ���� ��� �� ������ �׽�Ʈ
		20060913 : ĳ���� ���� ���� �׽�Ʈ
		20061013 : 1�� �۾��Ϸ�
		20061016 : ���� �� ����ó�� �Ϸ�
	@todo : Error_logs ����
*/

#include "SPCommon.h"

#include "SPUtil.h"
#include "SPWindowDEF.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowGrid.h"
#include "SPWindowSlider.h"
#include "SPWindowEdit.h"
#include "SPWindowEditMultiLine.h"

#include "SPWindowBeautyShop.h"
#include "SPWindowBeautyShopDataUnit.h"
#include "SPWindowBeautyShopDataMng.h"

#include "SPSubGameManager.h"
#include "SPGOBManager.h"
#include "SPGameObject.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModelUnit.h"
#include "SPPlayer.h"
#include "SPGOBStatus.h"		// ���¸� ���� �߰�

#include "SPWindowChat.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPNoticeBox.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPFont.h"

#include "SPCheckManager.h"
#include "SPItem.h"
#include "SPItemStatus.h"
#include "SPCommandConvert.h"

#include "SPEventDEF.h"			// for close
#include "SPEventManager.h"

#include <shellapi.h>
#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPDebug.h"
#include "_Error_log.h"


//------------------------------------------------------------------------------------
SPWindowBeautyShop::SPWindowBeautyShop(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
, m_pkPlayer(NULL), m_pBSData(NULL), m_pkBGTexture(NULL)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowBeautyShop::SPWindowBeautyShop(INSTANCE_ID InstanceID,
	int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_BEAUTYSHOP, InstanceID, iX, iY, iCX, iCY, pParent)	
, m_pkPlayer(NULL), m_pBSData(NULL), m_pkBGTexture(NULL)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowBeautyShop::~SPWindowBeautyShop()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowBeautyShop::Clean()
{
	SPWindow::Clean();

	SAFE_DELETE(m_pkPlayer);
	SAFE_DELETE(m_pBSData) ;
	SAFE_RELEASE( m_pkBGTexture );

	m_vCurBasicID.clear() ;
}

//------------------------------------------------------------------------------------
void SPWindowBeautyShop::Init()
{
	int iError = 0 ;
	std::string strError ;

	//D3D_SetupLogFile() ;
	m_bShowToolTip = false ;

	// �� â�� ���� �ʱ�ȭ
	InitBackground() ;
	InitCharWindow() ;
	InitBeautyShopWindow() ;

	// Layer Data
	m_pBSData = new BeautyShopManager() ;
	m_pBSData->LoadBeautyShopLDT() ;
	
	// Player
	m_pkPlayer = new SPPlayer;
	if( m_pkPlayer )
	{
		m_pkPlayer->SetLobbyAvatar(LOBBY_INFO);
		m_pkPlayer->Init();
		m_pkPlayer->SetShowValue(false, false, false);
		m_pkPlayer->SetPos(115.0, 245.0f);
	}

#ifdef _DEBUG
	m_pBSData->VerifyBeautyShopLDT();
#endif
}


/**
	��� �̹������� �� ��ǥ �ʱ�ȭ

*/
void SPWindowBeautyShop::InitBackground()
{

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &m_pkBGTexture );
	//////////////////////////////////////////////////////////////////////////
	//
	//	ĳ���� â ���
	//
	SetRect( &m_rcSrcCharBack[0], 1, 16,  161, 20);		// Upper
	SetRect( &m_rcDstCharBack[0], 34, 76, 194, 80);
	SetRect( &m_rcSrcCharBack[1], 1, 22, 161, 24);		// Middle
	SetRect( &m_rcDstCharBack[1], 34, 80, 194, 312);
	SetRect( &m_rcSrcCharBack[2], 1, 26, 161, 30);		// Lower
	SetRect( &m_rcDstCharBack[2], 34, 312, 194, 316);
	SetRect( &m_rcSrcCharBack[3], 375, 89, 511, 264);	// Background
	SetRect( &m_rcDstCharBack[3], 47, 89, 183, 264);
	SetRect( &m_rcSrcCharBack[4], 329, 1, 404, 14);		// Title Text
	SetRect( &m_rcDstCharBack[4], 41, 83, 116, 96);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Setting â ���
	//
	SetRect( &m_rcSrcDlgBack[0], 1, 1, 328, 5) ;	// Upper
	SetRect( &m_rcDstDlgBack[0], 210, 76, 537, 80) ;
	SetRect( &m_rcSrcDlgBack[1], 1, 7, 328, 9) ;		// Middle
	SetRect( &m_rcDstDlgBack[1], 210, 80, 537, 418) ;
	SetRect( &m_rcSrcDlgBack[2], 379, 254, 383, 258) ;		// ������
	SetRect( &m_rcDstDlgBack[2], 212, 103, 535, 399) ;
	SetRect( &m_rcSrcDlgBack[3], 1, 11, 328, 15) ;		// Lower
	SetRect( &m_rcDstDlgBack[3], 210, 418, 537, 422) ;
	SetRect( &m_rcSrcDlgBack[4], 127, 424, 236, 439) ;	// ����Ÿ��Ʋ
	SetRect( &m_rcDstDlgBack[4], 221, 83, 330, 98) ;
	SetRect( &m_rcSrcDlgBack[5], 296, 35, 298, 36) ;		// Ÿ��Ʋ ����
	SetRect( &m_rcDstDlgBack[5], 212, 102, 535, 103) ;
	
	SetRect( &m_rcSrcDlgBack[6], 497, 423, 501, 511) ;		// ����
	SetRect( &m_rcDstDlgBack[6], 228, 121, 232, 209) ;
	SetRect( &m_rcSrcDlgBack[7], 502, 423, 506, 511) ;		// �� ����
	SetRect( &m_rcDstDlgBack[7], 228, 237, 232, 325) ;
	SetRect( &m_rcSrcDlgBack[8], 497, 423, 501, 511) ;		// ��Ų ����
	SetRect( &m_rcDstDlgBack[8], 228, 345, 232, 381) ;

	SetRect( &m_rcSrcDlgBack[9], 507, 423, 511, 459) ;		// ��Ÿ��
	SetRect( &m_rcDstDlgBack[9], 439, 121, 443, 157) ;
	SetRect( &m_rcSrcDlgBack[10], 507, 423, 511, 459) ;		// ����÷�
	SetRect( &m_rcDstDlgBack[10], 439, 173, 443, 209) ;
	SetRect( &m_rcSrcDlgBack[11], 507, 475, 511, 511) ;		// �� ����
	SetRect( &m_rcDstDlgBack[11], 439, 237, 443, 273) ;
	SetRect( &m_rcSrcDlgBack[12], 507, 475, 511, 511) ;		// �÷�����
	SetRect( &m_rcDstDlgBack[12], 439, 285, 443, 321) ;
	SetRect( &m_rcSrcDlgBack[13], 507, 423, 511, 459) ;		// ��Ų
	SetRect( &m_rcDstDlgBack[13], 439, 345, 443, 381) ;

	SetRect( &m_rcSrcDlgBack[14], 489, 330, 510, 347) ;		// ��Ÿ�� ������
	SetRect( &m_rcDstDlgBack[14], 252, 121, 273, 138) ;
	SetRect( &m_rcSrcDlgBack[15], 489, 348, 510, 365) ;		// ����÷� ������
	SetRect( &m_rcDstDlgBack[15], 252, 172, 273, 189) ;
	SetRect( &m_rcSrcDlgBack[16], 489, 366, 510, 383) ;		// ����Ÿ�� ������
	SetRect( &m_rcDstDlgBack[16], 252, 232, 273, 249) ;
	SetRect( &m_rcSrcDlgBack[17], 489, 384, 510, 401) ;		// ���÷� ������
	SetRect( &m_rcDstDlgBack[17], 252, 284, 273, 301) ;
	SetRect( &m_rcSrcDlgBack[18], 489, 402, 510, 419) ;		// ��Ų ������
	SetRect( &m_rcDstDlgBack[18], 252, 344, 273, 361) ;

	SetRect( &m_rcSrcDlgBack[19], 427, 265, 511, 276) ;		// ��Ÿ��
	SetRect( &m_rcDstDlgBack[19], 274, 123, 358, 134) ;
	SetRect( &m_rcSrcDlgBack[20], 427, 277, 511, 288) ;		// ����÷�
	SetRect( &m_rcDstDlgBack[20], 274, 175, 358, 186) ;
	SetRect( &m_rcSrcDlgBack[21], 427, 289, 511, 300) ;		// ����Ÿ��
	SetRect( &m_rcDstDlgBack[21], 274, 235, 358, 246) ;
	SetRect( &m_rcSrcDlgBack[22], 427, 301, 511, 312) ;		// ���÷�
	SetRect( &m_rcDstDlgBack[22], 274, 287, 358, 298) ;
	SetRect( &m_rcSrcDlgBack[23], 427, 313, 511, 324) ;		// ��Ų
	SetRect( &m_rcDstDlgBack[23], 274, 347, 358, 358) ;

	SetRect( &m_rcSrcDlgBack[24], 329, 16, 495, 33) ;		// ��Ÿ��
	SetRect( &m_rcDstDlgBack[24], 252, 140, 418, 157) ;
	SetRect( &m_rcSrcDlgBack[25], 329, 16, 495, 33) ;		// ����÷�
	SetRect( &m_rcDstDlgBack[25], 252, 192, 418, 209) ;
	SetRect( &m_rcSrcDlgBack[26], 162, 16, 328, 33) ;		// ����Ÿ��
	SetRect( &m_rcDstDlgBack[26], 252, 252, 418, 269) ;
	SetRect( &m_rcSrcDlgBack[27], 162, 16, 328, 33) ;		// ���÷�
	SetRect( &m_rcDstDlgBack[27], 252, 304, 418, 321) ;
	SetRect( &m_rcSrcDlgBack[28], 329, 16, 495, 33) ;		// ��Ų
	SetRect( &m_rcDstDlgBack[28], 252, 364, 418, 381) ;
}

/**
	ĳ���� â�� Controls�� �ʱ�ȭ �Ѵ�.
	>> LDT ������ �о Character ������ �ʱ�ȭ �Ѵ�.
*/
void SPWindowBeautyShop::InitCharWindow()
{
	SPWindowButton* pkButton;

	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_CHAR_RESET, 46+34, 196+80, 68, 25, this);		//  �ʱ�ȭ ��ư
	pkButton->SetImage			("DATA/INTERFACE/CONCEPT/UI104.PNG", 374, 37);
	pkButton->SetImageHit		("DATA/INTERFACE/CONCEPT/UI104.PNG", 443, 37);
	pkButton->SetImagePush		("DATA/INTERFACE/CONCEPT/UI104.PNG", 374, 63);
	pkButton->SetImageDisable	("DATA/INTERFACE/CONCEPT/UI104.PNG", 443, 63);
	pkButton->Show();
}



/**
	ĳ���� ������ ���� â�� Controls�� �ʱ�ȭ �Ѵ�.
*/
void SPWindowBeautyShop::InitBeautyShopWindow()
{
	SPWindowStatic* pkTStatic = NULL;			// ���
	SPWindowStatic* pkBGStatic = NULL;			// ���
	SPWindowStatic* pkMStatic = NULL;			// �߾� ���
	SPWindowStatic* pkStatic = NULL;
	SPWindowStatic* pkTextStatic = NULL;
	SPWindowButton* pkButton;
	
	POINT stPos ;		///< ������ ����
	stPos.x = 210 ;
	stPos.y = 80 ;

	//------------------------------------------------------
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_CHANGE_CLOSE, stPos.x+306, stPos.y+5, 12, 12, this);	// �ݱ� ��ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 217);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 230);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 243);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 256);

	// -----------------------------------------------------------------------
	// ī�װ� Explain
	pkTextStatic = new SPWindowStatic(WIID_BEAUTYSHOP_CATEGORY_EXPLAIN + 0, stPos.x+157, stPos.y+41, 55, 16, this);	//  Explain
	pkTextStatic->SetFontColor(0xff888888);
	pkTextStatic = new SPWindowStatic(WIID_BEAUTYSHOP_CATEGORY_EXPLAIN + 1, stPos.x+157, stPos.y+93, 55, 16, this);	// Explain
	pkTextStatic->SetFontColor(0xff888888);
	pkTextStatic = new SPWindowStatic(WIID_BEAUTYSHOP_CATEGORY_EXPLAIN + 2, stPos.x+157, stPos.y+153, 55, 16, this);	// Explain
	pkTextStatic->SetFontColor(0xff888888);
	pkTextStatic = new SPWindowStatic(WIID_BEAUTYSHOP_CATEGORY_EXPLAIN + 3, stPos.x+157, stPos.y+205, 55, 16, this);	// Explain
	pkTextStatic->SetFontColor(0xff888888);
	pkTextStatic = new SPWindowStatic(WIID_BEAUTYSHOP_CATEGORY_EXPLAIN + 4, stPos.x+157, stPos.y+265, 55, 16, this);	// Explain
	pkTextStatic->SetFontColor(0xff888888);
	
	// ���μ��� Tooltip (������ ���� �;� �޽����� �޴´�.)
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_TOOLTIP + 0, stPos.x+61, stPos.y+63, 147, 12, this);	// Explain
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_TOOLTIP + 1, stPos.x+61, stPos.y+115, 147, 12, this);	// Explain
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_TOOLTIP + 2, stPos.x+61, stPos.y+175, 147, 12, this);	// Explain
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_TOOLTIP + 3, stPos.x+61, stPos.y+227, 147, 12, this);	// Explain
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_TOOLTIP + 4, stPos.x+61, stPos.y+287, 147, 12, this);	// Explain
	// ���μ��� Explain
	pkTextStatic = new SPWindowStatic(WIID_BEAUTYSHOP_DETAIL_EXPLAIN + 0, stPos.x+61, stPos.y+63, 147, 12, this);	//  Explain
	pkTextStatic->SetFontColor(0xff314c61);
	pkTextStatic = new SPWindowStatic(WIID_BEAUTYSHOP_DETAIL_EXPLAIN + 1, stPos.x+61, stPos.y+115, 147, 12, this);	// Explain
	pkTextStatic->SetFontColor(0xff314c61);
	pkTextStatic = new SPWindowStatic(WIID_BEAUTYSHOP_DETAIL_EXPLAIN + 2, stPos.x+61, stPos.y+175, 147, 12, this);	// Explain
	pkTextStatic->SetFontColor(0xff513939);
	pkTextStatic = new SPWindowStatic(WIID_BEAUTYSHOP_DETAIL_EXPLAIN + 3, stPos.x+61, stPos.y+227, 147, 12, this);	// Explain
	pkTextStatic->SetFontColor(0xff513939);
	pkTextStatic = new SPWindowStatic(WIID_BEAUTYSHOP_DETAIL_EXPLAIN + 4, stPos.x+61, stPos.y+287, 147, 12, this);	// Explain
	pkTextStatic->SetFontColor(0xff314c61);
	


	// -----------------------------------------------------------------------
	// ī�װ� ����ư, ����ư
	stPos.x = 212 ;
	stPos.y = 103 ;
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_CATEGORY_LEFT + 0, stPos.x+146, stPos.y+19, 9, 13, this);	//��Ÿ�� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 121);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 135);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 149);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 236, 16);
	pkButton->Show();
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_CATEGORY_RIGHT + 0, stPos.x+210, stPos.y+19, 9, 13, this);	//��Ÿ�� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 121);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 135);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 149);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 246, 16);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_CATEGORY_LEFT + 1, stPos.x+146, stPos.y+71, 9, 13, this);	//����÷� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 121);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 135);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 149);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 236, 16);
	pkButton->SetEnable(true) ;
	pkButton->Show();
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_CATEGORY_RIGHT + 1, stPos.x+210, stPos.y+71, 9, 13, this);	//����÷� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 121);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 135);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 149);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 246, 16);
	pkButton->SetEnable(true) ;
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_CATEGORY_LEFT + 2, stPos.x+146, stPos.y+131, 9, 13, this);	//������ ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 156, 165);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 176, 165);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 196, 165);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 179, 106);
	pkButton->SetEnable(true) ;
	pkButton->Show();
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_CATEGORY_RIGHT + 2, stPos.x+210, stPos.y+131, 9, 13, this);	//������ ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 166, 165);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 186, 165);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 206, 165);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 189, 106);
	pkButton->SetEnable(true) ;
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_CATEGORY_LEFT + 3, stPos.x+146, stPos.y+183, 9, 13, this);	//�÷����� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 156, 165);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 176, 165);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 196, 165);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 179, 106);
	pkButton->Show();
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_CATEGORY_RIGHT + 3, stPos.x+210, stPos.y+183, 9, 13, this);	//�÷����� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 166, 165);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 186, 165);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 206, 165);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 189, 106);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_CATEGORY_LEFT + 4, stPos.x+146, stPos.y+243, 9, 13, this);	//���� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 121);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 135);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 149);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 236, 16);
	pkButton->Show();
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_CATEGORY_RIGHT + 4, stPos.x+210, stPos.y+243, 9, 13, this);	//���� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 121);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 135);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 149);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 246, 16);
	pkButton->Show();
	
	// ���γ��� ����ư, ����ư
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_LEFT + 0, stPos.x+28, stPos.y+39, 9, 13, this);	//��Ÿ�� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 121);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 135);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 149);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 236, 16);
	pkButton->Show();
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_RIGHT + 0, stPos.x+210, stPos.y+39, 9, 13, this);	//��Ÿ�� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 121);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 135);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 149);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 246, 16);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_LEFT + 1, stPos.x+28, stPos.y+91, 9, 13, this);	//����÷� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 121);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 135);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 149);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 236, 16);
	pkButton->Show();
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_RIGHT + 1, stPos.x+210, stPos.y+91, 9, 13, this);	//����÷� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 121);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 135);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 149);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 246, 16);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_LEFT + 2, stPos.x+28, stPos.y+151, 9, 13, this);	//������ ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 156, 165);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 176, 165);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 196, 165);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 179, 106);
	pkButton->Show();
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_RIGHT + 2, stPos.x+210, stPos.y+151, 9, 13, this);	//������ ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 166, 165);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 186, 165);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 206, 165);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 189, 106);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_LEFT + 3, stPos.x+28, stPos.y+203, 9, 13, this);	//�÷����� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 156, 165);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 176, 165);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 196, 165);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 179, 106);
	pkButton->Show();
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_RIGHT + 3, stPos.x+210, stPos.y+203, 9, 13, this);	//�÷����� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 166, 165);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 186, 165);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 206, 165);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 189, 106);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_LEFT + 4, stPos.x+28, stPos.y+263, 9, 13, this);	//���� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 121);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 135);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 149);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 236, 16);
	pkButton->Show();
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_DETAIL_RIGHT + 4, stPos.x+210, stPos.y+263, 9, 13, this);	//���� ����ư
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 121);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 135);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 86, 149);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 246, 16);
	pkButton->Show();

	// -----------------------------------------------------------------------
	// ���� ��ư
	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_COMMIT_BTN + 0, stPos.x+239, stPos.y+27, 68, 17, this);	// ��Ÿ�� ����
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 440);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 458);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 476);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 494);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_COMMIT_BTN + 1, stPos.x+239, stPos.y+79, 68, 17, this);	// ����÷� ����
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 440);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 458);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 476);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 494);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_COMMIT_BTN + 2, stPos.x+239, stPos.y+139, 68, 17, this);	// ������ ����
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 440);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 458);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 476);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 494);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_COMMIT_BTN + 3, stPos.x+239, stPos.y+191, 68, 17, this);	// �÷����� ����
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 440);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 458);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 476);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 494);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_BEAUTYSHOP_COMMIT_BTN + 4, stPos.x+239, stPos.y+251, 68, 17, this);	// ���� ����
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 440);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 458);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 476);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 494);
	pkButton->Show();

}



/**
	����ڰ� ���� �԰��ִ� BASIC ��� �˻��ؼ� �����Ѵ�.
	g_pGOBManager->GetLocalPlayer() ������� Inven �������� ��� �о��
	���� ���߰� �ִ� BASIC ��� �����Ѵ�.
*/
void SPWindowBeautyShop::initCurUserLayer(bool bUpdate)
{
	// ������ ���´�.
	if( g_pGOBManager->GetLocalPlayer() != NULL )
	{
		m_iCurGender = (int)g_pGOBManager->GetLocalPlayer()->GetCheckStatusValue(CHECK_STATUS_GENDER);
		if( m_iCurGender == FEMALE )	// MALE:1, FEMALE:2
            m_iCurGender = 1 ;			// FEMALE_Index:1
		else
			m_iCurGender = 0 ;			// MALE_Index : 0
	}

	//���� ��� ���´�. (Basic�� ����)
	GetCurPlayerLayer() ;
	
	BS_CODE_PARSE parseItem ;
	for(int i = 0; i < (int)m_vCurBasicID.size(); i++)
	{
		parseItem.Clear() ;
		// ���Ƿ��� �����Ѵ�.
		if(  m_vCurBasicID[i] < 100000000 )
			continue ;

		// ������ ���� -> Dialog
		m_pBSData->SetDefaultCharSet(m_vCurBasicID[i], m_iCurGender, bUpdate) ;

		// Code List ����
		m_pBSData->Parsing_CharCode(m_vCurBasicID[i], &parseItem) ;

		ErrorFLogs->Error_Log( "BS_SET LAYER[%d] [ID:%d][layer:%d, gender:%d, Special:%d, Serial[%d], Color[%d]]", 
						i, m_vCurBasicID[i], parseItem.iLayerID, parseItem.iGender, parseItem.iSpecialID,
						parseItem.iSerialID, parseItem.iColor) ;
	}

	//////////////////////////////////////////////////////////////////////////
	// BeautyShop dialog Default Setting
	if( bUpdate )
	{
		SPWindow* pkCateStatic, *pkExplainStatic;
		for( int i = 0; i < BS_INDEX_MAX; i++ )
		{
			m_strCategory[i] = m_pBSData->GetCategoryString(true, i) ;
			m_strDetail[i] = m_pBSData->GetDetailString(true, i) ;
			m_bCurEnable[i] = m_pBSData->GetCategoryEnable(i) ;
			ErrorFLogs->Error_Log( "BS_SET CategoryString[%s]-[%s]:[%d]", m_strCategory[i].c_str(), m_strDetail[i].c_str(), m_bCurEnable[i]) ;

			// WindowText Setting
			pkCateStatic = Find(WIID_BEAUTYSHOP_CATEGORY_EXPLAIN + i, true);
			pkCateStatic->SetWindowText(m_strCategory[i].c_str()) ;

			pkExplainStatic = Find(WIID_BEAUTYSHOP_DETAIL_EXPLAIN + i, true);
			m_bCurEnable[i] ? pkExplainStatic->SetWindowText(m_strDetail[i].c_str()) : pkExplainStatic->SetWindowText("-") ;

			SetLayerButton(i, m_bCurEnable[i]) ;		// ������� ������ �ִٸ� ��Ȱ��ȭ
			// SetChangeCharacterLayer(i);
		}
	}
}


/**
	���� m_iCurCategoryIndex, m_iCurDetailIndex�� ������ BASIC Layer�� TEXT�� �����Ѵ�.
	@param bool bChange : �������� �ʿ� Refresh�� �ʿ����� ����
*/
void SPWindowBeautyShop::SetCurSelectLayer(int _iLayer)
{
	SPWindow* pkCateStatic, *pkExplainStatic;

	// �Ϲ�, �����, ��Ż
	pkCateStatic = Find(WIID_BEAUTYSHOP_CATEGORY_EXPLAIN + _iLayer, true);
	pkCateStatic->SetWindowText(m_strCategory[_iLayer].c_str()) ;

	// ���� ����
	pkExplainStatic = Find(WIID_BEAUTYSHOP_DETAIL_EXPLAIN + _iLayer, true);
	m_bCurEnable[_iLayer] ? pkExplainStatic->SetWindowText(m_strDetail[_iLayer].c_str()) : pkExplainStatic->SetWindowText("-") ;

}


/**
	ĳ������ Layer������ �����Ѵ�.
	>> ����� ������ ������ó�� Inventory�� ������ ����.
	>> ������ �ϸ� �ٷ� ����Ǿ�� �Ѵ�.
*/
void SPWindowBeautyShop::SetChangeCharacterLayer(int _iLayer)
{
	FIGURE_ITEM figureItem;
	BeautyShopUnit* pBSUnit = NULL ;
	BeautyShopUnit* pBSUnitColor = NULL ;

	// �Ӹ� ���� ��
	if( _iLayer == BS_HAIR || _iLayer == BS_HAIR_C )
	{
		pBSUnit = m_pBSData->GetSelectItem(BS_HAIR) ;
		pBSUnitColor = m_pBSData->GetSelectItem(BS_HAIR_C) ;
		figureItem.iItemID = ( pBSUnit->m_iGroupID > 1 ) ? pBSUnit->m_iItemCode : pBSUnit->m_iItemCode + pBSUnitColor->m_iItemCode;
		{
			figureItem.SlotInfo.ContainerType = FIGURE_BASIC ;
			figureItem.SlotInfo.SlotIndex = EQ_HAIR ;
			m_pkPlayer->ItemSetting(figureItem);
		}
	}

	// �����¿� ������
	if( _iLayer == BS_EYE || _iLayer == BS_EYE_C )
	{
		pBSUnit = m_pBSData->GetSelectItem(BS_EYE) ;
		pBSUnitColor = m_pBSData->GetSelectItem(BS_EYE_C) ;
		figureItem.iItemID = ( pBSUnit->m_iGroupID > 1 ) ? pBSUnit->m_iItemCode : pBSUnit->m_iItemCode + pBSUnitColor->m_iItemCode;
		{
			figureItem.SlotInfo.ContainerType = FIGURE_BASIC;
			figureItem.SlotInfo.SlotIndex = EQ_EXPRESSION;
			m_pkPlayer->ItemSetting(figureItem);
		}
	}

	// �Ǻλ�
	if( _iLayer == BS_SKIN)
	{
		pBSUnit = m_pBSData->GetSelectItem(BS_SKIN) ;
		{
			figureItem.iItemID = pBSUnit->m_iItemCode ;
			figureItem.SlotInfo.ContainerType = FIGURE_BASIC;
			figureItem.SlotInfo.SlotIndex = EQ_SKIN;
			m_pkPlayer->ItemSetting(figureItem);
		}
	}

	
	// Equip & Fashion�� �ٽ� ������.
	SetEquipCharacterLayer() ;
}


/**
	BASIC ���� ���� EQUIP & FASHION �� �ٽ� ���� �ش�.
*/
void SPWindowBeautyShop::SetEquipCharacterLayer()
{
	
	m_pkPlayer->RefreshEquipItem();
	
	m_pkPlayer->SPGOBPerformMessage(SPGM_ITEM_SC_TOGGLE_USE_WEAPON_SET, m_pkPlayer->GetActiveWeapon());
	m_pkPlayer->SetWeaponSeqItem();
}

/**
	������� Layer�� InvenArchieve���� ������ ������ �ʱ�ȭ �Ѵ�.
	BASIC, EQUIP, FASHION ��� �缳�� �Ѵ�.
*/
void SPWindowBeautyShop::GetCurPlayerLayer()
{
	SPPlayer* pkLocalPlayer;
	if( m_pkPlayer == NULL || (pkLocalPlayer = g_pGOBManager->GetLocalPlayer()) == NULL )		// Player Ȯ��
		return;

	SPPlayerInvenArchive* pkInvenArchive = pkLocalPlayer->GetInvenArchive();	// Player�� Inven ������
	if( pkInvenArchive == NULL )
		return;

	m_vCurBasicID.clear() ;
	m_pkPlayer->InitFigureInfo();
	
	m_pkPlayer->SetPlayerClass(pkLocalPlayer->GetPlayerClass());

	SPItem* pkItem;
	SPItemStatus* pkItemStatus ;
	FIGURE_ITEM FigureItem;
	int iSize, i, j;
	CONTAINER_TYPE eFigureIndex[3+1] = {FIGURE_BASIC, FIGURE_EQUIP, FIGURE_FASHION, FIGURE_EFFECT};
	for( i = 0; i < 3+1; i++ )
	{
		iSize = pkInvenArchive->GetInventorySize(eFigureIndex[i]);
		for( j = 0; j < iSize; j++ )
		{
			pkItem = pkInvenArchive->GetInventoryItem(eFigureIndex[i], j);
			if( pkItem == NULL )
				continue;

			FigureItem.iItemID = pkItem->GetItemStatusValue(MV_ITEM_GETUID);
			if( FigureItem.iItemID == 0 )
				continue;

			if( i == 0 && FigureItem.iItemID > 100000000 )			// FIGURE_BASIC && ���Ƿ� ����
			{
				m_vCurBasicID.push_back( FigureItem.iItemID ) ;
			}

			// �ð��� �������� ����
			pkItemStatus = pkItem->GetItemStatus();
			if( pkItemStatus == NULL || !pkItemStatus->IsTimeAvailable() )
				continue;

			// ���� ��� �� ��쿡�� ��ŵ�� �ϵ��� �Ѵ�.
			// => ���� �ʿ� ���� �� ���Ƽ� ���⸦ ���̵��� ��ġ��.
			//if( pkItem->GetItemStatus()->GetItemStatusID() != IS_EQUIP_BATTLE )
			//{
				FigureItem.SlotInfo.ContainerType = pkItem->GetContainerType();
				FigureItem.SlotInfo.SlotIndex = pkItem->GetSlotIndex() + 1;

				m_pkPlayer->ItemSetting(FigureItem);
			//}
		}
	}

	m_pkPlayer->SPGOBPerformMessage(SPGM_ITEM_SC_TOGGLE_USE_WEAPON_SET, pkLocalPlayer->GetActiveWeapon());
	m_pkPlayer->SetWeaponSeqItem();
}


/**
	ĳ������ �ִϸ��̼��� �ʱ�ȭ �Ѵ�.
	Lobby_Motion.LDT�� 10, 20, 30�� Animation�� ����Ѵ�.
*/
void SPWindowBeautyShop::InitPlayerAnimation()
{
	if( m_pkPlayer == NULL )
		return;

	SPGOBModelUnit* pkModelUnit = m_pkPlayer->GetModelUnit();
	if( pkModelUnit == NULL )
		return;

	m_pkPlayer->SetAction(ACTION_STOP, true);

	//m_uiAniIndex = -1;
	//pkModelUnit->SPSendMessage(MV_FRAMEINIT);
	//SetAniSeq(10);
}


/**
	Animation Index �� �̿��ؼ� Animation�� �����Ѵ�.
	@param unsigned int uiAniIndex : ���� Animation Index (Lobby_Motion.LDT)
	@param bool bLoop : ���� ��������
*/
void SPWindowBeautyShop::SetAniSeq(unsigned int uiAniIndex, bool bLoop)
{
	if( m_pkPlayer == NULL )
		return;

	if( m_uiAniIndex == uiAniIndex )
		return;

	if( m_uiAniIndex == 10 || m_uiAniIndex == 20 || m_uiAniIndex == 30 )
		m_uiBeforeAniIndex = m_uiAniIndex;

	m_uiAniIndex = uiAniIndex;
	SPGOBModelUnit*	pGOBModel = m_pkPlayer->GetModelUnit();
	pGOBModel->SPSendMessage(MV_SET_ANIMATION_INDEX, uiAniIndex);
	pGOBModel->SPSendMessage(MV_SET_ANI_LOOP, bLoop);

	if( bLoop )
		m_fAniMaxTime = 0.0f;
	else
	{
		pGOBModel->SPSendMessage(MV_GET_ANI_ACCUMUL_TIME, (WPARAM)&m_fAniMaxTime);
		m_fAniAccmulateTime = 0.0f;
	}
}


/**
	�����츦 ȭ�鿡 ����Ѵ�.
	>>> ȭ�鿡 ����Ҷ� LDT Data�� �ε��Ѵ�.
	@param bool bWithChild : Child Window ��� ����
*/
void SPWindowBeautyShop::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);

	if(bWithChild)
	{
		initCurUserLayer(true) ;	// ������� ���� ������ ����
		InitPlayerAnimation();		// �ִϸ��̼� �ʱ�ȭ
		
		this->SetFocus() ;		// â�� Focus
	}
	m_iErrorDlgState.Clear() ;
}


/**
	�����츦 ȭ�鿡�� �����.
	>>> LDT Data Struct ����
	@param bool bSendServer : ������ ���ۿ���
*/
void SPWindowBeautyShop::Hide(bool bSendServer)
{
	// Error â�� �ִ� ��� �ݾ��ش�.
	if( g_pInterfaceManager->GetModalWindow() )
	{
		g_pInterfaceManager->GetModalWindow()->Hide();
		g_pInterfaceManager->SetModalWindow(NULL);
	}

	SPWindow::Hide(bSendServer);
}


/**
	�ش� Layer�� ��ư�� Ȱ��ȭ, ��Ȱ��ȭ ��Ų��.
	@param int _iLayer : ���̾� Index
	@param bool bEnable : Ȱ��ȭ or ��Ȱ��ȭ
*/
void SPWindowBeautyShop::SetLayerButton(int _iLayer, bool bEnable)
{
	if( _iLayer < 0 || _iLayer > BS_INDEX_MAX-1 )
	{
		char szMsg[128];		
		wsprintf(szMsg, "[BeautyShop]Button Layer Index Error");
		g_pInterfaceManager->SetNotice(szMsg, this, NOTICE_TYPE_YES);
		return ;
	}

	m_bCurEnable[_iLayer] = bEnable ;
	SPWindow* pkButton = NULL ;
	pkButton = Find(WIID_BEAUTYSHOP_CATEGORY_LEFT + _iLayer, bEnable);
	pkButton->SetEnable(bEnable) ;
	pkButton = Find(WIID_BEAUTYSHOP_CATEGORY_RIGHT + _iLayer, bEnable);
	pkButton->SetEnable(bEnable) ;
	
	pkButton = Find(WIID_BEAUTYSHOP_DETAIL_LEFT + _iLayer, bEnable);
	pkButton->SetEnable(bEnable) ;
	pkButton = Find(WIID_BEAUTYSHOP_DETAIL_RIGHT + _iLayer, bEnable);
	pkButton->SetEnable(bEnable) ;

	pkButton = Find(WIID_BEAUTYSHOP_COMMIT_BTN + _iLayer, bEnable);
	pkButton->SetEnable(bEnable) ;

	pkButton = Find(WIID_BEAUTYSHOP_DETAIL_EXPLAIN + _iLayer, bEnable);
	m_bCurEnable[_iLayer] ? pkButton->SetWindowText(m_strDetail[_iLayer].c_str()) : pkButton->SetWindowText("-") ;
}



/**
	�� Frame���� Update ó�� �Ǵ� ����
*/
void SPWindowBeautyShop::Process(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Process(fTime);

	if( m_pkPlayer )
		m_pkPlayer->Process(fTime) ;
}


/**
	��� ������
	(����� �Ǵ� �̹����� ���� ������ �Ѵ�)
*/
void SPWindowBeautyShop::RenderBackground(float fTime)
{
	if( m_pkBGTexture )
	{
		// Character â ���
		for(int i = 0; i < 5; i++)
		{
			m_pkBGTexture->RenderTexture( &m_rcDstCharBack[i] , &m_rcSrcCharBack[i] );
		}

		// Dialog â ���
		for(int i = 0; i < 29; i++)
		{
			m_pkBGTexture->RenderTexture( &m_rcDstDlgBack[i] , &m_rcSrcDlgBack[i] );
		}
		g_pVideo->Flush();
	}
}


/**
	���͸�
*/
void SPWindowBeautyShop::Render(float fTime)
{
	if( !m_bShow )
		return;

	RenderBackground(fTime) ;	// ���

	SPWindow::Render(fTime);	// ��Ʈ��

	if( m_pkPlayer )
		m_pkPlayer->Render(fTime) ;	// ĳ����

}



/**
	Exit ��ư�� ���� ��
*/
int SPWindowBeautyShop::OnExit(WPARAM wParam, LPARAM lParam)
{
	// ���� ���¸� �����Ѵ�.
	g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);

	return 1;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
//	Error Dialog (g_pInterfaceManager->SetNotice ���)
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int SPWindowBeautyShop::OnYes( WPARAM wParam, LPARAM lParam)
{
	switch( m_iErrorDlgState.iDlgType )
	{
	//case BS_ERRDLG_NONE:
	//	break ;
	case BS_ERRDLG_CHANGE_CHOICE:			// ���� ����â
		if( EMPTY_NODE < m_iErrorDlgState.iSelectlayer && m_iErrorDlgState.iSelectlayer < BS_INDEX_MAX)
		{
			//g_pInterfaceManager->ForceReply();
			OnCommitCheck(m_iErrorDlgState.iSelectlayer , true) ;
		}
		break ;
	}
	m_iErrorDlgState.Clear();
	return 1;
}

int SPWindowBeautyShop::OnNo( WPARAM wParam, LPARAM lParam)
{
	// ��Ҵ� ���ٸ� ������ ����.
	switch( m_iErrorDlgState.iDlgType )
	{
	case BS_ERRDLG_NONE:
	case BS_ERRDLG_CHANGE_CHOICE:
		break ;
	}
	m_iErrorDlgState.Clear();
	return 1;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
//	��ư (ȭ��ǥ �̵� ����)
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**
	ī�װ��� ���� �̵� Ŭ��
*/
int SPWindowBeautyShop::OnChangeCategoryLeftClick(unsigned int uiID, WPARAM, LPARAM)
{
	int iIndex = uiID - WIID_BEAUTYSHOP_CATEGORY_LEFT;

	if( m_pBSData->OnChangeCategoryLeft(iIndex) )
	{
		m_strCategory[iIndex]	= m_pBSData->GetCategoryString(true, iIndex) ;
		m_strDetail[iIndex]		= m_pBSData->GetDetailString(true, iIndex) ;

		// ���������� ���ƿ��� ��� �Ʒ� ��ư Enable �Ǿ�� �Ѵ�.
		if( m_bCurEnable[1] != m_pBSData->GetCategoryEnable(1) )		SetLayerButton(1, true) ;
		if( m_bCurEnable[3] != m_pBSData->GetCategoryEnable(3) )		SetLayerButton(3, true) ;

		SetCurSelectLayer(iIndex);
		SetChangeCharacterLayer(iIndex);
	}
	
	return 1 ;
}

/**
	ī�װ��� ���� �̵� Ŭ��
	�ش� ī�װ��� ũ�⸸ŭ�� �̵��� �� �ֵ��� �Ѵ�.
*/
int SPWindowBeautyShop::OnChangeCategoryRightClick(unsigned int uiID, WPARAM, LPARAM)
{
	int iIndex = uiID - WIID_BEAUTYSHOP_CATEGORY_RIGHT;
	if( m_pBSData->OnChangeCategoryRight(iIndex) )
	{
		m_strCategory[iIndex]	= m_pBSData->GetCategoryString(true, iIndex) ;
		m_strDetail[iIndex]		= m_pBSData->GetDetailString(true, iIndex) ;

		// �������� �̵��ϴ� ��� �Ʒ� ��ư Disable �Ǿ�� �Ѵ�.
		if( m_bCurEnable[1] != m_pBSData->GetCategoryEnable(1) )		SetLayerButton(1, false) ;
		if( m_bCurEnable[3] != m_pBSData->GetCategoryEnable(3) )		SetLayerButton(3, false) ;

		SetCurSelectLayer(iIndex);
		SetChangeCharacterLayer(iIndex);
	}

	return 1 ;
}

/**
	���� ������ ���� �̵� Ŭ��
*/
int SPWindowBeautyShop::OnChangeDetailLeftClick(unsigned int uiID, WPARAM, LPARAM)
{
	int iIndex = uiID - WIID_BEAUTYSHOP_DETAIL_LEFT;
	if( m_pBSData->OnChangeDetailLeft(iIndex) )
	{
		m_strCategory[iIndex]	= m_pBSData->GetCategoryString(true, iIndex) ;
		m_strDetail[iIndex]		= m_pBSData->GetDetailString(true, iIndex) ;

		SetCurSelectLayer(iIndex);
		SetChangeCharacterLayer(iIndex);
	}

	return 1 ;
}

/**
	���� ������ ���� �̵� Ŭ��
*/
int SPWindowBeautyShop::OnChangeDetailRightClick(unsigned int uiID, WPARAM, LPARAM)
{
	int iIndex = uiID - WIID_BEAUTYSHOP_DETAIL_RIGHT;
	if( m_pBSData->OnChangeDetailRight(iIndex) )
	{
		m_strCategory[iIndex]	= m_pBSData->GetCategoryString(true, iIndex) ;
		m_strDetail[iIndex]		= m_pBSData->GetDetailString(true, iIndex) ;

		SetCurSelectLayer(iIndex);
		SetChangeCharacterLayer(iIndex);
	}

	return 1 ;
}


/**
	�ʱ�ȭ ��ư Ŭ��
*/
int SPWindowBeautyShop::OnCharacterRefresh(WPARAM, LPARAM)
{
	initCurUserLayer(true) ;	// ������� ���� ������ ����
	InitPlayerAnimation();		// �ִϸ��̼� �ʱ�ȭ

	return 1 ;
}


/**
	ToolTip ���� ����
	>> ����ڰ� �����ϰ� ���� ���� �������� ���
		SPItemStatus �� ������ ���� ���� Tooltip�� ȣ���ؾ� �Ѵ�.
*/
int SPWindowBeautyShop::OnItemDetailToolTipEnter(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	if( !m_bShowToolTip )	return 0 ;

	unsigned int uiUniqueID;
	int iEquipClass, iItemID, iItemColor;

	int iIndex = uiID - WIID_BEAUTYSHOP_DETAIL_TOOLTIP;
	int iExpectCode = m_pBSData->GetExpectItemCode(iIndex) ;
	if( iExpectCode < 0 )
		return 0 ;

	uiUniqueID	= iExpectCode ;	
	iItemColor	= iExpectCode % 100;
	iEquipClass = iExpectCode / 100000;
	iItemID		= (iExpectCode % 100000 - iItemColor) / 100;	

	SPItemStatus kItemStatus;
	kItemStatus.SetItemID(uiUniqueID);

	SPItemAttr* pItemAttr;
	g_pItemCluster->GetItemInfo(uiUniqueID, pItemAttr);
	if( pItemAttr )
	{
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID, pItemAttr->m_ePosID1);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS, iEquipClass);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID, iItemID);
		kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,	iItemColor);
		kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, pItemAttr->m_eRareLimit);		///<�� RARELEVEL ���� �ʿ�

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&kItemStatus, lParam);
		pWindow->Show();
	}

	return 1;
}

int SPWindowBeautyShop::OnItemDetailToolTipOut(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	if( !m_bShowToolTip )	return 0 ;

	int iIndex = uiID - WIID_BEAUTYSHOP_DETAIL_TOOLTIP;

	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	pkWindow->SPSendMessage(SPIM_TOOLTIP_OUT, NULL, lParam);
	pkWindow->Show();

	return 1 ;
}

int SPWindowBeautyShop::OnItemDetailToolTipMove(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	if( !m_bShowToolTip )	return 0 ;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow->IsShow() )	
		return 0 ;

	unsigned int uiUniqueID;
	int iEquipClass, iItemID, iItemColor;

	int iIndex = uiID - WIID_BEAUTYSHOP_DETAIL_TOOLTIP;
	int iExpectCode = m_pBSData->GetExpectItemCode(iIndex) ;
	if( iExpectCode < 0 )
		return 0 ;

	uiUniqueID	= iExpectCode ;	
	iItemColor	= iExpectCode % 100;
	iEquipClass = iExpectCode / 100000;
	iItemID		= (iExpectCode % 100000 - iItemColor) / 100;	

	SPItemStatus kItemStatus;
	kItemStatus.SetItemID(uiUniqueID);

	SPItemAttr* pItemAttr;
	g_pItemCluster->GetItemInfo(uiUniqueID, pItemAttr);
	if( pItemAttr )
	{
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID, pItemAttr->m_ePosID1);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS, iEquipClass);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID, iItemID);
		kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,	iItemColor);
		kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, 1);		///<�� RARELEVEL ���� �ʿ�

		pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&kItemStatus, lParam);
		pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)&kItemStatus, lParam);
		pWindow->Show();
	}

	return 1 ;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
//	���� Button && Packet Send & Recv
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

int SPWindowBeautyShop::SetErrorDlg(int iErrorCode, int _index)
{
	int iStringNum ;
	char szMsg[128];

	m_iErrorDlgState.Set(BS_ERRDLG_NONE, EMPTY_NODE) ;
	switch(iErrorCode)
	{
	case	ERROR_CANT_ITEMINFO :
		wsprintf(szMsg, "[BeautyShop]Can't Find Item Information.");
		g_pInterfaceManager->SetNotice(szMsg, this, NOTICE_TYPE_YES);
		break ;
	case	ERROR_HASNT_COUPON :
		iStringNum = 9007009 + m_pBSData->GetCategoryValue(_index) * 5 + _index ;
		wsprintf(szMsg, g_pResourceManager->GetGlobalString(9007001), g_pResourceManager->GetGlobalString(iStringNum));
		g_pInterfaceManager->SetNotice(szMsg, this, NOTICE_TYPE_YES);
		break ;
	case	ERROR_SAME_CHANGE :
		g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(9007002), this, NOTICE_TYPE_YES);
		break ;
	case	WARNING_EXPERT_CHANGE :			///< �� â�� YES, NO
		m_iErrorDlgState.Set(BS_ERRDLG_CHANGE_CHOICE, _index) ;
		g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(9007004), this);
		break ;
	case	ERROR_COLOR_CHANGE :
		g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(9007006), this, NOTICE_TYPE_YES);
		break ;
	case	ERROR_ITEM_LIMIT :
		g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(9007008), this, NOTICE_TYPE_YES);
		break ;
	}

	return 0 ;
}


/**
	�����ϱ� ��ư Ŭ��
	///< ���� �Ŀ��� �ڽ��� �԰� �ִ� ��� ����Ʈ�� ������Ʈ �ϵ��� �Ѵ�. (m_vCurBasicID)
*/
int SPWindowBeautyShop::OnBeautyShopItemCommit(unsigned int uiID, WPARAM, LPARAM)
{
	int iIndex = uiID - WIID_BEAUTYSHOP_COMMIT_BTN;
	if( iIndex < 0 || iIndex > BS_INDEX_MAX )
	{
		m_iErrorDlgState.Set(BS_ERRDLG_NONE, EMPTY_NODE) ;
		g_pInterfaceManager->SetNotice("[BeautyShop]Invalid Layer.", this, NOTICE_TYPE_YES);
		return 0 ;
	}
	if( m_pBSData->GetCategoryEnable(iIndex) == false )
	{
		m_iErrorDlgState.Set(BS_ERRDLG_NONE, EMPTY_NODE) ;
		g_pInterfaceManager->SetNotice("[BeautyShop]Select Layer is DISABLED.", this, NOTICE_TYPE_YES);
		return 0 ;
	}

	// ���� ���� üũ �� ��Ŷ ����
	return OnCommitCheck(iIndex) ;
}


/**
	�������� Ȯ���ϰ� �ش� �����ۿ� ���� ��Ŷ�� �����Ѵ�.
	(������->�Ϲ� ����� : Ȯ���� ������ ��ŵ���� üũ)
	@param int uiID : ���õ� Index
	@param bool bChecked : �̹� Ȯ�εǾ����� ���� Ȯ��
*/
int SPWindowBeautyShop::OnCommitCheck(unsigned int _index, bool bChecked)
{
	BeautyShopUnit* selectItem = NULL ;
	CONTAINER_SLOT sendSlot[2] ;
	int iErrorCode ;

	//////////////////////////////////////////////////////////////////////////
	// ���õ� ���̾��� �������� �����´�.
	selectItem =  m_pBSData->GetSelectItem(_index) ;
	if( selectItem == NULL )
	{
		char szMsg[128];		
		wsprintf(szMsg, "[BeautyShop]Can't Get Item Information.");
		g_pInterfaceManager->SetNotice(szMsg, this, NOTICE_TYPE_YES);
		return 0 ;
	}

	iErrorCode = m_pBSData->CommitItemValidCheck(_index, bChecked, sendSlot[0], sendSlot[1]) ;
	if( iErrorCode < 0 )
	{
		SetErrorDlg(iErrorCode, _index) ;
		return 0 ;
	}

	//////////////////////////////////////////////////////////////////////////
	// Layer
	m_iCurSelButton = _index ;
	UINT8 selectLayer ;
	if(_index == BS_HAIR || _index == BS_HAIR_C)		selectLayer = FIGURE_BASIC_HAIR ;
	else if (_index == BS_EYE || _index == BS_EYE_C)	selectLayer = FIGURE_BASIC_FACE ;
	else												selectLayer = FIGURE_BASIC_SKIN ;
	
	//////////////////////////////////////////////////////////////////////////
	//
	//	��Ŷ ����		ITEM_CS_BASIC_FIGURE_CHANGE,
	//
	// int					iBeautyShopID;		// BEAUTY_SHOP.LDT::ItemID #
	// UINT8				iBasicFigureSlot;	//FIGURE_BASIC_TYPE �� �ϳ�
	// CONTAINER_SLOT		UsingItemPos1;  // ����������������ġ
	// ITEMNO				iUsingItemNo1;  // ��������������Ű
	// CONTAINER_SLOT		UsingItemPos2;  // ����������������ġ
	// ITEMNO				iUsingItemNo2;  // ��������������Ű
	CPacket kPacket(ITEM_CS_BASIC_FIGURE_CHANGE);
	kPacket.AddUINT32(selectItem->m_iItemID);
	kPacket.AddUINT8(selectLayer);	
	kPacket.AddData(&sendSlot[0], sizeof(CONTAINER_SLOT));
	kPacket.AddUINT64(selectItem->m_iNeedItemCode1);
	kPacket.AddData(&sendSlot[1], sizeof(CONTAINER_SLOT));
	kPacket.AddUINT64(selectItem->m_iNeedItemCode2);

	(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
	g_pCheckManager->SetDBBlock(true);

	//DXUTOutputDebugString( "SelectItem: ID[%d], CODE[%d] Need[%d, %d] Layer[%d]\n", selectItem->m_iItemID, selectItem->m_iItemCode,
	//				selectItem->m_iNeedItemCode1, selectItem->m_iNeedItemCode2, selectLayer);

	return 1 ;
}



/**
	RECV �����ϱ� Ȯ�� 

*/
int SPWindowBeautyShop::OnRecvFigureChange(WPARAM wParam, LPARAM lParam)
{
	//ITEM_SC_BASIC_FIGURE_CHANGE,  // �ֺ��ε鿡�Դ� ITEM_SC_FIGURE_ON 
		//GU_ID					iGUID;
		//GLOBAL_STRING_ID		iResultError;	
		//int					iBeautyShopID;	// ����
		//TRASH_ITEM			UsingItemPos1;	// ����������������ġ
		//ITEMNO				iUsingItemNo1;	// ��������������Ű
		//TRASH_ITEM			UsingItemPos2;	// ����������������ġ
		//ITEMNO				iUsingItemNo2;	// ��������������Ű
		//CONTAINER_ITEM		NewCharItem;	// ������ ����

	g_pCheckManager->SetDBBlock(false);
	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 0 ;

	GU_ID RcvCharGUID ;
	GLOBAL_STRING_ID iError;
	UINT32		changedFigure ;
	UINT64 usedItemNo[2] ;
	TRASH_ITEM usedItem[2] ;
	CONTAINER_ITEM changedItem ;

	pkPacket->ExtractUINT64(&RcvCharGUID);
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != RcvCharGUID) {
			return 0 ;
		}
	}
	pkPacket->ExtractUINT32(&iError);
	if( iError )
	{
		m_iErrorDlgState.Set(BS_ERRDLG_NONE, EMPTY_NODE) ;
		g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(iError), this, NOTICE_TYPE_YES);
		return 0 ;
	}

	pkPacket->ExtractUINT32(&changedFigure) ;
	//////////////////////////////////////////////////////////////////////////
	// Item ����
	// ����� NeedItem �� Inventory���� ���ŵǾ�� �Ѵ�.
	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return 1;

	pkPacket->Extract(&usedItem[0], sizeof(TRASH_ITEM));
	pkPacket->ExtractUINT64(&usedItemNo[0]);
	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( usedItem[0].iContainerSlot.SlotIndex > 0 )
	{
		usedItem[0].iContainerSlot.SlotIndex -= 1;
		pkInvenArchive->TrashItem(usedItem[0]);
	}

	pkPacket->Extract(&usedItem[1], sizeof(TRASH_ITEM));
	pkPacket->ExtractUINT64(&usedItemNo[1]);
	if( usedItem[1].iContainerSlot.SlotIndex > 0 )
	{
		usedItem[1].iContainerSlot.SlotIndex -= 1;
		pkInvenArchive->TrashItem(usedItem[1]);
	}

	pkPacket->Extract(&changedItem, sizeof(CONTAINER_ITEM));

	//////////////////////////////////////////////////////////////////////////
	// ĳ���� ��ü�� �ǻ��� �����Ǿ�� �Ѵ�.
	changedItem.SlotInfo.SlotIndex -= 1 ;		// Index Change
	pkInvenArchive->ItemSetting(changedItem) ;

	if( m_pBSData ) 	m_pBSData->SetDefaultCharSet(changedItem.Item.iItemID, m_iCurGender, false) ;

	// ���� �޽��� â
	m_iErrorDlgState.Set(BS_ERRDLG_NONE, EMPTY_NODE) ;
	g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(9007007), this, NOTICE_TYPE_YES);

	return 1 ;
}



//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowBeautyShop)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_BEAUTYSHOP_CHANGE_CLOSE,	OnExit )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_BEAUTYSHOP_CHAR_RESET,		OnCharacterRefresh )

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_BEAUTYSHOP_CATEGORY_LEFT,	WIID_BEAUTYSHOP_CATEGORY_LEFT + BS_INDEX_MAX,	OnChangeCategoryLeftClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_BEAUTYSHOP_CATEGORY_RIGHT,	WIID_BEAUTYSHOP_CATEGORY_RIGHT + BS_INDEX_MAX,	OnChangeCategoryRightClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_BEAUTYSHOP_DETAIL_LEFT,		WIID_BEAUTYSHOP_DETAIL_LEFT + BS_INDEX_MAX,		OnChangeDetailLeftClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_BEAUTYSHOP_DETAIL_RIGHT,	WIID_BEAUTYSHOP_DETAIL_RIGHT + BS_INDEX_MAX,		OnChangeDetailRightClick)

SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	WIID_BEAUTYSHOP_DETAIL_TOOLTIP,	WIID_BEAUTYSHOP_DETAIL_TOOLTIP + BS_INDEX_MAX,		OnItemDetailToolTipEnter)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		WIID_BEAUTYSHOP_DETAIL_TOOLTIP, WIID_BEAUTYSHOP_DETAIL_TOOLTIP + BS_INDEX_MAX,		OnItemDetailToolTipOut	)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	WIID_BEAUTYSHOP_DETAIL_TOOLTIP, WIID_BEAUTYSHOP_DETAIL_TOOLTIP + BS_INDEX_MAX,		OnItemDetailToolTipMove	)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_BEAUTYSHOP_COMMIT_BTN,	WIID_BEAUTYSHOP_COMMIT_BTN + BS_INDEX_MAX,	OnBeautyShopItemCommit)

SPIMESSAGE_COMMAND( SPIM_BEAUTYSHOP_FIGURECHANGE,					OnRecvFigureChange )
SPIMESSAGE_COMMAND( SPIM_NOTICE_YES,								OnYes)
SPIMESSAGE_COMMAND( SPIM_NOTICE_NO,									OnNo)

SPIMESSAGE_OBJ_MAP_END(SPWindow)