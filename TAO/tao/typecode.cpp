// @ (#)typecode.cpp	1.4 95/09/19
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// TYPECODE:	basic implementation of TypeCodes
//
// Typecodes essentially consist of just the CDR octets that get
// marshaled and unmarshaled, and this code knows how to parse those
// octets and answer questions CORBA's TypeCode APIs require.
//
// NOTE: This isn't well tuned performance-wise.  Given how much is
// variable (byte order, alignment) it's clear tuning has its limits
// with respect to CDR bytecode interpretation.
//
// THREADING NOTE: Typecodes are readonly data structures, and the
// only mutual exclusion relates to reference counting and
// construction.

#if 0
#include "ace/OS.h"    // WARNING! This MUST come before objbase.h on WIN32!
#include <objbase.h>
#include <initguid.h>

#include "tao/orb.h"
#include "tao/cdr.h"
#include "tao/debug.h"
#endif

#include "tao/corba.h"

// @@ This is a botch...
size_t 
calc_key_union_attributes (CDR *stream,
			   size_t &alignment,
			   size_t &size_with_pad,
			   CORBA::Environment &env);

// Constructor for all other typecodes, including constants with
// non-empty parameter lists.  See "corba.hh" for details.

CORBA_TypeCode::CORBA_TypeCode (CORBA::TCKind kind,
				CORBA::ULong length,
				CORBA::Octet *buffer,
				CORBA::Boolean orb_owns_tc,
				CORBA::TypeCode_ptr parent)
  : _length (length),
    //    _buffer (buffer),
    _kind (kind),
    _parent (parent),
    refcount_ (1),
    _delete_flag (CORBA::B_FALSE),
    _orb_owns (orb_owns_tc),
    _private_state (new TC_Private_State (kind))
{
  // The CDR code used to interpret TypeCodes requires in-memory
  // alignments to match the "on-the-wire" alignments, simplifying
  // algorithms used to marshal/unmarshal.
  //
  // However, it's often hard to get compilers (in particular) to
  // generate data that's so aligned, since C++ doesn't provide
  // primitives giving control at that low a level.  Although there
  // are ways to get that alignment which work in almost all cases, we
  // need to ensure adequate alignment in _all_ cases.
  //
  // This code exists to ensure such alignment; since the constructor
  // is intended only for use by an IDL compiler or ORB code, it's not
  // currently a priority to ensure the allocated code is freed.

#if 0
  if ((((ptr_arith_t) buffer) & 0x03) != 0) 
    {
      ptr_arith_t temp;

      temp = (ptr_arith_t) ACE_OS::malloc (length + 4);
      temp += 3;
      temp &= ~0x03;
      _buffer = (CORBA::Octet *) temp;

      (void) ACE_OS::memcpy (_buffer, buffer, (size_t) length);
      _orb_owns = CORBA::B_FALSE;	// XXX may leak
    }
#endif /* 0 */

  // TAO comments: 
  // For free standing typecodes, we choose to always make a copy of the buffer passed
  // in. That way, our destructor doesn't have to deal with the case where the
  // buffer was either allocated in which case it must be freed or the case
  // where our buffer just points to the buffer passed in.
  if (!_parent)
    {
      // no parent. We are free standing
      ptr_arith_t temp;
      
      // allocate a buffer to hold the encapsulated stream. We allocate extra space
      // since we need a buffer that is aligned on a 4 byte word boundary. As a
      // result, it is quite possible that we may start accessing the buffer
      // from a position shifted to the right in the allocated buffer. As a
      // result, during destruction, we do not want part of the allocated heap
      // to remain dangling. Hence we save a handle to the original allocated
      // buffer
      non_aligned_buffer_ = new CORBA::Octet [length + 4];
      
      temp = (ptr_arith_t) non_aligned_buffer_;
      temp += 3;
      temp &= ~0x03;
      _buffer = (CORBA::Octet *) temp;
      
      (void) ACE_OS::memcpy (_buffer, buffer, (size_t) length);
      _orb_owns = CORBA::B_FALSE;  // the ORB does not own this typecode
    }
  else
    {
      // we are a child. We do not allocate a new buffer, but share it with our
      // parent. We know that our parent's buffer was properly aligned.
      _buffer = buffer;
    }
}

// Destructor.  For "indirected" typecodes and children, the typecode reuses the
// buffer owned by its parent

