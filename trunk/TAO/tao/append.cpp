// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    append.cpp
//
// = DESCRIPTION
//   Appends a CDR stream to another CDR stream. Due to the stringent alignment
//   requirements, it is not possible to simply append or memcpy. Instead we go
//   thru the same CDR encoding rules
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//     and Aniruddha Gokhale
//
// ============================================================================

#include "tao/Marshal.h"
#include "tao/CDR.h"
#include "tao/Environment.h"
#include "tao/Any.h"
#include "tao/GIOP.h"
#include "tao/debug.h"

ACE_RCSID(tao, append, "$Id$")

// Encode instances of arbitrary data types based only on typecode.
// "data" points to the data type; if it's not a primitve data type,
// the TypeCode interpreter is used to recursively encode its
// components.  "context" is the marshaling stream on which to encode
// the data value.

CORBA::TypeCode::traverse_status
TAO_Marshal_Primitive::append (CORBA::TypeCode_ptr tc,
                               TAO_InputCDR *src,
                               TAO_OutputCDR *dest,
                               CORBA::Environment &env)
{
  CORBA::Boolean continue_append = 1;
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE; // status of encode operation

  switch (tc->kind_)
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
      retval = CORBA::TypeCode::TRAVERSE_STOP;
      // we are not a primitive type
    }

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_append == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Primitive::append detected error\n"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

CORBA::TypeCode::traverse_status
TAO_Marshal_Any::append (CORBA::TypeCode_ptr,
                         TAO_InputCDR *src,
                         TAO_OutputCDR *dest,
                         CORBA::Environment &env)
{
  // Typecode of the element that makes the Any.
  CORBA::TypeCode_ptr elem_tc;

  // Status of append operation.
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;

  // Decode the typecode description for the element so that we can append the
  // data appropriately
  if ((retval = src->decode (CORBA::_tc_TypeCode,
                             &elem_tc,
                             0,
                             env))
      == CORBA::TypeCode::TRAVERSE_CONTINUE)
    {
      // encode the typecode
      retval = dest->encode (CORBA::_tc_TypeCode, &elem_tc, 0, env);
      if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          // append the data
          retval = dest->append (elem_tc, src, env);
          CORBA::release (elem_tc);
        }
    }
  if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Any::append detected error\n"));
      CORBA::release (elem_tc);
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
    }
  return retval;
}

CORBA::TypeCode::traverse_status
TAO_Marshal_TypeCode::append (CORBA::TypeCode_ptr,
                              TAO_InputCDR *src,
                              TAO_OutputCDR *dest,
                              CORBA::Environment  &env)
{
  CORBA::Boolean continue_append = 1;
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::ULong kind;

  // Decode the "kind" field of the typecode from the src for further
  // use. However, also write it back into the destination
  continue_append = (src->read_ulong (kind) ? dest->write_ulong (kind) : 0);

  if (continue_append == 1)
    {
      // Typecodes with empty parameter lists all have preallocated
      // constants.  We use those to reduce memory consumption and
      // heap access ... also, to speed things up!
      if ((kind < CORBA::TC_KIND_COUNT)
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
                retval = dest->append (CORBA::_tc_long, src, env);
              }
            break;

            // Indirected typecodes, illegal at "top level"
            case ~0u:
              {
                // read and write the negative offset
                retval = dest->append (CORBA::_tc_long, src, env);
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
                // write the encapsulation i.e., octet sequence
                retval = dest->append (TC_opaque, src, env);
              }
            } // end of switch
        }
      else // bad kind_ value to be decoded
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO_Marshal_TypeCode: "
                        "Bad kind_ value in CDR stream\n"));
          env.exception (new CORBA::BAD_TYPECODE ());
          return CORBA::TypeCode::TRAVERSE_STOP;
        }
    }

  if ((continue_append == 1) &&
      (retval == CORBA::TypeCode::TRAVERSE_CONTINUE))
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_TypeCode::append detected error\n"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// Encode Principal.

CORBA::TypeCode::traverse_status
TAO_Marshal_Principal::append (CORBA::TypeCode_ptr,
                               TAO_InputCDR *src,
                               TAO_OutputCDR *dest,
                               CORBA::Environment &env)
{
  // write the octet sequence representing the Principal
  return dest->append (TC_opaque, src, env);
}

