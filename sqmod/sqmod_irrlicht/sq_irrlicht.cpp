// sq_irrlicht.cpp

#include "stdafx.h"

#if defined( _MSC_VER ) || defined( _IRR_COMPILE_WITH_OPENGL_ ) || defined( _IRR_LINUX_X11_VIDMODE_ )
#	if !defined( OEX_ARM )
#		include <GL/gl.h>
#		define SQ_USE_OPENGL
#	endif
#endif

#if defined( _WIN32_WCE ) || defined( OEX_IPHONE )
#	define D2D_TYPE	irr::u32
#else
#	define D2D_TYPE	irr::s32
#endif

CSqIrrlicht::CSqIrrlicht()
{_STT();
	m_pCallbackQueue = oexNULL;

	m_pDevice = oexNULL;
	m_pSmgr = oexNULL;
	m_pDriver = oexNULL;
	m_pGui = oexNULL;
	m_pCamera = oexNULL;
	m_txtFps = oexNULL;
	m_llFrames = 0;
	m_fLastTime = 0;
	m_bQuit = 0;
	m_nWidth = 0;
	m_nHeight = 0;

	m_nDriverType = -1;
	m_colBackground = irr::video::SColor( 255, 0, 50, 100 );

	m_bStereo = 0;
	m_ulREyeKey = 0x0000ffff;
	m_ulLEyeKey = 0x00ff0000;
	m_fStereoWidth = 3.f;
	m_fStereoFocus = 1000.f;
}

CSqIrrlicht::~CSqIrrlicht()
{_STT();
	m_pCallbackQueue = oexNULL;

	m_pSmgr = oexNULL;
	m_pDriver = oexNULL;
	m_pGui = oexNULL;
	m_nDriverType = -1;
	m_txtFps = oexNULL;
	m_llFrames = 0;
	m_fLastTime = 0;
	m_bQuit = 0;
	m_nWidth = 0;
	m_nHeight = 0;

	// Lose animators
	m_lstMeshAnimators.clear();

	// Lose scene camera reference
	if ( m_pCamera )
	{	m_pCamera->drop();
		m_pCamera = oexNULL;
	} // end if

	// Lose device
	if ( m_pDevice )
	{	m_pDevice->closeDevice();
		m_pDevice->drop();
		m_pDevice = oexNULL;
	} // end if
}

int CSqIrrlicht::Init( const sqbind::stdString &sName, int width, int height, int fps )
{_STT();
	irr::SIrrlichtCreationParameters param;

#if defined( _WIN32_WCE )
	param.Bits = 16;
	param.Fullscreen = true;
	param.DriverType = irr::video::EDT_BURNINGSVIDEO;
	param.WindowSize = irr::core::dimension2d<D2D_TYPE>( width, height );
#elif defined( OEX_IPHONE )
	param.Bits = 16;
	param.Fullscreen = true;
	param.DriverType = irr::video::EDT_OGLES1;
	param.WindowSize = irr::core::dimension2d<D2D_TYPE>( width, height );
#else
	param.Bits = 32;
	param.AntiAlias = true;

#	if defined( OEX_WINDOWS ) && defined( _IRR_COMPILE_WITH_DIRECT3D_9_ )
//		param.DriverType = irr::video::EDT_DIRECT3D9;
		param.DriverType = irr::video::EDT_OPENGL;
#	else
		param.DriverType = irr::video::EDT_OPENGL;
#	endif
//	param.DriverType = irr::video::EDT_BURNINGSVIDEO;
	param.WindowSize = irr::core::dimension2d<D2D_TYPE>( width, height );
#endif

	m_pDevice = irr::createDeviceEx( param );

	if ( !m_pDevice )
		oexERROR( 0, oexT( "Failed to create irrlicht device" ) );

	else
	{
		// Save size
		m_nWidth = width;
		m_nHeight = height;

		// Save away driver type
		m_nDriverType = param.DriverType;

		// Set default event receiver
		m_er.SetDevice( this, &m_bQuit );
		m_pDevice->setEventReceiver( &m_er );

#if !defined( OEX_ANDROID )
		if ( sName.length() )
			m_pDevice->setWindowCaption( oexStrToStrWPtr( sName.c_str() ) );
#else
//		if ( sName.length() )
//			m_pDevice->setWindowCaption( sName.c_str() );
#endif

		// Grab components
		m_pSmgr = m_pDevice->getSceneManager();
		m_pDriver = m_pDevice->getVideoDriver();
		m_pGui = m_pDevice->getGUIEnvironment();

		if ( !oexCHECK_PTR( m_pSmgr ) )
			oexERROR( 0, oexT( "Scene manager is invalid" ) );

		if ( !oexCHECK_PTR( m_pDriver ) )
			oexERROR( 0, oexT( "Driver pointer is invalid" ) );

		if ( !oexCHECK_PTR( m_pGui ) )
			oexERROR( 0, oexT( "GUI environment is invalid" ) );

		m_pDriver->setTextureCreationFlag( irr::video::ETCF_ALWAYS_32_BIT, true );
//		m_pDriver->setTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS, true );


#if defined( _WIN32_WCE )

		// Set the filesystem relative to the executable
		m_pDevice->getFileSystem()->addFolderFileArchive ( oexStrToStr8( oexGetModulePath() ).Ptr() );

#endif

		if ( fps )
		{
			m_txtFps =
				m_pGui->addStaticText( L"FPS: xx", irr::core::rect< irr::s32 >( 60, 5, 200, 20 ), false );

			m_pGui->addButton( irr::core::rect< int >( 10, 5, 50, 20 ), 0, 2, L"Quit" );

		} // end if

		// Initialize physics engine
		m_cPhysics.Init();

	} // end else

	return 1;
}

/// Sets the ambient color lighting
int CSqIrrlicht::SetAmbientLight( CSqirrColorf &x_col )
{_STT();
	if ( !m_pDriver )
		return 0;

	m_pDriver->setAmbientLight( x_col.Obj() );

	return 1;
}

CSqirrNode CSqIrrlicht::AddLight( CSqirrVector3d &pos, CSqirrColorf &col, float rad )
{_STT();
	if ( !m_pSmgr )
		return CSqirrNode();

	return m_pSmgr->addLightSceneNode( 0, pos.Obj(), col.Obj(), rad );
}

CSqirrCamera CSqIrrlicht::AddCamera( CSqirrVector3d &pos, CSqirrVector3d &look )
{_STT();
	if ( !m_pSmgr )
		return CSqirrCamera();

	irr::scene::ICameraSceneNode *pCamera =
		m_pSmgr->addCameraSceneNode( 0, pos.Obj(), look.Obj() );

	if ( !pCamera )
		return CSqirrCamera();

	if ( !m_pCamera )
	{	m_pCamera = pCamera;
		m_pCamera->grab();
	} // end if

	return pCamera;
}

CSqirrNode CSqIrrlicht::AddCube( float size )
{_STT();
	if ( !m_pSmgr )
		return CSqirrNode();

	return m_pSmgr->addCubeSceneNode( size );
}

CSqirrAnimator CSqIrrlicht::AddRotateAnimator( CSqirrVector3d &speed )
{_STT();
	CSqirrAnimator rotate( m_pSmgr->createRotationAnimator( speed.Obj() ) );
	if ( rotate.Ptr() ) rotate.Ptr()->drop();
	return rotate;
}

int CSqIrrlicht::Draw( CSqirrColor &bg )
{_STT();
	if ( m_bQuit || !m_pDevice || !m_pDevice->run() )
		return -1;

	// Time values
	float fBootSeconds = oexGetBootSeconds();
	float fElapsed = m_fLastTime ? fBootSeconds - m_fLastTime : 0;

	// Update fps
#if !defined( OEX_ANDROID )
	if ( m_txtFps && !( m_llFrames % 10 ) )
		m_txtFps->setText ( oexMksW( m_pDriver->getFPS() ).Ptr() );
#else
//	if ( m_txtFps && !( m_llFrames % 10 ) )
//		m_txtFps->setText ( oexMks( m_pDriver->getFPS() ).Ptr() );
#endif

	// Animate meshes
	AnimateMeshes();

	// Update physics engine
	m_cPhysics.Update( fElapsed );

	if ( m_bStereo && m_pCamera )
		DrawAnaglyph( m_pDriver, m_pSmgr, m_pGui, m_pCamera, bg.Obj(),
					  m_fStereoWidth, m_fStereoFocus, m_nDriverType,
					  m_ulREyeKey, m_ulLEyeKey );

	else
	{
		m_pDriver->beginScene( true, true, bg.Obj() );
		m_pSmgr->drawAll();
		m_pGui->drawAll();
		m_pDriver->endScene();

	} // end else

	// Count a frame
	m_llFrames++;
	m_fLastTime = fBootSeconds;

	return 0;
}

