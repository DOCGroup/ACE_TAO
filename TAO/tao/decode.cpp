// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    decode.cpp
//
// = DESCRIPTION
//   Code for decoding different data types
//
//   The original code had a single static decoder function defined on
//   the CDR class that called traverse to interpret the data
//   types. This version defines a virtual method "decode" on each
//   class and avoids calling traverse.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//     and
//     Aniruddha Gokhale
//
// ============================================================================

#include "tao/Marshal.h"
#include "tao/CDR.h"
#include "tao/Environment.h"
#include "tao/GIOP.h"
#include "tao/Any.h"
#include "tao/Principal.h"
#include "tao/MProfile.h"
#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/varout.h"
#include "tao/ORB.h"
#include "tao/Union.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

ACE_RCSID(tao, decode, "$Id$")

// The decoder is exactly the reverse of the encoder, except that:
//
//      * Unmarshaling some data types involve allocating memory.  Such
//        types include sequences (the buffer), objrefs, Principals, Anys,
//        TypeCodes, and strings.
//
//      * The decoder is used when retrieving typecode parameters from
//        encapsulations.  This means it must deal with "CORBA::tk_indirect",
//        the magic value (~0u) signifying typecode indirection.
//
// This second case is identified by a bit of a hack: the second
// "data" value is used to hold the parent typecode, rather than being
// ignored.  This means that all other invocations of decoder () **
// MUST ** pass zero for the second data parameter, in case they
// decode a TypeCode.  If they didn't, this case might be signified
// inappropriately.
//
// XXX desirable to have a less hacky solution to that ... pull that
// code out into a separate routine called both by CDR::decoder () and
// by the code retrieving typecode parameters from encapsulations.

CORBA::TypeCode::traverse_status
TAO_Marshal_Primitive::decode (CORBA::TypeCode_ptr  tc,
                               const void *data,
                               const void *,
                               void *context,
                               CORBA::Environment &env)
{
  CORBA::Boolean continue_decoding = 1;
  TAO_InputCDR *stream = (TAO_InputCDR *) context;
  CORBA::TypeCode::traverse_status   retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE; // status of encode operation

  switch (tc->kind_)
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
      break;
    case CORBA::tk_short:
    case CORBA::tk_ushort:
      continue_decoding = stream->read_short (*(CORBA::Short *) data);
      break;
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_enum:
      continue_decoding = stream->read_long (*(CORBA::Long *) data);
      break;
    case CORBA::tk_double:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
      continue_decoding = stream->read_longlong (*(CORBA::LongLong *) data);
      break;
    case CORBA::tk_boolean:
      continue_decoding = stream->read_boolean (*(CORBA::Boolean *) data);
      break;
    case CORBA::tk_char:
    case CORBA::tk_octet:
      continue_decoding = stream->read_char (*(CORBA::Char *) data);
      break;
    case CORBA::tk_longdouble:
      continue_decoding = stream->read_longdouble (*(CORBA::LongDouble *) data);
      break;
    case CORBA::tk_wchar:
      continue_decoding = stream->read_wchar (*(CORBA::WChar *) data);
      break;
    default:
      retval = CORBA::TypeCode::TRAVERSE_STOP;
      // we are not a primitive type
    }
  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_decoding == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Primitive::decode detected error\n"));
      TAO_THROW_ENV_RETURN (CORBA::MARSHAL (CORBA::COMPLETED_MAYBE), env,
                            CORBA::TypeCode::TRAVERSE_STOP);
    }
}

