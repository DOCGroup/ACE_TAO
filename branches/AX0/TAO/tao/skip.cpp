// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     skip.cpp
//
// = DESCRIPTION
//     Code for skipping different data types
//
//     Data types encoded as CDR streams need to be skipped when they
//     are part of an Any.
//
// = AUTHOR
//     Aniruddha Gokhale
//
// ============================================================================

#include "tao/Marshal.h"
#include "tao/CDR.h"
#include "tao/Any.h"
#include "tao/Environment.h"
#include "tao/ValueBase.h"
#include "tao/debug.h"

ACE_RCSID(tao, skip, "$Id$")

CORBA::TypeCode::traverse_status
TAO_Marshal_Primitive::skip (CORBA::TypeCode_ptr  tc,
                             TAO_InputCDR *stream
                             ACE_ENV_ARG_DECL)
{
  CORBA::Boolean continue_skipping = 1;

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
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("TAO_Marshal_Primitive::skip detected error\n")
          ));
      ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                        CORBA::COMPLETED_MAYBE),
                        CORBA::TypeCode::TRAVERSE_STOP);
    }
}

CORBA::TypeCode::traverse_status
TAO_Marshal_Any::skip (CORBA::TypeCode_ptr,
                       TAO_InputCDR *stream
                       ACE_ENV_ARG_DECL)
{
  // Typecode of the element that makes the Any.
  CORBA::TypeCode_var elem_tc;

  // Status of encode operation.
  if (!(*stream >> elem_tc.inout ()))
    return CORBA::TypeCode::TRAVERSE_STOP;

  return TAO_Marshal_Object::perform_skip (elem_tc.in (),
                                           stream
                                            ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode::traverse_status
TAO_Marshal_TypeCode::skip (CORBA::TypeCode_ptr,
                            TAO_InputCDR *stream
                            ACE_ENV_ARG_DECL)
{
  CORBA::Boolean continue_skipping = 1;

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
            case CORBA::tk_value:
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
                        ACE_TEXT ("TAO_Marshal_TypeCode::skip: ")
                        ACE_TEXT ("Bad kind_ value in CDR stream")));
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                            CORBA::TypeCode::TRAVERSE_STOP);
        }
    }

  if (continue_skipping == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("TAO_Marshal_TypeCode::skip detected error\n")
          ));
      ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                        CORBA::COMPLETED_MAYBE),
                        CORBA::TypeCode::TRAVERSE_STOP);
    }
}

CORBA::TypeCode::traverse_status
TAO_Marshal_Principal::skip (CORBA::TypeCode_ptr,
                             TAO_InputCDR *stream
                             ACE_ENV_ARG_DECL)
{
  CORBA::Boolean continue_skipping = 1;

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
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("TAO_Marshal_Principal::skip detected error\n")
          ));
      ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                        CORBA::COMPLETED_MAYBE),
                        CORBA::TypeCode::TRAVERSE_STOP);
    }
}

CORBA::TypeCode::traverse_status
TAO_Marshal_ObjRef::skip (CORBA::TypeCode_ptr,
                          TAO_InputCDR *stream
                          ACE_ENV_ARG_DECL)
{
  CORBA::Boolean continue_skipping = 1;

  // return status
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;

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

        CORBA::ULong encap_len;
        // ProfileData is encoded as a sequence of octet. So first get
        // the length of the sequence.
        // Create the decoding stream from the encapsulation in the
        // buffer, and skip the encapsulation.
        if ( (continue_skipping = stream->read_ulong (encap_len)) == 0)
          continue;

        continue_skipping = stream->skip_bytes (encap_len);
      }

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_skipping == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("TAO_Marshal_ObjRef::skip detected error\n")
          ));
      ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                        CORBA::COMPLETED_MAYBE),
                        CORBA::TypeCode::TRAVERSE_STOP);
    }
}

