/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    iostream.h
//
// = AUTHOR
//    James CE Johnson <jcej@lads.com>
//
// = COAUTHOR
//    Jim Crossley <jim@lads.com>
//
// ============================================================================

/*
    Changes 4/5/97

    ACE_Streambuf_T
	
	New public functions added

	    char * reset_get_buffer( char * newbuf, u_int bufsiz, u_int inuse )
	    char * reset_put_buffer( char * newbuf, u_int bufsiz, u_int inuse )

		Resets the get/put buffer and returns a pointer to the
		previous buffer.

		If newbuf is not NULL and bufsiz matches the current
		streambuf_size_ value, then the current buffer will
		be replaced by newbuf.  If the stream sizes do not match,
		a NULL pointer is returned and the buffer left unchanged.

		If newbuf is NULL, a new buffer will be allocated from
		the heap.

		This kind of thing is handy if you're chaining streams.
		For instance, I have a stream built on a message queue
		which marshals and sends data from one thread to another.
		The receiving thread then sends that data to a peer()
		via a stream built on a socket.  It is more efficient
		to simply replace the socket stream's put buffer with that
		of the message queue rather than copying the data from
		one to another.

	    u_int streambuf_size( void )

		Get the size of the stream buffers.  Note that the get
		and put buffers are the same size.

	New protected functions added

	    void reset_base( void )

		Resets the base() pointers which toggle between get
		and put mode as well as the cur_mode_ value.

	Constructor changed

	    Initialization of the get/put buffers was moved here from
	    the derived class/template ACE_Streambuf<...>

	    At the same time, the new functions reset_get/put_buffer
	    are now used instead of the previous monolithic function.

    ACE_Streambuf<...>

	Constructor changed:

	    Initialization of the get/put buffers has been moved to the
	    baseclass ACE_Streambuf_T.  This will reduce template bloat
	    as well as giving us more functionality as described above.
 */

#if !defined (ACE_IOSTREAM_H)
#define ACE_IOSTREAM_H

#include "ace/INET_Addr.h"
#include "ace/Handle_Set.h"
#include <iomanip.h>

#if defined (ACE_HAS_STRING_CLASS)
#if defined (ACE_WIN32)
typedef CString ACE_IOStream_String;
#else
#include /**/ <String.h>
typedef String ACE_IOStream_String;
#endif /* ACE_WIN32 */
#endif /* ACE_HAS_STRING_CLASS */

#if defined (ACE_HAS_STRING_CLASS)

#if defined (DIGITAL_UNIX) && defined (DEC_CXX)
#include /**/ <stl_macros>
#endif /* DIGITAL_UNIX && DEC_CXX */

class QuotedString : public ACE_IOStream_String
{
public:
  inline QuotedString (void) { *this = ""; }
  inline QuotedString (const char * c) { *this = ACE_IOStream_String (c); }
  inline QuotedString (const ACE_IOStream_String& s) { *this = s; }
  inline QuotedString& operator= (const ACE_IOStream_String& s) 
  {
    return (QuotedString&) ACE_IOStream_String::operator= (s);
  }
  inline QuotedString & operator = (const char  c) {
    return (QuotedString&) ACE_IOStream_String::operator= (c); 
  }
  inline QuotedString & operator = (const char* c) { 
    return (QuotedString&) ACE_IOStream_String::operator= (c); 
  }
  inline bool operator < (const QuotedString& s) const {
    return *(ACE_IOStream_String *) this < (ACE_IOStream_String) s;
  }
#if defined (ACE_WIN32)
  inline int length (void) { return this->GetLength (); }
#endif /* ACE_WIN32 */
};

#endif /* ACE_HAS_STRING_CLASS */

///////////////////////////////////////////////////////////////////////////

