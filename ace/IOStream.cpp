// IOStream.cpp
// $Id$

#ifndef ACE_IOSTREAM_C
#define ACE_IOSTREAM_C

#define ACE_BUILD_DLL
#include "ace/IOStream.h"
#include "ace/Thread.h"
#include "ace/Handle_Set.h"

///////////////////////////////////////////////////////////////////////////

/* Here's a simple example of how iostream's non-virtual operators can
   get you in a mess:


  class myiostream : public iostream
  {
  public:
          myiostream& operator>> (String & s)
          {
                  ...
          }
  };

  ...

  int i;
  String s;
  myiostream foo (...);

  foo >> s;
  // OK
  // invokes myiostream::operator>> (String&) returning myiostream&

  foo >> i;
  // OK
  // invokes iostream::operator>> (int&) returning iostream&

  foo >> i >> s;
  // BAD
  // invokes iostream::operator>> (int&) then iostream::operator>> (String&)
  //
  // What has happened is that the first >> is invoked on the base class and returns
  // a reference to iostream.  The second >> has no idea of the ACE_IOStream and
  // gets invoked on iostream.  Probably NOT what you wanted!


  // In order to make all of this work the way you want, you have to do this:

  class myiostream : public iostream
  {
  public:
          myiostream& operator>> (int & i)
          {
                  return ((myiostream&)iostream::operator>> (i));
          }

          myiostream& operator>> (String & s)
          {
                  ...
          }
  };

  ...

  int i;
  String s;
  myiostream foo (...);

  foo >> s;
  // OK
  // invokes myiostream::operator>> (String&) returning myiostream&

  foo >> i;
  // OK
  // invokes myiostream::operator>> (int&) returning myiostream&


  foo >> i >> s;
  // OK
  // Because you provided operator>> (int&) in class myiostream, that
  // function will be invoked by the first >>.  Since it returns
  // a myiostream&, the second >> will be invoked as desired.  */


#if ! defined (ACE_IOSTREAM_BUILDING_TEMPLATE)

ACE_Time_Value * 
ACE_Streambuf_T::recv_timeout (ACE_Time_Value * tv)
{
  ACE_Time_Value * rval = recv_timeout_;
  if (tv)
    {
      recv_timeout_value_ = *tv;
      recv_timeout_ = &recv_timeout_value_;
    }
  else
    recv_timeout_ = NULL;

  return rval;
}

int
ACE_Streambuf_T::underflow (void)
{
  // If input mode is not set, any attempt to read from the stream is
  // a failure.

  if (! (mode_ & ios::in))
    return EOF;

  // If base () is empty then this is the first time any get/put
  // operation has been attempted on the stream.

  if (!this->base ())
    {
      // Set base () to use our private read buffer.  The arguments are:
      //        beginning of the buffer (base ())
      //        one-beyond the end of the buffer (ebase ())
      //        should base () be deleted on destruction
      //
      // We have to say "no" to the third parameter because we want to
      // explicitly handle deletion of the TWO buffers at destruction.
      //
      setb (this->eback_saved_,
            this->eback_saved_ + streambuf_size_, 0);

      // Using the new values for base (), initialize the get area.
      // This simply sets eback (), gptr () and egptr () described
      // earlier.
      setg (base (), base (), base ());

      // Set the put buffer such that puts will be disabled.  Any
      // attempt to put data will now cause overflow to be invoked.
      setp (0, 0);

      // Remember that we are now in getMode.  This will help us if
      // we're called prior to a mode change as well as helping us
      // when the mode does change.
      this->cur_mode_ = this->get_mode_;
    }
  else  // base () has been initialized already...
    {
      // If we are in put_mode_ now, then it is time to switch to get_mode_
      //
      // 1. get rid of any pending output
      // 2. rearrange base () to use our half of the buffer
      // 3. reset the mode
      //
      if (this->cur_mode_ == this->put_mode_)
        {
          // Dump any pending output to the peer.  This is not
          // really necessary because of the dual-buffer arrangement
          // we've set up but intuitively it makes sense to send
          // the pending data before we request data since the peer
          // will probably need what we're sending before it can
          // respond.
          if (out_waiting () && syncout () == EOF)
            return EOF;

          // We're about to disable put mode but before we do that, we
          // wan't to preserve it's state.
          this->pbase_saved_ = pbase ();
          this->pptr_saved_  = pptr ();
          this->epptr_saved_ = epptr ();
          //
          // Disable put mode as described in the constructor.
          //
          setp (0, 0);

          // Like the case where base () is false, we now point base ()
          // to use our private get buffer.

          setb (this->eback_saved_,
                this->eback_saved_ + streambuf_size_, 0);

          // And restore the previous state of the get pointers.

          setg (this->eback_saved_, this->gptr_saved_,
                this->egptr_saved_);

          // Finally, set our mode so that we don't get back into this
          // if () and so that overflow can operate correctly.
          cur_mode_ = get_mode_;
        }

      // There could be data in the input buffer if we switched to put
      // mode before reading everything.  In that case, we take this
      // opportunity to feed it back to the iostream.
      if (in_avail ())
        // Remember that we return an int so that we can give
        // back EOF.  The explicit cast prevents us from
        // returning a signed char when we're not returning EOF.
          return (u_char) *gptr ();
    }

  // We really shouldn't be here unless there is a lack of data in the
  // read buffer.  So... go get some more data from the peer.

  int result = fillbuf ();

  // fillbuf will give us EOF if there was an error with the peer.  In
  // that case, we can do no more input.

  if (EOF == result)
    {
      // Disable ourselves and return failure to the iostream.  That
      // should result in a call to have oursleves closed.
      setg (0, 0, 0);
      return EOF;
    }

  // Return the next available character in the input buffer.  Again,
  // we protect against sign extension.

  return (u_char) *gptr ();
}