int CSqIrrlicht::DrawAnaglyph( irr::video::IVideoDriver *pDriver,
							   irr::scene::ISceneManager *pSm,
							   irr::gui::IGUIEnvironment *pGui,
							   irr::scene::ICameraSceneNode *pCamera,
							   irr::video::SColor colBackground,
							   float fWidth, float fFocus, int nDriverType,
							   unsigned long ulREyeKey, unsigned long ulLEyeKey )
{_STT();
	// Right eye
	irr::core::vector3df reye = pCamera->getPosition();

	// Left eye
	irr::core::vector3df v( fWidth, 0, 0 );
	irr::core::matrix4 m;
	m.setRotationDegrees( pCamera->getRotation() + irr::core::vector3df( 90.f, 0, 0 ) );
	m.transformVect( v );
	irr::core::vector3df leye = reye + v;

	// Eye target
	irr::core::vector3df oldt = pCamera->getTarget();
	irr::core::vector3df eyet = ( oldt - reye ).normalize() * fFocus;
	pCamera->setTarget( eyet );

	IDirect3DDevice9 *pDdx9 = NULL;
	if ( nDriverType == irr::video::EDT_DIRECT3D9
			  && pDriver->getExposedVideoData().D3D9.D3DDev9 )
	  pDdx9 = pDriver->getExposedVideoData().D3D9.D3DDev9;

	pDriver->beginScene( true, true, colBackground );

	if ( 0 )
		;

	// Setup right eye
#if defined( SQ_USE_OPENGL )

	else if ( nDriverType == irr::video::EDT_OPENGL )
	{   glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glColorMask( 0 != ( ulREyeKey & 0x00ff0000 ),
					 0 != ( ulREyeKey & 0x0000ff00 ),
					 0 != ( ulREyeKey & 0x000000ff ),
					 0 != ( ulREyeKey & 0xff000000 ) );
	} // end if

#endif

#if defined( _IRR_COMPILE_WITH_DIRECT3D_9_ ) && defined( OEX_ENABLE_DIRECTX )

	else if ( pDdx9 )
		pDdx9->SetRenderState( D3DRS_COLORWRITEENABLE,
							   ( ulREyeKey & 0x00ff0000 ? D3DCOLORWRITEENABLE_RED : 0 )
							   | ( ulREyeKey & 0x0000ff00 ? D3DCOLORWRITEENABLE_GREEN : 0 )
							   | ( ulREyeKey & 0x000000ff ? D3DCOLORWRITEENABLE_BLUE : 0 )
							   | ( ulREyeKey & 0xff000000 ? D3DCOLORWRITEENABLE_ALPHA : 0 ) );
#endif

	pSm->drawAll();

	if ( 0 )
		;

#if defined( SQ_USE_OPENGL )

	// Left Eye
	if ( nDriverType == irr::video::EDT_OPENGL )
	{   glClear( GL_DEPTH_BUFFER_BIT );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glColorMask( 0 != ( ulLEyeKey & 0x00ff0000 ),
					 0 != ( ulLEyeKey & 0x0000ff00 ),
					 0 != ( ulLEyeKey & 0x000000ff ),
					 0 != ( ulLEyeKey & 0xff000000 ) );
	} // end if

#endif

#if defined( _IRR_COMPILE_WITH_DIRECT3D_9_ ) && defined( OEX_ENABLE_DIRECTX )

	else if ( pDdx9 )
	{   pDdx9->Clear(0 , 0 , D3DCLEAR_ZBUFFER, 0x000000 , 1.0f , 0);
		pDdx9->SetRenderState( D3DRS_COLORWRITEENABLE,
							   ( ulLEyeKey & 0x00ff0000 ? D3DCOLORWRITEENABLE_RED : 0 )
							   | ( ulLEyeKey & 0x0000ff00 ? D3DCOLORWRITEENABLE_GREEN : 0 )
							   | ( ulLEyeKey & 0x000000ff ? D3DCOLORWRITEENABLE_BLUE : 0 )
							   | ( ulLEyeKey & 0xff000000 ? D3DCOLORWRITEENABLE_ALPHA : 0 ) );
	} // end else if

#endif

	pCamera->setPosition( leye );
	pCamera->OnRegisterSceneNode();

	pSm->drawAll();

	if ( 0 )
		;

#if defined( SQ_USE_OPENGL )

	if ( nDriverType == irr::video::EDT_OPENGL )
		glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

#endif

#if defined( _IRR_COMPILE_WITH_DIRECT3D_9_ ) && defined( OEX_ENABLE_DIRECTX )

	else if ( pDdx9 )
		pDdx9->SetRenderState( D3DRS_COLORWRITEENABLE,
							   D3DCOLORWRITEENABLE_BLUE
							   | D3DCOLORWRITEENABLE_GREEN
							   | D3DCOLORWRITEENABLE_RED
							   | D3DCOLORWRITEENABLE_ALPHA );
#endif

	if ( pGui )
		pGui->drawAll();

	pDriver->endScene();

	// Restore original position
	pCamera->setPosition( reye );
	pCamera->setTarget( oldt );

	return 1;
}

int CSqIrrlicht::RenderToTexture( CSqirrTexture *txt, CSqirrNode *pCamera )
{_STT();
	// Sanity checks
	if ( !txt || !txt->Ptr() || !m_pSmgr || !m_pDriver )
		return 0;

	// User defined camera?
	int bUserCamera = 0;
	if ( pCamera && pCamera->IsValid()
		 && CSqirrNode::eTypeCamera == pCamera->GetNodeType() )
		m_pSmgr->setActiveCamera( (irr::scene::ICameraSceneNode*)pCamera->Ptr() ), bUserCamera = 1;

	// Setup to render to texture
	m_pDriver->setRenderTarget( txt->Ptr() );

	// Draw the scene
	m_pSmgr->drawAll();

	// Clear render target
	m_pDriver->setRenderTarget( 0, true, true, 0 );

	// Update mip maps if needed
	if ( txt->Ptr()->hasMipMaps() )
		txt->Ptr()->regenerateMipMapLevels();

	// Restore camera if needed
	if ( bUserCamera && m_pCamera )
		m_pSmgr->setActiveCamera( m_pCamera );

	return 1;
}

int CSqIrrlicht::Capture( sqbind::CSqBinary *pBin )
{_STT();
	if ( !pBin || !m_pDevice || 0 >= getWidth() || 0 >= getHeight() )
		return 0;

#ifdef SQ_USE_OPENGL

	if ( irr::video::EDT_OPENGL == m_nDriverType )
	{
		oex::oexULONG uSize = getWidth() * getHeight() * 4;
		if ( !pBin->Allocate( uSize ) )
			return 0;

		// Read pixels from open gl
		glReadPixels( 0, 0, getWidth(), getHeight(),
//					  GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV,
					  GL_RGBA, GL_UNSIGNED_BYTE,
					  (GLvoid*)pBin->Ptr() );

		pBin->setUsed( uSize );

		return 1;

	} // end if

#endif

	return 0;
}


CSqirrNode CSqIrrlicht::AddSkyDome( const sqbind::stdString &sFile,
									long lHorzRes, long lVertRes,
									float fTxtPercent, float fSpherePercent )
{_STT();
	if ( !sFile.length() )
		return CSqirrNode();

	return
		m_pSmgr->addSkyDomeSceneNode( m_pDriver->getTexture( sFile.c_str() ),
									  lHorzRes, lVertRes, fTxtPercent, fSpherePercent );
}

// local pos = ScreenToPlane( SquirrVector2d( x, y ), 50 );
CSqirrVector3d CSqIrrlicht::ScreenToPlane( CSqirrVector2d &ptScreen, float fDist )
{_STT();
	// Must have object
	if ( !m_pCamera || !m_pSmgr )
		return CSqirrVector3d();

	// Is mouse in the window?
	if ( 0 > ptScreen.Obj().X || 0 > ptScreen.Obj().Y )
		return CSqirrVector3d();

	// Update the camera view
	m_pCamera->OnRegisterSceneNode();

	// Get mouse line
	irr::core::line3df ml =
		m_pSmgr->getSceneCollisionManager()->
					getRayFromScreenCoordinates( irr::core::position2di( (unsigned int)ptScreen.Obj().X, (unsigned int)ptScreen.Obj().Y ),
												 m_pCamera );

	// Get the camera target line
	irr::core::line3df cl( m_pCamera->getPosition(), m_pCamera->getTarget() );

	// Calculate the orthogonal plane position
	irr::core::vector3df pos = cl.start + ( cl.getVector().normalize() * fDist );

	// Calculate camera line plane intersection
	irr::core::vector3df ptWorld;
	if ( !irr::core::plane3df( pos, cl.getVector().normalize() ).
					getIntersectionWithLine( ml.getMiddle(), ml.getVector(), ptWorld ) )
		return CSqirrVector3d();

	return ptWorld;
}

