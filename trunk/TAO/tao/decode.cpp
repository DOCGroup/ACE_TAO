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
//   The original code had a single static decoder function defined on the CDR
//   class that called traverse to interpret the data types. This version
//   defines a virtual method "decode" on each class and avoids calling traverse.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//     and 
//     Aniruddha Gokhale
// 
// ============================================================================

#if 0
#include "tao/orb.h"
#include "tao/cdr.h"
#include "tao/giop.h"
#include "tao/debug.h"
#endif

#include "tao/corba.h"

#if defined (HAVE_WIDEC_H)
#               include <widec.h>
#else
extern "C" 
{
  u_int wslen (const CORBA::WChar *);
  CORBA::WChar *wscpy (CORBA::WChar *, const CORBA::WChar *);
}
#endif /* HAVE_WIDEC_H */

extern CORBA::TypeCode TC_opaque;

// The decoder is exactly the reverse of the encoder, except that:
//
//      * Unmarshaling some data types involve allocating memory.  Such
//        types include sequences (the buffer), objrefs, Principals, Anys,
//        TypeCodes, and strings.
//
//      * The decoder is used when retrieving typecode parameters from
//        encapsulations.  This means it must deal with "CORBA::tk_indirect",
//        the magic value (~0) signifying typecode indirection.
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

CORBA::TypeCode_ptr __tc_consts [CORBA::TC_KIND_COUNT] = 
{
  CORBA::_tc_null
  , CORBA::_tc_void
  , CORBA::_tc_short
  , CORBA::_tc_long
  , CORBA::_tc_ushort

  , CORBA::_tc_ulong
  , CORBA::_tc_float
  , CORBA::_tc_double
  , CORBA::_tc_boolean
  , CORBA::_tc_char

  , CORBA::_tc_octet
  , CORBA::_tc_any
  , CORBA::_tc_TypeCode
  , CORBA::_tc_Principal
  , 0           // CORBA::_tc_Object ... type ID is CORBA_Object

  , 0           // CORBA_tk_struct
  , 0           // CORBA_tk_union
  , 0           // CORBA_tk_enum
  , 0           // CORBA::_tc_string ... unbounded
  , 0           // CORBA_tk_sequence

  , 0           // CORBA_tk_array

  , 0           // CORBA_tk_alias
  , 0           // CORBA_tk_except

  , CORBA::_tc_longlong
  , CORBA::_tc_ulonglong
  , CORBA::_tc_longdouble
  , CORBA::_tc_wchar
  , 0           // CORBA::_tc_wstring ... unbounded
};

