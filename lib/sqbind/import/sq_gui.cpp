/*------------------------------------------------------------------
// sq_gui.cpp
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


_SQBIND_REGISTER_CLASS_BEGIN( sqbind::CSqGui, CSqGui )
	_SQBIND_MEMBER_FUNCTION(  sqbind::CSqGui, get_cursor_pos )
	_SQBIND_MEMBER_FUNCTION(  sqbind::CSqGui, set_cursor_pos )
//	_SQBIND_MEMBER_FUNCTION(  sqbind::CSqGui,  )
//	_SQBIND_MEMBER_FUNCTION(  sqbind::CSqGui,  )
//	_SQBIND_MEMBER_FUNCTION(  sqbind::CSqGui,  )
//	_SQBIND_MEMBER_FUNCTION(  sqbind::CSqGui,  )
//	_SQBIND_MEMBER_FUNCTION(  sqbind::CSqGui,  )
//	_SQBIND_MEMBER_FUNCTION(  sqbind::CSqGui,  )
_SQBIND_REGISTER_CLASS_END()

void CSqGui::Register( sqbind::VM vm )
{_STT();
	_SQBIND_EXPORT( vm, CSqGui );
}

sqbind::CSqPos CSqGui::get_cursor_pos()
{_STT();
	return oexGui_GetCursorPos();
}

int CSqGui::set_cursor_pos( sqbind::CSqPos *p )
{_STT();
	if ( !p ) 
		return 0;
	return oexGui_SetCursorPos( oexPoint( p->getX(), p->getY() ) );
}