// IOStream.cpp
// $Id$

#if !defined (ACE_IOSTREAM_T_C)
#define ACE_IOSTREAM_T_C

#define ACE_BUILD_DLL
#include "ace/IOStream_T.h"

///////////////////////////////////////////////////////////////////////////


// We will be given a STREAM by the iostream object which creates us.
// See the ACE_IOStream_T template for how that works.  Like other
// streambuf objects, we can be input-only, output-only or both.

template <class STREAM>
ACE_Streambuf_T<STREAM>::ACE_Streambuf_T (STREAM *peer, u_int streambuf_size, int io_mode)
  : ACE_Streambuf (streambuf_size, io_mode), peer_ (peer)
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
ACE_IOStream_T<STREAM>::ACE_IOStream_T (STREAM & stream, u_int streambuf_size)
  : iostream (streambuf_ = new ACE_Streambuf_T<STREAM> ((STREAM *) this, streambuf_size)),
    STREAM (stream)
{
  iostream::init (this->streambuf_);
}

template <class STREAM>
ACE_IOStream_T<STREAM>::ACE_IOStream_T (u_int streambuf_size)
  : iostream (streambuf_ = new ACE_Streambuf_T<STREAM> ((STREAM *) this, streambuf_size))
{
  iostream::init (this->streambuf_);
}

// We have to get rid of the streambuf_ ourselves since we gave it to
// iostream ()

template <class STREAM>
ACE_IOStream_T<STREAM>::~ACE_IOStream_T (void)
{
  delete this->streambuf_;
}

// The only ambituity in the multiple inheritance is the close ()
// function.

template <class STREAM> int
ACE_IOStream_T<STREAM>::close (void)
{
  return STREAM::close ();
}

template <class STREAM> ACE_IOStream_T<STREAM> &
ACE_IOStream_T<STREAM>::operator>> (ACE_Time_Value *& tv)
{
	ACE_Time_Value * old_tv = this->streambuf_->recv_timeout (tv);
	tv = old_tv;
	return *this;
}

#if defined (ACE_HAS_STRING_CLASS)

// A simple string operator.  The base iostream has 'em for char* but
// that isn't always the best thing for a String.  If we don't provide
// our own here, we may not get what we want.

template <class STREAM> ACE_IOStream_T<STREAM> &
ACE_IOStream_T<STREAM>::operator>> (ACE_IOStream_String & v)
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


template <class STREAM> ACE_IOStream_T<STREAM> &
ACE_IOStream_T<STREAM>::operator<< (ACE_IOStream_String & v)
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
template <class STREAM> STREAM &
operator>> (STREAM & stream, QuotedString & str)
{
      char c;

      if (! (stream >> c)) // eat space up to the first char
	// stream.set (ios::eofbit|ios::failbit);
	return stream;

      str = "";	// Initialize the string

      // if we don't have a quote, append until we see space
      if (c != '"')
	for (str = c ; stream.get (c) && !isspace (c) ; str += c)
	  continue;
      else
	for (; stream.get (c) && c != '"' ; str += c)
	  if (c == '\\')
	    {
	      stream.get (c);
	      if (c != '"')
		str += '\\';
	    }
	
  return stream;
}

template <class STREAM> STREAM &
operator<< ( STREAM & stream, QuotedString & str)
{
	stream.put ('"');
	for (u_int i = 0 ; i < str.length () ; ++i)
	{
		if (str[i] == '"')
			stream.put ('\\');
		stream.put (str[i]);
	}
	stream.put ('"');

  return stream;
}

#endif /* ACE_HAS_STRING_CLASS */
#endif /* ACE_IOSTREAM_T_C */

