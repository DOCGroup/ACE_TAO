// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    deep_copy.cpp
//
// = DESCRIPTION
//   Code for deep_copy
//   The original code had a single static deep_copy function that called
//   traverse to interpret the data types. This version defines a static method
//   "deep_copy" on each class and avoids calling traverse.
//
//
//   Helper routine for "Any" copy constructor ...
//
//   "Deep Copy" from source to dest.  Memory is always there to be
//   copied to ... if this calls other deep_copy methods, it ensures that
//   this remains true (only really an issue for sequences) .
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//     and 
//     Aniruddha Gokhale
// 
// ============================================================================

#include "tao/orb.h"
#include "tao/cdr.h"
#include "tao/giop.h"
#include "tao/debug.h"

#if     defined (HAVE_WIDEC_H)
#               include <widec.h>
#else
extern "C" 
{
  u_int wslen (const CORBA_WChar *);
  CORBA_WChar *wscpy (CORBA_WChar *, const CORBA_WChar *);
}
#endif

extern CORBA_TypeCode TC_opaque;

  // Deep copy from "source" to "dest" ... this code "knows" a bit
  // about representations, verify it when porting to oddball
  // platforms with non-IEEE floating point values or atypical byte
  // and word sizes.
  //


CORBA_TypeCode::traverse_status
DEEP_COPY (CORBA_TypeCode_ptr  param,
           const void *source,
           const void *dest,
           CORBA_Environment &env)
{
  CORBA_TypeCode::traverse_status retval = CORBA_TypeCode::TRAVERSE_CONTINUE;

  switch (param->_kind)
    {
    case tk_null:
    case tk_void:
      break;
    case tk_short:
    case tk_ushort:
      *(CORBA_Short *) dest = *(CORBA_Short *) source;
      break;
    case tk_long:
    case tk_ulong:
    case tk_float:
    case tk_enum:
      *(CORBA_Long *) dest = *(CORBA_Long *) source;
      break;
    case tk_double:
    case tk_longlong:
    case tk_ulonglong:
      *(CORBA_LongLong *) dest = *(CORBA_LongLong *) source;
      break;
    case tk_boolean:
      *(CORBA_Boolean *) dest = *(CORBA_Boolean *) source;
      break;
    case tk_char:
    case tk_octet:
      *(CORBA_Char *) dest = *(CORBA_Char *) source;
      break;
    case tk_longdouble:
      *(CORBA_LongDouble *) dest = *(CORBA_LongDouble *) source;
      break;
    case tk_wchar:
      *(CORBA_WChar *) dest = *(CORBA_WChar *) source;
      break;
    case tk_any:
      retval = TAO_Marshal_Any::deep_copy (param, source, dest, env);
      break;
    case tk_TypeCode:
      retval = TAO_Marshal_TypeCode::deep_copy (param, source, dest, env);
      break;
    case tk_Principal:
      retval = TAO_Marshal_Principal::deep_copy (param, source, dest, env);
      break;
    case tk_objref:
      retval = TAO_Marshal_ObjRef::deep_copy (param, source, dest, env);
      break;
    case tk_struct:
      retval = TAO_Marshal_Struct::deep_copy (param, source, dest, env);
      break;
    case tk_union:
      retval = TAO_Marshal_Union::deep_copy (param, source, dest, env);
      break;
    case tk_string:
      retval = TAO_Marshal_String::deep_copy (param, source, dest, env);
      break;
    case tk_sequence:
      retval = TAO_Marshal_Sequence::deep_copy (param, source, dest, env);
      break;
    case tk_array:
      retval = TAO_Marshal_Array::deep_copy (param, source, dest, env);
      break;
    case tk_alias:
      retval = TAO_Marshal_Alias::deep_copy (param, source, dest, env);
      break;
    case tk_except:
      retval = TAO_Marshal_Except::deep_copy (param, source, dest, env);
      break;
    case tk_wstring:
      retval = TAO_Marshal_WString::deep_copy (param, source, dest, env);
      break;
    default:
      retval = CORBA_TypeCode::TRAVERSE_STOP;
    } // end of switch
  return retval;
}

