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

#if !defined (ACE_IOSTREAM_H)
#define ACE_IOSTREAM_H

#include "ace/OS.h"
#include <iomanip.h>

#if defined (__GNUC__) && !defined (CHORUS)
#include <String.h>

class QuotedString : public String
{
public:
  inline QuotedString & operator =(char  c) { return (QuotedString&) String::operator=(c); }
  inline QuotedString & operator =(char* c) { return (QuotedString&) String::operator=(c); }
};
#endif /* __GNUC__ */

template <class STREAM>
class ACE_Streambuf : public streambuf
  // = TITLE
  //     Create your custom streambuf by providing and ACE_*_Stream
  //     object to this template.  I have tested it with
  //     ACE_SOCK_Stream and it should work fine for others as well.
  //     I'm hoping to add functionality for ACE_SOCK_Dgram soon...
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
  ACE_Streambuf (STREAM * peer, int io_mode = ios::in | ios::out);
  // We will be given a STREAM by the iostream object which creates
  // us.  See the ACE_IOStream template for how that works.  Like
  // other streambuf objects, we can be input-only, output-only or
  // both.

  virtual ~ACE_Streambuf (void);
  // If the default allocation strategey were used the common buffer
  // would be deleted when the object destructs.  Since we are
  // providing separate read/write buffers, it is up to us to manage
  // their memory.

protected:
  virtual int sync (void);
  // Sync both input and output. See syncin/syncout below for
  // descriptions.

  // = Signatures for the underflow/overflow discussed above.
  virtual int underflow (void);

  virtual int overflow (int = EOF);
  // The overflow function receives the character which caused the
  // overflow.

private:
  STREAM *peer_;
  // This will be our ACE_SOCK_Stream or similar object.

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

  int get_one_byte (void);
  // Used by fillbuf and others to get exactly one byte from the peer.
  // recv_n is used to be sure we block until something is available.
};

// This macro defines the get operator for class MT into datatype DT.
// We will use it below to quickly override most (all?)  iostream get
// operators.  Notice how the ipfx() and isfx() functions are used.

#define ACE_OPERATORG(MT,DT) \
        inline virtual MT& operator>> (DT v) { \
        if (ipfx ()) iostream::operator>> (v); isfx (); return *this; \
        }

// This macro defines the put operator for class MT into datatype DT.
// We will use it below to quickly override most (all?)  iostream put
// operators.  Notice how the opfx() and osfx() functions are used.

#define ACE_OPERATORP(MT,DT) \
        inline virtual MT& operator<< (DT v) { \
        if (opfx ()) iostream::operator<< (v); osfx (); return *this; \
        }

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

// These are necessary in case somebody wants to derive from us and
// override one of these with a custom approach.

#if defined (ACE_LACKS_SIGNED_CHAR)
#define ACE_OPERATORG_SET(MT) \
        ACE_OPERATORG(MT,short &); \
        ACE_OPERATORG(MT,u_short &); \
        ACE_OPERATORG(MT,int &); \
        ACE_OPERATORG(MT,u_int &); \
        ACE_OPERATORG(MT,long &); \
        ACE_OPERATORG(MT,u_long &); \
        ACE_OPERATORG(MT,float &); \
        ACE_OPERATORG(MT,double &); \
        ACE_OPERATORG(MT,long double &); \
        ACE_OPERATORG(MT,char &) \
        ACE_OPERATORG(MT,u_char &); \
        ACE_OPERATORG(MT,char *) \
        ACE_OPERATORG(MT,u_char *); \
        virtual MT& operator>>(__omanip_ func) { (*func)(*this); return *this; } \
        virtual MT& operator>>(__manip_ func)  { (*func)(*this); return *this; }

#define ACE_OPERATORP_SET(MT) \
        ACE_OPERATORP(MT,short); \
        ACE_OPERATORP(MT,u_short); \
        ACE_OPERATORP(MT,int); \
        ACE_OPERATORP(MT,u_int); \
        ACE_OPERATORP(MT,long); \
        ACE_OPERATORP(MT,u_long); \
        ACE_OPERATORP(MT,float); \
        ACE_OPERATORP(MT,double); \
        ACE_OPERATORP(MT,char); \
        ACE_OPERATORP(MT,u_char); \
        ACE_OPERATORP(MT,const char *); \
        ACE_OPERATORP(MT,const u_char *); \
        ACE_OPERATORP(MT,const void *); \
        virtual MT& operator<<(__omanip_ func) { (*func)(*this); return *this; } \
        virtual MT& operator<<(__manip_ func)  { (*func)(*this); return *this; }
