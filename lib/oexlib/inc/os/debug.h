/*------------------------------------------------------------------
// debug.h
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
// CDebug
//
/// Os independent debugging functions
/**
	This class supplies basic debugging functionality.

*/
//==================================================================
class CDebug
{
private:
	CDebug() {}
	virtual ~CDebug() {}

public:

	//==============================================================
	// Trace()
	//==============================================================
	///	Outputs a string into the debugger
	/**
		\param [in] x_pStr	-	String to output
	*/
	static void Trace( oexCSTR x_pStr );

	//==============================================================
	// FmtTrace()
	//==============================================================
	/// Outputs a formated string into the debugger
	/**
		\param [in] x_pFmt	-	Format string
		\param [in] ...		-	Optional arguments, see sprintf() for
								details.
	*/
	static void FmtTrace( oexCSTR x_pFmt, ... );

	//==============================================================
	// vFmtTrace()
	//==============================================================
	/// Outputs a formated string into the debugger
	/**
		\param [in] x_pFmt	-	Format string
		\param [in] x_pArgs	-	Optional arguments, see sprintf() for
								details.
	*/
	static void vFmtTrace( oexCSTR x_pFmt, oexVaList &x_pArgs );

	//==============================================================
	// Break()
	//==============================================================
	/// Stops program execution and invokes the debugger
	static void Break();

	//==============================================================
	// Break()
	//==============================================================
	/// Stops program execution and invokes the debugger
	/**
		\param [in] x_nType		-	The type of break
		\param [in] x_pFile		-	File generating the break
		\param [in] x_uLine		-	File line number
		\param [in] x_pFunction	-	Function generating the break
		\param [in] x_pStr		-	String describing the operation
									that caused the break.
		\param [in] x_nRes		-	Result of the espression causing
									the error code.
		\param [in] x_nErr		-	Related error code.
	*/
	static void Break( oexINT x_nType, oexCSTR x_pFile, oexUINT x_uLine, oexCSTR8 x_pFunction, oexCSTR x_pStr, oexINT x_nRes = 0, oexINT x_nErr = 0 );

	//==============================================================
	// Break()
	//==============================================================
	/// Stops program execution and invokes the debugger
	/**
		\param [in] x_nType		-	The type of break
		\param [in] x_pFile		-	File generating the break
		\param [in] x_uLine		-	File line number
		\param [in] x_pFunction	-	Function generating the break
		\param [in] x_pModule	-	The module name that caused the
									break.
		\param [in] x_pStr		-	String describing the operation
									that caused the break.
		\param [in] x_nRes		-	Result of the espression causing
									the error code.
		\param [in] x_nErr		-	Related error code.

		\return

		\see
	*/
	static void Break( oexINT x_nType, oexCSTR x_pFile, oexUINT x_uLine, oexCSTR8 x_pFunction, oexCSTR x_pModule, oexCSTR x_pStr, oexINT x_nRes = 0, oexINT x_nErr = 0 );

	//==============================================================
	// EnableCrashReporting()
	//==============================================================
	/// Enable crash reporting
	/**
		\param [in]	pUrl	- URL to post crash info 
		\param [in] pSub	- Sub folder in which to save crash reports

		\see
	*/
	static void EnableCrashReporting( oexCSTR pUrl, oexCSTR pSub );

	//==============================================================
	// CreateCrashReport()
	//==============================================================
	/// Creates a crash report
	/**
		\param [in]	pUrl	- URL to post crash info 
		\param [in] pSub	- Sub folder in which to save crash reports
		\param [in] pEInfo	- Error description

		\see
	*/
	static void CreateCrashReport( oexCSTR pUrl, oexCSTR pSub, oexCSTR pEInfo );


	//==============================================================
	// GetExceptionCodeName()
	//==============================================================
	/// Returns the name for the exception code
	/**
		\param [in]	x_uCode	- Exception code

		\see
	*/
	static oexCSTR GetExceptionCodeName( oexUINT x_uCode );

	//==============================================================
	// GetExceptionCodeDesc()
	//==============================================================
	/// Returns the description for the exception code
	/**
		\param [in]	x_uCode	- Exception code

		\see
	*/
	static oexCSTR GetExceptionCodeDesc( oexUINT x_uCode );


};
