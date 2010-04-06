// ff_container.cpp

#include "stdafx.h"

CFfContainer::CFfContainer()
{_STT();
	m_pFormatContext = oexNULL;
	m_pCodecContext = oexNULL;
	m_pFrame = oexNULL;
	m_nVideoStream = -1;
	m_nAudioStream = -1;
	m_nWrite = 0;
	m_nRead = 0;
	m_bKeyRxd = 0;
	m_nFrames = 0;
	oexZero( m_pkt );
}

void CFfContainer::Destroy()
{_STT();

	CloseStream();

	if ( m_pkt.data )
		av_free_packet( &m_pkt );

	if ( m_pCodecContext )
		avcodec_close( m_pCodecContext );

	m_pCodecContext = oexNULL;
	m_pFrame = oexNULL;
	m_nVideoStream = -1;
	m_nAudioStream = -1;
	m_nWrite = 0;
	m_nRead = 0;
	m_bKeyRxd = 0;
	m_nFrames = 0;
	m_buf.Free();
	oexZero( m_pkt );
}

int CFfContainer::CloseStream()
{_STT();
	if ( m_nRead )
	{
		if ( m_pFormatContext )
			av_close_input_file( m_pFormatContext );

	} // end if

	else if ( 1 < m_nWrite )
	{
		if ( m_pFormatContext )
		{
			// Write the rest of the file
			av_write_trailer( m_pFormatContext );

			// Free streams
			for ( unsigned int i = 0; i < m_pFormatContext->nb_streams; i++ )
				if (  m_pFormatContext->streams[ i ] )
					av_freep( m_pFormatContext->streams[ i ] );
/*
			// Close file / socket resources
			if ( m_pFormatContext->oformat
				 && !( m_pFormatContext->oformat->flags & AVFMT_NOFILE )
				 && m_pFormatContext->pb )
				url_fclose( m_pFormatContext->pb );
*/
			// Free the stream
			av_free( m_pFormatContext );

		} // end if

	} // end else if

	m_pFormatContext = oexNULL;

}


