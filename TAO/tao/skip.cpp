// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    skip.cpp
//
// = DESCRIPTION
//   Code for skipping different data types
//
//   Data types encoded as CDR streams need to be skipped when they are part of
//   an Any.
//
// = AUTHOR
//     Aniruddha Gokhale
//
// ============================================================================

#include "tao/Marshal.h"
#include "tao/CDR.h"
#include "tao/GIOP.h"
#include "tao/Any.h"
#include "tao/Environment.h"
#include "tao/debug.h"

ACE_RCSID(tao, skip, "$Id$")

CORBA::TypeCode::traverse_status
TAO_Marshal_Primitive::skip (CORBA::TypeCode_ptr  tc,
                             void *context,
                             CORBA::Environment &env)
{
  CORBA::Boolean continue_skipping = 1;
  TAO_InputCDR *stream = ACE_static_cast (TAO_InputCDR *, context);

  // status of skip operation
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;

  switch (tc->kind_)
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
      break;
    case CORBA::tk_short:
    case CORBA::tk_ushort:
      continue_skipping = stream->skip_short ();
      break;
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_enum:
      continue_skipping = stream->skip_long ();
      break;
    case CORBA::tk_double:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
      continue_skipping = stream->skip_longlong ();
      break;
    case CORBA::tk_boolean:
      continue_skipping = stream->skip_boolean ();
      break;
    case CORBA::tk_char:
    case CORBA::tk_octet:
      continue_skipping = stream->skip_char ();
      break;
    case CORBA::tk_longdouble:
      continue_skipping = stream->skip_longdouble ();
      break;
    case CORBA::tk_wchar:
      continue_skipping = stream->skip_wchar ();
      break;
    default:
      retval = CORBA::TypeCode::TRAVERSE_STOP;
      // we are not a primitive type
    }
  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_skipping == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Primitive::skip detected error\n"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

CORBA::TypeCode::traverse_status
TAO_Marshal_Any::skip (CORBA::TypeCode_ptr,
                         void *context,
                         CORBA::Environment &env)
{
  // Typecode of the element that makes the Any.
  CORBA::TypeCode_ptr elem_tc;

  // Context is the CDR stream.
  TAO_InputCDR *stream = ACE_static_cast (TAO_InputCDR *, context);

  // Status of encode operation.
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;

  // First retrieve the TypeCode for the element so that we can skip
  // the value based on this typecode.
  if (stream->decode (CORBA::_tc_TypeCode,
                      &elem_tc,
                      0,
                      env) == CORBA::TypeCode::TRAVERSE_CONTINUE)
    {
      if (env.exception () == 0)
        retval = stream->skip (elem_tc, env);
      else
        retval = CORBA::TypeCode::TRAVERSE_STOP;
    }
  CORBA::release (elem_tc);
  return retval;
}

