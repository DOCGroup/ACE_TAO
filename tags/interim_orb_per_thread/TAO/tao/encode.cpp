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
//     and Aniruddha Gokhale
// 
// ============================================================================

#if 0
#include "tao/orb.h"
#include "tao/cdr.h"
#include "tao/giop.h"
#include "tao/debug.h"
#endif

#include "tao/corba.h"

#if     defined (HAVE_WIDEC_H)
#               include <widec.h>
#else
extern "C" 
{
  u_int wslen (const CORBA::WChar *);
  CORBA::WChar *wscpy (CORBA::WChar *, const CORBA::WChar *);
}
#endif /* HAVE_WIDEC_H */

extern CORBA::TypeCode TC_opaque;

// Encode instances of arbitrary data types based only on typecode.
// "data" points to the data type; if it's not a primitve data type,
// the TypeCode interpreter is used to recursively encode its
// components.  "context" is the marshaling stream on which to encode
// the data value.
//

CORBA::TypeCode::traverse_status
TAO_Marshal_Primitive::encode (CORBA::TypeCode_ptr tc,
			       const void *data,
			       const void *,
			       void *context,
			       CORBA::Environment &env)
{
  CORBA::Boolean continue_encoding = CORBA::B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE; // status of encode operation 

  switch (tc->_kind)
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
      break;
    case CORBA::tk_short:
    case CORBA::tk_ushort:
      continue_encoding = stream->put_short (*(CORBA::Short *) data);
      break;
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_enum:
      continue_encoding = stream->put_long (*(CORBA::Long *) data);
      break;
    case CORBA::tk_double:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
      continue_encoding = stream->put_longlong (*(CORBA::LongLong *) data);
      break;
    case CORBA::tk_boolean:
      continue_encoding = stream->put_boolean (*(CORBA::Boolean *) data);
      break;
    case CORBA::tk_char:
    case CORBA::tk_octet:
      continue_encoding = stream->put_char (*(CORBA::Char *) data);
      break;
    case CORBA::tk_longdouble:
      continue_encoding = stream->put_longdouble (*(CORBA::LongDouble *) data);
      break;
    case CORBA::tk_wchar:
      continue_encoding = stream->put_wchar (*(wchar_t *) data);
      break;
    default:
      retval = CORBA::TypeCode::TRAVERSE_STOP;
      // we are not a primitive type
    }

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_encoding == CORBA::B_TRUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Primitive::encode detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