// Much of this is similar to underflow.  I'll just hit the highlights
// rather than repeating a lot of what you've already seen.

int
ACE_Streambuf_T::overflow (int c)
{
  // Check to see if output is allowed at all.
  if (! (mode_ & ios::out))
    return EOF;

  // First invokation of a get or put function
  if (!base ())
    {
      // Set base () to use put's private buffer.
      //
      setb (this->pbase_saved_,
            this->pbase_saved_ + streambuf_size_, 0);

      // Set the put area using the new base () values.
      setp (base (), ebuf ());

      // Disable the get area.
      setg (0, 0, 0);

      // Set the mode for optimization.
      this->cur_mode_ = this->put_mode_;
    }
  else  // We're already reading or writting
    {
      // If we're coming out of get mode...
      if (this->cur_mode_ == this->get_mode_)
        {
          // Save the current get mode values
          this->eback_saved_ = eback ();
          this->gptr_saved_  = gptr ();
          this->egptr_saved_ = egptr ();

          // then disable the get buffer
          setg (0, 0, 0);

          // Reconfigure base () and restore the put pointers.
          setb (pbase_saved_, pbase_saved_ + streambuf_size_, 0);
          setp (base (), ebuf ());

          // Save the new mode.
          this->cur_mode_ = this->put_mode_;
        }

      // If there is output to be flushed, do so now.  We shouldn't
      // get here unless this is the case...

      if (out_waiting () && EOF == syncout ())
        return EOF;
    }

  // If we're not putting EOF, then we have to deal with the character
  // that is being put.  Perhaps we should do something special with EOF???

  if (c != EOF)
    {
      // We've already written any data that may have been in the
      // buffer, so we're guaranteed to have room in the buffer for
      // this new information.  So... we add it to the buffer and
      // adjust our 'next' pointer acordingly.
      *pptr () = c;
      pbump (1);
    }

  return 0;
}

// syncin

int
ACE_Streambuf_T::syncin (void)
{
  // As discussed, there really isn't any way to sync input from a socket-like
  // device.  We specifially override this base-class function so that it won't
  // do anything evil to us.
  return 0;
}

// syncout

int
ACE_Streambuf_T::syncout (void)
{
  // Unlike syncin, syncout is a doable thing.  All we have to do is
  // write whatever is in the output buffer to the peer.  flushbuf ()
  // is how we do it.

  if (flushbuf () == EOF)
    return EOF;
  else
    return 0;
}

int
ACE_Streambuf_T::sync (void)
{
  // sync () is fairly traditional in that it syncs both input and output.
  // We could have omitted the call to syncin () but someday, we may want it
  // to do something.

  syncin ();

  // Don't bother syncing the output unless there is data to be sent...

  if (out_waiting ())
    return syncout ();

  return 0;
}

// flushbuf