CORBA_TypeCode::~CORBA_TypeCode (void)
{
  if (_orb_owns)
    {
      // we are constants, don't do anything
      return;
    }
  else if (_parent) // check if we have a parent
    {
      // we have a parent which means that we were not directly created by IDL
      // compiler generated code, but by the precomputation logic. We should
      // delete ourselves and the subtree below us only if our parent was in
      // the process of deleteing itself
      if (_parent->_delete_flag) // parent is deleteing, so we have to go 
	{
	  _delete_flag = CORBA::B_TRUE; // set our delete flag to TRUE so that
				       // our children (if any) will know that
				       // we have initiated our destruction
	  delete _private_state; // delete any private state we have and thus
				 // free up the children
	  // we share the buffer octets of our parent. Hence we don't
	  // deallocate it
	  _buffer = 0;
	}
      // else, somebody maliciously tried to delete us, but we won't get
      // deleted
    }
  else
    {
      // we are free standing (IDL compiler generated code) and are to be deleted
      _delete_flag = CORBA::B_TRUE; // we indicate to our children that we are
				   // getting deleted 
      delete _private_state; // free up our children
      delete [] non_aligned_buffer_; // delete the original, possibly
				     // nonaligned, buffer
      _buffer = 0;
    }
}

// decreases the refcount and deletes when refcount reaches 0
void CORBA::release (CORBA::TypeCode_ptr tc)
{
  if (tc)
    tc->Release ();
}

// returns true if the typecode is NULL
CORBA::Boolean CORBA::is_nil (CORBA::TypeCode_ptr tc)
{
  return (CORBA::Boolean) tc == 0;
}

// COM's IUnknown support

// {A201E4C1-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_TypeCode,
0xa201e4c1, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

// COM stuff
u_long __stdcall
CORBA_TypeCode::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0));

  assert (this != 0);

  if (_orb_owns)
    return refcount_; // this better be 1
  else if (_parent)
    // we are owned by the parent
    //	  return _parent->Addref ();
    return refcount_; // 1
  else
    return refcount_++;
}

// COM stuff
u_long __stdcall
CORBA_TypeCode::Release (void)
{
  // This code is subtle since we need to make sure that we don't try
  // to release the lock after we've deleted this...
  ACE_MT (this->lock_.acquire ());

  ACE_ASSERT (this != 0);

  u_long result;

  if (_orb_owns)
    result = refcount_; // 1
  else if (_parent)
    //	  return _parent->Release ();
    result = refcount_; // 1
  else
    {
      result = --refcount_;
      ACE_MT (this->lock_.release ());

      if (result == 0)
	delete this;

      return result;
    }

  ACE_MT (this->lock_.release ());
  return result; 
}

// constructor for the private state
TC_Private_State::TC_Private_State (CORBA::TCKind kind)
  : tc_kind_ (kind),
    tc_id_known_ (CORBA::B_FALSE),
    tc_name_known_ (CORBA::B_FALSE),
    tc_member_count_known_ (CORBA::B_FALSE),
    tc_member_type_list_known_ (CORBA::B_FALSE),
    tc_member_label_list_known_ (CORBA::B_FALSE),
    tc_discriminator_type_known_ (CORBA::B_FALSE),
    tc_default_index_used_known_ (CORBA::B_FALSE),
    tc_length_known_ (CORBA::B_FALSE),
    tc_content_type_known_ (CORBA::B_FALSE),
    tc_size_known_ (CORBA::B_FALSE),
    tc_alignment_known_ (CORBA::B_FALSE),
    tc_discrim_pad_size_known_ (CORBA::B_FALSE),
    tc_id_ (0),
    tc_name_ (0),
    tc_member_count_ (0),
    tc_member_type_list_ (0),
    tc_member_label_list_ (0),
    tc_discriminator_type_ (0),
    tc_default_index_used_ (0),
    tc_length_ (0),
    tc_content_type_ (0),
    tc_size_ (0),
    tc_alignment_ (0),
    tc_discrim_pad_size_ (0)
{
}

// destructor for the private state. In effect, this cleans up all the children
// and the subtree we hold.
TC_Private_State::~TC_Private_State (void)
{
  // the following two just point into the buffer. So we just make it point to NULL
  tc_id_ = 0;
  tc_name_ = 0;

  // determine what kind of children we may have and free the space accordingly
  switch (tc_kind_)
    {
    case CORBA::tk_struct:
    case CORBA::tk_except:
      {
	// free up member type list
	for (CORBA::ULong i = 0; 
	     i < tc_member_count_;
	     i++)
	  {
	    // free up the memory allocated for the typecode
	    delete tc_member_type_list_[i];
	  }
	// now free up the array
	delete [] tc_member_type_list_;
	tc_member_count_ = 0;
	break;
      }
    case CORBA::tk_sequence:
    case CORBA::tk_array:
    case CORBA::tk_alias:
      // delete the content type
      delete tc_content_type_;
      break;
    case CORBA::tk_union:
      {
	// free up discriminator type, member label list, and member type list
	delete tc_discriminator_type_;
	if (tc_member_type_list_known_)
	  {
	    for (CORBA::ULong i = 0; 
		 i < tc_member_count_;
		 i++)
	      {
		// free up the memory allocated for the typecode
		delete tc_member_type_list_[i];
	      }
	    // now free up the array
	    delete [] tc_member_type_list_;
	  }
	if (tc_member_label_list_known_)
	  {
	    for (CORBA::ULong i = 0; 
		 i < tc_member_count_;
		 i++)
	      {
		// free up the label (Any_ptr)
		delete tc_member_label_list_[i];
	      }
	    delete [] tc_member_label_list_;
	  }
	tc_member_count_ = 0;
      }
    break;
    default:
      // nothing to do
      break;
    }
}