int CFfContainer::Open( const sqbind::stdString &sUrl, sqbind::CSqMulti *m )
{_STT();
	// Lose old container
	Destroy();

	// Did we get a valid string?
	if ( !sUrl.length() )
		return 0;

//	AVFormatParameters	fp;
//	oexZero( fp );
//	fp.initial_pause = 1;

	// Attempt to open it
	int res = av_open_input_file( &m_pFormatContext, oexStrToMbPtr( sUrl.c_str() ),
								  oexNULL, 0, oexNULL );
	if ( res )
	{	oexERROR( res, oexT( "av_open_input_file() failed" ) );
		Destroy();
		return 0;
	} // end if

	res = av_find_stream_info( m_pFormatContext );
	if ( 0 > res )
	{	oexERROR( res, oexT( "av_find_stream_info() failed" ) );
		Destroy();
		return 0;
	} // end if

	if ( m )
	{
		(*m)[ oexT( "filename" ) ].set( oexMbToStrPtr( m_pFormatContext->filename ) );
		(*m)[ oexT( "timestamp" ) ].set( oexMks( m_pFormatContext->timestamp ).Ptr() );
		(*m)[ oexT( "title" ) ].set( oexMbToStrPtr( m_pFormatContext->title ) );
		(*m)[ oexT( "author" ) ].set( oexMbToStrPtr( m_pFormatContext->author ) );
		(*m)[ oexT( "copyright" ) ].set( oexMbToStrPtr( m_pFormatContext->copyright ) );
		(*m)[ oexT( "comment" ) ].set( oexMbToStrPtr( m_pFormatContext->comment ) );
		(*m)[ oexT( "album" ) ].set( oexMbToStrPtr( m_pFormatContext->album ) );
		(*m)[ oexT( "year" ) ].set( oexMks( m_pFormatContext->year ).Ptr() );
		(*m)[ oexT( "track" ) ].set( oexMks( m_pFormatContext->track ).Ptr() );
		(*m)[ oexT( "genre" ) ].set( oexMbToStrPtr( m_pFormatContext->genre ) );
		(*m)[ oexT( "ctx_flags" ) ].set( oexMks( m_pFormatContext->ctx_flags ).Ptr() );
		(*m)[ oexT( "start_time" ) ].set( oexMks( m_pFormatContext->start_time ).Ptr() );
		(*m)[ oexT( "duration" ) ].set( oexMks( m_pFormatContext->duration ).Ptr() );
		(*m)[ oexT( "file_size" ) ].set( oexMks( m_pFormatContext->file_size ).Ptr() );
		(*m)[ oexT( "bit_rate" ) ].set( oexMks( m_pFormatContext->bit_rate ).Ptr() );
		(*m)[ oexT( "index_built" ) ].set( oexMks( m_pFormatContext->index_built ).Ptr() );
		(*m)[ oexT( "mux_rate" ) ].set( oexMks( m_pFormatContext->mux_rate ).Ptr() );
		(*m)[ oexT( "packet_size" ) ].set( oexMks( m_pFormatContext->packet_size ).Ptr() );
		(*m)[ oexT( "preload" ) ].set( oexMks( m_pFormatContext->preload ).Ptr() );
		(*m)[ oexT( "max_delay" ) ].set( oexMks( m_pFormatContext->max_delay ).Ptr() );
		(*m)[ oexT( "loop_output" ) ].set( oexMks( m_pFormatContext->loop_output ).Ptr() );
		(*m)[ oexT( "loop_input" ) ].set( oexMks( m_pFormatContext->loop_input ).Ptr() );
		(*m)[ oexT( "probesize" ) ].set( oexMks( m_pFormatContext->probesize ).Ptr() );
		(*m)[ oexT( "max_analyze_duration" ) ].set( oexMks( m_pFormatContext->max_analyze_duration ).Ptr() );
		(*m)[ oexT( "keylen" ) ].set( oexMks( m_pFormatContext->keylen ).Ptr() );

	} // end if

	// Find audio and video stream indices
	for ( unsigned int i = 0; i < m_pFormatContext->nb_streams; i++ )
		if ( CODEC_TYPE_VIDEO == m_pFormatContext->streams[ i ]->codec->codec_type )
			m_nVideoStream = i;
		else if ( CODEC_TYPE_AUDIO == m_pFormatContext->streams[ i ]->codec->codec_type )
			m_nAudioStream = i;

	// Did we find a stream?
	if ( 0 > m_nVideoStream && 0 > m_nAudioStream )
	{	oexERROR( 0, oexT( "file contains no video or audio streams" ) );
		Destroy();
		return 0;
	} // end if

	// Video stream
	if ( 0 <= m_nVideoStream
	     && m_pFormatContext->streams[ m_nVideoStream ]
		 &&  m_pFormatContext->streams[ m_nVideoStream ]->codec )
	{
		m_pCodecContext = m_pFormatContext->streams[ m_nVideoStream ]->codec;
		AVCodec *pCodec = avcodec_find_decoder( m_pCodecContext->codec_id );
		if ( !pCodec )
			m_pCodecContext = oexNULL;

		m_pCodecContext->workaround_bugs |= FF_BUG_AUTODETECT;
		m_pCodecContext->error_concealment = FF_EC_GUESS_MVS;
		m_pCodecContext->error_recognition = FF_ER_CAREFUL;
		if( pCodec->capabilities & CODEC_CAP_TRUNCATED )
			m_pCodecContext->flags |= CODEC_FLAG_TRUNCATED;

		if ( 0 > avcodec_open( m_pCodecContext, pCodec ) )
			m_pCodecContext = oexNULL;

	} // end if

	// Reading
	m_nRead = 1;

//	dump_format( m_pFormatContext, 0, 0, 0 );

	return 1;
}