CSqirrNode CSqIrrlicht::NodeAtScreen( CSqirrVector2d &ptScreen, long lMask )
{_STT();
	if ( !m_pSmgr || !m_pCamera || 0 > ptScreen.x() || 0 > ptScreen.y() )
		return CSqirrNode();

	irr::core::position2d< irr::s32 > pos;
	pos.X = (int)ptScreen.x(); pos.Y = (int)ptScreen.y();

	irr::core::line3d< irr::f32 > line =
		m_pSmgr->getSceneCollisionManager()->getRayFromScreenCoordinates( pos, m_pCamera );

	// Which node was clicked
	irr::scene::ISceneNode *pNode =
		m_pSmgr->getSceneCollisionManager()->getSceneNodeFromRayBB( line, lMask );

	if ( !pNode )
		return CSqirrNode();

	// Drop if not visible
	if ( !pNode->isVisible() )
		pNode = 0;

	const irr::core::list< irr::scene::ISceneNode* >& children = m_pSmgr->getRootSceneNode()->getChildren();
	for ( irr::core::list< irr::scene::ISceneNode* >::ConstIterator it = children.begin();
		  it != children.end(); ++it )
		if ( *it && *it != pNode && (*it)->isVisible() && 0 < (*it)->getID() && 0 != ( lMask & (*it)->getID() ) )
		{
			irr::core::aabbox3df bb = (*it)->getTransformedBoundingBox();
			if ( bb.intersectsWithLine( line ) )
				if ( !pNode || bb.intersectsWithBox( pNode->getTransformedBoundingBox() ) )
					pNode = *it;

		} // end for

	// Ensure mask is correct
	if ( 0 > pNode->getID() || !( lMask & pNode->getID() ) )
		pNode = NULL;

	return pNode;
}

int CSqIrrlicht::InsertPoints(   irr::scene::IMeshSceneNode *pNode,
								 irr::scene::SMeshBuffer **pMb,
								 unsigned int lNewVertices, unsigned int lNewIndices,
								 unsigned int &vi, unsigned int &ii )
{_STT();
	// Sanity check
	if ( !pNode || !pMb )
		return 0;

	// New mesh buffer
	*pMb = new irr::scene::SMeshBuffer();
	if ( !*pMb )
		return 0;

	// Existing mesh?
	irr::scene::IMeshBuffer *pOldMb = 0;
	if ( pNode->getMesh() && pNode->getMesh()->getMeshBufferCount() )
		pOldMb = pNode->getMesh()->getMeshBuffer( 0 );

	// Old buffer?
	if ( !pOldMb || 2 > pOldMb->getVertexCount() )
	{
		// Just add the two points
		(*pMb)->Vertices.set_used( lNewVertices );
		(*pMb)->Indices.set_used( lNewIndices );

		vi = 0;
		ii = 0;

		return 1;

	} // end else

	// Get current counts
	vi = pOldMb->getVertexCount();
	ii = pOldMb->getIndexCount();

	// Allocate new buffers
	(*pMb)->Vertices.set_used( vi + lNewVertices );
	(*pMb)->Indices.set_used( ii + lNewIndices );

	// Copy old data
	if ( vi )
//        memcpy( (*pMb)->getVertices(), pOldMb->getVertices(), pOldMb->getVertexPitch() * vi );
		memcpy( (*pMb)->getVertices(), pOldMb->getVertices(), irr::video::getVertexPitchFromType( pOldMb->getVertexType() ) * vi );
	if ( ii )
		memcpy( (*pMb)->getIndices(), pOldMb->getIndices(), sizeof( irr::u16 ) * ii );

	return 1;
}

int CSqIrrlicht::InsertPlane( irr::scene::IMeshSceneNode *pNode,
							   const irr::core::vector3df &tl, const irr::core::vector3df &tr,
							   const irr::core::vector3df &bl, const irr::core::vector3df &br,
							   irr::video::SColor &color, long lSides,
							   float minU, float maxU, float minV, float maxV )
{_STT();
	// Add new points
	unsigned int ovi = 0, oii = 0;
	irr::scene::SMeshBuffer *pMb = NULL;
	if ( !InsertPoints( pNode, &pMb, ( 1 < lSides ) ? 8 : 4, ( 1 < lSides ) ? 12 : 6, ovi, oii ) )
		return FALSE;

	unsigned int vi = ovi, ii = oii;

	pMb->Vertices[ vi++ ] = irr::video::S3DVertex( tl, irr::core::vector3df( 0, 0, 1 ), color, irr::core::vector2df( 1.f - minU, 1.f - maxV ) );
	pMb->Vertices[ vi++ ] = irr::video::S3DVertex( bl, irr::core::vector3df( 0, 0, 1 ), color, irr::core::vector2df( 1.f - minU, 1.f - minV ) );
	pMb->Vertices[ vi++ ] = irr::video::S3DVertex( br, irr::core::vector3df( 0, 0, 1 ), color, irr::core::vector2df( 1.f - maxU, 1.f - minV ) );
	pMb->Vertices[ vi++ ] = irr::video::S3DVertex( tr, irr::core::vector3df( 0, 0, 1 ), color, irr::core::vector2df( 1.f - maxU, 1.f - maxV ) );

	pMb->Indices[ ii++ ] = ovi + 1;
	pMb->Indices[ ii++ ] = ovi + 2;
	pMb->Indices[ ii++ ] = ovi + 3;
	pMb->Indices[ ii++ ] = ovi + 3;
	pMb->Indices[ ii++ ] = ovi + 0;
	pMb->Indices[ ii++ ] = ovi + 1;

	// Back side?
	if ( 1 < lSides )
	{
	pMb->Vertices[ vi++ ] = irr::video::S3DVertex( tl, irr::core::vector3df( 0, 0, -1 ), color, irr::core::vector2df( 1.f - minU, 1.f - maxV ) );
	pMb->Vertices[ vi++ ] = irr::video::S3DVertex( bl, irr::core::vector3df( 0, 0, -1 ), color, irr::core::vector2df( 1.f - minU, 1.f - minV ) );
	pMb->Vertices[ vi++ ] = irr::video::S3DVertex( br, irr::core::vector3df( 0, 0, -1 ), color, irr::core::vector2df( 1.f - maxU, 1.f - minV ) );
	pMb->Vertices[ vi++ ] = irr::video::S3DVertex( tr, irr::core::vector3df( 0, 0, -1 ), color, irr::core::vector2df( 1.f - maxU, 1.f - maxV ) );

		pMb->Indices[ ii++ ] = ovi + 4 + 1;
		pMb->Indices[ ii++ ] = ovi + 4 + 3;
		pMb->Indices[ ii++ ] = ovi + 4 + 2;
		pMb->Indices[ ii++ ] = ovi + 4 + 3;
		pMb->Indices[ ii++ ] = ovi + 4 + 1;
		pMb->Indices[ ii++ ] = ovi + 4 + 0;

	} // end if

	pMb->recalculateBoundingBox();

	irr::scene::SMesh *pMesh = new irr::scene::SMesh();
	if ( !pMesh ) return 0;

	pMesh->addMeshBuffer( pMb );
	pMb->drop();

	pMesh->recalculateBoundingBox();

	pNode->setMesh( pMesh );
	pMesh->drop();

	return 1;
}

CSqirrNode CSqIrrlicht::AddPlane( float fWidth, float fHeight, long lSides, CSqirrColor &rColor )
{_STT();
	if ( !m_pSmgr )
		return CSqirrNode();

	irr::scene::SMesh *pMesh = new irr::scene::SMesh();
	if ( !pMesh )
		return CSqirrNode();

	// Add empty mesh
	irr::scene::IMeshSceneNode *pNode =
		m_pSmgr->addMeshSceneNode( pMesh );
	pMesh->drop();

	if ( !pNode )
		return CSqirrNode();

	// Add a plane
	InsertPlane( pNode, irr::core::vector3df( 0, fHeight, 0 ),
						irr::core::vector3df( fWidth, fHeight, 0 ),
						irr::core::vector3df( 0, 0, 0 ),
						irr::core::vector3df( fWidth, 0, 0 ),
						rColor.Obj(), lSides, 0, 1, 0, 1 );

	for ( unsigned int i = 0; i < pNode->getMaterialCount(); i++ )
	{   pNode->getMaterial( i ).NormalizeNormals = true;
		pNode->getMaterial( i ).Shininess = 0;
	} // end for

	return pNode;
}