// COM stuff
HRESULT __stdcall
CORBA_TypeCode::QueryInterface (REFIID riid,
				void **ppv)
{
  *ppv = 0;

  if (IID_CORBA_TypeCode == riid || IID_IUnknown == riid)
    *ppv = this;

  if (*ppv == 0)
    return ResultFromScode (E_NOINTERFACE);

 (void) AddRef ();
  return NOERROR;
}

// This method is not yet implemented completely - low priority task
CORBA::Boolean
CORBA_TypeCode::private_equal (CORBA::TypeCode_ptr,
			       CORBA::Environment &) const
{
  // We come in here only if the typecode kinds of both are same
  // Handle each complex typecode separately.
  switch (_kind)
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
    case CORBA::tk_longdouble:
    case CORBA::tk_boolean:
    case CORBA::tk_octet:
    case CORBA::tk_char:
    case CORBA::tk_wchar:
    case CORBA::tk_TypeCode:
    case CORBA::tk_Principal:
      // all these are simple typecodes and the comparison is based solely on
      // the _kind field
      return CORBA::B_TRUE;
    case CORBA::tk_objref:
      //      return private_equal_objref (tc, env);
    case CORBA::tk_struct:
      //      return private_equal_struct (tc, env);
    case CORBA::tk_union:
      //      return private_equal_union (tc, env);
    case CORBA::tk_enum:
      //      return private_equal_enum (tc, env);
    case CORBA::tk_string:
      //      return private_equal_string (tc, env);
    case CORBA::tk_wstring:
      //      return private_equal_string (tc, env);
    case CORBA::tk_sequence:
      //      return private_equal_sequence (tc, env);
    case CORBA::tk_array:
      //      return private_equal_array (tc, env);
    case CORBA::tk_alias:
      //      return private_equal_alias (tc, env);
    case CORBA::tk_except:
      //      return private_equal_except (tc, env);
    default:
      // Not implemented yet
      return CORBA::B_TRUE;
    }
}

// Return the type ID (RepositoryId) for the TypeCode; it may be empty.
//
// NOTE the string returned here is owned by the typecode!!
//
// Valid only for objref, struct, union, enum, alias, and except. Raises
// BadKind exception for the rest of the cases.
TAO_CONST CORBA::String
CORBA_TypeCode::private_id (CORBA::Environment &env) const
{
  env.clear ();

  switch (_kind) 
    {
      // These are all complex typecodes, which have as their first
      // parameter (number zero) a repository/type ID string encoded
      // per CDR rules.  That means we can just return a pointer to
      // that string directly!

    case CORBA::tk_objref:
    case CORBA::tk_struct:
    case CORBA::tk_union:
    case CORBA::tk_enum:
    case CORBA::tk_alias:
    case CORBA::tk_except:
      {
	_private_state->tc_id_known_ = CORBA::B_TRUE;
	_private_state->tc_id_ = (CORBA::String) (_buffer
						 + 4	// skip byte order flag
							// and padding 
						 + 4);	// skip (strlen + 1) 
	return _private_state->tc_id_; // this is OK because the strings in the
				       // CDR stream are NULL terminated
      }
    // No other typecodes ever have type IDs
    default:
      env.exception (new CORBA::TypeCode::BadKind ());
      return 0;
    }
}

// return the name. The string is owned by the typecode
TAO_CONST CORBA::String
CORBA_TypeCode::private_name (CORBA::Environment &env) const
{
  env.clear ();

  switch (_kind) 
    {
      // These are all complex typecodes, which have as their second
      // parameter (number one) a name string encoded
      // per CDR rules.  That means we can just return a pointer to
      // that string directly!

    case CORBA::tk_objref:
    case CORBA::tk_struct:
    case CORBA::tk_union:
    case CORBA::tk_enum:
    case CORBA::tk_alias:
    case CORBA::tk_except:
      {
	CDR stream;

	stream.setup_encapsulation (_buffer, (size_t) _length);

	// skip the typecode ID
	if (stream.skip_string ())  // ID
	  {
	    _private_state->tc_name_known_ = CORBA::B_TRUE;
	    
	    // skip past the length field.
	    _private_state->tc_name_ = (CORBA::String) (stream.next + CDR::LONG_SIZE);  

	    return _private_state->tc_name_;
	  }
	else
	  {
	    env.exception (new CORBA::INTERNAL (CORBA::COMPLETED_NO));
	    return (CORBA::String)0;
	  }
      }
    // No other typecodes ever have type IDs
    default:
      env.exception (new CORBA::TypeCode::BadKind ());
      return (CORBA::String)0;
    }
}

// Return the number of members defined by this typecode
//
// Applicable to struct, union, enum, alias, and except
// For the rest of the cases, raises the BadKind exception.

