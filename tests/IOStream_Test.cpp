// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    IOStream_Test.cpp
//
// = DESCRIPTION
//    This is a simple test of the IOStream class that illustrates
//    how to use iostream operations on almost arbitrary I/O classes.
//
// = AUTHOR
//    James CE Johnson <jcej@lads.com>
//
// ============================================================================


#include "ace/Thread.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/IOStream.h"
#include "test_config.h"

typedef ACE_IOStream<ACE_SOCK_Stream> ACE_SOCK_IOStream;

short PORT = ACE_DEFAULT_SERVER_PORT;
u_long error_at_server = 0;
u_long error_at_client = 0;

/* The biggest drawback to an iostream is that it generally
   eats up whitespace when performing a get (>>) operation.

   That may be good if you're reading non-textual data but
   if you're trying to read a stream of words with embedded
   whitespace, it isn't going to be pleasant.

   If you've been blessed with the GNU String class, I've
   already provided a derived class, QuotedString, that
   makes dealing with strings very easy.

   If you're stuck with an array of characters then you
   will probably need somthing like I have below.


   On the other hand, one of the biggest advantages to an
   iostream is that it eats up whitespace :-)

   If you put (<<) your non-textual data to the iostream
   with any number of whitespace between the data then
   you can easily get (>>) the data from the iostream
   without having to worry about delimeters and such.


   The main thing to keep in mind when using an iostream
   between peers is that you MUST keep the data "fields"
   in sync.  That is, if the "putter" puts an int followed
   by a float followed by a double, you must make sure
   that the "getter" will be attempting to get an int
   then a float then a double.
 */

/* Since I can't rely on GNU's String class being everywhere
   (yet), here's a simple class that will work with quoted
   strings.  Use at your own risk!  It is very incomplete!
 */
class qchar
{
private:
	char c_;
public:
	qchar() { c_ = '\0'; }

	qchar(char c) : c_(c) { };

	operator char () const { return(c_); }

	qchar operator=(char c) { return(c_ = c); }

	int operator==(char c) { return(c_ == c); }

	friend ACE_SOCK_IOStream & operator>>(ACE_SOCK_IOStream & stream, qchar * buf);
	friend ACE_SOCK_IOStream & operator<<(ACE_SOCK_IOStream & stream, qchar * buf);
	friend ostream & operator<<(ostream & stream, qchar * buf);
};

/* This is taken almost directly from the QuotedString object that
   has been derived from GNU's String class.  The advantage to
   using QuotedString is that it is MUCH more robust than qchar
   will every be.
   */
ACE_SOCK_IOStream & operator>>(ACE_SOCK_IOStream & stream, qchar * buf)
{
	char c;

	*buf = '\0';	// Initialize the string

	if( !(stream >> c) ) // eat space up to the first char
	{
		return stream;
	}

	// if we don't have a quote, append until we see space
	if (c != '"')
	{
		for( *buf++ = c ; stream.get(c) && !isspace(c) ; *buf++ = c );
	}
	else
	{
		for( ; stream.get(c) && c != '"' ; *buf++ = c )
		{
			if (c == '\\')
			{
				stream.get(c);
				if (c != '"')
					*buf++ = '\\';
			}
		}
	}

	*buf = '\0';
	
	return stream;
}

ACE_SOCK_IOStream & operator<<(ACE_SOCK_IOStream & stream, qchar * buf)
{
	stream.put('"');
	while( *buf )
	{
		if( *buf == '"' )
			stream.put('\\');
		stream.put((char)*buf++);
	}
	stream.put('"');

	return stream;
}

ostream & operator<<(ostream & stream, qchar * buf)
{
	while( *buf )
		stream.put((char)*buf++);

	return stream;
}

/* Our client thread will initiate the test by sending some data
   to the server.
 */