int CFfContainer::ReadFrame( sqbind::CSqBinary *dat, sqbind::CSqMulti *m )
{_STT();
	if ( !m_pFormatContext )
		return -1;

	if ( !m_nRead )
		return -1;

	int res = av_read_frame( m_pFormatContext, &m_pkt );
	if ( res )
	{	oexSHOW( m_pkt.size );
		oexEcho( "no frame" );
		return -1;
	} // end if

	if ( m )
	{
		(*m)[ oexT( "flags" ) ].set( oexMks( m_pkt.flags ).Ptr() );
		(*m)[ oexT( "size" ) ].set( oexMks( m_pkt.size ).Ptr() );
		(*m)[ oexT( "stream_index" ) ].set( oexMks( m_pkt.stream_index ).Ptr() );
		(*m)[ oexT( "pos" ) ].set( oexMks( m_pkt.pos ).Ptr() );
		(*m)[ oexT( "dts" ) ].set( oexMks( m_pkt.dts ).Ptr() );
		(*m)[ oexT( "pts" ) ].set( oexMks( m_pkt.pts ).Ptr() );
		(*m)[ oexT( "duration" ) ].set( oexMks( m_pkt.duration ).Ptr() );
//		(*m)[ oexT( "convergence_duration" ) ].set( oexMks( pkt.convergence_duration ).Ptr() );

	} // end if

	if ( dat )
		dat->setBuffer( (sqbind::CSqBinary::t_byte*)m_pkt.data, m_pkt.size, 0 );

	return m_pkt.stream_index;
}

int CFfContainer::DecodeFrame( int stream, int fmt, sqbind::CSqBinary *dat, sqbind::CSqMulti *m )
{_STT();
	// Read a frame from the packet
	int res = -1;

//	oexPrintf( oexMks( oexT( "\r" ),  m_nFrames, oexT( " : " ) ).Ptr() );

	do
	{
		// Read frames from input stream
		res = ReadFrame( oexNULL, m );
		if ( 0 > res )
			return res;

	} while ( res != stream );

	// Waiting key frame?
	if ( !m_bKeyRxd )
	{	if ( 0 == ( m_pkt.flags & PKT_FLAG_KEY ) )
			return -1;
		m_bKeyRxd = 1;
	} // end if

	oexPrintf( oexMks( m_pkt.flags, " : ", m_pkt.size, " : ", m_buf.getUsed(), "    " ).Ptr() );
	oex::os::CSys::Flush_stdout();

	if ( 0 != ( m_pkt.flags & PKT_FLAG_KEY ) )
		oexEcho( " key frame" );
	else
		oexEcho( " i frame" );

//	if ( m_pkt.size > 150000 || m_pkt.size < 140000 )
//	{
//		oexEcho( "bad packet" );
//		return -1;
//	} // end if

/*
	m_buf.setUsed( 0 );

	// Ensure buffer size
	if ( ( m_buf.Size() - m_buf.getUsed() ) < ( m_pkt.size + FF_INPUT_BUFFER_PADDING_SIZE ) )
		m_buf.Allocate( 2 * ( m_buf.Size() + m_pkt.size + FF_INPUT_BUFFER_PADDING_SIZE ) );

	// Add new data
	m_buf.AppendBuffer( (sqbind::CSqBinary::t_byte*)m_pkt.data, m_pkt.size );
	m_pkt.data = (uint8_t*)m_buf._Ptr();
	m_pkt.size = m_buf.getUsed();

	// Zero padding
	int nPadding = m_buf.Size() - m_buf.getUsed();
	if ( 0 < nPadding )
	{
		// Don't zero more than twice the padding size
		if ( nPadding > ( FF_INPUT_BUFFER_PADDING_SIZE * 2 ) )
			nPadding = FF_INPUT_BUFFER_PADDING_SIZE * 2;

		// Set end to zero to ensure no overreading on damaged blocks
		oexZeroMemory( &m_buf._Ptr()[ m_buf.getUsed() ], nPadding );

		oexSHOW( nPadding );

	} // end if
*/
	// Video only atm
	if ( !dat || stream != m_nVideoStream || !m_pCodecContext )
		return -1;

	if ( !m_pFrame )
		m_pFrame = avcodec_alloc_frame();
	if ( !m_pFrame )
		return -1;

	int gpp = 0, used = 0;

#if defined( FFSQ_VIDEO2 )

	used = avcodec_decode_video2( m_pCodecContext, m_pFrame, &gpp, &m_pkt );
	if ( 0 >= used )
	{	oexSHOW( used );
		return -1;
	} // end if

#else

	used = avcodec_decode_video( m_pCodecContext, m_pFrame, &gpp, m_pkt.data, m_pkt.size );
//	used = avcodec_decode_video( m_pCodecContext, m_pFrame, &gpp, (const uint8_t*)m_buf.Ptr(), m_buf.getUsed() );
	if ( 0 >= used )
	{	oexSHOW( used );
		return -1;
	} // end if

#endif

/*
	// Left over data?
	if ( used < m_pkt.size )
	{
		if ( m_buf.getUsed() )
			m_buf.LShift( used );
		else
			m_buf.AppendBuffer( (sqbind::CSqBinary::t_byte*)&m_pkt.data[ used ], m_pkt.size - used );

	} // end if
	else
		m_buf.setUsed( 0 );
*/
	if ( !gpp )
		return -1;

	// Is it already the right format?
	if ( fmt == (int)m_pCodecContext->pix_fmt )
	{	int nSize = CFfConvert::CalcImageSize( fmt, m_pCodecContext->width, m_pCodecContext->height );
		dat->setBuffer( (sqbind::CSqBinary::t_byte*)m_pFrame->data[ 0 ], nSize, 0 );
		m_nFrames++;
		return m_pkt.stream_index;
	} // end if

	// Do colorspace conversion
	if ( !CFfConvert::ConvertColorFB( m_pFrame, m_pCodecContext->pix_fmt,
									  m_pCodecContext->width, m_pCodecContext->height,
									  fmt, dat, SWS_FAST_BILINEAR ) )
		return -1;

	// Frame
	m_nFrames++;

	return m_pkt.stream_index;
}

