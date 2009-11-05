// stdafx.cpp

#include "stdafx.h"

#ifndef assert
extern "C"  void assert(int expression)
{
}
#endif

extern "C"
{
//const uint64_t ff_pw_20 = 0x0014001400140014ULL;
//const uint64_t ff_pw_20 __attribute__ ((aligned (8))) = 0x0014001400140014ULL;
//const unsigned long long ff_pw_20 = 0x0014001400140014ULL;
//DECLARE_ALIGNED_8 (const uint64_t, ff_pw_20 ) = 0x0014001400140014ULL;
}

// Export Functions
SQBIND_REGISTER_CLASS_BEGIN( CFfDecoder, CFfDecoder )

	SQBIND_MEMBER_FUNCTION( CFfDecoder, Destroy )
	SQBIND_MEMBER_FUNCTION( CFfDecoder, Create )
	SQBIND_MEMBER_FUNCTION( CFfDecoder, FindStreamInfo )
	SQBIND_MEMBER_FUNCTION( CFfDecoder, Decode )
	SQBIND_MEMBER_FUNCTION( CFfDecoder, DecodeImage )
	SQBIND_MEMBER_FUNCTION( CFfDecoder, getWidth )
	SQBIND_MEMBER_FUNCTION( CFfDecoder, getHeight )

	SQBIND_STATIC_FUNCTION( CFfDecoder, LookupCodecId )
	SQBIND_STATIC_FUNCTION( CFfDecoder, LookupCodecName )	
	
	// Codec types
	SQBIND_GLOBALCONST( CODEC_ID_H264 )
	SQBIND_GLOBALCONST( CODEC_ID_MPEG4 )
	SQBIND_GLOBALCONST( CODEC_ID_MPEG1VIDEO )
	SQBIND_GLOBALCONST( CODEC_ID_MPEG2VIDEO )
	SQBIND_GLOBALCONST( CODEC_ID_H263 )
	SQBIND_GLOBALCONST( CODEC_ID_MJPEG )
	SQBIND_GLOBALCONST( CODEC_ID_MSMPEG4V1 )
	SQBIND_GLOBALCONST( CODEC_ID_MSMPEG4V2 )
	SQBIND_GLOBALCONST( CODEC_ID_MSMPEG4V3 )
	SQBIND_GLOBALCONST( CODEC_ID_WMV1 )
	SQBIND_GLOBALCONST( CODEC_ID_WMV2 )
	SQBIND_GLOBALCONST( CODEC_ID_WMV3 )
	SQBIND_GLOBALCONST( CODEC_ID_H263P )
	SQBIND_GLOBALCONST( CODEC_ID_HUFFYUV )
	SQBIND_GLOBALCONST( CODEC_ID_FFVHUFF )
	SQBIND_GLOBALCONST( CODEC_ID_FFV1 )
	SQBIND_GLOBALCONST( CODEC_ID_VP3 )
	SQBIND_GLOBALCONST( CODEC_ID_LJPEG )
	SQBIND_GLOBALCONST( CODEC_ID_VORBIS )
	SQBIND_GLOBALCONST( CODEC_ID_MJPEGB )
	SQBIND_GLOBALCONST( CODEC_ID_CSCD )
	SQBIND_GLOBALCONST( CODEC_ID_H263I )
	SQBIND_GLOBALCONST( CODEC_ID_SVQ1 )
	SQBIND_GLOBALCONST( CODEC_ID_DVVIDEO )
	SQBIND_GLOBALCONST( CODEC_ID_WMAV1 )
	SQBIND_GLOBALCONST( CODEC_ID_WMAV2 )
	SQBIND_GLOBALCONST( CODEC_ID_SVQ3 )
	SQBIND_GLOBALCONST( CODEC_ID_FLV1 )
	SQBIND_GLOBALCONST( CODEC_ID_ASV1 )
	SQBIND_GLOBALCONST( CODEC_ID_ASV2 )
	SQBIND_GLOBALCONST( CODEC_ID_CYUV )
	SQBIND_GLOBALCONST( CODEC_ID_INDEO3 )
	SQBIND_GLOBALCONST( CODEC_ID_MSVIDEO1 )
	SQBIND_GLOBALCONST( CODEC_ID_CINEPAK )
	SQBIND_GLOBALCONST( CODEC_ID_VCR1 )
	SQBIND_GLOBALCONST( CODEC_ID_THEORA )