static void *client_thread(void * _thr_mgr)
{
	ACE_NEW_THREAD;

	ACE_Thread_Manager * thr_mgr = (ACE_Thread_Manager *)_thr_mgr;
	ACE_Thread_Control thread_control (thr_mgr);

	ACE_SOCK_IOStream  server;
	ACE_INET_Addr      addr(PORT,ACE_DEFAULT_SERVER_HOST);
	ACE_SOCK_Connector connector;

	if( connector.connect(server,addr) < 0 )
	{
		ACE_DEBUG ((LM_ERROR, "Failed to connect to server thread"));
		return (void *) -1;
	}

	// Send a string to the server which it can interpret as a qchar[]
	char * str = "\"This is a test     string.\"";
	ACE_DEBUG ((LM_DEBUG, "Client Sending:  (%s)\n", str));
	server << str << endl;

	// Allow the server to get the string and echo it to the user.
	// (The iostream doesn't need this, but humans do :)
	//
	ACE_OS::sleep(2);

	// Send another string but this time the server will read it
	// as a char[].  Notice how the server's output doesn't include
	// all of the spaces sent by the client.
	//
	str = "\"THIS IS A     TEST STRING.\"";
	ACE_DEBUG ((LM_DEBUG, "Client Sending:  (%s)\n", str));
	server << str << endl;

	// Again, give the server time to display the happenings to the user.
	ACE_OS::sleep(2);

	// Read from the server an int, float, long, float double.
	// The iostream will pull them out by using the whitespace
	// provided by the server.
	//
	int i;
	float f1, f2;
	long l;
	double d;
	server >> i >> f1 >> l >> f2 >> d;

	ACE_DEBUG ((LM_DEBUG, "Client Received: int %d float %f long %d float %f double %f\n", i, f1, (int) l, f2, d));

        // check for proper received values
	if (i != 1  ||  (f1 < 0.123420 || f1 > 0.123422)  ||
            l != 666555444  ||  (f2 < 23.44 || f2 > 23.46)  ||
            (d < -47.1e+9 || d > -45.9e+9))
          {
            ACE_DEBUG ((LM_ERROR, "incorrect value received, should be:\n"
              "int 1 float 0.123421 long 666555444 float 23.450001 "
              "double -46500000000.00000\n"));
	    ++error_at_client;
          }

	// Reset the precision to limit ourselves
	// to two significant digits.
	server.precision(2);

	// Now, make a little change & send 'em back.
	i  *= -1;	server << i  << " ";
	f1 *= -1;	server << f1 << " ";
	l  *= -1;	server << l  << " ";
	f2 *= -1;	server << f2 << " ";
	d  *= -1;	server << d  << " ";
			server << endl;

	// Shut down the test.
	server.close();

	return 0;
}

/* Test the server's ability to receive data from the client and then begin
   a two-way conversation.
   */
void server_test(ACE_SOCK_IOStream & client)
{
	// Read a qbuf[] from the client.  Notice that all of the
	// client's whitespace is preserved.
	//
	qchar qbuf[1024];
	ACE_OS::memset(qbuf,0,sizeof(qbuf));
	client >> qbuf;
	ACE_DEBUG ((LM_DEBUG, "Server Received: (\"%s\")\n", qbuf));

	// Give the client time to announce the next test to the user
	ACE_OS::sleep(2);

	// Now we try to use a char[] to get a string from the
	// client.  Compared to the method above, this is quite
	// messy.  Notice also that whitespace is lost.
	//
	char buf[1024];
	ACE_OS::memset(buf,0,sizeof(buf));
	ACE_DEBUG ((LM_DEBUG, "Server Received: ("));
	while( buf[strlen(buf)-1] != '"' )
	{
		client >> buf;
		ACE_DEBUG ((LM_DEBUG, "%s ", buf));
	}
	ACE_DEBUG ((LM_DEBUG, ")\n"));

	// Send some non-textual data to the client.
	// We use a single character to separate the fields
	// but we could have used any valid whitespace.
	// The data will be sent if the iostream's buffer
	// gets filled or when we flush it with an explicit
	// client.sync() command or the implicit <<endl.
	//
	ACE_DEBUG ((LM_DEBUG, "Server Sending:  1 .12342134 666555444 23.45 -46.5e9 \n"));
	client << 1 << " ";
	client << .12342134 << " ";
	client << 666555444 << " ";
	client << 23.45 << " ";
	client << -46.5e9 << " ";
	client << endl;

	// The client will have changed the sign of each data field
	// and sent 'em all back to us.  At the same time, the
	// client used the precision() function to change the
	// significant digits for non-integer values.
	//
	int i;
	float f1, f2;
	long l;
	double d;
	client >> i >> f1 >> l >> f2 >> d;

	ACE_DEBUG ((LM_DEBUG, "Server Received: int %d float %g long %d float %g double %g\n", i, f1, (int) l, f2, d));

        // check for proper received values
	if (i != -1  ||  (f1 < -0.13 || f1 > -0.11)  ||
            l != -666555444  ||  (f2 < -24.0 || f2 > -22.0)  ||
            (d < 45e+9 || d > 47e+9))
          {
            ACE_DEBUG ((LM_ERROR, "incorrect value received, should be:\n"
              "int -1 float -0.12 long -666555444 float -23 double 4.6e+10\n"));
	    ++error_at_server;
          }

	return;
}

int
main (int argc, char *argv[])
{
	ACE_START_TEST ("IOStream_Test");

	if( argc > 1 )
		PORT = ACE_OS::atoi(argv[1]);

	ACE_Thread_Manager thr_mgr;
	ACE_INET_Addr addr(PORT);
	ACE_SOCK_Acceptor acceptor(addr);
	ACE_SOCK_IOStream connection;

	if (thr_mgr.spawn(ACE_THR_FUNC (client_thread),&thr_mgr,THR_NEW_LWP | THR_DETACHED) == -1)
	      ACE_ERROR_RETURN((LM_ERROR,"%p\n","spawing client thread"),1);

	if( acceptor.accept(connection) < 0 )
	{
		ACE_DEBUG ((LM_ERROR, "Failed to accept new connection"));
	}

	server_test(connection);

	// Wait for the client thread to exit
	thr_mgr.wait();

	connection.close();
	acceptor.close();

	ACE_END_TEST;
	return error_at_client || error_at_server;
}
