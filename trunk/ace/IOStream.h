/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    IOStream.h
//
// = AUTHOR
//    James CE Johnson <jcej@lads.com>
//
// = COAUTHOR
//    Jim Crossley <jim@lads.com>
//
// ============================================================================


#if !defined (ACE_IOSTREAM_H)
#define ACE_IOSTREAM_H

#if !defined (ACE_LACKS_ACE_IOSTREAM)

#include "ace/INET_Addr.h"
#include "ace/Handle_Set.h"

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

class ACE_Export ACE_Streambuf : public streambuf
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

  virtual ~ACE_Streambuf (void);
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
  ACE_Streambuf (u_int streambuf_size, int io_mode);

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
        PUT_PROT(MT,u_char *,CODE) \
        PUT_PROT(MT,void *,CODE) \
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

// Include the templates here.
#include "ace/IOStream_T.h"

#endif /* !ACE_LACKS_ACE_IOSTREAM */

#endif /* ACE_IOSTREAM_H */