// deep copy for primitives
CORBA_TypeCode::traverse_status
TAO_Marshal_Primitive::deep_copy (CORBA_TypeCode_ptr  tc,
                                  const void *source,
                                  const void *dest,
                                  CORBA_Environment &env)
{
  if (tc) 
    {
      CORBA_TCKind my_kind = tc->kind (env);

      if (env.exception () == 0)
        {
          switch (my_kind) 
            {
            case tk_null:
            case tk_void:
	      // @@ I assume this should be an error...
              break;
      
            case tk_char:
            case tk_octet:
              *(CORBA_Octet *) dest = *(CORBA_Octet *) source;
              return CORBA_TypeCode::TRAVERSE_CONTINUE;

            case tk_short:
            case tk_ushort:
              *(CORBA_Short *) dest = *(CORBA_Short *) source;
              return CORBA_TypeCode::TRAVERSE_CONTINUE;

            case tk_wchar:
              *(CORBA_WChar *) dest = *(CORBA_WChar *) source;
              return CORBA_TypeCode::TRAVERSE_CONTINUE;

            case tk_long:
            case tk_ulong:
            case tk_float:
            case tk_enum:
              *(CORBA_Long *) dest = *(CORBA_Long *) source;
              return CORBA_TypeCode::TRAVERSE_CONTINUE;

            case tk_longlong:
            case tk_ulonglong:
            case tk_double:
              *(CORBA_LongLong *) dest = *(CORBA_LongLong *) source;
              return CORBA_TypeCode::TRAVERSE_CONTINUE;

            case tk_longdouble:
              *(CORBA_LongDouble *) dest = *(CORBA_LongDouble *) source;
              return CORBA_TypeCode::TRAVERSE_CONTINUE;

            case tk_boolean:
              *(CORBA_Boolean *) dest = *(CORBA_Boolean *) source;
              return CORBA_TypeCode::TRAVERSE_CONTINUE;
            default:
              env.exception (new CORBA_BAD_TYPECODE (COMPLETED_MAYBE));
              dmsg ("TAO_Marshal_Primitive::deep_copy detected error");
              return CORBA_TypeCode::TRAVERSE_STOP;
            }
        }
      else
        {
          dmsg ("TAO_Marshal_Primitive::deep_copy detected error");
          return CORBA_TypeCode::TRAVERSE_STOP;
        }
    }

  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_MAYBE) );
  dmsg ("TAO_Marshal_Primitive::deep_copy detected error");
  return CORBA_TypeCode::TRAVERSE_STOP;
}

