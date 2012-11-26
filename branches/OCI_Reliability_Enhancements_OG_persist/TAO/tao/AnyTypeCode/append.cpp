//=============================================================================
/**
 *  @file     append.cpp
 *
 *  $Id$
 *
 *   Appends a CDR stream to another CDR stream. Due to the
 *   stringent alignment requirements, it is not possible to simply
 *   append or memcpy. Instead we go thru the same CDR encoding rules
 *
 *
 *  @author  Copyright 1994-1995 by Sun Microsystems Inc.  and Aniruddha Gokhale
 */
//=============================================================================


#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/AnyTypeCode/OctetSeqA.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/debug.h"
#include "tao/Valuetype_Adapter.h"
#include "tao/ORB_Core.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"

#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Encode instances of arbitrary data types based only on typecode.
// "data" points to the data type; if it's not a primitve data type,
// the TypeCode interpreter is used to recursively encode its
// components.  "context" is the marshaling stream on which to encode
// the data value.

TAO::traverse_status
TAO_Marshal_Primitive::append (CORBA::TypeCode_ptr tc,
                               TAO_InputCDR *src,
                               TAO_OutputCDR *dest)
{
  CORBA::Boolean continue_append = true;
  TAO::traverse_status retval =
    TAO::TRAVERSE_CONTINUE; // status of encode operation

  CORBA::TCKind const k = tc->kind ();

  switch (k)
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
      break;
    case CORBA::tk_short:
    case CORBA::tk_ushort:
      continue_append = dest->append_short (*src);
      break;
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_enum:
      continue_append = dest->append_long (*src);
      break;
    case CORBA::tk_double:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
      continue_append = dest->append_double (*src);
      break;
    case CORBA::tk_boolean:
      continue_append = dest->append_boolean (*src);
      break;
    case CORBA::tk_char:
    case CORBA::tk_octet:
      continue_append = dest->append_octet (*src);
      break;
    case CORBA::tk_longdouble:
      continue_append = dest->append_longdouble (*src);
      break;
    case CORBA::tk_wchar:
      continue_append = dest->append_wchar (*src);
      break;
    default:
      retval = TAO::TRAVERSE_STOP;
      // we are not a primitive type
    }

  if (retval == TAO::TRAVERSE_CONTINUE
      && continue_append == 1)
    return TAO::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((
        LM_DEBUG,
        ACE_TEXT ("TAO_Marshal_Primitive::append detected error\n")
      ));

  throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
}

TAO::traverse_status
TAO_Marshal_Any::append (CORBA::TypeCode_ptr,
                         TAO_InputCDR *src,
                         TAO_OutputCDR *dest)
{
  // Typecode of the element that makes the Any.
  CORBA::TypeCode_var elem_tc;

  if (!(*src >> elem_tc.inout ()))
    throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);

  if (!(*dest << elem_tc.in ()))
    throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);

  // append the data
  TAO::traverse_status retval =
    TAO_Marshal_Object::perform_append (elem_tc.in (), src, dest);

  if (retval != TAO::TRAVERSE_CONTINUE)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO_Marshal_Any::append detected error\n")));

      throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
    }

  return retval;
}