int CFfContainer::DecodeFrameBin( sqbind::CSqBinary *in, int fmt, sqbind::CSqBinary *out, sqbind::CSqMulti *m )
{_STT();

	if ( m_pkt.data )
		av_free_packet( &m_pkt );
	oexZero( m_pkt );

	m_pkt.data = (uint8_t*)in->Ptr();
	m_pkt.size = in->getUsed();

	m_buf.setUsed( 0 );

	// Ensure buffer size
	if ( ( m_buf.Size() - m_buf.getUsed() ) < ( m_pkt.size + FF_INPUT_BUFFER_PADDING_SIZE ) )
		m_buf.Allocate( 2 * ( m_buf.Size() + m_pkt.size + FF_INPUT_BUFFER_PADDING_SIZE ) );

	// Add new data
	m_buf.AppendBuffer( (sqbind::CSqBinary::t_byte*)m_pkt.data, m_pkt.size );
	m_pkt.data = (uint8_t*)m_buf._Ptr();
	m_pkt.size = m_buf.getUsed();

	// Zero padding
	int nPadding = m_buf.Size() - m_buf.getUsed();
	if ( 0 < nPadding )
	{
		// Don't zero more than twice the padding size
		if ( nPadding > ( FF_INPUT_BUFFER_PADDING_SIZE * 2 ) )
			nPadding = FF_INPUT_BUFFER_PADDING_SIZE * 2;

		// Set end to zero to ensure no overreading on damaged blocks
		oexZeroMemory( &m_buf._Ptr()[ m_buf.getUsed() ], nPadding );

		oexSHOW( nPadding );

	} // end if


	if ( !m_pFrame )
		m_pFrame = avcodec_alloc_frame();
	if ( !m_pFrame )
		return -1;

	int gpp = 0, used = 0;

#if defined( FFSQ_VIDEO2 )

	used = avcodec_decode_video2( m_pCodecContext, m_pFrame, &gpp, &m_pkt );
	if ( 0 >= used )
	{	oexSHOW( used );
		return -1;
	} // end if

#else

	used = avcodec_decode_video( m_pCodecContext, m_pFrame, &gpp, m_pkt.data, m_pkt.size );
//	used = avcodec_decode_video( m_pCodecContext, m_pFrame, &gpp, (const uint8_t*)m_buf.Ptr(), m_buf.getUsed() );
	if ( 0 >= used )
	{	oexSHOW( used );
		return -1;
	} // end if

#endif

/*
	// Left over data?
	if ( used < m_pkt.size )
	{
		if ( m_buf.getUsed() )
			m_buf.LShift( used );
		else
			m_buf.AppendBuffer( (sqbind::CSqBinary::t_byte*)&m_pkt.data[ used ], m_pkt.size - used );

	} // end if
	else
		m_buf.setUsed( 0 );
*/
	if ( !gpp )
		return -1;

	// Is it already the right format?
	if ( fmt == (int)m_pCodecContext->pix_fmt )
	{	int nSize = CFfConvert::CalcImageSize( fmt, m_pCodecContext->width, m_pCodecContext->height );
		out->setBuffer( (sqbind::CSqBinary::t_byte*)m_pFrame->data[ 0 ], nSize, 0 );
		m_nFrames++;
		return m_pkt.stream_index;
	} // end if

	// Do colorspace conversion
	if ( !CFfConvert::ConvertColorFB( m_pFrame, m_pCodecContext->pix_fmt,
									  m_pCodecContext->width, m_pCodecContext->height,
									  fmt, out, SWS_FAST_BILINEAR ) )
		return -1;

	// Frame
	m_nFrames++;

	return 1;
}