class ACE_Export ACE_Streambuf_T : public streambuf
  // = TITLE
  //     Create your custom streambuf by providing and ACE_*_Stream
  //     object to this template.  I have tested it with
  //     ACE_SOCK_Stream and it should work fine for others as well.
  //
  // = DESCRIPTION
  //     For any iostream object, the real work is done by the
  //     underlying streambuf class.  That is what we create here.
  //
  //     A streambuf has an internal buffer area into which data is
  //     read and written as the iostream requests and provides data.
  //     At some point during the read process, the iostream will
  //     realize that the streambuf has no more data.  The underflow
  //     function of the streambuf is then called.
  //
  //     Likewise, during the write process, the iostream will
  //     eventually notice that the streabuf's buffer has become full
  //     and will invoke the overflow function.
  //
  //     The empty/full state of the read/write "buffers" are
  //     controled by two sets pointers.  One set is dedicated to
  //     read, the other to write.  These pointers, in turn, reference
  //     a common buffer that is to be shared by both read and write
  //     operations.  It is this common buffer to which data is
  //     written and from which it is read.
  //
  //     The common buffer is used by functions of the streambuf as
  //     well as the iostream.  Because of this and the fact that it
  //     is "shared" by both read and write operators, there is a
  //     danger of data corruption if read and write operations are
  //     allowed to take place "at the same time".
  //
  //     To prevent data corruption, we manipulate the read and write
  //     pointer sets so that the streambuf is in either a read-mode
  //     or write-mode at all times and can never be in both modes at
  //     the same time.
  //
  //     In the constructor: set the read and write sets to NULL This
  //     causes the underflow or overflow operators to be invoked at
  //     the first IO activity of the iostream.
  //
  //     In the underflow function we arrange for the common buffer to
  //     reference our read buffer and for the write pointer set to be
  //     disabled.  If a write operation is performed by the iostream
  //     this will cause the overflow function to be invoked.
  //
  //     In the overflow function we arrange for the common buffer to
  //     reference our write buffer and for the read pointer set to be
  //     disabled.  This causes the underflow function to be invoked
  //     when the iostream "changes our mode".
  //
  //     The overflow function will also invoke the send_n function to
  //     flush the buffered data to our peer.  Similarly, the sync and
  //     syncout functions will cause send_n to be invoked to send the
  //     data.
  //
  //     Since socket's and the like do not support seeking, there can
  //     be no method for "syncing" the input.  However, since we
  //     maintain separate read/write buffers, no data is lost by
  //     "syncing" the input.  It simply remains buffered.
{
public:

  virtual ~ACE_Streambuf_T (void);
  // If the default allocation strategey were used the common buffer
  // would be deleted when the object destructs.  Since we are
  // providing separate read/write buffers, it is up to us to manage
  // their memory.

  ACE_Time_Value * recv_timeout( ACE_Time_Value * tv = NULL );
  // Get the current Time_Value pointer and provide a new one.

  char * reset_put_buffer(char * newBuffer = NULL, u_int _streambuf_size = 0, u_int _pptr = 0 );
  // Use this to allocate a new/different buffer for put operations.  If you
  // do not provide a buffer pointer, one will be allocated.  That is the
  // preferred method.  If you do provide a buffer, the size must match that
  // being used by the get buffer.
  // If successful, you will receive a pointer to the current put buffer.
  // It is your responsibility to delete this memory when you are done with it.

  u_int put_avail(void);
  // Return the number of bytes to be 'put' onto the stream media.
  //	pbase + put_avail = pptr

  char * reset_get_buffer(char * newBuffer = NULL, u_int _streambuf_size = 0, u_int _gptr = 0, u_int _egptr = 0 );
  // Use this to allocate a new/different buffer for get operations.  If you
  // do not provide a buffer pointer, one will be allocated.  That is the
  // preferred method.  If you do provide a buffer, the size must match that
  // being used by the put buffer.
  // If successful, you will receive a pointer to the current get buffer.
  // It is your responsibility to delete this memory when you are done with it.

  u_int get_waiting(void);
  // Return the number of bytes not yet gotten.
  //	eback + get_waiting = gptr
  //
  u_int get_avail(void);
  // Return the number of bytes in the get area (includes some already gotten);
  //	eback + get_avail = egptr
  //

  u_int streambuf_size(void);
  // Query the streambuf for the size of it's buffers.

protected:
  ACE_Streambuf_T (u_int streambuf_size, int io_mode);

  virtual int sync (void);
  // Sync both input and output. See syncin/syncout below for
  // descriptions.

  // = Signatures for the underflow/overflow discussed above.
  virtual int underflow (void);

  virtual int overflow (int = EOF);
  // The overflow function receives the character which caused the
  // overflow.

  void reset_base(void);
  // Resets the base() pointer and streambuf mode.  This is
  // used internally when get/put buffers are allocatd.

protected:
  // = Two pointer sets for manipulating the read/write areas.
  char *eback_saved_;
  char *gptr_saved_;
  char *egptr_saved_;
  char *pbase_saved_;
  char *pptr_saved_;
  char *epptr_saved_;

  // = With cur_mode_ we keep track of our current IO mode.

  // This helps us to optimize the underflow/overflow functions.
  u_char cur_mode_;
  const u_char get_mode_;
  const u_char put_mode_;

  int mode_;
  // mode tells us if we're working for an istream, ostream, or
  // iostream.

  const u_int streambuf_size_;
  // This defines the size of the input and output buffers.  It can
  // be set by the object constructor.

  ACE_Time_Value   recv_timeout_value_;
  ACE_Time_Value * recv_timeout_;
  // We want to allow the user to provide Time_Value pointers to
  // prevent infinite blocking while waiting to receive data.

  int syncin (void);
  // syncin is called when the input needs to be synced with the
  // source file.  In a filebuf, this results in the seek() system
  // call being used.  We can't do that on socket-like connections,
  // so this does basically nothing.  That's safe because we have a
  // separate read buffer to maintain the already-read data.  In a
  // filebuf, the single common buffer is used forcing the seek()
  // call.

  int syncout (void);
  // syncout is called when the output needs to be flushed.  This is
  // easily done by calling the peer's send_n function.

  int flushbuf (void);
  // flushbuf is the worker of syncout.  It is a separate function
  // because it gets used sometimes in different context.

  int fillbuf (void);
  // fillbuf is called in a couple of places.  This is the worker of
  // underflow.  It will attempt to fill the read buffer from the
  // peer.

  virtual int get_one_byte ( );
  // Used by fillbuf and others to get exactly one byte from the peer.
  // recv_n is used to be sure we block until something is available.
  // It is virtual because we really need to override it for
  // datagram-derived objects.

  virtual ssize_t send  ( char * buf, ssize_t len ) = 0;
  virtual ssize_t recv  ( char * buf, ssize_t len, ACE_Time_Value * tv = NULL ) = 0;
  virtual ssize_t recv  ( char * buf, ssize_t len, int flags, ACE_Time_Value * tv = NULL ) = 0;
  virtual ssize_t recv_n( char * buf, ssize_t len, int flags = 0, ACE_Time_Value * tv = NULL ) = 0;
  // Stream connections and "unconnected connections" (ie -- datagrams)
  // need to work just a little differently.  We derive custom
  // Streambuf objects for them and provide these functions at that time.

  virtual ACE_HANDLE get_handle(void)
  {
	return 0;
  }
};

