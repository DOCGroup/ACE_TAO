// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    deep_free.cpp
//
// = DESCRIPTION
//   Code for deep_free
//   The original code had a single static deep_free function that called
//   traverse to interpret the data types. This version defines a static method
//   "deep_free" on each class and avoids calling traverse.
//
//   Helper routine for "Any" destructor.
//
//   This frees all the memory pointed to by any given value held inside
//   of an "Any".  For most data types it does nothing, since most data
//   types don't hold any memory.  For a few, it calls other deep_free methods
//   to free the memory
//

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

#if	defined (HAVE_WIDEC_H)
#		include <widec.h>
#else
extern "C" 
{
  u_int wslen (const CORBA::WChar *);
  CORBA::WChar *wscpy (CORBA::WChar *, const CORBA::WChar *);
}
#endif /* HAVE_WIDEC_H */

extern CORBA::TypeCode TC_opaque;

  // Deep copy from "source" to "dest" ... this code "knows" a bit
  // about representations, verify it when porting to oddball
  // platforms with non-IEEE floating point values or atypical byte
  // and word sizes.
  //


CORBA::TypeCode::traverse_status
DEEP_FREE (CORBA::TypeCode_ptr  param,
	   const void *source,
	   const void *dest,
	   CORBA::Environment &env)
{
  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;

  switch (param->_kind)
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
    case CORBA::tk_short:
    case CORBA::tk_ushort:
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_float:
    case CORBA::tk_enum:
    case CORBA::tk_double:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
    case CORBA::tk_boolean:
    case CORBA::tk_char:
    case CORBA::tk_octet:
    case CORBA::tk_longdouble:
    case CORBA::tk_wchar:
      break;
    case CORBA::tk_any:
      retval = TAO_Marshal_Any::deep_free (param, source, dest, env);
      break;
    case CORBA::tk_TypeCode:
      retval = TAO_Marshal_TypeCode::deep_free (param, source, dest, env);
      break;
    case CORBA::tk_Principal:
      retval = TAO_Marshal_Principal::deep_free (param, source, dest, env);
      break;
    case CORBA::tk_objref:
      retval = TAO_Marshal_ObjRef::deep_free (param, source, dest, env);
      break;
    case CORBA::tk_struct:
      retval = TAO_Marshal_Struct::deep_free (param, source, dest, env);
      break;
    case CORBA::tk_union:
      retval = TAO_Marshal_Union::deep_free (param, source, dest, env);
      break;
    case CORBA::tk_string:
      retval = TAO_Marshal_String::deep_free (param, source, dest, env);
      break;
    case CORBA::tk_sequence:
      retval = TAO_Marshal_Sequence::deep_free (param, source, dest, env);
      break;
    case CORBA::tk_array:
      retval = TAO_Marshal_Array::deep_free (param, source, dest, env);
      break;
    case CORBA::tk_alias:
      retval = TAO_Marshal_Alias::deep_free (param, source, dest, env);
      break;
    case CORBA::tk_except:
      retval = TAO_Marshal_Except::deep_free (param, source, dest, env);
      break;
    case CORBA::tk_wstring:
      retval = TAO_Marshal_WString::deep_free (param, source, dest, env);
      break;
    default:
      retval = CORBA::TypeCode::TRAVERSE_STOP;
    } // end of switch
  return retval;
}