int
ACE_Streambuf_T::flushbuf (void)
{
  // pptr () is one character beyond the last character put
  // into the buffer.  pbase () points to the beginning of
  // the put buffer.  Unless pptr () is greater than pbase ()
  // there is nothing to be sent to the peer.
  //
  if (pptr () <= pbase ())
	return 0;

  // 4/12/97 -- JCEJ
  // Kludge!!!
  // If the remote side shuts down the connection, an attempt to
  // send () to the remote will result in the message 'Broken Pipe'
  // I think this is an OS message, I've tracked it down to the
  // ACE_OS::write () function.  That's the last one to be called
  // before the message.  I can only test this on Linux though, so
  // I don't know how other systems will react.
  //
  // To get around this gracefully, I do a PEEK recv () with an
  // immediate (nearly) timeout.  recv () is much more graceful
  // on it's failure.  If we get -1 from recv () not due to timeout
  // then we know we're SOL.
  //
  // Q:  Is 'errno' threadsafe?  Should the section below be a
  //     critical section?
  //
  //
  // char tbuf[1];
  // ACE_Time_Value to (0,1);
  // if (this->recv (tbuf, 1, MSG_PEEK, &to) == -1)
  // {
  // 	if (errno != ETIME)
  //	{
  //		perror ("OOPS preparing to send to peer");
  // 		return EOF;
  //	}
  // }
  //
  // The correct way to handle this is for the application to
  // trap (and ignore?) SIGPIPE.  Thanks to Amos Shapira
  // for reminding me of this.
  //

  // Starting at the beginning of the buffer, send as much
  // data as there is waiting.  send guarantees that all
  // of the data will be sent or an error will be returned.
  //
  if (this->send (pbase (), pptr () - pbase ()) == -1)
  {
	return EOF;
  }

  // Now that we've sent everything in the output buffer, we reset the
  // buffer pointers to appear empty.
  //
  setp (base (), ebuf ());

  return 0;
}

int 
ACE_Streambuf_T::get_one_byte (void)
{
  // The recv function will return immediately if there is no data
  // waiting.  So, we use recv_n to wait for exactly one byte to come
  // from the peer.  Later, we can use recv to see if there is
  // anything else in the buffer. (Ok, we could use flags to tell it
  // to block but I like this better.)

  if (this->recv_n (base (), 1, MSG_PEEK, recv_timeout_) != 1)
    return EOF;

  return 1;
}

int
ACE_Streambuf_T::fillbuf (void)
  // This will be called when the read (get) buffer has been
  // exhausted (ie -- gptr == egptr)
{
  // Invoke recv_n to get exactly one byte from the remote.  This
  // will block until something shows up.
  //
  if (get_one_byte () == EOF)
    return EOF;

  // Now, get whatever else may be in the buffer.  This will return if
  // there is nothing in the buffer. 

  int bc = this->recv (base (), blen (), recv_timeout_);

  // recv will give us -1 if there was a problem.  If there was
  // nothing waiting to be read, it will give us 0.  That isn't an
  // error.

  if (bc < 0)
    return EOF;

  // Move the get pointer to reflect the number of bytes we just read.

  setg (base (), base (), base () + bc);

  // Return the byte-read-count including the one from <get_one_byte>
  return bc;
}

ACE_Streambuf_T::ACE_Streambuf_T (u_int streambuf_size, int io_mode)
  : get_mode_ (1),
    put_mode_ (2),
    mode_ (io_mode),
    streambuf_size_ (streambuf_size),
    recv_timeout_ (NULL)
{
 (void)reset_get_buffer ();
 (void)reset_put_buffer ();
}

u_int ACE_Streambuf_T::streambuf_size (void)
{
  return streambuf_size_;
}

u_int ACE_Streambuf_T::get_waiting (void)
  // Return the number of bytes not yet gotten.
  //	eback + get_waiting = gptr
{
	return this->gptr_saved_ - this->eback_saved_;
}

u_int ACE_Streambuf_T::get_avail (void)
  // Return the number of bytes in the get area (includes some already gotten);
  //	eback + get_avail = egptr
{
	return this->egptr_saved_ - this->eback_saved_;
}

u_int ACE_Streambuf_T::put_avail (void)
  // Return the number of bytes to be 'put' onto the stream media.
  //	pbase + put_avail = pptr
{
	return this->pptr_saved_ - this->pbase_saved_;
}

