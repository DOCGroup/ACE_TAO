// This may look like C, but it's really -*- C++ -*-
//
// @(#) $Id$
// Copyright 1995 by Sun Microsystems, Inc.
//
// XDR stream interface and implementation (partially implemented)
//
// XXX as of 3-Nov-95 XDR_stream should only be relied on to marshal the
// simplest primitives ... not objrefs, typecodes, etc.  Also, the
// handling of sequences of chars/octets/shorts/wchars is wrong.
//

#ifndef	_xdr_hh
#define	_xdr_hh

#include	<sys/types.h>
#include	<netinet/in.h>

//
// Define on systems that fully support exceptions ... excluding
// some very common platforms like G++ and VC++ 2.2
//
// #define	THROWS_NOTHING	throw()

//
// On DevPro's 4.0.1a compilers, null throw specs carry a big
// performance penalty:  slower by about 25% here.  Disable!!
//
#define	THROWS_NOTHING

class XDR_stream
{
  public:
    enum {
	STANDARD_BUFSIZE = 4096,	// ~3 enet packets with TCP
	BUFFER_LEN = 1 + (STANDARD_BUFSIZE
				/ sizeof (CORBA_Long))
    };

				XDR_stream (
				    int		_fd
				) THROWS_NOTHING :
				    fd		(_fd)
				{
				    index = 0;
				    decode_flag = CORBA_B_FALSE;
				    max_index = 0;
				    is_last_frag = CORBA_B_FALSE;
				}

				~XDR_stream () THROWS_NOTHING {}


    //
    // Write the last bit of an encoded message ... or tell if we've
    // decoded all the data in an encoded message we're processing.
    //
    // NOTE:  these assume the mode is appropriate for these operations.
    //
    CORBA_Boolean		output_msg_at_end () THROWS_NOTHING;
    CORBA_Boolean		input_msg_at_end () THROWS_NOTHING
				{ return is_last_frag && index == max_index; }

    //
    // Tell if we're decoding or encoding data.
    //
    CORBA_Boolean		is_decoding () THROWS_NOTHING
				{ return decode_flag; }
    CORBA_Boolean		is_encoding () THROWS_NOTHING
				{ return !is_decoding (); }
    
    // private ... iff there's a routine to set it!
    CORBA_Boolean		decode_flag;	// true iff decoding

    //
    // ENCODING SUPPORT ... 
    //
    CORBA_Boolean		put32 (CORBA_Long word) THROWS_NOTHING;

    CORBA_Boolean		put_byte (char c) THROWS_NOTHING
				{ return put32 (c); }
    CORBA_Boolean		put_short (CORBA_Short s) THROWS_NOTHING
				{ return put32 (s); }
    CORBA_Boolean		put_long (CORBA_Long l) THROWS_NOTHING
				{ return put32 (l); }

    CORBA_Boolean		put_longlong (const CORBA_LongLong &ll)
				THROWS_NOTHING;

    inline CORBA_Boolean	put_char (CORBA_Char c) THROWS_NOTHING
				{ return put_byte ((char) c); }
    inline CORBA_Boolean	put_wchar (wchar_t wc) THROWS_NOTHING
				{
				    //
				    // "wchar_t" isn't always 2 bytes, such
				    // systems might need further conversion
				    // (e.g. hosts with multibyte characters
				    // native, rather than UNICODE)
				    //
				    return put_short ((short)wc);
				}
    
    inline CORBA_Boolean	put_boolean (CORBA_Boolean b) THROWS_NOTHING
				{ return put_byte ((char)
					    (b != CORBA_B_FALSE)); }

    inline CORBA_Boolean	put_octet (CORBA_Octet o) THROWS_NOTHING
				{ return put_byte ((char) o); }
    inline CORBA_Boolean	put_ushort (CORBA_UShort s) THROWS_NOTHING
				{ return put_short ((CORBA_Short) s); }
    inline CORBA_Boolean	put_ulong (CORBA_ULong l) THROWS_NOTHING
				{ return put_long ((CORBA_Long) l); }
    inline CORBA_Boolean	put_ulonglong (const CORBA_ULongLong &ll)
    				THROWS_NOTHING
				{ return
				    put_longlong ((CORBA_LongLong &) ll); }
				    
    inline CORBA_Boolean	put_float (float f) THROWS_NOTHING
				{ return put_long (*(CORBA_Long *) &f); }
    inline CORBA_Boolean	put_double (const double &d) THROWS_NOTHING
				{ return
				    put_longlong (*(CORBA_LongLong *) &d); }

    CORBA_Boolean		put_longdouble (const CORBA_LongDouble &ld)
				THROWS_NOTHING;

    //
    // marshaling interpreter ... 'context' really points to a stream.
    // 
    static CORBA_TypeCode::traverse_status
				encoder (
					CORBA_TypeCode_ptr	tc,
					const void		*data,
					const void     		*,
					void			*context,
					CORBA_Environment	&env
				) THROWS_NOTHING;