SQBIND_REGISTER_CLASS_END()
DECLARE_INSTANCE_TYPE( CFfDecoder );

SQBIND_REGISTER_CLASS_BEGIN( CFfEncoder, CFfEncoder )

	SQBIND_MEMBER_FUNCTION( CFfEncoder, Destroy )
	SQBIND_MEMBER_FUNCTION( CFfEncoder, Create )
	SQBIND_MEMBER_FUNCTION( CFfEncoder, IsLoaded )
	SQBIND_MEMBER_FUNCTION( CFfEncoder, Encode )
	SQBIND_MEMBER_FUNCTION( CFfEncoder, EncodeImage )

	// Codec types
	SQBIND_GLOBALCONST( CODEC_ID_H264 )
	SQBIND_GLOBALCONST( CODEC_ID_MPEG4 )
	SQBIND_GLOBALCONST( CODEC_ID_MPEG1VIDEO )
	SQBIND_GLOBALCONST( CODEC_ID_MPEG2VIDEO )
	SQBIND_GLOBALCONST( CODEC_ID_H263 )
	SQBIND_GLOBALCONST( CODEC_ID_MJPEG )
	SQBIND_GLOBALCONST( CODEC_ID_MSMPEG4V1 )
	SQBIND_GLOBALCONST( CODEC_ID_MSMPEG4V2 )
	SQBIND_GLOBALCONST( CODEC_ID_MSMPEG4V3 )
	SQBIND_GLOBALCONST( CODEC_ID_WMV1 )
	SQBIND_GLOBALCONST( CODEC_ID_WMV2 )
	SQBIND_GLOBALCONST( CODEC_ID_WMV3 )
	SQBIND_GLOBALCONST( CODEC_ID_H263P )
	SQBIND_GLOBALCONST( CODEC_ID_HUFFYUV )
	SQBIND_GLOBALCONST( CODEC_ID_FFVHUFF )
	SQBIND_GLOBALCONST( CODEC_ID_FFV1 )
	SQBIND_GLOBALCONST( CODEC_ID_VP3 )
	SQBIND_GLOBALCONST( CODEC_ID_LJPEG )
	SQBIND_GLOBALCONST( CODEC_ID_VORBIS )
	SQBIND_GLOBALCONST( CODEC_ID_MJPEGB )
	SQBIND_GLOBALCONST( CODEC_ID_CSCD )
	SQBIND_GLOBALCONST( CODEC_ID_H263I )
	SQBIND_GLOBALCONST( CODEC_ID_SVQ1 )
	SQBIND_GLOBALCONST( CODEC_ID_DVVIDEO )
	SQBIND_GLOBALCONST( CODEC_ID_WMAV1 )
	SQBIND_GLOBALCONST( CODEC_ID_WMAV2 )
	SQBIND_GLOBALCONST( CODEC_ID_SVQ3 )
	SQBIND_GLOBALCONST( CODEC_ID_FLV1 )
	SQBIND_GLOBALCONST( CODEC_ID_ASV1 )
	SQBIND_GLOBALCONST( CODEC_ID_ASV2 )
	SQBIND_GLOBALCONST( CODEC_ID_CYUV )
	SQBIND_GLOBALCONST( CODEC_ID_INDEO3 )
	SQBIND_GLOBALCONST( CODEC_ID_MSVIDEO1 )
	SQBIND_GLOBALCONST( CODEC_ID_CINEPAK )
	SQBIND_GLOBALCONST( CODEC_ID_VCR1 )
	SQBIND_GLOBALCONST( CODEC_ID_THEORA )

SQBIND_REGISTER_CLASS_END()
DECLARE_INSTANCE_TYPE( CFfEncoder );