char *
ACE_Streambuf_T::reset_get_buffer (char * newBuffer, u_int _streambuf_size, u_int _gptr, u_int _egptr)
//
// Typical usage:
//
//	u_int  newGptr  = otherStream->get_waiting ();
//	u_int  newEgptr = otherStream->get_avail ();
//	char * newBuf   = otherStream->reset_get_buffer ();
//	char * oldgetbuf = myStream->reset_get_buffer (newBuf, otherStream->streambuf_size (), newGptr, newEgptr);
//
//	'myStream' now has the get buffer of 'otherStream' and can use it in any way.
//	'otherStream' now has a new, empty get buffer.
//
{
  char * rval = this->eback_saved_;

  // The get area is where the iostrem will get data from.  This is
  // our read buffer.  There are three pointers which describe the
  // read buffer:
  //
  //    eback () - The beginning of the buffer.  Also the furthest
  //              point at which putbacks can be done.  Hence the name.
  //
  //    gptr ()  - Where the next character is to be got from.
  //
  //    egptr () - One position beyond the last get-able character.
  //
  // So that we can switch quicky from read to write mode without
  // any data copying, we keep copies of these three pointers in
  // the variables below.  Initially, they all point to the beginning
  // of our read-dedicated buffer.
  //
  if (newBuffer)
  {
	if (streambuf_size_ != _streambuf_size)
		return NULL;
	this->eback_saved_ = newBuffer;
  }
  else
  {
  	ACE_NEW_RETURN (this->eback_saved_, char[streambuf_size_], 0);
  }

  this->gptr_saved_ = this->eback_saved_ + _gptr;
  this->egptr_saved_ = this->eback_saved_ + _egptr;

  // Disable the get area initially.  This will cause underflow to be
  // invoked on the first get operation.
  setg (0, 0, 0);

  reset_base ();

  return rval;
}

char *
ACE_Streambuf_T::reset_put_buffer (char * newBuffer, u_int _streambuf_size, u_int _pptr)
//
// Typical usage:
//
//	u_int  newPptr = otherStream->put_avail ();
//	char * newBuf  = otherStream->reset_put_buffer ();
//	char * oldputbuf = otherStream->reset_put_buffer (newBuf, otherStream->streambuf_size (), newPptr);
//
{
  char * rval = this->pbase_saved_;

  // The put area is where the iostream will put data that needs to be
  // sent to the peer.  This becomes our write buffer.  The three
  // pointers which maintain this area are:
  //
  //    pbase () - The beginning of the put area.
  //
  //    pptr ()  - Where the next character is to be put.
  //
  //    epptr () - One beyond the last valid position for putting.
  //
  // Again to switch quickly between modes, we keep copies of
  // these three pointers.
  //
  if (newBuffer)
  {
	if (streambuf_size_ != _streambuf_size)
		return NULL;
	this->pbase_saved_ = newBuffer;
  }
  else
  {
  	ACE_NEW_RETURN (this->pbase_saved_, char[streambuf_size_], 0);
  }

  this->pptr_saved_ = this->pbase_saved_ + _pptr;
  this->epptr_saved_ = this->pbase_saved_ + streambuf_size_;

  // Disable the put area.  Overflow will be called by the first call
  // to any put operator.
  setp (0, 0);

  reset_base ();

  return rval;
}

void
ACE_Streambuf_T::reset_base (void)
{
  // Until we experience the first get or put operation, we do not
  // know what our current IO mode is.
  this->cur_mode_ = 0;

  // The common area used for reading and writting is called "base".
  // We initialize it this way so that the first get/put operation
  // will have to "allocate" base.  This allocation will set base to
  // the appropriate specific buffer and set the mode to the correct
  // value.
  setb (0, 0);
}

// If the default allocation strategey were used the common buffer
// would be deleted when the object destructs.  Since we are providing
// separate read/write buffers, it is up to us to manage their memory.

ACE_Streambuf_T::~ACE_Streambuf_T (void)
{
  delete [] this->eback_saved_;
  delete [] this->pbase_saved_;
}

#endif	// ACE_IOSTREAM_BUILDING_TEMPLATE

///////////////////////////////////////////////////////////////////////////


// We will be given a STREAM by the iostream object which creates us.
// See the ACE_IOStream template for how that works.  Like other
// streambuf objects, we can be input-only, output-only or both.

