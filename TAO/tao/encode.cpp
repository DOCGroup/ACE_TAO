// $Id$

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
//   The original code had a single static encoder function defined on
//   the CDR class that called traverse to interpret the data types.
//   This version defines a virtual method "encode" on each class and
//   avoids calling traverse.
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
#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/Principal.h"
#include "tao/varout.h"
#include "tao/Union.h"
#include "tao/debug.h"

ACE_RCSID(tao, encode, "$Id$")

// Encode instances of arbitrary data types based only on typecode.
// "data" points to the data type; if it's not a primitve data type,
// the TypeCode interpreter is used to recursively encode its
// components.  "context" is the marshaling stream on which to encode
// the data value.

CORBA::TypeCode::traverse_status
TAO_Marshal_Primitive::encode (CORBA::TypeCode_ptr tc,
                               const void *data,
                               const void *,
                               void *context,
                               CORBA::Environment &env)
{
  CORBA::Boolean continue_encoding = 1;
  TAO_OutputCDR *stream = (TAO_OutputCDR *) context;
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE; // status of encode operation

  switch (tc->kind_)
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
      break;
    case CORBA::tk_short:
    case CORBA::tk_ushort:
      continue_encoding = stream->write_short (*(CORBA::Short *) data);
      break;
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_enum:
      continue_encoding = stream->write_long (*(CORBA::Long *) data);
      break;
    case CORBA::tk_double:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
      continue_encoding = stream->write_longlong (*(CORBA::LongLong *) data);
      break;
    case CORBA::tk_boolean:
      continue_encoding = stream->write_boolean (*(CORBA::Boolean *) data);
      break;
    case CORBA::tk_char:
    case CORBA::tk_octet:
      continue_encoding = stream->write_char (*(CORBA::Char *) data);
      break;
    case CORBA::tk_longdouble:
      continue_encoding = stream->write_longdouble (*(CORBA::LongDouble *) data);
      break;
    case CORBA::tk_wchar:
      continue_encoding = stream->write_wchar (*(CORBA::WChar *) data);
      break;
    default:
      retval = CORBA::TypeCode::TRAVERSE_STOP;
      // we are not a primitive type
    }

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_encoding == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Primitive::encode detected error\n"));
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
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

  TAO_OutputCDR *stream = (TAO_OutputCDR *) context;

  // Status of encode operation
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;

  // Typecode of the element that makes the Any.
  CORBA::TypeCode_ptr elem_tc = any->type_;

  // Encode the typecode description for the element.
  retval = stream->encode (CORBA::_tc_TypeCode, &elem_tc, 0, env);
  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
    {
      // if the any owns the data, then the value is a CDR stream and
      // we simply append the CDR stream
      // The only exception is when the TypeCode is a tk_null, them
      // both cdr_ and value_ are 0.
      if (any->cdr_ != 0)
        {
          TAO_InputCDR in_strm (any->cdr_);
          retval = stream->append (elem_tc, &in_strm, env);
        }
      else if (any->value_ != 0)
        {
          // encode the value
          retval = stream->encode (elem_tc, any->value_, 0, env);
        }
    }

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Any::encode detected error\n"));
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
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
  CORBA::Boolean continue_encoding = 1;
  TAO_OutputCDR *stream = (TAO_OutputCDR *) context;
  CORBA::TypeCode_ptr tc2;         // typecode to be encoded

  tc2 = *(CORBA::TypeCode_ptr *) data;  // the data has to be a TypeCode_ptr

  // encode the "kind" field of the typecode
  continue_encoding = stream->write_ulong ((CORBA::ULong) tc2->kind_);
  if (continue_encoding == 1)
    {
      // now encode the parameters, if any
      switch (tc2->kind_)
        {
          // Most TypeCodes have empty parameter lists
        default:
          break;

          // A few have "simple" parameter lists
        case CORBA::tk_string:
        case CORBA::tk_wstring:
          continue_encoding = stream->write_ulong (tc2->length (env));
          break;

          // Indirected typecodes can't occur at "top level" like
          // this, only nested inside others!
        case ~0u:
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "indirected typecode at top level!\n"));
          continue_encoding = 0;
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
            // simply encode the encapsulation
            continue_encoding = stream->write_ulong (tc2->length_);
            continue_encoding = continue_encoding
              && stream->write_octet_array ((CORBA::Octet*)tc2->buffer_, tc2->length_);
          }
        }
    }
  if (continue_encoding == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_TypeCode::encode detected error\n"));
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
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
  TAO_OutputCDR *stream = (TAO_OutputCDR *) context;

  CORBA::Principal_ptr p = *(CORBA::Principal_ptr *) data;

  if ((*stream << p) == 0)
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// encode obj ref
CORBA::TypeCode::traverse_status
TAO_Marshal_ObjRef::encode (CORBA::TypeCode_ptr,
                            const void *data,
                            const void *,
                            void *context,
                            CORBA::Environment &env)
{
  TAO_OutputCDR *stream = (TAO_OutputCDR *) context;

  // Current version:  objref is really an STUB_Object.
  // @@ But, it need not be.  All IIOP specific processing has
  //    been move to the specific transport profile class!
  //
  // XXX this doesn't actually verify that the stuff got written
  // OK to the "wire" ...
  // @@ Seems to break here!
  CORBA::Object_ptr obj = *(CORBA::Object_ptr *) data;

  if ((*stream << obj) == 0)
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// encode structs
CORBA::TypeCode::traverse_status
TAO_Marshal_Struct::encode (CORBA::TypeCode_ptr tc,
                            const void *data,
                            const void *,
                            void *context,
                            CORBA::Environment &env)
{
  TAO_OutputCDR *stream = (TAO_OutputCDR *) context;
  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::Boolean continue_encoding = 1;
  CORBA::TypeCode_ptr param;
  CORBA::Long size, alignment, align_offset;

  void *start_addr = (void *)data;

  if (env.exception () == 0)
    {
      int member_count = tc->member_count (env);

      for (int i = 0;
           i < member_count && retval == CORBA::TypeCode::TRAVERSE_CONTINUE
             && continue_encoding == 1;
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
                      align_offset =
                        (ptr_arith_t) ptr_align_binary (data, alignment)
                        - (ptr_arith_t) data
                        + (ptr_arith_t) ptr_align_binary (start_addr, alignment)
                        - (ptr_arith_t) start_addr;
                      // if both the start_addr and data are not aligned as per
                      // the alignment, we do not add the offset
                      data = (const void *) ((ptr_arith_t) data +
                                             ((align_offset == alignment) ?
                                              0 : align_offset));
                      switch (param->kind_)
                        {
                        case CORBA::tk_null:
                        case CORBA::tk_void:
                          break;
                        case CORBA::tk_short:
                        case CORBA::tk_ushort:
                          continue_encoding = stream->write_short (*(CORBA::Short *) data);
                          break;
                        case CORBA::tk_long:
                        case CORBA::tk_ulong:
                        case CORBA::tk_float:
                        case CORBA::tk_enum:
                          continue_encoding = stream->write_long (*(CORBA::Long *) data);
                          break;
                        case CORBA::tk_double:
                        case CORBA::tk_longlong:
                        case CORBA::tk_ulonglong:
                          continue_encoding = stream->write_longlong (*(CORBA::LongLong *) data);
                          break;
                        case CORBA::tk_boolean:
                          continue_encoding = stream->write_boolean (*(CORBA::Boolean *) data);
                          break;
                        case CORBA::tk_char:
                        case CORBA::tk_octet:
                          continue_encoding = stream->write_char (*(CORBA::Char *) data);
                          break;
                        case CORBA::tk_longdouble:
                          continue_encoding = stream->write_longdouble (*(CORBA::LongDouble *) data);
                          break;
                        case CORBA::tk_wchar:
                          continue_encoding = stream->write_wchar (*(CORBA::WChar *) data);
                          break;
                        case CORBA::tk_any:
                        case CORBA::tk_Principal:
                        case CORBA::tk_struct:
                        case CORBA::tk_union:
                        case CORBA::tk_sequence:
                        case CORBA::tk_array:
                        case CORBA::tk_alias:
                        case CORBA::tk_except:
                        case CORBA::tk_string:
                        case CORBA::tk_wstring:
                        case CORBA::tk_TypeCode:
                          retval = stream->encode (param, data, 0, env);
                          break;

                        case CORBA::tk_objref:
                          {
                            // we know that the object pointer is stored in a
                            // TAO_Object_Field_T parametrized type
                            TAO_Object_Field_T<CORBA_Object>* field =
                              ACE_reinterpret_cast (TAO_Object_Field_T<CORBA_Object> *,
                                                    ACE_const_cast (void *, data));
                            CORBA::Object_ptr ptr = field->_upcast ();
                            retval = stream->encode (param, &ptr, 0, env);
                          }
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
      && continue_encoding == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Struct::encode detected error\n"));
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
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
  TAO_OutputCDR *stream = (TAO_OutputCDR *) context;

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
      CORBA::Boolean discrim_matched = 0;

      TAO_Base_Union *base_union = (TAO_Base_Union *)data;
      void *member_val;

      // encode the discriminator value
      discrim_val = base_union->_discriminant ();
      CORBA::TypeCode::traverse_status retval =
        stream->encode (discrim_tc, discrim_val, data2, env);

      if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          discrim_size_with_pad = tc->TAO_discrim_pad_size (env);
          if (env.exception () == 0)
            {
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
                              CORBA::TypeCode_var type = member_label->type ();
                              switch (type->kind (env))
                                {
                                case CORBA::tk_short:
                                  {
                                    CORBA::Short s;
                                    *member_label >>= s;
                                    if (s == *(CORBA::Short *) discrim_val)
                                      discrim_matched = 1;
                                  }
                                  break;
                                case CORBA::tk_ushort:
                                  {
                                    CORBA::UShort s;
                                    *member_label >>= s;
                                    if (s == *(CORBA::UShort *) discrim_val)
                                      discrim_matched = 1;
                                  }
                                  break;
                                case CORBA::tk_long:
                                  {
                                    CORBA::Long l;
                                    *member_label >>= l;
                                    if (l == *(CORBA::Long *) discrim_val)
                                      discrim_matched = 1;
                                  }
                                  break;
                                case CORBA::tk_ulong:
                                  {
                                    CORBA::ULong l;
                                    *member_label >>= l;
                                    if (l == *(CORBA::ULong *) discrim_val)
                                      discrim_matched = 1;
                                  }
                                  break;
                                case CORBA::tk_enum:
                                  {
                                    CORBA::Long l;
                                    TAO_InputCDR stream ((ACE_Message_Block *)
                                                         member_label->_tao_get_cdr ());
                                    (void)stream.decode (discrim_tc, &l, 0, env);
                                    if (l == *(CORBA::Long *) discrim_val)
                                      discrim_matched = 1;
                                  }
                                  break;
                                case CORBA::tk_char:
                                  {
                                    CORBA::Char c;
                                    *member_label >>= CORBA::Any::to_char (c);
                                    if (c == *(CORBA::Char *) discrim_val)
                                      discrim_matched = 1;
                                  }
                                  break;
                                case CORBA::tk_wchar:
                                  // @@ ASG TO-DO
                                  if (*(CORBA::WChar *) member_label->value () == *(CORBA::WChar *) discrim_val)
                                    discrim_matched = 1;
                                  break;
                                case CORBA::tk_boolean:
                                  {
                                    CORBA::Boolean b;
                                    *member_label >>= CORBA::Any::to_boolean (b);
                                    if (b == *(CORBA::Boolean *) discrim_val)
                                      discrim_matched = 1;
                                  }
                                  break;

                                default:
                                  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
                                  if (TAO_debug_level > 0)
                                    ACE_DEBUG ((LM_DEBUG,
                                                "Union::encode - "
                                                "Bad discriminant type\n"));
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
                                    {
                                      member_val = base_union->_access (0);
                                      // marshal according to the matched typecode
                                      return stream->encode (member_tc, member_val,
                                                             data2, env);
                                    }
                                }
                              else // error getting member type
                                {
                                  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
                                  if (TAO_debug_level > 0)
                                    ACE_DEBUG ((LM_DEBUG,
                                                "Union::encode - error "
                                                "getting member type:%d\n",
                                                i));
                                  return CORBA::TypeCode::TRAVERSE_STOP;
                                }

                            }
                          else // error getting member label
                            {
                              env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
                              ACE_DEBUG ((LM_DEBUG,
                                          "Union::encode - error "
                                          "member label : %d\n", i));
                              return CORBA::TypeCode::TRAVERSE_STOP;
                            }
                        } // end of while
                      // we are here only if there was no match
                      if (default_tc)
                        {
                          member_val = base_union->_access (0);
                          return stream->encode (default_tc, member_val, data2, env);
                        }
                      else
                        {
                          env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
                          if (TAO_debug_level > 0)
                            ACE_DEBUG ((LM_DEBUG,
                                        "Union::encode - failed. "
                                        "No match and no default case\n"));
                          return CORBA::TypeCode::TRAVERSE_STOP;
                        }
                    }
                  else // error getting member count
                    {
                      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
                      if (TAO_debug_level > 0)
                        ACE_DEBUG ((LM_DEBUG,
                                    "Union::encode - error getting "
                                    "member count\n"));
                      return CORBA::TypeCode::TRAVERSE_STOP;
                    }
                }
              else // error getting default index
                {
                  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
                  if (TAO_debug_level > 0)
                    ACE_DEBUG ((LM_DEBUG,
                                "Union::encode - error getting "
                                "default used\n"));
                  return CORBA::TypeCode::TRAVERSE_STOP;
                }
            }
          else // error getting discrim_pad_size
            {
              env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            "Union::encode - error getting "
                            "discrim padded size\n"));
              return CORBA::TypeCode::TRAVERSE_STOP;
            }
        }
      else // error encoding discriminant
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "Union::encode - error encoding discriminant\n"));
          env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
          return CORBA::TypeCode::TRAVERSE_STOP;
        }
    }
  else // error getting the discriminant
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Union::encode - error getting "
                    "the discriminant typecode\n"));
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
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
  TAO_OutputCDR *stream = (TAO_OutputCDR *) context;
  CORBA::String str = *(CORBA::String *) data;

  // Verify string satisfies bounds requirements.  We're not so
  // permissive as to send messages violating the interface spec
  // by having excessively long strings!
  CORBA::ULong bounds = tc->length (env);
  if (str != 0)
    {
      CORBA::ULong length = ACE_OS::strlen (str);
      if (bounds == 0 || bounds >= length)
        if (stream->write_string (length, str))
          return CORBA::TypeCode::TRAVERSE_CONTINUE;
    }
  else
    {
      if (stream->write_string (0, 0))
        return CORBA::TypeCode::TRAVERSE_CONTINUE;
    }
  return CORBA::TypeCode::TRAVERSE_STOP;
}