///////////////////////////////////////////////////////////////////////////

template <class STREAM>
class ACE_Streambuf : public ACE_Streambuf_T
{
public:
  ACE_Streambuf (STREAM * peer, u_int streambuf_size = ACE_STREAMBUF_SIZE, int io_mode = ios::in | ios::out);
  // We will be given a STREAM by the iostream object which creates
  // us.  See the ACE_IOStream template for how that works.  Like
  // other streambuf objects, we can be input-only, output-only or
  // both.

  virtual ssize_t send  ( char * buf, ssize_t len )
  {
	return peer_->send_n(buf,len);
  }
  virtual ssize_t recv  ( char * buf, ssize_t len, ACE_Time_Value * tv = NULL )
  {
	return this->recv( buf, len, 0, tv );
  }
  virtual ssize_t recv  ( char * buf, ssize_t len, int flags, ACE_Time_Value * tv = NULL )
  {
	ssize_t rval = peer_->recv(buf,len,flags,tv);
	return rval;
  }
  virtual ssize_t recv_n( char * buf, ssize_t len, int flags = 0, ACE_Time_Value * tv = NULL )
  {
	ssize_t rval =  peer_->recv_n(buf,len,flags,tv);
	return rval;
  }

protected:
  STREAM * peer_;
  // This will be our ACE_SOCK_Stream or similar object.

  virtual ACE_HANDLE get_handle(void)
  {
  	return peer_ ? peer_->get_handle() : 0 ;
  }
};

///////////////////////////////////////////////////////////////////////////

// These typedefs are provided by G++ (on some systems?) without the
// trailing '_'.  Since we can't count on 'em, I've defined them to
// what GNU wants here.
//
typedef ios& (*__manip_)(ios&);
typedef istream& (*__imanip_)(istream&);
typedef ostream& (*__omanip_)(ostream&);

