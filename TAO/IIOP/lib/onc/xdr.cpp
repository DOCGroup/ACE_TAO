// @(#)xdr.cpp	1.3 95/11/07
// Copyright 1995 by Sun Microsystems, Inc.
//
// XDR/TCP ORB Marshaler/unmarshaler
//
// Includes functions to encode 64 and 128 bit quantities, interpretive
// encoder/decoder.
//
// XXX note -- this is incomplete (see xdr.hh) and doesn't do
// exactly what the Prelude protocol does, particularly in the
// area of typecodes.
//
// XXX implement the put/get primitives for 64 and 128 bit data types
//
// XXX strings, principals, typecodes, char arrays, char seqs, and
// same for octets ... all should marshal the bulk characters tightly
// packed, four to a word, else this violates the XDR spec.
//
// XXX optimization 1:  allowed by current XDR spec, when marshaling
// true bulk data could eliminate a buffer copy by writing long
// buffers (bigger than 'remainder of current fragment') in single
// large chunks ... weigh cost of an extra kernel write against data
// copy getting added.  Applies to byte-structured bulk data only,
// not (portably) to arrays of longs etc.
//
// XXX optimization 2:  modifies current XDR spec:  when marshaling
// arrays of "short" data (e.g. unicode strings) pack tightly.
//

#include <unistd.h>
#include <string.h>
#include <widec.h>

#include <corba/orb.hh>

#include "runtime/debug.hh"

#include "onc/xdr.hh"


//
// I/O for 64 bit quantities -- integers, doubles
//
CORBA_Boolean
XDR_stream::put_longlong (const CORBA_LongLong &)
THROWS_NOTHING
{
    return CORBA_B_FALSE;
}

CORBA_Boolean
XDR_stream::get_longlong (CORBA_LongLong &)
THROWS_NOTHING
{
    return CORBA_B_FALSE;
}


//
// I/O for 128 bit quantities -- long doubles
//
CORBA_Boolean
XDR_stream::put_longdouble (const CORBA_LongDouble &)
THROWS_NOTHING
{
    return CORBA_B_FALSE;
}

CORBA_Boolean
XDR_stream::get_longdouble (CORBA_LongDouble &)
THROWS_NOTHING
{
    return CORBA_B_FALSE;
}