int CFfContainer::Create( const sqbind::stdString &sUrl, const sqbind::stdString &sType, sqbind::CSqMulti *m )
{_STT();
	Destroy();

	if ( !sUrl.length() )
		return 0;

	// What type of file?
	AVOutputFormat *pOut = 0;

	if ( sType.length() )
		pOut = guess_format( oexStrToMbPtr( sType.c_str() ), 0, 0 );
	else
		pOut = guess_format( 0, oexStrToMbPtr( sUrl.c_str() ), 0 );

	if ( !pOut )
	{	oexERROR( 0, oexT( "guess_format() failed" ) );
		Destroy();
		return 0;
	} // end if

	// Allocate format context
	m_pFormatContext = avformat_alloc_context();
	if ( !m_pFormatContext )
	{	oexERROR( 0, oexT( "avformat_alloc_context() failed" ) );
		Destroy();
		return 0;
	} // end if

	// Save format and file name
	m_pFormatContext->oformat = pOut;
	oexStrCpySz( m_pFormatContext->filename, oexStrToMbPtr( sUrl.c_str() ) );

	if ( m )
	{
		if ( m->isset( oexT( "flags" ) ) )
			m_pFormatContext->flags = (*m)[ oexT( "flags" ) ].toint();

		if ( m->isset( oexT( "max_analyze_duration" ) ) )
			m_pFormatContext->max_analyze_duration = (*m)[ oexT( "max_analyze_duration" ) ].toint();

		if ( m->isset( oexT( "probesize" ) ) )
			m_pFormatContext->probesize = (*m)[ oexT( "probesize" ) ].toint();

		if ( m->isset( oexT( "bit_rate" ) ) )
			m_pFormatContext->bit_rate = (*m)[ oexT( "bit_rate" ) ].toint();

		if ( m->isset( oexT( "loop_input" ) ) )
			m_pFormatContext->loop_input = (*m)[ oexT( "loop_input" ) ].toint();

		if ( m->isset( oexT( "loop_output" ) ) )
			m_pFormatContext->loop_output = (*m)[ oexT( "loop_output" ) ].toint();

	} // end if

	m_nWrite = 1;

/*
	int res = 0;
	if ( 0 > ( res = av_set_parameters( m_pFormatContext, oexNULL ) ) )
	{	oexERROR( res, oexT( "av_set_parameters() failed" ) );
		m_nWrite = 0;
		Destroy();
		return 0;
	} // end if
*/
	return 1;
}