CORBA::ULong
CORBA_TypeCode::private_member_count (CORBA::Environment &env) const
{
  env.clear ();

  switch (_kind) 
    {
    case CORBA::tk_alias:
      //      tc_member_count_known_ = CORBA::B_TRUE;
      //tc_member_count_ = 1;
      return 1;

    case CORBA::tk_enum:
    case CORBA::tk_except:
    case CORBA::tk_struct:
      {
	CORBA::ULong members;
	CDR stream;

	stream.setup_encapsulation (_buffer, (size_t)_length);

	// skip rest of header (type ID and name) and collect the
	// number of struct members
	if (!stream.skip_string ()	    	    // ID
	    || !stream.skip_string ()  	    // struct name
	    || !stream.get_ulong (members)) 
	  {
	    env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	    return 0;
	  }

	_private_state->tc_member_count_known_ = CORBA::B_TRUE;
	_private_state->tc_member_count_ = members;
	return _private_state->tc_member_count_;
      }
    case CORBA::tk_union:
      {
	CORBA::ULong members;
	CDR stream;

	stream.setup_encapsulation (_buffer, (size_t) _length);

	// skip rest of header (type ID, name, etc...) and collect the
	// number of struct members
	if (!stream.skip_string ()			// ID
	    || !stream.skip_string ()		// struct name
	    || !skip_typecode (stream)		// discriminant TC
	    || !stream.get_ulong (members)	// default used
	    || !stream.get_ulong (members))	// real member count
	  {
	    // this is a system exception indicating something is wrong with
	    // the typecode itself.
	    env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	    return 0;
	  }

	_private_state->tc_member_count_known_ = CORBA::B_TRUE;
	_private_state->tc_member_count_ = members;
	return _private_state->tc_member_count_;
      }
    default:
      env.exception (new CORBA::TypeCode::BadKind ());
      return 0;
    }
}

// Return the name for the nth member
// Applicable only to CORBA::tk_struct, CORBA::tk_union, and CORBA::tk_except
TAO_CONST CORBA::String 
CORBA_TypeCode::member_name (CORBA::ULong,
			     CORBA::Environment &) const
{
  // not implemented - low priority task
  return 0;
}

// NOTE special calling convention for stream.decode () when we're
// potentially deencapsulating an indirected typecode: the "data2"
// value indicates that this typecode is the parent.  See comments at
// stream.decode () for further details.
//
// Applicable only to struct, union, and except
//

CORBA::TypeCode_ptr
CORBA_TypeCode::private_member_type (CORBA::ULong index, 
				     CORBA::Environment &env) const
{
  CORBA::ULong temp, mcount;

  // Build the de-encapsulating CDR stream, bypassing the stringent
  // alignment tests (we're a bit looser in what we need here, and we
  // _know_ we're OK).  Then skip the byte order code.
  CDR stream;
  CORBA::TypeCode_ptr tc = 0;

  stream.setup_encapsulation (_buffer, (size_t)_length);

  switch (_kind) 
    {
    case CORBA::tk_except:
    case CORBA::tk_struct:		// index from 0
      mcount = member_count (env);		// clears env
      if (env.exception () == 0)
	{
	  // the first time in. Precompute and store types of all members
	  
	  // Allocate a list to hold the member typecodes
	  _private_state->tc_member_type_list_ = new CORBA::TypeCode_ptr [mcount];

	  if (_private_state->tc_member_type_list_)
	    {
	      // skip the id, name, and member_count part
	      if (!stream.skip_string ()	// type ID, hidden
		  || !stream.skip_string ()	// typedef name
		  || !stream.get_ulong (temp))  // member count
		{ 
		  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
		  return (CORBA::TypeCode_ptr)0;
		} 
	      else 
		{
		  // compute the typecodes for all the members and
		  // return the required one.
		  for (CORBA::ULong i = 0; i < mcount; i++) 
		    {
		      // the ith entry will have the typecode of the ith guy
		      if (!stream.skip_string ()  // skip the name
			  || stream.decode (CORBA::_tc_TypeCode,
					    // the typecode will be retrieved
					    // at the i-th location. The decode
					    // routine will allocate the
					    // storage to hold a typecode
					    &_private_state->tc_member_type_list_[i],
					    this,  // pass ourselves since we
						   // will be
						   // the parent. This is the
						   // case where the 3rd
						   // parameter is used in a
						   // decode method
					    env) != CORBA::TypeCode::TRAVERSE_CONTINUE) 
			{
			  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
			  return 0;
			}
		    }

		  _private_state->tc_member_type_list_known_ = CORBA::B_TRUE;

		  if (index < mcount)
		    return _private_state->tc_member_type_list_[index];
		  else
		    {
		      env.exception (new CORBA::TypeCode::Bounds ());
		      return (CORBA::TypeCode_ptr)0;
		    }
		}
	    }
	  else  // no memory for the member_list
	    {
	      env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
	      return (CORBA::TypeCode_ptr)0;
	    }
	}
      else // out of bounds
	{
	  env.exception (new CORBA::TypeCode::Bounds ());
	  return (CORBA::TypeCode_ptr)0;
	}
    case CORBA::tk_union:            // index from 0
      mcount = member_count (env);		// clears env
      if (env.exception () == 0)
	{
	  // the first time in. Precompute and store types of all members
	  _private_state->tc_member_type_list_ = new CORBA::TypeCode_ptr [mcount];
	  if (_private_state->tc_member_type_list_)
	    {
	      // skip the id, name, and discrimant type part
	      if (!stream.skip_string ()	// type ID, hidden
		  || !stream.skip_string ()	// typedef name
		  || !skip_typecode (stream))   // skip typecode for discriminant
		{ 
		  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
		  return (CORBA::TypeCode_ptr)0;
		} 
	      else if (!stream.get_ulong (temp)	    // default used
		       || !stream.get_ulong (temp)) // member count
		{
		  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
		  return 0;
		}
	      else  
		{
		  CORBA::Long scratch; // always big enough because labels can
				      // only be of a few different types of
				      // which "long" has the largest size

		  // get the typecode for the discriminator
		  tc = discriminator_type (env);
		  // compute the typecodes for all the members and return the
		  // required one

		  for (CORBA::ULong i = 0; i < mcount; i++) 
		    {
		      // the ith entry will have the typecode of the ith guy
		      if (stream.decode (tc, &scratch, this,  env) // member label
			  != CORBA::TypeCode::TRAVERSE_CONTINUE
			  || !stream.skip_string ()  // skip the name
			  || stream.decode (CORBA::_tc_TypeCode,  // get the typecode
					    &_private_state->tc_member_type_list_[i],
					    this, 
					    env) !=
			  CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
			  return 0;
			}
		    }
		  _private_state->tc_member_type_list_known_ = CORBA::B_TRUE;

		  if (index < mcount)
		    return _private_state->tc_member_type_list_[index];
		  else
		    {
		      env.exception (new CORBA::TypeCode::Bounds ());
		      return (CORBA::TypeCode_ptr)0;
		    }
		}
	    }
	  else  // no memory for the member_list
	    {
	      env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
	      return (CORBA::TypeCode_ptr)0;
	    }
	}
      else // out of bounds
	{
	  env.exception (new CORBA::TypeCode::Bounds ());
	  return (CORBA::TypeCode_ptr)0;
	}

    default:
      // bad kind
      env.exception (new CORBA::TypeCode::BadKind ());
      return (CORBA::TypeCode_ptr)0;
    }
}