TAO::traverse_status
TAO_Marshal_TypeCode::append (CORBA::TypeCode_ptr,
                              TAO_InputCDR *src,
                              TAO_OutputCDR *dest)
{
  CORBA::Boolean continue_append = true;
  TAO::traverse_status retval =
    TAO::TRAVERSE_CONTINUE;
  CORBA::ULong kind;

  // Decode the "kind" field of the typecode from the src for further
  // use. However, also write it back into the destination
  continue_append = (CORBA::Boolean) (src->read_ulong (kind)
                                      ? dest->write_ulong (kind)
                                      : false);

  if (continue_append == true)
    {
      // Typecodes with empty parameter lists all have preallocated
      // constants.  We use those to reduce memory consumption and
      // heap access ... also, to speed things up!
      if ((kind < CORBA::TAO_TC_KIND_COUNT)
          || (kind == ~0u))
        {
          // Either a non-constant typecode or an indirected typecode.
          switch (kind)
            {
              // Need special handling for all kinds of typecodes that
              // have nonempty parameter lists ...
            default:
              // nothing to de done
              break;
            case CORBA::tk_string:
            case CORBA::tk_wstring:
              {
                // read and write the bounds
                retval =
                  TAO_Marshal_Object::perform_append (CORBA::_tc_long,
                                                      src,
                                                      dest);
              }
            break;

            // Indirected typecodes, illegal at "top level"
            case ~0u:
              {
                // read and write the negative offset
                retval =
                  TAO_Marshal_Object::perform_append (CORBA::_tc_long,
                                                      src,
                                                      dest);
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
                // write the encapsulation i.e., octet sequence
                retval =
                  TAO_Marshal_Object::perform_append (CORBA::_tc_OctetSeq,
                                                      src,
                                                      dest);
              }
            } // end of switch
        }
      else // bad kind_ value to be decoded
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO_Marshal_TypeCode: ")
                          ACE_TEXT ("Bad kind_ value in CDR stream\n")));
            }

          throw ::CORBA::BAD_TYPECODE ();
        }
    }

  if (continue_append == 1 && retval == TAO::TRAVERSE_CONTINUE)
    {
      return TAO::TRAVERSE_CONTINUE;
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO_Marshal_TypeCode::append detected error\n")));
    }

  throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
}

TAO::traverse_status
TAO_Marshal_Principal::append (CORBA::TypeCode_ptr,
                               TAO_InputCDR *src,
                               TAO_OutputCDR *dest)
{
  // write the octet sequence representing the Principal
  return TAO_Marshal_Object::perform_append (CORBA::_tc_OctetSeq, src, dest);
}

TAO::traverse_status
TAO_Marshal_ObjRef::append (CORBA::TypeCode_ptr,
                            TAO_InputCDR *src,
                            TAO_OutputCDR *dest)
{
  CORBA::Boolean continue_append = true;

  // First, append the type hint. This will be the type_id encoded in an
  // object reference.
  dest->append_string (*src);

  // Read the profiles, discarding all until an IIOP profile comes by.
  // Once we see an IIOP profile, ignore any further ones.
  //
  // XXX this will need to change someday to let different protocol
  // code be accessed, not just IIOP.  Protocol modules will be
  // dynamically loaded from shared libraries via ORB_init (), and we
  // just need to be able to access such preloaded libraries here as
  // we unmarshal objrefs.

  CORBA::ULong profiles = 0;

  // get the count of profiles that follow. This will tell us the
  // length of the sequence
  continue_append = (CORBA::Boolean) (src->read_ulong (profiles)
                                      ? dest->write_ulong (profiles)
                                      : 0);

  // No profiles means a NIL objref.
  while (profiles-- != 0 && continue_append)
    {
      CORBA::ULong tag = 0;

      // get the profile ID tag
      if ((continue_append = (CORBA::Boolean) (src->read_ulong (tag)
                                               ? dest->write_ulong (tag)
                                               : 0))  == 0)
        continue;

      CORBA::ULong length = 0;
      if ((continue_append = (CORBA::Boolean) (src->read_ulong (length)
                              ? dest->write_ulong (length)
                              : 0)) == 0)
        continue;

      // @@ This can be optimized! Pre-allocating on the destination
      //    and then copying directly into that.
      CORBA::Octet* body = 0;
      ACE_NEW_RETURN (body,
                      CORBA::Octet[length],
                      TAO::TRAVERSE_STOP);
      continue_append =
        (CORBA::Boolean) (src->read_octet_array (body, length)
                          ? dest->write_octet_array (body, length)
                          : 0);
      delete [] body;
    }

  if (continue_append == 1)
    return TAO::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((
        LM_DEBUG,
        ACE_TEXT ("TAO_Marshal_ObjRef::append detected error\n")
      ));

  throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
}

TAO::traverse_status
TAO_Marshal_Struct::append (CORBA::TypeCode_ptr  tc,
                            TAO_InputCDR *src,
                            TAO_OutputCDR *dest)
{
  TAO::traverse_status retval =
    TAO::TRAVERSE_CONTINUE;
  CORBA::TypeCode_var param;

  // Number of fields in the struct.
  const CORBA::ULong member_count =
    tc->member_count ();

  for (CORBA::ULong i = 0;
       i < member_count && retval == TAO::TRAVERSE_CONTINUE;
       ++i)
    {
      // get member type
      param = tc->member_type (i);

      retval = TAO_Marshal_Object::perform_append (param.in (), src, dest);
    }

  if (retval == TAO::TRAVERSE_CONTINUE)
    return TAO::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Struct::append detected error\n")));

  throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
}

