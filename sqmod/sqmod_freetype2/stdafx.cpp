// stdafx.cpp

#include "stdafx.h"

// CFtFace
SQBIND_REGISTER_CLASS_BEGIN( CFtFace, CFtFace )

	SQBIND_MEMBER_FUNCTION( CFtFace, Destroy )
	SQBIND_MEMBER_FUNCTION( CFtFace, getLastError )
	SQBIND_MEMBER_FUNCTION( CFtFace, LoadChar )
	SQBIND_MEMBER_FUNCTION( CFtFace, DrawImg )	
	SQBIND_MEMBER_FUNCTION( CFtFace, DrawBin )	
	SQBIND_MEMBER_FUNCTION( CFtFace, setAngle )
	SQBIND_MEMBER_FUNCTION( CFtFace, getAngle )
	SQBIND_MEMBER_FUNCTION( CFtFace, setCharSize )
	SQBIND_MEMBER_FUNCTION( CFtFace, setPen )
	SQBIND_MEMBER_FUNCTION( CFtFace, setPenX )
	SQBIND_MEMBER_FUNCTION( CFtFace, getPenX )
	SQBIND_MEMBER_FUNCTION( CFtFace, setPenY )
	SQBIND_MEMBER_FUNCTION( CFtFace, getPenY )
	SQBIND_MEMBER_FUNCTION( CFtFace, CalcSize )
	SQBIND_MEMBER_FUNCTION( CFtFace, Str2Ascii )
	SQBIND_MEMBER_FUNCTION( CFtFace, setColor )
//	SQBIND_MEMBER_FUNCTION( CFtFace,  )
//	SQBIND_MEMBER_FUNCTION( CFtFace,  )

SQBIND_REGISTER_CLASS_END()
DECLARE_INSTANCE_TYPE( CFtFace );


void CFtFace::Register( sqbind::VM vm )
{_STT();
	SQBIND_EXPORT( vm, CFtFace );
}

// CFtLibrary
SQBIND_REGISTER_CLASS_BEGIN( CFtLibrary, CFtLibrary )

	SQBIND_MEMBER_FUNCTION( CFtLibrary, Destroy )
	SQBIND_MEMBER_FUNCTION( CFtLibrary, getLastError )
	SQBIND_MEMBER_FUNCTION( CFtLibrary, LoadFile )
	SQBIND_MEMBER_FUNCTION( CFtLibrary, LoadFont )
//	SQBIND_MEMBER_FUNCTION( CFtLibrary,  )
//	SQBIND_MEMBER_FUNCTION( CFtLibrary,  )

SQBIND_REGISTER_CLASS_END()
DECLARE_INSTANCE_TYPE( CFtLibrary );


void CFtLibrary::Register( sqbind::VM vm )
{_STT();
	SQBIND_EXPORT( vm, CFtLibrary );
}

// Export classes
static void SQBIND_Export_freetype2( sqbind::VM x_vm )
{_STT();
	if ( !oexCHECK_PTR( x_vm ) )
		return;

	CFtLibrary::Register( x_vm );
	CFtFace::Register( x_vm );
}

#if defined( SQBIND_STATIC )
	#include "sq_face.cpp"
	#include "sq_library.cpp"
#else

	static void SQBIND_Export( sqbind::VM x_vm )
	{_STT(); SQBIND_Export_freetype2( x_vm ); }

	// Include squirrel module exported symbols
	#include <sqmod_extern.hpp>

#endif