CSqirrNode CSqIrrlicht::AddGrid( float fWidth, float fHeight,
								 long lXPanels, long lYPanels,
								 float fSpace, long lSides,
								 CSqirrColor &rColor )
{_STT();
	if ( !m_pSmgr )
		return CSqirrNode();

	irr::scene::SMesh *pMesh = new irr::scene::SMesh();
	if ( !pMesh )
		return CSqirrNode();

	// Add empty mesh
	irr::scene::IMeshSceneNode *pNode =
		m_pSmgr->addMeshSceneNode( pMesh );
	pMesh->drop();

	if ( !pNode )
		return CSqirrNode();

	// Put a bunch of planes in the mesh
	long sy = lXPanels;
	long sx = lYPanels;
	float xp, yp;
	float xs = fWidth - fSpace;
	float ys = fHeight - fSpace;

	for ( long y = 0; y < sy; y++ )
	{
		yp = ( y - ( sy / 2 ) ) * ys + ( y - ( sy / 2 ) ) * fSpace;

		for ( long x = 0; x < sx; x++ )
		{
			xp = ( x - ( sx / 2 ) ) * xs + ( x - ( sx / 2 ) ) * fSpace;

			InsertPlane( pNode, irr::core::vector3df( xp, yp + ys, 0 ),
								irr::core::vector3df( xp + xs, yp + ys, 0 ),
								irr::core::vector3df( xp, yp, 0 ),
								irr::core::vector3df( xp + xs, yp, 0 ),
								rColor.Obj(), lSides,
								(float)x / (float)sx, (float)( x + 1 ) / (float)sx,
								(float)y / (float)sy, (float)( y + 1 ) / (float)sy );
		} // end for

	} // end for

	for ( unsigned int i = 0; i < pNode->getMaterialCount(); i++ )
	{   pNode->getMaterial( i ).NormalizeNormals = true;
		pNode->getMaterial( i ).Shininess = 0;
	} // end for

	return pNode;
}

/*

// Main entry point
function _init() : ( _g )
{
	local node = _irr.AddGrid( 10., 10., 10, 10, 0.2, 2
							   CSiColor( 255, 255, 255, 255 ), 2 );

	_irr.AddMeshAnimator( node, OnAnimate, 0 );

}

function _mod_fp( _x, _y )
{	local n = ( _x / _y ).tointeger();
	return _x - n * _y;
}

function OnAnimate( n, o, c )
{
//    _self.alert( n.tostring() );

	local pi = 3.141592654;
	local pi2 = pi * 2.;

	local attn = 6;
	local m = pi2 / 100;

	local clk = _irr.clock();

	for ( local i = 0; i < n; i++ )
	{
		// Normalize space
		local u = o.x( i ) * m;
//        local u = _mod_fp( o.x( i ) * m + clk, pi );
		local v = o.y( i ) * m;
//        local v = _mod_fp( o.y( i ) * m + clk, pi );
		local x = 0, y = 0, z = 0;

		// Cylinder
// 		x = sin( u );
//		y = ty;
//		z = cos( u );

		// Cone
//		x = sin( u ) * ( pi - v ) * 0.2;
//		y = v;
//		z = cos( u ) * ( pi - v ) * 0.2;

		// Sphere
//		x = sin( u ) * cos( v / 2 );
//		y = sin( v / 2 );
//		z = cos( u ) * cos( v / 2 );

		// Torus
//		x = ( 2 + cos( v ) ) * cos( u );
//		y = sin( v );
//		z = ( 2 + cos( v ) ) * sin( u );

		// Toroid
//		u += pi; u *= 1.5;
//		x = cos( u ) * ( u / ( 3 * pi ) * cos( v ) + 2 );
//		y = u * sin( v ) / ( 3 * pi );
//		z = sin( u ) * ( u / ( 3 * pi ) * cos( v ) + 2 );

		// Coil
//		x = cos( u * 1.5 ) * ( cos( v ) + 2 );
//		y = sin( u * 1.5 ) * ( cos( v ) + 2 );
//		z = sin( v ) + u;

		// Trefoil Knot
		local a = 0.5;
		u *= 2;
		x = a * ( cos( u ) * cos( v ) + 3 * cos( u ) * ( 1.5 + sin( 1.5 * u ) / 2 ) );
		y = a * ( sin( v ) + 2 * cos( 1.5 * u ) );
		z = a * ( sin( u ) * cos( v ) + 3 * sin( u ) * ( 1.5 + sin( 1.5 * u ) / 2 ) );

		// Nautilus
//		u += pi;
//		x = 0.5 * u * cos( u ) * ( cos( v ) + 1 );
//		y = 0.5 * u * sin( v );
//		z = 0.5 * u * sin( u ) * ( cos( v ) + 1 );

		// Mobius Strip
//		u += pi; // 0 <= u < 2pi
//		v *= 0.2; // -t <= v < t
//		x = cos( u ) + v * cos( u / 2 ) * cos( u );
//		y = sin( u ) + v * cos( u / 2 ) * sin( u );
//		z = v * sin( u / 2 );

		// Klein Bottle
//		local a = 2.0;
//		v += pi; // 0 <= v < 2pi
//		x = ( a + cos( u / 2 ) * sin( v ) - sin( u / 2 ) * sin( 2 * v ) ) * cos( u );
//		y = sin( u / 2 ) * sin( v ) + cos( u / 2 ) * sin( 2 * v );
//		z = ( a + cos( u / 2 ) * sin( v ) - sin( u / 2 ) * sin( 2 * v ) ) * sin( u );

		// Dini's surface
//		u += pi; u *= 2; // 0 <= u < 4pi
//		x = cos( u ) * sin( v );
//		y = -2 - ( cos( v ) + log( tan( v / 2 ) ) + 0.2 * u - 4 );
//		z = sin( u ) * sin( v );

		// Flag
//		 x = u;
//		 y = v;
//		 z = 0;

		// Waving
//        z += sin( u * 2. + clk / 2 ) + sin( u + ( clk * 2.2 ) );
//        z += sin( v * 1. + clk ) + sin( v * 1.1 + ( clk * 1.2 ) );
//        z *= ( ( u - pi ) / pi2 ) / attn;

		c.set( i, x / m, y / m, z / m );

	} // end for

}

*/
int CSqIrrlicht::AddMeshAnimator( sqbind::CSqEngineExport *e, CSqirrNode &n, SquirrelObject soF, long lFreq )
{_STT();
	if ( !m_pSmgr || !n.IsValid() || !e || !e->GetVmPtr() )
		return 0;

	if ( !lFreq )
	{
		CSqirrMeshAnimator ma;
		if ( !ma.Init( (irr::scene::IMeshSceneNode*)n.Ptr(), soF, e->GetVmPtr()->GetVMHandle(), 0 ) )
			return 0;

		// Let Squirrel manipulate the mesh
		ma.Run( m_pSmgr, oexGetBootSeconds(), e->GetEnginePtr() );

		// Just leave the mesh as is
		ma.Detach();

	} // end if

	else
	{
		CSqirrMeshAnimator &rMa = m_lstMeshAnimators[ &n ];

		if ( !rMa.Init( (irr::scene::IMeshSceneNode*)n.Ptr(), soF, e->GetVmPtr()->GetVMHandle(), lFreq ) )
		{   m_lstMeshAnimators.erase( &n );
			return 0;
		} // end if

		// Initialize the mesh
		rMa.Run( m_pSmgr, oexGetBootSeconds(), e->GetEnginePtr() );

	} // end else

	return 1;
}

void CSqIrrlicht::AnimateMeshes()
{_STT();
	if ( !m_pSmgr )
		return;

	// For each mesh animator
	for ( t_MeshAnimators::iterator it = m_lstMeshAnimators.begin();
			m_lstMeshAnimators.end() != it; it++ )
		it->second.Run( m_pSmgr, oexGetBootSeconds(), oexNULL );
}

CSqirrNode CSqIrrlicht::AddMesh( const sqbind::stdString &sFile, float x_fScale, int x_bClearFromCache )
{_STT();
	if ( !m_pSmgr || !sFile.length() )
		return CSqirrNode();

	try
	{
		irr::scene::IAnimatedMesh *pMesh = m_pSmgr->getMesh( oexStrToMbPtr( sFile.c_str() ) );
		if ( !pMesh )
			return CSqirrNode();

		irr::scene::IAnimatedMeshSceneNode *pAniMesh = m_pSmgr->addAnimatedMeshSceneNode( pMesh );
		if ( !pAniMesh )
			return CSqirrNode();

		// Set the scale
		pAniMesh->setScale( irr::core::vector3df( x_fScale, x_fScale, x_fScale ) );

		// Remove from cache?
		if ( x_bClearFromCache )
			m_pSmgr->getMeshCache()->removeMesh( pAniMesh->getMesh() );

		for ( unsigned int i = 0; i < pAniMesh->getMaterialCount(); i++ )
		{   pAniMesh->getMaterial( i ).NormalizeNormals = true;
			pAniMesh->getMaterial( i ).Shininess = 0;
		} // end for
//      pAniMesh->addShadowVolumeSceneNode( 0, true, 100.f );
		pAniMesh->setMaterialFlag( irr::video::EMF_LIGHTING, true );

		return pAniMesh;

	} // end try

	catch( ... )
	{
	} // end catch

	return CSqirrNode();
}

