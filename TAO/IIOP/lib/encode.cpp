// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    encode.cpp
//
// = DESCRIPTION
//   Code for encoding different data types
//
//   The original code had a single static encoder function defined on the CDR
//   class that called traverse to interpret the data types. This version
//   defines a virtual method "encode" on each class and avoids calling traverse.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//     and 
//     Aniruddha Gokhale
// 
// ============================================================================

#include <assert.h>
#include <limits.h>
#include <string.h>

#include        "orb.h"
#include	"cdr.h"
#include        "giop.h"

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

CORBA_TypeCode::traverse_status
TAO_Marshal_Primitive::encode(
		      CORBA_TypeCode_ptr  tc,
		      const void		*data,
		      const void		*parent_typecode,
		      void		*context,
		      CORBA_Environment	&env
	        )
{
  CORBA_Boolean	continue_encoding = CORBA_B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA_TypeCode::traverse_status   retval =
    CORBA_TypeCode::TRAVERSE_CONTINUE; // status of encode operation 

  switch(tc->_kind)
    {
    case tk_null:
    case tk_void:
      break;
    case tk_short:
    case tk_ushort:
      continue_encoding = stream->put_short (*(CORBA_Short *)data);
      break;
    case tk_long:
    case tk_ulong:
    case tk_float:
    case tk_enum:
      continue_encoding = stream->put_long(*(CORBA_Long *)data);
      break;
    case tk_double:
    case tk_longlong:
    case tk_ulonglong:
      continue_encoding = stream->put_longlong (*(CORBA_LongLong *)data);
      break;
    case tk_boolean:
      continue_encoding = stream->put_boolean (*(CORBA_Boolean *)data);
      break;
    case tk_char:
    case tk_octet:
      continue_encoding = stream->put_char(*(CORBA_Char *)data);
      break;
    case tk_longdouble:
      continue_encoding = stream->put_longdouble(*(CORBA_LongDouble *)data);
      break;
    case tk_wchar:
      continue_encoding = stream->put_wchar(*(wchar_t *)data);
      break;
    default:
      retval = CORBA_TypeCode::TRAVERSE_STOP;
      // we are not a primitive type
    }
  if ((retval == CORBA_TypeCode::TRAVERSE_CONTINUE) && (continue_encoding ==
							CORBA_B_TRUE)) 
    {
      return CORBA_TypeCode::TRAVERSE_CONTINUE;
    } 
  else 
    {
      env.exception (new CORBA_MARSHAL(COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Primitive::encode detected error");
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
}

CORBA_TypeCode::traverse_status
TAO_Marshal_Any::encode(
		      CORBA_TypeCode_ptr  tc,
		      const void		*data,
		      const void		*parent_typecode,
		      void		*context,
		      CORBA_Environment	&env
	        )
{
  CORBA_Any		*any = (CORBA_Any *)data;
  CORBA_TypeCode_ptr	elem_tc;  // typecode of the element that makes the Any
  void			*value;   // value maintained by the Any
  CORBA_Boolean	continue_encoding = CORBA_B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA_TypeCode::traverse_status   retval =
    CORBA_TypeCode::TRAVERSE_CONTINUE; // status of encode operation 

  elem_tc = any->type();
  // encode the typecode description for the element
  if (stream->encode(_tc_CORBA_TypeCode, &elem_tc, 0, env)
      == CORBA_TypeCode::TRAVERSE_CONTINUE) {
    value = any->value();
    // switch on the data type and handle the cases for primitives here for
    // efficiency rather than calling 
    switch(elem_tc->_kind)
      {
      case tk_null:
      case tk_void:
	break;
      case tk_short:
      case tk_ushort:
	continue_encoding = stream->put_short (*(CORBA_Short *)value);
	break;
      case tk_long:
      case tk_ulong:
      case tk_float:
      case tk_enum:
	continue_encoding = stream->put_long(*(CORBA_Long *)value);
	break;
      case tk_double:
      case tk_longlong:
      case tk_ulonglong:
	continue_encoding = stream->put_longlong (*(CORBA_LongLong *)value);
	break;
      case tk_boolean:
	continue_encoding = stream->put_boolean (*(CORBA_Boolean *)value);
	break;
      case tk_char:
      case tk_octet:
	continue_encoding = stream->put_char(*(CORBA_Char *)value);
	break;
      case tk_longdouble:
	continue_encoding = stream->put_longdouble(*(CORBA_LongDouble *)value);
	break;
      case tk_wchar:
	continue_encoding = stream->put_wchar(*(wchar_t *)value);
	break;
      case tk_any:
      case tk_TypeCode:
      case tk_Principal:
      case tk_objref:
      case tk_struct:
      case tk_union:
      case tk_string:
      case tk_sequence:
      case tk_array:
      case tk_alias:
      case tk_except:
      case tk_wstring:
	retval = stream->encode(elem_tc, value, 0, env);
	break;
      default:
	// anything else is an error
	retval = CORBA_TypeCode::TRAVERSE_STOP;
      }
  }
  if ((retval == CORBA_TypeCode::TRAVERSE_CONTINUE) && (continue_encoding ==
							  CORBA_B_TRUE)) 
    {
      return CORBA_TypeCode::TRAVERSE_CONTINUE;
    } 
  else 
    {
      env.exception (new CORBA_MARSHAL(COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Any::encode detected error");
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
}

CORBA_TypeCode::traverse_status
TAO_Marshal_TypeCode::encode(
			     CORBA_TypeCode_ptr  tc,
			     const void		*data,
			     const void		*parent_typecode,
			     void		*context,
			     CORBA_Environment	&env
			     )
{
  CORBA_Boolean	continue_encoding = CORBA_B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA_TypeCode_ptr tc2;         // typecode to be encoded

  tc2 = *(CORBA_TypeCode_ptr *) data;  // the data has to be a TypeCode_ptr

  // encode the "kind" field of the typecode
  continue_encoding = stream->put_ulong ((CORBA_ULong) tc2->_kind);
  if (continue_encoding == CORBA_B_TRUE)
    {
      // now encode the parameters, if any
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
  if (continue_encoding == CORBA_B_TRUE)
    {
      return CORBA_TypeCode::TRAVERSE_CONTINUE;
    }
  else 
    {
      env.exception (new CORBA_MARSHAL(COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_TypeCode::encode detected error");
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
}

// encode Principal
CORBA_TypeCode::traverse_status
TAO_Marshal_Principal::encode(
		      CORBA_TypeCode_ptr  tc,
		      const void		*data,
		      const void		*parent_typecode,
		      void		*context,
		      CORBA_Environment	&env
		     )
{
  CORBA_Boolean	continue_encoding = CORBA_B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA_TypeCode::traverse_status retval = CORBA_TypeCode::TRAVERSE_CONTINUE;

  CORBA_Principal_ptr p = *(CORBA_Principal_ptr *) data;

  if (p != 0) 
    {
      continue_encoding = stream->put_long (p->id.length);

      for (u_int i = 0; continue_encoding && i < p->id.length; i++)
	continue_encoding = stream->put_octet (p->id.buffer [i]);
    } 
  else
    {
      continue_encoding = stream->put_long (0);
    }
  if (continue_encoding == CORBA_B_TRUE)
    {
      return CORBA_TypeCode::TRAVERSE_CONTINUE;
    }
  else 
    {
      env.exception (new CORBA_MARSHAL(COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Principal::encode detected error");
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
}

// encode obj ref
CORBA_TypeCode::traverse_status
TAO_Marshal_ObjRef::encode(
		      CORBA_TypeCode_ptr  tc,
		      const void		*data,
		      const void		*parent_typecode,
		      void		*context,
		      CORBA_Environment	&env
		     )
{
  CORBA_Boolean	continue_encoding = CORBA_B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA_TypeCode::traverse_status retval = CORBA_TypeCode::TRAVERSE_CONTINUE;

  // Current version:  objref is really an IIOP_Object.
  //
  // This will change in the future; STUB_Object knows how to
  // marshal itself, that will be used.
  //
  // XXX this doesn't actually verify that the stuff got written
  // OK to the "wire" ... 
  CORBA_Object_ptr obj = *(CORBA_Object_ptr *) data;

  // NIL objrefs ... marshal as empty type hint, no elements.

  if (CORBA_is_nil (obj)) 
    {
      continue_encoding =
	stream->put_ulong (1)	// strlen
	&& stream->put_char (0)		// NUL 
	&& stream->put_ulong (0);	// no profiles
      return CORBA_TypeCode::TRAVERSE_CONTINUE;
    }
  else
    {

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
      stream->encode(_tc_CORBA_String, &objdata->type_id, 0, env);

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
      stream->encode(_tc_CORBA_String, &profile->host, 0, env);

      // UNSIGNED SHORT port number
      stream->put_ushort (profile->port);

      // OCTET SEQUENCE for object key
      stream->encode (&TC_opaque, &profile->object_key, 0, env);
      return CORBA_TypeCode::TRAVERSE_CONTINUE;
    }
}

// encode structs
CORBA_TypeCode::traverse_status
TAO_Marshal_Struct::encode (
		      CORBA_TypeCode_ptr  tc,
		      const void		*data,
		      const void		*parent_typecode,
		      void		*context,
		      CORBA_Environment	&env
		      )
{
  CDR			*stream = (CDR *)context;
  CORBA_Long i, 
    member_count;      // number of fields in the struct
  CORBA_TypeCode::traverse_status retval = CORBA_TypeCode::TRAVERSE_CONTINUE;
  CORBA_Boolean	continue_encoding = CORBA_B_TRUE;
  CORBA_TypeCode_ptr param;
  CORBA_Long size, alignment;

  member_count = tc->member_count(env);
  if (env.exception() == 0)
    {
      for(i=0; i < member_count && retval ==
	    CORBA_TypeCode::TRAVERSE_CONTINUE && continue_encoding ==
	    CORBA_B_TRUE; i++){ 
	param = tc->member_type(i, env);
	if (env.exception() == 0){
	  size = param->size(env);
	  if (env.exception() == 0){
	    alignment = param->alignment(env);
	    if (env.exception() == 0){
	      data = ptr_align_binary(data, alignment);
	      switch(param->_kind){
	      case tk_null:
	      case tk_void:
		break;
	      case tk_short:
	      case tk_ushort:
		continue_encoding = stream->put_short (*(CORBA_Short *)data);
		break;
	      case tk_long:
	      case tk_ulong:
	      case tk_float:
	      case tk_enum:
		continue_encoding = stream->put_long(*(CORBA_Long *)data);
		break;
	      case tk_double:
	      case tk_longlong:
	      case tk_ulonglong:
		continue_encoding = stream->put_longlong (*(CORBA_LongLong *)data);
		break;
	      case tk_boolean:
		continue_encoding = stream->put_boolean (*(CORBA_Boolean *)data);
		break;
	      case tk_char:
	      case tk_octet:
		continue_encoding = stream->put_char(*(CORBA_Char *)data);
		break;
	      case tk_longdouble:
		continue_encoding = stream->put_longdouble(*(CORBA_LongDouble *)data);
		break;
	      case tk_wchar:
		continue_encoding = stream->put_wchar(*(wchar_t *)data);
		break;
	      case tk_any:
	      case tk_TypeCode:
	      case tk_Principal:
	      case tk_objref:
	      case tk_struct:
	      case tk_union:
	      case tk_string:
	      case tk_sequence:
	      case tk_array:
	      case tk_alias:
	      case tk_except:
	      case tk_wstring:
		retval = stream->encode(param, data, 0, env);
		break;
	      default:
		break;
	      }
	      data = (char *)data + size;
	    } else {
	      return CORBA_TypeCode::TRAVERSE_STOP;
	    }
	  } else {
	    return CORBA_TypeCode::TRAVERSE_STOP;
	  }
	} else {
	  return CORBA_TypeCode::TRAVERSE_STOP;
	}
      }
    } else {
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
  if (retval == CORBA_TypeCode::TRAVERSE_CONTINUE && continue_encoding ==
      CORBA_B_TRUE){ 
    return CORBA_TypeCode::TRAVERSE_CONTINUE;
  } else {
    env.exception (new CORBA_MARSHAL(COMPLETED_MAYBE));
    dmsg ("marshaling encode_struct detected error");
    return CORBA_TypeCode::TRAVERSE_STOP;
  }
}

// encode unions
CORBA_TypeCode::traverse_status
TAO_Marshal_Union::encode(
		      CORBA_TypeCode_ptr  tc,
		      const void		*data,
		      const void		*data2,
		      void		*context,
		      CORBA_Environment	&env
		     )
{
  CORBA_Boolean	continue_encoding = CORBA_B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA_TypeCode::traverse_status retval = CORBA_TypeCode::TRAVERSE_CONTINUE;

  CORBA_TypeCode_ptr discrim_tc;
  CORBA_TypeCode_ptr member_tc;
  CORBA_Any_ptr member_label;
  CORBA_ULong discrim_size_with_pad;
  const void *discrim_val;
  CORBA_ULong member_count;
  CORBA_Long  default_index;
  CORBA_ULong i;
  CORBA_TypeCode_ptr default_tc = 0;
  CORBA_Boolean discrim_matched = CORBA_B_FALSE;

  discrim_tc = tc->discriminator_type(env);
  // get the discriminator type
  if (env.exception() == 0)
    {
      // encode the discriminator value
      retval = stream->encode(discrim_tc, data, data2, env);
      if (retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
	{
	  discrim_size_with_pad = tc->TAO_discrim_pad_size(env);
	  if (env.exception() == 0)
	    {
	      discrim_val = data; // save the pointer to the discriminator
				  // value 
	      // move the pointer to point to the actual value
	      data = (char *)data + discrim_size_with_pad;
	      data2 = (char *)data2 + discrim_size_with_pad;
	      // now get ready to marshal the actual union value
	      default_index = tc->default_index(env);
	      if (env.exception() == 0)
		{
		  member_count = tc->member_count(env);
		  if (env.exception () == 0)
		    {
		      // check which label value matches with the discriminator
		      // value. Accordingly, marshal the corresponding
		      // member_type. If none match, check if default exists
		      // and marshal accordingly. Otherwise it is an error.
		      i = 0;
		      while (member_count-- != 0)
			{
			  member_label = tc->member_label(i, env);
			  if (env.exception() == 0)
			    {
			      // do the matching
			      switch (member_label->type()->kind(env))
				{
				case tk_short:
				case tk_ushort:
				  if (*(CORBA_Short *)member_label->value() ==
				      *(CORBA_Short *)discrim_val)
				    {
				      discrim_matched = CORBA_B_TRUE;
				    }
				  break;
				case tk_long:
				case tk_ulong:
				case tk_enum:
				  if (*(CORBA_ULong *)member_label->value() ==
				      *(CORBA_ULong *)discrim_val)
				    {
				      discrim_matched = CORBA_B_TRUE;
				    }
				  break;
				case tk_char:
				  if (*(CORBA_Char *)member_label->value() ==
				      *(CORBA_Char *)discrim_val)
				    {
				      discrim_matched = CORBA_B_TRUE;
				    }
				  break;
				case tk_wchar:
				  if (*(CORBA_WChar *)member_label->value() ==
				      *(CORBA_WChar *)discrim_val)
				    {
				      discrim_matched = CORBA_B_TRUE;
				    }
				  break;
				case tk_boolean:
				  if (*(CORBA_Boolean *)member_label->value() ==
				      *(CORBA_Boolean *)discrim_val)
				    {
				      discrim_matched = CORBA_B_TRUE;
				    }
				  break;
				default:
				  env.exception (new CORBA_BAD_TYPECODE(COMPLETED_NO));
				  dmsg("Union::encode - Bad discriminant type");
				  return CORBA_TypeCode::TRAVERSE_STOP;
				}// end of switch

			      // get the member typecode
			      member_tc = tc->member_type (i, env);
			      if (env.exception() == 0)
				{
				  if (default_index >= 0 && default_index-- == 0)
				    {
				      // have we reached the default label?, if so,
				      // save a handle to the typecode for the default
				      default_tc = member_tc;
				    }
				  if (discrim_matched)
				    {
				      // marshal according to the matched typecode
				      return stream->encode(member_tc, data,
							    data2, env);
				    }
				}
			      else // error getting member type
				{
				  env.exception(new CORBA_BAD_TYPECODE(COMPLETED_NO));
				  dmsg1 ("Union::encode - error getting member type:%d",i);
				  return CORBA_TypeCode::TRAVERSE_STOP;
				}
			      
			    }
			  else // error getting member label
			    {
			      env.exception(new CORBA_BAD_TYPECODE(COMPLETED_NO));
			      dmsg1("Union::encode - error member label : %d", i);
			      return CORBA_TypeCode::TRAVERSE_STOP;
			    }
			  i++;
			} // end of while
		      // we are here only if there was no match
		      if (default_tc)
			{
			  return stream->encode(default_tc, data, data2, env);
			}
		      else
			{
			  return CORBA_TypeCode::TRAVERSE_CONTINUE;
			}
		    } 
		  else // error getting member count
		    {
		      env.exception(new CORBA_MARSHAL(COMPLETED_MAYBE));
		      dmsg ("Union::encode - error getting member count");
		      return CORBA_TypeCode::TRAVERSE_STOP;
		    }
		}
	      else // error getting default index
		{
		  env.exception(new CORBA_BAD_TYPECODE(COMPLETED_NO));
		  dmsg ("Union::encode - error getting default used");
		  return CORBA_TypeCode::TRAVERSE_STOP;
		}
	    }
	  else // error getting discrim_pad_size
	    {
	      env.exception(new CORBA_BAD_TYPECODE(COMPLETED_NO));
	      dmsg ("Union::encode - error getting discrim padded size");
	      return CORBA_TypeCode::TRAVERSE_STOP;
	    }
	}
      else // error encoding discriminant
	{
	  env.exception(new CORBA_MARSHAL(COMPLETED_MAYBE));
	  dmsg ("Union::encode - error encoding discriminant");
	  return CORBA_TypeCode::TRAVERSE_STOP;
	}
    }
  else // error getting the discriminant
    {
      env.exception(new CORBA_BAD_TYPECODE(COMPLETED_NO));
      dmsg ("Union::encode - error getting the discriminant typecode");
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
}

// encode string
CORBA_TypeCode::traverse_status
TAO_Marshal_String::encode (
		      CORBA_TypeCode_ptr  tc,
		      const void		*data,
		      const void		*parent_typecode,
		      void		*context,
		      CORBA_Environment	&env
	        )
{
  CORBA_Boolean	continue_encoding = CORBA_B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA_TypeCode::traverse_status retval = CORBA_TypeCode::TRAVERSE_CONTINUE;
  CORBA_String str = *(CORBA_String *) data;
  CORBA_ULong len;
  CORBA_ULong bounds;

  // Be nice to programmers: treat nulls as empty strings not
  // errors.  (OMG-IDL supports languages that don't use the
  // C/C++ notion of null v. empty strings; nulls aren't part of
  // the OMG-IDL string model.)
  if (str != 0) 
    {

      // Verify string satisfies bounds requirements.  We're not so
      // permissive as to send messages violating the interface spec
      // by having excessively long strings!
      bounds = tc->length(env);
      if (env.exception () == 0)
	{
	  // get the actual length of the string
	  len = ACE_OS::strlen ((char *) str);

	  // if it is an unbounded string or if the length is less than the
	  // bounds for an unbounded string
	  if ((bounds == 0) || (len <= bounds))
	    {

	      // Encode the string, followed by a NUL character.

	      for (continue_encoding = stream->put_ulong (len + 1);
		   continue_encoding != CORBA_B_FALSE && *str;
		   continue_encoding = stream->put_char (*str++))
		continue;

	      stream->put_char (0);
	      return CORBA_TypeCode::TRAVERSE_CONTINUE;
	    }
	  else
	    {
	      return CORBA_TypeCode::TRAVERSE_STOP;
	    }
	}
      else
	{
	  return CORBA_TypeCode::TRAVERSE_STOP;
	}
    }
  else 
    {
      // empty string
      stream->put_ulong (1);
      stream->put_char (0);
      return CORBA_TypeCode::TRAVERSE_CONTINUE;
    }
}

// encode sequence
CORBA_TypeCode::traverse_status
TAO_Marshal_Sequence::encode (
			      CORBA_TypeCode_ptr  tc,
			      const void		*data,
			      const void		*parent_typecode,
			      void		*context,
			      CORBA_Environment	&env
			      )
{
  CORBA_Boolean	continue_encoding = CORBA_B_TRUE;  
  CDR			*stream = (CDR *)context;
  CORBA_OctetSeq 		*seq = (CORBA_OctetSeq *) data;
  CORBA_TypeCode::traverse_status retval =
    CORBA_TypeCode::TRAVERSE_CONTINUE;  // return status
  CORBA_TypeCode_ptr	tc2;  // typecode of the element
  size_t  size; // size of element
  CORBA_ULong  bounds;
  CORBA_ULong  len = seq ? seq->length : 0;
  char *value;

  //
  // First marshal the sequence length, verifying that
  // it's within the sequence bounds ...
  //
  if (len > 0)
    {
      // retrieve the bounds of the sequence
      bounds = tc->length (env);
      if (env.exception () == 0)
	{
	  // encode only if it is an unbounded sequence or if length is
	  // less/equal to the bounds
	  if (bounds == 0 || len <= bounds) 
	    {
	      bounds = len;  // number of times you encode
	      continue_encoding = stream->put_ulong (seq->length);
	      if (continue_encoding && seq->length != 0) 
		{
		  // get element typecode
		  tc2 = tc->content_type (env);  
		  if (env.exception () == 0)
		    {
		      size = tc2->size (env);
		      if (env.exception() == 0)
			{
			  value = (char *)seq->buffer;
			  switch(tc2->_kind)
			    {
			    case tk_null:
			    case tk_void:
			      return CORBA_TypeCode::TRAVERSE_CONTINUE;
			    case tk_short:
			    case tk_ushort:
			      // For primitives, compute the size only once
			      while (bounds-- && continue_encoding == CORBA_B_TRUE)
				{
				  continue_encoding = stream->put_short (*(CORBA_Short *)value);
				  value += size;
				}
			      //			      CORBA_release(tc2);
			      if (continue_encoding == CORBA_B_TRUE)
				{
				  return CORBA_TypeCode::TRAVERSE_CONTINUE;
				}
			      break;
			    case tk_long:
			    case tk_ulong:
			    case tk_float:
			      // For primitives, compute the size only once
			      while (bounds-- && continue_encoding == CORBA_B_TRUE)
				{
				  continue_encoding = stream->put_long (*(CORBA_Long *)value);
				  value += size;
				}
			      //			      CORBA_release(tc2);
			      if (continue_encoding == CORBA_B_TRUE)
				{
				  return CORBA_TypeCode::TRAVERSE_CONTINUE;
				}
			      break;
			    case tk_double:
			    case tk_longlong:
			    case tk_ulonglong:
			      // For primitives, compute the size only once
			      while (bounds-- && continue_encoding == CORBA_B_TRUE)
				{
				  continue_encoding = stream->put_longlong (*(CORBA_LongLong *)value);
				  value += size;
				}
			      //			      CORBA_release(tc2);
			      if (continue_encoding == CORBA_B_TRUE)
				{
				  return CORBA_TypeCode::TRAVERSE_CONTINUE;
				}
			      break;
			    case tk_boolean:
			      // For primitives, compute the size only once
			      while (bounds-- && continue_encoding == CORBA_B_TRUE)
				{
				  continue_encoding = stream->put_boolean (*(CORBA_Boolean *)value);
				  value += size;
				}
			      //			      CORBA_release(tc2);
			      if (continue_encoding == CORBA_B_TRUE)
				{
				  return CORBA_TypeCode::TRAVERSE_CONTINUE;
				}
			      break;
			    case tk_char:
			    case tk_octet:
			      // For primitives, compute the size only once
			      while (bounds-- && continue_encoding == CORBA_B_TRUE)
				{
				  continue_encoding = stream->put_char (*(CORBA_Char *)value);
				  value += size;
				}
			      //			      CORBA_release(tc2);
			      if (continue_encoding == CORBA_B_TRUE)
				{
				  return CORBA_TypeCode::TRAVERSE_CONTINUE;
				}
			      break;
			    case tk_longdouble:
			      // For primitives, compute the size only once
			      while (bounds-- && continue_encoding == CORBA_B_TRUE)
				{
				  continue_encoding = stream->put_longdouble (*(CORBA_LongDouble *)value);
				  value += size;
				}
			      //			      CORBA_release(tc2);
			      if (continue_encoding == CORBA_B_TRUE)
				{
				  return CORBA_TypeCode::TRAVERSE_CONTINUE;
				}
			      break;
			    case tk_wchar:
			      // For primitives, compute the size only once
			      while (bounds-- && continue_encoding == CORBA_B_TRUE)
				{
				  continue_encoding = stream->put_wchar (*(CORBA_WChar *)value);
				  value += size;
				}
			      //			      CORBA_release(tc2);
			      if (continue_encoding == CORBA_B_TRUE)
				{
				  return CORBA_TypeCode::TRAVERSE_CONTINUE;
				}
			      break;
			    case tk_enum:
			      // For primitives, compute the size only once
			      while (bounds-- && continue_encoding == CORBA_B_TRUE)
				{
				  continue_encoding = stream->put_long (*(CORBA_Long *)value);
				  value += size;
				}
			      //			      CORBA_release(tc2);
			      if (continue_encoding == CORBA_B_TRUE)
				{
				  return CORBA_TypeCode::TRAVERSE_CONTINUE;
				}
			      break;
			      // handle all aggregate types here
			    case tk_any:
			    case tk_TypeCode:
			    case tk_Principal:
			    case tk_objref:
			    case tk_struct:
			    case tk_union:
			    case tk_string:
			    case tk_sequence:
			    case tk_array:
			    case tk_alias:
			    case tk_except:
			    case tk_wstring:
			      // For those aggregate types whose size is constant, we
			      // compute it only once
			      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
				{
				  retval = stream->encode(tc2, value, 0, env);
				  value += size;
				}
			      //			      CORBA_release(tc2);
			      if (retval == CORBA_TypeCode::TRAVERSE_CONTINUE){
				return CORBA_TypeCode::TRAVERSE_CONTINUE;
			      }
			      break;
			    default:
			      break;
			    } // end of switch
			} // no exception computing size
		    } // no exception computing content type
		} // seq length not 0
	    } // within bounds or unbounded
	} // no exception computing bounds
    } // length is > 0
  else 
    {
      // length is 0, encode it
      continue_encoding = stream->put_ulong (len);
      if (continue_encoding == CORBA_B_TRUE) {
	return CORBA_TypeCode::TRAVERSE_CONTINUE;
      }
    }
  // error exit
  env.exception (new CORBA_MARSHAL(COMPLETED_NO));
  dmsg ("marshaling TAO_Marshal_Sequence::encode detected error");
  return CORBA_TypeCode::TRAVERSE_STOP;
}

// encode array
CORBA_TypeCode::traverse_status
TAO_Marshal_Array::encode (
			   CORBA_TypeCode_ptr  tc,
			   const void		*data,
			   const void		*parent_typecode,
			   void		*context,
			   CORBA_Environment	&env
			   )
{
  CORBA_Boolean	continue_encoding = CORBA_B_TRUE;  
  CDR			*stream = (CDR *)context;
  CORBA_TypeCode::traverse_status retval =
    CORBA_TypeCode::TRAVERSE_CONTINUE;  // return status
  CORBA_TypeCode_ptr	tc2;  // typecode of the element
  size_t  size; // size of element
  CORBA_ULong  bounds;
  char *value = (char *)data;

  // retrieve the bounds of the array
  bounds = tc->length (env);
  if (env.exception () == 0)
    {

      // get element typecode
      tc2 = tc->content_type (env);  
      if (env.exception () == 0)
	{
	  size = tc2->size (env);
	  if (env.exception() == 0)
	    {
	      switch(tc2->_kind)
		{
		case tk_null:
		case tk_void:
		  return CORBA_TypeCode::TRAVERSE_CONTINUE;
		case tk_short:
		case tk_ushort:
		  // For primitives, compute the size only once
		  while (bounds-- && continue_encoding == CORBA_B_TRUE)
		    {
		      continue_encoding = stream->put_short (*(CORBA_Short *)value);
		      value += size;
		    }
		  //		  CORBA_release(tc2);
		  if (continue_encoding == CORBA_B_TRUE)
		    {
		      return CORBA_TypeCode::TRAVERSE_CONTINUE;
		    }
		  break;
		case tk_long:
		case tk_ulong:
		case tk_float:
		  // For primitives, compute the size only once
		  while (bounds-- && continue_encoding == CORBA_B_TRUE)
		    {
		      continue_encoding = stream->put_long (*(CORBA_Long *)value);
		      value += size;
		    }
		  //		  CORBA_release(tc2);
		  if (continue_encoding == CORBA_B_TRUE)
		    {
		      return CORBA_TypeCode::TRAVERSE_CONTINUE;
		    }
		  break;
		case tk_double:
		case tk_longlong:
		case tk_ulonglong:
		  // For primitives, compute the size only once
		  while (bounds-- && continue_encoding == CORBA_B_TRUE)
		    {
		      continue_encoding = stream->put_longlong (*(CORBA_LongLong *)value);
		      value += size;
		    }
		  //		  CORBA_release(tc2);
		  if (continue_encoding == CORBA_B_TRUE)
		    {
		      return CORBA_TypeCode::TRAVERSE_CONTINUE;
		    }
		  break;
		case tk_boolean:
		  // For primitives, compute the size only once
		  while (bounds-- && continue_encoding == CORBA_B_TRUE)
		    {
		      continue_encoding = stream->put_boolean (*(CORBA_Boolean *)value);
		      value += size;
		    }
		  //		  CORBA_release(tc2);
		  if (continue_encoding == CORBA_B_TRUE)
		    {
		      return CORBA_TypeCode::TRAVERSE_CONTINUE;
		    }
		  break;
		case tk_char:
		case tk_octet:
		  // For primitives, compute the size only once
		  while (bounds-- && continue_encoding == CORBA_B_TRUE)
		    {
		      continue_encoding = stream->put_char (*(CORBA_Char *)value);
		      value += size;
		    }
		  //		  CORBA_release(tc2);
		  if (continue_encoding == CORBA_B_TRUE)
		    {
		      return CORBA_TypeCode::TRAVERSE_CONTINUE;
		    }
		  break;
		case tk_longdouble:
		  // For primitives, compute the size only once
		  while (bounds-- && continue_encoding == CORBA_B_TRUE)
		    {
		      continue_encoding = stream->put_longdouble (*(CORBA_LongDouble *)value);
		      value += size;
		    }
		  //		  CORBA_release(tc2);
		  if (continue_encoding == CORBA_B_TRUE)
		    {
		      return CORBA_TypeCode::TRAVERSE_CONTINUE;
		    }
		  break;
		case tk_wchar:
		  // For primitives, compute the size only once
		  while (bounds-- && continue_encoding == CORBA_B_TRUE)
		    {
		      continue_encoding = stream->put_wchar (*(CORBA_WChar *)value);
		      value += size;
		    }
		  //		  CORBA_release(tc2);
		  if (continue_encoding == CORBA_B_TRUE)
		    {
		      return CORBA_TypeCode::TRAVERSE_CONTINUE;
		    }
		  break;
		case tk_enum:
		  // For primitives, compute the size only once
		  while (bounds-- && continue_encoding == CORBA_B_TRUE)
		    {
		      continue_encoding = stream->put_long (*(CORBA_Long *)value);
		      value += size;
		    }
		  //		  CORBA_release(tc2);
		  if (continue_encoding == CORBA_B_TRUE)
		    {
		      return CORBA_TypeCode::TRAVERSE_CONTINUE;
		    }
		  break;
		  // handle all aggregate types here
		case tk_any:
		case tk_TypeCode:
		case tk_Principal:
		case tk_objref:
		case tk_struct:
		case tk_union:
		case tk_string:
		case tk_sequence:
		case tk_array:
		case tk_alias:
		case tk_except:
		case tk_wstring:
		  // For those aggregate types whose size is constant, we
		  // compute it only once
		  while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
		    {
		      retval = stream->encode(tc2, value, 0, env);
		      value += size;
		    }
		  //		  CORBA_release(tc2);
		  if (retval == CORBA_TypeCode::TRAVERSE_CONTINUE){
		    return CORBA_TypeCode::TRAVERSE_CONTINUE;
		  }
		  break;
		default:
		  break;
		} // end of switch
	    } // no exception computing size
	} // no exception computing content type
    } // no exception computing bounds
  // error exit
  env.exception (new CORBA_MARSHAL(COMPLETED_NO));
  dmsg ("marshaling TAO_Marshal_Sequence::encode detected error");
  return CORBA_TypeCode::TRAVERSE_STOP;
}

CORBA_TypeCode::traverse_status
TAO_Marshal_Alias::encode(
		      CORBA_TypeCode_ptr  tc,
		      const void		*data,
		      const void		*parent_typecode,
		      void		*context,
		      CORBA_Environment	&env
	        )
{
  CORBA_TypeCode_ptr	tc2;  // typecode of the aliased type
  CORBA_Boolean	continue_encoding = CORBA_B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA_TypeCode::traverse_status   retval =
    CORBA_TypeCode::TRAVERSE_CONTINUE; // status of encode operation 
  char *value = (char *)data;

  tc2 = tc->content_type(env);
  if (env.exception() == 0)
    {
    // switch on the data type and handle the cases for primitives here for
    // efficiency rather than calling 
    switch(tc2->_kind)
      {
      case tk_null:
      case tk_void:
	break;
      case tk_short:
      case tk_ushort:
	continue_encoding = stream->put_short (*(CORBA_Short *)value);
	break;
      case tk_long:
      case tk_ulong:
      case tk_float:
      case tk_enum:
	continue_encoding = stream->put_long(*(CORBA_Long *)value);
	break;
      case tk_double:
      case tk_longlong:
      case tk_ulonglong:
	continue_encoding = stream->put_longlong (*(CORBA_LongLong *)value);
	break;
      case tk_boolean:
	continue_encoding = stream->put_boolean (*(CORBA_Boolean *)value);
	break;
      case tk_char:
      case tk_octet:
	continue_encoding = stream->put_char(*(CORBA_Char *)value);
	break;
      case tk_longdouble:
	continue_encoding = stream->put_longdouble(*(CORBA_LongDouble *)value);
	break;
      case tk_wchar:
	continue_encoding = stream->put_wchar(*(wchar_t *)value);
	break;
      case tk_any:
      case tk_TypeCode:
      case tk_Principal:
      case tk_objref:
      case tk_struct:
      case tk_union:
      case tk_string:
      case tk_sequence:
      case tk_array:
      case tk_alias:
      case tk_except:
      case tk_wstring:
	retval = stream->encode(tc2, value, 0, env);
	break;
      default:
	// anything else is an error
	retval = CORBA_TypeCode::TRAVERSE_STOP;
      }
  }
  //  tc2->Release();
  if ((retval == CORBA_TypeCode::TRAVERSE_CONTINUE) && (continue_encoding ==
							  CORBA_B_TRUE)) 
    {
      return CORBA_TypeCode::TRAVERSE_CONTINUE;
    } 
  else 
    {
      env.exception (new CORBA_MARSHAL(COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Alias::encode detected error");
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
}


// encode exception
CORBA_TypeCode::traverse_status
TAO_Marshal_Except::encode (
		      CORBA_TypeCode_ptr  tc,
		      const void		*data,
		      const void		*parent_typecode,
		      void		*context,
		      CORBA_Environment	&env
		      )
{
  CDR			*stream = (CDR *)context;
  CORBA_Long i, 
    member_count;      // number of fields in the struct
  CORBA_TypeCode::traverse_status retval = CORBA_TypeCode::TRAVERSE_CONTINUE;
  CORBA_Boolean	continue_encoding = CORBA_B_TRUE;
  CORBA_TypeCode_ptr param;
  CORBA_Long size, alignment;

  data = (char *)data + sizeof(CORBA_Exception);

  member_count = tc->member_count(env);
  if (env.exception() == 0)
    {
      for(i=0; i < member_count && retval ==
	    CORBA_TypeCode::TRAVERSE_CONTINUE && continue_encoding ==
	    CORBA_B_TRUE; i++){ 
	param = tc->member_type(i, env);
	if (env.exception() == 0){
	  size = param->size(env);
	  if (env.exception() == 0){
	    alignment = param->alignment(env);
	    if (env.exception() == 0){
	      data = ptr_align_binary(data, alignment);
	      switch(param->_kind){
	      case tk_null:
	      case tk_void:
		break;
	      case tk_short:
	      case tk_ushort:
		continue_encoding = stream->put_short (*(CORBA_Short *)data);
		break;
	      case tk_long:
	      case tk_ulong:
	      case tk_float:
	      case tk_enum:
		continue_encoding = stream->put_long(*(CORBA_Long *)data);
		break;
	      case tk_double:
	      case tk_longlong:
	      case tk_ulonglong:
		continue_encoding = stream->put_longlong (*(CORBA_LongLong *)data);
		break;
	      case tk_boolean:
		continue_encoding = stream->put_boolean (*(CORBA_Boolean *)data);
		break;
	      case tk_char:
	      case tk_octet:
		continue_encoding = stream->put_char(*(CORBA_Char *)data);
		break;
	      case tk_longdouble:
		continue_encoding = stream->put_longdouble(*(CORBA_LongDouble *)data);
		break;
	      case tk_wchar:
		continue_encoding = stream->put_wchar(*(wchar_t *)data);
		break;
	      case tk_any:
	      case tk_TypeCode:
	      case tk_Principal:
	      case tk_objref:
	      case tk_struct:
	      case tk_union:
	      case tk_string:
	      case tk_sequence:
	      case tk_array:
	      case tk_alias:
	      case tk_except:
	      case tk_wstring:
		retval = stream->encode(param, data, 0, env);
		break;
	      default:
		break;
	      }
	      data = (char *)data + size;
	    } else {
	      return CORBA_TypeCode::TRAVERSE_STOP;
	    }
	  } else {
	    return CORBA_TypeCode::TRAVERSE_STOP;
	  }
	} else {
	  return CORBA_TypeCode::TRAVERSE_STOP;
	}
      }
    } else {
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
  if (retval == CORBA_TypeCode::TRAVERSE_CONTINUE && continue_encoding ==
      CORBA_B_TRUE){ 
    return CORBA_TypeCode::TRAVERSE_CONTINUE;
  } else {
    env.exception (new CORBA_MARSHAL(COMPLETED_MAYBE));
    dmsg ("TAO_Marshal_Except detected error");
    return CORBA_TypeCode::TRAVERSE_STOP;
  }
}


// encode wstring
CORBA_TypeCode::traverse_status
TAO_Marshal_WString::encode (
		      CORBA_TypeCode_ptr  tc,
		      const void		*data,
		      const void		*parent_typecode,
		      void		*context,
		      CORBA_Environment	&env
	        )
{
  CORBA_Boolean	continue_encoding = CORBA_B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA_TypeCode::traverse_status retval = CORBA_TypeCode::TRAVERSE_CONTINUE;
  CORBA_WChar *str = *(CORBA_WChar **) data;
  CORBA_ULong len;
  CORBA_ULong bounds;

  // Be nice to programmers: treat nulls as empty strings not
  // errors.  (OMG-IDL supports languages that don't use the
  // C/C++ notion of null v. empty strings; nulls aren't part of
  // the OMG-IDL string model.)
  if (str != 0) 
    {

      // Verify string satisfies bounds requirements.  We're not so
      // permissive as to send messages violating the interface spec
      // by having excessively long strings!
      bounds = tc->length(env);
      if (env.exception () == 0)
	{
	  // get the actual length of the string
	  len = wslen ((CORBA_WChar *) str);

	  // if it is an unbounded string or if the length is less than the
	  // bounds for an unbounded string
	  if ((bounds == 0) || (len <= bounds))
	    {

	      // Encode the string, followed by a NUL character.

	      for (continue_encoding = stream->put_ulong (len + 1);
		   continue_encoding != CORBA_B_FALSE && *str;
		   continue_encoding = stream->put_wchar (*str++))
		continue;

	      stream->put_wchar (0);
	    }
	  else
	    {
	      return CORBA_TypeCode::TRAVERSE_STOP;
	    }
	}
      else
	{
	  return CORBA_TypeCode::TRAVERSE_STOP;
	}
    }
  else 
    {
      // empty string
      stream->put_ulong (1);
      stream->put_wchar (0);
      return CORBA_TypeCode::TRAVERSE_CONTINUE;
    }
}

