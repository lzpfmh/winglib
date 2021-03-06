/*------------------------------------------------------------------
// fifo_sync.h
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

//==================================================================
// CFifoSync
//
/// Implements a first in first out synchronized buffer
/**
	Use this to add variable length blocks into a synchronized
	first-in-first-out buffer.
*/
//==================================================================
class CFifoSync : public CCircBuf
{
public:

    enum
    {
        /// The default number of max buffers
        eDefaultMaxBuffers = 1024
    };

	/// Size type
	typedef CCircBuf::t_size	t_size;

#ifndef OEX_NOPACK
#	pragma pack( push, 1 )
#endif

    struct SBufferInfo
    {
	    /// Fifo head pointer
	    t_size				        uHeadPtr;

	    /// Fifo tail pointer
	    t_size				        uTailPtr;
    };

#ifndef OEX_NOPACK
#	pragma pack( pop )
#endif

public:

	/// Constructor
	CFifoSync( oexBOOL x_bSync = oexTRUE, t_size x_uSize = 0, CCircBuf::t_eWm x_nWriteMode = eWmGrow, t_size x_uMaxBuffers = eDefaultMaxBuffers );

	/// Destructor
	virtual ~CFifoSync();

  	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases all resources associated with the circular buffer.
	virtual void Destroy();

	//==============================================================
	// SetName()
	//==============================================================
    /// Sets the shared memory name
    /**
        \param [in] x_pName -   Shared memory name
        \param [in] x_uLen  -   Length of the string in x_pName.
                                Can be zero if the string is
                                NULL terminated.

    */
    virtual CFifoSync& SetName( oexCSTR x_pName, t_size x_uLen = 0 )
    {
        if ( x_pName && !x_uLen )
            x_uLen = zstr::Length( x_pName );

        // Set shared name for the block sizes
        if ( !x_pName || !*x_pName )
            m_auSize.SetName( oexNULL );
        else
            m_auSize.SetName( CStr( x_pName, x_uLen ).Append( oexT( "_block" ) ).Ptr() );

		// Set the name
        CCircBuf::SetName( x_pName, x_uLen );

        return *this;
    }

	//==============================================================
	// Allocate()
	//==============================================================
	/// Pre allocates the specified amount of data for the circular buffer
	/**
		\param [in] x_uSize		-	Number of bytes to allocate.

		\return Non-zero if success

		\see
	*/
	virtual oexBOOL Allocate( t_size x_uSize )
    {
        if ( !CCircBuf::Allocate( x_uSize ) )
            return oexFALSE;

        m_uMaxBuffers = cmn::NextPower2( x_uSize / 16 );

        return AllocateBuffers();
    }

	//==============================================================
	// Allocate()
	//==============================================================
	/// Pre allocates the specified amount of data for the circular buffer
	/**
		\param [in] x_uSize		    -	Number of bytes to allocate.
        \param [in] x_uMaxBuffers   -   Maximum number of buffers

		\return Non-zero if success

		\see
	*/
	virtual oexBOOL Allocate( t_size x_uSize, t_size x_uMaxBuffers )
    {
        if ( !CCircBuf::Allocate( x_uSize ) )
            return oexFALSE;

        m_uMaxBuffers = cmn::NextPower2( x_uMaxBuffers );

        return AllocateBuffers();
    }

	//==============================================================
	// Destroy2()
	//==============================================================
    /// Destroys the fifo sync
    void Destroy2();

	//==============================================================
	// SkipBlock()
	//==============================================================
	/// Skips the next data block in the buffer
	/**
		\return Non-zero if success
	*/
	oexBOOL SkipBlock();

	//==============================================================
	// EndFifoWrite()
	//==============================================================
	/// Advances head fifo head pointer, i.e. commits write
	/**
		\return Non-zero if success
	*/
	oexBOOL EndFifoWrite();

	//==============================================================
	// InitFifoWrite()
	//==============================================================
	/// Ensures space for a fifo write
	/**
		\return Non-zero if success
	*/
	oexBOOL InitFifoWrite( t_size x_uSize );

	//==============================================================
	// AddFifo()
	//==============================================================
	/// Adds a data block to the fifo
	/**
		\param [in] x_pBuf		-	Pointer to the data to add
		\param [in] x_uSize		-	Size of the data in x_pBuf
		\param [in] x_uEncode	-	Optional encoding

		\return Non-zero if success

		\see
	*/
	oexBOOL AddFifo( oexCPVOID x_pBuf, t_size x_uSize, oexUINT x_uEncode = 0 );

	//==============================================================
	// AllocateBuffers()
	//==============================================================
	/// Allocates buffers for fifo
	/**
		\return Non-zero if success
	*/
	oexBOOL AllocateBuffers();

	//==============================================================
	// Write()
	//==============================================================
	/// Writes a block to the fifo
	/**
		\param [in] x_pBuf		-	Pointer to the data block
		\param [in] x_uSize		-	Size of the data in x_pBuf
		\param [in] x_uEncode	-	Optional encoding

		\return Number of bytes written

		\see
	*/
	virtual t_size Write( oexCPVOID x_pBuf, t_size x_uSize, oexUINT x_uEncode = 0 );

