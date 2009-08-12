/*------------------------------------------------------------------
// sq_capture.cpp
//
// Copyright (c) 1997
// Robert Umbehant
// winglib@wheresjames.com
// http://www.wheresjames.com
//
// Redistribution and use in source and binary forms, with or
// without modification, are permitted for commercial and
// non-commercial purposes, provided that the following
// conditions are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * The names of the developers or contributors may not be used to
//   endorse or promote products derived from this software without
//   specific prior written permission.
//
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
//   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
//   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
//   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
//   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
//   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//----------------------------------------------------------------*/

#include "../stdafx.h"

using namespace sqbind;


SQBIND_REGISTER_CLASS_BEGIN( sqbind::CSqCapture, CSqCapture )
	SQBIND_MEMBER_FUNCTION(  sqbind::CSqCapture, Init )
	SQBIND_MEMBER_FUNCTION(  sqbind::CSqCapture, Capture )
	SQBIND_MEMBER_FUNCTION(  sqbind::CSqCapture, GetSupportedFormats )
	SQBIND_MEMBER_FUNCTION(  sqbind::CSqCapture, GetFormatDescription )
SQBIND_REGISTER_CLASS_END()

void CSqCapture::Register( sqbind::VM vm )
{
	SQBIND_EXPORT( vm, CSqCapture );
}

int CSqCapture::Destroy()
{
	// Close the capture device
	m_cap.Destroy();

	return 1;
}

int CSqCapture::Init( int nDevice, int nChannel, int nWidth, int nHeight, int nFps, const stdString &sFormat, int nInit )
{
	oex::CStr sFmt = sFormat.c_str();

	if ( sFmt.Length() < 4 )
		sFmt = oexT( "JPEG" );

	// Open the capture device
	if ( !m_cap.Open( 0, nDevice, nChannel, nWidth, nHeight, *(const oex::oexUINT*)sFmt.Ptr(), nFps, 0 ) )
		return -1;

//	if ( !m_cap.Open( 0, nDevice, nChannel, nWidth, nHeight, oexFOURCC( 'Y', 'U', 'Y', 'V' ), nFps, 0 ) )
//	CStr().Fmt( oexT( "VIDIOC_S_FMT : Unsupported format: %x - suggested type: %x" ),
//												(unsigned int)m_uFormat

	if ( !nInit )
		return 1;

	if ( !m_cap.Init() )
		return -2;

	if ( !m_cap.StartCapture() )
		return -3;

	return 1;
}

stdString SaveImage( void *pBuf, long lSize, long lWidth, long lHeight )
{
	if ( !pBuf || !lSize )
		return stdString();

	oex::CImage img;
	if ( !img.Create( lWidth, lHeight ) )
		return stdString();

	memcpy( img.GetBits(), pBuf, lSize / 2 );

	unsigned char *pEnc = 0;
	int lEnc = 0;
	if ( !img.Encode( &pEnc, &lEnc, "jpg" ) || !pEnc || !lEnc )
		return stdString();

	stdString ret;
	ret.assign( (char*)pEnc, lEnc );

	return ret;
}

stdString CSqCapture::GetSupportedFormats()
{
	return m_cap.GetSupportedFormats().Ptr();
}

stdString CSqCapture::GetFormatDescription( const stdString &sFormat )
{
	// Convert to multi-byte
	oex::CStr8 fmt = oexStrToMb( sFormat.c_str() );
	if ( fmt.Length() < 4 )
		return oexT( "" );

	// Convert to integer
	return m_cap.GetFormatDescription( *(oex::oexUINT*)fmt.Ptr() ).Ptr();
}


stdString CSqCapture::Capture( const stdString &sEncode )
{
	if ( !m_cap.WaitForFrame( 3000 ) )
		return stdString();

	stdString ret;
	if ( m_cap.GetBuffer() && m_cap.GetBufferSize() )
		ret.assign( (char*)m_cap.GetBuffer(), m_cap.GetBufferSize() );

	// Release video buffer
	m_cap.ReleaseFrame();

	return ret;

/*
	oex::CDib img;
	if ( !oexVERIFY( img.Create( oexNULL, oexNULL, cCapture.GetWidth(), cCapture.GetHeight(), 24, 1 ) ) )
		return -7;




	// Convert to RGB
	img.Copy( cCapture.GetBuffer(), cCapture.GetBufferSize() );
//		img.CopySBGGR8( cCapture.GetBuffer() );
//		img.CopyGrey( cCapture.GetBuffer() );

	int nImageSize = oex::CImage::GetScanWidth( cCapture.GetWidth(), 24 )
						* oex::cmn::Abs( cCapture.GetHeight() );

//		oexPrintf( oexT( "Writing image to file : %s\n" ), oexStrToMb( sFile ).Ptr() );
//		oexPrintf( oexT( "w=%d, h=%d, Buffer Size = %d, Image Size = %d\n" ), cCapture.GetWidth(), cCapture.GetHeight(), cCapture.GetBufferSize(), nImageSize );

	if ( !oexVERIFY( oex::CDib::SaveDibFile( sFile.Ptr(), img.GetImageHeader(), img.GetBuffer(), nImageSize ) ) )
		return -8;

	if ( !oexVERIFY( cCapture.ReleaseFrame() ) )
		return -9;

*/
}