template <class STREAM>
ACE_Streambuf<STREAM>::ACE_Streambuf (STREAM *peer, u_int streambuf_size, int io_mode)
  : ACE_Streambuf_T (streambuf_size, io_mode), peer_ (peer)
{
  // A streambuf allows for unbuffered IO where every character is
  // read as requested and written as provided.  To me, this seems
  // terribly inefficient for socket-type operations, so I've disabled
  // it.  All of the work would be done by the underflow/overflow
  // functions anyway and I haven't implemented anything there to
  // support unbuffered IO.

  this->unbuffered (0);

  // Linebuffered is similar to unbuffered.  Again, I don't have any
  // need for this and I don't see the advantage.  I believe this
  // would have to be supported by underflow/overflow to be effective.
#if !defined (ACE_LACKS_LINEBUFFERED_STREAMBUF)
  this->linebuffered (0);
#endif /* ! ACE_LACKS_LINEBUFFERED_STREAMBUF */

}


///////////////////////////////////////////////////////////////////////////


// The typical constructor.  This will initiailze your STREAM and then
// setup the iostream baseclass to use a custom streambuf based on
// STREAM.

template <class STREAM>
ACE_IOStream<STREAM>::ACE_IOStream (STREAM & stream, u_int streambuf_size)
  : iostream (streambuf_ = new ACE_Streambuf<STREAM> ((STREAM *) this, streambuf_size)),
    STREAM (stream)
{
}

template <class STREAM>
ACE_IOStream<STREAM>::ACE_IOStream (u_int streambuf_size)
  : iostream (streambuf_ = new ACE_Streambuf<STREAM> ((STREAM *) this, streambuf_size))
{
}

// We have to get rid of the streambuf_ ourselves since we gave it to
// iostream ()

template <class STREAM>
ACE_IOStream<STREAM>::~ACE_IOStream (void)
{
  delete this->streambuf_;
}

// The only ambituity in the multiple inheritance is the close ()
// function.

template <class STREAM> int
ACE_IOStream<STREAM>::close (void)
{
  return STREAM::close ();
}

template <class STREAM> ACE_IOStream<STREAM> &
ACE_IOStream<STREAM>::operator>> (ACE_Time_Value *& tv)
{
	ACE_Time_Value * old_tv = this->streambuf_->recv_timeout (tv);
	tv = old_tv;
	return *this;
}

#if (ACE_HAS_STRING_CLASS)

// A simple string operator.  The base iostream has 'em for char* but
// that isn't always the best thing for a String.  If we don't provide
// our own here, we may not get what we want.

template <class STREAM> ACE_IOStream<STREAM> &
ACE_IOStream<STREAM>::operator>> (ACE_IOStream_String & v)
{
  if (ipfx0 ())
  {
    char c;
    iostream::operator>> (c);

    for (v = c ; this->get (c) && !isspace (c) ; v += c)
      continue;
  }

  isfx ();

  return *this;
}


template <class STREAM> ACE_IOStream<STREAM> &
ACE_IOStream<STREAM>::operator<< (ACE_IOStream_String & v)
{
  if (opfx ())
    {
#if defined (ACE_WIN32)
      for (int i = 0 ; i < v.GetLength () ; ++i)
#else
	for (u_int i = 0 ; i < (u_int) v.length () ; ++i)
#endif
	  this->put (v[i]);
    }

  osfx ();

  return *this;
}


//////////////////////////////////////////////////////////////////
// A more clever put operator for strings that knows how to
// deal with quoted strings containing back-quoted quotes.
//
template <class STREAM> ACE_IOStream<STREAM> &
ACE_IOStream<STREAM>::operator>> (QuotedString & str)
{
  if (ipfx0 ())
    {
      char c;

      if (! (*this >> c)) // eat space up to the first char
	// this->set (ios::eofbit|ios::failbit);
	return *this;

      str = "";	// Initialize the string

      // if we don't have a quote, append until we see space
      if (c != '"')
	for (str = c ; this->get (c) && !isspace (c) ; str += c)
	  continue;
      else
	for (; this->get (c) && c != '"' ; str += c)
	  if (c == '\\')
	    {
	      this->get (c);
	      if (c != '"')
		str += '\\';
	    }
    }

  isfx ();
	
  return *this;
}

template <class STREAM> ACE_IOStream<STREAM> &
ACE_IOStream<STREAM>::operator<< (QuotedString & str)
{
  if (opfx ())
  {
	this->put ('"');
	for (u_int i = 0 ; i < str.length () ; ++i)
	{
		if (str[i] == '"')
			this->put ('\\');
		this->put (str[i]);
	}
	this->put ('"');
  }

  osfx ();

  return *this;
}

#endif /* ACE_HAS_STRING_CLASS */
#endif /* ACE_IOSTREAM_C */