int CSqIrrlicht::SetVertexColorAlpha( CSqirrNode &x_node, float x_fAlpha )
{
	if ( !m_pSmgr || !x_node.IsValid() )
		return 0;

    // Set vertex colors
	if ( CSqirrNode::eTypeMesh == x_node.GetNodeType() )
        m_pSmgr->getMeshManipulator()->setVertexColorAlpha( ( (irr::scene::IMeshSceneNode*)x_node.Ptr() )->getMesh(), (irr::s32)x_fAlpha );

	else if ( CSqirrNode::eTypeAnimatedMesh == x_node.GetNodeType() )
        m_pSmgr->getMeshManipulator()->setVertexColorAlpha( ( (irr::scene::IAnimatedMeshSceneNode*)x_node.Ptr() )->getMesh()->getMesh( 0 ), (irr::s32)x_fAlpha );

    else
		return 0;

    return 1;
}


int CSqIrrlicht::SetVertexColors( CSqirrNode &x_node, CSqirrColor &x_col )
{_STT();

	if ( !m_pSmgr || !x_node.IsValid() )
		return 0;

	// Set vertex colors
	if ( CSqirrNode::eTypeMesh == x_node.GetNodeType() )
	m_pSmgr->getMeshManipulator()->setVertexColors( ( (irr::scene::IMeshSceneNode*)x_node.Ptr() )->getMesh(),
													  x_col.Obj() );
//	m_pSmgr->getMeshManipulator()->apply( irr::scene::SVertexColorSetManipulator(color), ( (irr::scene::IMeshSceneNode*)x_node.Ptr() )->getMesh() );

	else if ( CSqirrNode::eTypeAnimatedMesh == x_node.GetNodeType() )
		m_pSmgr->getMeshManipulator()->setVertexColors( ( (irr::scene::IAnimatedMeshSceneNode*)x_node.Ptr() )->getMesh()->getMesh( 0 ),
													  x_col.Obj() );

	else
		return 0;

	return 1;
}


CSqirrTexture CSqIrrlicht::CreateTexture( long lWidth, long lHeight, int bMipMapping )
{_STT();
	if ( !m_pDriver || !m_pSmgr || 0 >= lWidth || 0 >= lHeight )
		return CSqirrTexture();

	// Set mip-mapping state
	int bCurMipMapping = m_pDriver->getTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS );
	m_pDriver->setTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS, bMipMapping ? true : false );

	// Create texture
//	CSqirrTexture tex( m_pDriver->addTexture( irr::core::dimension2d< D2D_TYPE >( lWidth, lHeight ), "Texture" /*, ECF_A8R8G8B8 */ ) );
	CSqirrTexture tex( m_pDriver->addTexture( irr::core::dimension2d< irr::u32 >( lWidth, lHeight ), "Texture" /*, ECF_A8R8G8B8 */ ) );

	// Restore mip mapping state
	m_pDriver->setTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS, bCurMipMapping ? true : false );

	return tex;
}

CSqirrTexture CSqIrrlicht::CreateRenderTexture( long lWidth, long lHeight, int bMipMapping )
{_STT();
	if ( !m_pDriver || !m_pSmgr || 0 >= lWidth || 0 >= lHeight )
		return CSqirrTexture();

	// Set mip-mapping state
	int bCurMipMapping = m_pDriver->getTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS );
	m_pDriver->setTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS, bMipMapping ? true : false );

	// Create texture
//	CSqirrTexture tex( m_pDriver->createRenderTargetTexture( irr::core::dimension2d< irr::s32 >( lWidth, lHeight ) ) );
	CSqirrTexture tex( m_pDriver->addRenderTargetTexture( irr::core::dimension2d< irr::u32 >( lWidth, lHeight ) ) );

	// Restore mip mapping state
	m_pDriver->setTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS, bCurMipMapping ? true : false );

	return tex;
}


CSqirrTexture CSqIrrlicht::LoadTexture( const sqbind::stdString &sFile, int bMipMapping )
{_STT();
	if ( !m_pSmgr || !sFile.length() )
		return CSqirrTexture();

	if ( !oexExists( sFile.c_str() ) )
		return CSqirrTexture();

	// Set mip-mapping state
	int bCurMipMapping = m_pDriver->getTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS );
	m_pDriver->setTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS, bMipMapping ? true : false );

	// Load the texture
	CSqirrTexture tex( m_pDriver->getTexture( sFile.c_str() ) );

	// Restore mip mapping state
	m_pDriver->setTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS, bCurMipMapping ? true : false );

	return tex;
}

int CSqIrrlicht::InsertSphere( irr::scene::SMeshBuffer *pMb, const irr::core::vector3df &center,
								 float fWidth, float fHeight, long lPoints, const irr::video::SColor &color )
{_STT();
	float fHWidth = fWidth / 2;
	float fHHeight = fHeight / 2;

	irr::u16 uPoints = (irr::u16)lPoints;

	irr::u16 vi = pMb->Vertices.size();
	if ( vi ) pMb->Vertices.reallocate( vi + ( uPoints * 4 ) );
	pMb->Vertices.set_used( vi + ( uPoints * 4 ) );

	irr::u16 ii = pMb->Indices.size();
	if ( ii ) pMb->Indices.reallocate( ii + ( lPoints * 12 - 12 ) );
	pMb->Indices.set_used( ii + ( lPoints * 12 - 12 ) );

	irr::u16 tr = 0, str = uPoints, sbr = uPoints * 2, br = uPoints * 3;
	for ( irr::u16 i = 0; i < uPoints; i++ )
	{
		// Index points
		irr::u16 ti = tr + i, sti = str + i, sbi = sbr + i, bi = br + i;

		// Create vertex
		irr::core::vector3df v( center.X + cos( irr::core::PI * 2 * i / uPoints ) * fHWidth,
								center.Y + fHHeight,
								center.Z + sin( irr::core::PI * 2 * i / uPoints ) * fHWidth );

		// Texture coords
		irr::core::vector2df t( cos( irr::core::PI * 2 * i / uPoints ),
								sin( irr::core::PI * 2 * i / uPoints ) );

		// Top
		pMb->Vertices[ vi + ti ] = irr::video::S3DVertex( v, irr::core::vector3df( v ).normalize(), color, t );

		// Side / Top
		pMb->Vertices[ vi + sti ] = irr::video::S3DVertex( v, irr::core::vector3df( v ).normalize(), color,
														   irr::core::vector2df( (float)i / (float)( uPoints - 1 ), 0.f ) );

		// Side / Bottom
		v.Y = center.Y - fHHeight;
		pMb->Vertices[ vi + sbi ] = irr::video::S3DVertex( v, irr::core::vector3df( v ).normalize(), color,
														   irr::core::vector2df( (float)i / (float)( uPoints - 1 ), 1.f ) );

		// Bottom
		pMb->Vertices[ vi + bi ] = irr::video::S3DVertex( v, irr::core::vector3df( v ).normalize(), color, t );

		// Side / top
		pMb->Indices[ ii++ ] = vi + sbi;
		pMb->Indices[ ii++ ] = vi + sti;
		pMb->Indices[ ii++ ] = vi + ( ( ( i + 1 ) < uPoints ) ? sti + 1 : str );

		// Side / bottom
		pMb->Indices[ ii++ ] = vi + sbi;
		pMb->Indices[ ii++ ] = vi + ( ( ( i + 1 ) < uPoints ) ? sti + 1 : str );
		pMb->Indices[ ii++ ] = vi + ( ( ( i + 1 ) < uPoints ) ? sbi + 1 : sbr );

		if ( 2 <= i )
		{
			// Top
			pMb->Indices[ ii++ ] = vi + tr;
			pMb->Indices[ ii++ ] = vi + ti;
			pMb->Indices[ ii++ ] = vi + ( ti - 1 );

			// Bottom
			pMb->Indices[ ii++ ] = vi + br;
			pMb->Indices[ ii++ ] = vi + ( bi - 1 );
			pMb->Indices[ ii++ ] = vi + bi;

		} // end if

	} // end for

	return 1;
}

CSqirrNode CSqIrrlicht::AddSphere( float fRadius, long lPolyCount )
{_STT();
	if ( !m_pSmgr )
		return CSqirrNode();

	return m_pSmgr->addSphereSceneNode( fRadius, lPolyCount );
}