// Trying to do something like is shown below instead of using the
// __*manip typedefs causes Linux do segfault when "<<endl" is done.
//
//        virtual MT& operator<<(ios& (*func)(ios&))  { (*func)(*this); return *this; }



// This macro defines the get operator for class MT into datatype DT.
// We will use it below to quickly override most (all?)  iostream get
// operators.  Notice how the ipfx() and isfx() functions are used.

#define GET_SIG(MT,DT)		inline virtual MT& operator>> (DT v)
#define GET_CODE { 			\
	if (ipfx (0))					\
	{						\
		iostream::operator>> (v);		\
	}						\
	isfx ();					\
	return *this;					\
	}
#define GET_PROT(MT,DT,CODE)	GET_SIG(MT,DT)	CODE
#define GET_FUNC(MT,DT)		GET_PROT(MT,DT,GET_CODE)

// This macro defines the put operator for class MT into datatype DT.
// We will use it below to quickly override most (all?)  iostream put
// operators.  Notice how the opfx() and osfx() functions are used.

#define PUT_SIG(MT,DT)		inline virtual MT& operator<< (DT v)
#define PUT_CODE {			\
	if (opfx ())					\
	{						\
		iostream::operator<< (v);		\
	}						\
	osfx ();					\
	return *this;					\
	}
#define PUT_PROT(MT,DT,CODE)	PUT_SIG(MT,DT)	CODE
#define PUT_FUNC(MT,DT)		PUT_PROT(MT,DT,PUT_CODE)


// These are necessary in case somebody wants to derive from us and
// override one of these with a custom approach.

#define GET_FUNC_SET0(MT,CODE,CODE2) \
        GET_PROT(MT,short &,CODE) \
        GET_PROT(MT,u_short &,CODE) \
        GET_PROT(MT,int &,CODE) \
        GET_PROT(MT,u_int &,CODE) \
        GET_PROT(MT,long &,CODE) \
        GET_PROT(MT,u_long &,CODE) \
        GET_PROT(MT,float &,CODE) \
        GET_PROT(MT,double &,CODE) \
        GET_PROT(MT,char &,CODE) \
        GET_PROT(MT,u_char &,CODE) \
        GET_PROT(MT,char *,CODE) \
        GET_PROT(MT,u_char *,CODE) \
        inline virtual MT& operator>>(__omanip_ func) CODE2 \
        inline virtual MT& operator>>(__manip_ func)  CODE2

#define PUT_FUNC_SET0(MT,CODE,CODE2) \
        PUT_PROT(MT,short,CODE) \
        PUT_PROT(MT,u_short,CODE) \
        PUT_PROT(MT,int,CODE) \
        PUT_PROT(MT,u_int,CODE) \
        PUT_PROT(MT,long,CODE) \
        PUT_PROT(MT,u_long,CODE) \
        PUT_PROT(MT,float,CODE) \
        PUT_PROT(MT,double,CODE) \
        PUT_PROT(MT,char,CODE) \
        PUT_PROT(MT,u_char,CODE) \
        PUT_PROT(MT,const char *,CODE) \
        PUT_PROT(MT,const u_char *,CODE) \
        PUT_PROT(MT,const void *,CODE) \
        inline virtual MT& operator<<(__omanip_ func) CODE2 \
        inline virtual MT& operator<<(__manip_ func)  CODE2


#if defined (ACE_LACKS_SIGNED_CHAR)
  #define GET_FUNC_SET1(MT,CODE,CODE2) GET_FUNC_SET0(MT,CODE,CODE2)
  #define PUT_FUNC_SET1(MT,CODE,CODE2) PUT_FUNC_SET0(MT,CODE,CODE2)
#else
  #define GET_FUNC_SET1(MT,CODE,CODE2) \
          GET_PROT(MT,signed char &,CODE) \
          GET_PROT(MT,signed char *,CODE) \
          GET_FUNC_SET0(MT,CODE,CODE2)

  #define PUT_FUNC_SET1(MT,CODE,CODE2) \
          PUT_FUNC(MT,signed char) \
          PUT_FUNC(MT,const signed char *) \
          PUT_FUNC_SET0(MT,CODE,CODE2)
#endif /* ACE_LACKS_SIGNED_CHAR */

#define GET_MANIP_CODE	{ if( ipfx() ) { (*func)(*this); } isfx(); return *this; }
#define PUT_MANIP_CODE	{ if( opfx() ) { (*func)(*this); } osfx(); return *this; }

