/*------------------------------------------------------------------
// oex_resources.h
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

// Resources
#if defined( OEX_RESOURCES )

	// Resource macros
#	define oexGetResource		OEX_NAMESPACE::COexResourceHelper().GetResource
#	define oexGetResourceCount	OEX_NAMESPACE::COexResourceHelper().GetResourceCount
#	define oexGetResourceName	OEX_NAMESPACE::COexResourceHelper().GetResourceName
#	define oexGetResourceSize	OEX_NAMESPACE::COexResourceHelper().GetResourceSize
#	define oexGetResourcePtr	OEX_NAMESPACE::COexResourceHelper().GetResourcePtr

	class COexResourceHelper
	{
	public:

		COexResourceHelper()
		{
			m_p = oexNULL;
			m_l = 0;
		}

		COexResourceHelper( CStr sName )
		{
			if ( GetResource( sName, &m_p, &m_l ) )
				m_p = oexNULL, m_l = 0;
		}

		CStr8 GetResource( CStr sName )
		{
			if ( GetResource( sName, &m_p, &m_l ) )
				return CStr8();

			return oex::zip::CUncompress::Uncompress( (oexCHAR8*)m_p, m_l );
		}

		CStr8 GetResource( oexLONG i )
		{
			if ( GetResource( i, &m_p, &m_l ) )
				return CStr8();

			return oex::zip::CUncompress::Uncompress( (oexCHAR8*)m_p, m_l );
		}

		oexLONG GetResource( CStr sName, const void ** p, oexLONG *l )
		{
			// Search for the item
			CStr8 sName8 = oexStrToMb( sName );
			for ( int i = 0; _g_oexlib_resources[ i ].name; i++ )
				if ( sName8 == _g_oexlib_resources[ i ].name )
				{	*p = _g_oexlib_resources[ i ].data;
					*l = _g_oexlib_resources[ i ].size;
					return 0;
				} // end if

			return -1;
		}

		oexLONG GetResource( oexLONG i, const void ** p, oexLONG *l )
		{
			if ( 0 > i || GetResourceCount() <= i )
				return -1;

			// Save resource info
			*p = _g_oexlib_resources[ i ].data;
			*l = _g_oexlib_resources[ i ].size;

			return 0;
		}

		oexLONG GetResourceSize( oexLONG i )
		{
			if ( 0 > i || GetResourceCount() <= i )
				return -1;

			return _g_oexlib_resources[ i ].size;
		}

		oexCPVOID GetResourcePtr( oexLONG i )
		{
			if ( 0 > i || GetResourceCount() <= i )
				return 0;

			return _g_oexlib_resources[ i ].data;
		}

		CStr GetResourceName( oexLONG i )
		{
			if ( 0 > i || GetResourceCount() <= i )
				return CStr();

			return oexMbToStrPtr( _g_oexlib_resources[ i ].name );
		}

		oexLONG GetResourceCount()
		{
			oexLONG i = 0;
			while ( _g_oexlib_resources[ i ].name )
				i++;

			return i;
		}

	private:

		/// Pointer to resource
		const void 		*m_p;

		/// Length of resource
		long 			m_l;

	};

#else

#	define OEX_NO_RESOURCES		1

#endif