TAO::traverse_status
TAO_Marshal_Union::append (CORBA::TypeCode_ptr tc,
                           TAO_InputCDR *src,
                           TAO_OutputCDR *dest)
{
  CORBA::TypeCode_var discrim_tc = tc->discriminator_type ();

  CORBA::ULong kind = discrim_tc->kind ();

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
        if (!src->read_short (short_v)
            || !dest->write_short (short_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_ushort:
      {
        if (!src->read_ushort (ushort_v)
            || !dest->write_ushort (ushort_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_long:
      {
        if (!src->read_long (long_v)
            || !dest->write_long (long_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_ulong:
      {
        if (!src->read_ulong (ulong_v)
            || !dest->write_ulong (ulong_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_enum:
      {
        if (!src->read_ulong (enum_v)
            || !dest->write_ulong (enum_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_char:
      {
        if (!src->read_char (char_v)
            || !dest->write_char (char_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_wchar:
      {
        if (!src->read_wchar (wchar_v)
            || !dest->write_wchar (wchar_v))
          return TAO::TRAVERSE_STOP;
      }
      break;

    case CORBA::tk_boolean:
      {
        if (!src->read_boolean (boolean_v)
            || !dest->write_boolean (boolean_v))
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
          return TAO_Marshal_Object::perform_append (member_tc.in (),
                                                     src,
                                                     dest);
        }

      // If we're here, we have an implicit default case, and we
      // should just return without appending anything, since no
      // union member was marshaled in the first place.
      return TAO::TRAVERSE_CONTINUE;
    }

  // If we found the member successfully then just use that one...
  CORBA::TypeCode_var member_tc =
    tc->member_type (current_member);
  return TAO_Marshal_Object::perform_append (member_tc.in (),
                                             src,
                                             dest
                                            );
}

TAO::traverse_status
TAO_Marshal_String::append (CORBA::TypeCode_ptr,
                            TAO_InputCDR *src,
                            TAO_OutputCDR *dest)
{
  CORBA::Boolean continue_append = true;

  // On decode, omit the check against specified string bounds, and
  // cope with illegal "zero length" strings (all lengths on the wire
  // must include a NUL).
  //
  // This is on the principle of being gracious in what we accept; we
  // don't generate messages that fail to comply with protocol specs,
  // but we will accept them when it's clear how to do so.

  continue_append = dest->append_string (*src);
  if (continue_append == 1)
    return TAO::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_TypeCode::append detected error\n")));

  throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
}

TAO::traverse_status
TAO_Marshal_Sequence::append (CORBA::TypeCode_ptr  tc,
                              TAO_InputCDR *src,
                              TAO_OutputCDR *dest)
{
  // Size of element.
  CORBA::ULong bounds;

  // First unmarshal the sequence length ... we trust it to be right
  // here, on the "be gracious in what you accept" principle.  We
  // don't generate illegal sequences (i.e. length > bounds).

  CORBA::Boolean continue_append =
    (CORBA::Boolean) (src->read_ulong (bounds)
                      ? dest->write_ulong (bounds)
                      : 0);

  if (!continue_append)
    {
      ACE_DEBUG ((
          LM_DEBUG,
          ACE_TEXT ("TAO_Marshal_Sequence::append detected error\n")
        ));
      throw ::CORBA::MARSHAL ();
    }

  if (bounds == 0)
    {
      return TAO::TRAVERSE_CONTINUE;
    }

  if (continue_append)
    {
      // Get element typecode.
      CORBA::TypeCode_var tc2 =
        tc->content_type ();

      TAO::traverse_status retval =
        TAO::TRAVERSE_CONTINUE;

      CORBA::TCKind kind = tc2->kind ();

      switch (kind)
        {
        case CORBA::tk_octet:
          {
            char* buf;
            if (dest->adjust (ACE_CDR::OCTET_SIZE * bounds,
                              ACE_CDR::OCTET_ALIGN, buf) == 0)
              {
                if (src->read_octet_array ((ACE_CDR::Octet*)buf, bounds) == 0)
                  retval = TAO::TRAVERSE_STOP;
              }
          }
          break;
        case CORBA::tk_boolean:
          {
            char* buf;
            if (dest->adjust (ACE_CDR::OCTET_SIZE * bounds,
                              ACE_CDR::OCTET_ALIGN, buf) == 0)
              {
                if (src->read_boolean_array ((ACE_CDR::Boolean*)buf, bounds) == 0)
                  retval = TAO::TRAVERSE_STOP;
              }
          }
          break;
        case CORBA::tk_char:
          {
            char* buf;
            if (dest->adjust (ACE_CDR::OCTET_SIZE * bounds,
                              ACE_CDR::OCTET_ALIGN, buf) == 0)
              {
                if (src->read_char_array ((ACE_CDR::Char*)buf, bounds) == 0)
                  retval = TAO::TRAVERSE_STOP;
              }
          }
          break;
        case CORBA::tk_short:
          {
            char* buf;
            if (dest->adjust (ACE_CDR::SHORT_SIZE * bounds,
                              ACE_CDR::SHORT_ALIGN, buf) == 0)
              {
                if (src->read_short_array ((ACE_CDR::Short*)buf, bounds) == 0)
                  retval = TAO::TRAVERSE_STOP;
              }
          }
          break;
        case CORBA::tk_ushort:
          {
            char* buf;
            if (dest->adjust (ACE_CDR::SHORT_SIZE * bounds,
                              ACE_CDR::SHORT_ALIGN, buf) == 0)
              {
                if (src->read_ushort_array ((ACE_CDR::UShort*)buf, bounds) == 0)
                  retval = TAO::TRAVERSE_STOP;
              }
          }
          break;
        case CORBA::tk_wchar:
          {
            char* buf;
            if (dest->adjust (ACE_CDR::SHORT_SIZE * bounds,
                              ACE_CDR::SHORT_ALIGN, buf) == 0)
              {
                if (src->read_wchar_array ((ACE_CDR::WChar*)buf, bounds) == 0)
                  retval = TAO::TRAVERSE_STOP;
              }
          }
          break;
        case CORBA::tk_long:
          {
            char* buf;
            if (dest->adjust (ACE_CDR::LONG_SIZE * bounds,
                              ACE_CDR::LONG_ALIGN, buf) == 0)
              {
                if (src->read_long_array ((ACE_CDR::Long*)buf, bounds) == 0)
                  retval = TAO::TRAVERSE_STOP;
              }
          }
          break;
        case CORBA::tk_ulong:
          {
            char* buf;
            if (dest->adjust (ACE_CDR::LONG_SIZE * bounds,
                              ACE_CDR::LONG_ALIGN, buf) == 0)
              {
                if (src->read_ulong_array ((ACE_CDR::ULong*)buf, bounds) == 0)
                  retval = TAO::TRAVERSE_STOP;
              }
          }
          break;
        case CORBA::tk_float:
          {
            char* buf;
            if (dest->adjust (ACE_CDR::LONG_SIZE * bounds,
                              ACE_CDR::LONG_ALIGN, buf) == 0)
              {
                if (src->read_float_array ((ACE_CDR::Float*)buf, bounds) == 0)
                  retval = TAO::TRAVERSE_STOP;
              }
          }
          break;
        case CORBA::tk_double:
          {
            char* buf;
            if (dest->adjust (ACE_CDR::LONGLONG_SIZE * bounds,
                              ACE_CDR::LONGLONG_ALIGN, buf) == 0)
              {
                if (src->read_double_array ((ACE_CDR::Double*)buf, bounds) == 0)
                  retval = TAO::TRAVERSE_STOP;
              }
          }
          break;
        case CORBA::tk_longlong:
          {
            char* buf;
            if (dest->adjust (ACE_CDR::LONGLONG_SIZE * bounds,
                              ACE_CDR::LONGLONG_ALIGN, buf) == 0)
              {
                if (src->read_longlong_array ((ACE_CDR::LongLong*)buf, bounds) == 0)
                  retval = TAO::TRAVERSE_STOP;
              }
          }
          break;
        case CORBA::tk_ulonglong:
          {
            char* buf;
            if (dest->adjust (ACE_CDR::LONGLONG_SIZE * bounds,
                              ACE_CDR::LONGLONG_ALIGN, buf) == 0)
              {
                if (src->read_ulonglong_array ((ACE_CDR::ULongLong*)buf, bounds) == 0)
                  retval = TAO::TRAVERSE_STOP;
              }
          }
          break;
        case CORBA::tk_longdouble:
          {
            char* buf;
            if (dest->adjust (ACE_CDR::LONGDOUBLE_SIZE * bounds,
                              ACE_CDR::LONGDOUBLE_ALIGN, buf) == 0)
              {
                if (src->read_longdouble_array ((ACE_CDR::LongDouble*)buf, bounds) == 0)
                  retval = TAO::TRAVERSE_STOP;
              }
          }
          break;

        default:
          while (bounds-- && retval == TAO::TRAVERSE_CONTINUE)
            {
              retval = TAO_Marshal_Object::perform_append (tc2.in (),
                                                           src,
                                                           dest
                                                           );
            }
          break;
        }// end of switch

      if (retval == TAO::TRAVERSE_CONTINUE)
        return TAO::TRAVERSE_CONTINUE;
    }
  // error exit
  if (TAO_debug_level > 0)
    ACE_DEBUG ((
        LM_DEBUG,
        ACE_TEXT ("marshaling TAO_Marshal_Sequence::append detected error\n")
      ));

  throw ::CORBA::MARSHAL ();
}

TAO::traverse_status
TAO_Marshal_Array::append (CORBA::TypeCode_ptr  tc,
                           TAO_InputCDR *src,
                           TAO_OutputCDR *dest)
{
  // retrieve the bounds of the array
  CORBA::ULong bounds = tc->length ();

  // get element typecode
  CORBA::TypeCode_var tc2 = tc->content_type ();

  // For CORBA basic types, the copy can be optimized
  CORBA::TCKind kind = tc2->kind ();

  // Return status.
  TAO::traverse_status retval =
    TAO::TRAVERSE_CONTINUE;

  switch (kind)
    {
    case CORBA::tk_octet:
      {
        char* buf;
        if (dest->adjust (ACE_CDR::OCTET_SIZE * bounds,
                          ACE_CDR::OCTET_ALIGN, buf) == 0)
          {
            if (src->read_octet_array ((ACE_CDR::Octet*)buf, bounds) == 0)
              retval = TAO::TRAVERSE_STOP;
          }
      }
      break;
    case CORBA::tk_boolean:
      {
        char* buf;
        if (dest->adjust (ACE_CDR::OCTET_SIZE * bounds,
                          ACE_CDR::OCTET_ALIGN, buf) == 0)
          {
            if (src->read_boolean_array ((ACE_CDR::Boolean*)buf, bounds) == 0)
              retval = TAO::TRAVERSE_STOP;
          }
      }
      break;
    case CORBA::tk_char:
      {
        char* buf;
        if (dest->adjust (ACE_CDR::OCTET_SIZE * bounds,
                          ACE_CDR::OCTET_ALIGN, buf) == 0)
          {
            if (src->read_char_array ((ACE_CDR::Char*)buf, bounds) == 0)
              retval = TAO::TRAVERSE_STOP;
          }
      }
      break;
    case CORBA::tk_short:
      {
        char* buf;
        if (dest->adjust (ACE_CDR::SHORT_SIZE * bounds,
                          ACE_CDR::SHORT_ALIGN, buf) == 0)
          {
            if (src->read_short_array ((ACE_CDR::Short*)buf, bounds) == 0)
              retval = TAO::TRAVERSE_STOP;
          }
      }
      break;
    case CORBA::tk_ushort:
      {
        char* buf;
        if (dest->adjust (ACE_CDR::SHORT_SIZE * bounds,
                          ACE_CDR::SHORT_ALIGN, buf) == 0)
          {
            if (src->read_ushort_array ((ACE_CDR::UShort*)buf, bounds) == 0)
              retval = TAO::TRAVERSE_STOP;
          }
      }
      break;
    case CORBA::tk_wchar:
      {
        char* buf;
        if (dest->adjust (ACE_CDR::SHORT_SIZE * bounds,
                          ACE_CDR::SHORT_ALIGN, buf) == 0)
          {
            if (src->read_wchar_array ((ACE_CDR::WChar*)buf, bounds) == 0)
              retval = TAO::TRAVERSE_STOP;
          }
      }
      break;
    case CORBA::tk_long:
      {
        char* buf;
        if (dest->adjust (ACE_CDR::LONG_SIZE * bounds,
                          ACE_CDR::LONG_ALIGN, buf) == 0)
          {
            if (src->read_long_array ((ACE_CDR::Long*)buf, bounds) == 0)
              retval = TAO::TRAVERSE_STOP;
          }
      }
      break;
    case CORBA::tk_ulong:
      {
        char* buf;
        if (dest->adjust (ACE_CDR::LONG_SIZE * bounds,
                          ACE_CDR::LONG_ALIGN, buf) == 0)
          {
            if (src->read_ulong_array ((ACE_CDR::ULong*)buf, bounds) == 0)
              retval = TAO::TRAVERSE_STOP;
          }
      }
      break;
    case CORBA::tk_float:
      {
        char* buf;
        if (dest->adjust (ACE_CDR::LONG_SIZE * bounds,
                          ACE_CDR::LONG_ALIGN, buf) == 0)
          {
            if (src->read_float_array ((ACE_CDR::Float*)buf, bounds) == 0)
              retval = TAO::TRAVERSE_STOP;
          }
      }
      break;
    case CORBA::tk_double:
      {
        char* buf;
        if (dest->adjust (ACE_CDR::LONGLONG_SIZE * bounds,
                          ACE_CDR::LONGLONG_ALIGN, buf) == 0)
          {
            if (src->read_double_array ((ACE_CDR::Double*)buf, bounds) == 0)
              retval = TAO::TRAVERSE_STOP;
          }
      }
      break;
    case CORBA::tk_longlong:
      {
        char* buf;
        if (dest->adjust (ACE_CDR::LONGLONG_SIZE * bounds,
                          ACE_CDR::LONGLONG_ALIGN, buf) == 0)
          {
            if (src->read_longlong_array ((ACE_CDR::LongLong*)buf, bounds) == 0)
              retval = TAO::TRAVERSE_STOP;
          }
      }
      break;
    case CORBA::tk_ulonglong:
      {
        char* buf;
        if (dest->adjust (ACE_CDR::LONGLONG_SIZE * bounds,
                          ACE_CDR::LONGLONG_ALIGN, buf) == 0)
          {
            if (src->read_ulonglong_array ((ACE_CDR::ULongLong*)buf, bounds) == 0)
              retval = TAO::TRAVERSE_STOP;
          }
      }
      break;
    case CORBA::tk_longdouble:
      {
        char* buf;
        if (dest->adjust (ACE_CDR::LONGDOUBLE_SIZE * bounds,
                          ACE_CDR::LONGDOUBLE_ALIGN, buf) == 0)
          {
            if (src->read_longdouble_array ((ACE_CDR::LongDouble*)buf, bounds) == 0)
              retval = TAO::TRAVERSE_STOP;
          }
      }
      break;
    default:
      while (bounds-- && retval == TAO::TRAVERSE_CONTINUE)
        {
          retval = TAO_Marshal_Object::perform_append (tc2.in (),
                                                       src,
                                                       dest
                                                      );
        }
      break;
    }// end of switch

  if (retval == TAO::TRAVERSE_CONTINUE)
    return retval;

  // error exit
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Sequence::append detected error\n")));

  throw ::CORBA::MARSHAL ();
}

TAO::traverse_status
TAO_Marshal_Alias::append (CORBA::TypeCode_ptr  tc,
                           TAO_InputCDR *src,
                           TAO_OutputCDR *dest)
{
  // Typecode of the aliased type.
  CORBA::TypeCode_var tc2;
  CORBA::Boolean continue_append = true;

  // Status of decode operation.
  TAO::traverse_status retval =
    TAO::TRAVERSE_CONTINUE;

  tc2 = tc->content_type ();

  retval = TAO_Marshal_Object::perform_append (tc2.in (), src, dest);

  if (retval == TAO::TRAVERSE_CONTINUE
      && continue_append == true)
    return TAO::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Alias::append detected error\n")));
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
TAO_Marshal_Except::append (CORBA::TypeCode_ptr  tc,
                            TAO_InputCDR *src,
                            TAO_OutputCDR *dest
                            )
{
  TAO::traverse_status retval =
    TAO::TRAVERSE_CONTINUE;
  CORBA::Boolean continue_append = true;
  CORBA::TypeCode_var param;

  // first append the RepositoryID
  continue_append = dest->append_string (*src);

  // Number of fields in the struct.
  const CORBA::ULong member_count =
    tc->member_count ();

  for (CORBA::ULong i = 0;
       i < member_count
         && retval == TAO::TRAVERSE_CONTINUE
         && continue_append == 1;
       ++i)
    {
      param = tc->member_type (i);

      retval = TAO_Marshal_Object::perform_append (param.in (),
                                                   src,
                                                   dest);
    }

  if (retval == TAO::TRAVERSE_CONTINUE
      && continue_append == 1)
    return TAO::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Except::append detected error\n")));

  throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
}

TAO::traverse_status
TAO_Marshal_WString::append (CORBA::TypeCode_ptr,
                             TAO_InputCDR *src,
                             TAO_OutputCDR *dest
                             )
{
  CORBA::Boolean continue_append = true;

  // On decode, omit the check against specified wstring bounds, and
  // cope with illegal "zero length" strings (all lengths on the wire
  // must include a NUL).
  //
  // This is on the principle of being gracious in what we accept; we
  // don't generate messages that fail to comply with protocol specs,
  // but we will accept them when it's clear how to do so.

  continue_append = dest->append_wstring (*src);

  if (continue_append == 1)
    return TAO::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_WString::append detected error\n")));

  throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
}

TAO::traverse_status
TAO_Marshal_Value::append (CORBA::TypeCode_ptr  tc,
                           TAO_InputCDR *src,
                           TAO_OutputCDR *dest
                           )
{
  TAO::traverse_status retval =
    TAO::TRAVERSE_CONTINUE;

  // Use the same method to append our base valuetype.
  // To achive this we'll need to distinguish between
  // first-time/nested appends so that we won't attempt to
  // append rep_id several times.
  //
  if (this->nested_processing_ == 0)
    {
      this->nested_processing_ = 1;

      CORBA::ULong value_tag;

      if (!src->read_ulong (value_tag) ||
          !dest->write_ulong (value_tag))
        {
          return TAO::TRAVERSE_STOP;
        }

      TAO_ORB_Core *orb_core = src->orb_core ();
      if (orb_core == 0)
        {
          orb_core = TAO_ORB_Core_instance ();

          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_WARNING,
                          "TAO (%P|%t) WARNING: extracting "
                          "valuetype using default ORB_Core\n"));
            }
        }

      TAO_Valuetype_Adapter *adapter = orb_core->valuetype_adapter();

      if (value_tag == 0) // Null value type pointer.
        {
          //We are done.
          return retval;
        }
      else if (value_tag & adapter->type_info_single ())
        {
          // Append repository id which is of type string.
          dest->append_string (*src);
        }
      else
        {
          //@@ boris: VT CDR
          return TAO::TRAVERSE_STOP;
        }
    }

  CORBA::TypeCode_var param;
  if (CORBA::tk_value_box == tc->kind ())
    {
      param = tc->content_type ();
      retval = TAO_Marshal_Object::perform_append (param.in (),
                                                   src,
                                                   dest
                                                   );
    }
  else // tc->kind () must be  tk_value  or  tk_event
    {
      // Handle our base valuetype if any.
      param = tc->concrete_base_type ();
      if (CORBA::tk_null != param->kind ())
        {
          retval = this->append (param.in (),
                                 src,
                                 dest
                                );
        }

      if (retval == TAO::TRAVERSE_CONTINUE)
        {
          // Number of fields in the struct.
          const CORBA::ULong member_count =
            tc->member_count ();

          for (CORBA::ULong i = 0;
               i < member_count && retval == TAO::TRAVERSE_CONTINUE;
               ++i)
            {
              // get member type
              param = tc->member_type (i);

              retval =
                TAO_Marshal_Object::perform_append (param.in (),
                                                    src,
                                                    dest
                                                   );
            }
        }
    }

  if (retval == TAO::TRAVERSE_CONTINUE)
    return TAO::TRAVERSE_CONTINUE;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Marshal_Value::append detected error\n")));

  throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
}

TAO_END_VERSIONED_NAMESPACE_DECL
