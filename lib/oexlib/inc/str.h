/*------------------------------------------------------------------
// str.h
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


/// Generic string class
/**
    
  \code

  \endcode

*/
template < typename T > class TStr
{
public:

    /// Default constructor
    TStr()
    {
        m_lLength = 0;
        m_lOffset = 0;

#if defined( _DEBUG ) || defined( OEX_ENABLE_RELEASE_MODE_MEM_CHECK )
        m_pFile = oexNULL;
        m_uLine = 0;
#endif
    }

	TStr( TStr &str )
	{	m_lLength = 0;
        m_lOffset = 0;
#if defined( _DEBUG ) || defined( OEX_ENABLE_RELEASE_MODE_MEM_CHECK )
        m_pFile = oexNULL;
        m_uLine = 0;
#endif
		Set( str );
//		if ( str.Length() ) Set( str.Ptr() );
	}

	TStr( oexCONST T *pStr )
	{	m_lLength = 0;
        m_lOffset = 0;
#if defined( _DEBUG ) || defined( OEX_ENABLE_RELEASE_MODE_MEM_CHECK )
        m_pFile = oexNULL;
        m_uLine = 0;
#endif
		Set( pStr );
	}

	TStr( oexCONST T *pStr, oexUINT uSize )
	{	m_lLength = 0;
        m_lOffset = 0;
#if defined( _DEBUG ) || defined( OEX_ENABLE_RELEASE_MODE_MEM_CHECK )
        m_pFile = oexNULL;
        m_uLine = 0;
#endif
		Set( pStr, uSize );
	}

	TStr( oexCONST T *pStr, oexINT nStart, oexUINT uLen )
	{	m_lLength = 0;
        m_lOffset = 0;
#if defined( _DEBUG ) || defined( OEX_ENABLE_RELEASE_MODE_MEM_CHECK )
        m_pFile = oexNULL;
        m_uLine = 0;
#endif
		Sub( pStr, nStart, uLen );
	}

	TStr( oexCONST T *pStr, oexUINT uSize, oexINT nStart, oexUINT uLen )
	{	m_lLength = 0;
        m_lOffset = 0;
#if defined( _DEBUG ) || defined( OEX_ENABLE_RELEASE_MODE_MEM_CHECK )
        m_pFile = oexNULL;
        m_uLine = 0;
#endif
		Sub( pStr, uSize, nStart, uLen );
	}

	TStr( oexCONST oexINT nVal )
	{	m_lLength = 0;
        m_lOffset = 0;
#if defined( _DEBUG ) || defined( OEX_ENABLE_RELEASE_MODE_MEM_CHECK )
        m_pFile = oexNULL;
        m_uLine = 0;
#endif
		SetNum( "%li", (oexLONG)nVal ); 
	}

	TStr( oexCONST oexUINT uVal )
	{	m_lLength = 0;
        m_lOffset = 0;
#if defined( _DEBUG ) || defined( OEX_ENABLE_RELEASE_MODE_MEM_CHECK )
        m_pFile = oexNULL;
        m_uLine = 0;
#endif
		SetNum( "%lu", (oexULONG)uVal ); 
	}

	TStr( oexCONST oexDOUBLE dStr )
	{	m_lLength = 0;
        m_lOffset = 0;
#if defined( _DEBUG ) || defined( OEX_ENABLE_RELEASE_MODE_MEM_CHECK )
        m_pFile = oexNULL;
        m_uLine = 0;
#endif
		SetNumTrim( "%f", oexNULL, "0", (oexDOUBLE)dStr ); 
	}

	TStr( oexCONST T tVal )
	{	m_lLength = 0;
        m_lOffset = 0;
#if defined( _DEBUG ) || defined( OEX_ENABLE_RELEASE_MODE_MEM_CHECK )
        m_pFile = oexNULL;
        m_uLine = 0;
#endif
		Set( tVal ); 
	}

    /// Destructor
    ~TStr()
    {   Destroy();
    }

    /// Releases the string
    void Destroy()
    {
        // Zero length
        m_lLength = 0;

        // Reset the offset
        m_lOffset = 0;

        // Lose the string memory
        m_mem.OexDelete();
    }

public:

    /// Shares memory of another object
    TStr& Share( oexCONST TStr &x_str )
    {      
        // Share the memory
        m_lLength = x_str.m_lLength;
        m_lOffset = x_str.m_lOffset;
        m_mem.Share( x_str.m_mem );
        return *this;
    }

    /// Creates our own copy of a string if it's shared
	oexBOOL Unshare()
	{
		// Create our own buffer if shared
		if ( 1 == m_mem.GetRefCount() )
			return oexTRUE;

		oexUINT uOldSize = m_mem.Size();

		if ( uOldSize )
		{
            TMem< T > mem;
            if ( !oexVERIFY_PTR( mem.OexNew( uOldSize ).Ptr() ) )
                return oexFALSE;

            // Copy string data
			os::CSys::MemCpy( mem.Ptr(), m_mem.c_Ptr(), uOldSize * sizeof( T ) );
            
            // Use the new memory
			m_mem.Assume( mem );

		} // end if

		else 
            Destroy();

		return oexTRUE;

	}

	/// Allocates at least the number of bytes specified
	T* Allocate( oexUINT x_uSize )
	{	
//		oexTRACE( "TStr::Allocate( %lu )\n", uSize );

		// Break any existing share
		Unshare();

        T* pMem;
        oexBOOL bExisting = oexNULL != m_mem.Ptr();

        // Do we already have the memory
        if ( m_mem.Size() == ( x_uSize + 1 ) )
            pMem = m_mem.Ptr();

        else
        {
            pMem = m_mem._Log( m_uLine, m_pFile ).Resize( x_uSize + 1 ).Ptr();
		    if ( !oexVERIFY_PTR( pMem ) )
			    return oexNULL;

        } // end if

        // Set empty string if new buffer
        if ( !bExisting )
            pMem[ 0 ] = 0;

		// Null terminate our new buffer
		pMem[ x_uSize ] = 0;

		// User can't possibly copy any data
		if ( !x_uSize ) m_lLength = 0;

		// We'll scan for the length of the buffer later
		else m_lLength = -1;

		return pMem;
    }

	/// Returns a const pointer to the internal string buffer
	oexCONST T* Ptr()
	{	
		// Ensure string
		if ( !m_mem.Size() )
			OexAllocate( 0 );

		return m_mem.Ptr( m_lOffset );
	}

	/// Returns a const pointer to the internal string buffer
    /**
        !!! Possibly strange behavior, this function evaluates
            the length of the string if lOffset is not zero, so
            the open buffer you get form Allocate() will be
            canceled if you call this function with a non-zero
            offset.  I decided not to have it evaluate the string
            length when lOffset is zero to avoid needing an external 
            pointer when using open buffers and Allocate().
    */
	oexCONST T* Ptr( oexLONG x_lOffset )
    {	
		// Ensure string
		if ( !m_mem.Size() )
			OexAllocate( 0 );

        if ( x_lOffset )
        {
		    // Check for offset beyond the end of the buffer
		    oexASSERT( Length() >= x_lOffset );

    		// Don't point beyond the end of the buffer
	    	if ( x_lOffset > Length() ) 
                x_lOffset = m_lLength;

        } // end if

		return m_mem.Ptr( x_lOffset + m_lOffset );
	}

	/// Returns a writable pointer (use with care)
	T* _Ptr()
	{	return (T*)Ptr(); }

	/// Returns a writable pointer (use with care)
	T* _Ptr( oexLONG x_lOffset )
	{	return (T*)Ptr( x_lOffset ); }

	/// Returns the specified character
	T& operator []( oexLONG x_lOffset )
	{	return *_Ptr( x_lOffset ); }

	/// Returns the first character in the buffer
	oexCONST T& operator *() 
    { return *Ptr(); }

	/// Calculates the length of the string by finding the NULL terminator
	oexLONG CalculateLength()
	{
		m_lLength = 0;
		oexLONG lMax = m_mem.Size();

		// Do we have a buffer?
		if ( 0 >= lMax )
			return m_lLength;

		// Get a pointer to the whole string buffer
		T* pStr = m_mem.Ptr();

		// NULL terminate the end of memory so we don't over-run the buffer
		pStr[ lMax - 1 ] = 0;

        // Get the length
        m_lLength = str::Length( pStr, lMax );

		// Resize the memory block ( this adds over-run protection )
		m_mem.Resize( m_lLength + 1 );

		// Return the length of the buffer
		return m_lLength;
	}

	/// Returns the length of the string, calculates if needed.
	oexLONG Length() 
	{	if ( 0 > m_lLength ) 
			return CalculateLength();
		return m_lLength - m_lOffset; 
	}

    /// Manually sets the length
    /// !!! This allows NULL characters to be in the string.
    oexLONG SetLength( oexLONG x_lLength )
    {
        // Ensure we have that much data
        if ( !OexAllocate( m_lOffset + x_lLength ) )
            return 0;

        // Just accept the callers size
        m_lLength = x_lLength;

        return m_lLength;
    }

public:

	/// Assignment operator
	TStr& operator = ( oexCONST T* pStr )
	{	return Set( pStr ); }

	/// Assignment operator
	TStr& operator = ( oexCONST TStr &x_str )
	{	return Set( x_str ); }

    /// Assignment operator
	TStr& operator = ( oexCONST T chVal )
	{	return Set( chVal ); }

	/// Concatenation operator
	TStr& operator += ( oexCONST T* pStr )
	{	return Append( pStr ); }

	/// Concatenation operator
	TStr operator += ( TStr &str )
	{	return Append( str ); }


	TStr& operator = ( oexCONST oexINT nVal )
	{	return SetNum( "%li", (oexLONG)nVal ); }

	TStr& operator = ( oexCONST oexUINT uVal )
	{	return SetNum( "%lu", (oexULONG)uVal ); }

	TStr& operator = ( oexCONST oexDOUBLE dStr )
	{	return SetNumTrim( "%f", oexNULL, "0", (oexDOUBLE)dStr ); }

	TStr& operator += ( oexCONST oexINT nVal )
	{	return AppendNum( "%li", (oexLONG)nVal ); }

	TStr& operator += ( oexCONST oexUINT uVal )
	{	return AppendNum( "%lu", (oexULONG)uVal ); }

	TStr& operator += ( oexCONST oexDOUBLE dVal )
	{	return AppendNumTrim( "%f", oexNULL, "0", (oexDOUBLE)dVal ); }

	TStr& operator += ( oexCONST T chVal )
	{	return Append( &chVal, 1 ); }

	TStr& operator += ( oexCONST TStr &str )
	{	return Append( str.Ptr() ); }

	TStr& operator << ( oexCONST oexINT nVal )
	{	return AppendNum( "%li", (oexLONG)nVal ); }

	TStr& operator << ( oexCONST oexUINT uVal )
	{	return AppendNum( "%lu", (oexULONG)uVal ); }

	TStr& operator << ( oexCONST oexDOUBLE dVal )
	{	return AppendNumTrim( "%f", oexNULL, "0", (oexDOUBLE)dVal ); }

	TStr& operator << ( oexCSTR pStr )
	{	return Append( pStr ); }

	TStr& operator << ( TStr &str )
	{	return Append( str.Ptr(), str.Length() ); }

	TStr& operator << ( oexCONST T chVal )
	{	return Append( &chVal, 1 ); }

	TStr& Chr( oexUINT uCh )
	{	return Set( (T)uCh ); }

	TStr& ChrAppend( oexUINT uCh )
	{	return Append( &( (T)uCh ), 1 ); }


    /// Compare to const string
    oexBOOL operator == ( oexCONST T *x_pStr )
    {   return !Compare( x_pStr, zstr::Length( x_pStr ) ); }

    /// Compare to other object
    oexBOOL operator == ( oexCONST TStr &x_str )
    {   
        TStr &rStr = (TStr&)x_str;

        // See if we're sharing the same memory
        if ( rStr.m_mem.c_Ptr() == m_mem.c_Ptr() )
            return oexTRUE;

        // Lengths must match
        if ( Length() != rStr.Length() )
            return oexFALSE;

        // I suppose we'll have to actually compare the strings
        return !Compare( rStr.m_mem.c_Ptr(), rStr.Length() );
    }


public:

    /// Compare to const string
    oexINT Compare( oexCONST T *x_pPtr, oexUINT x_uLen )
    {   return str::Compare( Ptr(), Length(), x_pPtr, x_uLen ); }

    /// Extracts a sub string
	TStr& Sub( oexUINT x_uStart, oexUINT x_uLen = 0 )
	{
        // Current string length
		oexUINT uSize = Length();

		// Can't shorten the string if we don't own the memory
		if ( x_uLen && x_uLen < ( uSize - x_uStart ) && 1 != m_mem.GetRefCount() )
			return Set( SubStr( x_uStart, x_uLen ) );

        // Wiping out the string?
        if ( x_uStart >= uSize )
        {   Destroy(); return *this; }

        // Shift the start of the string
        m_lOffset += x_uStart;

        // Do we need to adjust the length?
        if ( x_uLen && x_uLen < ( uSize - x_uStart ) )
        {   OexAllocate( m_lOffset + x_uLen );
            m_lLength = m_lOffset + x_uLen;
        } // end if

		return *this;

	}

	TStr& Sub( TStr &str, oexINT nStart, oexUINT uLen )
	{	return Sub( str.Ptr(), str.Length(), nStart, uLen ); }

	TStr& Sub( oexCONST T* pStr, oexINT nStart, oexUINT uLen )
	{	return Sub( pStr, zstr::Length( pStr ), nStart, uLen ); }

	TStr& Sub( oexCONST T* pStr, oexUINT uSize, oexINT nStart, oexUINT uLen )
	{
		oexUINT uStart;
		if ( 0 <= nStart ) uStart = (oexUINT)nStart;
		else nStart = (oexUINT)( (oexINT)uSize - nStart );

		// Sanity checks
		if ( !oexVERIFY_PTR( pStr ) || !uSize || uStart >= uSize ) 
		{	OexAllocate( 0 ); return *this; }

		// Was a length given?
		if ( !uLen ) 
			uLen = uSize - uStart;

        // Allocate memory for sub string
		if ( !oexVERIFY( OexAllocate( uLen ) ) )
		{	OexAllocate( 0 ); return *this; }

		// Copy the sub string
		m_mem.MemCpy( &pStr[ uStart ], uLen );

		// Save the new string length
		m_lLength = uLen;

		return *this;
	}

	/// Returns a sub string
	TStr SubStr( oexINT nStart, oexUINT uLen )
	{	return TStr( Ptr(), Length(), nStart, uLen );    
    }

public:

	/// Moves part of the string from one place to another
	/// May destroy the string if there is overlap
	TStr& Move( oexUINT uFrom, oexUINT uTo, oexUINT uSize )
	{	Move( _Ptr(), Length(), uFrom, uTo, uSize ); return *this; }
	oexINT Move( T* pPtr, oexUINT uLen, oexUINT uFrom, oexUINT uTo, oexUINT uSize )
	{
		// Sanity check
		if ( uFrom >= uLen || uTo >= uLen )
			return 0;

		// Ensure no buffer over-running
		if ( ( uFrom + uSize ) > uLen )
			uSize = uLen - uFrom;
		if ( ( uTo + uSize ) > uLen )
			uSize = uLen - uTo;

		// Get buffer pointers
		T *pFrom = &pPtr[ uFrom ];
		T* pTo = &pPtr[ uTo ];

		// Copy the data
		for ( oexUINT i = 0; i < uSize; i++ )
			pTo[ i ] = pFrom[ i ];

		return uSize;
	}

	/// Removes characters in the specified range
	TStr& DropRange( T tMin, T tMax, oexBOOL bIn )
	{	
		T* pPtr = _Ptr();
		oexLONG lLen = Length();
		oexINT g = -1, b = -1, i = 0, l = 0;
		
		if ( bIn )
		{	for( i = 0; i < lLen; i++ )
			{	if ( pPtr[ i ] < tMin || pPtr[ i ] > tMax )
				{	if ( 0 > g ) g = i; l++; }
				else
				{	if ( 0 > b ) b = i, g = -1;
					else if ( 0 <= g ) Move( pPtr, lLen, g, b, i - g ), b = l, g = -1;
				} // end else
			} // end for
		} // end if

		else
		{	for( i = 0; i < lLen; i++ )
			{	if ( pPtr[ i ] >= tMin && pPtr[ i ] <= tMax )
				{	if ( 0 > g ) g = i; l++; }
				else
				{	if ( 0 > b ) b = i, g = -1;
					else if ( 0 <= g ) Move( pPtr, lLen, g, b, i - g ), b = l, g = -1;
				} // end else
			} // end for
		} // end if

		// Anything change?
		if ( 0 > b )
			return *this;

		// Move remainder if needed
		if ( 0 <= g )
			Move( pPtr, lLen, g, b, i - g ), b = i, g = -1;

		// Change the length of the string
		OexAllocate( l );
		m_lLength = l;

		return *this;
	}

	/// Removes characters in the specified array
	TStr& Drop( oexCONST T *pChars, oexBOOL bIn )
	{	
		T* pPtr = _Ptr();
		oexLONG lLen = Length();
		oexINT g = -1, b = -1, i = 0, l = 0;
        oexLONG lChars = zstr::Length( pChars );
		
		if ( bIn )
		{	for( i = 0; i < lLen; i++ )
			{	if ( 0 > str::FindCharacter( pChars, lChars, pPtr[ i ] ) )
				{	if ( 0 > g ) g = i; l++; }
				else
				{	if ( 0 > b ) b = i, g = -1;
					else if ( 0 <= g ) Move( pPtr, lLen, g, b, i - g ), b = l, g = -1;
				} // end else
			} // end for
		} // end if

		else
		{	for( i = 0; i < lLen; i++ )
			{	if ( 0 <= str::FindCharacter( pChars, lChars, pPtr[ i ] ) )
				{	if ( 0 > g ) g = i; l++; }
				else
				{	if ( 0 > b ) b = i, g = -1;
					else if ( 0 <= g ) Move( pPtr, lLen, g, b, i - g ), b = l, g = -1;
				} // end else
			} // end for
		} // end if


		// Move remainder if needed
		if ( 0 <= g )
			Move( pPtr, lLen, g, b, i - g ), b = i, g = -1;

		// Change the length of the string
		OexAllocate( l );
		m_lLength = l;

		return *this;
	}


public:

	TStr& Set( oexCONST TStr &x_str )
	{   Share( x_str );
		return *this;
	}

	TStr& Set( oexCONST T* x_pStr )
	{	return Set( x_pStr, zstr::Length( x_pStr ) ); }

	TStr& Set( oexCONST T* x_pStr, oexUINT uSize )
	{
		// Verify pointer
		if ( !oexVERIFY_PTR( x_pStr ) )
			return *this;

        // Ditch the offset
        m_lOffset = 0;

		// Allocate space for new string
		if ( !oexVERIFY_PTR( OexAllocate( uSize ) ) || !uSize )
			return *this;
		
		// Copy the string data
		m_mem.MemCpy( x_pStr, uSize );

		// Save the new string length
		m_lLength = uSize;

		return *this;
	}

	TStr& Set( oexCONST T x_chVal )
	{
		if ( !x_chVal )
		{	OexAllocate( 0 ); return *this; }

        // Ditch the offset
        m_lOffset = 0;

		// Allocate space for new string
		T* pPtr = OexAllocate( 1 );
		if ( !oexVERIFY( pPtr ) )
			return *this;

		pPtr[ 0 ] = x_chVal;

		m_lLength = 1;

		return *this;
	}

	// Concatenation operator
	TStr& Append( TStr &sStr )
	{	return Append( sStr.Ptr(), sStr.Length() ); }

	// Concatenation operator
	TStr& Append( oexCONST T* pStr )
	{	return Append( pStr, zstr::Length( pStr ) ); }

	// Concatenation operator
	TStr& Append( oexCONST T* pStr, oexUINT uSize )
	{
		// Allocate space for new string
		oexUINT uOldSize = Length();
		if ( !oexVERIFY( OexAllocate( uOldSize + uSize ) ) )
			return *this;

		// Append string
		os::CSys::MemCpy( m_mem.Ptr( uOldSize ), pStr, uSize );

		// Add new string length
		m_lLength = uOldSize + uSize;

		return *this;
	}

public:

	/// Trims uChars from the left side of the buffer
	/*
		This is efficient, it does not re-allocate or copy.
	*/
	TStr& RTrim( oexINT nChars )
	{   oexINT lLen = Length();
        if ( lLen <= nChars ) 
            Destroy();
        else
            Sub( 0, lLen - nChars );

		return *this;
	}

	/// Drops the specified characters from the end of the string
	TStr& RTrim( oexCONST T* pChars )
	{	oexVALIDATE_PTR( pChars ); oexLONG lLen = Length();
        oexINT i = str::RSkipCharacters( Ptr(), lLen, pChars, zstr::Length( pChars ) );
		if ( !i || i >= lLen ) return *this;
		RTrim( lLen - i - 1 );
		return *this;
	}

	/// Trims uChars from the left side of the buffer
	/*
		This is efficient, it moves the string pointer 
		without re-allocating or copying.
	*/
	TStr& LTrim( oexINT nChars )
	{   oexINT lLen = Length();
        if ( lLen <= nChars ) 
            Destroy();
        else
            Sub( nChars );

		return *this;
	}

	/// Drops the specified characters from the start of the string
	TStr& LTrim( oexCONST T* pChars, oexCONST T* pEscape = oexNULL )
	{	oexVALIDATE_PTR( pChars ); oexVALIDATE_PTR_NULL_OK( pEscape );
        oexINT i = str::SkipCharacters( Ptr(), Length(), pChars, zstr::Length( pChars ) );
		if ( 0 > i ) return *this;
		LTrim( i );
		return *this;
	}

	TStr& operator >>= ( oexUINT uChars )
	{	return LTrim( uChars ); }

	TStr& operator <<= ( oexUINT uChars )
	{	return RTrim( uChars ); }

	TStr& operator ++( int ) { return LTrim( 1 ); }

	TStr& operator --( int ) { return RTrim( 1 ); }

public:

    /// Converts letters from upper case to lower case
    TStr& ToLower()
    {   if ( Length() ) str::ToLower( _Ptr(), Length() ); return *this; }

    /// Converts letters from upper case to lower case
    TStr& ToUpper()
    {   if ( Length() ) str::ToUpper( _Ptr(), Length() ); return *this; }

public:

	TStr& vFmt( oexCONST T* x_pFmt, oexCPVOID x_pArgs )
	{
		// Verify input string
		if ( !oexVERIFY( x_pFmt ) )
		{	OexAllocate( 0 ); return *this; }

		T* pPt = oexNULL;
		oexRESULT res;
		oexUINT uSize = oexSTRSIZE;
		do
		{	
			// Allocate buffer
			// The - 1 is a performance boost, this keeps us under the TMemory block size.
			T* pPtr = OexAllocate( uSize - 1 ); 
			if ( !oexVERIFY_PTR( pPtr ) )
				return *this;

			// Attempt to format the string
			os::CSys::vStrFmt( &res, pPtr, uSize - 1, x_pFmt, x_pArgs );

			// Shift up size
			uSize <<= 1;

		} while ( uSize && os::CSys::c_StrErr_INSUFFICIENT_BUFFER == res );

		// Verfiy that the string was copied correctly
		oexASSERT( os::CSys::c_StrErr_OK == res );

		// Set the length of the string
		Length();

		return *this;
	}

	/// Formats a string
	TStr& Fmt( oexCONST T* pFmt, ... )
	{	return vFmt( pFmt, ( (oexPVOID*)&pFmt ) + 1 ); }

	/// Sets a number into the string using rules of Fmt()
	/// Truncates results larger than 256 characters
	TStr& SetNum( oexCONST T* pFmt, ... )
	{	T tBuf[ 256 ] = oexT( "" );
		os::CSys::vStrFmt( tBuf, sizeof( tBuf ), pFmt, ( ( (oexPVOID*)&pFmt ) + 1 ) );
		return Set( tBuf );
	}

	/// Sets a number into the string using rules of Fmt()
	/// Truncates results larger than 256 characters
    /// Optional pre and post trimming
	TStr& SetNumTrim( oexCONST T* pFmt, oexCONST T* pLTrim, oexCONST T* pRTrim, ... )
	{   os::CSys::vStrFmt( OexAllocate( 256 ), 256, pFmt, ( ( (oexPVOID*)&pRTrim ) + 1 ) );
        if ( pLTrim ) LTrim( pLTrim );
        if ( pRTrim ) RTrim( pRTrim );
        return *this;
	}

	/// Appends a formatted number to the string using rules of Fmt()
	/// Truncates results larger than 256 characters
	TStr& AppendNum( oexCONST T* pFmt, ... )
	{	T tBuf[ 256 ] = oexT( "" );
		os::CSys::vStrFmt( tBuf, sizeof( tBuf ), pFmt, ( ( (oexPVOID*)&pFmt ) + 1 ) );
		return Append( tBuf );		
	}

    /// Sets a number into the string using rules of Fmt()
	/// Truncates results larger than 256 characters
    /// Optional pre and post trimming
	TStr& AppendNumTrim( oexCONST T* pFmt, oexCONST T* pLTrim, oexCONST T* pRTrim, ... )
	{   TStr str;        
   		os::CSys::vStrFmt( str.OexAllocate( 256 ), 256, pFmt, ( ( (oexPVOID*)&pRTrim ) + 1 ) );
        if ( pLTrim ) str.LTrim( pLTrim );
        if ( pRTrim ) str.RTrim( pRTrim );
        return Append( str );
	}

	/// Converts the string to a GUID, returns oexNULL if this string does not contain a GUID
	oexCONST oexGUID * StringToGuid( oexGUID *x_pGuid )
    {	return guid::StringToGuid( x_pGuid, Ptr(), Length() ); }

	/// Converts a GUID to a string, if pGuid is NULL, a unique GUID is created
	TStr& GuidToString( oexCONST oexGUID *x_pGuid = oexNULL )
	{	guid::GuidToString( OexAllocate( 36 /* + 1 is implicit*/ ), 37, x_pGuid ); 
		oexVERIFY( Length() == 36 );
		return *this; 		
	}

public:

    /// Converts to a number
    oexINT64 ToNum( oexINT x_nMax = 0, oexUINT x_uRadix = 10, oexINT *x_pnEnd = oexNULL, oexBOOL x_bTrim = oexFALSE )
    {   if ( !x_nMax || x_nMax > Length() ) x_nMax = Length();
        oexINT nEnd = 0; oexINT64 llNum = os::CSys::StrToNum( Ptr(), x_nMax, x_uRadix, &nEnd ); 
        if ( x_bTrim ) LTrim( nEnd );
        if ( x_pnEnd ) *x_pnEnd = nEnd;
        return llNum;
    }

	/// Converts to long
	oexLONG ToLong( oexUINT uRadix = 10 ) { return os::CSys::StrToLong( Ptr(), uRadix ); }

	/// Converts to unsigned long
	oexULONG ToULong( oexUINT uRadix = 10 ) { return os::CSys::StrToULong( Ptr(), uRadix ); }

	/// Converts to double
	oexDOUBLE ToDouble() { return os::CSys::StrToDouble( Ptr( 0 ) ); }
    
    /// Converts to a string, 
	oexCONST T* ToString() { return Ptr(); }

	oexBOOL Cmp( TStr &str )
	{	return Cmp( str.Ptr(), str.Length() ); }

	oexBOOL Cmp( oexCONST T* pStr )
	{	return Cmp( pStr, zstr::Length( pStr ) ); }

	oexBOOL Cmp( oexCONST T* pStr, oexUINT uSize )
	{	
        if ( uSize != Length() ) 
            return oexFALSE;
		if ( !uSize ) 
            return oexTRUE;
		return !os::CSys::MemCmp( m_mem.Ptr(), pStr, uSize );
	}

	oexBOOL ICmp( TStr &str )
	{	return ICmp( str.Ptr(), str.Length() ); }

	oexBOOL ICmp( oexCONST T* pStr )
	{	return ICmp( pStr, zstr::Length( pStr ) ); }

	oexBOOL ICmp( oexCONST T* pStr, oexUINT uSize )
    {   return !str::ICompare( Ptr(), Length(), pStr, uSize ); }

	oexBOOL CmpLen( TStr &str, oexINT x_lLen = -1 )
	{	if ( 0 > x_lLen ) x_lLen = str.Length();
        return CmpLen( str.Ptr(), str.Length(), x_lLen ); 
    }

	oexBOOL CmpLen( oexCONST T* pStr, oexINT x_lLen = -1 )
	{	oexINT lLen = zstr::Length( pStr );
        if ( 0 > x_lLen ) x_lLen = lLen;
		return CmpLen( pStr, lLen, x_lLen ); 
    }

	oexBOOL CmpLen( oexCONST T* pStr, oexUINT uSize, oexINT x_lLen = -1 )
    {   if ( 0 > x_lLen ) x_lLen = uSize;
        return !str::CompareLen( Ptr(), Length(), pStr, uSize, x_lLen ); 
    }

	oexBOOL ICmpLen( TStr &str, oexINT x_lLen = -1 )
	{	if ( 0 > x_lLen ) x_lLen = str.Length();
	 	return ICmpLen( str.Ptr(), str.Length(), x_lLen ); 
    }

	oexBOOL ICmpLen( oexCONST T* pStr, oexINT x_lLen = -1 )
	{	oexINT lLen = zstr::Length( pStr );
        if ( 0 > x_lLen ) x_lLen = lLen;
		return ICmpLen( pStr, lLen, x_lLen ); 
    }

	oexBOOL ICmpLen( oexCONST T* pStr, oexUINT uSize, oexINT x_lLen = -1 )
    {   if ( 0 > x_lLen ) x_lLen = uSize;
        return !str::ICompareLen( Ptr(), Length(), pStr, uSize, x_lLen ); 
    }

	/// Returns the offset of pSub in the string, or
    /// -1 if a pStr is not found.
	oexINT Match( oexCONST T* pSub )
    {   return str::FindSubStr( Ptr(), Length(), pSub, zstr::Length( pSub ) ); }

public:

	/// Replaces single characters in a string
	TStr& Replace( oexCONST T x_tFind, oexCONST T x_tReplace )
    {   str::Replace( _Ptr(), Length(), x_tFind, x_tReplace ); return *this; }


	//==============================================================
	// Compare functions
	//==============================================================
	static oexINT CmpCompare( TStr pStr1, TStr pStr2 )
    {	return str::Compare( pStr1.Ptr(), pStr1.Length(), pStr2.Ptr(), pStr2.Length() ); }

	static oexINT CmpSubStr( TStr pStr1, TStr pStr2 )
	{	return str::FindSubStr( pStr1.Ptr(), pStr1.Length(), pStr2.Ptr(), pStr2.Length() ); }


public:

    //==============================================================
	// Parsing functions
	//==============================================================

    /// Trims white-space from the front and end of the string
    TStr& TrimWhiteSpace()
    {   return SkipWhiteSpace().RSkipWhiteSpace(); }

	/// Skips white-space characters
	/// White space characters 'c' where ( '!' > c > '~' )
	TStr& SkipWhiteSpace()
	{	return FindInRange( oexT( '!' ), oexT( '~' ) ); }

	/// Skips white-space characters
	/// White space characters 'c' where ( '!' > c > '~' )
	TStr& RSkipWhiteSpace()
	{	return RFindInRange( oexT( '!' ), oexT( '~' ) ); }

	/// Skips any non-white space characters
	/// Non-white space characters 'c' where ( '!' <= c <= '~' )
	TStr& SkipNonWhiteSpace()
	{	return SkipInRange( oexT( '!' ), oexT( '~' ) ); }

	/// Skips any non-white space characters
	/// Non-white space characters 'c' where ( '!' <= c <= '~' )
	TStr& RSkipNonWhiteSpace()
	{	return RSkipInRange( oexT( '!' ), oexT( '~' ) ); }

	/// Skips to the next line, current line is removed from the string.
	TStr& NextLine()
	{	Find( oexT( "\r\n" ) ); return Skip( oexT( "\r\n" ) ); }

	/// Removes white space from the string
	TStr& DropWhiteSpace()
	{	return DropRange( oexT( '!' ), oexT( '~' ), oexFALSE ); }

	/// Skips any character 'c' where ( tMin > c > tMax )
	/// Out of range characters are removed from the string
	TStr& FindInRange( T tMin, T tMax )
	{	oexINT p = str::FindInRange( Ptr(), Length(), tMin, tMax );
		if ( 0 < p ) LTrim( p ); else if ( 0 > p ) Destroy();
		return *this;
	}

	/// Skips any character 'c' where ( tMin > c > tMax )
	/// Out of range characters are removed from the string
	TStr& RFindInRange( T tMin, T tMax )
	{	oexINT lLen = Length(); if ( !lLen ) return *this;
        oexINT p = str::RFindInRange( Ptr(), lLen, tMin, tMax );
		if ( 0 <= p && p < lLen ) RTrim( lLen - p - 1 ); else if ( 0 > p ) Destroy();
		return *this;
	}

	/// Skips any character 'c' where ( tMin <= c <= tMax )
	/// In range characters are removed from the string
	TStr& SkipInRange( T tMin, T tMax )
	{	oexUINT uLen = Length();
		oexINT p = str::SkipInRange( Ptr(), Length(), tMin, tMax );
		if ( 0 < p ) LTrim( p ); else if ( 0 > p ) Destroy();
		return *this;
	}

	/// Skips any character 'c' where ( tMin <= c <= tMax )
	/// In range characters are removed from the string
	TStr& RSkipInRange( T tMin, T tMax )
	{	oexINT lLen = Length(); if ( !lLen ) return *this;
		oexINT p = str::RSkipInRange( Ptr(), lLen, tMin, tMax );
		if ( 0 <= p && p < lLen ) RTrim( lLen - p - 1 ); else if ( 0 > p ) Destroy();
		return *this;
	}

	/// Skips any characters not in pChars and removes
	/// them from the string
	TStr& Find( oexCONST T* pChars )
	{	oexINT p = str::FindCharacters( Ptr(), Length(), pChars, zstr::Length( pChars ) );
		if ( 0 < p ) LTrim( p ); else if ( 0 > p ) Destroy();
		return *this;
	}

	/// Skips any characters in pChars and removes them from
	/// the string.
	TStr& Skip( oexCONST T* pChars )
    {	oexINT p = str::SkipCharacters( Ptr(), Length(), pChars, zstr::Length( pChars ) );
		if ( 0 < p ) LTrim( p ); else if ( 0 > p ) Destroy();
		return *this;
	}

	/// Splits off a token and returns it
	TStr ParseQuoted( oexCONST T *pOpen, oexCONST T *pClose, oexCONST T *pEscape = oexNULL )
    {	oexVERIFY_PTR_NULL_OK( pEscape );
        oexINT i = str::ParseQuoted(    Ptr(), Length(), 
                                        pOpen, zstr::Length( pOpen ), 
                                        pClose, zstr::Length( pOpen ), 
                                        pEscape, pEscape ? zstr::Length( pEscape ) ? 0 );
		if ( 0 >= i ) return TStr();
		TStr str = SubStr( 1, i - 1 );
		LTrim( i + 1 );
		return str;
	}

	/// Splits off a token and returns it
	TStr ParseToken( oexCONST T* pValid )
	{	oexVALIDATE_PTR( pValid );
		oexINT i = 0;
		TStr str = Token( pValid, &i );
		if ( i ) LTrim( i );
		return str;
	}

	/// Splits off a token and returns it
	TStr ParseNextToken( oexCONST T* pValid )
	{	oexVALIDATE_PTR( pValid );
		oexINT i = 0;
		TStr str = NextToken( pValid, &i );
		if ( i ) LTrim( i );
		return str;
	}

	/// Splits off a token and returns it
	TStr ParseNextToken( T tMin = oexT( '!' ), T tMax = oexT( '~' ) )
	{	oexINT i = 0;
		TStr str = NextToken( tMin, tMax, &i );
		if ( i ) LTrim( i );
		return str;
	}

	/// Splits off a string up any of the terminator characters,
	/// Ignores terminator characters if preceeded by any character
	/// in the escape string.
	TStr Parse( oexCONST T* pTerm, oexCONST T* pEscape = oexNULL )
	{	oexVALIDATE_PTR( pTerm ); oexVALIDATE_PTR_NULL_OK( pEscape );
        oexINT i = str::FindTerm( Ptr(), Length(), pTerm, zstr::Length( pTerm ), pEscape, pEscape ? zstr::Length( pEscape ) : 0 );
		if ( 0 > i ) return TStr();
		TStr str( Ptr(), 0, i );
		LTrim( i );
		return str;
	}

	/// Splits off a string up any of the terminator characters,
	/// Ignores terminator characters if preceeded by any character
	/// in the escape string.
	TStr RParse( oexCONST T* pTerm )
	{	oexVALIDATE_PTR( pTerm );
        oexINT lLen = Length();
        oexINT i = str::RFindTerm( Ptr(), lLen, pTerm, zstr::Length( pTerm ) );
		if ( 0 > i ) return TStr();
		TStr str( Ptr( i ) );
		RTrim( lLen - i );
		return str;
	}

	oexBOOL IsMatchAt( oexUINT i, oexCONST T* pChars )
	{	oexVALIDATE_PTR( pChars ); 
        return 0 <= str::FindCharacter( pChars, zstr::Length( pChars ), *Ptr( i ) ); 
	}

    oexBOOL IsInRangeAt( oexUINT i, T tMin, T tMax )
	{   T ch = *Ptr( i );
        return tMin <= ch && tMax >= ch;
	}

    oexBOOL IsWhiteSpaceAt( oexUINT i )
    {   return !IsInRangeAt( i, oexT( '!' ), oexT( '~' ) ); }

	/// Finds the first token in a string and returns it
	TStr NextToken( T tMin, T tMax, oexINT *puI = oexNULL )
	{	return NextToken( Ptr(), Length(), tMin, tMax, puI ); }

	static TStr NextToken( oexCONST T* pStr, T tMin, T tMax, oexINT *puI = oexNULL )
	{	return NextToken( pStr, CRawStr::Length( pStr ), tMin, tMax, puI ); }

	static TStr NextToken( TStr str, T tMin, T tMax, oexINT *puI = oexNULL )
	{	return NextToken( str.Ptr(), str.Length(), tMin, tMax, puI ); }

	static TStr NextToken( oexCONST T* pStr, oexUINT uSize, T tMin, T tMax, oexINT *puI  = oexNULL )
	{
		// Sanity check
		if ( !oexVERIFY_PTR( pStr ) )
			return CStr();

		oexUINT i = 0;

		// Ensure valid index pointer
		if ( !oexVERIFY_PTR_NULL_OK( puI ) ) 
			puI = oexNULL;

		// Start at this index
		else if ( puI ) 
			i = *puI;

		// Ensure we're not beyond the buffer
		if ( i >= uSize ) 
			return CStr();

		// Skip to previous position
		pStr += i;
		uSize -= i;

		// Find the start of a token
        oexINT nStart = str::FindInRange( pStr, uSize, tMin, tMax );
		if ( 0 > nStart ) 
			return CStr();

		// Find the end of the token
		oexINT nEnd = str::SkipInRange( &pStr[ nStart ], uSize - nStart, tMin, tMax );
		if ( 0 >= nEnd ) 
			return CStr();

		// Add offset
		if ( puI ) 
			*puI += nStart + nEnd;

		return CStr( &pStr[ nStart ], 0, nEnd );
	}


	/// Finds the first token in a string and returns it
	TStr NextToken( oexCONST T* pValid, oexINT *puI = oexNULL )
	{	return NextToken( Ptr(), Length(), pValid, puI ); }

	static TStr NextToken( oexCONST T* pStr, oexCONST T* pValid, oexINT *puI = oexNULL )
	{	return NextToken( pStr, zstr::Length( pStr ), pValid, puI ); }

	static TStr NextToken( TStr str, oexCONST T* pValid, oexINT *puI = oexNULL )
	{	return NextToken( str.Ptr(), str.Length(), pValid, puI ); }

	static TStr NextToken( oexCONST T* pStr, oexUINT uSize, oexCSTR pValid, oexINT *puI  = oexNULL )
	{
		// Sanity check
		if ( !oexVERIFY_PTR( pStr ) || !oexVERIFY_PTR_NULL_OK( pValid ) )
			return CStr();

		if ( !pValid )
			return CStr( pStr, uSize );		

		oexUINT i = 0;
		oexUINT uValid = zstr::Length( pValid );

		// Ensure valid index pointer
		if ( !oexVERIFY_PTR_NULL_OK( puI ) ) 
			puI = oexNULL;

		// Start at this index
		else if ( puI ) 
			i = *puI;

		// Ensure we're not beyond the buffer
		if ( i >= uSize ) 
			return CStr();

		// Skip to previous position
		pStr += i;
		uSize -= i;

		// Find the start of a token
		oexINT nStart = str::FindCharacters( pStr, uSize, pValid, uValid );
		if ( 0 > nStart ) 
			return CStr();

		// Find the end of the token
		oexINT nEnd = str::SkipCharacters( &pStr[ nStart ], uSize - nStart, pValid, uValid );
		if ( 0 >= nEnd ) 
			return CStr();

		// Add offset
		if ( puI ) 
			*puI += nStart + nEnd;

		return CStr( &pStr[ nStart ], 0, nEnd );
	}

	/// Returns the token at the current point in the string
	TStr Token( oexCONST T* pValid, oexINT *puI = oexNULL )
	{	return Token( Ptr(), Length(), pValid, puI ); }
	static TStr Token( oexCONST T* pStr, oexCONST T* pValid, oexINT *puI = oexNULL )
	{	return Token( pStr, zstr::Length( pStr ), pValid, puI ); }
	static TStr Token( TStr str, oexCONST T* pValid, oexINT *puI = oexNULL )
	{	return Token( str.Ptr(), str.Length(), pValid, puI ); }
	static TStr Token( oexCONST T* pStr, oexUINT uSize, oexCSTR pValid, oexINT *puI  = oexNULL )
	{
		// Sanity check
		if ( !oexVERIFY_PTR( pStr ) || !oexVERIFY_PTR_NULL_OK( pValid ) )
			return CStr();

		if ( !pValid )
			return CStr( pStr, uSize );		

		oexUINT i = 0;
		oexUINT uValid = zstr::Length( pValid );

		// Ensure valid index pointer
		if ( !oexVERIFY_PTR_NULL_OK( puI ) ) 
			puI = oexNULL;

		// Start at this index
		else if ( puI ) 
			i = *puI;

		// Ensure we're not beyond the buffer
		if ( i >= uSize ) 
			return CStr();

		// Skip to previous position
		pStr += i;
		uSize -= i;

		// Find the end of the token
		oexINT nEnd = str::SkipCharacters( pStr, uSize, pValid, uValid );
		if ( 0 >= nEnd ) 
			return CStr();

		// Add offset
		if ( puI ) 
			*puI += nEnd;

		return CStr( pStr, 0, nEnd );
	}


private:

    /// The string memory
    TMem< T >       m_mem;

    /// The length of the string
    oexINT          m_lLength;
    
    /// Offset into the string, this is invaluable for text parsing
    oexINT          m_lOffset;
    
#if defined( _DEBUG ) || defined( OEX_ENABLE_RELEASE_MODE_MEM_CHECK )

public:
    
    /// Registers the source file and line number
    TStr& _Log( oexUINT x_uLine, oexCSTR x_pFile )
    {   m_uLine = x_uLine;
        m_pFile = x_pFile;
        return *this;
    }

private:

    /// Holds the line number of the allocating call
    oexUINT         m_uLine;

    /// Holds the file name of the allocating call
    oexCSTR         m_pFile;

#endif

};


/// Unicode / Multibyte string class
typedef TStr< oexTCHAR >    CStr;

#if ( _MSC_VER < 1300 )

/// Unicode string class
typedef TStr< unsigned short >     CStrW;

#else

/// Unicode string class
typedef TStr< wchar_t >     CStrW;

#endif

/// 8 bit character string
typedef TStr< oexCHAR >     CStr8;

/// 16 bit character string
typedef TStr< oexINT16 >    CStr16;

/// 32 bit character string
typedef TStr< oexINT32 >    CStr32;

/// 64 bit character string
typedef TStr< oexINT64 >    CStr64;
