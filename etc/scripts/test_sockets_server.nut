
class CGlobal
{
	server = CSqSocket();
};

local _g = CGlobal();

function WaitKey()
{	_self.echo( "\n...press any key...\n" );
	_self.get_key();
}

function _init() : ( _g )
{
	_self.echo( "\n... Starting server ...\n" );

	if ( !_g.server.Bind( 9876 ) )
	{	_self.echo( "Bind() : " + _g.server.getLastError() );
		WaitKey();
		return;
	} // end if

	if ( !_g.server.Listen( 0 ) )
	{	_self.echo( "Listen() : " + _g.server.getLastError() );
		WaitKey();
		return;
	} // end if

	_self.echo( "server listening on : http://localhost:9876" ); 

/*
	while( 1 )
	{

	// Wait for connect
	if ( !_g.server.WaitEvent( CSqSocket().EVT_ACCEPT, 30000 ) )
	{	_self.echo( "WaitEvent( EVT_ACCEPT ) : " + _g.server.getLastError() );
		WaitKey();
		return;
	} // end if

	{ // Handle session inline

		local session = CSqSocket();
		if ( !_g.server.Accept( session ) )
		{	_self.echo( "Accept() : " + _g.server.getLastError() );
			WaitKey();
			return;
		} // end if

		_self.echo( "\n... Writing data to synchronous socket..." );

		local data = "Synchronous!";
		local write = "HTTP/1.1 200\r\nContent-Length: " + data.len() + "\r\n\r\n" + data;
		if ( !session.Write( write, 0 ) )
		{	_self.echo( "Write() : " + session.getLastError() );
			WaitKey();
			return;
		} // end if
		_self.echo( write );
	}
	}
*/
/*
	while ( 1 )
	{

	// Wait for connect
	if ( !_g.server.WaitEvent( CSqSocket().EVT_ACCEPT, 30000 ) )
	{	_self.echo( "WaitEvent( EVT_ACCEPT ) : " + _g.server.getLastError() );
		WaitKey();
		return;
	} // end if

	{ // Handle session in own thread

		local session = CSqSocket();

		// Accept the session
		if ( !_g.server.Accept( session ) )
		{	_self.echo( "Accept() : " + _g.server.getLastError() );
			WaitKey();
			return;
		} // end if

		// Set the script that will handle the session
		_self.echo( "... Spawning asynchronous session..." );
		session.setScript( _self.queue(), "session", "test_sockets_session.nut", "SetSocket" );

		// Wait for session to handle connection
//		session.WaitEvent( CSqSocket().EVT_CLOSE, 30000 );

//		_self.sleep( 1000 );

	}

	}
	_self.echo( "\n...done...\n" );
*/
}

function _idle() : ( _g )
{
	// Wait for connect request
	while ( _g.server.WaitEvent( CSqSocket().EVT_ACCEPT, 1000 ) )
	{
		// Create a new socket to handle this session
		local session = CSqSocket();

		// Accept the session
		if ( !_g.server.Accept( session ) )
		{	_self.echo( "Accept() : " + _g.server.getLastError() );
			return 0;
		} // end if

		// Show connection info
		local addr = CSqSockAddress();
		_g.server.getPeerAddress( addr );
		_self.echo( "Connection from : " + addr.getDotAddress() );

		// Set the script that will handle the session
		session.setScript( _self.queue(), "session", "test_sockets_session.nut", "SetSocket" );

		// Script will take ownership of the session now...

	} // end while

	return 0;
}