// Return member labels for CORBA::tk_union typecodes.
CORBA::Any_ptr
CORBA_TypeCode::private_member_label (CORBA::ULong n,
			      CORBA::Environment &env) const
{
  env.clear ();

  // this function is only applicable to the CORBA::tk_union TC
  if (_kind == CORBA::tk_union)
    {
      CDR stream;

      stream.setup_encapsulation (_buffer, (size_t)_length);

      // skip ID and name, and then get the discriminant TC
      CORBA::TypeCode_ptr    tc = 0;

      if (!stream.skip_string ()       // type ID, hidden
	  || !stream.skip_string ()    // typedef name
	  || !skip_typecode (stream))  // skip discriminant typecode
	{
	  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	  return 0;
	}

      // skip default used, and get member count
      CORBA::ULong member_count;

      if (!stream.get_ulong (member_count)	// default used
	  || !stream.get_ulong (member_count))  // member count
	{
	  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	  dmsg ("TypeCode::private_member_label -- error reading from stream");
	  return 0;
	}

      // member labels are of type Any. However, the actual types are
      // restricted to simple types
      _private_state->tc_member_label_list_ = new CORBA::Any_ptr [member_count];
      if (_private_state->tc_member_label_list_)
	{
	  tc = discriminator_type (env); // retrieve the discriminator type as
					 // this decides what the label is
	  for (CORBA::ULong i = 0; i < member_count; i++) 
	    {
	      // allocate buffer to hold the member label value
	      void *buf = new CORBA::Octet [tc->size (env)];
	      if (stream.decode (tc, buf, this,  env)
		  != CORBA::TypeCode::TRAVERSE_CONTINUE
		  || !stream.skip_string ()   	    // member name
		  || !skip_typecode (stream)) // member type
		{	    // member TC
		  dmsg1 ("TypeCode::private_member_label -- error getting typecode for member %d",i);
		  delete [] buf;
		  // XXXASG: free this list on error
		  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
		  return 0;
		}
	      else
		{
		  _private_state->tc_member_label_list_[i] = new CORBA::Any (tc,
									    buf, CORBA::B_TRUE); 
		}
	    }
	}
      else
	{
	  env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
	  return 0;
	}

      _private_state->tc_member_label_list_known_ = CORBA::B_TRUE;

      // If caller asked for the label for a nonexistent member, they get
      // an error report!
      if (n >= member_count)  
	{
	  env.exception (new CORBA::TypeCode::Bounds ());
	  return 0;
	}
      else
	return _private_state->tc_member_label_list_[n];
    }
  else // wrong typecode
    {
      env.exception (new CORBA::TypeCode::BadKind ());
      return 0;
    }
}