// deep copy for primitives
CORBA::TypeCode::traverse_status
TAO_Marshal_Primitive::deep_free (CORBA::TypeCode_ptr  tc,
				  const void *,
				  const void *,
				  CORBA::Environment &env)
{
  CORBA::TCKind my_kind;

  if (tc) 
    {
      my_kind = tc->kind (env);

      if (env.exception () == 0)
	{
	  switch (my_kind) 
	    {
	    case CORBA::tk_null:
	    case CORBA::tk_void:
	    case CORBA::tk_char:
	    case CORBA::tk_octet:
	    case CORBA::tk_short:
	    case CORBA::tk_ushort:
	    case CORBA::tk_wchar:
	    case CORBA::tk_long:
	    case CORBA::tk_ulong:
	    case CORBA::tk_float:
	    case CORBA::tk_enum:
	    case CORBA::tk_longlong:
	    case CORBA::tk_ulonglong:
	    case CORBA::tk_double:
	    case CORBA::tk_longdouble:
	    case CORBA::tk_boolean:
	      return CORBA::TypeCode::TRAVERSE_CONTINUE;
	    default:
	      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_MAYBE));
	      dmsg ("TAO_Marshal_Primitive::deep_free detected error");
	      return CORBA::TypeCode::TRAVERSE_STOP;
	    }
	}
      else
	{
	  dmsg ("TAO_Marshal_Primitive::deep_free detected error");
	  return CORBA::TypeCode::TRAVERSE_STOP;
	}
    }
  else 
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_MAYBE) );
      dmsg ("TAO_Marshal_Primitive::deep_free detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// deep_free structs
CORBA::TypeCode::traverse_status
TAO_Marshal_Struct::deep_free (CORBA::TypeCode_ptr  tc,
			       const void *source,
			       const void *dest,
			       CORBA::Environment &env)
{
  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::TypeCode_ptr param;
  CORBA::Long size, alignment;
  CDR stream;

  if (tc)
    {
      // compute the number of fields in the struct
      int member_count = tc->member_count (env);
      if (env.exception () == 0)
	{
	  for (int i = 0; i < member_count && retval ==
		CORBA::TypeCode::TRAVERSE_CONTINUE; i++)
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
			    case CORBA::tk_null:
			    case CORBA::tk_void:
			    case CORBA::tk_short:
			    case CORBA::tk_ushort:
			    case CORBA::tk_long:
			    case CORBA::tk_ulong:
			    case CORBA::tk_float:
			    case CORBA::tk_enum:
			    case CORBA::tk_double:
			    case CORBA::tk_longlong:
			    case CORBA::tk_ulonglong:
			    case CORBA::tk_boolean:
			    case CORBA::tk_char:
			    case CORBA::tk_octet:
			    case CORBA::tk_longdouble:
			    case CORBA::tk_wchar:
			      break;
			    case CORBA::tk_any:
			      retval = TAO_Marshal_Any::deep_free (param, source, dest, env);
			      break;
			    case CORBA::tk_TypeCode:
			      retval = TAO_Marshal_TypeCode::deep_free (param, source, dest, env);
			      break;
			    case CORBA::tk_Principal:
			      retval = TAO_Marshal_Principal::deep_free (param, source, dest, env);
			      break;
			    case CORBA::tk_objref:
			      retval = TAO_Marshal_ObjRef::deep_free (param, source, dest, env);
			      break;
			    case CORBA::tk_struct:
			      retval = TAO_Marshal_Struct::deep_free (param, source, dest, env);
			      break;
			    case CORBA::tk_union:
			      retval = TAO_Marshal_Union::deep_free (param, source, dest, env);
			      break;
			    case CORBA::tk_string:
			      retval = TAO_Marshal_String::deep_free (param, source, dest, env);
			      break;
			    case CORBA::tk_sequence:
			      retval = TAO_Marshal_Sequence::deep_free (param, source, dest, env);
			      break;
			    case CORBA::tk_array:
			      retval = TAO_Marshal_Array::deep_free (param, source, dest, env);
			      break;
			    case CORBA::tk_alias:
			      retval = TAO_Marshal_Alias::deep_free (param, source, dest, env);
			      break;
			    case CORBA::tk_except:
			      retval = TAO_Marshal_Except::deep_free (param, source, dest, env);
			      break;
			    case CORBA::tk_wstring:
			      retval = TAO_Marshal_WString::deep_free (param, source, dest, env);
			      break;
			    default:
			      retval = CORBA::TypeCode::TRAVERSE_STOP;
			    } // end of switch
			  source = (char *)source + size;
			}  
		      else  // exception computing alignment 
			{
			  dmsg ("TAO_Marshal_Struct::deep_free detected error");
			  return CORBA::TypeCode::TRAVERSE_STOP;
			}
		    } 
		  else  // exception computing size 
		    {
		      dmsg ("TAO_Marshal_Struct::deep_free detected error");
		      return CORBA::TypeCode::TRAVERSE_STOP;
		    }
		} 
	      else  // exception computing typecode
		{
		  dmsg ("TAO_Marshal_Struct::deep_free detected error");
		  return CORBA::TypeCode::TRAVERSE_STOP;
		}
	    } // end of loop
	  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
	    return CORBA::TypeCode::TRAVERSE_CONTINUE;
	  else
	    {
	      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
	      dmsg ("TAO_Marshal_Struct::deep_free detected error");
	      return CORBA::TypeCode::TRAVERSE_STOP;
	    }
	} 
      else // exception getting member count 
	{
	  dmsg ("TAO_Marshal_Struct::deep_free detected error");
	  return CORBA::TypeCode::TRAVERSE_STOP;
	}
    }
  else // no typecode
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Struct::deep_free detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// deep_free for union
CORBA::TypeCode::traverse_status
TAO_Marshal_Union::deep_free (CORBA::TypeCode_ptr  tc,
			      const void *data,
			      const void *data2,
			      CORBA::Environment &env)
{
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
      // deep_free the discriminator value
      retval = DEEP_FREE (discrim_tc, data, data2, env);
      if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
	{
	  discrim_size_with_pad = tc->TAO_discrim_pad_size (env);
	  if (env.exception () == 0)
	    {
	      discrim_val = data; // save the pointer to the discriminator
				  // value 
	      // move the pointer to point to the actual value
	      data = (char *)data + discrim_size_with_pad;
	      data2 = (char *)data2 + discrim_size_with_pad;
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
				  if (*(CORBA::Short *)member_label->value () ==
				      *(CORBA::Short *)discrim_val)
				    discrim_matched = CORBA::B_TRUE;
				  break;
				case CORBA::tk_long:
				case CORBA::tk_ulong:
				case CORBA::tk_enum:
				  if (*(CORBA::ULong *)member_label->value () ==
				      *(CORBA::ULong *)discrim_val)
				      discrim_matched = CORBA::B_TRUE;
				  break;
				case CORBA::tk_char:
				  if (*(CORBA::Char *)member_label->value () ==
				      *(CORBA::Char *)discrim_val)
				    discrim_matched = CORBA::B_TRUE;
				  break;
				case CORBA::tk_wchar:
				  if (*(CORBA::WChar *)member_label->value () ==
				      *(CORBA::WChar *)discrim_val)
				    discrim_matched = CORBA::B_TRUE;
				  break;
				case CORBA::tk_boolean:
				  if (*(CORBA::Boolean *)member_label->value () ==
				      *(CORBA::Boolean *)discrim_val)
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
				    {
				      // have we reached the default label?, if so,
				      // save a handle to the typecode for the default
				      default_tc = member_tc;
				    }
				  if (discrim_matched)
				    // marshal according to the matched typecode
				    return DEEP_FREE (member_tc, data,
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
			} // end of while
		      // we are here only if there was no match
		      if (default_tc)
			return DEEP_FREE (default_tc, data, data2, env);
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

// deep_free for Sequence
CORBA::TypeCode::traverse_status
TAO_Marshal_Sequence::deep_free (CORBA::TypeCode_ptr  tc,
				 const void *source,
				 const void *dest,
				 CORBA::Environment &env)
{
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;  // return status
  CORBA::TypeCode_ptr	tc2;  // typecode of the element
  size_t  size; // size of element
  CORBA::ULong  bounds;
  char *value1;
  CORBA::OctetSeq          *src;
  CDR stream;  // used only to access the marshal_object factory

  // Rely on binary format of sequences -- all are the same
  // except for the type pointed to by "buffer"

  if (tc)
    {
      src = (CORBA::OctetSeq *) source;

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

		  value1 = (char *)src->buffer;
		  switch (tc2->_kind)
		    {
		    case CORBA::tk_null:
		    case CORBA::tk_void:
		    case CORBA::tk_short:
		    case CORBA::tk_ushort:
		    case CORBA::tk_long:
		    case CORBA::tk_ulong:
		    case CORBA::tk_float:
		    case CORBA::tk_double:
		    case CORBA::tk_longlong:
		    case CORBA::tk_ulonglong:
		    case CORBA::tk_boolean:
		    case CORBA::tk_char:
		    case CORBA::tk_octet:
		    case CORBA::tk_longdouble:
		    case CORBA::tk_wchar:
		    case CORBA::tk_enum:
		      delete [] src->buffer;
		      //		      CORBA::release (tc2);
		      return CORBA::TypeCode::TRAVERSE_CONTINUE;

		      // handle all aggregate types here
		    case CORBA::tk_any:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Any::deep_free (tc2, source, dest, env);
			  value1 = (char *)value1 + size;
			}
		      break;
		    case CORBA::tk_TypeCode:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_TypeCode::deep_free (tc2, source, dest, env);
			  value1 = (char *)value1 + size;
			}
		      break;
		    case CORBA::tk_Principal:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Principal::deep_free (tc2, source, dest, env);
			  value1 = (char *)value1 + size;
			}
		      break;
		    case CORBA::tk_objref:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_ObjRef::deep_free (tc2, source, dest, env);
			  value1 = (char *)value1 + size;
			}
		      break;
		    case CORBA::tk_struct:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Struct::deep_free (tc2, source, dest, env);
			  value1 = (char *)value1 + size;
			}
		      break;
		    case CORBA::tk_union:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Union::deep_free (tc2, source, dest, env);
			  value1 = (char *)value1 + size;
			}
		      break;
		    case CORBA::tk_string:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Any::deep_free (tc2, source, dest, env);
			  value1 = (char *)value1 + size;
			}
		      retval = TAO_Marshal_String::deep_free (tc2, source, dest, env);
		      break;
		    case CORBA::tk_sequence:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Sequence::deep_free (tc2, source, dest, env);
			  value1 = (char *)value1 + size;
			}
		      break;
		    case CORBA::tk_array:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Array::deep_free (tc2, source, dest, env);
			  value1 = (char *)value1 + size;
			}
		      break;
		    case CORBA::tk_alias:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Alias::deep_free (tc2, source, dest, env);
			  value1 = (char *)value1 + size;
			}
		      break;
		    case CORBA::tk_except:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Except::deep_free (tc2, source, dest, env);
			  value1 = (char *)value1 + size;
			}
		      break;
		    case CORBA::tk_wstring:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_WString::deep_free (tc2, source, dest, env);
			  value1 = (char *)value1 + size;
			}
		      break;
		    default:
		      retval = CORBA::TypeCode::TRAVERSE_STOP;
		      break;
		    } // end of switch
		  //		  CORBA::release (tc2);
		  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
		  else
		    {
		      // error exit
		      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
		      dmsg ("marshaling TAO_Marshal_Sequence::deep_free detected error");
		      return CORBA::TypeCode::TRAVERSE_STOP;
		    }
	    } 
	  else // exception computing size
	    {
	      //	      CORBA::release (tc2);
	      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_MAYBE));
	      dmsg ("marshaling TAO_Marshal_Sequence::deep_free detected error");
	      return CORBA::TypeCode::TRAVERSE_STOP;
	    }
	} 
      else // exception computing content type
	{
	  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_MAYBE));
	  dmsg ("marshaling TAO_Marshal_Sequence::deep_free detected error");
	  return CORBA::TypeCode::TRAVERSE_STOP;
	}
    }
  else // no typecode
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Struct::deep_free detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// deep_free for Array
CORBA::TypeCode::traverse_status
TAO_Marshal_Array::deep_free (CORBA::TypeCode_ptr  tc,
			      const void *source,
			      const void *dest,
			      CORBA::Environment &env)
{
  CORBA::TypeCode::traverse_status retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE;  // return status
  CORBA::TypeCode_ptr	tc2;  // typecode of the element
  size_t  size; // size of element
  CORBA::ULong  bounds;
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
		    case CORBA::tk_null:
		    case CORBA::tk_void:
		    case CORBA::tk_short:
		    case CORBA::tk_ushort:
		    case CORBA::tk_long:
		    case CORBA::tk_ulong:
		    case CORBA::tk_float:
		    case CORBA::tk_double:
		    case CORBA::tk_longlong:
		    case CORBA::tk_ulonglong:
		    case CORBA::tk_boolean:
		    case CORBA::tk_char:
		    case CORBA::tk_octet:
		    case CORBA::tk_longdouble:
		    case CORBA::tk_wchar:
		    case CORBA::tk_enum:
		      //		      CORBA::release (tc2);
		      return CORBA::TypeCode::TRAVERSE_CONTINUE;

		      // handle all aggregate types here
		    case CORBA::tk_any:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Any::deep_free (tc2, source, dest, env);
			  source = (char *)source + size;
			}
		      break;
		    case CORBA::tk_TypeCode:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_TypeCode::deep_free (tc2, source, dest, env);
			  source = (char *)source + size;
			}
		      break;
		    case CORBA::tk_Principal:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Principal::deep_free (tc2, source, dest, env);
			  source = (char *)source + size;
			}
		      break;
		    case CORBA::tk_objref:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_ObjRef::deep_free (tc2, source, dest, env);
			  source = (char *)source + size;
			}
		      break;
		    case CORBA::tk_struct:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Struct::deep_free (tc2, source, dest, env);
			  source = (char *)source + size;
			}
		      break;
		    case CORBA::tk_union:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Union::deep_free (tc2, source, dest, env);
			  source = (char *)source + size;
			}
		      break;
		    case CORBA::tk_string:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_String::deep_free (tc2, source, dest, env);
			  source = (char *)source + size;
			}
		      break;
		    case CORBA::tk_sequence:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Sequence::deep_free (tc2, source, dest, env);
			  source = (char *)source + size;
			}
		      break;
		    case CORBA::tk_array:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Array::deep_free (tc2, source, dest, env);
			  source = (char *)source + size;
			}
		      break;
		    case CORBA::tk_alias:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Alias::deep_free (tc2, source, dest, env);
			  source = (char *)source + size;
			}
		      break;
		    case CORBA::tk_except:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_Except::deep_free (tc2, source, dest, env);
			  source = (char *)source + size;
			}
		      break;
		    case CORBA::tk_wstring:
		      while (bounds-- && retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  retval = TAO_Marshal_WString::deep_free (tc2, source, dest, env);
			  source = (char *)source + size;
			}
		      break;
		    default:
		      retval = CORBA::TypeCode::TRAVERSE_STOP;
		      break;
		    } // end of switch
		  //		  CORBA::release (tc2);
 		  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE)
		    return CORBA::TypeCode::TRAVERSE_CONTINUE;
		  else
		    {
		      // error exit
		      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
		      dmsg ("marshaling TAO_Marshal_Sequence::deep_free detected error");
		      return CORBA::TypeCode::TRAVERSE_STOP;
		    }
		} // no exception computing size
	      else
		//		  CORBA::release (tc2);
		return CORBA::TypeCode::TRAVERSE_STOP;
	    } 
	  else // exception computing content type
	    return CORBA::TypeCode::TRAVERSE_STOP;
	} 
      else // exception getting bounds
	return CORBA::TypeCode::TRAVERSE_STOP;
    }
  else // no typecode
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Struct::deep_free detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// deep_free alias
CORBA::TypeCode::traverse_status
TAO_Marshal_Alias::deep_free (CORBA::TypeCode_ptr  tc,
			      const void *source,
			      const void *dest,
			      CORBA::Environment &env)
{
  CORBA::TypeCode_ptr	tc2;  // typecode of the aliased type
  CDR stream;  // to access the marshal object
  CORBA::TypeCode::traverse_status   retval =
    CORBA::TypeCode::TRAVERSE_CONTINUE; // status of encode operation 

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
	    case CORBA::tk_null:
	    case CORBA::tk_void:
	    case CORBA::tk_short:
	    case CORBA::tk_ushort:
	    case CORBA::tk_long:
	    case CORBA::tk_ulong:
	    case CORBA::tk_float:
	    case CORBA::tk_enum:
	    case CORBA::tk_double:
	    case CORBA::tk_longlong:
	    case CORBA::tk_ulonglong:
	    case CORBA::tk_boolean:
	    case CORBA::tk_char:
	    case CORBA::tk_octet:
	    case CORBA::tk_longdouble:
	    case CORBA::tk_wchar:
	      //	      CORBA::release (tc2);
	      return CORBA::TypeCode::TRAVERSE_CONTINUE;
	    case CORBA::tk_any:
	      retval = TAO_Marshal_Any::deep_free (tc2, source, dest, env);
	      break;
	    case CORBA::tk_TypeCode:
	      retval = TAO_Marshal_TypeCode::deep_free (tc2, source, dest, env);
	      break;
	    case CORBA::tk_Principal:
	      retval = TAO_Marshal_Principal::deep_free (tc2, source, dest, env);
	      break;
	    case CORBA::tk_objref:
	      retval = TAO_Marshal_ObjRef::deep_free (tc2, source, dest, env);
	      break;
	    case CORBA::tk_struct:
	      retval = TAO_Marshal_Struct::deep_free (tc2, source, dest, env);
	      break;
	    case CORBA::tk_union:
	      retval = TAO_Marshal_Union::deep_free (tc2, source, dest, env);
	      break;
	    case CORBA::tk_string:
	      retval = TAO_Marshal_String::deep_free (tc2, source, dest, env);
	      break;
	    case CORBA::tk_sequence:
	      retval = TAO_Marshal_Sequence::deep_free (tc2, source, dest, env);
	      break;
	    case CORBA::tk_array:
	      retval = TAO_Marshal_Array::deep_free (tc2, source, dest, env);
	      break;
	    case CORBA::tk_alias:
	      retval = TAO_Marshal_Alias::deep_free (tc2, source, dest, env);
	      break;
	    case CORBA::tk_except:
	      retval = TAO_Marshal_Except::deep_free (tc2, source, dest, env);
	      break;
	    case CORBA::tk_wstring:
	      retval = TAO_Marshal_WString::deep_free (tc2, source, dest, env);
	      break;
	    default:
	      // anything else is an error
	      retval = CORBA::TypeCode::TRAVERSE_STOP;
	    }
	  //	  CORBA::release (tc2);
	  if (retval == CORBA::TypeCode::TRAVERSE_CONTINUE) 
	    return CORBA::TypeCode::TRAVERSE_CONTINUE;
	  else 
	    {
	      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));
	      dmsg ("TAO_Marshal_Alias::decode detected error");
	      return CORBA::TypeCode::TRAVERSE_STOP;
	    }
	}
      else // exception getting content_type
	return CORBA::TypeCode::TRAVERSE_STOP;
    }
  else // no typecode
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Struct::deep_free detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}