SQBIND_REGISTER_CLASS_BEGIN( CFfConvert, CFfConvert )

	SQBIND_STATIC_FUNCTION( CFfConvert, CalcImageSize )
	SQBIND_STATIC_FUNCTION( CFfConvert, ConvertColorBB )
	SQBIND_STATIC_FUNCTION( CFfConvert, ConvertColorIB )
	SQBIND_STATIC_FUNCTION( CFfConvert, ConvertColorBI )

	// Format tyeps
	SQBIND_GLOBALCONST( PIX_FMT_RGB24 )
	SQBIND_GLOBALCONST( PIX_FMT_BGR24 )
	SQBIND_GLOBALCONST( PIX_FMT_RGB32 )
	SQBIND_GLOBALCONST( PIX_FMT_BGR32 )

	SQBIND_GLOBALCONST( PIX_FMT_MONOWHITE )
	SQBIND_GLOBALCONST( PIX_FMT_MONOBLACK )
	SQBIND_GLOBALCONST( PIX_FMT_GRAY8 )
	SQBIND_GLOBALCONST( PIX_FMT_GRAY16LE )
	SQBIND_GLOBALCONST( PIX_FMT_GRAY16BE )

	SQBIND_GLOBALCONST( PIX_FMT_YUV410P )
	SQBIND_GLOBALCONST( PIX_FMT_YUV411P )
	SQBIND_GLOBALCONST( PIX_FMT_YUV420P )
	SQBIND_GLOBALCONST( PIX_FMT_YUV422P )
	SQBIND_GLOBALCONST( PIX_FMT_YUV440P )
	SQBIND_GLOBALCONST( PIX_FMT_YUV444P )

	SQBIND_GLOBALCONST( PIX_FMT_YUYV422 )

	SQBIND_GLOBALCONST( PIX_FMT_YUVJ420P )
	SQBIND_GLOBALCONST( PIX_FMT_YUVJ422P )
	SQBIND_GLOBALCONST( PIX_FMT_YUVJ444P )
	SQBIND_GLOBALCONST( PIX_FMT_YUVJ440P )
	SQBIND_GLOBALCONST( PIX_FMT_YUVA420P )

	// Processing types
	SQBIND_GLOBALCONST( SWS_FAST_BILINEAR )
	SQBIND_GLOBALCONST( SWS_BILINEAR )
	SQBIND_GLOBALCONST( SWS_BICUBIC )
	SQBIND_GLOBALCONST( SWS_X )
	SQBIND_GLOBALCONST( SWS_POINT )
	SQBIND_GLOBALCONST( SWS_AREA )
	SQBIND_GLOBALCONST( SWS_BICUBLIN )
	SQBIND_GLOBALCONST( SWS_GAUSS )
	SQBIND_GLOBALCONST( SWS_SINC )
	SQBIND_GLOBALCONST( SWS_LANCZOS )
	SQBIND_GLOBALCONST( SWS_SPLINE )
	SQBIND_GLOBALCONST( SWS_SRC_V_CHR_DROP_MASK )
	SQBIND_GLOBALCONST( SWS_SRC_V_CHR_DROP_SHIFT )
	SQBIND_GLOBALCONST( SWS_PARAM_DEFAULT )
	SQBIND_GLOBALCONST( SWS_PRINT_INFO )
	SQBIND_GLOBALCONST( SWS_FULL_CHR_H_INT )
	SQBIND_GLOBALCONST( SWS_FULL_CHR_H_INP )
	SQBIND_GLOBALCONST( SWS_DIRECT_BGR )
	SQBIND_GLOBALCONST( SWS_ACCURATE_RND )
	SQBIND_GLOBALCONST( SWS_CPU_CAPS_MMX )
	SQBIND_GLOBALCONST( SWS_CPU_CAPS_MMX2 )
	SQBIND_GLOBALCONST( SWS_CPU_CAPS_3DNOW )
	SQBIND_GLOBALCONST( SWS_CPU_CAPS_ALTIVEC )
	SQBIND_GLOBALCONST( SWS_CPU_CAPS_BFIN )
	SQBIND_GLOBALCONST( SWS_MAX_REDUCE_CUTOFF )
	SQBIND_GLOBALCONST( SWS_CS_ITU709 )
	SQBIND_GLOBALCONST( SWS_CS_FCC )
	SQBIND_GLOBALCONST( SWS_CS_ITU601 )
	SQBIND_GLOBALCONST( SWS_CS_ITU624 )
	SQBIND_GLOBALCONST( SWS_CS_SMPTE170M )
	SQBIND_GLOBALCONST( SWS_CS_SMPTE240M )
	SQBIND_GLOBALCONST( SWS_CS_DEFAULT )