CSqirrNode CSqIrrlicht::AddSphereMesh( float fWidth, float fHeight, long lPoints )
{_STT();
	if ( !m_pSmgr )
		return CSqirrNode();

	if ( 2 > lPoints )
		return CSqirrNode();

	irr::scene::SMeshBuffer *pMb = new irr::scene::SMeshBuffer();

	InsertSphere( pMb, irr::core::vector3df(), fWidth, fHeight, lPoints, irr::video::SColor( 255, 255, 255, 255 ) );

	pMb->recalculateBoundingBox();

	irr::scene::SMesh *pMesh = new irr::scene::SMesh();
	pMesh->addMeshBuffer( pMb );
	pMb->drop();

	pMesh->recalculateBoundingBox();

	irr::scene::IMeshSceneNode *pNode =
		m_pSmgr->addMeshSceneNode( pMesh );
	pMesh->drop();

	for ( unsigned int i = 0; i < pNode->getMaterialCount(); i++ )
	{   pNode->getMaterial( i ).NormalizeNormals = true;
		pNode->getMaterial( i ).Shininess = 0;
//        pNode->getMaterial( i ).Shininess = 20;	// 0.5 - 128
//        pNode->getMaterial( i ).SpecularColor.set( 255, 255, 255, 255 );
	} // end for

	return pNode;
}

int CSqIrrlicht::InsertCylinder( irr::scene::SMeshBuffer *pMb, const irr::core::vector3df &center,
								 float fWidth, float fHeight, long lPoints, const irr::video::SColor &color )
{_STT();
	if ( !pMb )
		return 0;

	float fHWidth = fWidth / 2;
	float fHHeight = fHeight / 2;

	unsigned short uPoints = (unsigned short)lPoints;

	unsigned short vi = pMb->Vertices.size();
	if ( vi ) pMb->Vertices.reallocate( vi + ( uPoints * 4 ) );
	pMb->Vertices.set_used( vi + ( lPoints * 4 ) );

	unsigned short ii = pMb->Indices.size();
	if ( ii ) pMb->Indices.reallocate( ii + ( uPoints * 12 - 12 ) );
	pMb->Indices.set_used( ii + ( lPoints * 12 - 12 ) );

	unsigned short tr = 0, str = uPoints, sbr = uPoints * 2, br = uPoints * 3;
	for ( unsigned short i = 0; i < lPoints; i++ )
	{
		// Index points
		unsigned short ti = tr + i, sti = str + i, sbi = sbr + i, bi = br + i;

		// Create vertex
		irr::core::vector3df v( center.X + cos( irr::core::PI * 2 * i / uPoints ) * fHWidth,
								center.Y + fHHeight,
								center.Z + sin( irr::core::PI * 2 * i / uPoints ) * fHWidth );

		// Texture coords
		irr::core::vector2df t( cos( irr::core::PI * 2 * i / uPoints ),
								sin( irr::core::PI * 2 * i / uPoints ) );

		// Top
		pMb->Vertices[ vi + ti ] = irr::video::S3DVertex( v, irr::core::vector3df( v ).normalize(), color, t );

		// Side / Top
		pMb->Vertices[ vi + sti ] = irr::video::S3DVertex( v, irr::core::vector3df( v ).normalize(), color,
														   irr::core::vector2df( (float)i / (float)( uPoints - 1 ), 0.f ) );

		// Side / Bottom
		v.Y = center.Y - fHHeight;
		pMb->Vertices[ vi + sbi ] = irr::video::S3DVertex( v, irr::core::vector3df( v ).normalize(), color,
														   irr::core::vector2df( (float)i / (float)( uPoints - 1 ), 1.f ) );

		// Bottom
		pMb->Vertices[ vi + bi ] = irr::video::S3DVertex( v, irr::core::vector3df( v ).normalize(), color, t );

		// Side / top
		pMb->Indices[ ii++ ] = vi + sbi;
		pMb->Indices[ ii++ ] = vi + sti;
		pMb->Indices[ ii++ ] = vi + ( ( ( i + 1 ) < uPoints ) ? sti + 1 : str );

		// Side / bottom
		pMb->Indices[ ii++ ] = vi + sbi;
		pMb->Indices[ ii++ ] = vi + ( ( ( i + 1 ) < uPoints ) ? sti + 1 : str );
		pMb->Indices[ ii++ ] = vi + ( ( ( i + 1 ) < uPoints ) ? sbi + 1 : sbr );

		if ( 2 <= i )
		{
			// Top
			pMb->Indices[ ii++ ] = vi + tr;
			pMb->Indices[ ii++ ] = vi + ti;
			pMb->Indices[ ii++ ] = vi + ( ti - 1 );

			// Bottom
			pMb->Indices[ ii++ ] = vi + br;
			pMb->Indices[ ii++ ] = vi + ( bi - 1 );
			pMb->Indices[ ii++ ] = vi + bi;

		} // end if

	} // end for

	return 1;
}

CSqirrNode CSqIrrlicht::AddCylinderMesh( float fWidth, float fHeight, long lPoints )
{_STT();
	if ( !m_pSmgr )
		return CSqirrNode();

	if ( 2 > lPoints )
		return CSqirrNode();

	irr::scene::SMeshBuffer *pMb = new irr::scene::SMeshBuffer();

	InsertCylinder( pMb, irr::core::vector3df(), fWidth, fHeight, lPoints, irr::video::SColor( 255, 255, 255, 255 ) );

	pMb->recalculateBoundingBox();

	irr::scene::SMesh *pMesh = new irr::scene::SMesh();
	pMesh->addMeshBuffer( pMb );
	pMb->drop();

	pMesh->recalculateBoundingBox();

	irr::scene::IMeshSceneNode *pNode =
		m_pSmgr->addMeshSceneNode( pMesh );
	pMesh->drop();

	for ( unsigned int i = 0; i < pNode->getMaterialCount(); i++ )
	{   pNode->getMaterial( i ).NormalizeNormals = true;
		pNode->getMaterial( i ).Shininess = 0;
	} // end for

	return pNode;
}

int CSqIrrlicht::InsertCone( irr::scene::SMeshBuffer *pMb, irr::core::vector3df center,
							 float fWidth, float fHeight, long lPoints, const irr::video::SColor &color )
{_STT();

	float fHWidth = fWidth / 2;

	oex::oexUSHORT uPoints = (oex::oexUSHORT)lPoints;

	oex::oexUSHORT vi = pMb->Vertices.size();
	if ( vi ) pMb->Vertices.reallocate( vi + ( lPoints + 1 ) );
	pMb->Vertices.set_used( vi + ( lPoints + 1 ) );

	long ii = pMb->Indices.size();
	if ( ii ) pMb->Indices.reallocate( ii + ( lPoints * 6 - 6 ) );
	pMb->Indices.set_used( ii + ( lPoints * 6 - 6 ) );

	// Top point
	pMb->Vertices[ vi + uPoints ] = irr::video::S3DVertex( 0.f, fHeight, 0.f,  0.f, 1.f, 0.f, color, 0.5f, 0.5f );

	// Create the bottom of the cone
	for ( oex::oexUSHORT i = 0; i < lPoints; i++ )
	{
		// Create vertex
		irr::core::vector3df v( center.X + cos( irr::core::PI * 2 * i / uPoints ) * fHWidth,
								center.Y + 0.f,
								center.Z + sin( irr::core::PI * 2 * i / uPoints ) * fHWidth );

		pMb->Vertices[ vi + i ] = irr::video::S3DVertex( v, irr::core::vector3df( v ).normalize(), color,
														 irr::core::vector2df( cos( irr::core::PI * 2 * i / uPoints ),
																			   sin( irr::core::PI * 2 * i / uPoints ) ) );

		// Side
		pMb->Indices[ ii++ ] = vi + uPoints;
		pMb->Indices[ ii++ ] = vi + ( ( ( i + 1 ) < uPoints ) ? i + 1 : 0 );
		pMb->Indices[ ii++ ] = vi + i;

		if ( 2 <= i )
		{
			// Bottom
			pMb->Indices[ ii++ ] = vi + 0;
			pMb->Indices[ ii++ ] = vi + ( i - 1 );
			pMb->Indices[ ii++ ] = vi + i;

		} // end if

	} // end for

	return 1;
}

CSqirrNode CSqIrrlicht::AddConeMesh( float fWidth, float fHeight, long lPoints )
{_STT();
	if ( !m_pSmgr )
		return CSqirrNode();

	if ( 2 > lPoints )
		return CSqirrNode();

	irr::scene::SMeshBuffer *pMb = new irr::scene::SMeshBuffer();

	InsertCone( pMb, irr::core::vector3df(), fWidth, fHeight, lPoints, irr::video::SColor( 255, 255, 255, 255 ) );

	pMb->recalculateBoundingBox();

	irr::scene::SMesh *pMesh = new irr::scene::SMesh();
	pMesh->addMeshBuffer( pMb );
	pMb->drop();

	pMesh->recalculateBoundingBox();

	irr::scene::IMeshSceneNode *pNode =
		m_pSmgr->addMeshSceneNode( pMesh );
	pMesh->drop();

	for ( unsigned int i = 0; i < pNode->getMaterialCount(); i++ )
	{   pNode->getMaterial( i ).NormalizeNormals = true;
		pNode->getMaterial( i ).Shininess = 0;
	} // end for

	return pNode;
}


