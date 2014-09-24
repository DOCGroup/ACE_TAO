//=============================================================================
/**
 *  @file     skip.cpp
 *
 *  $Id$
 *
 *   Code for skipping different data types
 *
 *   Data types encoded as CDR streams need to be skipped when they
 *   are part of an Any.
 *
 *
 *  @author  Aniruddha Gokhale
 */
//=============================================================================


#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/Any.h"

#include "tao/debug.h"
#include "tao/Valuetype_Adapter.h"
#include "tao/ORB_Core.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"

#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::traverse_status
TAO_Marshal_Primitive::skip (CORBA::TypeCode_ptr  tc, TAO_InputCDR *stream)
{
  CORBA::Boolean continue_skipping = true;

  // Status of skip operation.
  TAO::traverse_status retval = TAO::TRAVERSE_CONTINUE;

  CORBA::TCKind const k = tc->kind ();

  switch (k)
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
      retval = TAO::TRAVERSE_STOP;
      // we are not a primitive type
    }
  if (retval == TAO::TRAVERSE_CONTINUE && continue_skipping)
    return TAO::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("TAO_Marshal_Primitive::skip detected error\n")
          ));
      throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
    }
}

TAO::traverse_status
TAO_Marshal_Any::skip (CORBA::TypeCode_ptr, TAO_InputCDR *stream)
{
  // Typecode of the element that makes the Any.
  CORBA::TypeCode_var elem_tc;

  // Status of encode operation.
  if (!(*stream >> elem_tc.inout ()))
    return TAO::TRAVERSE_STOP;

  return TAO_Marshal_Object::perform_skip (elem_tc.in (), stream);
}

TAO::traverse_status
TAO_Marshal_TypeCode::skip (CORBA::TypeCode_ptr, TAO_InputCDR *stream)
{
  CORBA::Boolean continue_skipping = true;

  // Typecode kind.
  CORBA::ULong kind;

  // Decode the "kind" field of the typecode from the stream.
  continue_skipping = stream->read_ulong (kind);

  if (continue_skipping)
    {
      // Typecodes with empty parameter lists all have preallocated
      // constants.  We use those to reduce memory consumption and
      // heap access ... also, to speed things up!
      if ((kind < CORBA::TAO_TC_KIND_COUNT) ||
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
            case CORBA::tk_value_box:
            case CORBA::tk_native:
            case CORBA::tk_abstract_interface:
            case CORBA::tk_local_interface:
            case CORBA::tk_component:
            case CORBA::tk_home:
            case CORBA::tk_event:
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
            TAOLIB_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO_Marshal_TypeCode::skip: ")
                        ACE_TEXT ("Bad kind_ value in CDR stream\n")));
          throw ::CORBA::BAD_TYPECODE ();
        }
    }

  if (continue_skipping)
    return TAO::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("TAO_Marshal_TypeCode::skip detected error\n")
          ));
      throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
    }
}

TAO::traverse_status
TAO_Marshal_Principal::skip (CORBA::TypeCode_ptr, TAO_InputCDR *stream)
{
  CORBA::Boolean continue_skipping = true;

  // specifies the number of bytes in the Principal
  CORBA::ULong len;

  continue_skipping = stream->read_ulong (len);
  if (len > 0 && continue_skipping)
    {
      continue_skipping = stream->skip_bytes (len);
    }

  if (continue_skipping)
    return TAO::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("TAO_Marshal_Principal::skip detected error\n")
          ));
      throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
    }
}

TAO::traverse_status
TAO_Marshal_ObjRef::skip (CORBA::TypeCode_ptr, TAO_InputCDR *stream)
{
  CORBA::Boolean continue_skipping = true;

  // return status
  TAO::traverse_status retval = TAO::TRAVERSE_CONTINUE;

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

  if (retval == TAO::TRAVERSE_CONTINUE && continue_skipping)
    return TAO::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("TAO_Marshal_ObjRef::skip detected error\n")
          ));
      throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
    }
}