CORBA::TypeCode::traverse_status
TAO_Marshal_Any::decode (CORBA::TypeCode_ptr,
                         const void *data,
                         const void *,
                         void *context,
                         CORBA::Environment &env)
{
  CORBA::Any *any = (CORBA::Any *) data;

  // Typecode of the element that makes the Any.
  CORBA::TypeCode_var elem_tc;

  // Context is the CDR stream.
  TAO_InputCDR *stream = (TAO_InputCDR *) context;

  // Status of encode operation.
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;

  // Decode the typecode description for the element.
  retval = stream->decode (CORBA::_tc_TypeCode,
                           &elem_tc.out (),
                           0,
                           env);
  TAO_CHECK_ENV_RETURN (env, retval);
  if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
    return retval;

  // Let the Any maintain a pointer to the CDR stream
  // @@ ASG + CORYAN - The following commented line would have been a great
  // optimization. However, it turns out that although the Message_Block is
  // heap-allocated, the actual buffer i.e., data block is allocated on the
  // function call stack. Once we are out of these chain of functions and
  // return into the stub, we have lost the activation record for the
  // actual buffer. Hence it makes no sense keeping pointers to stack
  // memory.
  //
  // See STUB_Object.cpp::do_static_call in which a GIOP_Invocation is
  // allocated on stack
#if 0
  any->cdr_ = ACE_Message_Block::duplicate ((ACE_Message_Block *)
                                            stream->start ());
#endif
  // one solution is to heap allocate the GIOP_Invocation. However, that
  // would be bad since not all requests will use Anys.
  //
  // One solution is to allocate a new Message_Block with its own heap
  // allocated data_block. (We may optimize this using allocators for known
  // sizes). We allocate a Message_Block of the size that is required by
  // the data type held by the Any. To find what is the size of this data
  // in the CDR, we traverse the CDR by skipping past this data type. We
  // then get an offset using the "begin" and "end" shown below that tells
  // us the size. The skipping is done on a temporary CDR stream and not on
  // the actual incoming CDR stream. Once we have allocated a new
  // Message_Block, we simply append the data into it from the original CDR
  // stream.
  char *begin, *end;
  TAO_InputCDR temp (*stream);

  begin = stream->rd_ptr ();
  retval = temp.skip (elem_tc.in (), env);
  TAO_CHECK_ENV_RETURN (env, retval);

  if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
    return retval;

  end = temp.rd_ptr ();

  ACE_Message_Block* cdr;

  // We need to allocate more memory than in the original
  // stream, first to guarantee that the buffer is aligned in
  // memory and next because the realignment may introduce
  // extra padding. 2*MAX_ALIGNMENT should be enough.
  // @@EXC@@ This doesn't seem to be exception safe.
  ACE_NEW_RETURN (cdr,
                  ACE_Message_Block (end - begin
                                     + 2*CDR::MAX_ALIGNMENT),
                  CORBA::TypeCode::TRAVERSE_STOP);
  // Align the buffer before creating the CDR stream.
  CDR::mb_align (cdr);
  TAO_OutputCDR out (cdr);

  retval = out.append (elem_tc.in (), stream, env);
  TAO_CHECK_ENV_RETURN (env, retval);
  if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
    return retval;

  ACE_Message_Block::release (any->cdr_);
  if (any->any_owns_data_ && any->value_ != 0)
    DEEP_FREE (any->type_, any->value_, 0, env);
  TAO_CHECK_ENV_RETURN (env, CORBA::TypeCode::TRAVERSE_STOP);

  any->cdr_ = cdr;
  any->value_ = 0;
  any->type_ = elem_tc._retn ();
  any->any_owns_data_ = 1;

  if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Any::decode detected error\n"));
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
    }
  return retval;
}