CORBA::TypeCode_ptr 
CORBA_TypeCode::private_discriminator_type (CORBA::Environment &env) const
{
  CDR stream;
  
  stream.setup_encapsulation (_buffer, (size_t)_length);

  // skip ID and name, and then get the discriminant TC
  
  if (!stream.skip_string ()		// type ID, hidden
      || !stream.skip_string ()	        // typedef name
      || stream.decode (CORBA::_tc_TypeCode,
			&_private_state->tc_discriminator_type_, this,  env) !=
      CORBA::TypeCode::TRAVERSE_CONTINUE) 
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
      return 0;
    }
  else
    {
      _private_state->tc_discriminator_type_known_ = CORBA::B_TRUE;
      return _private_state->tc_discriminator_type_;
    }
}

CORBA::Long
CORBA_TypeCode::private_default_index (CORBA::Environment &env) const
{
  CDR stream;

  stream.setup_encapsulation (_buffer, (size_t)_length);

  // skip ID and name, and then get the discriminant TC
  
  if (!stream.skip_string ()		// type ID, hidden
      || !stream.skip_string ()	        // typedef name
      || !skip_typecode (stream)        // skip discriminant
      || !stream.get_long (_private_state->tc_default_index_used_))
      
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
      return 0;
    }
  else
    {
      _private_state->tc_default_index_used_known_ = CORBA::B_TRUE;
      return _private_state->tc_default_index_used_;
    }
}

CORBA::Long
CORBA_TypeCode::private_length (CORBA::Environment &env) const
{
  CDR stream;
#if defined(TAO_NEEDS_UNUSED_VARIABLES)  
  CORBA::TypeCode_ptr tc = 0;
#endif /* TAO_NEEDS_UNUSED_VARIABLES */

  stream.setup_encapsulation (_buffer, (size_t)_length);
  switch (_kind)
    {
    case CORBA::tk_sequence:
    case CORBA::tk_array:
      {
	// skip the typecode of the element and get the bounds
	if (!skip_typecode (stream) // skip typecode
	    || !stream.get_ulong (_private_state->tc_length_))
	  {
	    env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
	    return 0;
	  }
	else
	  {
	    _private_state->tc_length_known_ = CORBA::B_TRUE;
	    return _private_state->tc_length_;
	  }
      case CORBA::tk_string:
      case CORBA::tk_wstring:
	{
	  if (stream.get_ulong (_private_state->tc_length_))
	    {
	      _private_state->tc_length_known_ = CORBA::B_TRUE;
	      return _private_state->tc_length_;
	    }
	  else
	    {
	      env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
	      return 0;
	    }
	}
      default:
	env.exception (new CORBA::TypeCode::BadKind);
	return 0;
      }
    }
}

CORBA::TypeCode_ptr
CORBA_TypeCode::private_content_type (CORBA::Environment &env) const
{
  CDR stream;

  stream.setup_encapsulation (_buffer, (size_t) _length);
  switch (_kind)
    {
    case CORBA::tk_sequence:
    case CORBA::tk_array:
      {
	// retrieve the content type
	if (stream.decode (CORBA::_tc_TypeCode, &_private_state->tc_content_type_,
			   this,  env) != 
	    CORBA::TypeCode::TRAVERSE_CONTINUE) // element type
	  {
	    env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
	    return 0;
	  }
	else
	  {
	    _private_state->tc_content_type_known_ = CORBA::B_TRUE;
	    return _private_state->tc_content_type_;
	  }
      case CORBA::tk_alias:
	{
	  if (!stream.skip_string ()  // typeID
	      || !stream.skip_string () // name
	      || stream.decode (CORBA::_tc_TypeCode,
				&_private_state->tc_content_type_, this,  env) != 
	      CORBA::TypeCode::TRAVERSE_CONTINUE)
	    {
	      env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
	      return 0;
	    }
	  else
	    {
	      _private_state->tc_content_type_known_ = CORBA::B_TRUE;
	      return _private_state->tc_content_type_;
	    }
	}
      default:
	env.exception (new CORBA::TypeCode::BadKind);
	return 0;
      }
    }
}

CORBA::ULong
CORBA_TypeCode::private_discrim_pad_size (CORBA::Environment &env)
{
  CDR stream;
  size_t discrim_size,
    overall_align;

  stream.setup_encapsulation (_buffer, (size_t)_length);

 (void) calc_key_union_attributes (&stream, overall_align, discrim_size, env);

  if (env. exception () == 0)
    {
      _private_state->tc_discrim_pad_size_known_ = CORBA::B_TRUE;
      _private_state->tc_discrim_pad_size_ = discrim_size;
      return discrim_size;
    }
  else
    return 0;
}
// ************ The following are deprecated ****************