	//==============================================================
	// Write()
	//==============================================================
	/// Write a string to the buffer
	/**
		\param [in] x_pStr	-	String value to write to buffer

		\return Number of bytes written

		\see
	*/
	virtual t_size Write( oexCSTR8 x_pStr )
    {   return Write( x_pStr, zstr::Length( x_pStr ) ); }

	//==============================================================
	// Write()
	//==============================================================
	/// Write a string to the buffer
	/**
		\param [in] x_sStr	-	String to write

		\return Number of bytes written

		\see
	*/
	virtual t_size Write( CStr8 x_sStr )
    {   return Write( x_sStr.Ptr(), x_sStr.Length() ); }

	// Import read functions
	using CCircBuf::Read;

	//==============================================================
	// Read()
	//==============================================================
	/// Reads a block of data from the fifo
	/**
		\param [out] x_pBuf		-	Pointer to buffer that receives the data
		\param [in] x_uSize		-	Size of buffer in x_pBuf
		\param [in] x_puRead	-	Number of bytes read into x_pBuf
		\param [in] x_uEncode	-	Optional decoding

		\return Non-zero if success

		\see
	*/
	virtual oexBOOL Read( oexPVOID x_pBuf, t_size x_uSize, t_size *x_puRead, oexUINT x_uEncode = 0 );

    virtual oexBOOL Read( CStr8 &x_sStr, t_size x_uMax = 0 );

    virtual CStr8 Read( t_size x_uMax = 0 )
    {   CStr8 str; Read( str, x_uMax ); return str; }


	//==============================================================
	// Peek()
	//==============================================================
	/// Peeks at the data in the fifo buffer
	/**
		\param [out] x_pBuf		-	Pointer to buffer that receives the data
		\param [in] x_uSize		-	Size of buffer in x_pBuf
		\param [in] x_puRead	-	Number of bytes read into x_pBuf
		\param [in] x_lOffset	-	Offset from first byte to start read
		\param [in] x_uEncode	-	Optional decoding

		\return

		\see
	*/
	virtual oexBOOL Peek( oexPVOID x_pBuf, t_size x_uSize, t_size *x_puRead, oexLONG x_lOffset = 0, oexUINT x_uEncode = 0 );

    virtual oexBOOL Peek( CStr8 &x_sStr, t_size x_uMax = 0, oexBOOL x_bAppend = oexFALSE );

    virtual CStr8 Peek( t_size x_uMax = 0 )
    {   CStr8 str; Peek( str, x_uMax ); return str; }

	virtual oexBOOL Peek( t_size x_uBuffer, oexPVOID x_pBuf, t_size x_uSize, t_size *x_puRead, oexLONG x_lOffset = 0, oexUINT x_uEncode = 0 );

    virtual oexBOOL Peek( t_size x_uBuffer, CStr8 &x_sStr, t_size x_uMax = 0, oexBOOL x_bAppend = oexFALSE );

    virtual CStr8 Peek( t_size x_uBuffer, t_size x_uMax = 0 )
    {   CStr8 str; Peek( x_uBuffer, str, x_uMax ); return str; }

	virtual oexBOOL Empty()
	{
		oexAutoLock ll( *this );
		if ( !ll.IsLocked() )
            return oexFALSE;

		// Reset buffer pointers
		if ( m_pBi )
		{	m_pBi->uHeadPtr = 0;
			m_pBi->uTailPtr = 0;
		} // endi f

		// Empty the circular buffer
		return CCircBuf::Empty();
	}

	//==============================================================
	// GetMaxBuffers()
	//==============================================================
	/// Returns the number of total buffer slots in the fifo
	t_size GetMaxBuffers() { return m_uMaxBuffers; }

	//==============================================================
	// SetMaxBuffers()
	//==============================================================
	/// Sets the total number of buffers slots in the fifo
	/**
		\param [in] x_uMaxBuffers	-	Number of buffers
	*/
//	void SetMaxBuffers( t_size x_uMaxBuffers, t_size x_uBufferSize )
//    {   m_uMaxBuffers = cmn::NextPower2( x_uMaxBuffers ); Allocate( x_uBufferSize ); }

	//==============================================================
	// GetUsedBuffers()
	//==============================================================
	/// Returns the number of buffer slots in the fifo that are in use
	t_size GetUsedBuffers()
	{
        oexAutoLock ll( *this );
	    if ( !ll.IsLocked() )
            return oexFALSE;

        if ( !m_pBi )
            return oexFALSE;

        return GetMaxRead( m_pBi->uTailPtr, m_pBi->uHeadPtr, m_uMaxBuffers );
    }

private:

	/// Maximum fifo buffer size
	t_size				        m_uMaxBuffers;

    /// Buffer information
    SBufferInfo                 *m_pBi;

	/// Buffer information
	SBufferInfo					m_bi;

    /// Pointer to size buffer
    t_size                      *m_pBuf;

	/// Fifo block size values
	TMem< t_size >              m_auSize;
};