#else
#define ACE_OPERATORG_SET(MT) \
        ACE_OPERATORG(MT,short &); \
        ACE_OPERATORG(MT,u_short &); \
        ACE_OPERATORG(MT,int &); \
        ACE_OPERATORG(MT,u_int &); \
        ACE_OPERATORG(MT,long &); \
        ACE_OPERATORG(MT,u_long &); \
        ACE_OPERATORG(MT,float &); \
        ACE_OPERATORG(MT,double &); \
        ACE_OPERATORG(MT,long double &); \
        ACE_OPERATORG(MT,char &) \
        ACE_OPERATORG(MT,u_char &); \
        ACE_OPERATORG(MT,signed char &); \
        ACE_OPERATORG(MT,char *) \
        ACE_OPERATORG(MT,u_char *); \
        ACE_OPERATORG(MT,signed char *); \
        virtual MT& operator>>(__omanip_ func) { (*func)(*this); return *this; } \
        virtual MT& operator>>(__manip_ func)  { (*func)(*this); return *this; }

#define ACE_OPERATORP_SET(MT) \
        ACE_OPERATORP(MT,short); \
        ACE_OPERATORP(MT,u_short); \
        ACE_OPERATORP(MT,int); \
        ACE_OPERATORP(MT,u_int); \
        ACE_OPERATORP(MT,long); \
        ACE_OPERATORP(MT,u_long); \
        ACE_OPERATORP(MT,float); \
        ACE_OPERATORP(MT,double); \
        ACE_OPERATORP(MT,char); \
        ACE_OPERATORP(MT,u_char); \
        ACE_OPERATORP(MT,signed char); \
        ACE_OPERATORP(MT,const char *); \
        ACE_OPERATORP(MT,const u_char *); \
        ACE_OPERATORP(MT,const signed char *); \
        ACE_OPERATORP(MT,const void *); \
        virtual MT& operator<<(__omanip_ func) { (*func)(*this); return *this; } \
        virtual MT& operator<<(__manip_ func)  { (*func)(*this); return *this; }
#endif /* ACE_LACKS_SIGNED_CHAR */

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
  ACE_IOStream (void);
  // The default constructor.  This will initiailze your STREAM and
  // then setup the iostream baseclass to use a custom streambuf based
  // on STREAM.


  virtual ~ACE_IOStream (void);
  // We have to get rid of the streambuf_ ourselves since we gave it
  // to iostream();

  virtual int close (void);
  // The only ambituity in the multiple inheritance is the close()
  // function.

#if defined (__GNUC__)
  virtual ACE_IOStream& operator>>(String & v);
  // A simple string operator.  The base iostream has 'em for char*
  // but that isn't always the best thing for a String.  If we don't
  // provide our own here, we may not get what we want.

  virtual ACE_IOStream& operator>>(QuotedString &v);
  // A more clever operator that handles quoted strings so that we
  // can get strings containing whitespace!

  virtual ACE_IOStream& operator<<(QuotedString &v);
  // The converse of the QuotedString put operator.
#endif /* __GNUG__ */

  // = Using the macros to provide get/set operators.
  ACE_OPERATORG_SET (ACE_IOStream<STREAM>);
  ACE_OPERATORP_SET (ACE_IOStream<STREAM>);

  // = These are handy to have around for overriding.

  // The *pfx functions are called at the beginning of each get/put
  // operator.  The *sfx are called at the end.  In a derivative
  // class, I've overridden the osfx() operator such that a space will
  // be inserted after every put operation so that my transmitted
  // "fields" are always separated.

#if defined (ACE_LACKS_IOSTREAM_FX)
  // These should be faked out to do the right thing, if we knew
  // what that was.  Instead, they're just faked out.
  virtual int ipfx (int need = 0) { ACE_UNUSED_ARG (need); return 1; }
  virtual int ipfx0(void)         { return 1; }  // Optimized ipfx(0)
  virtual int ipfx1(void)         { return 1; }  // Optimized ipfx(1)
  virtual void isfx (void)        { /* null */ }
  virtual int opfx (void)         { return 1; }
  virtual void osfx (void)        { /* null */ }
#else
#if defined (__GNUC__) 
  virtual int ipfx0(void)         { return(iostream::ipfx0()); }  // Optimized ipfx(0)
  virtual int ipfx1(void)         { return(iostream::ipfx1()); }  // Optimized ipfx(1)
#else
  virtual int ipfx0(void)         { return(iostream::ipfx(0)); }
  virtual int ipfx1(void)         { return(iostream::ipfx(1)); }
#endif
  virtual int ipfx (int need = 0) { return(iostream::ipfx(need)); }
  virtual void isfx (void)        { iostream::isfx(); }
  virtual int opfx (void)         { return(iostream::opfx()); }
  virtual void osfx (void)        { iostream::osfx(); }
#endif /* ACE_LACKS_IOSTREAM_FX */

protected:
  ACE_Streambuf<STREAM> *streambuf_;
  // This is where all of the action takes place.  The
  // streambuf_ is the interface to the underlying STREAM.

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

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "IOStream.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* ACE_IOSTREAM_H */