TAO::traverse_status
TAO_Marshal_Struct::skip (CORBA::TypeCode_ptr  tc, TAO_InputCDR *stream)
{
  TAO::traverse_status retval = TAO::TRAVERSE_CONTINUE;
  CORBA::TypeCode_var param;

  // Number of fields in the struct.
  CORBA::ULong const member_count =
    tc->member_count ();

  for (CORBA::ULong i = 0;
       i < member_count && retval == TAO::TRAVERSE_CONTINUE;
       ++i)
    {
      param = tc->member_type (i);

      retval = TAO_Marshal_Object::perform_skip (param.in (), stream);
    }

  if (retval == TAO::TRAVERSE_CONTINUE)
    return TAO::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    TAOLIB_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Struct::skip detected error\n")));

  throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
}

TAO::traverse_status
TAO_Marshal_Union::skip (CORBA::TypeCode_ptr  tc, TAO_InputCDR *src)
{
  CORBA::TypeCode_var discrim_tc =
    tc->discriminator_type ();

  CORBA::ULong const kind =
    discrim_tc->kind ();

  // Save the discriminator value in a temporary variable...
  CORBA::Short short_v = CORBA::Short();
  CORBA::UShort ushort_v = CORBA::UShort();
  CORBA::Long long_v = CORBA::Long();
  CORBA::ULong ulong_v = CORBA::ULong();
  CORBA::ULong enum_v = CORBA::ULong();
  CORBA::Char char_v = CORBA::Char();
  CORBA::WChar wchar_v = CORBA::WChar();
  CORBA::Boolean boolean_v = false;

  switch (kind)
    {
    case CORBA::tk_short:
      {
        if (!src->read_short (short_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_ushort:
      {
        if (!src->read_ushort (ushort_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_long:
      {
        if (!src->read_long (long_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_ulong:
      {
        if (!src->read_ulong (ulong_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_enum:
      {
        if (!src->read_ulong (enum_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_char:
      {
        if (!src->read_char (char_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_wchar:
      {
        if (!src->read_wchar (wchar_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_boolean:
      {
        if (!src->read_boolean (boolean_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    default:
      return TAO::TRAVERSE_STOP;
    }

  const CORBA::ULong member_count =
    tc->member_count ();

  const CORBA::ULong null_member = ~static_cast<CORBA::ULong> (0U);

  CORBA::ULong current_member = null_member;
  CORBA::ULong default_member = null_member;

  for (CORBA::ULong i = 0;
       i < member_count && current_member == null_member;
       ++i)
    {
      CORBA::Any_var any = tc->member_label (i);

      CORBA::Octet o;
      if ((any >>= CORBA::Any::to_octet (o)) && o == 0)
        {
          CORBA::ULong default_index =
            tc->default_index ();

          if (i != default_index)
            throw ::CORBA::BAD_TYPECODE ();
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
            TAO::Any_Impl *impl = any->impl ();

            if (impl->encoded ())
              {
                TAO::Unknown_IDL_Type * const unk =
                  dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

                if (!unk)
                  throw ::CORBA::INTERNAL ();

                // We don't want unk's rd_ptr to move, in case
                // we are shared by another Any, so we use this
                // to copy the state, not the buffer.
                TAO_InputCDR for_reading (unk->_tao_get_cdr ());

                for_reading.read_ulong (d);
              }
            else
              {
                TAO_OutputCDR out;
                impl->marshal_value (out);
                TAO_InputCDR cdr (out);
                cdr.read_ulong (d);
              }

            if (d == enum_v)
              {
                current_member = i;
              }
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
          return TAO::TRAVERSE_STOP;
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
            tc->member_type (default_member);
          return TAO_Marshal_Object::perform_skip (member_tc.in (), src);
        }

      // If we're here, we have an implicit default case, and we
      // should just return without skipping anything, since no
      // union member was marshaled in the first place.
      return TAO::TRAVERSE_CONTINUE;
    }

  // If we found the member successfully then just use that one...
  CORBA::TypeCode_var member_tc =
    tc->member_type (current_member);

  return TAO_Marshal_Object::perform_skip (member_tc.in (), src);
}

TAO::traverse_status
TAO_Marshal_String::skip (CORBA::TypeCode_ptr, TAO_InputCDR *stream)
{
  CORBA::Boolean continue_skipping = true;

  // On decode, omit the check against specified string bounds, and
  // cope with illegal "zero length" strings (all lengths on the wire
  // must include a NUL).
  //
  // This is on the principle of being gracious in what we accept; we
  // don't generate messages that fail to comply with protocol specs,
  // but we will accept them when it's clear how to do so.

  continue_skipping = stream->skip_string ();
  if (continue_skipping)
    return TAO::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO_Marshal_String::skip detected error\n")));
      throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
    }
}

TAO::traverse_status
TAO_Marshal_Sequence::skip (CORBA::TypeCode_ptr tc, TAO_InputCDR *stream)
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
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO_Marshal_Sequence::skip detected error\n")));
      throw ::CORBA::MARSHAL ();
    }

  // No point decoding an empty sequence.
  if (bounds == 0)
    return TAO::TRAVERSE_CONTINUE;

  // Get element typecode.
  CORBA::TypeCode_var tc2 =
    tc->content_type ();

  // For CORBA basic types, the skip can be optimized
  CORBA::TCKind const kind = tc2->kind ();

  char *dummy = 0;
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
      while (bounds-- && continue_skipping)
        {
          continue_skipping =
            TAO_Marshal_Object::perform_skip (tc2.in (), stream);
        }
      break;
    }// end of switch

  if (continue_skipping)
    return TAO::TRAVERSE_CONTINUE;

  // error exit
  if (TAO_debug_level > 0)
    TAOLIB_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Sequence::skip detected error\n")));

  throw ::CORBA::MARSHAL ();
}

TAO::traverse_status
TAO_Marshal_Array::skip (CORBA::TypeCode_ptr tc, TAO_InputCDR *stream)
{
  CORBA::Boolean continue_skipping = true;

  // retrieve the bounds of the array
  CORBA::ULong bounds = tc->length ();

  // get element typecode
  // Typecode of the element.
  CORBA::TypeCode_var tc2 = tc->content_type ();

  // For CORBA basic types, the skip can be optimized
  CORBA::TCKind const kind = tc2->kind ();

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
      while (bounds-- && continue_skipping)
        {
          int stop =
            TAO_Marshal_Object::perform_skip (tc2.in (), stream);
          if (stop == TAO::TRAVERSE_STOP)
            continue_skipping = false;
        }
      break;
    }// end of switch

  if (continue_skipping)
    return TAO::TRAVERSE_CONTINUE;

  // error exit
  if (TAO_debug_level > 0)
    TAOLIB_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Sequence::skip detected error\n")));

  throw ::CORBA::MARSHAL ();
}

TAO::traverse_status
TAO_Marshal_Alias::skip (CORBA::TypeCode_ptr  tc, TAO_InputCDR *stream)
{
  // Typecode of the aliased type.
  CORBA::TypeCode_var tc2;
  CORBA::Boolean continue_skipping = true;

  // Status of decode operation.
  TAO::traverse_status retval =
    TAO::TRAVERSE_CONTINUE;

  tc2 = tc->content_type ();

  retval = TAO_Marshal_Object::perform_skip (tc2.in (), stream);

  //  tc2->_decr_refcnt ();
  if (retval == TAO::TRAVERSE_CONTINUE
      && continue_skipping)
    return TAO::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    TAOLIB_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Alias::skip detected error\n")));

  throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
}

// Decode exception For exceptions, the "hidden" type ID near the
// front of the on-wire representation was previously unmarshaled and
// mapped to the "tc" typcode we're using to traverse the memory ...
// at the same time its vtable, refcount, and other state was
// established.
//
// NOTE: This is asymmetric with respect to encoding exceptions.
TAO::traverse_status
TAO_Marshal_Except::skip (CORBA::TypeCode_ptr tc, TAO_InputCDR *stream)
{
  TAO::traverse_status retval =
    TAO::TRAVERSE_CONTINUE;
  CORBA::TypeCode_var param;

  // skip the Repository ID
  if (!stream->skip_string ())
    return TAO::TRAVERSE_STOP;

  // Number of fields in the exception
  const CORBA::ULong member_count =
    tc->member_count ();

  for (CORBA::ULong i = 0;
       i < member_count && retval == TAO::TRAVERSE_CONTINUE;
       ++i)
    {
      param = tc->member_type (i);

      retval = TAO_Marshal_Object::perform_skip (param.in (), stream);
    }

  if (retval == TAO::TRAVERSE_CONTINUE)
    return TAO::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    TAOLIB_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Except::skip detected error\n")));

  throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
}

// decode wstring
TAO::traverse_status
TAO_Marshal_WString::skip (CORBA::TypeCode_ptr, TAO_InputCDR *stream)
{
  CORBA::Boolean continue_skipping = true;

  // On decode, omit the check against specified wstring bounds, and
  // cope with illegal "zero length" strings (all lengths on the wire
  // must include a NUL).
  //
  // This is on the principle of being gracious in what we accept; we
  // don't generate messages that fail to comply with protocol specs,
  // but we will accept them when it's clear how to do so.

  // "zero length" wstrings are legal in GIOP 1.2.

  continue_skipping = stream->skip_wstring ();

  if (continue_skipping)
    return TAO::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    TAOLIB_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_WString::skip detected error\n")));
  throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
}

TAO::traverse_status
TAO_Marshal_Value::skip (CORBA::TypeCode_ptr tc, TAO_InputCDR *stream)
{
  TAO::traverse_status retval = TAO::TRAVERSE_CONTINUE;

  // Use the same method to skip over our base valuetype.
  // To achive this we'll need to distinguish between
  // first-time/nested skips so that we won't attempt to
  // skip rep_id several times.
  //
  if (this->nested_processing_ == false)
    {
      this->nested_processing_ = true;

      CORBA::Long value_tag;

      if (!stream->read_long (value_tag))
        {
          return TAO::TRAVERSE_STOP;
        }

      TAO_ORB_Core *orb_core = stream->orb_core ();
      if (orb_core == 0)
        {
          orb_core = TAO_ORB_Core_instance ();

          if (TAO_debug_level > 0)
            {
              TAOLIB_DEBUG ((LM_WARNING,
                          "TAO (%P|%t) WARNING: extracting "
                          "valuetype using default ORB_Core\n"));
            }
        }

      TAO_Valuetype_Adapter *adapter = orb_core->valuetype_adapter();

      if (value_tag == 0) // Null value type pointer.
        {
          // We are done.
          return retval;
        }
      else if (adapter->is_type_info_single(value_tag))
        {
          // Skip a single repository id which is of type string.
          stream->skip_string ();
        }
      else if (adapter->is_type_info_list(value_tag))
        {
          CORBA::Long num_types;
          if (!stream->read_long (num_types))
            {
              return TAO::TRAVERSE_STOP;
            }
          while (num_types > 0)
            {
              stream->skip_string();
              num_types--;
            }
        }
      else if (!adapter->is_type_info_implied (value_tag))
        {
          //@@ boris: VT CDR
          return TAO::TRAVERSE_STOP;
        }

      if (adapter->is_value_chunked (value_tag))
        {
          CORBA::Long chunk_tag = 0;
          while (chunk_tag != -1)
            {
              if (!stream->read_long (chunk_tag))
                return TAO::TRAVERSE_STOP;

              if (chunk_tag > 0)
                {
                  if (!stream->skip_bytes(chunk_tag))
                    return TAO::TRAVERSE_STOP;
                }
            }
          return TAO::TRAVERSE_CONTINUE;
        }

    }

  CORBA::TypeCode_var param;

  if (CORBA::tk_value_box == tc->kind ())
    {
      param = tc->content_type ();
      retval = TAO_Marshal_Object::perform_skip (param.in (), stream);
    }
  else // tc->kind () must be  tk_value  or  tk_event
    {
      // Handle our base valuetype if any.
      param = tc->concrete_base_type ();
      if (CORBA::tk_null != param->kind ())
        {
          retval = this->skip (param.in (), stream);
        }

      if (retval == TAO::TRAVERSE_CONTINUE)
        {
          // Number of fields in the valuetype.
          CORBA::ULong const member_count =
            tc->member_count ();

          for (CORBA::ULong i = 0;
               i < member_count && retval == TAO::TRAVERSE_CONTINUE;
               ++i)
            {
              param = tc->member_type (i);
              retval = TAO_Marshal_Object::perform_skip (
                         param.in (), stream);
            }
        }
    }

  if (retval == TAO::TRAVERSE_CONTINUE)
    return TAO::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    TAOLIB_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Value::skip detected error\n")));

  throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
}

TAO_END_VERSIONED_NAMESPACE_DECL