//
// Encode instances of arbitrary data types based only on typecode.  "data"
// points to the data type; if it's not a primitve data type, the TypeCode
// interpreter is used to recursively encode its components.  "context" is
// the marshaling stream on which to encode the data value.
//
// This is a fairly typical TypeCode interpreter visit() routine; it works
// on a single data value in conjunction with context information, and must
// handle all IDL data types.
//
CORBA_TypeCode::traverse_status
XDR_stream::encoder (
    CORBA_TypeCode_ptr  tc,
    const void		*data,
    const void		*,
    void		*context,
    CORBA_Environment	&env
)
THROWS_NOTHING
{
    CORBA_Boolean	continue_encoding = CORBA_B_TRUE;
    XDR_stream		*stream = (XDR_stream *)context;

    switch (tc->_kind) {
      case tk_null:
      case tk_void:
	// nothing to encode!
	break;
      
      case tk_char:
      case tk_octet:
	continue_encoding = stream->put_char (*(char *)data);
	break;

      case tk_short:
      case tk_ushort:
	continue_encoding = stream->put_short (*(short *)data);
	break;

      case tk_long:
      case tk_ulong:
      case tk_float:
	continue_encoding = stream->put_long (*(CORBA_Long *)data);
	break;

      case tk_double:
      case tk_longlong:
      case tk_ulonglong:
        continue_encoding = stream->put_longlong (*(CORBA_LongLong *)data);
	break;

      case tk_boolean:
        continue_encoding = stream->put_boolean (*(CORBA_Boolean *)data);
	break;

      case tk_enum:
	{
	    //
	    // NOTE assumption that this is in-range.
	    //
	    // XXX should check this, it's a hard-to-recover error
	    // for the other side
	    //
	    unsigned	value = *(unsigned *)data;
	    continue_encoding = stream->put_ulong (value);
	}
	break;

      case tk_any:
        {
	    CORBA_Any	*any = (CORBA_Any *)data;

	    tc = any->type ();
	    if (encoder (_tc_CORBA_TypeCode, &tc, 0, context, env)
		    != CORBA_TypeCode::TRAVERSE_CONTINUE)
		return CORBA_TypeCode::TRAVERSE_STOP;

	    data = any->value ();
	    return encoder (tc, data, 0, context, env);
        }
	// NOTREACHED

      case tk_TypeCode:
	{
	    CORBA_TypeCode_ptr	tc2;

	    tc2 = *(CORBA_TypeCode_ptr *)data;

	    continue_encoding = stream->put_ulong ((CORBA_ULong) tc2->_kind);
	    if (continue_encoding == CORBA_B_FALSE)
		break;

	    switch (tc2->_kind) {
	      //
	      // Most TypeCodes have empty parameter lists
	      //
	      default:
	        break;

	      //
	      // A few have "simple" parameter lists
	      //
	      case tk_string:
	      case tk_wstring:
		continue_encoding = stream->put_ulong (tc2->_length);
		break;

	      //
	      // Indirected typecodes can't occur at "top level" like
	      // this, only nested inside others!
	      //
	      case ~0:
	        dmsg ("indirected typecode at top level!");
	        continue_encoding = CORBA_B_FALSE;
	        break;

	      //
	      // The rest have "complex" parameter lists that are already
	      // encoded as bulk octets ... put length, then octets
	      //
	      case tk_objref:
	      case tk_struct:
	      case tk_union:
	      case tk_enum:
	      case tk_sequence:
	      case tk_array:
	      case tk_alias:
	      case tk_except:
	        {
		    unsigned 	i;

		    continue_encoding = stream->put_ulong (tc2->_length);
	            for (i = 0; i < tc2->_length && continue_encoding; i++)
			continue_encoding =
			    stream->put_octet (tc2->_buffer [i]);
	        }
	    }
	}
	break;

      case tk_Principal:
	{
	    CORBA_Principal_ptr	p = *(CORBA_Principal_ptr*) data;
	    unsigned			i;

	    if (p != 0) {
		continue_encoding = stream->put_long (p->id.length);
		for (i = 0; continue_encoding && i < p->id.length; i++)
		    continue_encoding = stream->put_octet (p->id.buffer [i]);
	    } else
		continue_encoding = stream->put_long (0);
	}
	break;

      case tk_objref:
	// XXX implement me
	break;

      case tk_sequence:
	{
	    //
	    // First marshal the sequence length, verifying that
	    // it's within the sequence bounds ...
	    //
	    CORBA_OctetSeq 		*seq = (CORBA_OctetSeq *) data;
	    CORBA_ULong		len = seq ? seq->length : 0;

	    if (len > 0) {
		CORBA_ULong		bounds;

		bounds = tc->ulong_param (1, env);
		if (env.exception () != 0)
		    return CORBA_TypeCode::TRAVERSE_STOP;

		if (bounds != 0 && len > bounds) {
		    env.exception (new CORBA_BAD_PARAM (COMPLETED_MAYBE));
		    return CORBA_TypeCode::TRAVERSE_STOP;
		}
	    }
	    continue_encoding = stream->put_ulong (len);

	    //
	    // Fast exit on error or empty sequence
	    //
	    if (!continue_encoding || len == 0)
		break;
	}
	// FALLTHROUGH

      case tk_struct:
      case tk_union:
      case tk_array:
      case tk_alias:
	//
	// Marshal each member in order.
	//
	return tc->traverse (data, 0, encoder, context, env);

      case tk_except:
	//
	// Convert the the "hidden" TypeCode at the beginning of the
	// exception into an on-the-wire ID, then marshal the members
	// in order (traversal skips that hidden typecode, and more).
	//
	// NOTE:  This is asymmetric with respect to decoding the exception,
	// since whoever decodes must pull off the ID and map it to the
	// typecode to be used to unmarshal it (search among legal choices).
	//
	{
	    CORBA_String	id = tc->id (env);

	    if (env.exception () == 0) {
		continue_encoding =
		    encoder (_tc_CORBA_String, &id, 0, context, env)
			    == CORBA_TypeCode::TRAVERSE_CONTINUE
		    && tc->traverse (data, 0, encoder, context, env);
	    } else
		continue_encoding = CORBA_B_FALSE;
	}
	break;

      case tk_string:
	{
	    CORBA_String	str = *(CORBA_String *) data;
	    CORBA_ULong	len, bounds;

	    //
	    // Be nice to programmers:  treat nulls as empty strings
	    // not errors.  (OMG-IDL supports languages that don't use
	    // the C/C++ notion of null v. empty strings; nulls aren't
	    // part of the OMG-IDL string model.)
	    //
	    if (str == 0) {
		stream->put_ulong (1);
		stream->put_char (0);
		break;
	    }

	    //
	    // Verify string satisfies bounds requirements.  We're not
	    // so permissive as to send messages violating the interface
	    // spec by having excessively long strings!
	    //
	    bounds = tc->ulong_param (0, env);
	    if (env.exception () != 0)
		return CORBA_TypeCode::TRAVERSE_STOP;
	    len = strlen ((char *)str);

	    if (bounds != 0 && len > bounds) {
		continue_encoding = CORBA_B_FALSE;
		break;
	    }

	    //
	    // Encode the string, followed by a NUL character.
	    //
	    continue_encoding = stream->put_ulong (len + 1);
	    while (continue_encoding != CORBA_B_FALSE && *str)
		continue_encoding = stream->put_char (*str++);
	    stream->put_char (0);
	}
	break;

      case tk_wstring:
        {
	    wchar_t		*str = *(wchar_t **) data;
	    CORBA_ULong		len, bounds;

	    //
	    // Be nice to programmers:  treat nulls as empty strings
	    // not errors.  (OMG-IDL supports languages that don't use
	    // the C/C++ notion of null v. empty strings; nulls aren't
	    // part of the OMG-IDL string model.)
	    //
	    if (str == 0) {
		stream->put_ulong (1);
		stream->put_wchar (0);
		break;
	    }

	    //
	    // Verify wide string satisfies bounds requirements.  We're
	    // not so permissive as to send messages violating the interface
	    // spec by having excessively long strings!
	    //
	    bounds = tc->ulong_param (0, env);
	    if (env.exception () != 0)
		return CORBA_TypeCode::TRAVERSE_STOP;
	    len = wslen (str);
	    if (bounds != 0 && len > bounds) {
		continue_encoding = CORBA_B_FALSE;
		break;
	    }

	    //
	    // Encode the wide string, followed by a NUL character.
	    //
	    continue_encoding = stream->put_ulong (wslen (str) + 1);
	    while (continue_encoding != CORBA_B_FALSE && *str)
		continue_encoding = stream->put_wchar (*str++);
	    stream->put_wchar (0);
        }
	break;

      case tk_longdouble:
	continue_encoding =
		stream->put_longdouble (*(CORBA_LongDouble *)data);
	break;

      case tk_wchar:
	continue_encoding = stream->put_wchar (*(wchar_t *)data);
	break;
	
      // case ~0:
      default:
	dmsg ("encoder default case ?");
	continue_encoding = CORBA_B_FALSE;
	break;
    }

    if (continue_encoding == CORBA_B_FALSE) {
	env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
	dmsg ("marshaling encoder detected error");
	return CORBA_TypeCode::TRAVERSE_STOP;
    }
    return CORBA_TypeCode::TRAVERSE_CONTINUE;
}