CSqirrNode CSqIrrlicht::AddArrowMesh( float fWidth, float fConeHeight, float fShaftHeight, long lPoints )
{_STT();
	if ( !m_pSmgr )
		return CSqirrNode();

	if ( 2 > lPoints )
		return CSqirrNode();

    irr::scene::SMeshBuffer *pMb = new irr::scene::SMeshBuffer();

    InsertCone( pMb, irr::core::vector3df(), fWidth, fConeHeight, lPoints, irr::video::SColor( 255, 255, 255, 255 ) );
    InsertCylinder( pMb, irr::core::vector3df( 0, -( fShaftHeight / 2.f ), 0 ), fWidth / 3.f, fShaftHeight, lPoints, irr::video::SColor( 255, 255, 255, 255 ) );

    pMb->recalculateBoundingBox();

    irr::scene::SMesh *pMesh = new irr::scene::SMesh();
    pMesh->addMeshBuffer( pMb );
    pMb->drop();

    pMesh->recalculateBoundingBox();

    irr::scene::IMeshSceneNode *pNode =
        m_pSmgr->addMeshSceneNode( pMesh );
    pMesh->drop();

    for ( oex::oexUINT i = 0; i < pNode->getMaterialCount(); i++ )
    {   pNode->getMaterial( i ).NormalizeNormals = true;
        pNode->getMaterial( i ).Shininess = 0;
    } // end for

    return pNode;
}


int CSqIrrlicht::OnEvent( const irr::SEvent& rEvent )
{_STT();
	switch( rEvent.EventType )
	{
		case irr::EET_KEY_INPUT_EVENT :
		{
			// If we have a queue
			if ( m_pCallbackQueue )
			{
				// Mouse callback
				m_pCallbackQueue->execute( oexNULL, oexT( "" ), m_sCallbackFunction,
										   oexT( "99" ),
										   oexMks( rEvent.KeyInput.Key ).Ptr(),
										   oexMks( rEvent.KeyInput.Char ).Ptr(),
										   oexMks( ( rEvent.KeyInput.PressedDown ? 1 : 0 )
												 | ( rEvent.KeyInput.Shift ? 2 : 0 )
												 | ( rEvent.KeyInput.Control ? 4 : 0 )
												 ).Ptr() );

			} // end if

		} break;


		case irr::EET_MOUSE_INPUT_EVENT :
		{
			// If we have a queue
			if ( m_pCallbackQueue )
			{
				// Mouse callback
				m_pCallbackQueue->execute( oexNULL, oexT( "" ), m_sCallbackFunction,
										   oexMks( rEvent.MouseInput.Event ).Ptr(),
										   oexMks( rEvent.MouseInput.X ).Ptr(),
										   oexMks( rEvent.MouseInput.Y ).Ptr(),
										   oexMks( rEvent.MouseInput.Wheel ).Ptr() );

			} // end if

/*
			switch( rEvent.MouseInput.Event )
			{
				case irr::EMIE_MOUSE_MOVED:
					oexEcho( "EMIE_MOUSE_MOVED" );
					break;

				case irr::EMIE_LMOUSE_PRESSED_DOWN:
					oexEcho( "EMIE_LMOUSE_PRESSED_DOWN" );
					break;

				case irr::EMIE_LMOUSE_LEFT_UP:
					oexEcho( "EMIE_LMOUSE_LEFT_UP" );
					break;

				case irr::EMIE_MMOUSE_PRESSED_DOWN:
					oexEcho( "EMIE_MMOUSE_PRESSED_DOWN" );
					break;

				case irr::EMIE_MMOUSE_LEFT_UP:
					oexEcho( "EMIE_MMOUSE_LEFT_UP" );
					break;

				case irr::EMIE_RMOUSE_PRESSED_DOWN:
					oexEcho( "EMIE_RMOUSE_PRESSED_DOWN" );
					break;

				case irr::EMIE_RMOUSE_LEFT_UP:
					oexEcho( "EMIE_RMOUSE_LEFT_UP" );
					break;

				case irr::EMIE_MOUSE_WHEEL:
					oexEcho( "EMIE_MOUSE_WHEEL" );
					break;

				default:
					break;

			} // end switch
*/
		} break;

		case irr::EET_GUI_EVENT :
		{
			// Check for close button
			if ( irr::gui::EGET_BUTTON_CLICKED == rEvent.GUIEvent.EventType )
				if ( rEvent.GUIEvent.Caller->getID() == 2 )
					m_bQuit = 1;
		} break;

		default :
			break;

	} // end switch

	return 0;
}

int CSqIrrlicht::getLineIntersect( CSqirrLine &line, CSqirrVector3d &pt, CSqirrVector3d &axis, CSqirrVector3d *intersect )
{_STT();
	// Get line
	irr::core::line3df l(  line.start().x(), line.start().y(), line.start().z(),
							line.end().x(), line.end().y(), line.end().z() );

	// Pick an axis we don't need as the normal for the plane
	irr::core::vector3df normal;
	if ( !axis.x() ) normal.X = 1;
	else if ( !axis.y() ) normal.Y = 1;
	else if ( !axis.z() ) normal.Z = 1;
	else normal = l.getVector();

	// Get mouse line / plane intersection
	if ( !irr::core::plane3df( pt.Obj(), normal ).
			getIntersectionWithLine( l.getMiddle(), l.getVector(), intersect->Obj() ) )
		return 0;

	// Revert the axis we don't want to change
	if ( !axis.x() ) intersect->x() = pt.x();
	if ( !axis.y() ) intersect->y() = pt.y();
	if ( !axis.z() ) intersect->z() = pt.z();

	return 1;
}

int CSqIrrlicht::getIntersect( CSqirrVector2d &ptScreen, CSqirrVector3d &pt, CSqirrVector3d &axis, CSqirrVector3d *intersect )
{_STT();
	// Must have object
	if ( !m_pCamera || !m_pSmgr )
		return 0;

	// Is mouse in the window?
	if ( 0 > ptScreen.Obj().X || 0 > ptScreen.Obj().Y )
		return 0;

	m_pCamera->OnRegisterSceneNode();

	// Get mouse line
	irr::core::line3df ml =
		m_pSmgr->getSceneCollisionManager()->
					getRayFromScreenCoordinates( irr::core::position2di( (unsigned int)ptScreen.Obj().X, (unsigned int)ptScreen.Obj().Y ),
												 m_pCamera );

	// Pick an axis we don't need as the normal for the plane
	irr::core::vector3df normal;
	if ( !axis.x() )
		normal.X = 1;
	else if ( !axis.y() )
		normal.Y = 1;
	else if ( !axis.z() )
		normal.Z = 1;
	else normal =
		ml.getVector();

	// Get mouse line / plane intersection
	if ( !irr::core::plane3df( pt.Obj(), normal ).
					getIntersectionWithLine( ml.getMiddle(), ml.getVector(), intersect->Obj() ) )
		return 0;

	// Revert the axis we don't want to change
	if ( !axis.x() ) intersect->x() = pt.x();
	if ( !axis.y() ) intersect->y() = pt.y();
	if ( !axis.z() ) intersect->z() = pt.z();

	return 1;
}

int CSqIrrlicht::getMouseIntersect( CSqirrVector2d &ptScreen, CSqirrVector3d &pt, CSqirrVector3d &axis, CSqirrVector3d *intersect )
{_STT();
	// Must have object
	if ( !m_pCamera || !m_pSmgr )
		return 0;

	// Is mouse in the window?
	if ( 0 > ptScreen.x() || 0 > ptScreen.y() )
		return 0;

	m_pCamera->OnRegisterSceneNode();

	// Get mouse line
	irr::core::line3df ml =
		m_pSmgr->getSceneCollisionManager()->
					getRayFromScreenCoordinates( irr::core::position2di( (int)ptScreen.x(), (int)ptScreen.y() ),
												 m_pCamera );

	// Picky thing, line should intersect plane
	if ( ml.start.X == ml.end.X )
		ml.end.X += 1;
	if ( ml.start.Y == ml.end.Y )
		ml.end.Y += 1;
	if ( ml.start.Z == ml.end.Z )
		ml.end.Z += 1;

	// Pick an axis we don't need as the normal for the plane
	irr::core::vector3df normal;
	if ( !axis.x() ) normal.X = 1;
	else if ( !axis.y() ) normal.Y = 1;
	else if ( !axis.z() ) normal.Z = 1;
	else normal = ml.getVector();

	// Get mouse line / plane intersection
	if ( !irr::core::plane3df( pt.Obj(), normal ).
			getIntersectionWithLimitedLine( ml.start, ml.end, intersect->Obj() ) )
		return 0;

	// Revert the axis we don't want to change
	if ( !axis.x() ) intersect->x() = pt.x();
	if ( !axis.y() ) intersect->y() = pt.y();
	if ( !axis.z() ) intersect->z() = pt.z();

	return 1;
}