// encode sequence
CORBA::TypeCode::traverse_status
TAO_Marshal_Sequence::encode (CORBA::TypeCode_ptr tc,
                              const void *data,
                              const void *,
                              void *context,
                              CORBA::Environment &env)
{
  CORBA::Boolean continue_encoding = 1;
  TAO_OutputCDR *stream = (TAO_OutputCDR *) context;
  TAO_Base_Sequence *seq = (TAO_Base_Sequence *)data;
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;  // return status
  CORBA::TypeCode_ptr    tc2;  // typecode of the element
  size_t  size; // size of element
  CORBA::ULong  len = seq ? seq->length_ : 0;
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
              // We just encode the current elements in the sequence
              bounds = len;
              continue_encoding = stream->write_ulong (bounds);
              if (continue_encoding && bounds != 0)
                {
                  // get element typecode
                  tc2 = tc->content_type (env);
                  if (env.exception () == 0)
                    {
                      size = tc2->size (env);
                      if (env.exception () == 0)
                        {
                          value = (char *) seq->buffer_;
                          switch (tc2->kind_)
                            {
                            case CORBA::tk_null:
                            case CORBA::tk_void:
                              return CORBA::TypeCode::TRAVERSE_CONTINUE;

                            case CORBA::tk_short:
                            case CORBA::tk_ushort:
                              continue_encoding = continue_encoding &&
                                stream->write_short_array
                                ((CORBA::Short*)value, bounds);
                              if (continue_encoding == 1)
                                return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;

                            case CORBA::tk_long:
                            case CORBA::tk_ulong:
                            case CORBA::tk_float:
                            case CORBA::tk_enum:
                              // For primitives, compute the size only once
                              continue_encoding = continue_encoding &&
                                stream->write_long_array
                                ((CORBA::Long*)value, bounds);
                              if (continue_encoding == 1)
                                return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;

                            case CORBA::tk_double:
                            case CORBA::tk_longlong:
                            case CORBA::tk_ulonglong:
                              // For primitives, compute the size only once
                              continue_encoding = continue_encoding &&
                                stream->write_longlong_array
                                ((CORBA::LongLong*)value, bounds);
                              if (continue_encoding == 1)
                                return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;

                            case CORBA::tk_boolean:
                              // For primitives, compute the size only once
                              continue_encoding = continue_encoding &&
                                stream->write_boolean_array
                                ((CORBA::Boolean*)value, bounds);
                              if (continue_encoding == 1)
                                return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;

                            case CORBA::tk_char:
                              // For primitives, compute the size only once
                              continue_encoding = continue_encoding &&
                                stream->write_char_array
                                ((CORBA::Char*)value, bounds);
                              if (continue_encoding == 1)
                                return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;

                            case CORBA::tk_octet:
#if !defined (TAO_NO_COPY_OCTET_SEQUENCES)
                              // For primitives, compute the size only once
                              continue_encoding = continue_encoding &&
                                stream->write_octet_array
                                ((CORBA::Octet*)value, bounds);
#else
                              {
                                TAO_Unbounded_Sequence<CORBA::Octet> *oseq =
                                  ACE_dynamic_cast(TAO_Unbounded_Sequence<CORBA::Octet>*,seq);
                                if (oseq->mb_ == 0)
                                  {
                                    continue_encoding = continue_encoding &&
                                      stream->write_octet_array
                                      ((CORBA::Octet*)value, bounds);
                                  }
                                else
                                  {
                                    continue_encoding = continue_encoding &&
                                      stream->write_octet_array_mb (oseq->mb ());
                                  }
                              }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES */
                              if (continue_encoding == 1)
                                return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;

                            case CORBA::tk_longdouble:
                              // For primitives, compute the size only once
                              continue_encoding = continue_encoding &&
                                stream->write_longdouble_array
                                ((CORBA::LongDouble*)value, bounds);
                              if (continue_encoding == 1)
                                return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;

                            case CORBA::tk_wchar:
                              // For primitives, compute the size only once
                              continue_encoding = continue_encoding &&
                                stream->write_wchar_array
                                ((CORBA::WChar*)value, bounds);
                              if (continue_encoding == 1)
                                return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;

                              // handle all aggregate types here

                            case CORBA::tk_any:
                            case CORBA::tk_TypeCode:
                            case CORBA::tk_Principal:
                            case CORBA::tk_struct:
                            case CORBA::tk_union:
                            case CORBA::tk_string:
                            case CORBA::tk_sequence:
                            case CORBA::tk_array:
                            case CORBA::tk_alias:
                            case CORBA::tk_except:
                            case CORBA::tk_wstring:
                              // For those aggregate types whose size
                              // is constant, we compute it only once
                              while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                                {
                                  retval = stream->encode (tc2, value, 0, env);
                                  value += size;
                                }
                              //                              CORBA::release (tc2);
                              if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                                return CORBA::TypeCode::TRAVERSE_CONTINUE;
                              break;

                            case CORBA::tk_objref:
                              {
                                size = sizeof (CORBA_Object_ptr);
                                while (bounds-- &&
                                       retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                                  {
                                    CORBA_Object_ptr ptr =
                                      seq->_upcast (value);
                                    retval = stream->encode (tc2, &ptr, 0,  env);
                                    if (env.exception () != 0) break;
                                    value += size;
                                  }
                                if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                                  return retval;
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
      continue_encoding = stream->write_ulong (len);
      if (continue_encoding == 1)
        return CORBA::TypeCode::TRAVERSE_CONTINUE;

    }
  // If an error was detected but no exception was raised then raise a
  // marshal exception.
  if (env.exception () == 0)
    env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
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
  CORBA::Boolean continue_encoding = 1;
  TAO_OutputCDR *stream = (TAO_OutputCDR *) context;
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
              switch (tc2->kind_)
                {
                case CORBA::tk_null:
                case CORBA::tk_void:
                  return CORBA::TypeCode::TRAVERSE_CONTINUE;

                case CORBA::tk_short:
                case CORBA::tk_ushort:
                  continue_encoding = continue_encoding &&
                    stream->write_short_array
                    ((CORBA::Short*)value, bounds);
                  if (continue_encoding == 1)
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;

                case CORBA::tk_long:
                case CORBA::tk_ulong:
                case CORBA::tk_float:
                  // For primitives, compute the size only once
                  continue_encoding = continue_encoding &&
                    stream->write_long_array
                    ((CORBA::Long*)value, bounds);
                  if (continue_encoding == 1)
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;

                case CORBA::tk_double:
                case CORBA::tk_longlong:
                case CORBA::tk_ulonglong:
                  // For primitives, compute the size only once
                  continue_encoding = continue_encoding &&
                    stream->write_longlong_array
                    ((CORBA::LongLong*)value, bounds);
                  if (continue_encoding == 1)
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;

                case CORBA::tk_boolean:
                  // For primitives, compute the size only once
                  continue_encoding = continue_encoding &&
                    stream->write_boolean_array
                    ((CORBA::Boolean*)value, bounds);
                  if (continue_encoding == 1)
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;

                case CORBA::tk_char:
                case CORBA::tk_octet:
                  // For primitives, compute the size only once
                  continue_encoding = continue_encoding &&
                    stream->write_octet_array
                    ((CORBA::Octet*)value, bounds);
                  if (continue_encoding == 1)
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;

                case CORBA::tk_longdouble:
                  // For primitives, compute the size only once
                  continue_encoding = continue_encoding &&
                    stream->write_longdouble_array
                    ((CORBA::LongDouble*)value, bounds);
                  if (continue_encoding == 1)
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;

                case CORBA::tk_wchar:
                  // For primitives, compute the size only once
                  continue_encoding = continue_encoding &&
                    stream->write_wchar_array
                    ((CORBA::WChar*)value, bounds);
                  if (continue_encoding == 1)
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;

                case CORBA::tk_enum:
                  // For primitives, compute the size only once
                  continue_encoding = continue_encoding &&
                    stream->write_long_array
                    ((CORBA::Long*)value, bounds);
                  if (continue_encoding == 1)
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
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO_Marshal_Sequence::encode detected error\n"));
  env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
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
  CORBA::Boolean continue_encoding = 1;
  TAO_OutputCDR *stream = (TAO_OutputCDR *) context;
  CORBA::TypeCode::traverse_status   retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE; // status of encode operation
  char *value = (char *) data;

  tc2 = tc->content_type (env);
  if (env.exception () == 0)
    {
    // switch on the data type and handle the cases for primitives here for
    // efficiency rather than calling
    switch (tc2->kind_)
      {
      case CORBA::tk_null:
      case CORBA::tk_void:
        break;
      case CORBA::tk_short:
      case CORBA::tk_ushort:
        continue_encoding = stream->write_short (*(CORBA::Short *) value);
        break;
      case CORBA::tk_long:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_enum:
        continue_encoding = stream->write_long (*(CORBA::Long *) value);
        break;
      case CORBA::tk_double:
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
        continue_encoding = stream->write_longlong (*(CORBA::LongLong *) value);
        break;
      case CORBA::tk_boolean:
        continue_encoding = stream->write_boolean (*(CORBA::Boolean *) value);
        break;
      case CORBA::tk_char:
      case CORBA::tk_octet:
        continue_encoding = stream->write_char (*(CORBA::Char *) value);
        break;
      case CORBA::tk_longdouble:
        continue_encoding = stream->write_longdouble (*(CORBA::LongDouble *) value);
        break;
      case CORBA::tk_wchar:
        continue_encoding = stream->write_wchar (*(CORBA::WChar *) value);
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
        retval = stream->encode (tc2, data, 0, env);
        break;
      default:
        // anything else is an error
        retval = CORBA::TypeCode::TRAVERSE_STOP;
      }
  }
  //  tc2->_decr_refcnt ();
  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_encoding == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Alias::encode detected error\n"));
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
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
  CORBA::Boolean continue_encoding = 1;
  TAO_OutputCDR *stream = (TAO_OutputCDR *) context;

  if (env.exception () == 0)
    {
      CORBA::TypeCode_ptr param;
      CORBA::Long size, alignment;

      // first encode the RepositoryID which we can grab from the
      // typecode pointer
      continue_encoding = stream->write_string (tc->id (env));

      data = (char *) data + sizeof (CORBA::Exception);
      // @@ (ASG) The reason this is done is because we want to skip the size
      // of the the base class and its private data members (type_ and
      // refcount_). After skipping these data members, we will have the data
      // members of the derived class which must be encoded.

      int member_count = tc->member_count (env);

      for (int i = 0;
           i < member_count && retval == CORBA::TypeCode::TRAVERSE_CONTINUE
             && continue_encoding == 1;
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
                      switch (param->kind_){
                      case CORBA::tk_null:
                      case CORBA::tk_void:
                        break;
                      case CORBA::tk_short:
                      case CORBA::tk_ushort:
                        continue_encoding = stream->write_short (*(CORBA::Short *) data);
                        break;
                      case CORBA::tk_long:
                      case CORBA::tk_ulong:
                      case CORBA::tk_float:
                      case CORBA::tk_enum:
                        continue_encoding = stream->write_long (*(CORBA::Long *) data);
                        break;
                      case CORBA::tk_double:
                      case CORBA::tk_longlong:
                      case CORBA::tk_ulonglong:
                        continue_encoding = stream->write_longlong (*(CORBA::LongLong *) data);
                        break;
                      case CORBA::tk_boolean:
                        continue_encoding = stream->write_boolean (*(CORBA::Boolean *) data);
                        break;
                      case CORBA::tk_char:
                      case CORBA::tk_octet:
                        continue_encoding = stream->write_char (*(CORBA::Char *) data);
                        break;
                      case CORBA::tk_longdouble:
                        continue_encoding = stream->write_longdouble (*(CORBA::LongDouble *) data);
                        break;
                      case CORBA::tk_wchar:
                        continue_encoding = stream->write_wchar (*(CORBA::WChar *) data);
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
      && continue_encoding == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Except::encode detected error\n"));
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
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
  CORBA::WChar *str = *(CORBA::WChar **) data;
  TAO_OutputCDR *stream = (TAO_OutputCDR *) context;

  // Verify string satisfies bounds requirements.  We're not so
  // permissive as to send messages violating the interface spec
  // by having excessively long strings!
  CORBA::ULong bounds = tc->length (env);

  if (str != 0)
    {
      // get the actual length of the string
      CORBA::ULong len = ACE_OS::wslen ((CORBA::WChar *) str);

      // if it is an unbounded string or if the length is less than the
      // bounds for an unbounded string
      if (bounds == 0 || len <= bounds)
        if (stream->write_wstring (len, str))
          return CORBA::TypeCode::TRAVERSE_CONTINUE;
    }
  else
    {
      if (stream->write_wstring (0, 0))
        return CORBA::TypeCode::TRAVERSE_CONTINUE;
    }
  return CORBA::TypeCode::TRAVERSE_STOP;
}