// deep_free structs
CORBA::TypeCode::traverse_status
TAO_Marshal_Except::deep_free (CORBA::TypeCode_ptr  tc,
			       const void *,
			       const void *,
			       CORBA::Environment &env)
{
#if 0
  // temporarily commented out to make compiler happy
  CORBA::Long i, 
    member_count;      // number of fields in the struct
  CORBA::TypeCode::traverse_status retval = CORBA::TypeCode::TRAVERSE_CONTINUE;
  CORBA::TypeCode_ptr param;
  CORBA::Long size, alignment;
  CDR stream;
#endif /* 0 */

  if (tc)
    // XXX: Exceptions are currently leaked because of bugs lurking
    // in this area.  Keep in mind that there are two things to
    // free: (a) the typecode in the exception base class; (b) any
    // pointers held by a user-defined exception, such as an objref
    // or string.
    //
    // Since this code does nothing, it should leak BOTH of those
    // kinds of memory.  Since it's not supposed to be called except
    // when the exception really is being freed, it should only be
    // called when the reference count in the exception base class
    // is zero.
    //
    // It's not clear which of those assertions actually hold.
    //
    // The code SHOULD be just like the traverse () call for a
    // structure, with (a) a precondition that the reference count
    // is zero, (b) an assertion that the typecode in the exception
    // and "tc" are equivalent, (c) releasing that typecode found
    // within the exception.
    //
    return CORBA::TypeCode::TRAVERSE_CONTINUE;
  else // no typecode
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_MAYBE));
      dmsg ("TAO_Marshal_Struct::deep_free detected error");
      return CORBA::TypeCode::TRAVERSE_STOP;
    }
}