// say how many parameters this typecode has; normally a fixed number,
// some are variable length.
//
// NOTE: This refers to "real" parameters, not what shows up in the
// IFR spec !!  That is, "hidden" parameters are counted here, this
// doesn't strictly comply with what CORBA says "param_count"
// provides.

CORBA::ULong
CORBA_TypeCode::param_count (CORBA::Environment &env) const
{
  env.clear ();

  switch (_kind) 
    {
    default:
      return 0;

    case CORBA::tk_string:
    case CORBA::tk_wstring:
      return 1;

    case CORBA::tk_objref:
    case CORBA::tk_sequence:
    case CORBA::tk_array:
      return 2;

    case CORBA::tk_alias:
      return 3;

    case CORBA::tk_except:
    case CORBA::tk_struct:
      {
	CORBA::ULong members;
	CDR stream;

	stream.setup_encapsulation (_buffer, (size_t)_length);

	// skip rest of header (type ID and name) and collect the
	// number of struct members
	if (!stream.skip_string ()	    	    // ID
	    || !stream.skip_string ()  	    // struct name
	    || !stream.get_ulong (members)) 
	  {
	    env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	    return 0;
	  }

	return 3 + 2 * members;
      }
    case CORBA::tk_enum:
      {
	CORBA::ULong members;
	CDR stream;

	stream.setup_encapsulation (_buffer, (size_t)_length);

	// skip rest of header (type ID and name) and collect the
	// number of struct members
	if (!stream.skip_string ()			// ID
	    || !stream.skip_string ()		// typedef name
	    || !stream.get_ulong (members)) 
	  {
	    env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	    return 0;
	  }

	return 3 + members;
      }
    case CORBA::tk_union:
      {
	CORBA::ULong members;
	CDR stream;

	stream.setup_encapsulation (_buffer, (size_t) _length);

	// skip rest of header (type ID, name, etc...) and collect the
	// number of struct members
	if (!stream.skip_string ()			// ID
	    || !stream.skip_string ()		// struct name
	    || !skip_typecode (stream)		// discriminant TC
	    || !stream.get_ulong (members)	// default used
	    || !stream.get_ulong (members))	// real member count
	  {
	    env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	    return 0;
	  }

	return 5 + 3 * members;
      }
    }
}


// Internal hack, used until member_count () and length () are
// implemented.  Doesn't support all the types that those routines
// support.

CORBA::ULong
CORBA_TypeCode::ulong_param (CORBA::ULong n,
			     CORBA::Environment &env) const
{
  CORBA::ULong temp;

  temp = param_count (env);		// clears env
  if (env.exception ())
    return 0;

  if (temp < n) 
    {
      env.exception (new CORBA::TypeCode::Bounds);
      return 0;
    }

  // Get parameters for non-empty typecodes; their parameter lists are
  // encapsulated CDR (for complex ones) or inlined (for simple ones).
  switch (_kind) 
    {
    default:				// most have no long params
      break;

      // Array, sequence ... complex parameter lists
    case CORBA::tk_array:		// param 1 is an integer
    case CORBA::tk_sequence:		// ... identical content
      { 
	if (n == 0)
	  break;

	// Build CDR stream for encapsulated params, and skip the
	// typecode up front.
	CDR stream;

	stream.setup_encapsulation (_buffer, (size_t)_length);
	if (!skip_typecode (stream)) 
	  {
	    env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
	    return 0;
	  }

	// Then comes the "bounds" parameter.
	if (!stream.get_ulong (temp))
	  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	return temp;
      }

    // string, wstring ... simple parameter lists, containing just the
    // string bounds (zero indicates unbounded).  Stored specially
    case CORBA::tk_string:
    case CORBA::tk_wstring:
      if (n != 0)
	break;
      return _length;
    }
  env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
  return 0;
}

// Internal hack, used until member_type (), discriminator_type (),
// and content_type () are implemented.
//
// NOTE special calling convention for stream.decode () when we're
// potentially deencapsulating an indirected typecode: the "data2"
// value indicates that this typecode is the parent.  See comments at
// stream.decode () for further details.