CORBA::TypeCode::traverse_status
TAO_Marshal_Any::encode (CORBA::TypeCode_ptr,
			 const void *data,
			 const void *,
			 void *context,
			 CORBA::Environment &env)
{
  CORBA::Any *any = (CORBA::Any *) data;

  // Typecode of the element that makes the Any.
  CORBA::TypeCode_ptr elem_tc;

  // Value maintained by the Any.
  void *value;   

  CORBA::Boolean continue_encoding = CORBA::B_TRUE;

  // Context is the CDR stream.
  CDR *stream = (CDR *) context;  

  // Status of encode operation 
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE; 

  elem_tc = any->type ();

  // Encode the typecode description for the element.
  if (stream->encode (CORBA::_tc_TypeCode, &elem_tc, 0, env)
      == CORBA::TypeCode::TRAVERSE_CONTINUE) {
    value = (void *) any->value ();

    // Switch on the data type and handle the cases for primitives
    // here for efficiency rather than calling.
    switch (elem_tc->_kind)
      {
      case CORBA::tk_null:
      case CORBA::tk_void:
        break;
      case CORBA::tk_short:
      case CORBA::tk_ushort:
        continue_encoding = stream->put_short (*(CORBA::Short *) value);
        break;
      case CORBA::tk_long:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_enum:
        continue_encoding = stream->put_long (*(CORBA::Long *) value);
        break;
      case CORBA::tk_double:
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
        continue_encoding = stream->put_longlong (*(CORBA::LongLong *) value);
        break;
      case CORBA::tk_boolean:
        continue_encoding = stream->put_boolean (*(CORBA::Boolean *) value);
        break;
      case CORBA::tk_char:
      case CORBA::tk_octet:
        continue_encoding = stream->put_char (*(CORBA::Char *) value);
        break;
      case CORBA::tk_longdouble:
        continue_encoding = stream->put_longdouble (*(CORBA::LongDouble *) value);
        break;
      case CORBA::tk_wchar:
        continue_encoding = stream->put_wchar (*(wchar_t *) value);
        break;
      case CORBA::tk_any:
      case CORBA::tk_TypeCode:
      case CORBA::tk_Principal:
      case CORBA::tk_objref:
      case CORBA::tk_struct:
      case CORBA::tk_union:
      case CORBA::tk_string:
      case CORBA::tk_sequence:
      case CORBA::tk_array:
      case CORBA::tk_alias:
      case CORBA::tk_except:
      case CORBA::tk_wstring:
        retval = stream->encode (elem_tc, value, 0, env);
        break;
      default:
        // anything else is an error
        retval = CORBA::TypeCode::TRAVERSE_STOP;
      }
  }

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_encoding == CORBA::B_TRUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Any::encode detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

CORBA::TypeCode::traverse_status
TAO_Marshal_TypeCode::encode (CORBA::TypeCode_ptr,
			      const void *data,
			      const void *,
			      void *context,
			      CORBA::Environment &env)
{
  CORBA::Boolean continue_encoding = CORBA::B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA::TypeCode_ptr tc2;         // typecode to be encoded

  tc2 = *(CORBA::TypeCode_ptr *) data;  // the data has to be a TypeCode_ptr

  // encode the "kind" field of the typecode
  continue_encoding = stream->put_ulong ((CORBA::ULong) tc2->_kind);
  if (continue_encoding == CORBA::B_TRUE)
    {
      // now encode the parameters, if any
      switch (tc2->_kind) 
        {
          // Most TypeCodes have empty parameter lists
        default:
          break;

          // A few have "simple" parameter lists
        case CORBA::tk_string:
        case CORBA::tk_wstring:
          continue_encoding = stream->put_ulong (tc2->_length);
          break;

          // Indirected typecodes can't occur at "top level" like
          // this, only nested inside others!
        case ~0:
          dmsg ("indirected typecode at top level!");
          continue_encoding = CORBA::B_FALSE;
          break;

          // The rest have "complex" parameter lists that are
          // already encoded as bulk octets ... put length, then
          // octets.
        case CORBA::tk_objref:
        case CORBA::tk_struct:
        case CORBA::tk_union:
        case CORBA::tk_enum:
        case CORBA::tk_sequence:
        case CORBA::tk_array:
        case CORBA::tk_alias:
        case CORBA::tk_except:
          {
            continue_encoding = stream->put_ulong (tc2->_length);

            for (u_int i = 0; i < tc2->_length && continue_encoding; i++)
              continue_encoding = stream->put_octet (tc2->_buffer [i]);
          }
        }
    }
  if (continue_encoding == CORBA::B_TRUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_TypeCode::encode detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// encode Principal
CORBA::TypeCode::traverse_status
TAO_Marshal_Principal::encode (CORBA::TypeCode_ptr,
			       const void *data,
			       const void *,
			       void *context,
			       CORBA::Environment &env)
{
  CORBA::Boolean continue_encoding = CORBA::B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream

  CORBA::Principal_ptr p = *(CORBA::Principal_ptr *) data;

  if (p != 0) 
    {
      continue_encoding = stream->put_long (p->id.length);

      for (u_int i = 0;
	   continue_encoding && i < p->id.length;
	   i++)
        continue_encoding = stream->put_octet (p->id.buffer [i]);
    } 
  else
    continue_encoding = stream->put_long (0);
  if (continue_encoding == CORBA::B_TRUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Principal::encode detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// encode obj ref
CORBA::TypeCode::traverse_status
TAO_Marshal_ObjRef::encode (CORBA::TypeCode_ptr,
			    const void *data,
			    const void *,
			    void *context,
			    CORBA::Environment &env)
{
  CORBA::Boolean continue_encoding = CORBA::B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream

  // Current version:  objref is really an IIOP_Object.
  //
  // This will change in the future; STUB_Object knows how to
  // marshal itself, that will be used.
  //
  // XXX this doesn't actually verify that the stuff got written
  // OK to the "wire" ... 
  CORBA::Object_ptr obj = *(CORBA::Object_ptr *) data;

  // NIL objrefs ... marshal as empty type hint, no elements.

  if (CORBA::is_nil (obj)) 
    {
      continue_encoding =
        stream->put_ulong (1)   // strlen
        && stream->put_char (0)         // NUL 
        && stream->put_ulong (0);       // no profiles
      return CORBA::TypeCode::TRAVERSE_CONTINUE;
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

      if (obj->QueryInterface (IID_IIOP_Object, (void **) &objdata) != NOERROR) 
        {
          env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
          return CORBA::TypeCode::TRAVERSE_STOP;
        }
      obj->Release ();
      profile = &objdata->profile;

      // STRING, a type ID hint
      stream->encode (CORBA::_tc_string, &objdata->type_id, 0, env);

      // UNSIGNED LONG, value one, count of the sequence of
      // encapsulated protocol profiles;
      stream->put_ulong (1);

      // UNSIGNED LONG, tag for this protocol profile;
      stream->put_ulong (TAO_IOP_TAG_INTERNET_IOP);

      // UNSIGNED LONG, number of succeeding bytes in the
      // encapsulation.  We don't actually need to make the
      // encapsulation, as nothing needs stronger alignment than
      // this longword; it guarantees the rest is aligned for us.
      u_int hostlen;

      hostlen = ACE_OS::strlen ((char *) profile->host);
      stream->put_ulong (1                              // byte order
                         + 3                            // version + pad byte
                         + 4                            // sizeof (strlen)
                         + hostlen + 1                  // strlen + null
                         + (~hostlen & 01)              // optional pad byte
                         + 2                            // port
                         + (hostlen & 02)               // optional pad short
                         + 4                            // sizeof (key length)
                         + profile->object_key.length); // key length

      // CHAR describing byte order, starting the encapsulation

      stream->put_char (MY_BYTE_SEX);

      // IIOP::Version, two characters (version 1.0) padding
      stream->put_char (profile->iiop_version.major);
      stream->put_char (profile->iiop_version.minor);

      // STRING hostname from profile
      stream->encode (CORBA::_tc_string, &profile->host, 0, env);

      // UNSIGNED SHORT port number
      stream->put_ushort (profile->port);

      // OCTET SEQUENCE for object key
      stream->encode (&TC_opaque, &profile->object_key, 0, env);
      return CORBA::TypeCode::TRAVERSE_CONTINUE;
    }
}

// encode structs
CORBA::TypeCode::traverse_status
TAO_Marshal_Struct::encode (CORBA::TypeCode_ptr tc,
			    const void *data,
			    const void *,
			    void *context,
			    CORBA::Environment &env)
{
  CDR *stream = (CDR *) context;
  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::Boolean continue_encoding = CORBA::B_TRUE;
  CORBA::TypeCode_ptr param;
  CORBA::Long size, alignment;

  if (env.exception () == 0)
    {
      int member_count = tc->member_count (env);

      for (int i = 0; 
	   i < member_count && retval == CORBA::TypeCode::TRAVERSE_CONTINUE 
	     && continue_encoding == CORBA::B_TRUE;
	   i++)
	{
	  param = tc->member_type (i, env);
	  if (env.exception () == 0)
	    {
	      size = param->size (env);
	      if (env.exception () == 0)
		{
		  alignment = param->alignment (env);
		  if (env.exception () == 0)
		    {
		      data = ptr_align_binary (data, alignment);
		      switch (param->_kind)
			{
			case CORBA::tk_null:
			case CORBA::tk_void:
			  break;
			case CORBA::tk_short:
			case CORBA::tk_ushort:
			  continue_encoding = stream->put_short (*(CORBA::Short *) data);
			  break;
			case CORBA::tk_long:
			case CORBA::tk_ulong:
			case CORBA::tk_float:
			case CORBA::tk_enum:
			  continue_encoding = stream->put_long (*(CORBA::Long *) data);
			  break;
			case CORBA::tk_double:
			case CORBA::tk_longlong:
			case CORBA::tk_ulonglong:
			  continue_encoding = stream->put_longlong (*(CORBA::LongLong *) data);
			  break;
			case CORBA::tk_boolean:
			  continue_encoding = stream->put_boolean (*(CORBA::Boolean *) data);
			  break;
			case CORBA::tk_char:
			case CORBA::tk_octet:
			  continue_encoding = stream->put_char (*(CORBA::Char *) data);
			  break;
			case CORBA::tk_longdouble:
			  continue_encoding = stream->put_longdouble (*(CORBA::LongDouble *) data);
			  break;
			case CORBA::tk_wchar:
			  continue_encoding = stream->put_wchar (*(wchar_t *) data);
			  break;
			case CORBA::tk_any:
			case CORBA::tk_TypeCode:
			case CORBA::tk_Principal:
			case CORBA::tk_objref:
			case CORBA::tk_struct:
			case CORBA::tk_union:
			case CORBA::tk_string:
			case CORBA::tk_sequence:
			case CORBA::tk_array:
			case CORBA::tk_alias:
			case CORBA::tk_except:
			case CORBA::tk_wstring:
			  retval = stream->encode (param, data, 0, env);
			  break;
			default:
			  break;
			}
		      data = (char *) data + size;
		    } 
		  else 
		    return CORBA::TypeCode::TRAVERSE_STOP;
		} 
	      else 
		return CORBA::TypeCode::TRAVERSE_STOP;
	    } 
	  else 
	    return CORBA::TypeCode::TRAVERSE_STOP;
	}
    } 
  else 
      return CORBA::TypeCode::TRAVERSE_STOP;

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE 
      && continue_encoding == CORBA::B_TRUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("marshaling encode_struct detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// encode unions
CORBA::TypeCode::traverse_status
TAO_Marshal_Union::encode (CORBA::TypeCode_ptr tc,
			   const void *data,
			   const void *data2,
			   void *context,
			   CORBA::Environment &env)
{
  CDR *stream = (CDR *) context;  // context is the CDR stream

  CORBA::TypeCode_ptr discrim_tc = tc->discriminator_type (env);
  // get the discriminator type

  if (env.exception () == 0)
    {
      CORBA::TypeCode_ptr member_tc;
      CORBA::Any_ptr member_label;
      CORBA::ULong discrim_size_with_pad;
      const void *discrim_val;
      CORBA::ULong member_count;
      CORBA::Long default_index;
      CORBA::TypeCode_ptr default_tc = 0;
      CORBA::Boolean discrim_matched = CORBA::B_FALSE;

      // encode the discriminator value
      CORBA::TypeCode::traverse_status retval = 
	stream->encode (discrim_tc, data, data2, env);

      if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          discrim_size_with_pad = tc->TAO_discrim_pad_size (env);
          if (env.exception () == 0)
            {
              discrim_val = data; // save the pointer to the discriminator
				  // value 
              // move the pointer to point to the actual value
              data = (char *) data + discrim_size_with_pad;
              data2 = (char *) data2 + discrim_size_with_pad;
              // now get ready to marshal the actual union value
              default_index = tc->default_index (env);
              if (env.exception () == 0)
                {
		  // get the member count
                  member_count = tc->member_count (env);
                  if (env.exception () == 0)
                    {
                      // Check which label value matches with the
                      // discriminator value. Accordingly, marshal the
                      // corresponding member_type. If none match,
                      // check if default exists and marshal
                      // accordingly. Otherwise it is an error.
                      for (int i = 0; member_count-- != 0; i++)
                        {
                          member_label = tc->member_label (i, env);
                          if (env.exception () == 0)
                            {
                              // do the matching
                              switch (member_label->type ()->kind (env))
                                {
                                case CORBA::tk_short:
                                case CORBA::tk_ushort:
                                  if (*(CORBA::Short *) member_label->value () == *(CORBA::Short *) discrim_val)
				    discrim_matched = CORBA::B_TRUE;
                                  break;
                                case CORBA::tk_long:
                                case CORBA::tk_ulong:
                                case CORBA::tk_enum:
                                  if (*(CORBA::ULong *) member_label->value () == *(CORBA::ULong *) discrim_val)
				    discrim_matched = CORBA::B_TRUE;
                                  break;
                                case CORBA::tk_char:
                                  if (*(CORBA::Char *) member_label->value () == *(CORBA::Char *) discrim_val)
				    discrim_matched = CORBA::B_TRUE;
                                  break;
                                case CORBA::tk_wchar:
                                  if (*(CORBA::WChar *) member_label->value () == *(CORBA::WChar *) discrim_val)
				    discrim_matched = CORBA::B_TRUE;
                                  break;
                                case CORBA::tk_boolean:
                                  if (*(CORBA::Boolean *) member_label->value () == *(CORBA::Boolean *) discrim_val)
				    discrim_matched = CORBA::B_TRUE;
                                  break;
                                default:
                                  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
                                  dmsg ("Union::encode - Bad discriminant type");
                                  return CORBA::TypeCode::TRAVERSE_STOP;
                                }// end of switch

                              // get the member typecode
                              member_tc = tc->member_type (i, env);
                              if (env.exception () == 0)
                                {
                                  if (default_index >= 0 && default_index-- == 0)
                                    {
                                      // have we reached the default label?, if so,
                                      // save a handle to the typecode for the default
                                      default_tc = member_tc;
                                    }
                                  if (discrim_matched)
				    // marshal according to the matched typecode
				    return stream->encode (member_tc, data,
							   data2, env);
                                }
                              else // error getting member type
                                {
                                  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
                                  dmsg1 ("Union::encode - error getting member type:%d",i);
                                  return CORBA::TypeCode::TRAVERSE_STOP;
                                }
                              
                            }
                          else // error getting member label
                            {
                              env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
                              dmsg1 ("Union::encode - error member label : %d", i);
                              return CORBA::TypeCode::TRAVERSE_STOP;
                            }
                        } // end of while
                      // we are here only if there was no match
                      if (default_tc)
			return stream->encode (default_tc, data, data2, env);
                      else
			{
			  env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
			  dmsg ("Union::encode - failed. No match and no default case");
			  return CORBA::TypeCode::TRAVERSE_STOP;
			}
                    } 
                  else // error getting member count
                    {
                      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
                      dmsg ("Union::encode - error getting member count");
                      return CORBA::TypeCode::TRAVERSE_STOP;
                    }
                }
              else // error getting default index
                {
                  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
                  dmsg ("Union::encode - error getting default used");
                  return CORBA::TypeCode::TRAVERSE_STOP;
                }
            }
          else // error getting discrim_pad_size
            {
              env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
              dmsg ("Union::encode - error getting discrim padded size");
              return CORBA::TypeCode::TRAVERSE_STOP;
            }
        }
      else // error encoding discriminant
        {
          env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
          dmsg ("Union::encode - error encoding discriminant");
          return CORBA::TypeCode::TRAVERSE_STOP;
        }
    }
  else // error getting the discriminant
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
      dmsg ("Union::encode - error getting the discriminant typecode");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// encode string
CORBA::TypeCode::traverse_status
TAO_Marshal_String::encode (CORBA::TypeCode_ptr tc,
			    const void *data,
			    const void *,
			    void *context,
			    CORBA::Environment &env)
{
  CORBA::Boolean continue_encoding = CORBA::B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA::String str = *(CORBA::String *) data;

  // Be nice to programmers: treat nulls as empty strings not
  // errors. (OMG-IDL supports languages that don't use the C/C++
  // notion of null v. empty strings; nulls aren't part of the OMG-IDL
  // string model.)
  if (str != 0) 
    {
      // Verify string satisfies bounds requirements.  We're not so
      // permissive as to send messages violating the interface spec
      // by having excessively long strings!
      CORBA::ULong bounds = tc->length (env);

      if (env.exception () == 0)
        {
          // get the actual length of the string
	  CORBA::ULong len = ACE_OS::strlen ((char *) str);

          // if it is an unbounded string or if the length is less
          // than the bounds for an unbounded string
          if (bounds == 0 || len <= bounds)
            {

              // Encode the string, followed by a NUL character.

              for (continue_encoding = stream->put_ulong (len + 1); // length +
								    // 1 for
								    // the NULL
								    // character
                   continue_encoding != CORBA::B_FALSE && *str;
                   continue_encoding = stream->put_char (*str++))
                continue;
	      // put a NULL terminating character
              stream->put_char (0);
              return CORBA::TypeCode::TRAVERSE_CONTINUE;
            }
          else
	    return CORBA::TypeCode::TRAVERSE_STOP;
        }
      else
	return CORBA::TypeCode::TRAVERSE_STOP;
    }
  else 
    {
      // empty string
      stream->put_ulong (1);
      stream->put_char (0);
      return CORBA::TypeCode::TRAVERSE_CONTINUE;
    }
}

// encode sequence
CORBA::TypeCode::traverse_status
TAO_Marshal_Sequence::encode (CORBA::TypeCode_ptr tc,
                              const void *data,
                              const void *,
                              void *context,
                              CORBA::Environment &env)
{
  CORBA::Boolean continue_encoding = CORBA::B_TRUE;  
  CDR *stream = (CDR *) context;
  CORBA::OctetSeq *seq = (CORBA::OctetSeq *) data;
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;  // return status
  CORBA::TypeCode_ptr    tc2;  // typecode of the element
  size_t  size; // size of element
  CORBA::ULong  len = seq ? seq->length : 0;
  char *value;

  // First marshal the sequence length, verifying that it's within the
  // sequence bounds ...

  if (len > 0)
    {
      // retrieve the bounds of the sequence
      CORBA::ULong bounds = tc->length (env);
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
                      if (env.exception () == 0)
                        {
                          value = (char *) seq->buffer;
                          switch (tc2->_kind)
                            {
                            case CORBA::tk_null:
                            case CORBA::tk_void:
                              return CORBA::TypeCode::TRAVERSE_CONTINUE;
                            case CORBA::tk_short:
                            case CORBA::tk_ushort:
                              // For primitives, compute the size only once
                              while (bounds-- && continue_encoding == CORBA::B_TRUE)
                                {
                                  continue_encoding = stream->put_short (*(CORBA::Short *) value);
                                  value += size;
                                }
                              //                              CORBA::release (tc2);
                              if (continue_encoding == CORBA::B_TRUE)
				return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;
                            case CORBA::tk_long:
                            case CORBA::tk_ulong:
                            case CORBA::tk_float:
                              // For primitives, compute the size only once
                              while (bounds-- && continue_encoding == CORBA::B_TRUE)
                                {
                                  continue_encoding = stream->put_long (*(CORBA::Long *) value);
                                  value += size;
                                }
                              //                              CORBA::release (tc2);
                              if (continue_encoding == CORBA::B_TRUE)
				return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;
                            case CORBA::tk_double:
                            case CORBA::tk_longlong:
                            case CORBA::tk_ulonglong:
                              // For primitives, compute the size only once
                              while (bounds-- && continue_encoding == CORBA::B_TRUE)
                                {
                                  continue_encoding = stream->put_longlong (*(CORBA::LongLong *) value);
                                  value += size;
                                }
                              //                              CORBA::release (tc2);
                              if (continue_encoding == CORBA::B_TRUE)
				return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;
                            case CORBA::tk_boolean:
                              // For primitives, compute the size only once
                              while (bounds-- && continue_encoding == CORBA::B_TRUE)
                                {
                                  continue_encoding = stream->put_boolean (*(CORBA::Boolean *) value);
                                  value += size;
                                }
                              //                              CORBA::release (tc2);
                              if (continue_encoding == CORBA::B_TRUE)
				return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;
                            case CORBA::tk_char:
                            case CORBA::tk_octet:
                              // For primitives, compute the size only once
                              while (bounds-- && continue_encoding == CORBA::B_TRUE)
                                {
                                  continue_encoding = stream->put_char (*(CORBA::Char *) value);
                                  value += size;
                                }
                              //                              CORBA::release (tc2);
                              if (continue_encoding == CORBA::B_TRUE)
				return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;
                            case CORBA::tk_longdouble:
                              // For primitives, compute the size only once
                              while (bounds-- && continue_encoding == CORBA::B_TRUE)
                                {
                                  continue_encoding = stream->put_longdouble (*(CORBA::LongDouble *) value);
                                  value += size;
                                }
                              //                              CORBA::release (tc2);
                              if (continue_encoding == CORBA::B_TRUE)
				return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;
                            case CORBA::tk_wchar:
                              // For primitives, compute the size only once
                              while (bounds-- && continue_encoding == CORBA::B_TRUE)
                                {
                                  continue_encoding = stream->put_wchar (*(CORBA::WChar *) value);
                                  value += size;
                                }
                              //                              CORBA::release (tc2);
                              if (continue_encoding == CORBA::B_TRUE)
				return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;
                            case CORBA::tk_enum:
                              // For primitives, compute the size only once
                              while (bounds-- && continue_encoding == CORBA::B_TRUE)
                                {
                                  continue_encoding = stream->put_long (*(CORBA::Long *) value);
                                  value += size;
                                }
                              //                              CORBA::release (tc2);
                              if (continue_encoding == CORBA::B_TRUE)
				return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;
                              // handle all aggregate types here
                            case CORBA::tk_any:
                            case CORBA::tk_TypeCode:
                            case CORBA::tk_Principal:
                            case CORBA::tk_objref:
                            case CORBA::tk_struct:
                            case CORBA::tk_union:
                            case CORBA::tk_string:
                            case CORBA::tk_sequence:
                            case CORBA::tk_array:
                            case CORBA::tk_alias:
                            case CORBA::tk_except:
                            case CORBA::tk_wstring:
                              // For those aggregate types whose size is constant, we
                              // compute it only once
                              while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                                {
                                  retval = stream->encode (tc2, value, 0, env);
                                  value += size;
                                }
                              //                              CORBA::release (tc2);
                              if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                                return CORBA::TypeCode::TRAVERSE_CONTINUE;
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
      if (continue_encoding == CORBA::B_TRUE) 
        return CORBA::TypeCode::TRAVERSE_CONTINUE;

    }
  // error exit
  env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
  dmsg ("marshaling TAO_Marshal_Sequence::encode detected error");
  return CORBA::TypeCode::TRAVERSE_STOP;
}

// encode array
CORBA::TypeCode::traverse_status
TAO_Marshal_Array::encode (CORBA::TypeCode_ptr tc,
                           const void *data,
                           const void *,
                           void *context,
                           CORBA::Environment &env)
{
  CORBA::Boolean continue_encoding = CORBA::B_TRUE;  
  CDR *stream = (CDR *) context;
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;  // return status
  size_t  size; // size of element
  CORBA::ULong  bounds;
  char *value = (char *) data;

  // retrieve the bounds of the array
  bounds = tc->length (env);
  if (env.exception () == 0)
    {
      // get element typecode.
      CORBA::TypeCode_ptr tc2 = tc->content_type (env);  

      if (env.exception () == 0)
        {
          size = tc2->size (env);
          if (env.exception () == 0)
            {
              switch (tc2->_kind)
                {
                case CORBA::tk_null:
                case CORBA::tk_void:
                  return CORBA::TypeCode::TRAVERSE_CONTINUE;
                case CORBA::tk_short:
                case CORBA::tk_ushort:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_encoding == CORBA::B_TRUE)
                    {
                      continue_encoding = stream->put_short (*(CORBA::Short *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_encoding == CORBA::B_TRUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                case CORBA::tk_long:
                case CORBA::tk_ulong:
                case CORBA::tk_float:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_encoding == CORBA::B_TRUE)
                    {
                      continue_encoding = stream->put_long (*(CORBA::Long *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_encoding == CORBA::B_TRUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                case CORBA::tk_double:
                case CORBA::tk_longlong:
                case CORBA::tk_ulonglong:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_encoding == CORBA::B_TRUE)
                    {
                      continue_encoding = stream->put_longlong (*(CORBA::LongLong *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_encoding == CORBA::B_TRUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                case CORBA::tk_boolean:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_encoding == CORBA::B_TRUE)
                    {
                      continue_encoding = stream->put_boolean (*(CORBA::Boolean *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_encoding == CORBA::B_TRUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                case CORBA::tk_char:
                case CORBA::tk_octet:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_encoding == CORBA::B_TRUE)
                    {
                      continue_encoding = stream->put_char (*(CORBA::Char *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_encoding == CORBA::B_TRUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                case CORBA::tk_longdouble:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_encoding == CORBA::B_TRUE)
                    {
                      continue_encoding = stream->put_longdouble (*(CORBA::LongDouble *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_encoding == CORBA::B_TRUE)
                    {
                      return CORBA::TypeCode::TRAVERSE_CONTINUE;
                    }
                  break;
                case CORBA::tk_wchar:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_encoding == CORBA::B_TRUE)
                    {
                      continue_encoding = stream->put_wchar (*(CORBA::WChar *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_encoding == CORBA::B_TRUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                case CORBA::tk_enum:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_encoding == CORBA::B_TRUE)
                    {
                      continue_encoding = stream->put_long (*(CORBA::Long *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_encoding == CORBA::B_TRUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                  // handle all aggregate types here
                case CORBA::tk_any:
                case CORBA::tk_TypeCode:
                case CORBA::tk_Principal:
                case CORBA::tk_objref:
                case CORBA::tk_struct:
                case CORBA::tk_union:
                case CORBA::tk_string:
                case CORBA::tk_sequence:
                case CORBA::tk_array:
                case CORBA::tk_alias:
                case CORBA::tk_except:
                case CORBA::tk_wstring:
                  // For those aggregate types whose size is constant, we
                  // compute it only once
                  while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                    {
                      retval = stream->encode (tc2, value, 0, env);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                default:
                  break;
                } // end of switch
            } // no exception computing size
        } // no exception computing content type
    } // no exception computing bounds
  // error exit
  env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
  dmsg ("marshaling TAO_Marshal_Sequence::encode detected error");
  return CORBA::TypeCode::TRAVERSE_STOP;
}

CORBA::TypeCode::traverse_status
TAO_Marshal_Alias::encode (CORBA::TypeCode_ptr tc,
			   const void *data,
			   const void *,
			   void *context,
			   CORBA::Environment &env)
{
  CORBA::TypeCode_ptr tc2;  // typecode of the aliased type
  CORBA::Boolean continue_encoding = CORBA::B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA::TypeCode::traverse_status   retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE; // status of encode operation 
  char *value = (char *) data;

  tc2 = tc->content_type (env);
  if (env.exception () == 0)
    {
    // switch on the data type and handle the cases for primitives here for
    // efficiency rather than calling 
    switch (tc2->_kind)
      {
      case CORBA::tk_null:
      case CORBA::tk_void:
        break;
      case CORBA::tk_short:
      case CORBA::tk_ushort:
        continue_encoding = stream->put_short (*(CORBA::Short *) value);
        break;
      case CORBA::tk_long:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_enum:
        continue_encoding = stream->put_long (*(CORBA::Long *) value);
        break;
      case CORBA::tk_double:
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
        continue_encoding = stream->put_longlong (*(CORBA::LongLong *) value);
        break;
      case CORBA::tk_boolean:
        continue_encoding = stream->put_boolean (*(CORBA::Boolean *) value);
        break;
      case CORBA::tk_char:
      case CORBA::tk_octet:
        continue_encoding = stream->put_char (*(CORBA::Char *) value);
        break;
      case CORBA::tk_longdouble:
        continue_encoding = stream->put_longdouble (*(CORBA::LongDouble *) value);
        break;
      case CORBA::tk_wchar:
        continue_encoding = stream->put_wchar (*(wchar_t *) value);
        break;
      case CORBA::tk_any:
      case CORBA::tk_TypeCode:
      case CORBA::tk_Principal:
      case CORBA::tk_objref:
      case CORBA::tk_struct:
      case CORBA::tk_union:
      case CORBA::tk_string:
      case CORBA::tk_sequence:
      case CORBA::tk_array:
      case CORBA::tk_alias:
      case CORBA::tk_except:
      case CORBA::tk_wstring:
        retval = stream->encode (tc2, value, 0, env);
        break;
      default:
        // anything else is an error
        retval = CORBA::TypeCode::TRAVERSE_STOP;
      }
  }
  //  tc2->Release ();
  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_encoding == CORBA::B_TRUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Alias::encode detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}


// encode exception
CORBA::TypeCode::traverse_status
TAO_Marshal_Except::encode (CORBA::TypeCode_ptr tc,
			    const void *data,
			    const void *,
			    void *context,
			    CORBA::Environment &env)
{
  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::Boolean continue_encoding = CORBA::B_TRUE;

  if (env.exception () == 0)
    {
      CDR *stream = (CDR *) context;
      CORBA::TypeCode_ptr param;
      CORBA::Long size, alignment;

      data = (char *) data + sizeof (CORBA::Exception);

      int member_count = tc->member_count (env);

      for (int i = 0; 
	   i < member_count && retval == CORBA::TypeCode::TRAVERSE_CONTINUE 
	     && continue_encoding == CORBA::B_TRUE; 
	   i++)
	{
	  param = tc->member_type (i, env);
	  if (env.exception () == 0)
	    {
	      size = param->size (env);
	      if (env.exception () == 0)
		{
		  alignment = param->alignment (env);
		  if (env.exception () == 0)
		    {
		      data = ptr_align_binary (data, alignment);
		      switch (param->_kind){
		      case CORBA::tk_null:
		      case CORBA::tk_void:
			break;
		      case CORBA::tk_short:
		      case CORBA::tk_ushort:
			continue_encoding = stream->put_short (*(CORBA::Short *) data);
			break;
		      case CORBA::tk_long:
		      case CORBA::tk_ulong:
		      case CORBA::tk_float:
		      case CORBA::tk_enum:
			continue_encoding = stream->put_long (*(CORBA::Long *) data);
			break;
		      case CORBA::tk_double:
		      case CORBA::tk_longlong:
		      case CORBA::tk_ulonglong:
			continue_encoding = stream->put_longlong (*(CORBA::LongLong *) data);
			break;
		      case CORBA::tk_boolean:
			continue_encoding = stream->put_boolean (*(CORBA::Boolean *) data);
			break;
		      case CORBA::tk_char:
		      case CORBA::tk_octet:
			continue_encoding = stream->put_char (*(CORBA::Char *) data);
			break;
		      case CORBA::tk_longdouble:
			continue_encoding = stream->put_longdouble (*(CORBA::LongDouble *) data);
			break;
		      case CORBA::tk_wchar:
			continue_encoding = stream->put_wchar (*(wchar_t *) data);
			break;
		      case CORBA::tk_any:
		      case CORBA::tk_TypeCode:
		      case CORBA::tk_Principal:
		      case CORBA::tk_objref:
		      case CORBA::tk_struct:
		      case CORBA::tk_union:
		      case CORBA::tk_string:
		      case CORBA::tk_sequence:
		      case CORBA::tk_array:
		      case CORBA::tk_alias:
		      case CORBA::tk_except:
		      case CORBA::tk_wstring:
			retval = stream->encode (param, data, 0, env);
			break;
		      default:
			break;
		      }
		      data = (char *) data + size;
		    } 
		  else 
		    return CORBA::TypeCode::TRAVERSE_STOP;
		} 
	      else 
		return CORBA::TypeCode::TRAVERSE_STOP;
	    } 
	  else 
	    return CORBA::TypeCode::TRAVERSE_STOP;
	}
    } 
  else 
    return CORBA::TypeCode::TRAVERSE_STOP;

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE 
      && continue_encoding == CORBA::B_TRUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Except detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}


// encode wstring
CORBA::TypeCode::traverse_status
TAO_Marshal_WString::encode (CORBA::TypeCode_ptr tc,
			     const void *data,
			     const void *,
			     void *context,
			     CORBA::Environment &env)
{
  CORBA::Boolean continue_encoding = CORBA::B_TRUE;

  CORBA::WChar *str = *(CORBA::WChar **) data;
  CDR *stream = (CDR *) context;  // context is the CDR stream

  // Be nice to programmers: treat nulls as empty strings not
  // errors. (OMG-IDL supports languages that don't use the
  // C/C++ notion of null v. empty strings; nulls aren't part of
  // the OMG-IDL string model.)
  if (str != 0) 
    {
      // Verify string satisfies bounds requirements.  We're not so
      // permissive as to send messages violating the interface spec
      // by having excessively long strings!
      CORBA::ULong bounds = tc->length (env);

      if (env.exception () == 0)
        {
          // get the actual length of the string
	  CORBA::ULong len = wslen ((CORBA::WChar *) str);

          // if it is an unbounded string or if the length is less than the
          // bounds for an unbounded string
          if ((bounds == 0) || (len <= bounds))
            {

              // Encode the string, followed by a NUL character.

              for (continue_encoding = stream->put_ulong (len + 1);
                   continue_encoding != CORBA::B_FALSE && *str;
                   continue_encoding = stream->put_wchar (*str++))
                continue;

              stream->put_wchar (0);
              return CORBA::TypeCode::TRAVERSE_CONTINUE;
            }
          else
	    return CORBA::TypeCode::TRAVERSE_STOP;
        }
      else
	return CORBA::TypeCode::TRAVERSE_STOP;
    }
  else 
    {
      // empty string
      stream->put_ulong (1);
      stream->put_wchar (0);
      return CORBA::TypeCode::TRAVERSE_CONTINUE;
    }
}