// deep_copy for Principal
CORBA_TypeCode::traverse_status
TAO_Marshal_Principal::deep_copy (CORBA_TypeCode_ptr  tc,
                                  const void *source,
                                  const void *dest,
                                  CORBA_Environment &env)
{
  if (tc)
    {
      CORBA_Principal_ptr src = *(CORBA_Principal_ptr *) source;
      CORBA_Principal_ptr dst = *(CORBA_Principal_ptr *) dest = new CORBA_Principal;
      
      if (dst)
        {
          // Principals are just opaque IDs ... copy them
          assert (src->id.length <= UINT_MAX);
          dst->id.length = dst->id.maximum = src->id.length;

          if (dst->id.length > 0) 
            {
              dst->id.buffer = new CORBA_Octet [(u_int) dst->id.length];
              if (dst->id.buffer)
                {
                  ACE_OS::memcpy (dst->id.buffer,
				  src->id.buffer,
				  (size_t) dst->id.length);
                  return CORBA_TypeCode::TRAVERSE_CONTINUE;
                }
              else
                {
                  env.exception (new CORBA_NO_MEMORY (COMPLETED_MAYBE) );
                  dmsg ("TAO_Marshal_Principal::deep_copy detected error");
                  return CORBA_TypeCode::TRAVERSE_STOP;
                }
            } 
          else
            {
              dst->id.buffer = 0;
              return CORBA_TypeCode::TRAVERSE_CONTINUE;
            }
        }
      else
        {
          env.exception (new CORBA_NO_MEMORY (COMPLETED_MAYBE) );
          dmsg ("TAO_Marshal_Principal::deep_copy detected error");
          return CORBA_TypeCode::TRAVERSE_STOP;
        }
    }
  else 
    {
      env.exception (new CORBA_BAD_TYPECODE (COMPLETED_MAYBE) );
      dmsg ("TAO_Marshal_Primitive::deep_copy detected error");
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
}

// deep_copy structs
CORBA_TypeCode::traverse_status
TAO_Marshal_Struct::deep_copy (CORBA_TypeCode_ptr  tc,
                               const void *source,
                               const void *dest,
                               CORBA_Environment &env)
{
  if (tc)
    {
      // compute the number of fields in the struct
      int member_count = tc->member_count (env);

      if (env.exception () == 0)
        {
	  CORBA_TypeCode::traverse_status retval =
	    CORBA_TypeCode::TRAVERSE_CONTINUE;

          for (int i = 0; i < member_count 
		 && retval == CORBA_TypeCode::TRAVERSE_CONTINUE;
	       i++)
            {
              // get the typecode for the ith field
	      CORBA_TypeCode_ptr param = tc->member_type (i, env);
              if (env.exception () == 0)
                {
                  // get the size of the field
                  CORBA_Long size = param->size (env);
                  if (env.exception () == 0)
                    {
                      // get the alignment of the field
                      CORBA_Long alignment = param->alignment (env);
                      if (env.exception () == 0)
                        {
                          switch (param->_kind)
                            {
                            case tk_null:
                            case tk_void:
                              break;
                            case tk_short:
                            case tk_ushort:
                              *(CORBA_Short *) dest = *(CORBA_Short *) source;
                              break;
                            case tk_long:
                            case tk_ulong:
                            case tk_float:
                            case tk_enum:
                              *(CORBA_Long *) dest = *(CORBA_Long *) source;
                              break;
                            case tk_double:
                            case tk_longlong:
                            case tk_ulonglong:
                              *(CORBA_LongLong *) dest = *(CORBA_LongLong *) source;
                              break;
                            case tk_boolean:
                              *(CORBA_Boolean *) dest = *(CORBA_Boolean *) source;
                              break;
                            case tk_char:
                            case tk_octet:
                              *(CORBA_Char *) dest = *(CORBA_Char *) source;
                              break;
                            case tk_longdouble:
                              *(CORBA_LongDouble *) dest = *(CORBA_LongDouble *) source;
                              break;
                            case tk_wchar:
                              *(CORBA_WChar *) dest = *(CORBA_WChar *) source;
                              break;
                            case tk_any:
                              retval = TAO_Marshal_Any::deep_copy (param, source, dest, env);
                              break;
                            case tk_TypeCode:
                              retval = TAO_Marshal_TypeCode::deep_copy (param, source, dest, env);
                              break;
                            case tk_Principal:
                              retval = TAO_Marshal_Principal::deep_copy (param, source, dest, env);
                              break;
                            case tk_objref:
                              retval = TAO_Marshal_ObjRef::deep_copy (param, source, dest, env);
                              break;
                            case tk_struct:
                              retval = TAO_Marshal_Struct::deep_copy (param, source, dest, env);
                              break;
                            case tk_union:
                              retval = TAO_Marshal_Union::deep_copy (param, source, dest, env);
                              break;
                            case tk_string:
                              retval = TAO_Marshal_String::deep_copy (param, source, dest, env);
                              break;
                            case tk_sequence:
                              retval = TAO_Marshal_Sequence::deep_copy (param, source, dest, env);
                              break;
                            case tk_array:
                              retval = TAO_Marshal_Array::deep_copy (param, source, dest, env);
                              break;
                            case tk_alias:
                              retval = TAO_Marshal_Alias::deep_copy (param, source, dest, env);
                              break;
                            case tk_except:
                              retval = TAO_Marshal_Except::deep_copy (param, source, dest, env);
                              break;
                            case tk_wstring:
                              retval = TAO_Marshal_WString::deep_copy (param, source, dest, env);
                              break;
                            default:
                              retval = CORBA_TypeCode::TRAVERSE_STOP;
                            } // end of switch
                          source = (char *) source + size;
                          dest = (char *) dest + size;
                        }  
                      else  // exception computing alignment 
                        {
                          dmsg ("TAO_Marshal_Struct::deep_copy detected error");
                          return CORBA_TypeCode::TRAVERSE_STOP;
                        }
                    } 
                  else  // exception computing size 
                    {
                      dmsg ("TAO_Marshal_Struct::deep_copy detected error");
                      return CORBA_TypeCode::TRAVERSE_STOP;
                    }
                } 
              else  // exception computing typecode
                {
                  dmsg ("TAO_Marshal_Struct::deep_copy detected error");
                  return CORBA_TypeCode::TRAVERSE_STOP;
                }
            } // end of loop
          if (retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
	    return CORBA_TypeCode::TRAVERSE_CONTINUE;
          else
            {
              env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
              dmsg ("TAO_Marshal_Struct::deep_copy detected error");
              return CORBA_TypeCode::TRAVERSE_STOP;
            }
        } 
      else // exception getting member count 
        {
          dmsg ("TAO_Marshal_Struct::deep_copy detected error");
          return CORBA_TypeCode::TRAVERSE_STOP;
        }
    }
  else // no typecode
    {
      env.exception (new CORBA_BAD_TYPECODE (COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Struct::deep_copy detected error");
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
}

// deep_copy for union
CORBA_TypeCode::traverse_status
TAO_Marshal_Union::deep_copy (CORBA_TypeCode_ptr  tc,
                              const void *data,
                              const void *data2,
                              CORBA_Environment &env)
{
  CORBA_TypeCode_ptr discrim_tc = tc->discriminator_type (env);

  // Get the discriminator type.
  if (env.exception () == 0)
    {
      // deep_copy the discriminator value
      CORBA_TypeCode::traverse_status retval =
	DEEP_COPY (discrim_tc, data, data2, env);

      if (retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
        {
	  CORBA_ULong discrim_size_with_pad =
	    tc->TAO_discrim_pad_size (env);

          if (env.exception () == 0)
            {
	      CORBA_TypeCode_ptr default_tc = 0;

	      const void *discrim_val = data; // save the pointer to the discriminator
	      // value 
              // move the pointer to point to the actual value
              data = (char *) data + discrim_size_with_pad;
              data2 = (char *) data2 + discrim_size_with_pad;
              // now get ready to marshal the actual union value
              CORBA_Long default_index = tc->default_index (env);
              if (env.exception () == 0)
                {
                  int member_count = tc->member_count (env);
                  if (env.exception () == 0)
                    {
                      // check which label value matches with the discriminator
                      // value. Accordingly, marshal the corresponding
                      // member_type. If none match, check if default exists
                      // and marshal accordingly. Otherwise it is an error.
                      for (int i = 0; member_count-- != 0; i++)
			{
			  CORBA_Any_ptr member_label = tc->member_label (i, env);
			  if (env.exception () == 0)
			    {
			      CORBA_Boolean discrim_matched = CORBA_B_FALSE;

			      // do the matching
			      switch (member_label->type ()->kind (env))
				{
				case tk_short:
				case tk_ushort:
				  if (*(CORBA_Short *) member_label->value () ==
				      *(CORBA_Short *) discrim_val)
				    discrim_matched = CORBA_B_TRUE;
				  break;
				case tk_long:
				case tk_ulong:
				case tk_enum:
				  if (*(CORBA_ULong *) member_label->value () ==
				      *(CORBA_ULong *) discrim_val)
				    discrim_matched = CORBA_B_TRUE;
				  break;
				case tk_char:
				  if (*(CORBA_Char *) member_label->value () ==
				      *(CORBA_Char *) discrim_val)
				    discrim_matched = CORBA_B_TRUE;
				  break;
				case tk_wchar:
				  if (*(CORBA_WChar *) member_label->value () ==
				      *(CORBA_WChar *) discrim_val)
				    discrim_matched = CORBA_B_TRUE;
				  break;
				case tk_boolean:
				  if (*(CORBA_Boolean *) member_label->value () ==
				      *(CORBA_Boolean *) discrim_val)
				    discrim_matched = CORBA_B_TRUE;
				  break;
				default:
				  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
				  return CORBA_TypeCode::TRAVERSE_STOP;
				}// end of switch

			      // get the member typecode
			      CORBA_TypeCode_ptr member_tc = tc->member_type (i, env);
			      if (env.exception () == 0)
				{
				  if (default_index >= 0 && default_index-- == 0)
				    // have we reached the default label?, if so,
				    // save a handle to the typecode for the default
				    default_tc = member_tc;
				  if (discrim_matched)
				    // marshal according to the matched typecode
				    return DEEP_COPY (member_tc, data,
						      data2, env);
				}
			      else
				{
				  env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
				  return CORBA_TypeCode::TRAVERSE_STOP;
				}
			    }
			  else
			    {
			      env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
			      return CORBA_TypeCode::TRAVERSE_STOP;
			    }
			} // end of while
		      
		      // we are here only if there was no match
		      if (default_tc)
			return DEEP_COPY (default_tc, data, data2, env);
		      else
			return CORBA_TypeCode::TRAVERSE_CONTINUE;
		    }
                  else
                    {
                      env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
                      return CORBA_TypeCode::TRAVERSE_STOP;
                    }
                }
              else
                {
                  env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
                  return CORBA_TypeCode::TRAVERSE_STOP;
                }
            }
          else
            {
              env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
              return CORBA_TypeCode::TRAVERSE_STOP;
            }
        }
      else
        {
          env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
          return CORBA_TypeCode::TRAVERSE_STOP;
        }
    }
  else
    {
      env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
}

// deep_copy for Sequence
CORBA_TypeCode::traverse_status
TAO_Marshal_Sequence::deep_copy (CORBA_TypeCode_ptr  tc,
                                 const void *source,
                                 const void *dest,
                                 CORBA_Environment &env)
{
  CORBA_TypeCode::traverse_status retval =
    CORBA_TypeCode::TRAVERSE_CONTINUE;  // return status
  CORBA_TypeCode_ptr    tc2;  // typecode of the element
  size_t  size; // size of element
  CORBA_ULong  bounds;
  char *value1, *value2;
  CORBA_OctetSeq          *src, *dst;
  CDR stream;  // used only to access the marshal_object factory

  // Rely on binary format of sequences -- all are the same
  // except for the type pointed to by "buffer"

  if (tc)
    {
      src = (CORBA_OctetSeq *) source;
      dst = (CORBA_OctetSeq *) dest;

      assert (src->length <= UINT_MAX);
      dst->length = dst->maximum = src->length;

      // get element typecode
      tc2 = tc->content_type (env);  
      if (env.exception () == 0)
        {
          // get the size of the element
          size = tc2->size (env);
          if (env.exception () == 0)
            {
              // compute the length of the sequence
              bounds = src->length;

              // allocate a buffer to hold the sequence
              dst->buffer = new CORBA_Octet [size *(size_t) src->maximum];
              if (dst->buffer)
                {
                  value1 = (char *) src->buffer;
                  value2 = (char *) dst->buffer;

                  switch (tc2->_kind)
                    {
                    case tk_null:
                    case tk_void:
                      //                      CORBA_release (tc2);
                      return CORBA_TypeCode::TRAVERSE_CONTINUE;
                    case tk_short:
                    case tk_ushort:
                    case tk_long:
                    case tk_ulong:
                    case tk_float:
                    case tk_double:
                    case tk_longlong:
                    case tk_ulonglong:
                    case tk_boolean:
                    case tk_char:
                    case tk_octet:
                    case tk_longdouble:
                    case tk_wchar:
                    case tk_enum:
                      // just do a memcpy rather than copying each element
                      ACE_OS::memcpy (value2, value1, size*bounds);
                      //                      CORBA_release (tc2);
                      return CORBA_TypeCode::TRAVERSE_CONTINUE;

                      // handle all aggregate types here
                    case tk_any:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Any::deep_copy (tc2, source, dest, env);
                          value1 = (char *) value1 + size;
                          value2 = (char *) value2 + size;
                        }
                      break;
                    case tk_TypeCode:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_TypeCode::deep_copy (tc2, source, dest, env);
                          value1 = (char *) value1 + size;
                          value2 = (char *) value2 + size;
                        }
                      break;
                    case tk_Principal:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Principal::deep_copy (tc2, source, dest, env);
                          value1 = (char *) value1 + size;
                          value2 = (char *) value2 + size;
                        }
                      break;
                    case tk_objref:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_ObjRef::deep_copy (tc2, source, dest, env);
                          value1 = (char *) value1 + size;
                          value2 = (char *) value2 + size;
                        }
                      break;
                    case tk_struct:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Struct::deep_copy (tc2, source, dest, env);
                          value1 = (char *) value1 + size;
                          value2 = (char *) value2 + size;
                        }
                      break;
                    case tk_union:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Union::deep_copy (tc2, source, dest, env);
                          value1 = (char *) value1 + size;
                          value2 = (char *) value2 + size;
                        }
                      break;
                    case tk_string:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Any::deep_copy (tc2, source, dest, env);
                          value1 = (char *) value1 + size;
                          value2 = (char *) value2 + size;
                        }
                      retval = TAO_Marshal_String::deep_copy (tc2, source, dest, env);
                      break;
                    case tk_sequence:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Sequence::deep_copy (tc2, source, dest, env);
                          value1 = (char *) value1 + size;
                          value2 = (char *) value2 + size;
                        }
                      break;
                    case tk_array:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Array::deep_copy (tc2, source, dest, env);
                          value1 = (char *) value1 + size;
                          value2 = (char *) value2 + size;
                        }
                      break;
                    case tk_alias:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Alias::deep_copy (tc2, source, dest, env);
                          value1 = (char *) value1 + size;
                          value2 = (char *) value2 + size;
                        }
                      break;
                    case tk_except:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Except::deep_copy (tc2, source, dest, env);
                          value1 = (char *) value1 + size;
                          value2 = (char *) value2 + size;
                        }
                      break;
                    case tk_wstring:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_WString::deep_copy (tc2, source, dest, env);
                          value1 = (char *) value1 + size;
                          value2 = (char *) value2 + size;
                        }
                      break;
                    default:
                      retval = CORBA_TypeCode::TRAVERSE_STOP;
                      break;
                    } // end of switch
                  //              CORBA_release (tc2);
                  if (retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
		    return CORBA_TypeCode::TRAVERSE_CONTINUE;
                  else
                    {
                      // error exit
                      env.exception (new CORBA_MARSHAL (COMPLETED_NO));
                      dmsg ("marshaling TAO_Marshal_Sequence::deep_copy detected error");
                      return CORBA_TypeCode::TRAVERSE_STOP;
                    }
                }
              else 
                {
                  // error exit
                  //              CORBA_release (tc2);
                  env.exception (new CORBA_NO_MEMORY (COMPLETED_MAYBE));
                  dmsg ("marshaling TAO_Marshal_Sequence::deep_copy detected error");
                  return CORBA_TypeCode::TRAVERSE_STOP;
                }
            } 
          else // exception computing size
            {
              //              CORBA_release (tc2);
              dmsg ("marshaling TAO_Marshal_Sequence::deep_copy detected error");
              return CORBA_TypeCode::TRAVERSE_STOP;
            }
        } 
      else // exception computing content type
        {
          dmsg ("marshaling TAO_Marshal_Sequence::deep_copy detected error");
          return CORBA_TypeCode::TRAVERSE_STOP;
        }
    }
  else // no typecode
    {
      env.exception (new CORBA_BAD_TYPECODE (COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Struct::deep_copy detected error");
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
  return retval;
}

// deep_copy for Array
CORBA_TypeCode::traverse_status
TAO_Marshal_Array::deep_copy (CORBA_TypeCode_ptr  tc,
                              const void *source,
                              const void *dest,
                              CORBA_Environment &env)
{
  CORBA_TypeCode::traverse_status retval =
    CORBA_TypeCode::TRAVERSE_CONTINUE;  // return status
  CORBA_TypeCode_ptr    tc2;  // typecode of the element
  size_t  size; // size of element
  CORBA_ULong  bounds;
  CDR stream;  // used only to access the marshal_object factory

  // Rely on binary format of sequences -- all are the same
  // except for the type pointed to by "buffer"

  if (tc)
    {
      bounds = tc->length (env);
      if (env.exception () == 0)
        {
          // get element typecode
          tc2 = tc->content_type (env);  
          if (env.exception () == 0)
            {
              // get the size of the element type
              size = tc2->size (env);
              if (env.exception () == 0)
                {
                  switch (tc2->_kind)
                    {
                    case tk_null:
                    case tk_void:
                      //                      CORBA_release (tc2);
                      return CORBA_TypeCode::TRAVERSE_CONTINUE;
                    case tk_short:
                    case tk_ushort:
                    case tk_long:
                    case tk_ulong:
                    case tk_float:
                    case tk_double:
                    case tk_longlong:
                    case tk_ulonglong:
                    case tk_boolean:
                    case tk_char:
                    case tk_octet:
                    case tk_longdouble:
                    case tk_wchar:
                    case tk_enum:
                      // just do a memcpy rather than copying each element
                      ACE_OS::memcpy ((char *) dest, (char *) source, size*bounds);
                      //                      CORBA_release (tc2);
                      return CORBA_TypeCode::TRAVERSE_CONTINUE;
                      // handle all aggregate types here
                    case tk_any:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Any::deep_copy (tc2, source, dest, env);
                          source = (char *) source + size;
                          dest = (char *) dest + size;
                        }
                      break;
                    case tk_TypeCode:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_TypeCode::deep_copy (tc2, source, dest, env);
                          source = (char *) source + size;
                          dest = (char *) dest + size;
                        }
                      break;
                    case tk_Principal:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Principal::deep_copy (tc2, source, dest, env);
                          source = (char *) source + size;
                          dest = (char *) dest + size;
                        }
                      break;
                    case tk_objref:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_ObjRef::deep_copy (tc2, source, dest, env);
                          source = (char *) source + size;
                          dest = (char *) dest + size;
                        }
                      break;
                    case tk_struct:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Struct::deep_copy (tc2, source, dest, env);
                          source = (char *) source + size;
                          dest = (char *) dest + size;
                        }
                      break;
                    case tk_union:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Union::deep_copy (tc2, source, dest, env);
                          source = (char *) source + size;
                          dest = (char *) dest + size;
                        }
                      break;
                    case tk_string:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Any::deep_copy (tc2, source, dest, env);
                          source = (char *) source + size;
                          dest = (char *) dest + size;
                        }
                      retval = TAO_Marshal_String::deep_copy (tc2, source, dest, env);
                      break;
                    case tk_sequence:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Sequence::deep_copy (tc2, source, dest, env);
                          source = (char *) source + size;
                          dest = (char *) dest + size;
                        }
                      break;
                    case tk_array:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Array::deep_copy (tc2, source, dest, env);
                          source = (char *) source + size;
                          dest = (char *) dest + size;
                        }
                      break;
                    case tk_alias:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Alias::deep_copy (tc2, source, dest, env);
                          source = (char *) source + size;
                          dest = (char *) dest + size;
                        }
                      break;
                    case tk_except:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_Except::deep_copy (tc2, source, dest, env);
                          source = (char *) source + size;
                          dest = (char *) dest + size;
                        }
                      break;
                    case tk_wstring:
                      while (bounds-- && retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
                        {
                          retval = TAO_Marshal_WString::deep_copy (tc2, source, dest, env);
                          source = (char *) source + size;
                          dest = (char *) dest + size;
                        }
                      break;
                    default:
                      retval = CORBA_TypeCode::TRAVERSE_STOP;
                      break;
                    } // end of switch
                  //              CORBA_release (tc2);
                  if (retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
		    return CORBA_TypeCode::TRAVERSE_CONTINUE;
                  else
                    {
                      // error exit
                      env.exception (new CORBA_MARSHAL (COMPLETED_NO));
                      dmsg ("marshaling TAO_Marshal_Sequence::deep_copy detected error");
                      return CORBA_TypeCode::TRAVERSE_STOP;
                    }
                } // no exception computing size
              else
		//              CORBA_release (tc2);
		return CORBA_TypeCode::TRAVERSE_STOP;
            } // no exception computing content type
          else
	    return CORBA_TypeCode::TRAVERSE_STOP;
        } // bounds are fine
      else
	return CORBA_TypeCode::TRAVERSE_STOP;
    }
  else // no typecode
    {
      env.exception (new CORBA_BAD_TYPECODE (COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Struct::deep_copy detected error");
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
}

// deep_copy alias
CORBA_TypeCode::traverse_status
TAO_Marshal_Alias::deep_copy (CORBA_TypeCode_ptr  tc,
                              const void *source,
                              const void *dest,
                              CORBA_Environment &env)
{
  CORBA_TypeCode_ptr    tc2;  // typecode of the aliased type
  CDR stream;  // to access the marshal object
  CORBA_TypeCode::traverse_status   retval =
    CORBA_TypeCode::TRAVERSE_CONTINUE; // status of deep_copy operation 

  if (tc)
    {
      // get element type
      tc2 = tc->content_type (env);
      if (env.exception () == 0)
        {
          // switch on the data type and handle the cases for primitives here for
          // efficiency
          switch (tc2->_kind)
            {
            case tk_null:
            case tk_void:
              //              CORBA_release (tc2);
              return CORBA_TypeCode::TRAVERSE_CONTINUE;
            case tk_short:
            case tk_ushort:
              *(CORBA_Short *) dest = *(CORBA_Short *) source;
              //              CORBA_release (tc2);
              return CORBA_TypeCode::TRAVERSE_CONTINUE;
            case tk_long:
            case tk_ulong:
            case tk_float:
            case tk_enum:
              *(CORBA_Long *) dest = *(CORBA_Long *) source;
              //              CORBA_release (tc2);
              return CORBA_TypeCode::TRAVERSE_CONTINUE;
            case tk_double:
            case tk_longlong:
            case tk_ulonglong:
              *(CORBA_LongLong *) dest = *(CORBA_LongLong *) source;
              //              CORBA_release (tc2);
              return CORBA_TypeCode::TRAVERSE_CONTINUE;
            case tk_boolean:
              *(CORBA_Boolean *) dest = *(CORBA_Boolean *) source;
              //              CORBA_release (tc2);
              return CORBA_TypeCode::TRAVERSE_CONTINUE;
            case tk_char:
            case tk_octet:
              *(CORBA_Char *) dest = *(CORBA_Char *) source;
              //              CORBA_release (tc2);
              return CORBA_TypeCode::TRAVERSE_CONTINUE;
            case tk_longdouble:
              *(CORBA_LongDouble *) dest = *(CORBA_LongDouble *) source;
              //              CORBA_release (tc2);
              return CORBA_TypeCode::TRAVERSE_CONTINUE;
            case tk_wchar:
              *(CORBA_WChar *) dest = *(CORBA_WChar *) source;
              //              CORBA_release (tc2);
              return CORBA_TypeCode::TRAVERSE_CONTINUE;
            case tk_any:
              retval = TAO_Marshal_Any::deep_copy (tc2, source, dest, env);
              break;
            case tk_TypeCode:
              retval = TAO_Marshal_TypeCode::deep_copy (tc2, source, dest, env);
              break;
            case tk_Principal:
              retval = TAO_Marshal_Principal::deep_copy (tc2, source, dest, env);
              break;
            case tk_objref:
              retval = TAO_Marshal_ObjRef::deep_copy (tc2, source, dest, env);
              break;
            case tk_struct:
              retval = TAO_Marshal_Struct::deep_copy (tc2, source, dest, env);
              break;
            case tk_union:
              retval = TAO_Marshal_Union::deep_copy (tc2, source, dest, env);
              break;
            case tk_string:
              retval = TAO_Marshal_String::deep_copy (tc2, source, dest, env);
              break;
            case tk_sequence:
              retval = TAO_Marshal_Sequence::deep_copy (tc2, source, dest, env);
              break;
            case tk_array:
              retval = TAO_Marshal_Array::deep_copy (tc2, source, dest, env);
              break;
            case tk_alias:
              retval = TAO_Marshal_Alias::deep_copy (tc2, source, dest, env);
              break;
            case tk_except:
              retval = TAO_Marshal_Except::deep_copy (tc2, source, dest, env);
              break;
            case tk_wstring:
              retval = TAO_Marshal_WString::deep_copy (tc2, source, dest, env);
              break;
            default:
              // anything else is an error
              retval = CORBA_TypeCode::TRAVERSE_STOP;
            }
          //      CORBA_release (tc2);
          if (retval == CORBA_TypeCode::TRAVERSE_CONTINUE) 
	    return CORBA_TypeCode::TRAVERSE_CONTINUE;
          else 
            {
              env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
              dmsg ("TAO_Marshal_Alias::decode detected error");
              return CORBA_TypeCode::TRAVERSE_STOP;
            }
        }
      else // exception getting content_type
	return CORBA_TypeCode::TRAVERSE_STOP;
    }
  else // no typecode
    {
      env.exception (new CORBA_BAD_TYPECODE (COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Struct::deep_copy detected error");
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
}

// deep_copy structs
CORBA_TypeCode::traverse_status
TAO_Marshal_Except::deep_copy (CORBA_TypeCode_ptr  tc,
                               const void *source,
                               const void *dest,
                               CORBA_Environment &env)
{
  CORBA_TypeCode::traverse_status retval = CORBA_TypeCode::TRAVERSE_CONTINUE;
  CORBA_TypeCode_ptr param;
  CORBA_Long size, alignment;
  CDR stream;

  if (tc)
    {
      // Exceptions in memory have a "hidden" typecode up front, used
      // to ensure that memory is appropriately freed and to hold the
      // exception ID.  We just copy that typecode

      *(CORBA_TypeCode_ptr *) dest = *(CORBA_TypeCode_ptr *) source;
      (void) (*(CORBA_TypeCode_ptr *) dest)->AddRef ();

      // compute the number of fields in the struct
      int member_count = tc->member_count (env);
      if (env.exception () == 0)
        {
          for (int i = 0;
	       i < member_count 
		 && retval == CORBA_TypeCode::TRAVERSE_CONTINUE;
	       i++)
            {
              // get the typecode for the ith field
              param = tc->member_type (i, env);
              if (env.exception () == 0)
                {
                  // get the size of the field
                  size = param->size (env);
                  if (env.exception () == 0)
                    {
                      // get the alignment of the field
                      alignment = param->alignment (env);
                      if (env.exception () == 0)
                        {
                          switch (param->_kind)
                            {
                            case tk_null:
                            case tk_void:
                              break;
                            case tk_short:
                            case tk_ushort:
                              *(CORBA_Short *) dest = *(CORBA_Short *) source;
                              break;
                            case tk_long:
                            case tk_ulong:
                            case tk_float:
                            case tk_enum:
                              *(CORBA_Long *) dest = *(CORBA_Long *) source;
                              break;
                            case tk_double:
                            case tk_longlong:
                            case tk_ulonglong:
                              *(CORBA_LongLong *) dest = *(CORBA_LongLong *) source;
                              break;
                            case tk_boolean:
                              *(CORBA_Boolean *) dest = *(CORBA_Boolean *) source;
                              break;
                            case tk_char:
                            case tk_octet:
                              *(CORBA_Char *) dest = *(CORBA_Char *) source;
                              break;
                            case tk_longdouble:
                              *(CORBA_LongDouble *) dest = *(CORBA_LongDouble *) source;
                              break;
                            case tk_wchar:
                              *(CORBA_WChar *) dest = *(CORBA_WChar *) source;
                              break;
                            case tk_any:
                              retval = TAO_Marshal_Any::deep_copy (param, source, dest, env);
                              break;
                            case tk_TypeCode:
                              retval = TAO_Marshal_TypeCode::deep_copy (param, source, dest, env);
                              break;
                            case tk_Principal:
                              retval = TAO_Marshal_Principal::deep_copy (param, source, dest, env);
                              break;
                            case tk_objref:
                              retval = TAO_Marshal_ObjRef::deep_copy (param, source, dest, env);
                              break;
                            case tk_struct:
                              retval = TAO_Marshal_Struct::deep_copy (param, source, dest, env);
                              break;
                            case tk_union:
                              retval = TAO_Marshal_Union::deep_copy (param, source, dest, env);
                              break;
                            case tk_string:
                              retval = TAO_Marshal_String::deep_copy (param, source, dest, env);
                              break;
                            case tk_sequence:
                              retval = TAO_Marshal_Sequence::deep_copy (param, source, dest, env);
                              break;
                            case tk_array:
                              retval = TAO_Marshal_Array::deep_copy (param, source, dest, env);
                              break;
                            case tk_alias:
                              retval = TAO_Marshal_Alias::deep_copy (param, source, dest, env);
                              break;
                            case tk_except:
                              retval = TAO_Marshal_Except::deep_copy (param, source, dest, env);
                              break;
                            case tk_wstring:
                              retval = TAO_Marshal_WString::deep_copy (param, source, dest, env);
                              break;
                            default:
                              retval = CORBA_TypeCode::TRAVERSE_STOP;
                            } // end of switch
                          source = (char *) source + size;
                          dest = (char *) dest + size;
                        }  
                      else  // exception computing alignment 
                        {
                          dmsg ("TAO_Marshal_Struct::deep_copy detected error");
                          return CORBA_TypeCode::TRAVERSE_STOP;
                        }
                    } 
                  else  // exception computing size 
                    {
                      dmsg ("TAO_Marshal_Struct::deep_copy detected error");
                      return CORBA_TypeCode::TRAVERSE_STOP;
                    }
                } 
              else  // exception computing typecode
                {
                  dmsg ("TAO_Marshal_Struct::deep_copy detected error");
                  return CORBA_TypeCode::TRAVERSE_STOP;
                }
            } // end of loop
          if (retval == CORBA_TypeCode::TRAVERSE_CONTINUE)
	    return CORBA_TypeCode::TRAVERSE_CONTINUE;
          else
            {
              env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));
              dmsg ("TAO_Marshal_Struct::deep_copy detected error");
              return CORBA_TypeCode::TRAVERSE_STOP;
            }
        } 
      else // exception getting member count 
        {
          dmsg ("TAO_Marshal_Struct::deep_copy detected error");
          return CORBA_TypeCode::TRAVERSE_STOP;
        }
    }
  else // no typecode
    {
      env.exception (new CORBA_BAD_TYPECODE (COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Struct::deep_copy detected error");
      return CORBA_TypeCode::TRAVERSE_STOP;
    }
}