int CFfContainer::InitWrite()
{_STT();
	if ( !m_pFormatContext || !m_pFormatContext->oformat )
		return 0;

	if ( !m_nWrite )
		return 0;

	int res = 0;
	if ( 0 > ( res = av_set_parameters( m_pFormatContext, oexNULL ) ) )
	{	oexERROR( res, oexT( "av_set_parameters() failed" ) );
		Destroy();
		return 0;
	} // end if

	if ( !( m_pFormatContext->oformat->flags & AVFMT_NOFILE ) )
		if ( 0 > ( res = url_fopen( &m_pFormatContext->pb, m_pFormatContext->filename, URL_WRONLY ) ) )
		{	oexERROR( res, oexT( "url_fopen() failed" ) );
			Destroy();
			return 0;
		} // end if

    if ( 0 > ( res = av_write_header( m_pFormatContext ) ) )
	{	oexERROR( res, oexT( "av_write_header() failed" ) );
		Destroy();
		return 0;
	} // end if

	// Writing
	m_nWrite = 2;

	return 1;
}

int CFfContainer::AddVideoStream( int codec_id, int width, int height, int fps )
{_STT();
	if ( !m_pFormatContext )
		return -1;

	if ( !m_nWrite )
		return -1;

	if ( 0 <= m_nVideoStream )
		return m_nVideoStream;

	AVStream *pst = av_new_stream( m_pFormatContext, 0 );
	if ( !pst )
		return -1;

	m_nVideoStream = 0;

    AVCodecContext *pcc = pst->codec;
	if ( !pcc )
		return -1;

	// Fill in codec info
	pcc->codec_id = (CodecID)codec_id;
	pcc->codec_type = CODEC_TYPE_VIDEO;
	pcc->bit_rate = 2000000;
	pcc->width = width;
	pcc->height = height;
    pcc->time_base.num = 1;
    pcc->time_base.den = fps;
	pcc->gop_size = 12;

//	oex::CStr sFName( m_pFormatContext->oformat->name );
//	if (  sFName == oexT( "3gp" ) || sFName == oexT( "mov" ) || sFName == oexT( "mp4" ) )
//		pcc->flags |= CODEC_FLAG_GLOBAL_HEADER;

	return m_nVideoStream;
}

int CFfContainer::WriteFrame( sqbind::CSqBinary *dat, sqbind::CSqMulti *m )
{_STT();
	if ( !m_pFormatContext )
		return 0;

	if ( !m_nWrite )
		return 0;

	if ( 0 > m_nVideoStream )
		return 0;

	AVStream *pStream = m_pFormatContext->streams[ m_nVideoStream ];
	if ( !pStream )
		return 0;

	AVCodecContext *pCodec = pStream->codec;
	if ( !pCodec )
		return 0;

	AVPacket pkt;
	oexZero( pkt );
	av_init_packet( &pkt );

	if ( pCodec->coded_frame )
		pkt.pts = pCodec->coded_frame->pts;
//	pkt.flags |= PKT_FLAG_KEY;

	if ( m )
	{
		if ( m->isset( oexT( "flags" ) ) )
			pkt.flags = (*m)[ oexT( "flags" ) ].toint();

/*
		if ( m->isset( oexT( "dts" ) ) )
			pkt.dts = (*m)[ oexT( "dts" ) ].toint();

		if ( m->isset( oexT( "pts" ) ) )
			pkt.dts = (*m)[ oexT( "pts" ) ].toint();

		if ( m->isset( oexT( "duration" ) ) )
			pkt.flags = (*m)[ oexT( "duration" ) ].toint();
*/
//		pkt.pts = 0; //AV_NOPTS_VALUE;
//		pkt.dts = 0; //AV_NOPTS_VALUE;
//		pkt.duration = 0;
//		pkt.pos = -1;
//		pkt.convergence_duration = 0; //AV_NOPTS_VALUE;


	} // end if

	pkt.stream_index = pStream->index;
	pkt.data = (uint8_t*)dat->_Ptr();
	pkt.size = dat->getUsed();

	if ( av_write_frame( m_pFormatContext, &pkt ) )
		return 0;

	return 1;
}

