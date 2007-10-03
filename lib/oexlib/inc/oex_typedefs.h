/*------------------------------------------------------------------
// oex_typedefs.h
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

#pragma once

// Native processor integer size in bits
#define oexSIZEOFINT 4

typedef long						oexRESULT;
		
#if defined( _MSC_VER )
typedef __int16                     oexINT16;
typedef unsigned __int16            oexUINT16;
typedef __int32						oexINT32;
typedef unsigned __int32			oexUINT32;
typedef __int64						oexINT64;
typedef unsigned __int64			oexUINT64;
#else
typedef short                       oexINT16;
typedef unsigned short              oexUINT16;
typedef int				 	        oexINT32;
typedef unsigned int    			oexUINT32;
typedef long long			   		oexINT64;
typedef unsigned long long			oexUINT64;
#endif
		
		
typedef int							oexINT;
typedef unsigned int				oexUINT;
typedef long						oexLONG;
typedef unsigned long				oexULONG;
typedef short						oexSHORT;
typedef unsigned short				oexUSHORT;
typedef float						oexFLOAT;
typedef double						oexDOUBLE;
typedef bool						oexBOOL;
typedef char						oexCHAR;
typedef unsigned char				oexUCHAR;

// String types
typedef char                        oexCHAR8;
typedef oexCHAR8 *                  oexSTR8;
typedef const oexCHAR8 *            oexCSTR8;
typedef short                       oexCHAR16;
typedef oexCHAR16 *                 oexSTR16;
typedef const oexCHAR16 *           oexCSTR16;
typedef long                        oexCHAR32;
typedef oexCHAR32 *                 oexSTR32;
typedef const oexCHAR32 *           oexCSTR32;
typedef wchar_t                     oexCHARW;
typedef oexCHARW *                  oexSTRW;
typedef const oexCHARW *            oexCSTRW;

#ifdef UNICODE
	typedef wchar_t					oexTCHAR;
#else
	typedef char					oexTCHAR;
#endif

typedef oexTCHAR*					oexSTR;
typedef oexCONST oexTCHAR*			oexCSTR;

typedef void*						oexPVOID;
typedef void oexCONST *				oexCPVOID;

typedef oexUINT						oexTYPEOF_PTR;


struct oexGUID 
{
    union
    {
        struct
        {
            oexUINT32   Data1;
            oexUINT16   Data2;
            oexUINT16   Data3;
            oexUCHAR    Data4[ 8 ];
        };

        oexUCHAR        ucBuf[ 16 ];
    };

};


/// Finger saver ;)
/// This class can be used efficiently as a return type
class oexAUTOGUID : public oexGUID
{
public:

    /// Default constructor
    oexAUTOGUID() {}
    
    /// Copy constructor
    oexAUTOGUID( oexCONST oexGUID &r ) 
    {   Copy( r ); }
    
    /// Copy constructor
    oexAUTOGUID( oexCONST oexGUID *p ) 
    {   Copy( p ); }

    /// Copy constructor
    oexAUTOGUID( oexCONST oexAUTOGUID &r ) 
    {   Copy( (oexGUID&)r ); }
    
    /// Copy constructor
    oexAUTOGUID( oexCONST oexAUTOGUID *p ) 
    {   Copy( (oexGUID*)p ); }

    /// Copy operator
    oexAUTOGUID& operator = ( oexCONST oexGUID &r ) 
    {   return Copy( r ); }

    /// Copy operator
    oexAUTOGUID& operator = ( oexCONST oexGUID *p ) 
    {   return Copy( p ); }
    
    /// Copy operator
    oexAUTOGUID& operator = ( oexCONST oexAUTOGUID &r ) 
    {   Copy( (oexGUID&)r ); }

    /// Copy operator
    oexAUTOGUID& operator = ( oexCONST oexAUTOGUID *p ) 
    {   Copy( (oexGUID*)p ); }

    /// Copies guid
    oexAUTOGUID& Copy( oexCONST oexGUID &r )
    {   *(oexINT64*)this = *(oexINT64*)&r;
        ( (oexINT64*)this )[ 1 ] = ( (oexINT64*)&r )[ 1 ];
        return *this;
    }

    /// Copies guid
    oexAUTOGUID& Copy( oexCONST oexGUID *p )
    {   *(oexINT64*)this = *(oexINT64*)p;
        ( (oexINT64*)this )[ 1 ] = ( (oexINT64*)p )[ 1 ];
        return *this;
    }

    /// GUID reference
    operator oexGUID& ()
    {   return *this; }

    /// GUID pointer
    operator oexGUID* ()
    {   return this; }

};