CORBA::TypeCode::traverse_status
TAO_Marshal_TypeCode::skip (CORBA::TypeCode_ptr,
                            void *context,
                            CORBA::Environment  &env)
{
  CORBA::Boolean continue_skipping = 1;

  // Context is the CDR stream.
  TAO_InputCDR *stream = ACE_static_cast (TAO_InputCDR *, context);

#if 0
  // Typecode to be decoded.
  CORBA::TypeCode_ptr *tcp;
#endif /* 0 */

  // Typecode kind.
  CORBA::ULong kind;

  // Decode the "kind" field of the typecode from the stream.
  continue_skipping = stream->read_ulong (kind);

  if (continue_skipping == 1)
    {
      // Typecodes with empty parameter lists all have preallocated
      // constants.  We use those to reduce memory consumption and
      // heap access ... also, to speed things up!
      if ((kind < CORBA::TC_KIND_COUNT) ||
          (kind == ~0u))
        {
          // Either a non-constant typecode or an indirected typecode.
          switch (kind)
            {
              // Need special handling for all kinds of typecodes that
              // have nonempty parameter lists ...
            default:
              // simple typecodes, nothing to do
              break;
            case CORBA::tk_string:
            case CORBA::tk_wstring:
              {
                // skip the bounds
                continue_skipping = stream->skip_ulong ();
              }
            break;

            // Indirected typecodes, illegal at "top level".
            case ~0u:
              {
                // skip the long indicating the encapsulation offset,
                continue_skipping = stream->skip_long ();
              }
            break;

            // The rest have "complex" parameter lists that are
            // encoded as bulk octets ...
            case CORBA::tk_objref:
            case CORBA::tk_struct:
            case CORBA::tk_union:
            case CORBA::tk_enum:
            case CORBA::tk_sequence:
            case CORBA::tk_array:
            case CORBA::tk_alias:
            case CORBA::tk_except:
              {
                CORBA::ULong length;

                // get the encapsulation length
                continue_skipping = stream->read_ulong (length);
                if (!continue_skipping)
                  break;
                // skip the encapsulation
                continue_skipping = stream->skip_bytes (length);
              }
            } // end of switch
        }
      else // bad kind_ value to be decoded
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO_Marshal_TypeCode::skip: "
                        "Bad kind_ value in CDR stream"));
          env.exception (new CORBA::BAD_TYPECODE (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));
          return CORBA::TypeCode::TRAVERSE_STOP;
        }
    }

  if (continue_skipping == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_TypeCode::skip detected error\n"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// Encode Principal.

CORBA::TypeCode::traverse_status
TAO_Marshal_Principal::skip (CORBA::TypeCode_ptr,
                               void *context,
                               CORBA::Environment &env)
{
  CORBA::Boolean continue_skipping = 1;

  // Context is the CDR stream.
  TAO_InputCDR *stream = ACE_static_cast (TAO_InputCDR *, context);

  // specifies the number of bytes in the Principal
  CORBA::ULong len;

  continue_skipping = stream->read_ulong (len);
  if (len > 0 && continue_skipping)
    {
      continue_skipping = stream->skip_bytes (len);
    }

  if (continue_skipping == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Principal::skip detected error\n"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

CORBA::TypeCode::traverse_status
TAO_Marshal_ObjRef::skip (CORBA::TypeCode_ptr,
                          void *context,
                          CORBA::Environment &env)
{
  CORBA::Boolean continue_skipping = 1;

  // Context is the CDR stream.
  TAO_InputCDR *stream = ACE_static_cast (TAO_InputCDR *, context);
  // return status
  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;

  // First, skip the type hint. This will be the type_id encoded in an
  // object reference.
  stream->skip_string ();

  // Read the profiles, discarding all until an IIOP profile comes by.
  // Once we see an IIOP profile, ignore any further ones.
  //
  // XXX this will need to change someday to let different protocol
  // code be accessed, not just IIOP.  Protocol modules will be
  // dynamically loaded from shared libraries via ORB_init (), and we
  // just need to be able to access such preloaded libraries here as
  // we unmarshal objrefs.
  CORBA::ULong profiles = 0;

  // get the count of profiles that follow
  continue_skipping = stream->read_ulong (profiles);

  while (profiles-- != 0 && continue_skipping)
      {
        CORBA::ULong tag;

        // get the profile ID tag
        if ( (continue_skipping = stream->read_ulong (tag)) == 0)
          continue;

        if (tag != TAO_IOP_TAG_INTERNET_IOP)
          {
            continue_skipping = stream->skip_string ();
            continue;
          }

        // OK, we've got an IIOP profile.  It's going to be
        // encapsulated ProfileData.  Create a new decoding stream and
        // context for it, and tell the "parent" stream that this data
        // isn't part of it any more.

        CORBA::ULong encap_len;
        // ProfileData is encoded as a sequence of octet. So first get
        // the length of the sequence.
        // Create the decoding stream from the encapsulation in the
        // buffer, and skip the encapsulation.
        if ( (continue_skipping = stream->read_ulong (encap_len)) == 0)
          continue;

        TAO_InputCDR str (*stream, encap_len);

        continue_skipping =
          str.good_bit ()
          && stream->skip_bytes (encap_len);

        if (!continue_skipping)
          continue;

        // Read and verify major, minor versions, ignoring IIOP
        // profiles whose versions we don't understand.
        //
        // XXX this doesn't actually go back and skip the whole
        // encapsulation...
        if (!(str.skip_octet ()
              && str.skip_octet ()))
          continue;

        // skip host and port
        if (!str.skip_string ()
            || !str.skip_ushort ())
          {
            if (TAO_debug_level > 0)
              ACE_DEBUG ((LM_DEBUG,
                          "error decoding IIOP host/port"));
            env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
            return CORBA::TypeCode::TRAVERSE_STOP;
          }

        // ... and object key.
        if (str.skip (TC_opaque,
                      env) != CORBA::TypeCode::TRAVERSE_CONTINUE)
          {
            env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
            return CORBA::TypeCode::TRAVERSE_STOP;
          }
      }

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_skipping == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_ObjRef::skip detected error\n"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// Decode structs.
CORBA::TypeCode::traverse_status
TAO_Marshal_Struct::skip (CORBA::TypeCode_ptr  tc,
                            void *context,
                            CORBA::Environment &env)
{
  TAO_InputCDR *stream = ACE_static_cast (TAO_InputCDR *, context);
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::TypeCode_ptr param;

  // Number of fields in the struct.
  int member_count = tc->member_count (env);

  if (env.exception () == 0)
    for (int i = 0; i < member_count
           && retval == CORBA::TypeCode::TRAVERSE_CONTINUE;
         i++)
      {
        param = tc->member_type (i, env);
        if (env.exception () == 0)
          {
            retval = stream->skip (param, env);
          }
        else
          return CORBA::TypeCode::TRAVERSE_STOP;
      }
  else
    return CORBA::TypeCode::TRAVERSE_STOP;

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Struct::skip detected error\n"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// Encode unions.
CORBA::TypeCode::traverse_status
TAO_Marshal_Union::skip (CORBA::TypeCode_ptr  tc,
                         void *context,
                         CORBA::Environment &env)
{
  // Context is the CDR stream.
  TAO_InputCDR *stream = ACE_static_cast (TAO_InputCDR *, context);

  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;

  CORBA::TypeCode_ptr discrim_tc;
  CORBA::TypeCode_ptr member_tc;
  CORBA::Any_ptr member_label;
  CORBA::Long discrim_val;
  CORBA::ULong member_count;
  CORBA::Long  default_index;
  CORBA::ULong i;
  CORBA::TypeCode_ptr default_tc = 0;
  CORBA::Boolean discrim_matched = 0;

  // get the discriminator type which will enable us to skip the discriminator
  // value
  discrim_tc = tc->discriminator_type (env);

  if (env.exception () == 0)
    {
      // decode the discriminator value
      retval = stream->decode (discrim_tc, &discrim_val, 0, env);
      if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          // now get ready to skip the actual union value
          default_index = tc->default_index (env);

          if (env.exception () == 0)
            {
              member_count = tc->member_count (env);
              if (env.exception () == 0)
                {
                  // check which label value matches with the discriminator
                  // value. Accordingly, marshal the corresponding
                  // member_type. If none match, check if default exists
                  // and marshal accordingly. Otherwise it is an error.

                  for (i = 0; i < member_count; i++)
                    {
                      member_label = tc->member_label (i, env);
                      if (env.exception () == 0)
                        {
                          // do the matching
			                    CORBA::TypeCode_var type = member_label->type ();
                          switch (type->kind (env))
                            {
                              case CORBA::tk_short:
                                {
                                  CORBA::Short s;
                                  *member_label >>= s;
                                  if (s == *(CORBA::Short *) &discrim_val)
                                    discrim_matched = 1;
                                }
                                break;
                              case CORBA::tk_ushort:
                                {
                                  CORBA::UShort s;
                                  *member_label >>= s;
                                  if (s == *(CORBA::UShort *) &discrim_val)
                                    discrim_matched = 1;
                                }
                                break;
                              case CORBA::tk_long:
                                {
                                  CORBA::Long l;
                                  *member_label >>= l;
                                  if (l == *(CORBA::Long *) &discrim_val)
                                    discrim_matched = 1;
                                }
                                break;
                              case CORBA::tk_ulong:
                                {
                                  CORBA::ULong l;
                                  *member_label >>= l;
                                  if (l == *(CORBA::ULong *) &discrim_val)
                                    discrim_matched = 1;
                                }
                                break;
                              case CORBA::tk_enum:
                                {
                                  CORBA::ULong ul;
                                  TAO_InputCDR stream (member_label->_tao_get_cdr ());
                                  (void)stream.decode (discrim_tc, &ul, 0, env);
                                  if (ul == *(CORBA::ULong *) &discrim_val)
                                    discrim_matched = 1;
                                }
                                break;
                              case CORBA::tk_char:
                                {
                                  CORBA::Char c;
                                  *member_label >>= CORBA::Any::to_char (c);
                                  if (c == *(CORBA::Char *) &discrim_val)
                                    discrim_matched = 1;
                                }
                                break;
                              case CORBA::tk_wchar:
                                CORBA::WChar wc;
                                *member_label >>= CORBA::Any::to_wchar (wc);
                                if (wc == *(CORBA::WChar *) &discrim_val)
                                  discrim_matched = 1;
                                break;
                              case CORBA::tk_boolean:
                                {
                                  CORBA::Boolean b;
                                  *member_label >>= CORBA::Any::to_boolean (b);
                                  if (b == *(CORBA::Boolean *) &discrim_val)
                                    discrim_matched = 1;
                                }
                                break;
                              default:
                                env.exception (new CORBA::BAD_TYPECODE (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));
                                return CORBA::TypeCode::TRAVERSE_STOP;
                            }// end of switch

                          // get the member typecode
                          member_tc = tc->member_type (i, env);
                          if (env.exception () == 0)
                            {
                              if (default_index >= 0 && default_index-- == 0)
                                // have we reached the default label?, if so,
                                // save a handle to the typecode for the default
                                default_tc = member_tc;
                              if (discrim_matched)
                                {
                                  // marshal according to the matched typecode
                                  return stream->skip (member_tc, env);
                                }
                            }
                          else
                            {
                              env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
                              return CORBA::TypeCode::TRAVERSE_STOP;
                            }
                        }
                      else
                        {
                          env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
                          return CORBA::TypeCode::TRAVERSE_STOP;
                        }
                    } // end of for loop
                  // we are here only if there was no match
                  if (default_tc)
                    return stream->skip (default_tc, env);
                  else
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                }
              else
                {
                  env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
                  return CORBA::TypeCode::TRAVERSE_STOP;
                }
            }
          else
            {
              env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
              return CORBA::TypeCode::TRAVERSE_STOP;
            }
        }
      else
        {
          env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
          return CORBA::TypeCode::TRAVERSE_STOP;
        }
    }
  else
    {
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// decode string
CORBA::TypeCode::traverse_status
TAO_Marshal_String::skip (CORBA::TypeCode_ptr,
                          void *context,
                          CORBA::Environment &env)
{
  CORBA::Boolean continue_skipping = 1;
  // Context is the CDR stream.
  TAO_InputCDR *stream = ACE_static_cast (TAO_InputCDR *, context);


  // On decode, omit the check against specified string bounds, and
  // cope with illegal "zero length" strings (all lengths on the wire
  // must include a NUL).
  //
  // This is on the principle of being gracious in what we accept; we
  // don't generate messages that fail to comply with protocol specs,
  // but we will accept them when it's clear how to do so.

  continue_skipping = stream->skip_string ();
  if (continue_skipping == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_TypeCode::skip detected error"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// Decode sequence.

CORBA::TypeCode::traverse_status
TAO_Marshal_Sequence::skip (CORBA::TypeCode_ptr  tc,
                            void *context,
                            CORBA::Environment &env)
{
  CORBA::Boolean continue_skipping = 1;
  TAO_InputCDR *stream = ACE_static_cast (TAO_InputCDR *, context);
  // Typecode of the element.
  CORBA::TypeCode_ptr tc2;
  // Size of element.
  CORBA::ULong bounds;

  // First unmarshal the sequence length ... we trust it to be right
  // here, on the "be gracious in what you accept" principle.  We
  // don't generate illegal sequences (i.e. length > bounds).

  continue_skipping = stream->read_ulong (bounds);

  if (continue_skipping)
    {
      // No point decoding an empty sequence.
      if (bounds > 0)
        {
          // Get element typecode.
          tc2 = tc->content_type (env);

          if (env.exception () == 0)
            {
              while (bounds-- && continue_skipping == 1)
                {
                  continue_skipping = stream->skip (tc2, env);
                }
              if (continue_skipping)
                return CORBA::TypeCode::TRAVERSE_CONTINUE;
            } // no exception computing content type
        } // length is > 0
      else
        return CORBA::TypeCode::TRAVERSE_CONTINUE;
    }
  // error exit
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_Marshal_Sequence::skip detected error\n"));
  env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));
  return CORBA::TypeCode::TRAVERSE_STOP;
}

// Decode array.

CORBA::TypeCode::traverse_status
TAO_Marshal_Array::skip (CORBA::TypeCode_ptr  tc,
                           void *context,
                           CORBA::Environment &env)
{
  CORBA::Boolean continue_skipping = 1;
  TAO_InputCDR *stream = ACE_static_cast (TAO_InputCDR *, context);

  // Typecode of the element.
  CORBA::TypeCode_ptr tc2;

  CORBA::ULong  bounds;

  // retrieve the bounds of the array
  bounds = tc->length (env);
  if (env.exception () == 0)
    {
      // get element typecode
      tc2 = tc->content_type (env);
      if (env.exception () == 0)
        {
          while (bounds-- && continue_skipping == 1)
            {
              continue_skipping = stream->skip (tc2, env);
            }
          if (continue_skipping)
            return CORBA::TypeCode::TRAVERSE_CONTINUE;
        } // no exception computing content type
    } // no exception computing bounds
  // error exit
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_Marshal_Sequence::skip detected error\n"));
  env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));
  return CORBA::TypeCode::TRAVERSE_STOP;
}

// Decode alias.
CORBA::TypeCode::traverse_status
TAO_Marshal_Alias::skip (CORBA::TypeCode_ptr  tc,
                         void *context,
                         CORBA::Environment &env)
{
  // Typecode of the aliased type.
  CORBA::TypeCode_ptr tc2;
  CORBA::Boolean continue_skipping = 1;

  // Context is the CDR stream.
  TAO_InputCDR *stream = ACE_static_cast (TAO_InputCDR *, context);

  // Status of decode operation.
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;

  tc2 = tc->content_type (env);
  if (env.exception () == 0)
    {
      retval = stream->skip (tc2, env);
    }
  //  tc2->_decr_refcnt ();
  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_skipping == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Alias::skip detected error\n"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// Decode exception For exceptions, the "hidden" type ID near the
// front of the on-wire representation was previously unmarshaled and
// mapped to the "tc" typcode we're using to traverse the memory ...
// at the same time its vtable, refcount, and other state was
// established.
//
// NOTE: This is asymmetric with respect to encoding exceptions.
CORBA::TypeCode::traverse_status
TAO_Marshal_Except::skip (CORBA::TypeCode_ptr  tc,
                          void *context,
                          CORBA::Environment &env)
{
  TAO_InputCDR *stream = ACE_static_cast (TAO_InputCDR *, context);
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::TypeCode_ptr param;

  // skip the Repository ID
  if (!stream->skip_string ())
    return CORBA::TypeCode::TRAVERSE_STOP;

  // Number of fields in the exception
  int member_count = tc->member_count (env);
  if (env.exception () == 0)
    {
      for (int i = 0; i < member_count
             && retval == CORBA::TypeCode::TRAVERSE_CONTINUE;
           i++)
        {
          param = tc->member_type (i, env);
          if (env.exception () == 0)
            {
              retval = stream->skip (param, env);
            }
          else
            return CORBA::TypeCode::TRAVERSE_STOP;
        }
    }
  else
      return CORBA::TypeCode::TRAVERSE_STOP;

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Except::skip detected error\n"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// decode wstring
CORBA::TypeCode::traverse_status
TAO_Marshal_WString::skip (CORBA::TypeCode_ptr,
                           void *context,
                           CORBA::Environment &env)
{
  CORBA::Boolean continue_skipping = 1;
  TAO_InputCDR *stream = ACE_static_cast (TAO_InputCDR *, context);
  CORBA::ULong len;

  // On decode, omit the check against specified wstring bounds, and
  // cope with illegal "zero length" strings (all lengths on the wire
  // must include a NUL).
  //
  // This is on the principle of being gracious in what we accept; we
  // don't generate messages that fail to comply with protocol specs,
  // but we will accept them when it's clear how to do so.

  continue_skipping = stream->read_ulong (len);

  if (len != 0)
    while (continue_skipping != 0 && len--)
      {
        continue_skipping = stream->skip_wchar ();
      }

  if (continue_skipping == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_WString::skip detected error\n"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}