//
// Array of typecodes used to unmarshal ...
//
extern CORBA_TypeCode_ptr __tc_consts [TC_KIND_COUNT];


CORBA_TypeCode::traverse_status
XDR_stream::decoder (
    CORBA_TypeCode_ptr  tc,
    const void		*data,
    const void		*,
    void		*context,
    CORBA_Environment	&env
)
THROWS_NOTHING
{
    CORBA_Boolean	continue_decoding = CORBA_B_TRUE;
    XDR_stream			*stream = (XDR_stream *)context;

    switch (tc->_kind) {
      case tk_null:
      case tk_void:
	// nothing to decode!
	break;
      
      case tk_char:
      case tk_octet:
	continue_decoding = stream->get_char (*(CORBA_Char *)data);
	break;

      case tk_short:
      case tk_ushort:
	continue_decoding = stream->get_short (*(short *)data);
	break;

      case tk_long:
      case tk_ulong:
      case tk_float:
	continue_decoding = stream->get_long (*(CORBA_Long *)data);
	break;

      case tk_longlong:
      case tk_ulonglong:
      case tk_double:
        continue_decoding = stream->get_longlong (*(CORBA_LongLong *)data);
	break;

      case tk_boolean:
        continue_decoding = stream->get_boolean (*(CORBA_Boolean *)data);
	break;

      case tk_enum:
	{
	    CORBA_ULong	val;

	    //
	    // NOTE assumption that this is in-range.
	    //
	    // XXX should check this, it's rather hard to recover
	    // from such errors since they "do not occur" and are
	    // essentially never tested for.
	    //
	    continue_decoding = stream->get_ulong (val);
	    *(unsigned *)data = (unsigned) val;
	}
	break;

      case tk_any:
	{
	    CORBA_Any			*any = (CORBA_Any *)data;
	    CORBA_TypeCode_ptr	tc2;
	    void			*value;

	    if (decoder (_tc_CORBA_TypeCode, &tc2, 0, context, env)
		    != CORBA_TypeCode::TRAVERSE_CONTINUE)
		return CORBA_TypeCode::TRAVERSE_STOP;

	    value = new CORBA_Octet [tc2->size (env)];

	    if (decoder (tc2, value, 0, context, env)
		    != CORBA_TypeCode::TRAVERSE_CONTINUE) {
		delete value;
		CORBA_release (tc2);
		return CORBA_TypeCode::TRAVERSE_STOP;
	    }
	    (void) new (any) CORBA_Any (tc2, value, CORBA_B_TRUE);
        }
        break;

      case tk_TypeCode:
	{
	    CORBA_ULong		kind;
	    CORBA_TypeCode_ptr	*tcp;

	    continue_decoding = stream->get_ulong (kind);
	    if (continue_decoding == CORBA_B_FALSE)
		break;
	    if (kind >= TC_KIND_COUNT) {
		continue_decoding = CORBA_B_FALSE;
		break;
	    }
	    
	    tcp = (CORBA_TypeCode_ptr *)data;

	    //
	    // Typecodes with empty parameter lists all have preallocated
	    // constants.  We use those to reduce memory consumption and
	    // heap access ... also, to speed things up!
	    //
	    if (((*tcp) = __tc_consts [(unsigned) kind]) != 0) {
		*tcp = __tc_consts [(unsigned) kind];
		break;
	    } else switch (kind) {
	      //
	      // Need special handling for all kinds of typecodes that have
	      // nonempty parameter lists ...
	      //
	      default:			// error: missed a case! 
		env.exception (new CORBA_INTERNAL (COMPLETED_MAYBE));
		return CORBA_TypeCode::TRAVERSE_STOP;

	      //
	      // Some have "simple" parameter lists ... some of these also
	      // have preallocated constants that could be used.
	      //
	      case tk_string:
	      case tk_wstring:
		{
		    CORBA_ULong	bound;

		    continue_decoding = stream->get_ulong (bound);
		    if (continue_decoding) {
			if (bound == 0) {
			    if (kind == tk_string)
				*tcp = _tc_CORBA_String;
			    else
				*tcp = _tc_CORBA_WString;
			} else {
			    *tcp = new CORBA_TypeCode ((CORBA_TCKind) kind,
				bound, 0, CORBA_B_TRUE);
			}
		    }
		}
		break;

	      //
	      // Indirected typecodes, illegal at "top level" but we allow
	      // unmarshaling of them here because we use the same code to
	      // read "off the wire" (where they're illegal) and to read
	      // out of an encapsulation stream.  We distinguish the case
	      // where this is legal as described above.
	      //
	      case ~0:
		// XXX implement me
		break;

	      //
	      // The rest have "complex" parameter lists that are
	      // encoded as bulk octets ...
	      //
	      case tk_objref:
	      case tk_struct:
	      case tk_union:
	      case tk_enum:
	      case tk_sequence:
	      case tk_array:
	      case tk_alias:
	      case tk_except:
	        {
		    unsigned		len, i;
		    CORBA_ULong	length;
		    CORBA_Octet	*buffer;

		    continue_decoding = stream->get_ulong (length);
		    if (!continue_decoding)
			break;

		    // if length > MAXUNSIGNED, error ...
		    len = (unsigned) length;

		    buffer = new CORBA_Octet [len];

	            for (i = 0; i < len && continue_decoding; i++)
			continue_decoding = stream->get_octet (buffer [i]);

		    if (!continue_decoding) {
			delete buffer;
			break;
		    }
		    *tcp = new CORBA_TypeCode ((CORBA_TCKind)kind,
			    len, buffer, CORBA_B_TRUE);
	        }
	    }
	}
	break;

      case tk_Principal:
	{
	    CORBA_Principal_ptr	*pp = (CORBA_Principal_ptr *)data;
	    CORBA_ULong	len;

	    continue_decoding = stream->get_ulong (len);
	    if (len == 0)
		*pp = 0;
	    else {
		*pp = new CORBA_Principal;
		(*pp)->id.buffer = new CORBA_Octet [(size_t) len];
		(*pp)->id.maximum = (*pp)->id.length = len;

		for (unsigned i = 0;
			continue_decoding != CORBA_B_FALSE && i < len;
			i++)
		    continue_decoding = stream->get_octet (
					(*pp)->id.buffer [i]);
	    }
	}
	break;

      case tk_objref:
	// XXX implement me
	break;

      case tk_sequence:
	{
	    //
	    // First unmarshal the sequence length ... we trust it
	    // to be right here, on the "be gracious in what you
	    // accept" principle.  We don't generate illegal sequences
	    // (i.e. length > bounds).
	    //
	    CORBA_OctetSeq 		*seq = (CORBA_OctetSeq *) data;

	    continue_decoding = stream->get_ulong (seq->length);
	    seq->maximum = seq->length;
	    seq->buffer = 0;

	    //
	    // Fast exit on empty sequences or errors
	    //
	    if (!continue_decoding || seq->length == 0)
		break;

	    //
	    // ... then allocate the memory into which we'll unmarshal
	    //
	    CORBA_TypeCode_ptr	tc2;
	    size_t			size;

	    tc2 = tc->typecode_param (0, env);
	    if (env.exception ())
		return CORBA_TypeCode::TRAVERSE_STOP;

	    size = tc2->size (env);
	    if (env.exception ())
		return CORBA_TypeCode::TRAVERSE_STOP;

	    tc2->Release ();

	    seq->buffer = new CORBA_Octet [size * (size_t) seq->maximum];
	}
	// FALLTHROUGH

      case tk_struct:
      case tk_union:
      case tk_array:
      case tk_alias:
	//
	// Unmarshal all the individual elements using the per-member
	// description held in the "parent" TypeCode.
	//
	
	// FALLTHROUGH

      case tk_except:
	//
	// For exceptions, the "hidden" type ID near the front of the
	// on-wire representation was previously unmarshaled and mapped
	// to the "tc" typcode we're using to traverse the memory ...
	// at the same time its vtable, refcount, and other state was
	// established.
	//
	// NOTE:  This is asymmetric with respect to encoding exceptions.
	//
	return tc->traverse (data, 0, decoder, context, env);

      case tk_string:
	{
	    CORBA_String	str;
	    CORBA_ULong	len = 0;

	    //
	    // On decode, omit the check against specified string bounds,
	    // and cope with illegal "zero length" strings (all lengths
	    // on the wire must include a NUL).
	    //
	    // This is on the principle of being gracious in what we accept;
	    // we don't generate messages that fail to comply with protocol
	    // specs, but we will accept them when it's clear how to do so.
	    //
	    continue_decoding = stream->get_ulong (len);
	    *((CORBA_String*)data) = str = new CORBA_Char [(size_t) (len)];
	    if (len != 0)
		while (continue_decoding != CORBA_B_FALSE && len-- != 0) {
		    continue_decoding = stream->get_char (*(CORBA_Char *)str);
		    str++;
		}
	    break;
	}

      case tk_wstring:
        {
	    wchar_t		*str;
	    CORBA_ULong	len = 0;

	    //
	    // On decode, omit the check against specified wstring bounds,
	    // and cope with illegal "zero length" strings (all lengths
	    // on the wire must include a NUL).
	    //
	    // This is on the principle of being gracious in what we accept;
	    // we don't generate messages that fail to comply with protocol
	    // specs, but we will accept them when it's clear how to do so.
	    //
	    continue_decoding = stream->get_ulong (len);
	    *((wchar_t **)data) = str = new wchar_t [(size_t) (len)];
	    if (len != 0) {
		while (continue_decoding != CORBA_B_FALSE && len--) {
		    continue_decoding = stream->get_wchar (*str);
		    str++;
		}
	    }
	}
	break;

      case tk_longdouble:
	continue_decoding =
	    stream->get_longdouble (*(CORBA_LongDouble *)data);
	break;

      case tk_wchar:
	continue_decoding = stream->get_wchar (*(wchar_t *)data);
	break;

      // case ~0:
      default:
	continue_decoding = CORBA_B_FALSE;
	dmsg ("decode, default case?");
	break;
    }

    if (continue_decoding == CORBA_B_FALSE) {
	env.exception (new CORBA_MARSHAL (COMPLETED_NO));
	dmsg ("marshaling decoder detected error");
	return CORBA_TypeCode::TRAVERSE_STOP;
    }
    return CORBA_TypeCode::TRAVERSE_CONTINUE;
}


//
// Write an XDR message fragment out on the stream.
//
CORBA_Boolean
XDR_stream::flush_frag (CORBA_Boolean is_last)
THROWS_NOTHING
{
    return CORBA_B_FALSE;

#if	0
    int			status;
    CORBA_ULong		size, header;

    size = index * sizeof (CORBA_ULong);	// byte length of msg
    if (is_last)
	header = 0x80000000 | size;		// indicates last frag
    else
	header = size;
    buffer [0] = htonl (header);

    //
    // XXX for portability, loop until there's no error.  Some
    // platforms/mode don't guarantee full TCP writes even when
    // async (or nonblocking) mode was not set on this socket.
    //
    size += sizeof (CORBA_ULong);
    status = write (fd, &buffer [0], size);

    index = 0;

    return (status == size);
#endif
}


//
// Read an XDR message fragment in from the stream.
//
CORBA_Boolean
XDR_stream::read_frag ()
THROWS_NOTHING
{
    // read cookie, split out size and "is last" flag
    // read rest of buffer

    return CORBA_B_FALSE;
}