CORBA::TypeCode::traverse_status
TAO_Marshal_Primitive::decode (CORBA::TypeCode_ptr  tc,
                               const void *data,
                               const void *,
                               void *context,
                               CORBA::Environment &env)
{
  CORBA::Boolean continue_decoding = CORBA::B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA::TypeCode::traverse_status   retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE; // status of encode operation 

  switch (tc->_kind)
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
      break;
    case CORBA::tk_short:
    case CORBA::tk_ushort:
      continue_decoding = stream->get_short (*(CORBA::Short *) data);
      break;
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_enum:
      continue_decoding = stream->get_long (*(CORBA::Long *) data);
      break;
    case CORBA::tk_double:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
      continue_decoding = stream->get_longlong (*(CORBA::LongLong *) data);
      break;
    case CORBA::tk_boolean:
      continue_decoding = stream->get_boolean (*(CORBA::Boolean *) data);
      break;
    case CORBA::tk_char:
    case CORBA::tk_octet:
      continue_decoding = stream->get_char (*(CORBA::Char *) data);
      break;
    case CORBA::tk_longdouble:
      continue_decoding = stream->get_longdouble (*(CORBA::LongDouble *) data);
      break;
    case CORBA::tk_wchar:
      continue_decoding = stream->get_wchar (*(CORBA::WChar *) data);
      break;
    default:
      retval = CORBA::TypeCode::TRAVERSE_STOP;
      // we are not a primitive type
    }
  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_decoding == CORBA::B_TRUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Primitive::decode detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
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
  CORBA::TypeCode_ptr elem_tc; // typecode of the element that makes the Any
  void *value = 0;   // value maintained by the Any
  CORBA::Boolean continue_decoding = CORBA::B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE; // status of encode operation 

  // decode the typecode description for the element
  if (stream->decode (CORBA::_tc_TypeCode, 
                      &elem_tc,
                      this,
                      env) == CORBA::TypeCode::TRAVERSE_CONTINUE) 
    {
      value = new CORBA::Octet[elem_tc->size (env)];
      if (env.exception () == 0)
        {
          // switch on the data type and handle the cases for
          // primitives here for efficiency rather than calling
          switch (elem_tc->_kind)
            {
            case CORBA::tk_null:
            case CORBA::tk_void:
              break;
            case CORBA::tk_short:
            case CORBA::tk_ushort:
              continue_decoding = stream->get_short (*(CORBA::Short *) value);
              break;
            case CORBA::tk_long:
            case CORBA::tk_ulong:
            case CORBA::tk_float:
            case CORBA::tk_enum:
              continue_decoding = stream->get_long (*(CORBA::Long *) value);
              break;
            case CORBA::tk_double:
            case CORBA::tk_longlong:
            case CORBA::tk_ulonglong:
              continue_decoding = stream->get_longlong (*(CORBA::LongLong *) value);
              break;
            case CORBA::tk_boolean:
              continue_decoding = stream->get_boolean (*(CORBA::Boolean *) value);
              break;
            case CORBA::tk_char:
            case CORBA::tk_octet:
              continue_decoding = stream->get_char (*(CORBA::Char *) value);
              break;
            case CORBA::tk_longdouble:
              continue_decoding = stream->get_longdouble (*(CORBA::LongDouble *) value);
              break;
            case CORBA::tk_wchar:
              continue_decoding = stream->get_wchar (*(CORBA::WChar *) value);
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
              retval = stream->decode (elem_tc, value, 0, env);
              break;
            default:
              // anything else is an error
              retval = CORBA::TypeCode::TRAVERSE_STOP;
            }
        }
      else
        retval = CORBA::TypeCode::TRAVERSE_STOP;
    }
  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_decoding == CORBA::B_TRUE)
    {
      // allocate an Any and populate it with the value and typecode. This
      // eventually appears as "data"
      (void) new (any) CORBA::Any (elem_tc, value, CORBA::B_TRUE);
      return CORBA::TypeCode::TRAVERSE_CONTINUE;
    }
  else 
    {
      // free the allocated storage and release the typecode
      delete [] value;
      CORBA::release (elem_tc);
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Any::decode detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

CORBA::TypeCode::traverse_status
TAO_Marshal_TypeCode::decode (CORBA::TypeCode_ptr,
                              const void *data,
                              const void *parent_typecode,
                              void *context,
                              CORBA::Environment  &env)
{
  CORBA::Boolean continue_decoding = CORBA::B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA::TypeCode_ptr *tcp;        // typecode to be decoded
  CORBA::ULong kind;               // typecode kind
  CORBA::TypeCode_ptr parent = (CORBA::TypeCode_ptr) parent_typecode; // TypeCode
								    // for the
								    // parent
  
  // decode the "kind" field of the typecode from the stream
  continue_decoding = stream->get_ulong (kind);

  if (continue_decoding == CORBA::B_TRUE)
    {
      tcp = (CORBA::TypeCode_ptr *) data;  // the data has to be a TypeCode_ptr*

      // Typecodes with empty parameter lists all have preallocated
      // constants.  We use those to reduce memory consumption and
      // heap access ... also, to speed things up!
      if (kind < CORBA::TC_KIND_COUNT
          && (*tcp = __tc_consts [(u_int) kind]) != 0)
        *tcp = __tc_consts [(u_int) kind];
      else
        {
          if (kind == ~(CORBA::ULong)0 || kind < CORBA::TC_KIND_COUNT)
            {
	      // either a non-constant typecode or an indirected typecode
              switch (kind) 
                {
                  // Need special handling for all kinds of typecodes
                  // that have nonempty parameter lists ...
                default:                        // error: missed a case! 
                  env.exception (new CORBA::INTERNAL (CORBA::COMPLETED_MAYBE));
                  return CORBA::TypeCode::TRAVERSE_STOP;

                  // Some have "simple" parameter lists ... some of these also
                  // have preallocated constants that could be used.
                case CORBA::tk_string:
                case CORBA::tk_wstring:
                  {
                    CORBA::ULong bound;

                    continue_decoding = stream->get_ulong (bound);
                    if (continue_decoding) 
                      {
                        if (bound == 0) 
                          {
                            if (kind == CORBA::tk_string)
                              *tcp = CORBA::_tc_string;
                            else
                              *tcp = CORBA::_tc_wstring;
                          } 
                        else 
                          {
			    // bounded string. Save the bounds
                            *tcp = new CORBA::TypeCode ((CORBA::TCKind) kind,
                                                       bound, 0, CORBA::B_FALSE,
						       parent);
			    //                            (*tcp)->_parent = parent;
                          }
                      }
                  }
                break;

                // Indirected typecodes, illegal at "top level" but we
                // allow unmarshaling of them here because we use the
                // same code to read "off the wire" (where they're
                // illegal) and to read out of an encapsulation
                // stream.  We distinguish the case where this is
                // legal as described above.
                case ~0:
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
                    CDR indir_stream;
                    CORBA::Long offset;

                    continue_decoding = stream->get_long (offset);
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
                    if (continue_decoding) 
                      {
			// the offset must be such that the indir_stream.next
			// should point to the TypeCode kind value of the
			// TypeCode to which we are referring to.
                        indir_stream.buffer = indir_stream.next
                          = stream->next + offset;
                        indir_stream.remaining = indir_stream.length = 8;

                        // Reject indirections outside parent's scope.
                        if (indir_stream.next < parent->_buffer)
                          continue_decoding = CORBA::B_FALSE;
                      }

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
                      continue_decoding = indir_stream.get_ulong (indir_kind);

                    if (continue_decoding
                        && indir_kind >= CORBA::TC_KIND_COUNT)
                      continue_decoding = CORBA::B_FALSE;

		    // now retrieve the encapsulation length
                    if (continue_decoding)
                      continue_decoding = indir_stream.get_ulong (indir_len);

                    // Now construct indirected typecode.  This shares the
                    // typecode octets with the "parent" typecode,
                    // increasing the amount of memory sharing and
                    // reducing the cost of getting typecodes.
                    if (continue_decoding) 
                      {
                        *tcp = new CORBA::TypeCode ((CORBA::TCKind) indir_kind,
                                                   indir_len,  // length of encapsulation
                                                   indir_stream.next, // octet buffer
                                                   CORBA::B_FALSE, // ORB
								  // doesn't own
						   parent); // this is our parent
#if 0
                        (*tcp)->_parent = parent;
                        parent->AddRef ();
#endif /* 0 */
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
#if defined(TAO_NEEDS_UNUSED_VARIABLES)                    
                    CORBA::Octet *buffer;
#endif                    

                    continue_decoding = stream->get_ulong (length);
                    if (!continue_decoding)
                      break;

                    // if length > MAXUNSIGNED, error ...
                    u_int len = (u_int) length;

#if 0
		    buffer = new CORBA::Octet[len];

                    for (u_int i = 0; i < len && continue_decoding; i++)
                      continue_decoding = stream->get_octet (buffer [i]);
#endif

                    if (!continue_decoding) 
                      {
			//  delete [] buffer;
                        break;
                      }
                    *tcp = new CORBA::TypeCode ((CORBA::TCKind) kind,
                                               len,
					       //                     buffer,
					       stream->next,
                                               CORBA::B_FALSE,
					       parent);
		    // skip length number of bytes in the stream, else we may
		    // leave the stream in an undefined state
		    (void) stream->skip_bytes (length);
		    //                    (*tcp)->_parent = parent;
                  }
                } // end of switch
            }
          else // bad _kind value to be decoded
            {
              env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
              dmsg ("TAO_Marshal_TypeCode: Bad _kind value in CDR stream");
              return CORBA::TypeCode::TRAVERSE_STOP;
            }
        }

    }
  if (continue_decoding == CORBA::B_TRUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_TypeCode::decode detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// encode Principal
CORBA::TypeCode::traverse_status
TAO_Marshal_Principal::decode (CORBA::TypeCode_ptr,
                               const void *data,
                               const void *,
                               void *context,
                               CORBA::Environment &env)
{
  CORBA::Boolean continue_decoding = CORBA::B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA::Principal_ptr *pp = (CORBA::Principal_ptr *) data;
  CORBA::ULong len;

  continue_decoding = stream->get_ulong (len);
  if (len == 0)
    *pp = 0;  // null principal
  else 
    {
      // allocate storage for Principal and its buffer
      *pp = new CORBA::Principal;
      (*pp)->id.buffer = new CORBA::Octet [ (size_t) len];
      (*pp)->id.maximum = (*pp)->id.length = len;

      for (u_int i = 0;
           continue_decoding != CORBA::B_FALSE && i < len;
           i++)
        continue_decoding = stream->get_octet ((*pp)->id.buffer [i]);
    }

  if (continue_decoding == CORBA::B_TRUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Principal::decode detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// decode obj ref
CORBA::TypeCode::traverse_status
TAO_Marshal_ObjRef::decode (CORBA::TypeCode_ptr,
                            const void *data,
                            const void *,
                            void *context,
                            CORBA::Environment &env)
{
  CORBA::Boolean continue_decoding = CORBA::B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::String type_hint;

  // First, read the type hint. This will be the type_id encoded in an
  // object reference.
  stream->decode (CORBA::_tc_string, &type_hint, 0, env);

  // Read the profiles, discarding all until an IIOP profile comes by.
  // Once we see an IIOP profile, ignore any further ones.
  //
  // XXX this will need to change someday to let different protocol
  // code be accessed, not just IIOP.  Protocol modules will be
  // dynamically loaded from shared libraries via ORB_init (), and we
  // just need to be able to access such preloaded libraries here as
  // we unmarshal objrefs.

  CORBA::ULong profiles;
  IIOP_Object *objdata = 0;

  // get the count of profiles that follow
  continue_decoding = stream->get_ulong (profiles);

  // No profiles means a NIL objref.

  if (profiles == 0) 
    {
      *(CORBA::Object_ptr *) data = CORBA::Object::_nil ();
      delete type_hint;
    } 
  else
    {
      while (profiles-- != 0 && continue_decoding) 
        {
          CORBA::ULong tmp;

          // get the profile ID tag
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

          // ProfileData is encoded as a sequence of octet. So first get the
          // length of the sequence
          continue_decoding = stream->get_ulong (tmp);
          assert (stream->remaining >= tmp);

          // Create the decoding stream from the encapsulation in
          // the buffer, and skip the encapsulation.
          CDR str;

          str.setup_encapsulation (stream->next, (size_t) tmp);

          stream->next += (u_int) tmp;
          stream->remaining -= (u_int) tmp;

          objdata = new IIOP_Object (type_hint);

          IIOP::ProfileBody     *profile = &objdata->profile;

          // Read and verify major, minor versions, ignoring IIOP
          // profiles whose versions we don't understand.
          //
          // XXX this doesn't actually go back and skip the whole
          // encapsulation...
          if (! (str.get_octet (profile->iiop_version.major)
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
          if (str.decode (CORBA::_tc_string, &profile->host, 0, env)
              != CORBA::TypeCode::TRAVERSE_CONTINUE
              || !str.get_ushort (profile->port))
            {
              env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
              dmsg ("error decoding IIOP host/port");
              objdata->Release ();
              return CORBA::TypeCode::TRAVERSE_STOP;
            }

          // ... and object key

          continue_decoding = str.decode (&TC_opaque,
                                          &profile->object_key,
                                          0,
                                          env) == CORBA::TypeCode::TRAVERSE_CONTINUE;

          if (str.remaining != 0) 
            {
              env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
              dmsg ("extra data at end of IIOP profile data");
              objdata->Release ();
              return CORBA::TypeCode::TRAVERSE_STOP;
            }
        }
    }
  if (objdata == 0) 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg2 ("no IIOP v%d.%d (or earlier) profile in IOR!", 
             IIOP::MY_MAJOR, IIOP::MY_MINOR);
      return CORBA::TypeCode::TRAVERSE_STOP;
    } 
  else 
    {
      // retrieve the CORBA::Object from the IIOP_Object we created before.
      if (objdata->QueryInterface (IID_CORBA_Object,
                                   (void **) data) != NOERROR)
        continue_decoding = CORBA::B_FALSE;
      objdata->Release ();
    }
  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE 
      && continue_decoding == CORBA::B_TRUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("marshaling encode_struct detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// decode structs
CORBA::TypeCode::traverse_status
TAO_Marshal_Struct::decode (CORBA::TypeCode_ptr  tc,
                            const void *data,
                            const void *,
                            void *context,
                            CORBA::Environment &env)
{
  CDR *stream = (CDR *) context;
  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::Boolean continue_decoding = CORBA::B_TRUE;
  CORBA::TypeCode_ptr param;
  CORBA::Long size, alignment;

  // number of fields in the struct
  int member_count = tc->member_count (env);

  if (env.exception () == 0)
    {
      for (int i = 0; i < member_count 
	     && retval == CORBA::TypeCode::TRAVERSE_CONTINUE 
	     && continue_decoding == CORBA::B_TRUE; 
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
			  continue_decoding = stream->get_short (*(CORBA::Short *) data);
			  break;
			case CORBA::tk_long:
			case CORBA::tk_ulong:
			case CORBA::tk_float:
			case CORBA::tk_enum:
			  continue_decoding = stream->get_long (*(CORBA::Long *) data);
			  break;
			case CORBA::tk_double:
			case CORBA::tk_longlong:
			case CORBA::tk_ulonglong:
			  continue_decoding = stream->get_longlong (*(CORBA::LongLong *) data);
			  break;
			case CORBA::tk_boolean:
			  continue_decoding = stream->get_boolean (*(CORBA::Boolean *) data);
			  break;
			case CORBA::tk_char:
			case CORBA::tk_octet:
			  continue_decoding = stream->get_char (*(CORBA::Char *) data);
			  break;
			case CORBA::tk_longdouble:
			  continue_decoding = stream->get_longdouble (*(CORBA::LongDouble *) data);
			  break;
			case CORBA::tk_wchar:
			  continue_decoding = stream->get_wchar (*(CORBA::WChar *) data);
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
      && continue_decoding == CORBA::B_TRUE)
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
TAO_Marshal_Union::decode (CORBA::TypeCode_ptr  tc,
                           const void *data,
                           const void *data2,
                           void *context,
                           CORBA::Environment &env)
{
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;

  CORBA::TypeCode_ptr discrim_tc;
  CORBA::TypeCode_ptr member_tc;
  CORBA::Any_ptr member_label;
  CORBA::ULong discrim_size_with_pad;
  const void *discrim_val;
  CORBA::ULong member_count;
  CORBA::Long  default_index;
  CORBA::ULong i;
  CORBA::TypeCode_ptr default_tc = 0;
  CORBA::Boolean discrim_matched = CORBA::B_FALSE;

  discrim_tc = tc->discriminator_type (env);
  // get the discriminator type

  if (env.exception () == 0)
    {
      // decode the discriminator value
      retval = stream->decode (discrim_tc, data, data2, env);
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
                              switch (member_label->type ()->kind (env))
                                {
                                case CORBA::tk_short:
                                case CORBA::tk_ushort:
                                  if (*(CORBA::Short *) member_label->value () ==
                                      *(CORBA::Short *) discrim_val)
				    discrim_matched = CORBA::B_TRUE;
                                  break;
                                case CORBA::tk_long:
                                case CORBA::tk_ulong:
                                case CORBA::tk_enum:
                                  if (*(CORBA::ULong *) member_label->value () ==
                                      *(CORBA::ULong *) discrim_val)
				    discrim_matched = CORBA::B_TRUE;
                                  break;
                                case CORBA::tk_char:
                                  if (*(CORBA::Char *) member_label->value () ==
                                      *(CORBA::Char *) discrim_val)
				    discrim_matched = CORBA::B_TRUE;
                                  break;
                                case CORBA::tk_wchar:
                                  if (*(CORBA::WChar *) member_label->value () ==
                                      *(CORBA::WChar *) discrim_val)
				    discrim_matched = CORBA::B_TRUE;
                                  break;
                                case CORBA::tk_boolean:
                                  if (*(CORBA::Boolean *) member_label->value () ==
                                      *(CORBA::Boolean *) discrim_val)
				    discrim_matched = CORBA::B_TRUE;
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
				    // marshal according to the matched typecode
				    return stream->decode (member_tc, data,
							   data2, env);
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
			return stream->decode (default_tc, data, data2, env);
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
  CORBA::Boolean continue_decoding = CORBA::B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA::ULong len = 0;
  CORBA::String str;

  // On decode, omit the check against specified string bounds, and
  // cope with illegal "zero length" strings (all lengths on the wire
  // must include a NUL).
  //
  // This is on the principle of being gracious in what we accept; we
  // don't generate messages that fail to comply with protocol specs,
  // but we will accept them when it's clear how to do so.

  continue_decoding = stream->get_ulong (len);
  // note that the encoded length is 1 more than the length of the string
  // because it also accounts for the terminating NULL character
  *((CORBA::String *) data) = str = new CORBA::Char [(size_t) (len)];

  if (len != 0)
    {
    while (continue_decoding != CORBA::B_FALSE && len-- != 0) 
      {
        continue_decoding = stream->get_char (*(CORBA::Char *) str);
        str++;
      }
    }
  if (continue_decoding == CORBA::B_TRUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_TypeCode::decode detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// decode sequence
CORBA::TypeCode::traverse_status
TAO_Marshal_Sequence::decode (CORBA::TypeCode_ptr  tc,
                              const void *data,
                              const void *,
                              void *context,
                              CORBA::Environment &env)
{
  CORBA::Boolean continue_decoding = CORBA::B_TRUE;  
  CDR *stream = (CDR *) context;
  CORBA::OctetSeq *seq = (CORBA::OctetSeq *) data;
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;  // return status
  CORBA::TypeCode_ptr    tc2;  // typecode of the element
  size_t  size; // size of element
  CORBA::ULong  bounds;
  char *value;

  // First unmarshal the sequence length ... we trust it to be right
  // here, on the "be gracious in what you accept" principle.  We
  // don't generate illegal sequences (i.e. length > bounds).

  continue_decoding = stream->get_ulong (seq->length);
  seq->maximum = seq->length;
  seq->buffer = 0;

  if (continue_decoding)
    {
      // no point decoding an empty sequence
      if (seq->length > 0)
        {
          // get element typecode
          tc2 = tc->content_type (env);  

          if (env.exception () == 0)
            {
              size = tc2->size (env);

              if (env.exception () == 0)
                {
                  bounds = seq->length;
                  // allocate a buffer to hold the sequence
                  seq->buffer = new CORBA::Octet [size *(size_t) seq->maximum];
                  value = (char *) seq->buffer;
                  switch (tc2->_kind)
                    {
                    case CORBA::tk_null:
                    case CORBA::tk_void:
                      return CORBA::TypeCode::TRAVERSE_CONTINUE;
                    case CORBA::tk_short:
                    case CORBA::tk_ushort:
                      // For primitives, compute the size only once
                      while (bounds-- && continue_decoding == CORBA::B_TRUE)
                        {
                          continue_decoding = stream->get_short (*(CORBA::Short *) value);
                          value += size;
                        }
                      //                      CORBA::release (tc2);
                      if (continue_decoding == CORBA::B_TRUE)
			return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;
                    case CORBA::tk_long:
                    case CORBA::tk_ulong:
                    case CORBA::tk_float:
                      // For primitives, compute the size only once
                      while (bounds-- && continue_decoding == CORBA::B_TRUE)
                        {
                          continue_decoding = stream->get_long (*(CORBA::Long *) value);
                          value += size;
                        }
                      //                      CORBA::release (tc2);
                      if (continue_decoding == CORBA::B_TRUE)
			return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;
                    case CORBA::tk_double:
                    case CORBA::tk_longlong:
                    case CORBA::tk_ulonglong:
                      // For primitives, compute the size only once
                      while (bounds-- && continue_decoding == CORBA::B_TRUE)
                        {
                          continue_decoding = stream->get_longlong (*(CORBA::LongLong *) value);
                          value += size;
                        }
                      //                      CORBA::release (tc2);
                      if (continue_decoding == CORBA::B_TRUE)
			return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;
                    case CORBA::tk_boolean:
                      // For primitives, compute the size only once
                      while (bounds-- && continue_decoding == CORBA::B_TRUE)
                        {
                          continue_decoding = stream->get_boolean (*(CORBA::Boolean *) value);
                          value += size;
                        }
                      //                      CORBA::release (tc2);
                      if (continue_decoding == CORBA::B_TRUE)
			return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;
                    case CORBA::tk_char:
                    case CORBA::tk_octet:
                      // For primitives, compute the size only once
                      while (bounds-- && continue_decoding == CORBA::B_TRUE)
                        {
                          continue_decoding = stream->get_char (*(CORBA::Char *) value);
                          value += size;
                        }
                      //                      CORBA::release (tc2);
                      if (continue_decoding == CORBA::B_TRUE)
			return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;
                    case CORBA::tk_longdouble:
                      // For primitives, compute the size only once
                      while (bounds-- && continue_decoding == CORBA::B_TRUE)
                        {
                          continue_decoding = stream->get_longdouble (*(CORBA::LongDouble *) value);
                          value += size;
                        }
                      //                      CORBA::release (tc2);
                      if (continue_decoding == CORBA::B_TRUE)
			return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;
                    case CORBA::tk_wchar:
                      // For primitives, compute the size only once
                      while (bounds-- && continue_decoding == CORBA::B_TRUE)
                        {
                          continue_decoding = stream->get_wchar (*(CORBA::WChar *) value);
                          value += size;
                        }
                      //                      CORBA::release (tc2);
                      if (continue_decoding == CORBA::B_TRUE)
			return CORBA::TypeCode::TRAVERSE_CONTINUE;
                      break;
                    case CORBA::tk_enum:
                      // For primitives, compute the size only once
                      while (bounds-- && continue_decoding == CORBA::B_TRUE)
                        {
                          continue_decoding = stream->get_long (*(CORBA::Long *) value);
                          value += size;
                        }
                      //                      CORBA::release (tc2);
                      if (continue_decoding == CORBA::B_TRUE)
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
                      //                      CORBA::release (tc2);
                      if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
                        return CORBA::TypeCode::TRAVERSE_CONTINUE;
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
  // error exit
  env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
  dmsg ("marshaling TAO_Marshal_Sequence::decode detected error");
  return CORBA::TypeCode::TRAVERSE_STOP;
}

// decode array
CORBA::TypeCode::traverse_status
TAO_Marshal_Array::decode (CORBA::TypeCode_ptr  tc,
                           const void *data,
                           const void *,
                           void *context,
                           CORBA::Environment &env)
{
  CORBA::Boolean continue_decoding = CORBA::B_TRUE;  
  CDR *stream = (CDR *) context;
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;  // return status
  CORBA::TypeCode_ptr    tc2;  // typecode of the element
  size_t  size; // size of element
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
              switch (tc2->_kind)
                {
                case CORBA::tk_null:
                case CORBA::tk_void:
                  return CORBA::TypeCode::TRAVERSE_CONTINUE;
                case CORBA::tk_short:
                case CORBA::tk_ushort:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_decoding == CORBA::B_TRUE)
                    {
                      continue_decoding = stream->get_short (*(CORBA::Short *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_decoding == CORBA::B_TRUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                case CORBA::tk_long:
                case CORBA::tk_ulong:
                case CORBA::tk_float:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_decoding == CORBA::B_TRUE)
                    {
                      continue_decoding = stream->get_long (*(CORBA::Long *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_decoding == CORBA::B_TRUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                case CORBA::tk_double:
                case CORBA::tk_longlong:
                case CORBA::tk_ulonglong:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_decoding == CORBA::B_TRUE)
                    {
                      continue_decoding = stream->get_longlong (*(CORBA::LongLong *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_decoding == CORBA::B_TRUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                case CORBA::tk_boolean:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_decoding == CORBA::B_TRUE)
                    {
                      continue_decoding = stream->get_boolean (*(CORBA::Boolean *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_decoding == CORBA::B_TRUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                case CORBA::tk_char:
                case CORBA::tk_octet:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_decoding == CORBA::B_TRUE)
                    {
                      continue_decoding = stream->get_char (*(CORBA::Char *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_decoding == CORBA::B_TRUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                case CORBA::tk_longdouble:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_decoding == CORBA::B_TRUE)
                    {
                      continue_decoding = stream->get_longdouble (*(CORBA::LongDouble *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_decoding == CORBA::B_TRUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                case CORBA::tk_wchar:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_decoding == CORBA::B_TRUE)
                    {
                      continue_decoding = stream->get_wchar (*(CORBA::WChar *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_decoding == CORBA::B_TRUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
                  break;
                case CORBA::tk_enum:
                  // For primitives, compute the size only once
                  while (bounds-- && continue_decoding == CORBA::B_TRUE)
                    {
                      continue_decoding = stream->get_long (*(CORBA::Long *) value);
                      value += size;
                    }
                  //              CORBA::release (tc2);
                  if (continue_decoding == CORBA::B_TRUE)
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
  dmsg ("marshaling TAO_Marshal_Sequence::decode detected error");
  return CORBA::TypeCode::TRAVERSE_STOP;
}

// decode alias
CORBA::TypeCode::traverse_status
TAO_Marshal_Alias::decode (CORBA::TypeCode_ptr  tc,
                           const void *data,
                           const void *,
                           void *context,
                           CORBA::Environment &env)
{
  CORBA::TypeCode_ptr    tc2;  // typecode of the aliased type
  CORBA::Boolean continue_decoding = CORBA::B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA::TypeCode::traverse_status   retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE; // status of decode operation 
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
        continue_decoding = stream->get_short (*(CORBA::Short *) value);
        break;
      case CORBA::tk_long:
      case CORBA::tk_ulong:
      case CORBA::tk_float:
      case CORBA::tk_enum:
        continue_decoding = stream->get_long (*(CORBA::Long *) value);
        break;
      case CORBA::tk_double:
      case CORBA::tk_longlong:
      case CORBA::tk_ulonglong:
        continue_decoding = stream->get_longlong (*(CORBA::LongLong *) value);
        break;
      case CORBA::tk_boolean:
        continue_decoding = stream->get_boolean (*(CORBA::Boolean *) value);
        break;
      case CORBA::tk_char:
      case CORBA::tk_octet:
        continue_decoding = stream->get_char (*(CORBA::Char *) value);
        break;
      case CORBA::tk_longdouble:
        continue_decoding = stream->get_longdouble (*(CORBA::LongDouble *) value);
        break;
      case CORBA::tk_wchar:
        continue_decoding = stream->get_wchar (*(CORBA::WChar *) value);
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
        retval = stream->decode (tc2, value, 0, env);
        break;
      default:
        // anything else is an error
        retval = CORBA::TypeCode::TRAVERSE_STOP;
      }
  }
  //  tc2->Release ();
  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE
      && continue_decoding == CORBA::B_TRUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Alias::decode detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// decode exception For exceptions, the "hidden" type ID near the
// front of the on-wire representation was previously unmarshaled and
// mapped to the "tc" typcode we're using to traverse the memory ...
// at the same time its vtable, refcount, and other state was
// established.
//
// NOTE:  This is asymmetric with respect to encoding exceptions.
CORBA::TypeCode::traverse_status
TAO_Marshal_Except::decode (CORBA::TypeCode_ptr  tc,
                            const void *data,
                            const void *,
                            void *context,
                            CORBA::Environment   &env)
{
  CDR *stream = (CDR *) context;
  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::Boolean continue_decoding = CORBA::B_TRUE;
  CORBA::TypeCode_ptr param;
  CORBA::Long size, alignment;

  data = (char *) data + sizeof (CORBA::Exception);

  // number of fields in the struct
  int member_count = tc->member_count (env);
  if (env.exception () == 0)
    {
      for (int i = 0; i < member_count 
	     && retval == CORBA::TypeCode::TRAVERSE_CONTINUE 
	     && continue_decoding == CORBA::B_TRUE; i++)
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
			  continue_decoding = stream->get_short (*(CORBA::Short *) data);
			  break;
			case CORBA::tk_long:
			case CORBA::tk_ulong:
			case CORBA::tk_float:
			case CORBA::tk_enum:
			  continue_decoding = stream->get_long (*(CORBA::Long *) data);
			  break;
			case CORBA::tk_double:
			case CORBA::tk_longlong:
			case CORBA::tk_ulonglong:
			  continue_decoding = stream->get_longlong (*(CORBA::LongLong *) data);
			  break;
			case CORBA::tk_boolean:
			  continue_decoding = stream->get_boolean (*(CORBA::Boolean *) data);
			  break;
			case CORBA::tk_char:
			case CORBA::tk_octet:
			  continue_decoding = stream->get_char (*(CORBA::Char *) data);
			  break;
			case CORBA::tk_longdouble:
			  continue_decoding = stream->get_longdouble (*(CORBA::LongDouble *) data);
			  break;
			case CORBA::tk_wchar:
			  continue_decoding = stream->get_wchar (*(CORBA::WChar *) data);
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
      && continue_decoding == CORBA::B_TRUE)
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Except detected error");
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
  CORBA::Boolean continue_decoding = CORBA::B_TRUE;
  CDR *stream = (CDR *) context;  // context is the CDR stream
  CORBA::WChar *str = *(CORBA::WChar **) data;
  CORBA::ULong len;

  // On decode, omit the check against specified wstring bounds, and
  // cope with illegal "zero length" strings (all lengths on the wire
  // must include a NUL).
  //
  // This is on the principle of being gracious in what we accept; we
  // don't generate messages that fail to comply with protocol specs,
  // but we will accept them when it's clear how to do so.

  continue_decoding = stream->get_ulong (len);

  *((CORBA::WChar **) data) = str = new CORBA::WChar [(size_t) (len)];

  if (len != 0) 
    while (continue_decoding != CORBA::B_FALSE && len--) 
      {
        continue_decoding = stream->get_wchar (*str);
        str++;
      }

  if (continue_decoding == CORBA::B_TRUE)
      return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_TypeCode::decode detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