CORBA::TypeCode_ptr
CORBA_TypeCode::typecode_param (CORBA::ULong n,
				CORBA::Environment &env) const
{
  CORBA::ULong temp;

  temp = param_count (env);		// clears env
  if (env.exception ())
    return 0;

  if (temp < n) 
    {
      env.exception (new CORBA::TypeCode::Bounds);
      return 0;
    }

  // Build the de-encapsulating CDR stream, bypassing the stringent
  // alignment tests (we're a bit looser in what we need here, and we
  // _know_ we're OK).  Then skip the byte order code.

  CDR stream;
  CORBA::TypeCode_ptr tc = 0;

  stream.setup_encapsulation (_buffer, (size_t)_length);

  switch (_kind) 
    {
    default:				// most have no tc params
      break;

    case CORBA::tk_sequence:		// param 0 is a tc
    case CORBA::tk_array:
      if (n != 0)
	break;
      if (stream.decode (CORBA::_tc_TypeCode,
			 &tc,
			 this,
			 env) != CORBA::TypeCode::TRAVERSE_CONTINUE) 
	{
	  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	  return 0;
	}
      return tc;

    case CORBA::tk_alias:		// #1 is a tc
      if (n != 2)
	break;
      if (!stream.skip_string ()		// type ID, hidden
	  || !stream.skip_string ()	// typedef name
	  || stream.decode (CORBA::_tc_TypeCode, &tc, this,  env)
	  != CORBA::TypeCode::TRAVERSE_CONTINUE) 
	{
	  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	  return 0;
	}
      return tc;

    case CORBA::tk_except:
    case CORBA::tk_struct:		// #5 and up are tc, index from 0
      if (n < 4 || n & 0x1) 
	{	// tc is at odd number of param list
	  env.exception (new CORBA::TypeCode::Bounds);
	  return 0;
	}

      if (!stream.skip_string ()		// type ID, hidden
	  || !stream.skip_string ()	// typedef name
	  || !stream.get_ulong (temp)) 
	{ 	// member count
	  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	  return 0;
	} 
      else 
	{
	  temp = (n - 3) / 2;

	  // skip member pairs to the one we want
	  for (CORBA::ULong i = 0; i < temp; i++) 
	    {
	      // skip to the member being asked
	      if (!stream.skip_string ()	// member name
		  || !skip_typecode (stream)) 
		{
		  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
		  return 0;
		}
	    }

	  if (!stream.skip_string ()
	      || stream.decode (CORBA::_tc_TypeCode, &tc, this, 
			       env)!= CORBA::TypeCode::TRAVERSE_CONTINUE) 
	    {
	      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	      return 0;
	    }
	  return tc;
	}

    case CORBA::tk_union:            // #6 and up are tc, index from 0
      if (n != 2 && (n < 7 || (n - 7) % 3)) 
	{
	  env.exception (new CORBA::TypeCode::Bounds);
	  return 0;
	}

      if (!stream.skip_string ()		// type ID, hidden
	  || !stream.skip_string ()	// typedef name
	  || stream.decode (CORBA::_tc_TypeCode,
			   &tc, this,
			    env) != CORBA::TypeCode::TRAVERSE_CONTINUE)  // TC
	{
	  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	  return 0;
	} 
      else if (!stream.get_ulong (temp)	// default used
		 || !stream.get_ulong (temp))	// member count
	{
	  tc->Release ();
	  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	  return 0;
	}

      if (n == 2)
	return tc;

      temp = (n - 7) / 3;

      // skip to the member requested

      CORBA::Long scratch;		// always big enough

      for (CORBA::ULong i = 0; i < temp; i++) 
	{
	  if (stream.decode (tc, &scratch, this,  env) // member label
	      != CORBA::TypeCode::TRAVERSE_CONTINUE
	      || !stream.skip_string ()		// member name
	      || !skip_typecode (stream)) 
	    {	// member typecode
	      tc->Release ();
	      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	      return 0;
	    }
	}

      // member label
      if (stream.decode (tc, 
			&scratch, this,
			 env) != CORBA::TypeCode::TRAVERSE_CONTINUE
	  || !stream.skip_string ())		// member name
	{
	  tc->Release ();
	  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	  return 0;
	}
      tc->Release ();

      if (stream.decode (CORBA::_tc_TypeCode,
			&tc, this,
			 env) != CORBA::TypeCode::TRAVERSE_CONTINUE) 
	{
	  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	  return 0;
	}
      return tc;
    }

  env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
  return 0;
}

// skip a typecode encoding in a given CDR stream
// This is just a helper function
CORBA::Boolean 
CORBA_TypeCode::skip_typecode (CDR &stream)
{
  CORBA::ULong kind;
  CORBA::ULong temp;

  if (stream.get_ulong (kind) 
      && (kind < CORBA::TC_KIND_COUNT || kind == ~CORBA::ULong(0)))
    {

      switch (kind) 
	{
	  // Most TypeCodes have empty parameter lists, nothing to skip
	default:
	  break;

	  // Some have single integer parameters, easy to skip.  Some have
	  // preallocated constants that could be used.
	case CORBA::tk_string:
	case CORBA::tk_wstring:
	case ~0:
	  return stream.get_ulong (temp);

	  // The rest have "complex" parameter lists that are
	  // encoded as bulk octets ... just skip them.
	case CORBA::tk_objref:
	case CORBA::tk_struct:
	case CORBA::tk_union:
	case CORBA::tk_enum:
	case CORBA::tk_sequence:
	case CORBA::tk_array:
	case CORBA::tk_alias:
	case CORBA::tk_except:
	  return stream.get_ulong (temp) != CORBA::B_FALSE
	    && stream.skip_bytes (temp) != CORBA::B_FALSE;
	}

      return CORBA::B_TRUE;
    }
  else
    return CORBA::B_FALSE;
}