#define GET_FUNC_SET(MT)	GET_FUNC_SET1(MT,GET_CODE,GET_MANIP_CODE)
#define PUT_FUNC_SET(MT)	PUT_FUNC_SET1(MT,PUT_CODE,PUT_MANIP_CODE)
#define	GETPUT_FUNC_SET(MT)	GET_FUNC_SET(MT) PUT_FUNC_SET(MT)


#define GET_SIG_SET(MT)		GET_FUNC_SET1(MT,= 0;,= 0;)
#define PUT_SIG_SET(MT)		PUT_FUNC_SET1(MT,= 0;,= 0;)
#define	GETPUT_SIG_SET(MT)	GET_SIG_SET(MT) PUT_SIG_SET(MT)


///////////////////////////////////////////////////////////////////////////

template <class STREAM>
class ACE_IOStream : public iostream, public STREAM
  // = TITLE
  //     A template adapter for creating an iostream-like object using
  //     an ACE IPC Stream for the actual I/O.  Iostreams use an
  //     underlying streambuf object for the IO interface.  The
  //     iostream class and derivatives provide you with a host of
  //     convenient operators that access the streambuf.
  //
  // = DESCRIPTION
  //     We inherit all characteristics of iostream and your <STREAM>
  //     class.  When you create a new class from this template, you
  //     can use it anywhere you would have used your original
  //     <STREAM> class.
  //
  //     To create an iostream for your favorite ACE IPC class (e.g.,
  //     <ACE_SOCK_Stream>), feed that class to this template's
  //     <STREAM> parameter, e.g.,
  //
  //     typedef ACE_Svc_Handler<ACE_SOCK_iostream,
  //                             ACE_INET_Addr, ACE_NULL_SYNCH>
  //             Service_Handler;
  //
  //     Because the operators in the iostream class are not virtual,
  //     you cannot easily provide overloads in your custom
  //     ACE_IOStream classes.  To make these things work correctly,
  //     you need to overload ALL operators of the ACE_IOStream you
  //     create. I've attempted to do that here to make things easier
  //     for you but there are no guarantees.
  //
  //     In the iostream.cpp file is an example of why it is necessary
  //     to overload all of the get/put operators when you want to
  //     customize only one or two.
{
public:
  ACE_IOStream (STREAM & stream, u_int streambuf_size = ACE_STREAMBUF_SIZE);
  ACE_IOStream (u_int streambuf_size = ACE_STREAMBUF_SIZE);
  // The default constructor.  This will initiailze your STREAM and
  // then setup the iostream baseclass to use a custom streambuf based
  // on STREAM.

  virtual ~ACE_IOStream (void);
  // We have to get rid of the streambuf_ ourselves since we gave it
  // to iostream();

  virtual int close (void);
  // The only ambituity in the multiple inheritance is the close()
  // function.

#if defined (ACE_HAS_STRING_CLASS)
  virtual ACE_IOStream<STREAM> & operator>>(ACE_IOStream_String & v);
  // A simple string operator.  The base iostream has 'em for char*
  // but that isn't always the best thing for a String.  If we don't
  // provide our own here, we may not get what we want.

  virtual ACE_IOStream<STREAM> & operator<<(ACE_IOStream_String & v);
  // The converse of the String put operator.

  virtual ACE_IOStream<STREAM> & operator>>(QuotedString &v);
  // A more clever operator that handles quoted strings so that we
  // can get strings containing whitespace!

  virtual ACE_IOStream<STREAM> & operator<<(QuotedString &v);
  // The converse of the QuotedString put operator.
#endif /* ACE_HAS_STRING_CLASS */

  // = Using the macros to provide get/set operators.
#if !defined (ACE_LACKS_IOSTREAM_SETGET)
  GETPUT_FUNC_SET (ACE_IOStream<STREAM>)
#endif /* ACE_LACKS_IOSTREAM_SETSET */

#if defined (ACE_LACKS_IOSTREAM_FX)
  virtual int ipfx (int need = 0) {  return good(); }
  virtual int ipfx0(void)         {  return good(); }  // Optimized ipfx(0)
  virtual int ipfx1(void)         {  return good(); }  // Optimized ipfx(1)
  virtual void isfx (void)        {  return; }
  virtual int opfx (void)         {  return good(); }
  virtual void osfx (void)        {  put(' '); return; }
#else
#if defined (__GNUC__) 
  virtual int ipfx0(void)         { return(iostream::ipfx0()); }  // Optimized ipfx(0)
  virtual int ipfx1(void)         { return(iostream::ipfx1()); }  // Optimized ipfx(1)
#else
  virtual int ipfx0(void)         {  return(iostream::ipfx(0)); }
  virtual int ipfx1(void)         {  return(iostream::ipfx(1)); }
#endif
  virtual int ipfx (int need = 0) {  return(iostream::ipfx(need)); }
  virtual void isfx (void)        {  iostream::isfx(); return; }
  virtual int opfx (void)         {  return(iostream::opfx()); }
  virtual void osfx (void)        {  iostream::osfx(); return; }
#endif /* ACE_LACKS_IOSTREAM_FX */

  ACE_IOStream<STREAM> & operator>>(ACE_Time_Value *&tv);
  // Allow the programmer to provide a timeout for read operations.
  // Give it a pointer to NULL to block forever.

protected:

  ACE_Streambuf<STREAM> *streambuf_;
  // This is where all of the action takes place.  The
  // streambuf_ is the interface to the underlying STREAM.

private:
  // We move these into the private section so that they cannot
  // be used by the application programmer.  This is necessary
  // because streambuf_ will be buffering IO on the STREAM
  // object.  If these functions were used in your program,
  // there is a danger of getting the datastream out of sync.
  ssize_t send (...);
  ssize_t recv (...);
  ssize_t send_n (...);
  ssize_t recv_n (...);
};