SQBIND_REGISTER_CLASS_END()
DECLARE_INSTANCE_TYPE( CFfConvert );

#define FFF_KEY_FRAME	PKT_FLAG_KEY

// Export Functions
SQBIND_REGISTER_CLASS_BEGIN( CFfContainer, CFfContainer )

	SQBIND_MEMBER_FUNCTION( CFfContainer, Open )
	SQBIND_MEMBER_FUNCTION( CFfContainer, isOpen )
	SQBIND_MEMBER_FUNCTION( CFfContainer, ReadFrame )
	SQBIND_MEMBER_FUNCTION( CFfContainer, DecodeFrame )

	SQBIND_MEMBER_FUNCTION( CFfContainer, Create )
	SQBIND_MEMBER_FUNCTION( CFfContainer, InitWrite )
	SQBIND_MEMBER_FUNCTION( CFfContainer, AddVideoStream )
	SQBIND_MEMBER_FUNCTION( CFfContainer, WriteFrame )

	SQBIND_MEMBER_FUNCTION( CFfContainer, Destroy )
	SQBIND_MEMBER_FUNCTION( CFfContainer, getWidth )
	SQBIND_MEMBER_FUNCTION( CFfContainer, getHeight )
	SQBIND_MEMBER_FUNCTION( CFfContainer, getVideoFormat )
	SQBIND_MEMBER_FUNCTION( CFfContainer, getVideoCodecId )
	SQBIND_MEMBER_FUNCTION( CFfContainer, getVideoStream )
	SQBIND_MEMBER_FUNCTION( CFfContainer, getAudioStream )
	SQBIND_MEMBER_FUNCTION( CFfContainer, getFrameCount )

	SQBIND_GLOBALCONST( FFF_KEY_FRAME )

SQBIND_REGISTER_CLASS_END()
DECLARE_INSTANCE_TYPE( CFfContainer );

// Export Functions
SQBIND_REGISTER_CLASS_BEGIN( CFfTranscode, CFfTranscode )

	SQBIND_MEMBER_FUNCTION( CFfTranscode, Init )
	SQBIND_MEMBER_FUNCTION( CFfTranscode, Destroy )
	SQBIND_MEMBER_FUNCTION( CFfTranscode, Transcode )
	SQBIND_MEMBER_FUNCTION( CFfTranscode, GetRaw )
	SQBIND_MEMBER_FUNCTION( CFfTranscode, GetImage )
	SQBIND_MEMBER_FUNCTION( CFfTranscode, getDecoderCodecId )
	SQBIND_MEMBER_FUNCTION( CFfTranscode, getEncoderCodecId )
	SQBIND_MEMBER_FUNCTION( CFfTranscode, getWidth )
	SQBIND_MEMBER_FUNCTION( CFfTranscode, getHeight )

SQBIND_REGISTER_CLASS_END()
DECLARE_INSTANCE_TYPE( CFfTranscode );

// Export classes
static void SQBIND_Export_ffmpeg( sqbind::VM x_vm )
{
	if ( !oexCHECK_PTR( x_vm ) )
		return;

#if defined( OEX_DEBUG )
	av_log_set_level( AV_LOG_INFO );
//	av_log_set_level( AV_LOG_DEBUG );
#else
//	av_log_set_level( AV_LOG_WARNING );
	av_log_set_level( AV_LOG_ERROR );
//	av_log_set_level( AV_LOG_FATAL );
#endif

	// Register codecs
	av_register_all();

    SQBIND_EXPORT( x_vm, CFfDecoder );
    SQBIND_EXPORT( x_vm, CFfEncoder );
    SQBIND_EXPORT( x_vm, CFfConvert );
    SQBIND_EXPORT( x_vm, CFfContainer );
    SQBIND_EXPORT( x_vm, CFfTranscode );
}

#if defined( SQBIND_STATIC )
	#include "ff_decoder.cpp"
	#include "ff_encoder.cpp"
	#include "ff_convert.cpp"
	#include "ff_container.cpp"
	#include "ff_transcode.cpp"
#else

	static void SQBIND_Export( sqbind::VM x_vm )
	{	SQBIND_Export_ffmpeg( x_vm ); }

	// Include squirrel module exported symbols
	#include <sqmod_extern.hpp>

#endif