CORBA::TypeCode::traverse_status
TAO_Marshal_TypeCode::decode (CORBA::TypeCode_ptr,
                              const void *data,
                              const void *parent_typecode,
                              void *context,
                              CORBA::Environment  &env)
{
  CORBA::Boolean continue_decoding = 1;

  // Context is the CDR stream.
  TAO_InputCDR *stream = (TAO_InputCDR *) context;

  // Typecode to be decoded.
  CORBA::TypeCode_ptr *tcp;

  // Typecode kind.
  CORBA::ULong kind;

  static CORBA::TypeCode_ptr tc_consts [CORBA::TC_KIND_COUNT] =
  {
    CORBA::_tc_null,
    CORBA::_tc_void,
    CORBA::_tc_short,
    CORBA::_tc_long,
    CORBA::_tc_ushort,

    CORBA::_tc_ulong,
    CORBA::_tc_float,
    CORBA::_tc_double,
    CORBA::_tc_boolean,
    CORBA::_tc_char,

    CORBA::_tc_octet,
    CORBA::_tc_any,
    CORBA::_tc_TypeCode,
    CORBA::_tc_Principal,

    0, // CORBA::_tc_Object ... type ID is CORBA_Object
    0, // CORBA_tk_struct
    0, // CORBA_tk_union
    0, // CORBA_tk_enum
    0, // CORBA::_tc_string ... unbounded
    0, // CORBA_tk_sequence
    0, // CORBA_tk_array
    0, // CORBA_tk_alias
    0, // CORBA_tk_except

    CORBA::_tc_longlong,
    CORBA::_tc_ulonglong,
    CORBA::_tc_longdouble,
    CORBA::_tc_wchar,
    0           // CORBA::_tc_wstring ... unbounded
  };

  // TypeCode for the parent. The most likely situation when a parent will be
  // provided is when we are precomputing the private state of an IDL compiler
  // generated or an ORB owned TypeCode, OR we are decoding an indirected
  // TypeCode. In such circumstances, the decoded
  // TypeCode will share resources with its parent and cannot be freed until
  // its parent is being freed.
  CORBA::TypeCode_ptr parent = (CORBA::TypeCode_ptr) parent_typecode;

  // Decode the "kind" field of the typecode from the stream
  continue_decoding = stream->read_ulong (kind);

  if (continue_decoding == 1)
    {
      // The data has to be a TypeCode_ptr *.
      tcp = (CORBA::TypeCode_ptr *) data;

      // Typecodes with empty parameter lists all have preallocated
      // constants.  We use those to reduce memory consumption and
      // heap access ... also, to speed things up!
      if (kind < CORBA::TC_KIND_COUNT
          && (*tcp = tc_consts [(u_int) kind]) != 0)
        // parent is ignored
        *tcp = CORBA::TypeCode::_duplicate (tc_consts [(u_int) kind]);
      else if (kind == ~0u || kind < CORBA::TC_KIND_COUNT)
        {
          // Either a non-constant typecode or an indirected typecode.
          switch (kind)
            {
              // Need special handling for all kinds of typecodes that
              // have nonempty parameter lists ...
            default:
              // Error: missed a case!
              env.exception (new CORBA::INTERNAL (CORBA::COMPLETED_MAYBE));
              return CORBA::TypeCode::TRAVERSE_STOP;

              // Some have "simple" parameter lists ... some of these
              // also have preallocated constants that could be used.
            case CORBA::tk_string:
            case CORBA::tk_wstring:
              {
                CORBA::ULong bound;

                continue_decoding = stream->read_ulong (bound);
                if (continue_decoding)
                  {
                    if (bound == 0)
                      {
                        // unbounded string. Let us reuse the ORB owned
                        // _tc_string or _tc_wstring
                        if (kind == CORBA::tk_string)
                          *tcp = CORBA::TypeCode::_duplicate
                            (CORBA::_tc_string);
                        else
                          *tcp = CORBA::TypeCode::_duplicate
                            (CORBA::_tc_wstring);
                      }
                    else
                      {
                        // bounded string. Create a TypeCode. If it is does not
                        // have a parent, then the application must free it.

                        // allocate a new TypeCode
#if 1
                        // This may produce a memory leak, because
                        // callers are sloppy about removing this
                        // objects.
                        CORBA::Long _oc_bounded_string [] =
                        {TAO_ENCAP_BYTE_ORDER, 0};
                        // Bounded string. Save the bounds
                        _oc_bounded_string [1] = (CORBA::Long) bound;
                        *tcp = new CORBA::TypeCode (ACE_static_cast(CORBA::TCKind, kind),
                                                    8,
                                                    ACE_reinterpret_cast(char*,_oc_bounded_string),
                                                    0, sizeof
                                                    (CORBA::String_var), 0);
#elif 0
                        // This one fails because we are passing the
                        // parent but the buffer (_oc_bounded_string) is
                        // not pointing to the parent CDR stream
                        // (hence no sharing) and the length is wrong
                        // (should be 8 not bounds).
                        CORBA::Long _oc_bounded_string [] =
                        {TAO_ENCAP_BYTE_ORDER, 0};
                        // Bounded string. Save the bounds
                        _oc_bounded_string [1] = (CORBA::Long) bound;
                        *tcp = new CORBA::TypeCode ((CORBA::TCKind) kind,
                                                    bound, (char *) &_oc_bounded_string,
                                                    0, sizeof
                                                    (CORBA::String_var), parent);
#else
                        // This depends on the fact that <stream> is
                        // actually pointing to the parent CDR stream,
                        // it is untested.
                        *tcp = new CORBA::TypeCode ((CORBA::TCKind) kind,
                                                    8,
                                                    stream->rd_ptr () - 8,
                                                    0, 0, parent);
#endif
                      }
                  }
              }
            break;

            // Indirected typecodes, illegal at "top level" but we
            // allow unmarshaling of them here because we use the same
            // code to read "off the wire" (where they're illegal) and
            // to read out of an encapsulation stream.  We distinguish
            // the case where this is legal as described above.
            case ~0u:
              {
                if (parent_typecode == 0)
                  {
                    env.exception (new CORBA::INTERNAL (CORBA::COMPLETED_MAYBE));
                    return CORBA::TypeCode::TRAVERSE_STOP;
                  }

                // Get the long indicating the encapsulation offset,
                // then set up indirection stream that's like "stream"
                // but has space enough only for the typecode and the
                // length for the encapsulated parameters.
                //
                // The offset must be negative
                CORBA::Long offset;

                continue_decoding = stream->read_long (offset);
                if (continue_decoding)
                  {
                    // Since indirected typecodes cannot occur at the
                    // topmost level, they can occur starting only at the
                    // second and subsequent levels. This means that a
                    // normal encoding of that typecode occurred somewhere
                    // before in the stream. As a result the offset field
                    // must always be negative. See the CORBA spec for details.
                    continue_decoding = (offset < 0);
                  }

                // Slava Galperin <galperin@teknowledge.com> clarifies
                // this:
                // CORBA Spec says:
                //
                // The encoding of such an indirection is as a
                // TypeCode with a TCKind value that has the special
                // value 2^32 -1 (0xffffffff, all ones). Such
                // typecodes have a single (simple) parameter, which
                // is the long offset (in units of octets) from the
                // simple parameter. (This means that an offset of
                // negative four (-4) is illegal because it will be
                // self-indirecting.)
                // (CORBA V2.2 CDR Transfer Syntax February 1998 page 13-17)
                //
                // This apparently assumes offset from the <em>
                // beginning </em> of the simple parameter.
                // [Right, because otherwise the value -8 would be
                // illegal]
                // Because at this point stream is positioned after
                // the parameter, we need to account for that when
                // constructing indir_stream by subtracting 4 (length
                // of the offset parameter itself).

                TAO_InputCDR indir_stream (*stream, 8, offset - 4);

                continue_decoding = indir_stream.good_bit ();

                // Get "kind" and length of target typecode
                //
                // XXX this currently assumes the TCKind to which we
                // indirect is the same byte order as the "parent"
                // typecode -- not the right assumption; see how the
                // TypeCode interpreter does it.

                CORBA::ULong indir_kind;
                CORBA::ULong indir_len;

                // retrieve the typecode kind
                if (continue_decoding)
                  continue_decoding = indir_stream.read_ulong (indir_kind);

                if (continue_decoding
                    && indir_kind >= CORBA::TC_KIND_COUNT)
                  continue_decoding = 0;

                // now retrieve the encapsulation length
                if (continue_decoding)
                  continue_decoding = indir_stream.read_ulong (indir_len);

                // Now construct indirected typecode.  This shares the
                // typecode octets with the "parent" typecode,
                // increasing the amount of memory sharing and
                // reducing the cost of getting typecodes.
                if (continue_decoding)
                  {
                    *tcp = new CORBA::TypeCode ((CORBA::TCKind) indir_kind,
                                                indir_len,
                                                indir_stream.rd_ptr(),
                                                0,
                                                0,
                                                parent);
                  }
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
                continue_decoding = stream->read_ulong (length);
                if (!continue_decoding)
                  break;

                // if length > MAXUNSIGNED, error ...
                u_int len = (u_int) length;

                // create a new typecode
                *tcp = new CORBA::TypeCode ((CORBA::TCKind) kind,
                                            len,
                                            stream->rd_ptr (),
                                            0,
                                            0,
                                            parent);
                // skip length number of bytes in the stream, else we may
                // leave the stream in an undefined state
                (void) stream->skip_bytes (length);
              }
            } // end of switch
        }
      else // bad kind_ value to be decoded
        {
          env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO_Marshal_TypeCode:decode: "
                        "Bad kind_ value in CDR stream\n"));
          return CORBA::TypeCode::TRAVERSE_STOP;
        }
    }

  if (continue_decoding == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_TypeCode::decode detected error\n"));
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// Encode Principal.

CORBA::TypeCode::traverse_status
TAO_Marshal_Principal::decode (CORBA::TypeCode_ptr,
                               const void *data,
                               const void *,
                               void *context,
                               CORBA::Environment &env)
{
  // Context is the CDR stream.
  TAO_InputCDR *stream = (TAO_InputCDR *) context;

  CORBA::Principal_ptr x;

  if ((*stream >> x) == 0)
    {
      *(CORBA_Principal_ptr*)data = CORBA::Principal::_nil ();
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }

  *(CORBA_Principal **)data = x;
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// Decode obj ref.  An IOR
CORBA::TypeCode::traverse_status
TAO_Marshal_ObjRef::decode (CORBA::TypeCode_ptr,
                            const void *data, // where the result will go
                            const void *,
                            void *context,
                            CORBA::Environment &env)
{
  // Context is the CDR stream.
  TAO_InputCDR *stream = (TAO_InputCDR *) context;

  CORBA::Object_ptr object;

  if ((*stream >> object) == 0)
    {
      *(CORBA_Object_ptr*)data = CORBA::Object::_nil ();
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }

  *(CORBA_Object **)data = object;
  return CORBA::TypeCode::TRAVERSE_CONTINUE;
}

// Decode structs.
CORBA::TypeCode::traverse_status
TAO_Marshal_Struct::decode (CORBA::TypeCode_ptr  tc,
                            const void *data,
                            const void *,
                            void *context,
                            CORBA::Environment &env)
{
  TAO_InputCDR *stream = (TAO_InputCDR *) context;
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::Boolean continue_decoding = 1;
  CORBA::TypeCode_ptr param;
  CORBA::Long size, alignment, align_offset;

  void *start_addr = (void *)data;

  // Number of fields in the struct.
  int member_count = tc->member_count (env);

  if (env.exception () == 0)
    for (int i = 0; i < member_count
           && retval == CORBA::TypeCode::TRAVERSE_CONTINUE
           && continue_decoding == 1;
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
                        continue_decoding =
                          stream->read_short (*(CORBA::Short *) data);
                        break;
                      case CORBA::tk_long:
                      case CORBA::tk_ulong:
                      case CORBA::tk_float:
                      case CORBA::tk_enum:
                        continue_decoding =
                          stream->read_long (*(CORBA::Long *) data);
                        break;
                      case CORBA::tk_double:
                      case CORBA::tk_longlong:
                      case CORBA::tk_ulonglong:
                        continue_decoding =
                          stream->read_longlong (*(CORBA::LongLong *) data);
                        break;
                      case CORBA::tk_boolean:
                        continue_decoding =
                          stream->read_boolean (*(CORBA::Boolean *) data);
                        break;
                      case CORBA::tk_char:
                      case CORBA::tk_octet:
                        continue_decoding =
                          stream->read_char (*(CORBA::Char *) data);
                        break;
                      case CORBA::tk_longdouble:
                        continue_decoding =
                          stream->read_longdouble (*(CORBA::LongDouble *) data);
                        break;
                      case CORBA::tk_wchar:
                        continue_decoding =
                          stream->read_wchar (*(CORBA::WChar *) data);
                        break;
                      case CORBA::tk_TypeCode:
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
                        retval = stream->decode (param, data, 0, env);
                        break;

                      case CORBA::tk_objref:
                        {
                          CORBA_Object_ptr object;
                          retval = stream->decode (param, &object, 0, env);
                          if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
                              && env.exception () == 0)
                            {
                              // The representation of a base
                              // CORBA::Object is a little different.
                              // @@ TODO maybe equivalent() is the right
                              // method here.
                              CORBA::Boolean is_corba_object =
                                param->equal (CORBA::_tc_Object, env);
                              if (env.exception () == 0)
                                {
                                  if (is_corba_object == 0)
                                    {
                                      TAO_Object_Field* field =
                                        ACE_reinterpret_cast (TAO_Object_Field *,
                                                              ACE_const_cast (void *, data));
                                      field->_downcast (object, env);
                                      // The size of this field is different...
                                      size = sizeof(TAO_Object_Field_T<CORBA_Object>);
                                    }
                                  else
                                    {
                                      CORBA_Object_ptr* tmp =
                                        ACE_reinterpret_cast(CORBA_Object_ptr*,
                                                             ACE_const_cast(void*,data));
                                      *tmp = object;
                                    }
                                }
                              else
                                {
                                  retval = CORBA::TypeCode::TRAVERSE_STOP;
                                }
                            }
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
  else
    return CORBA::TypeCode::TRAVERSE_STOP;

  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_decoding == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Struct::decode detected error\n"));
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// Encode unions.
CORBA::TypeCode::traverse_status
TAO_Marshal_Union::decode (CORBA::TypeCode_ptr  tc,
                           const void *data,
                           const void *data2,
                           void *context,
                           CORBA::Environment &env)
{
  // Context is the CDR stream.
  TAO_InputCDR *stream = (TAO_InputCDR *) context;

  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;

  CORBA::TypeCode_ptr discrim_tc;
  CORBA::TypeCode_ptr member_tc;
  CORBA::Any_ptr member_label;
  CORBA::ULong discrim_size_with_pad;
  const void *discrim_val;
  CORBA::ULong member_count;
  CORBA::Long  default_index;
  CORBA::ULong i;
  CORBA::TypeCode_ptr default_tc = 0;
  CORBA::Boolean discrim_matched = 0;
  TAO_Base_Union *base_union = (TAO_Base_Union *)data;
  void *member_val;

  discrim_tc = tc->discriminator_type (env);
  // get the discriminator type

  if (env.exception () == 0)
    {
      // decode the discriminator value
      discrim_val = base_union->_discriminant ();
      retval = stream->decode (discrim_tc, discrim_val, data2, env);
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
                                      member_val = base_union->_access (1);
                                      // marshal according to the matched typecode
                                      return stream->decode (member_tc, member_val,
                                                           data2, env);
                                    }
                                }
                              else
                                {
                                  env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
                                  return CORBA::TypeCode::TRAVERSE_STOP;
                                }
                            }
                          else
                            {
                              env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
                              return CORBA::TypeCode::TRAVERSE_STOP;
                            }
                        } // end of for loop
                      // we are here only if there was no match
                      if (default_tc)
                        {
                          member_val = base_union->_access (1);
                          return stream->decode (default_tc, member_val, data2, env);
                        }
                      else
                        return CORBA::TypeCode::TRAVERSE_CONTINUE;
                    }
                  else
                    {
                      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
                      return CORBA::TypeCode::TRAVERSE_STOP;
                    }
                }
              else
                {
                  env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
                  return CORBA::TypeCode::TRAVERSE_STOP;
                }
            }
          else
            {
              env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
              return CORBA::TypeCode::TRAVERSE_STOP;
            }
        }
      else
        {
          env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
          return CORBA::TypeCode::TRAVERSE_STOP;
        }
    }
  else
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// decode string
CORBA::TypeCode::traverse_status
TAO_Marshal_String::decode (CORBA::TypeCode_ptr,
                            const void *data,
                            const void *,
                            void *context,
                            CORBA::Environment &env)
{
  CORBA::Boolean continue_decoding = 1;
  // Context is the CDR stream.
  TAO_InputCDR *stream = (TAO_InputCDR *) context;

  CORBA::String* str_ptr = (CORBA::String*)data;

  // On decode, omit the check against specified string bounds, and
  // cope with illegal "zero length" strings (all lengths on the wire
  // must include a NUL).
  //
  // This is on the principle of being gracious in what we accept; we
  // don't generate messages that fail to comply with protocol specs,
  // but we will accept them when it's clear how to do so.

  continue_decoding = stream->read_string (*str_ptr);
  if (continue_decoding == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_String::decode detected error\n"));
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// Decode sequence.

CORBA::TypeCode::traverse_status
TAO_Marshal_Sequence::decode (CORBA::TypeCode_ptr  tc,
                              const void *data,
                              const void *,
                              void *context,
                              CORBA::Environment &env)
{
  CORBA::Boolean continue_decoding = 1;
  TAO_InputCDR *stream = (TAO_InputCDR *) context;
  TAO_Base_Sequence *seq = (TAO_Base_Sequence *)data;
  // Return status.
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;
  // Typecode of the element.
  CORBA::TypeCode_ptr tc2;
  // Size of element.
  size_t size;
  CORBA::ULong bounds;
  char *value;

  // First unmarshal the sequence length ... we trust it to be right
  // here, on the "be gracious in what you accept" principle.  We
  // don't generate illegal sequences (i.e. length > bounds).

  continue_decoding = stream->read_ulong (bounds);

  if (continue_decoding)
    {
      // No point decoding an empty sequence.
      if (bounds > 0)
        {
          // Get element typecode.
          tc2 = tc->content_type (env);

          if (env.exception () == 0)
            {
              size = tc2->size (env);

              if (env.exception () == 0)
                {
#if defined (TAO_NO_COPY_OCTET_SEQUENCES)
                  // The treatment of octet sequences is completely
                  // different.
                  if (tc2->kind_ == CORBA::tk_octet
                      && ACE_BIT_DISABLED (stream->start ()->flags (),
                                           ACE_Message_Block::DONT_DELETE))
                    {
                      TAO_Unbounded_Sequence<CORBA::Octet>* seq2 =
                        ACE_dynamic_cast(TAO_Unbounded_Sequence<CORBA::Octet>*, seq);
                      seq2->replace (bounds, stream->start ());
                      seq2->mb ()->wr_ptr (seq2->mb ()->rd_ptr () + bounds);
                      stream->skip_bytes (bounds);
                      return CORBA::TypeCode::TRAVERSE_CONTINUE;
                    }
#endif /* defined (TAO_NO_COPY_OCTET_SEQUENCES) */

                  // Allocate the buffer using the virtual
                  // _allocate_buffer method, hence the right
                  // constructors are invoked and size for the array
                  // is OK.  The sequence will release it, since its
                  // release_ field is 1.
                  if (seq->maximum_ < bounds)
                    {
                      seq->_deallocate_buffer ();
                      seq->maximum_ = bounds;
                      seq->release_ = 1;
                      seq->buffer_ = 0;
                      seq->_allocate_buffer (bounds);
                    }
                  // In any case the sequence length is changed.
                  seq->length_ = bounds;


                  value = (char *) seq->buffer_;

                  switch (tc2->kind_)
                    {
                    case CORBA::tk_null:
                    case CORBA::tk_void:
                      return CORBA::TypeCode::TRAVERSE_CONTINUE;

                    case CORBA::tk_short:
                    case CORBA::tk_ushort:
                      // For primitives, compute the size only once
                      continue_decoding = continue_decoding &&
                        stream->read_short_array
                        ((CORBA::Short *) value, bounds);
                      if (continue_decoding == 1)
                        return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;

                    case CORBA::tk_long:
                    case CORBA::tk_ulong:
                    case CORBA::tk_float:
                    case CORBA::tk_enum:
                      // For primitives, compute the size only once
                      continue_decoding = continue_decoding &&
                        stream->read_long_array
                        ((CORBA::Long *) value, bounds);
                      if (continue_decoding == 1)
                        return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;

                    case CORBA::tk_double:
                    case CORBA::tk_longlong:
                    case CORBA::tk_ulonglong:
                      // For primitives, compute the size only once
                      continue_decoding = continue_decoding &&
                        stream->read_longlong_array
                        ((CORBA::LongLong *) value, bounds);
                      if (continue_decoding == 1)
                        return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;

                    case CORBA::tk_boolean:
                      // For primitives, compute the size only once
                      continue_decoding = continue_decoding &&
                        stream->read_octet_array
                        ((CORBA::Octet *) value, bounds);
                      if (continue_decoding == 1)
                        return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;

                    case CORBA::tk_char:
                      // For primitives, compute the size only once
                      continue_decoding = continue_decoding &&
                        stream->read_char_array
                        ((CORBA::Char *) value, bounds);
                      if (continue_decoding == 1)
                        return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;

                    case CORBA::tk_octet:
                      // For primitives, compute the size only once
                      continue_decoding = continue_decoding &&
                        stream->read_octet_array
                        ((CORBA::Octet *) value, bounds);
                      if (continue_decoding == 1)
                        return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;

                    case CORBA::tk_longdouble:
                      // For primitives, compute the size only once
                      continue_decoding = continue_decoding &&
                        stream->read_longdouble_array
                        ((CORBA::LongDouble *) value, bounds);
                      if (continue_decoding == 1)
                        return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;

                    case CORBA::tk_wchar:
                      // For primitives, compute the size only once
                      continue_decoding = continue_decoding &&
                        stream->read_wchar_array
                        ((CORBA::WChar *) value, bounds);
                      if (continue_decoding == 1)
                        return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;

                      // handle all aggregate types here
                    case CORBA::tk_string:
                    case CORBA::tk_wstring:
                    case CORBA::tk_any:
                    case CORBA::tk_TypeCode:
                    case CORBA::tk_Principal:
                    case CORBA::tk_struct:
                    case CORBA::tk_union:
                    case CORBA::tk_sequence:
                    case CORBA::tk_array:
                    case CORBA::tk_alias:
                    case CORBA::tk_except:
                      // For those aggregate types whose size is
                      // constant, we compute it only once.
                      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = stream->decode (tc2, value, 0, env);
                          value += size;
                        }
                      // CORBA::release (tc2);
                      if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                        return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;

                    case CORBA::tk_objref:
                      {
                        size = sizeof (CORBA_Object_ptr);
                        while (bounds-- &&
                               retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                          {
                            CORBA_Object_ptr ptr;
                            retval = stream->decode (tc2, &ptr, 0,  env);
                            if (env.exception () != 0) break;
                            seq->_downcast (value, ptr, env);
                            if (env.exception () != 0) break;
                            CORBA::release (ptr);
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
        } // length is > 0
      else
        return CORBA::TypeCode::TRAVERSE_CONTINUE;
    }
  // If an error was detected but no exception was raised then raise a
  // marshal exception.
  if (env.exception () == 0)
    env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
  return CORBA::TypeCode::TRAVERSE_STOP;
}

// Decode array.

CORBA::TypeCode::traverse_status
TAO_Marshal_Array::decode (CORBA::TypeCode_ptr  tc,
                           const void *data,
                           const void *,
                           void *context,
                           CORBA::Environment &env)
{
  CORBA::Boolean continue_decoding = 1;
  TAO_InputCDR *stream = (TAO_InputCDR *) context;

  // Return status.
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;

  // Typecode of the element.
  CORBA::TypeCode_ptr tc2;

  // Size of element.
  size_t  size;
  CORBA::ULong  bounds;
  char *value = (char *) data;

  // retrieve the bounds of the array
  bounds = tc->length (env);
  if (env.exception () == 0)
    {

      // get element typecode
      tc2 = tc->content_type (env);
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
                  // For primitives, compute the size only once
                  continue_decoding = continue_decoding &&
                    stream->read_short_array
                    ((CORBA::Short *) value, bounds);
                  if (continue_decoding == 1)
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;

                case CORBA::tk_long:
                case CORBA::tk_ulong:
                case CORBA::tk_float:
                case CORBA::tk_enum:
                  // For primitives, compute the size only once
                  continue_decoding = continue_decoding &&
                    stream->read_long_array
                    ((CORBA::Long *) value, bounds);
                  if (continue_decoding == 1)
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;

                case CORBA::tk_double:
                case CORBA::tk_longlong:
                case CORBA::tk_ulonglong:
                  // For primitives, compute the size only once
                  continue_decoding = continue_decoding &&
                    stream->read_longlong_array
                    ((CORBA::LongLong *) value, bounds);
                  if (continue_decoding == 1)
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;

                case CORBA::tk_boolean:
                  // For primitives, compute the size only once
                  continue_decoding = continue_decoding &&
                    stream->read_octet_array
                    ((CORBA::Octet *) value, bounds);
                  if (continue_decoding == 1)
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;

                case CORBA::tk_char:
                case CORBA::tk_octet:
                  // For primitives, compute the size only once
                  continue_decoding = continue_decoding &&
                    stream->read_octet_array
                    ((CORBA::Octet *) value, bounds);
                  if (continue_decoding == 1)
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;

                case CORBA::tk_longdouble:
                  // For primitives, compute the size only once
                  continue_decoding = continue_decoding &&
                    stream->read_longdouble_array
                    ((CORBA::LongDouble *) value, bounds);
                  if (continue_decoding == 1)
                    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;

                case CORBA::tk_wchar:
                  // For primitives, compute the size only once
                  continue_decoding = continue_decoding &&
                    stream->read_wchar_array
                    ((CORBA::WChar *) value, bounds);
                  if (continue_decoding == 1)
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
                      retval = stream->decode (tc2, value, 0, env);
                      value += size;
                    }
                  // CORBA::release (tc2);
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
                "TAO_Marshal_Sequence::decode detected error\n"));
  env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
  return CORBA::TypeCode::TRAVERSE_STOP;
}

// Decode alias.
CORBA::TypeCode::traverse_status
TAO_Marshal_Alias::decode (CORBA::TypeCode_ptr  tc,
                           const void *data,
                           const void *,
                           void *context,
                           CORBA::Environment &env)
{
  // Typecode of the aliased type.
  CORBA::TypeCode_ptr tc2;
  CORBA::Boolean continue_decoding = 1;

  // Context is the CDR stream.
  TAO_InputCDR *stream = (TAO_InputCDR *) context;

  // Status of decode operation.
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;
  char *value = (char *) data;

  tc2 = tc->content_type (env);
  if (env.exception () == 0)
    {
    // Switch on the data type and handle the cases for primitives
    // here for efficiency rather than calling.
    switch (tc2->kind_)
      {
      case CORBA::tk_null:
      case CORBA::tk_void:
        break;
      case CORBA::tk_short:
      case CORBA::tk_ushort:
        continue_decoding =
          stream->read_short (*(CORBA::Short *) value);
        break;
      case CORBA::tk_long:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_enum:
        continue_decoding =
          stream->read_long (*(CORBA::Long *) value);
        break;
      case CORBA::tk_double:
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
        continue_decoding =
          stream->read_longlong (*(CORBA::LongLong *) value);
        break;
      case CORBA::tk_boolean:
        continue_decoding =
          stream->read_boolean (*(CORBA::Boolean *) value);
        break;
      case CORBA::tk_char:
      case CORBA::tk_octet:
        continue_decoding =
          stream->read_char (*(CORBA::Char *) value);
        break;
      case CORBA::tk_longdouble:
        continue_decoding =
          stream->read_longdouble (*(CORBA::LongDouble *) value);
        break;
      case CORBA::tk_wchar:
        continue_decoding =
          stream->read_wchar (*(CORBA::WChar *) value);
        break;
      case CORBA::tk_string:
      case CORBA::tk_wstring:
      case CORBA::tk_any:
      case CORBA::tk_TypeCode:
      case CORBA::tk_Principal:
      case CORBA::tk_objref:
      case CORBA::tk_struct:
      case CORBA::tk_union:
      case CORBA::tk_sequence:
      case CORBA::tk_array:
      case CORBA::tk_alias:
      case CORBA::tk_except:
        retval = stream->decode (tc2, data, 0, env);
        break;
      default:
        // anything else is an error
        retval = CORBA::TypeCode::TRAVERSE_STOP;
      }
  }
  //  tc2->_decr_refcnt ();
  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_decoding == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Alias::decode detected error\n"));
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
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
TAO_Marshal_Except::decode (CORBA::TypeCode_ptr  tc,
                            const void *data,
                            const void *,
                            void *context,
                            CORBA::Environment &env)
{
  TAO_InputCDR *stream = (TAO_InputCDR *) context;
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::Boolean continue_decoding = 1;
  CORBA::TypeCode_ptr param;
  CORBA::Long size, alignment;

  data = (char *) data + sizeof (CORBA::Exception);
  // @@ (ASG) The reason this is done is because we want to skip the size
  // of the the base class and its private data members (type_ and
  // refcount_). After skipping these data members, we will have the data
  // members of the derived class which must be encoded.

  // Number of fields in the struct.
  int member_count = tc->member_count (env);
  if (env.exception () == 0)
    {
      for (int i = 0; i < member_count
             && retval == CORBA::TypeCode::TRAVERSE_CONTINUE
             && continue_decoding == 1; i++)
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
                      switch (param->kind_)
                        {
                        case CORBA::tk_null:
                        case CORBA::tk_void:
                          break;
                        case CORBA::tk_short:
                        case CORBA::tk_ushort:
                          continue_decoding =
                            stream->read_short (*(CORBA::Short *) data);
                          break;
                        case CORBA::tk_long:
                        case CORBA::tk_ulong:
                        case CORBA::tk_float:
                        case CORBA::tk_enum:
                          continue_decoding =
                            stream->read_long (*(CORBA::Long *) data);
                          break;
                        case CORBA::tk_double:
                        case CORBA::tk_longlong:
                        case CORBA::tk_ulonglong:
                          continue_decoding =
                            stream->read_longlong (*(CORBA::LongLong *) data);
                          break;
                        case CORBA::tk_boolean:
                          continue_decoding =
                            stream->read_boolean (*(CORBA::Boolean *) data);
                          break;
                        case CORBA::tk_char:
                        case CORBA::tk_octet:
                          continue_decoding =
                            stream->read_char (*(CORBA::Char *) data);
                          break;
                        case CORBA::tk_longdouble:
                          continue_decoding =
                            stream->read_longdouble (*(CORBA::LongDouble *) data);
                          break;
                        case CORBA::tk_wchar:
                          continue_decoding =
                            stream->read_wchar (*(CORBA::WChar *) data);
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
                          retval = stream->decode (param, data, 0, env);
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
      && continue_decoding == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_Except::decode detected error\n"));
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// decode wstring
CORBA::TypeCode::traverse_status
TAO_Marshal_WString::decode (CORBA::TypeCode_ptr,
                             const void *data,
                             const void *,
                             void *context,
                             CORBA::Environment &env)
{
  CORBA::Boolean continue_decoding = 1;
  TAO_InputCDR *stream = (TAO_InputCDR *) context;
  CORBA::WChar *str = *(CORBA::WChar **) data;
  CORBA::ULong len;

  // On decode, omit the check against specified wstring bounds, and
  // cope with illegal "zero length" strings (all lengths on the wire
  // must include a NUL).
  //
  // This is on the principle of being gracious in what we accept; we
  // don't generate messages that fail to comply with protocol specs,
  // but we will accept them when it's clear how to do so.

  continue_decoding = stream->read_ulong (len);

  ACE_NEW_RETURN (str,
                  CORBA::WChar [(size_t) (len)],
                  CORBA::TypeCode::TRAVERSE_CONTINUE);
  *((CORBA::WChar **) data) = str;

  if (len != 0)
    while (continue_decoding != 0 && len--)
      {
        continue_decoding = stream->read_wchar (*str);
        str++;
      }

  if (continue_decoding == 1)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Marshal_WString::decode detected error\n"));
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}