    //
    // DECODING SUPPORT
    //
    // XXX fix lack of error reporting here !!
    //

    CORBA_Long			get32 () THROWS_NOTHING
				{
				    if (index <= max_index
					    || read_frag () == CORBA_B_TRUE)
					return ntohl (buffer [index++]);
				    else
					return 0;
				}


    CORBA_Boolean		get_byte (char &c) THROWS_NOTHING
				{
				    c = (char) get32();
				    return CORBA_B_TRUE;
				}

    CORBA_Boolean		get_short (CORBA_Short &s) THROWS_NOTHING
				{
				    s = (short) get32();
				    return CORBA_B_TRUE;
				}

    CORBA_Boolean		get_long (CORBA_Long &l) THROWS_NOTHING
				{
				    l = get32();
				    return CORBA_B_TRUE;
				}

    CORBA_Boolean		get_longlong (CORBA_LongLong &ll)
				THROWS_NOTHING;

    inline CORBA_Boolean	get_char (CORBA_Char &o) THROWS_NOTHING
				{ return get_byte ((char &) o); }
    inline CORBA_Boolean	get_wchar (wchar_t &wc) THROWS_NOTHING
				{
				    short s;

				    //
				    // wchar_t isn't always "short"
				    //
				    CORBA_Boolean retval = get_short (s);
				    wc = s;
				    return retval;
				}

    inline CORBA_Boolean	get_boolean (CORBA_Boolean &b) THROWS_NOTHING
				{
				    CORBA_Char c;

				    //
				    // CORBA_Boolean is rarely 'char'
				    //
				    CORBA_Boolean retval = get_char (c);
				    b = (c == 1);
				    return retval;
				}

    inline CORBA_Boolean	get_octet (CORBA_Octet &o) THROWS_NOTHING
				{ return get_byte ((char &) o); }
    inline CORBA_Boolean	get_ushort (CORBA_UShort &s) THROWS_NOTHING
				{ return get_short ((short&) s); }
    inline CORBA_Boolean	get_ulong (CORBA_ULong &l) THROWS_NOTHING
				{ return get_long ((CORBA_Long &) l); }
    inline CORBA_Boolean	get_ulonglong (const CORBA_ULongLong &ull)
    				THROWS_NOTHING
				{ return
				    get_longlong ((CORBA_LongLong &) ull); }

    inline CORBA_Boolean	get_float (float &f) THROWS_NOTHING
				{ return get_long ((CORBA_Long &) f); }
    inline CORBA_Boolean	get_double (double &d) THROWS_NOTHING
				{ return
				    get_longlong ((CORBA_LongLong &) d); }

    CORBA_Boolean		get_longdouble (CORBA_LongDouble &ld)
				THROWS_NOTHING;

    //
    // unmarshaling interpreter ... 'context' really points to a buffer.
    // 
    static CORBA_TypeCode::traverse_status
				decoder (
					CORBA_TypeCode_ptr	tc,
					const void		*data,
					const void		*,
					void			*context,
					CORBA_Environment	&env
				) THROWS_NOTHING;

  private:
    //
    // Low level I/O primitives ... flush a fragment (maybe as the last one),
    // read a fragment in.
    //
    CORBA_Boolean		flush_frag (CORBA_Boolean is_last)
				THROWS_NOTHING;
    CORBA_Boolean		read_frag () THROWS_NOTHING;

    //
    // The actual buffer and the index to the current entry.
    // (Next buffer entry read/written is index+1 ...)
    //
    CORBA_Long			buffer [BUFFER_LEN];
    unsigned			index;

    //
    // The (TCP) stream on which this writes its message fragments.
    //
    const int			fd;

    //
    // Used when reading fragments ... max_index controls where the
    // end of the fragment is recorded to be, and is_last_frag says
    // if it's OK to read_frag() to get the next fragment.
    //
    // To move to the next message in the stream, create a new XDR
    // stream (e.g. on the stack).
    //
    CORBA_Long			max_index;
    CORBA_Boolean		is_last_frag;

    //
    // Two operations not supported by this class.
    //
				XDR_stream (const XDR_stream &);
    XDR_stream			&operator = (const XDR_stream &);
};

inline CORBA_Boolean
XDR_stream::output_msg_at_end () THROWS_NOTHING
{
  return flush_frag (CORBA_B_TRUE);
}

inline CORBA_Boolean
XDR_stream::put32 (CORBA_Long word) THROWS_NOTHING
{
  if (index < (BUFFER_LEN - 1)
      || (flush_frag (CORBA_B_FALSE)
	  == CORBA_B_TRUE)) {
    buffer [++index] = htonl (word);
    return CORBA_B_TRUE;
  } else
    return CORBA_B_FALSE;
}

#endif	// _xdr_hh