CORBA::TypeCode::traverse_status
TAO_Marshal_Struct::skip (CORBA::TypeCode_ptr  tc,
                          TAO_InputCDR *stream
                          ACE_ENV_ARG_DECL)
{
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::TypeCode_var param;

  // Number of fields in the struct.
  int member_count = tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  for (int i = 0; i < member_count
         && retval == CORBA::TypeCode::TRAVERSE_CONTINUE;
       i++)
    {
      param = tc->member_type (i ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

      retval = TAO_Marshal_Object::perform_skip (param.in (),
                                                 stream
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
    }

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Struct::skip detected error\n")));
  ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                    CORBA::COMPLETED_MAYBE),
                    CORBA::TypeCode::TRAVERSE_STOP);
}

CORBA::TypeCode::traverse_status
TAO_Marshal_Union::skip (CORBA::TypeCode_ptr  tc,
                         TAO_InputCDR *src
                         ACE_ENV_ARG_DECL)
{
  CORBA::TypeCode_var discrim_tc =
    tc->discriminator_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  CORBA::ULong kind =
    discrim_tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  // Save the discriminator value in a temporary variable...
  CORBA::Short short_v;
  CORBA::UShort ushort_v;
  CORBA::Long long_v;
  CORBA::ULong ulong_v;
  CORBA::ULong enum_v;
  CORBA::Char char_v;
  CORBA::WChar wchar_v;
  CORBA::Boolean boolean_v;

  switch (kind)
    {
    case CORBA::tk_short:
      {
        if (!src->read_short (short_v))
          return CORBA::TypeCode::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_ushort:
      {
        if (!src->read_ushort (ushort_v))
          return CORBA::TypeCode::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_long:
      {
        if (!src->read_long (long_v))
          return CORBA::TypeCode::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_ulong:
      {
        if (!src->read_ulong (ulong_v))
          return CORBA::TypeCode::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_enum:
      {
        if (!src->read_ulong (enum_v))
          return CORBA::TypeCode::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_char:
      {
        if (!src->read_char (char_v))
          return CORBA::TypeCode::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_wchar:
      {
        if (!src->read_wchar (wchar_v))
          return CORBA::TypeCode::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_boolean:
      {
        if (!src->read_boolean (boolean_v))
          return CORBA::TypeCode::TRAVERSE_STOP;
      }
      break;

    default:
      return CORBA::TypeCode::TRAVERSE_STOP;
    }

  CORBA::ULong member_count =
    tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  const CORBA::ULong null_member = ~ACE_static_cast (CORBA::ULong, 0U);

  CORBA::ULong current_member = null_member;
  CORBA::ULong default_member = null_member;

  for (CORBA::ULong i = 0;
       i != member_count && current_member == null_member;
       ++i)
    {
      CORBA::Any_var any = tc->member_label (i ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

      CORBA::Octet o;
      if ((any >>= CORBA::Any::to_octet (o)) && o == 0)
        {
          CORBA::ULong default_index =
            tc->default_index (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

          if (i != default_index)
            ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                              CORBA::TypeCode::TRAVERSE_STOP);
          // Found the default branch, save its position and continue
          // trying to find the current value...
          default_member = i;
          continue;
        }

      switch (kind)
        {
        case CORBA::tk_short:
          {
            CORBA::Short d;
            if ((any >>= d) && d == short_v)
              current_member = i;
          }
          break;

        case CORBA::tk_ushort:
          {
            CORBA::UShort d;
            if ((any >>= d) && d == ushort_v)
              current_member = i;
          }
          break;

        case CORBA::tk_long:
          {
            CORBA::Long d;
            if ((any >>= d) && d == long_v)
              current_member = i;
          }
          break;

        case CORBA::tk_ulong:
          {
            CORBA::ULong d;
            if ((any >>= d) && d == ulong_v)
              current_member = i;
          }
          break;

        case CORBA::tk_enum:
          {
            CORBA::ULong d;

            // Create an special Any to handle this case.
            CORBA::Any tmp;
            tmp._tao_replace (CORBA::_tc_ulong,
                              any->_tao_byte_order (),
                              any->_tao_get_cdr ());
            if ((tmp >>= d) && d == enum_v)
              current_member = i;
          }
          break;

        case CORBA::tk_char:
          {
            CORBA::Char d;
            if ((any >>= CORBA::Any::to_char (d)) && d == char_v)
              current_member = i;
          }
          break;

        case CORBA::tk_wchar:
          {
            CORBA::WChar d;
            if ((any >>= CORBA::Any::to_wchar (d)) && d == wchar_v)
              current_member = i;
          }
          break;

        case CORBA::tk_boolean:
          {
            CORBA::Boolean d;
            if ((any >>= CORBA::Any::to_boolean (d)) && d == boolean_v)
              current_member = i;
          }
          break;

        default:
          return CORBA::TypeCode::TRAVERSE_STOP;
        }
    }

  if (current_member == null_member)
    {
      // Cannot find the current member, check if there is a
      // default...
      if (default_member != null_member)
        {
          // Good, use the default to append...
          CORBA::TypeCode_var member_tc =
            tc->member_type (default_member ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
          return TAO_Marshal_Object::perform_skip (member_tc.in (),
                                                   src
                                                   ACE_ENV_ARG_PARAMETER);
        }

      // If we're here, we have an implicit default case, and we
      // should just return without demarshaling anything, since no
      // union member was marshaled in the first place.
      return CORBA::TypeCode::TRAVERSE_CONTINUE;
    }

  // If we found the member successfully then just use that one...
  CORBA::TypeCode_var member_tc =
    tc->member_type (current_member ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
  return TAO_Marshal_Object::perform_skip (member_tc.in (),
                                           src
                                           ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode::traverse_status
TAO_Marshal_String::skip (CORBA::TypeCode_ptr,
                          TAO_InputCDR *stream
                          ACE_ENV_ARG_DECL)
{
  CORBA::Boolean continue_skipping = 1;

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
                    ACE_TEXT ("TAO_Marshal_TypeCode::skip detected error")));
      ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                        CORBA::COMPLETED_MAYBE),
                        CORBA::TypeCode::TRAVERSE_STOP);
    }
}

CORBA::TypeCode::traverse_status
TAO_Marshal_Sequence::skip (CORBA::TypeCode_ptr  tc,
                            TAO_InputCDR *stream
                            ACE_ENV_ARG_DECL)
{
  // Size of element.
  CORBA::ULong bounds;

  // First unmarshal the sequence length ... we trust it to be right
  // here, on the "be gracious in what you accept" principle.  We
  // don't generate illegal sequences (i.e. length > bounds).

  CORBA::Boolean continue_skipping =
    stream->read_ulong (bounds);

  if (!continue_skipping)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO_Marshal_Sequence::skip detected error\n")));
      ACE_THROW_RETURN (CORBA::MARSHAL (),
                        CORBA::TypeCode::TRAVERSE_STOP);
    }

  // No point decoding an empty sequence.
  if (bounds == 0)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;

  // Get element typecode.
  CORBA::TypeCode_var tc2 =
    tc->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  // For CORBA basic types, the skip can be optimized
  CORBA::TCKind kind = tc2->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  char *dummy;
  switch (kind)
    {
    case CORBA::tk_octet:
    case CORBA::tk_boolean:
    case CORBA::tk_char:
      {
        stream->adjust (0, ACE_CDR::OCTET_ALIGN, dummy);
        continue_skipping =
          stream->skip_bytes (ACE_CDR::OCTET_SIZE * bounds);
      }
      break;
    case CORBA::tk_short:
    case CORBA::tk_ushort:
    case CORBA::tk_wchar:
      {
        stream->adjust (0, ACE_CDR::SHORT_ALIGN, dummy);
        continue_skipping =
          stream->skip_bytes (ACE_CDR::SHORT_SIZE * bounds);
      }
      break;
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
      {
        stream->adjust (0, ACE_CDR::LONG_ALIGN, dummy);
        continue_skipping =
          stream->skip_bytes (ACE_CDR::LONG_SIZE * bounds);
      }
      break;
    case CORBA::tk_double:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
      {
        stream->adjust (0, ACE_CDR::LONGLONG_ALIGN, dummy);
        continue_skipping =
          stream->skip_bytes (ACE_CDR::LONGLONG_SIZE * bounds);
      }
      break;
    case CORBA::tk_longdouble:
      {
        stream->adjust (0, ACE_CDR::LONGDOUBLE_ALIGN, dummy);
        continue_skipping =
          stream->skip_bytes (ACE_CDR::LONGDOUBLE_SIZE * bounds);
      }
      break;

    default:
      while (bounds-- && continue_skipping == 1)
        {
          continue_skipping =
            TAO_Marshal_Object::perform_skip (tc2.in (),
                                              stream
                                               ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
        }
      break;
    }// end of switch

  if (continue_skipping)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;

  // error exit
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Sequence::skip detected error\n")));
  ACE_THROW_RETURN (CORBA::MARSHAL (),
                    CORBA::TypeCode::TRAVERSE_STOP);
}

CORBA::TypeCode::traverse_status
TAO_Marshal_Array::skip (CORBA::TypeCode_ptr  tc,
                         TAO_InputCDR *stream
                         ACE_ENV_ARG_DECL)
{
  CORBA::Boolean continue_skipping = 1;


  // retrieve the bounds of the array
  CORBA::ULong bounds = tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  // get element typecode
  // Typecode of the element.
  CORBA::TypeCode_var tc2 = tc->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  // For CORBA basic types, the skip can be optimized
  CORBA::TCKind kind = tc2->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  char *dummy;
  switch (kind)
    {
    case CORBA::tk_octet:
    case CORBA::tk_boolean:
    case CORBA::tk_char:
      {
        stream->adjust (0, ACE_CDR::OCTET_ALIGN, dummy);
        continue_skipping =
          stream->skip_bytes (ACE_CDR::OCTET_SIZE * bounds);
      }
      break;
    case CORBA::tk_short:
    case CORBA::tk_ushort:
    case CORBA::tk_wchar:
      {
        stream->adjust (0, ACE_CDR::SHORT_ALIGN, dummy);
        continue_skipping =
          stream->skip_bytes (ACE_CDR::SHORT_SIZE * bounds);
      }
      break;
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
      {
        stream->adjust (0, ACE_CDR::LONG_ALIGN, dummy);
        continue_skipping =
          stream->skip_bytes (ACE_CDR::LONG_SIZE * bounds);
      }
      break;
    case CORBA::tk_double:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
      {
        stream->adjust (0, ACE_CDR::LONGLONG_ALIGN, dummy);
        continue_skipping =
          stream->skip_bytes (ACE_CDR::LONGLONG_SIZE * bounds);
      }
      break;
    case CORBA::tk_longdouble:
      {
        stream->adjust (0, ACE_CDR::LONGDOUBLE_ALIGN, dummy);
        continue_skipping =
          stream->skip_bytes (ACE_CDR::LONGDOUBLE_SIZE * bounds);
      }
      break;

    default:
      while (bounds-- && continue_skipping == 1)
        {
          int stop =
            TAO_Marshal_Object::perform_skip (tc2.in (),
                                              stream
                                               ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
          if (stop == CORBA::TypeCode::TRAVERSE_STOP)
            continue_skipping = 0;
        }
      break;
    }// end of switch

  if (continue_skipping)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;

  // error exit
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Sequence::skip detected error\n")));
  ACE_THROW_RETURN (CORBA::MARSHAL (),
                    CORBA::TypeCode::TRAVERSE_STOP);
}

CORBA::TypeCode::traverse_status
TAO_Marshal_Alias::skip (CORBA::TypeCode_ptr  tc,
                         TAO_InputCDR *stream
                         ACE_ENV_ARG_DECL)
{
  // Typecode of the aliased type.
  CORBA::TypeCode_var tc2;
  CORBA::Boolean continue_skipping = 1;

  // Status of decode operation.
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;

  tc2 = tc->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  retval = TAO_Marshal_Object::perform_skip (tc2.in (),
                                             stream
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  //  tc2->_decr_refcnt ();
  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_skipping == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Alias::skip detected error\n")));
  ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                    CORBA::COMPLETED_MAYBE),
                    CORBA::TypeCode::TRAVERSE_STOP);
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
                          TAO_InputCDR *stream
                          ACE_ENV_ARG_DECL)
{
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::TypeCode_var param;

  // skip the Repository ID
  if (!stream->skip_string ())
    return CORBA::TypeCode::TRAVERSE_STOP;

  // Number of fields in the exception
  int member_count = tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  for (int i = 0; i < member_count
         && retval == CORBA::TypeCode::TRAVERSE_CONTINUE;
       i++)
    {
      param = tc->member_type (i ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

      retval = TAO_Marshal_Object::perform_skip (param.in (),
                                                 stream
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
    }

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Except::skip detected error\n")));
  ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                    CORBA::COMPLETED_MAYBE),
                    CORBA::TypeCode::TRAVERSE_STOP);
}

// decode wstring
CORBA::TypeCode::traverse_status
TAO_Marshal_WString::skip (CORBA::TypeCode_ptr,
                           TAO_InputCDR *stream
                           ACE_ENV_ARG_DECL)
{
  ACE_CDR::Boolean continue_skipping = 1;

  // On decode, omit the check against specified wstring bounds, and
  // cope with illegal "zero length" strings (all lengths on the wire
  // must include a NUL).
  //
  // This is on the principle of being gracious in what we accept; we
  // don't generate messages that fail to comply with protocol specs,
  // but we will accept them when it's clear how to do so.

  // "zero length" wstrings are legal in GIOP 1.2.

  continue_skipping = stream->skip_wstring ();

  if (continue_skipping == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_WString::skip detected error\n")));
  ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                    CORBA::COMPLETED_MAYBE),
                    CORBA::TypeCode::TRAVERSE_STOP);
}

CORBA::TypeCode::traverse_status
TAO_Marshal_Value::skip (CORBA::TypeCode_ptr  tc,
                         TAO_InputCDR *stream
                         ACE_ENV_ARG_DECL)
{
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::TypeCode_var param;

  // Use the same method to skip over our base valuetype.
  // To achive this we'll need to distinguish between
  // first-time/nested skips so that we won't attempt to
  // skip rep_id several times.
  //
  if (nested_processing_ == 0)
    {
      nested_processing_ = 1;

      CORBA::ULong value_tag;

      if (!stream->read_ulong (value_tag))
        {
          return CORBA::TypeCode::TRAVERSE_STOP;
        }

      if (value_tag == 0) // Null value type pointer.
        {
          //We are done.
          return retval;
        }
      else if (value_tag & TAO_OBV_GIOP_Flags::Type_info_single)
        {
          // Skip a single repository id which is of type string.
          stream->skip_string ();
        }
      else
        {
          //@@ boris: VT CDR
          return CORBA::TypeCode::TRAVERSE_STOP;
        }
    }

  // Handle our base valuetype if any.
  param = tc->concrete_base_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
  if (param->kind () != CORBA::tk_null)
    {
      retval = this->skip (param.in (), stream ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

      if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          return retval;
        }
    }

  // Number of fields in the valuetype.
  int member_count = tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

  for (int i = 0; i < member_count
         && retval == CORBA::TypeCode::TRAVERSE_CONTINUE;
       i++)
    {
      param = tc->member_type (i ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);

      retval = TAO_Marshal_Object::perform_skip (param.in (),
                                                 stream
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::TRAVERSE_STOP);
    }

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Value::skip detected error\n")));
  ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                    CORBA::COMPLETED_MAYBE),
                    CORBA::TypeCode::TRAVERSE_STOP);
}
