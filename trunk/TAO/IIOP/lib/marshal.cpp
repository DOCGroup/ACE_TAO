// @(#)marshal.cpp	1.7 95/11/04
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// CDR:		Marshaling interpreter
//
// This marshaling interpreter is driven by the typecode interpreter.
// The typecode interpreter understands each compiler environment's
// rules for data structure layout; this understands CDR's rules for
// on-the-wire data structure layout.
//
// Apart from some high level rules related to construction of complex
// data types, the marshaling interpreter just knows how to encode
// primitive data types and allocate memory on decode.
//
// NOTE: to reduce the amount of compiled code, this "knows" facts
// like native float/long/ulong being in legal CDR format, and that
// "char" is native in ISO Latin/1 (so no transformation is needed,
// and octet/char marshaling code is identical).  On exotic platforms
// where this is not true, some of the merged "switch" branches will
// need to be split.
//
// REMEMBER: goal is to have the typecode interpreter plus one side of
// the marshaling interpreter reside in a CPU's code cache; or at
// least to have as litle as possible _outside_ cache when marshaling.
// Compiled marshaling code will have a few less instructions, but
// most will of them will be outside the instruction cache; access
// time to get at them will be high.
//
// NOTE: One interesting optimization is inlining the primitive
// put/get calls ... it'd typically save at least 40% in terms of
// instruction count on each of these critical paths by eliminating
// subroutine call overhead.  Since it would increase code size, such
// changes might not be desirable on machines with small caches.
// Also, with network I/O being today's most significant bottleneck,
// such optimizations haven't been well explored.
//
// THREADING NOTE: The only threading concern is as always, that data
// structures being manipulated by any given thread must be reserved
// to it by some mechanism (e.g. mutex).  This uses no mutable data
// outside of the thread stack, so the onus is entirely on the caller.
//

#include	<assert.h>
#include	<limits.h>
#include	<string.h>
#include	<orb.h>

#include	"debug.h"
#include	"cdr.h"

#include	"giop.h"

#if	defined (HAVE_WIDEC_H)
#		include <widec.h>
#else
extern "C" 
{
  u_int wslen (const CORBA_WChar *);
  CORBA_WChar *wscpy (CORBA_WChar *, const CORBA_WChar *);
}
#endif

extern CORBA_TypeCode TC_opaque;

// Encode instances of arbitrary data types based only on typecode.
// "data" points to the data type; if it's not a primitve data type,
// the TypeCode interpreter is used to recursively encode its
// components.  "context" is the marshaling stream on which to encode
// the data value.
//
// This is a fairly typical TypeCode interpreter visit() routine; it
// works on a single data value in conjunction with context
// information, and must handle all IDL data types.