CORBA::TypeCode::traverse_status
TAO_Marshal_ObjRef::append (CORBA::TypeCode_ptr,
                            TAO_InputCDR *src,
                            TAO_OutputCDR *dest,
                            CORBA::Environment &env)
{
  CORBA::Boolean continue_append = 1;

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

  CORBA::ULong profiles;

  // get the count of profiles that follow. This will tell us the length of the
  // sequence
  continue_append = (src->read_ulong (profiles) ?
                     dest->write_ulong (profiles) : 0);

  // No profiles means a NIL objref.
  while (profiles-- != 0 && continue_append)
    {
      CORBA::ULong tag;

      // get the profile ID tag
      if ((continue_append = (src->read_ulong (tag) ?
                              dest->write_ulong (tag) : 0))
          == 0)
        continue;

      if (tag != TAO_IOP_TAG_INTERNET_IOP)
        {
          continue_append = dest->append_string (*src);
          continue;
        }

      // OK, we've got an IIOP profile.  It's going to be
      // encapsulated ProfileData.  Create a new decoding stream and
      // context for it, and tell the "parent" stream that this data
      // isn't part of it any more.

      // ProfileData is encoded as an encapsulated sequence of octets.
      continue_append = (dest->append (TC_opaque, src, env) ==
                         CORBA::TypeCode::TRAVERSE_CONTINUE) ? 1 : 0;
    }

  if (continue_append == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "TAO_Marshal_ObjRef::append detected error\n"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// Decode structs.
CORBA::TypeCode::traverse_status
TAO_Marshal_Struct::append (CORBA::TypeCode_ptr  tc,
                            TAO_InputCDR *src,
                            TAO_OutputCDR *dest,
                            CORBA::Environment &env)
{
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::Boolean continue_append = 1;
  CORBA::TypeCode_ptr param;

  // Number of fields in the struct.
  int member_count = tc->member_count (env);

  if (env.exception () == 0)
    for (int i = 0; i < member_count
           && retval == CORBA::TypeCode::TRAVERSE_CONTINUE
           && continue_append == 1;
         i++)
      {
        // get member type
        param = tc->member_type (i, env);
        if (env.exception () == 0)
          {
            retval = dest->append (param, src, env);
          }
        else
          return CORBA::TypeCode::TRAVERSE_STOP;
      }
  else
    return CORBA::TypeCode::TRAVERSE_STOP;

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_append == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Struct::encode detected error\n"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// Encode unions.
CORBA::TypeCode::traverse_status
TAO_Marshal_Union::append (CORBA::TypeCode_ptr  tc,
                           TAO_InputCDR *src,
                           TAO_OutputCDR *dest,
                           CORBA::Environment &env)
{
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

  // get the discriminator type
  discrim_tc = tc->discriminator_type (env);

  if (env.exception () == 0)
    {
      // decode the discriminator value
      retval = src->decode (discrim_tc, &discrim_val, 0, env);
      if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          // write the discriminant back to the dest
          retval = dest->encode (discrim_tc, &discrim_val, 0, env);
          if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
            {
              // now get ready to marshal the actual union value
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

                      for (i = 0; member_count-- != 0; i++)
                        {
                          member_label = tc->member_label (i, env);
                          if (env.exception () == 0)
                            {
			                        CORBA::TypeCode_var type = member_label->type ();
                              // do the matching
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
                                    CORBA::Long l;
                                    TAO_InputCDR stream (member_label->_tao_get_cdr ());
                                    (void)stream.decode (discrim_tc, &l, 0, env);
                                    if (l == *(CORBA::Long *) &discrim_val)
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
                                  {
                                    CORBA::WChar wc;
                                    *member_label >>= CORBA::Any::to_wchar (wc);
                                    if (wc == *(CORBA::WChar *) &discrim_val)
                                      discrim_matched = 1;
                                  }
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
                                  env.exception (new CORBA::BAD_TYPECODE ());
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
                                      return dest->append (member_tc, src,
                                                           env);
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
                        {
                          return dest->append (default_tc, src, env);
                        }
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
  else
    {
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// decode string
CORBA::TypeCode::traverse_status
TAO_Marshal_String::append (CORBA::TypeCode_ptr,
                            TAO_InputCDR *src,
                            TAO_OutputCDR *dest,
                            CORBA::Environment &env)
{
  CORBA::Boolean continue_append = 1;

  // On decode, omit the check against specified string bounds, and
  // cope with illegal "zero length" strings (all lengths on the wire
  // must include a NUL).
  //
  // This is on the principle of being gracious in what we accept; we
  // don't generate messages that fail to comply with protocol specs,
  // but we will accept them when it's clear how to do so.

  continue_append = dest->append_string (*src);
  if (continue_append == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_TypeCode::append detected error"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// Decode sequence.

CORBA::TypeCode::traverse_status
TAO_Marshal_Sequence::append (CORBA::TypeCode_ptr  tc,
                              TAO_InputCDR *src,
                              TAO_OutputCDR *dest,
                              CORBA::Environment &env)
{
  CORBA::Boolean continue_append = 1;
  // Return status.
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;
  // Typecode of the element.
  CORBA::TypeCode_ptr tc2;
  // Size of element.
  CORBA::ULong bounds;

  // First unmarshal the sequence length ... we trust it to be right
  // here, on the "be gracious in what you accept" principle.  We
  // don't generate illegal sequences (i.e. length > bounds).

  continue_append = (src->read_ulong (bounds) ?
                     dest->write_ulong (bounds) : 0);

  if (continue_append)
    {
      // Get element typecode.
      tc2 = tc->content_type (env);

      if (env.exception () == 0)
        {
          // For those aggregate types whose size is
          // constant, we compute it only once.
          while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
            {
              retval = dest->append (tc2, src, env);
            }
          // CORBA::release (tc2);
          if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
            return CORBA::TypeCode::TRAVERSE_CONTINUE;
        } // no exception computing content type
    }
  // error exit
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "marshaling TAO_Marshal_Sequence::append detected error\n"));
  env.exception (new CORBA::MARSHAL ());
  return CORBA::TypeCode::TRAVERSE_STOP;
}

// Decode array.

CORBA::TypeCode::traverse_status
TAO_Marshal_Array::append (CORBA::TypeCode_ptr  tc,
                           TAO_InputCDR *src,
                           TAO_OutputCDR *dest,
                           CORBA::Environment &env)
{
  // unused:  CORBA::Boolean continue_append = 1;

  // Return status.
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;

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
          while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
            {
              retval = dest->append (tc2, src, env);
            }
          // CORBA::release (tc2);
          if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
            return CORBA::TypeCode::TRAVERSE_CONTINUE;
        } // no exception computing content type
    } // no exception computing bounds

  // error exit
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_Marshal_Sequence::append detected error"));
  env.exception (new CORBA::MARSHAL ());
  return CORBA::TypeCode::TRAVERSE_STOP;
}

// Decode alias.
CORBA::TypeCode::traverse_status
TAO_Marshal_Alias::append (CORBA::TypeCode_ptr  tc,
                           TAO_InputCDR *src,
                           TAO_OutputCDR *dest,
                           CORBA::Environment &env)
{
  // Typecode of the aliased type.
  CORBA::TypeCode_ptr tc2;
  CORBA::Boolean continue_append = 1;

  // Status of decode operation.
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;

  tc2 = tc->content_type (env);
  if (env.exception () == 0)
    {
      retval = dest->append (tc2, src, env);
    }

  //  tc2->_decr_refcnt ();
  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_append == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Alias::append detected error\n"));
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
TAO_Marshal_Except::append (CORBA::TypeCode_ptr  tc,
                            TAO_InputCDR *src,
                            TAO_OutputCDR *dest,
                            CORBA::Environment &env)
{
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::Boolean continue_append = 1;
  CORBA::TypeCode_ptr param;

  // first append the RepositoryID
  continue_append = dest->append_string (*src);

  // Number of fields in the struct.
  int member_count = tc->member_count (env);
  if (env.exception () == 0)
    {
      for (int i = 0; i < member_count
             && retval == CORBA::TypeCode::TRAVERSE_CONTINUE
             && continue_append == 1; i++)
        {
          param = tc->member_type (i, env);
          if (env.exception () == 0)
            {
              retval = dest->append (param, src, env);
            }
          else
            return CORBA::TypeCode::TRAVERSE_STOP;
        }
    }
  else
      return CORBA::TypeCode::TRAVERSE_STOP;

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_append == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Except::append detected error\n"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// decode wstring
CORBA::TypeCode::traverse_status
TAO_Marshal_WString::append (CORBA::TypeCode_ptr,
                             TAO_InputCDR *src,
                             TAO_OutputCDR *dest,
                             CORBA::Environment &env)
{
  CORBA::Boolean continue_append = 1;

  // On decode, omit the check against specified wstring bounds, and
  // cope with illegal "zero length" strings (all lengths on the wire
  // must include a NUL).
  //
  // This is on the principle of being gracious in what we accept; we
  // don't generate messages that fail to comply with protocol specs,
  // but we will accept them when it's clear how to do so.

  continue_append = dest->append_wstring (*src);

  if (continue_append == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_WString::append detected error\n"));
      env.exception (new CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}