///////////////////////////////////////////////////////////////////////////

template <class STREAM>
class ACE_SOCK_Dgram_SC : public STREAM
// Datagrams don't have the notion of a "peer".  Each send and receive
// on a datagram can go to a different peer if you want.  If you're
// using datagrams for stream activity, you probably want 'em all to
// go to (and come from) the same place.  That's what this class is
// for.  BTW: 'Dgram_SC' is short for 'Datagram-Self-Contained'.
// Here, we keep an address object so that we can remember who last
// sent us data.  When we write back, we're then able to write back to
// that same address.
{
protected:
  ACE_INET_Addr peer_;

public:
  ACE_SOCK_Dgram_SC (void)
    {
    }

  ACE_SOCK_Dgram_SC (STREAM &source, ACE_INET_Addr &dest)
    : STREAM (source), peer_ (dest)
    {
    }

  inline ssize_t send_n (char *buf, ssize_t len)
    {
      return STREAM::send (buf, len, peer_);
    }

  inline ssize_t recv  (char * buf, ssize_t len, ACE_Time_Value * tv = NULL)
    {
      return recv (buf, len, 0, tv);
    }

  inline ssize_t recv (char * buf, ssize_t len, int flags, ACE_Time_Value * tv = NULL)
    {
      if (tv != 0)
	{
	  ACE_HANDLE handle = this->get_handle ();
	  ACE_Handle_Set handle_set;
		
	  handle_set.set_bit (handle);
		
	  switch (ACE_OS::select (int (handle) + 1,
				  (fd_set *) handle_set, // read_fds.
				  (fd_set *) 0,          // write_fds.
				  (fd_set *) 0,          // exception_fds.
				  tv))
	    {
	    case 0:
	      errno = ETIME;
	    case -1:
	      return -1;
	    default:
	      ;	// Do the 'recv' below
	    }
	}

      int rval = STREAM::recv (buf, len, peer_, flags);
#ifdef WIN32
      if (rval == SOCKET_ERROR)
	if (WSAGetLastError() == WSAEMSGSIZE)
	  if (flags & MSG_PEEK)
	    rval = len;
#endif
	
      return rval < len ? rval : len;
    }
  inline ssize_t recv_n (char * buf,
			 ssize_t len, int flags = 0, 
			 ACE_Time_Value * tv = NULL)    
    {
      int rval = this->recv (buf, len, flags, tv);
      return rval;
    }

  inline int get_remote_addr (ACE_INET_Addr &addr) const
    {
      addr = peer_;
      return 0;
    }

};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#  if ! defined( ACE_IOSTREAM_C )
#    define ACE_IOSTREAM_BUILDING_TEMPLATE
#  endif
#  include "IOStream.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* ACE_IOSTREAM_H */