CORBA_TypeCode::traverse_status
CDR::encoder (CORBA_TypeCode_ptr tc,
	      const void *data,
	      const void *,
	      void *context,
	      CORBA_Environment &env)
{
  CORBA_Boolean	continue_encoding = CORBA_B_TRUE;
  CDR *stream = (CDR *) context;

  switch (tc->_kind) 
    {
    case tk_null:
    case tk_void:
      // nothing to encode!
      break;

    case tk_char:
    case tk_octet:
      continue_encoding = stream->put_char (*(char *) data);
      break;

    case tk_short:
    case tk_ushort:
      continue_encoding = stream->put_short (*(short *) data);
      break;

    case tk_long:
    case tk_ulong:
    case tk_float:
      continue_encoding = stream->put_long (*(CORBA_Long *) data);
      break;

    case tk_double:
    case tk_longlong:
    case tk_ulonglong:
      continue_encoding = stream->put_longlong (*(CORBA_LongLong *) data);
      break;

    case tk_boolean:
      continue_encoding = stream->put_boolean (*(CORBA_Boolean *) data);
      break;

    case tk_enum:
      {
	// NOTE assumption that this is in-range.
	//
	// XXX should check this, it's a hard-to-recover error for the
	// other side

	u_int value = *(u_int *) data;
	continue_encoding = stream->put_ulong (value);
      }
    break;

    case tk_any:
      {
	CORBA_Any *any = (CORBA_Any *) data;

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
	CORBA_TypeCode_ptr tc2;

	tc2 = *(CORBA_TypeCode_ptr *) data;

	continue_encoding = stream->put_ulong ((CORBA_ULong) tc2->_kind);
	if (continue_encoding == CORBA_B_FALSE)
	  break;

	switch (tc2->_kind) 
	  {
	    // Most TypeCodes have empty parameter lists
	  default:
	    break;

	    // A few have "simple" parameter lists
	  case tk_string:
	  case tk_wstring:
	    continue_encoding = stream->put_ulong (tc2->_length);
	    break;

	    // Indirected typecodes can't occur at "top level" like
	    // this, only nested inside others!
	  case ~0:
	    dmsg ("indirected typecode at top level!");
	    continue_encoding = CORBA_B_FALSE;
	    break;

	    // The rest have "complex" parameter lists that are
	    // already encoded as bulk octets ... put length, then
	    // octets.
	  case tk_objref:
	  case tk_struct:
	  case tk_union:
	  case tk_enum:
	  case tk_sequence:
	  case tk_array:
	  case tk_alias:
	  case tk_except:
	    {
	      continue_encoding = stream->put_ulong (tc2->_length);

	      for (u_int i = 0; i < tc2->_length && continue_encoding; i++)
		continue_encoding = stream->put_octet (tc2->_buffer [i]);
	    }
	  }
      }
    break;

    case tk_Principal:
      {
	CORBA_Principal_ptr p = *(CORBA_Principal_ptr *) data;

	if (p != 0) 
	  {
	    continue_encoding = stream->put_long (p->id.length);

	    for (u_int i = 0; continue_encoding && i < p->id.length; i++)
	      continue_encoding = stream->put_octet (p->id.buffer [i]);
	  } 
	else
	  continue_encoding = stream->put_long (0);
      }
    break;

    case tk_objref:

      // Current version:  objref is really an IIOP_Object.
      //
      // This will change in the future; STUB_Object knows how to
      // marshal itself, that will be used.
      //
      // XXX this doesn't actually verify that the stuff got written
      // OK to the "wire" ... 
      {
	CORBA_Object_ptr obj = *(CORBA_Object_ptr *) data;

	// NIL objrefs ... marshal as empty type hint, no elements.

	if (CORBA_is_nil (obj)) 
	  {
	    continue_encoding =
	      stream->put_ulong (1)	// strlen
	      && stream->put_char (0)		// NUL 
	      && stream->put_ulong (0);	// no profiles
	    break;
	  }

	// All other objrefs ... narrow to a "real type" that we
	// recognize, then marshal.
	//
	// XXX this will be changed so it narrows to STUB_Object and
	// then asks that surrogate/proxy to marshal itself.
	//
	// For now, the original code is minimally changed.

	IIOP_Object *objdata;
	IIOP::ProfileBody *profile;

	if (obj->QueryInterface (IID_IIOP_Object,
				 (void **) &objdata) != NOERROR) 
	  {
	    env.exception (new CORBA_MARSHAL (COMPLETED_NO));
	    return CORBA_TypeCode::TRAVERSE_STOP;
	  }
	obj->Release ();
	profile = &objdata->profile;

	// STRING, a type ID hint
	encoder (_tc_CORBA_String, &objdata->type_id, 0, context, env);

	// UNSIGNED LONG, value one, count of the sequence of
	// encapsulated protocol profiles;
	stream->put_ulong (1);

	// UNSIGNED LONG, tag for this protocol profile;
	stream->put_ulong (IOP::TAG_INTERNET_IOP);

	// UNSIGNED LONG, number of succeeding bytes in the
	// encapsulation.  We don't actually need to make the
	// encapsulation, as nothing needs stronger alignment than
	// this longword; it guarantees the rest is aligned for us.
	u_int hostlen;

	hostlen = ACE_OS::strlen ((char *) profile->host);
	stream->put_ulong (1				// byte order
			   + 3				// version + pad byte
			   + 4				// sizeof (strlen)
			   + hostlen + 1			// strlen + null
			   + (~hostlen & 01)		// optional pad byte
			   + 2				// port
			   + (hostlen & 02)		// optional pad short
			   + 4				// sizeof (key length)
			   + profile->object_key.length);	// key length

	// CHAR describing byte order, starting the encapsulation

	stream->put_char (MY_BYTE_SEX);

	// IIOP::Version, two characters (version 1.0) padding
	stream->put_char (profile->iiop_version.major);
	stream->put_char (profile->iiop_version.minor);

	// STRING hostname from profile
	encoder (_tc_CORBA_String, &profile->host, 0, context, env);

	// UNSIGNED SHORT port number
	stream->put_ushort (profile->port);

	// OCTET SEQUENCE for object key
	encoder (&TC_opaque, &profile->object_key, 0, context, env);
      }
    break;

    case tk_sequence:
      {
	// First marshal the sequence length, verifying that it's
	// within the sequence bounds ...
	CORBA_OctetSeq *seq = (CORBA_OctetSeq *) data;
	CORBA_ULong len = seq ? seq->length : 0;

	if (len > 0) 
	  {
	    CORBA_ULong bounds;

	    bounds = tc->ulong_param (1, env);
	    if (env.exception () != 0)
	      return CORBA_TypeCode::TRAVERSE_STOP;

	    if (bounds != 0 && len > bounds) 
	      {
		env.exception (new CORBA_BAD_PARAM (COMPLETED_MAYBE));
		return CORBA_TypeCode::TRAVERSE_STOP;
	      }
	  }
	continue_encoding = stream->put_ulong (len);

	// Fast exit on error or empty sequence
	if (!continue_encoding || len == 0)
	  break;
      }
    // FALLTHROUGH

    case tk_struct:
    case tk_union:
    case tk_array:
    case tk_alias:
      // Marshal each member in order.
      return tc->traverse (data, 0, encoder, context, env);

    case tk_except:
      // Convert the the "hidden" TypeCode at the beginning of the
      // exception into an on-the-wire ID, then marshal the members in
      // order (traversal skips that hidden typecode, and more).
      //
      // NOTE: This is asymmetric with respect to decoding the
      // exception, since whoever decodes must pull off the ID and map
      // it to the typecode to be used to unmarshal it (search among
      // legal choices).
      {
	CORBA_String id = tc->id (env);

	if (env.exception () == 0) 
	  {
	    continue_encoding =
	      encoder (_tc_CORBA_String, &id, 0, context, env)
	      == CORBA_TypeCode::TRAVERSE_CONTINUE
	      && tc->traverse (data, 0, encoder, context, env);
	  } 
	else
	  continue_encoding = CORBA_B_FALSE;
      }
    break;

    case tk_string:
      {
	CORBA_String str = *(CORBA_String *) data;
	CORBA_ULong len;
	CORBA_ULong bounds;

	// Be nice to programmers: treat nulls as empty strings not
	// errors.  (OMG-IDL supports languages that don't use the
	// C/C++ notion of null v. empty strings; nulls aren't part of
	// the OMG-IDL string model.)
	if (str == 0) 
	  {
	    stream->put_ulong (1);
	    stream->put_char (0);
	    break;
	  }

	// Verify string satisfies bounds requirements.  We're not so
	// permissive as to send messages violating the interface spec
	// by having excessively long strings!
	bounds = tc->ulong_param (0, env);
	if (env.exception () != 0)
	  return CORBA_TypeCode::TRAVERSE_STOP;
	len = ACE_OS::strlen ((char *) str);

	if (bounds != 0 && len > bounds) 
	  {
	    continue_encoding = CORBA_B_FALSE;
	    break;
	  }

	// Encode the string, followed by a NUL character.

	for (continue_encoding = stream->put_ulong (len + 1);
	     continue_encoding != CORBA_B_FALSE && *str;
	     continue_encoding = stream->put_char (*str++))
	  continue;

	stream->put_char (0);
      }
    break;

    case tk_wstring:
      {
	CORBA_WChar *str = *(CORBA_WChar **) data;
	CORBA_ULong len;
	CORBA_ULong bounds;

	// Be nice to programmers: treat nulls as empty strings not
	// errors.  (OMG-IDL supports languages that don't use the
	// C/C++ notion of null v. empty strings; nulls aren't part of
	// the OMG-IDL string model.)

	if (str == 0) 
	  {
	    stream->put_ulong (1);
	    stream->put_wchar (0);
	    break;
	  }

	// Verify wide string satisfies bounds requirements.  We're
	// not so permissive as to send messages violating the
	// interface spec by having excessively long strings!

	bounds = tc->ulong_param (0, env);
	if (env.exception () != 0)
	  return CORBA_TypeCode::TRAVERSE_STOP;
	len = wslen (str);
	if (bounds != 0 && len > bounds) 
	  {
	    continue_encoding = CORBA_B_FALSE;
	    break;
	  }

	// Encode the wide string, followed by a NUL character.

	for (continue_encoding = stream->put_ulong (wslen (str) + 1);
	     continue_encoding != CORBA_B_FALSE && *str;
	     continue_encoding = stream->put_wchar (*str++))
	  continue;

	stream->put_wchar (0);
      }
    break;

    case tk_longdouble:
      continue_encoding = stream->put_longdouble (*(CORBA_LongDouble *) data);
      break;

    case tk_wchar:
      continue_encoding = stream->put_wchar (*(CORBA_WChar *) data);
      break;
	
      // case ~0:
    default:
      dmsg ("encoder default case ?");
      continue_encoding = CORBA_B_FALSE;
      break;
    }

  if (continue_encoding == CORBA_B_FALSE) 
    {
      env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
      dmsg ("marshaling encoder detected error");
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
  return CORBA_TypeCode::TRAVERSE_CONTINUE;
}

// This table of TypeCode constants lets us unmarshal most typecodes
// using the predefined constants, rather than constantly reallocating
// them.
//
// XXX CFRONT-based compilers can't cope with this table
// initialization, and need some kind of init function.  Luckily,
// they're increasingly rare in any "production" environment.

#if THE_COMPILER_CAN_FIND_THIS_PROPERLY
// The extern reference in onc/xdr.cpp doesn't seem to be able to find
// this decl with as long as the const is part of it.  I even tried
// changing the ref decl to have 'const' in it, to no avail.  --cjc
const				
#endif
CORBA_TypeCode_ptr __tc_consts [TC_KIND_COUNT] = 
{
  _tc_CORBA_Null
  , _tc_CORBA_Void
  , _tc_CORBA_Short
  , _tc_CORBA_Long
  , _tc_CORBA_UShort

  , _tc_CORBA_ULong
  , _tc_CORBA_Float
  , _tc_CORBA_Double
  , _tc_CORBA_Boolean
  , _tc_CORBA_Char

  , _tc_CORBA_Octet
  , _tc_CORBA_Any
  , _tc_CORBA_TypeCode
  , _tc_CORBA_Principal
  , 0		// _tc_CORBA_Object ... type ID is CORBA::Object

  , 0		// tk_struct
  , 0		// tk_union
  , 0		// tk_enum
  , 0		// _tc_CORBA_String ... unbounded
  , 0		// tk_sequence

  , 0		// tk_array

  , 0		// tk_alias
  , 0		// tk_except

  , _tc_CORBA_LongLong
  , _tc_CORBA_ULongLong
  , _tc_CORBA_LongDouble
  , _tc_CORBA_WChar
  , 0		// _tc_CORBA_WString ... unbounded
};

// The decoder is exactly the reverse of the encoder, except that:
//
//	* Unmarshaling some data types involve allocating memory.  Such
//	  types include sequences (the buffer), objrefs, Principals, Anys,
//	  TypeCodes, and strings.
//
//	* The decoder is used when retrieving typecode parameters from
//	  encapsulations.  This means it must deal with "tk_indirect",
//	  the magic value (~0) signifying typecode indirection.
//
// This second case is identified by a bit of a hack: the second
// "data" value is used to hold the parent typecode, rather than being
// ignored.  This means that all other invocations of decoder() **
// MUST ** pass zero for the second data parameter, in case they
// decode a TypeCode.  If they didn't, this case might be signified
// inappropriately.
//
// XXX desirable to have a less hacky solution to that ... pull that
// code out into a separate routine called both by CDR::decoder() and
// by the code retrieving typecode parameters from encapsulations.

CORBA_TypeCode::traverse_status
CDR::decoder (CORBA_TypeCode_ptr tc,
	      const void *data,
	      const void *parent_typecode,
	      void *context,
	      CORBA_Environment &env)
{
  CORBA_Boolean	continue_decoding = CORBA_B_TRUE;
  CDR *stream = (CDR *) context;

  switch (tc->_kind) 
    {
    case tk_null:
    case tk_void:
      // nothing to decode!
      break;
      
    case tk_char:
    case tk_octet:
      continue_decoding = stream->get_char (*(CORBA_Char *) data);
      break;

    case tk_short:
    case tk_ushort:
      continue_decoding = stream->get_short (*(short *) data);
      break;

    case tk_long:
    case tk_ulong:
    case tk_float:
      continue_decoding = stream->get_long (*(CORBA_Long *) data);
      break;

    case tk_longlong:
    case tk_ulonglong:
    case tk_double:
      continue_decoding = stream->get_longlong (*(CORBA_LongLong *) data);
      break;

    case tk_boolean:
      continue_decoding = stream->get_boolean (*(CORBA_Boolean *) data);
      break;

    case tk_enum:
      {
	CORBA_ULong val;

	// NOTE assumption that this is in-range.
	//
	// XXX should check this, it's rather hard to recover from
	// such errors since they "do not occur" and are essentially
	// never tested for.
	continue_decoding = stream->get_ulong (val);
	*(u_int *) data = (u_int) val;
      }
    break;

    case tk_any:
      {
	CORBA_Any *any = (CORBA_Any *) data;
	CORBA_TypeCode_ptr tc2;
	void *value;

	if (decoder (_tc_CORBA_TypeCode,
		     &tc2, 0, 
		     context, env) != CORBA_TypeCode::TRAVERSE_CONTINUE)
	  return CORBA_TypeCode::TRAVERSE_STOP;

	value = new CORBA_Octet[tc2->size (env)];

	if (decoder (tc2, 
		     value, 0, 
		     context, env) != CORBA_TypeCode::TRAVERSE_CONTINUE) 
	  {
	    delete value;
	    CORBA_release (tc2);
	    return CORBA_TypeCode::TRAVERSE_STOP;
	  }
	(void) new (any) CORBA_Any (tc2, value, CORBA_B_TRUE);
      }
    break;

    case tk_TypeCode:
      {
	CORBA_ULong kind;
	CORBA_TypeCode_ptr *tcp;

	continue_decoding = stream->get_ulong (kind);
	if (continue_decoding == CORBA_B_FALSE)
	  break;
	if (kind >= TC_KIND_COUNT) 
	  {
	    continue_decoding = CORBA_B_FALSE;
	    break;
	  }
	    
	tcp = (CORBA_TypeCode_ptr *) data;

	// Typecodes with empty parameter lists all have preallocated
	// constants.  We use those to reduce memory consumption and
	// heap access ... also, to speed things up!

	if (((*tcp) = __tc_consts [(u_int) kind]) != 0) 
	  {
	    *tcp = __tc_consts [(u_int) kind];
	    break;
	  } 
	else 
	  switch (kind) 
	    {
	      // Need special handling for all kinds of typecodes that have
	      // nonempty parameter lists ...
	    default:			// error: missed a case! 
	      env.exception (new CORBA_INTERNAL (COMPLETED_MAYBE));
	      return CORBA_TypeCode::TRAVERSE_STOP;

	      // Some have "simple" parameter lists ... some of these also
	      // have preallocated constants that could be used.
	    case tk_string:
	    case tk_wstring:
	      {
		CORBA_ULong bound;

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

	    // Indirected typecodes, illegal at "top level" but we
	    // allow unmarshaling of them here because we use the same
	    // code to read "off the wire" (where they're illegal) and
	    // to read out of an encapsulation stream.  We distinguish
	    // the case where this is legal as described above.
	    case ~0:
	      {
		CORBA_TypeCode_ptr parent;

		if (parent_typecode == 0) 
		  {
		    env.exception (new CORBA_INTERNAL (COMPLETED_MAYBE));
		    return CORBA_TypeCode::TRAVERSE_STOP;
		  }
		parent = (CORBA_TypeCode_ptr) parent_typecode;

		// Get the long indicating the encapsulation offset,
		// then set up indirection stream that's like "stream"
		// but has space enough only for the typecode and the
		// length for the encapsulated parameters.
		CDR indir_stream;
		CORBA_Long offset;

		continue_decoding = stream->get_long (offset);
		if (continue_decoding)
		  continue_decoding = (offset < 0);
		if (continue_decoding) 
		  {
		    indir_stream.buffer = indir_stream.next
		      = stream->next + offset;
		    indir_stream.remaining = indir_stream.length = 8;

		    // Reject indirections outside parent's scope.
		    if (indir_stream.next < parent->_buffer)
		      continue_decoding = CORBA_B_FALSE;
		  }

		// Get "kind" and length of target typecode
		//
		// XXX this currently assumes the TCKind to which we
		// indirect is the same byte order as the "parent"
		// typecode -- not the right assumption; see how the
		// TypeCode interpreter does it.

		CORBA_ULong indir_kind;
		CORBA_ULong indir_len;

		if (continue_decoding)
		  continue_decoding = stream->get_ulong (indir_kind);
		if (continue_decoding
		    && indir_kind >= TC_KIND_COUNT)
		  continue_decoding = CORBA_B_FALSE;
		if (continue_decoding)
		  continue_decoding = stream->get_ulong (indir_len);

		// Now construct indirected typecode.  This shares the
		// typecode octets with the "parent" typecode,
		// increasing the amount of memory sharing and
		// reducing the cost of getting typecodes.
		if (continue_decoding) 
		  {
		    *tcp = new CORBA_TypeCode ((CORBA_TCKind) indir_kind,
					       indir_len,
					       indir_stream.next,
					       CORBA_B_FALSE);
		    (*tcp)->_parent = parent;
		    parent->AddRef ();
		  }
	      }
	    break;

	    // The rest have "complex" parameter lists that are
	    // encoded as bulk octets ...
	    case tk_objref:
	    case tk_struct:
	    case tk_union:
	    case tk_enum:
	    case tk_sequence:
	    case tk_array:
	    case tk_alias:
	    case tk_except:
	      {
		u_int len;
		u_int i;
		CORBA_ULong length;
		CORBA_Octet *buffer;

		continue_decoding = stream->get_ulong (length);
		if (!continue_decoding)
		  break;

		// if length > MAXUNSIGNED, error ...
		len = (u_int) length;

		buffer = new CORBA_Octet [len];

		for (i = 0; i < len && continue_decoding; i++)
		  continue_decoding = stream->get_octet (buffer [i]);

		if (!continue_decoding) {
		  delete buffer;
		  break;
		}
		*tcp = new CORBA_TypeCode ((CORBA_TCKind) kind,
					   len,
					   buffer,
					   CORBA_B_TRUE);
	      }
	    }
      }
    break;

    case tk_Principal:
      {
	CORBA_Principal_ptr *pp = (CORBA_Principal_ptr *) data;
	CORBA_ULong len;

	continue_decoding = stream->get_ulong (len);
	if (len == 0)
	  *pp = 0;
	else 
	  {
	    *pp = new CORBA_Principal;
	    (*pp)->id.buffer = new CORBA_Octet [(size_t) len];
	    (*pp)->id.maximum = (*pp)->id.length = len;

	    for (u_int i = 0;
		 continue_decoding != CORBA_B_FALSE && i < len;
		 i++)
	      continue_decoding = stream->get_octet ((*pp)->id.buffer [i]);
	  }
      }
    break;

    case tk_objref:
      {
	// First, read the type hint.

	CORBA_String type_hint;

	decoder (_tc_CORBA_String, &type_hint, 0, context, env);

	// Read the profiles, discarding all until an IIOP profile
	// comes by.  Once we see an IIOP profile, ignore any further
	// ones.
	//
	// XXX this will need to change someday to let different
	// protocol code be accessed, not just IIOP.  Protocol modules
	// will be dynamically loaded from shared libraries via
	// ORB_init(), and we just need to be able to access such
	// preloaded libraries here as we unmarshal objrefs.

	CORBA_ULong profiles;
	IIOP_Object *objdata = 0;

	stream->get_ulong (profiles);

	// No profiles means a NIL objref.

	if (profiles == 0) 
	  {
	    *(CORBA_Object_ptr *) data = CORBA_Object::_nil ();
	    delete type_hint;
	    break;
	  } 

	while (profiles-- != 0 && continue_decoding) 
	  {
	    CORBA_ULong tmp;

	    stream->get_ulong (tmp);

	    if (tmp != IOP::TAG_INTERNET_IOP || objdata != 0) 
	      {
		continue_decoding = stream->skip_string ();
		continue;
	      }

	    // OK, we've got an IIOP profile.  It's going to be
	    // encapsulated ProfileData.  Create a new decoding stream
	    // and context for it, and tell the "parent" stream that
	    // this data isn't part of it any more.

	    continue_decoding = stream->get_ulong (tmp);
	    assert (stream->remaining >= tmp);

	    // Create the decoding stream from the encapsulation in
	    // the buffer, and skip the encapsulation.
	    CDR str;

	    str.setup_encapsulation (stream->next, (size_t) tmp);

	    stream->next += (u_int) tmp;
	    stream->remaining -= (u_int) tmp;

	    objdata = new IIOP_Object (type_hint);

	    IIOP::ProfileBody	*profile = &objdata->profile;

	    // Read and verify major, minor versions, ignoring IIOP
	    // profiles whose versions we don't understand.
	    //
	    // XXX this doesn't actually go back and skip the whole
	    // encapsulation...
	    if (!(str.get_octet (profile->iiop_version.major)
		  && profile->iiop_version.major == IIOP::MY_MAJOR
		  && str.get_octet (profile->iiop_version.minor)
		  && profile->iiop_version.minor <= IIOP::MY_MINOR)) 
	      {
		dmsg2 ("detected new v%d.%d IIOP profile",
		       profile->iiop_version.major,
		       profile->iiop_version.minor);
		objdata->type_id = 0;
		objdata->Release ();
		objdata = 0;
		continue;
	      }
		    
	    // Get host and port
	    if (decoder (_tc_CORBA_String, &profile->host, 0, &str, env)
		!= CORBA_TypeCode::TRAVERSE_CONTINUE
		|| !str.get_ushort (profile->port)) 
	      {
		env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
		dmsg ("error decoding IIOP host/port");
		objdata->Release ();
		return CORBA_TypeCode::TRAVERSE_STOP;
	      }

	    // ... and object key

	    continue_decoding = decoder (&TC_opaque, 
					 &profile->object_key,
					 0,
					 &str,
					 env) == CORBA_TypeCode::TRAVERSE_CONTINUE;

	    if (str.remaining != 0) 
	      {
		env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
		dmsg ("extra data at end of IIOP profile data");
		objdata->Release ();
		return CORBA_TypeCode::TRAVERSE_STOP;
	      }
	  }
	if (objdata == 0) 
	  {
	    env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
	    dmsg2 ("no IIOP v%d.%d (or earlier) profile in IOR!", 
		   IIOP::MY_MAJOR, IIOP::MY_MINOR);
	    return CORBA_TypeCode::TRAVERSE_STOP;
	  } 
	else 
	  {
	    if (objdata->QueryInterface (IID_CORBA_Object,
					 (void **) data) != NOERROR)
	      continue_decoding = CORBA_B_FALSE;
	    objdata->Release ();
	  }
      }
    break;

    case tk_sequence:
      {
	// First unmarshal the sequence length ... we trust it to be
	// right here, on the "be gracious in what you accept"
	// principle.  We don't generate illegal sequences
	// (i.e. length > bounds).

	CORBA_OctetSeq *seq = (CORBA_OctetSeq *) data;

	continue_decoding = stream->get_ulong (seq->length);
	seq->maximum = seq->length;
	seq->buffer = 0;

	// Fast exit on empty sequences or errors
	if (!continue_decoding || seq->length == 0)
	  break;

	// ... then allocate the memory into which we'll unmarshal
	CORBA_TypeCode_ptr tc2;
	size_t size;

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
      // Unmarshal all the individual elements using the per-member
      // description held in the "parent" TypeCode.
	
      // FALLTHROUGH

    case tk_except:
      // For exceptions, the "hidden" type ID near the front of the
      // on-wire representation was previously unmarshaled and mapped
      // to the "tc" typcode we're using to traverse the memory ...
      // at the same time its vtable, refcount, and other state was
      // established.
      //
      // NOTE:  This is asymmetric with respect to encoding exceptions.
      return tc->traverse (data, 0, decoder, context, env);

    case tk_string:
      {
	CORBA_String str;
	CORBA_ULong len = 0;

	// On decode, omit the check against specified string bounds,
	// and cope with illegal "zero length" strings (all lengths on
	// the wire must include a NUL).
	//
	// This is on the principle of being gracious in what we
	// accept; we don't generate messages that fail to comply with
	// protocol specs, but we will accept them when it's clear how
	// to do so.

	continue_decoding = stream->get_ulong (len);
	*((CORBA_String*) data) = str = new CORBA_Char [(size_t) (len)];

	if (len != 0)
	  while (continue_decoding != CORBA_B_FALSE && len-- != 0) 
	    {
	      continue_decoding = stream->get_char (*(CORBA_Char *) str);
	      str++;
	    }
	break;
      }

    case tk_wstring:
      {
	CORBA_WChar *str;
	CORBA_ULong len = 0;

	// On decode, omit the check against specified wstring bounds,
	// and cope with illegal "zero length" strings (all lengths on
	// the wire must include a NUL).
	//
	// This is on the principle of being gracious in what we
	// accept; we don't generate messages that fail to comply with
	// protocol specs, but we will accept them when it's clear how
	// to do so.

	continue_decoding = stream->get_ulong (len);

	*((CORBA_WChar **) data) = str = new CORBA_WChar [(size_t) (len)];

	if (len != 0) 
	  while (continue_decoding != CORBA_B_FALSE && len--) 
	    {
	      continue_decoding = stream->get_wchar (*str);
	      str++;
	    }
      }
    break;

    case tk_longdouble:
      continue_decoding = stream->get_longdouble (*(CORBA_LongDouble *) data);
      break;

    case tk_wchar:
      continue_decoding = stream->get_wchar (*(CORBA_WChar *) data);
      break;

      // case ~0:
    default:
      continue_decoding = CORBA_B_FALSE;
      dmsg ("decode, default case?");
      break;
    }

  if (continue_decoding == CORBA_B_FALSE) 
    {
      env.exception (new CORBA_MARSHAL (COMPLETED_NO));
      dmsg ("marshaling decoder detected error");
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
  return CORBA_TypeCode::TRAVERSE_CONTINUE;
}