int CSqIrrlicht::getPickPoint( CSqirrVector2d &ptScreen, CSqirrVector3d &pt, CSqirrVector3d *intersect )
{_STT();
	// Must have object
	if ( !m_pCamera || !m_pSmgr )
		return 0;

	// Is mouse in the window?
	if ( 0 > ptScreen.x() || 0 > ptScreen.y() )
		return 0;

	m_pCamera->OnRegisterSceneNode();

	// Get mouse line
	irr::core::line3df ml =
		m_pSmgr->getSceneCollisionManager()->
					getRayFromScreenCoordinates( irr::core::position2di( (int)ptScreen.x(), (int)ptScreen.y() ),
												 m_pCamera );

	// New position
	*intersect = ml.start + ml.getVector().normalize() *
					(float)m_pCamera->getPosition().getDistanceFrom( pt.Obj() );

	return 1;
}

CSqirrVector2d CSqIrrlicht::getNodeScreenPos( CSqirrNode &rNode )
{_STT();
	// Must have object
	if ( !m_pCamera || !m_pSmgr )
		return CSqirrVector2d( -1, -1 );

	irr::core::vector3df pos3d( rNode.GetPosition().Obj() );

	m_pCamera->OnRegisterSceneNode();

	irr::core::position2di pos2d =
		m_pSmgr->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition( pos3d, m_pCamera );

	return CSqirrVector2d( (float)pos2d.X, (float)pos2d.Y );
}

CSqirrVector2d CSqIrrlicht::getScreenPos( CSqirrVector3d &v )
{_STT();
	// Must have object
	if ( !m_pCamera || !m_pSmgr )
		return CSqirrVector2d( -1, -1 );

	m_pCamera->OnRegisterSceneNode();

	irr::core::position2di pos2d =
		m_pSmgr->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition( v.Obj(), m_pCamera );

	return CSqirrVector2d( (float)pos2d.X, (float)pos2d.Y );
}

float CSqIrrlicht::getNodeDist( CSqirrNode &v )
{_STT();

	// Must have object
	if ( !m_pCamera || !m_pSmgr )
		return 0;

	m_pCamera->OnRegisterSceneNode();

	return ( v.GetAbsoluteCenter().Obj() - m_pCamera->getPosition() ).getLength();
}

int CSqIrrlicht::screenToWorld( CSqirrVector2d &ptScreen, CSqirrVector3d *ptWorld, float fDist )
{_STT();
	// Must have object
	if ( !m_pCamera || !m_pSmgr )
		return 0;

	// Is mouse in the window?
	if ( 0 > ptScreen.Obj().X || 0 > ptScreen.Obj().Y )
		return 0;

	m_pCamera->OnRegisterSceneNode();

	// Get mouse line
	irr::core::line3df ml =
		m_pSmgr->getSceneCollisionManager()->
					getRayFromScreenCoordinates( irr::core::position2di( (unsigned int)ptScreen.Obj().X, (unsigned int)ptScreen.Obj().Y ),
												 m_pCamera );

	// Calculate position
	*ptWorld = ml.start + ( ml.getVector().normalize() * fDist );

	return 1;
}

int CSqIrrlicht::screenToWorldBox( CSqirrRect2d &rcScreen, CSqirrBoundingBox3d *bbWorld, float fDist )
{_STT();
	// Must have object
	if ( !m_pCamera || !m_pSmgr )
		return 0;

	// Is mouse in the window?
	if ( 0 > rcScreen.ltObj().X || 0 > rcScreen.ltObj().Y
		 || 0 > rcScreen.rbObj().X || 0 > rcScreen.rbObj().Y )
		return 0;

	m_pCamera->OnRegisterSceneNode();

	// Left / Top
	irr::core::line3df lt =
		m_pSmgr->getSceneCollisionManager()->
					getRayFromScreenCoordinates( irr::core::position2di( (unsigned int)rcScreen.ltObj().X, (unsigned int)rcScreen.ltObj().Y ),
												 m_pCamera );
	bbWorld->Obj().MinEdge = lt.start + ( lt.getVector().normalize() * fDist );

	// Right / Bottom
	irr::core::line3df rb =
		m_pSmgr->getSceneCollisionManager()->
					getRayFromScreenCoordinates( irr::core::position2di( (unsigned int)rcScreen.rbObj().X, (unsigned int)rcScreen.rbObj().Y ),
												 m_pCamera );
	bbWorld->Obj().MaxEdge = rb.start + ( rb.getVector().normalize() * fDist );


	// Ensure correct ordering
	if ( bbWorld->Obj().MinEdge.X > bbWorld->Obj().MaxEdge.X )
		oex::cmn::Swap( bbWorld->Obj().MinEdge.X, bbWorld->Obj().MaxEdge.X );
	if ( bbWorld->Obj().MinEdge.Y > bbWorld->Obj().MaxEdge.Y )
		oex::cmn::Swap( bbWorld->Obj().MinEdge.Y, bbWorld->Obj().MaxEdge.Y );
	if ( bbWorld->Obj().MinEdge.Z > bbWorld->Obj().MaxEdge.Z )
		oex::cmn::Swap( bbWorld->Obj().MinEdge.Z, bbWorld->Obj().MaxEdge.Z );

	return 1;
}


int CSqIrrlicht::screenToPlane( CSqirrVector2d &ptScreen, CSqirrVector3d *ptWorld, float fDist )
{_STT();
	// Must have object
	if ( !m_pCamera || !m_pSmgr )
		return 0;

	// Is mouse in the window?
	if ( 0 > ptScreen.Obj().X || 0 > ptScreen.Obj().Y )
		return 0;

	// Update the camera view
	m_pCamera->OnRegisterSceneNode();

	// Get mouse line
	irr::core::line3df ml =
		m_pSmgr->getSceneCollisionManager()->
					getRayFromScreenCoordinates( irr::core::position2di( (unsigned int)ptScreen.Obj().X, (unsigned int)ptScreen.Obj().Y ),
												 m_pCamera );

	// Get the camera target line
	irr::core::line3df cl( m_pCamera->getPosition(),
						   m_pCamera->getTarget() );

	// Calculate the orthogonal plane position
	irr::core::vector3df pos = cl.start + ( cl.getVector().normalize() * fDist );

	// Calculate camera line plane intersection
	if ( !irr::core::plane3df( pos, cl.getVector().normalize() ).
					getIntersectionWithLine( ml.getMiddle(), ml.getVector(), ptWorld->Obj() ) )
		return 0;

	return 1;
}

int CSqIrrlicht::FillVolume( CSqirrNode &rNode, CSqirrBoundingBox3d &rBb )
{_STT();
	if ( !rNode.IsValid() )
		return 0;

	rNode.Obj().setScale( irr::core::vector3df( 1, 1, 1 ) );
	irr::core::vector3df pos = rNode.Obj().getPosition();
	irr::core::aabbox3df bb = rNode.Obj().getTransformedBoundingBox();

	// Calculate existing node widths
	float xn = bb.MaxEdge.X - bb.MinEdge.X;
	float yn = bb.MaxEdge.Y - bb.MinEdge.Y;
	float zn = bb.MaxEdge.Z - bb.MinEdge.Z;

	// Calculate desired widths
	float xd = rBb.Obj().MaxEdge.X - rBb.Obj().MinEdge.X;
	float yd = rBb.Obj().MaxEdge.Y - rBb.Obj().MinEdge.Y;
	float zd = rBb.Obj().MaxEdge.Z - rBb.Obj().MinEdge.Z;

	// Calculate conversion scales
	float x_scale = xn ? xd / xn : 1;
	float y_scale = yn ? yd / yn : 1;
	float z_scale = zn ? zd / zn : 1;

	// Set new sale
	rNode.Obj().setScale( irr::core::vector3df( x_scale, y_scale, z_scale ) );

	// Set new position
	rNode.Obj().setPosition( irr::core::vector3df( 	rBb.Obj().MinEdge.X + ( x_scale * ( pos.X - bb.MinEdge.X ) ),
													rBb.Obj().MinEdge.Y + ( y_scale * ( pos.Y - bb.MinEdge.Y ) ),
													rBb.Obj().MinEdge.Z + ( z_scale * ( pos.Z - bb.MinEdge.Z ) ) ) );

	return 1;
}

