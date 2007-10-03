/*------------------------------------------------------------------
// oex_common.h
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

// For tracking allocations
#define OexAllocNew             oex::CAlloc( __LINE__, oexTEXT( __FILE__ ) ).New
#define OexAllocDelete          oex::CAlloc( __LINE__, oexTEXT( __FILE__ ) ).Delete
#define OexAllocResize          oex::CAlloc( __LINE__, oexTEXT( __FILE__ ) ).Resize

#define OexAllocConstruct       oex::CAlloc( __LINE__, oexTEXT( __FILE__ ) ).Construct
#define OexAllocConstructArray  oex::CAlloc( __LINE__, oexTEXT( __FILE__ ) ).ConstructArray
#define OexAllocDestruct        oex::CAlloc( __LINE__, oexTEXT( __FILE__ ) ).Destruct


// Object function macros
#define OexNew                  _Log( __LINE__, oexTEXT( __FILE__ ) ).New
#define OexDelete               _Log( __LINE__, oexTEXT( __FILE__ ) ).Delete
#define OexResize               _Log( __LINE__, oexTEXT( __FILE__ ) ).Resize

#define OexConstruct            _Log( __LINE__, oexTEXT( __FILE__ ) ).Construct
#define OexConstructArray       _Log( __LINE__, oexTEXT( __FILE__ ) ).ConstructArray
#define OexDestruct             _Log( __LINE__, oexTEXT( __FILE__ ) ).Destruct

#define OexAllocate             _Log( __LINE__, oexTEXT( __FILE__ ) ).Allocate

template < class T_BASE > class TClassFactoryBase
{
public:
	virtual T_BASE* New() { return oexNULL; }
	virtual void Delete( T_BASE* p ) { }
    virtual oexBOOL IsValid() { return oexFALSE; }
};

template < class T_BASE, class T_DERIVED > class TClassFactory : public TClassFactoryBase< T_BASE >
{
public:
	virtual T_BASE* New() { return (T_BASE*)( oexNEW T_DERIVED ); }
	virtual void Delete( T_BASE* p ) { oexDELETE ( (T_DERIVED*)p ); }
    virtual oexBOOL IsValid() { return oexTRUE; }
};		

namespace cmn
{
    /// Returns the largest of a or b
    template < typename T >
        T Max( T a, T b )
    {   return ( a > b ) ? a : b; }

    /// Returns the largest of a, b, or c
    template < typename T >
        T Max( T a, T b, T c )
    {   return a > b ? ( a > c ? a : c ) : ( b > c ? b : c ); }

    /// Returns the smallest of a or b
    template < typename T >
        T Min( T a, T b )
    {   return ( a < b ) ? a : b; }

    /// Returns the smallest of a, b, or c
    template < typename T >
        T Min( T a, T b, T c )
    {   return a < b ? ( a < c ? a : c ) : ( b < c ? b : c ); }

    /// Returns val limited to the range of min and max
    template < typename T >
        T Range( T val, T min, T max )
    {   return ( val < min ) ? min : ( val > max ) ? max : val; }

    /// Compares two values ensuring the return type is bool
    template < typename T >
        oexBOOL Eq( T a, T b )
    {   return ( a == b ) ? oexTRUE : oexFALSE; }

    /// Compares two values ensuring the return type is bool
    template < typename T >
        oexBOOL Neq( T a, T b )
    {   return ( a != b ) ? oexTRUE : oexFALSE; }

    /// Compares two values ensuring the return type is bool
    template < typename T >
        oexBOOL Lt( T a, T b )
    {   return ( a < b ) ? oexTRUE : oexFALSE; }

    /// Compares two values ensuring the return type is bool
    template < typename T >
        oexBOOL Gt( T a, T b )
    {   return ( a > b ) ? oexTRUE : oexFALSE; }

    /// Invokes the NOT (!) operator ensuring the return type is bool
    template < typename T >
        oexBOOL Not( T a )
    {   return ( !a ) ? oexTRUE : oexFALSE; }

    /// Returns the difference between two values
    template < typename T >
        T Dif( T a, T b )
    {   return a > b ? a - b : b - a; }

    /// Returns the absolute value
    template < typename T >
        T Abs( T v )
    {   return ( v >= 0 ) ? v : -v; }

    /// Scales a value v from range( 0, cur ) to range( 0, tgt )
    template < typename T >
        T Scale( T v, T cur, T tgt )
    {   if ( !cur ) return 0; return v * tgt / cur; }

    /// Scales a value v from range( min1, max1 ) to range( min2, max2 )
    template < typename T >
        T Scale( T v, T min1, T max1, T min2, T max2 )
    {   return min2 + Scale( v - min1, max1 - min1, max2 - min2 ); }

    /// Smooths value a to b using  ( a * weight + b ) / ( weight + 1 )
    template < typename T >
        T Smooth( T a, T b, T weight )
    {   return ( a * weight + b ) / ( weight + 1 ); }

    /// Smoothes value a to b using ( ( a << weight ) - a + b ) >> weight
    template < typename T >
        T FastSmooth( T a, T b, T weight )
    {   return ( ( a << weight ) - a + b ) >> weight; }

    /// Swaps value a and b, this will usually be the most efficient Swap function
    template < typename T >
        void Swap( T &a, T &b )
    {   T t = a; a = b; b = t; }

    /// Swaps value a and b without a temporary
    template < typename T >
        void SwapNoTemp( T &a, T &b )
    {   a += b; b = a - b; a -= b; }

    /// Swaps integer value a and b without a temporary
    template < typename T >
        void SwapXor( T &a, T &b )
    {   a ^= b ^= a ^= b; }

    /// Values for pi
    oexCONST oexDOUBLE c_PI = 3.141592654;
    oexCONST oexDOUBLE c_PI2 = c_PI * (oexDOUBLE)2;

    /// Returns the opposite angle
    template< typename T >
        T OppositeAngle( T a, T m = (T)c_PI2 )
    {   T h = m / 2; return ( a >= h ) ? a - h : a + h; }

    /// Calculates the minimum difference betwen the two angles
    template < typename T >
	    T MinDifAngle( T a, T b, T m = (T)c_PI2 )
	{   T dif = Dif( a, b );
		return ( dif >= ( m / 2 ) ) ? ( m - dif ) : dif;
	}

    /// Averages two angles
    template< typename T >
        T MinAvgAngle( T a, T b, T m = (T)c_PI2 )
    {   if ( a > b ) Swap( a, b );
        T h = m / 2, dif = b - a, avg;
        if ( dif > h )
        {   dif = ( m - dif ) / 2;
            avg = ( dif > a ) ? b + dif : a - dif;
        } // end if
        else avg = a + dif / 2;        
        if ( dif == h && avg >= h ) avg -= h;
        return avg;
    }
        
    /// Aligns value on 16 bit boundry
    template < typename T >
        T Align2( T val )
    {   return ( val + 1 ) & ( ~1 ); }

    /// Aligns value on 32 bit boundry
    template < typename T >
        T Align4( T val )
    {   return ( val + 3 ) & ( ~3 ); }

    /// Aligns value on 64 bit boundry
    template < typename T >
        T Align8( T val )
    {   return ( val + 7 ) & ( ~7 ); }

    /// Aligns value on 128 bit boundry
    template < typename T >
        T Align16( T val )
    {   return ( val + 15 ) & ( ~0x0f ); }

    /// Aligns value on processor dependent boundry
#if oexSIZEOFINT == 2
    template < typename T >
        T Align( T val ) { return Align2( val ); }
#elif oexSIZEOFINT == 4
    template < typename T >
        T Align( T val ) { return Align4( val ); }
#elif oexSIZEOFINT == 8
    template < typename T >
        T Align( T val ) { return Align8( val ); }
#elif oexSIZEOFINT == 16
    template < typename T >
        T Align( T val ) { return Align16( val ); }
#endif

    /// Returns the next power of two equal to or greater than v, or v if there isn't one
    template< typename T >
	    T NextPower2( T v )
	{	T n = 2;
		while ( n && n < v ) n <<= 1;
		if ( !n ) return v;
		return n;
	}

    /// Returns the state of the specified bit in buf
    template < typename T > 
        oexBOOL GetBit( T *buf, oexUINT bit )
    {   return 0 != ( ( (oexUCHAR*)buf)[ bit >> 3 ] & ( 1 << ( bit & 7 ) ) ) ? oexTRUE : oexFALSE; }

    /// Sets the state of the specified bit in buf
    template < typename T > 
        void SetBit( T *buf, oexUINT bit, oexBOOL set )
    {   if ( set ) ( (oexUCHAR*)buf)[ bit >> 3 ] |= ( 1 << ( bit & 7 ) );
        else ( (oexUCHAR*)buf)[ bit >> 3 ] &= ~( 1 << ( bit & 7 ) );
    }

    /// Copies bits number of bits from src[ soff ] to dst[ doff ]
    /// where doff and soff are offsets in bits.
    /**

        +++ This could probably be made a little more efficient by using
            masks and copying multiple bits at once, but that would make my 
            brain hurt at the moment.
    */
    template < typename T >
        void CopyBits( T *dst, oexUINT doff, T *src, oexUINT soff, oexUINT bits )
    {   while ( bits-- )
            SetBit( dst, doff++, GetBit( src, soff++ ) );
    }

};

