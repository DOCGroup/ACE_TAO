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

#include "ace/OS.h"    // WARNING! This MUST come before objbase.h on WIN32!
#include <objbase.h>
#include <initguid.h>

#include "tao/orb.h"
#include "tao/cdr.h"
#include "tao/debug.h"

// @@ This is a botch...
size_t 
calc_key_union_attributes (CDR *stream,
			   size_t &alignment,
			   size_t &size_with_pad,
			   CORBA_Environment &env);

// constructor for the private state
TC_Private_State::TC_Private_State (void)
  : tc_id_known_ (CORBA_B_FALSE),
    tc_name_known_ (CORBA_B_FALSE),
    tc_member_count_known_ (CORBA_B_FALSE),
    tc_member_type_list_known_ (CORBA_B_FALSE),
    tc_member_label_list_known_ (CORBA_B_FALSE),
    tc_discriminator_type_known_ (CORBA_B_FALSE),
    tc_default_index_used_known_ (CORBA_B_FALSE),
    tc_length_known_ (CORBA_B_FALSE),
    tc_content_type_known_ (CORBA_B_FALSE),
    tc_size_known_ (CORBA_B_FALSE),
    tc_alignment_known_ (CORBA_B_FALSE),
    tc_discrim_pad_size_known_ (CORBA_B_FALSE),
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

// Constructor for all other typecodes, including constants with
// non-empty parameter lists.  See "corba.hh" for details.

CORBA_TypeCode::CORBA_TypeCode (CORBA_TCKind kind,
				CORBA_ULong length,
				CORBA_Octet *buffer,
				CORBA_Boolean orb_owns_tc) 
  : _length (length),
    _buffer (buffer),
    _kind (kind),
    _parent (0),
    refcount_ (1),
    _delete_flag (CORBA_B_FALSE),
    _orb_owns (orb_owns_tc),
    _private_state (new TC_Private_State)
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

  if ((((ptr_arith_t) buffer) & 0x03) != 0) 
    {
      ptr_arith_t temp;

      temp = (ptr_arith_t) ACE_OS::malloc (length + 4);
      temp += 3;
      temp &= ~0x03;
      _buffer = (CORBA_Octet *) temp;

      (void) ACE_OS::memcpy (_buffer, buffer, (size_t) length);
      _orb_owns = CORBA_B_FALSE;	// XXX may leak
    }
}

// Destructor.  For "indirected" typecodes, the typecode reuses the
// buffer owned by its parent, and so rather than deleting the buffer
// it just drops the parent's refcount.

CORBA_TypeCode::~CORBA_TypeCode (void)
{
  if (_orb_owns)
    // we are constants, don't do anything
    return;
  else if (_parent) // check if we have a parent
    {
      if (_parent->_delete_flag) // parent is deleteing, so we have to go 
	{
	  _delete_flag = CORBA_B_TRUE;
	  child_free ();
	  delete _private_state;
	  ACE_OS::free (_buffer); // our buffer was allocated on the heap
	}
      // else, somebody maliciously tried to delete us
    }
  else
    {
      // we are free standing and are to be deleted
      _delete_flag = CORBA_B_TRUE; // we indicate to our children that we
      // are getting deleted
      child_free (); // first delete any children and the subtree
      delete _private_state;
      //	  delete _buffer; // careful. This may be static
    }
}

// decreases the refcount and deletes when refcount reaches 0
void CORBA_release (CORBA_TypeCode_ptr tc)
{
  if (tc)
    tc->Release ();
}

// returns true if the typecode is NULL
CORBA_Boolean CORBA_is_nil (CORBA_TypeCode_ptr tc)
{
  return (CORBA_Boolean) tc == 0;
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

void 
CORBA_TypeCode::child_free (void)
{
  switch (_kind)
    {
    case tk_struct:
    case tk_except:
      {
	// free up members

	for (CORBA_ULong i = 0; 
	     i < _private_state->tc_member_count_;
	     i++)
	  // @@ This might be a memory leak if tc_member_type_list_[i]
	  // points to an *array*.  If it does, the fix is to add []...
	  delete _private_state->tc_member_type_list_[i];

	break;
      }
    case tk_sequence:
    case tk_array:
    case tk_alias:
      delete _private_state->tc_content_type_;
      break;
    case tk_union:
      // implement this
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

CORBA_Boolean
CORBA_TypeCode::private_equal (CORBA_TypeCode_ptr,
			       CORBA_Environment &) const
{
  // We come in here only if the typecode kinds of both are same
  // Handle each complex typecode separately.
  switch (_kind)
    {
    case tk_null:
    case tk_void:
    case tk_short:
    case tk_ushort:
    case tk_long:
    case tk_ulong:
    case tk_float:
    case tk_double:
    case tk_longlong:
    case tk_longdouble:
    case tk_boolean:
    case tk_octet:
    case tk_char:
    case tk_wchar:
    case tk_TypeCode:
    case tk_Principal:
      // all these are simple typecodes and the comparison is based solely on
      // the _kind field
      return CORBA_B_TRUE;
    case tk_objref:
      //      return private_equal_objref (tc, env);
    case tk_struct:
      //      return private_equal_struct (tc, env);
    case tk_union:
      //      return private_equal_union (tc, env);
    case tk_enum:
      //      return private_equal_enum (tc, env);
    case tk_string:
      //      return private_equal_string (tc, env);
    case tk_wstring:
      //      return private_equal_string (tc, env);
    case tk_sequence:
      //      return private_equal_sequence (tc, env);
    case tk_array:
      //      return private_equal_array (tc, env);
    case tk_alias:
      //      return private_equal_alias (tc, env);
    case tk_except:
      //      return private_equal_except (tc, env);
    default:
      // Not implemented yet
      return CORBA_B_FALSE;
    }
}

// Return the type ID (RepositoryId) for the TypeCode; it may be empty.
//
// NOTE the string returned here is owned by the typecode!!
//
// Valid only for objref, struct, union, enum, alias, and except. Raises
// BadKind exception for the rest of the cases.
const CORBA_String
CORBA_TypeCode::private_id (CORBA_Environment &env) const
{
  env.clear ();

  switch (_kind) 
    {
      // These are all complex typecodes, which have as their first
      // parameter (number zero) a repository/type ID string encoded
      // per CDR rules.  That means we can just return a pointer to
      // that string directly!

    case tk_objref:
    case tk_struct:
    case tk_union:
    case tk_enum:
    case tk_alias:
    case tk_except:
      _private_state->tc_id_known_ = CORBA_B_TRUE;
      _private_state->tc_id_ = (CORBA_String) (_buffer
					       + 4		// skip byte order flag and padding
					       + 4);		// skip (strlen + 1)
      return _private_state->tc_id_;
      // No other typecodes ever have type IDs
    default:
      env.exception (new CORBA_BadKind ());
      return 0;
    }
}

// return the name. The string is owned by the typecode
const CORBA_String
CORBA_TypeCode::private_name (CORBA_Environment &env) const
{
  env.clear ();

  switch (_kind) 
    {
      // These are all complex typecodes, which have as their second
      // parameter (number one) a name string encoded
      // per CDR rules.  That means we can just return a pointer to
      // that string directly!

    case tk_objref:
    case tk_struct:
    case tk_union:
    case tk_enum:
    case tk_alias:
    case tk_except:
      {
	CDR stream;

	stream.setup_encapsulation (_buffer, (size_t) _length);

	// skip the typecode ID
	if (stream.skip_string ())  // ID
	  {
	    _private_state->tc_name_known_ = CORBA_B_TRUE;
	    
	    // skip past the length field.
	    _private_state->tc_name_ = (CORBA_String) (stream.next + CDR::LONG_SIZE);  

	    return _private_state->tc_name_;
	  }
	else
	  {
	    env.exception (new CORBA_INTERNAL (COMPLETED_NO));
	    return (CORBA_String)0;
	  }
	// No other typecodes ever have type IDs
      }
    break;
    default:
      env.exception (new CORBA_BadKind ());
      return 0;
    }
}

// Return the number of members defined by this typecode
//
// Applicable to struct, union, enum, alias, and except
// For the rest of the cases, raises the BadKind exception.

CORBA_ULong
CORBA_TypeCode::private_member_count (CORBA_Environment &env) const
{
  env.clear ();

  switch (_kind) 
    {
    case tk_alias:
      //      tc_member_count_known_ = CORBA_B_TRUE;
      //tc_member_count_ = 1;
      return 1;

    case tk_enum:
    case tk_except:
    case tk_struct:
      {
	CORBA_ULong members;
	CDR stream;

	stream.setup_encapsulation (_buffer, (size_t)_length);

	// skip rest of header (type ID and name) and collect the
	// number of struct members
	if (!stream.skip_string ()	    	    // ID
	    || !stream.skip_string ()  	    // struct name
	    || !stream.get_ulong (members)) 
	  {
	    env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	    return 0;
	  }

	_private_state->tc_member_count_known_ = CORBA_B_TRUE;
	_private_state->tc_member_count_ = members;
	return _private_state->tc_member_count_;
      }
    case tk_union:
      {
	CORBA_ULong members;
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
	    env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	    return 0;
	  }

	_private_state->tc_member_count_known_ = CORBA_B_TRUE;
	_private_state->tc_member_count_ = members;
	return _private_state->tc_member_count_;
      }
    default:
      env.exception (new CORBA_BadKind ());
      return 0;
    }
}

// Return the name for the nth member
// Applicable only to tk_struct, tk_union, and tk_except
const CORBA_String 
CORBA_TypeCode::member_name (CORBA_ULong,
			     CORBA_Environment &) const
{
  return 0;
}

// NOTE special calling convention for stream.decode () when we're
// potentially deencapsulating an indirected typecode: the "data2"
// value indicates that this typecode is the parent.  See comments at
// stream.decode () for further details.
//
// Applicable only to struct, union, and except
//

CORBA_TypeCode_ptr
CORBA_TypeCode::private_member_type (CORBA_ULong index, 
				     CORBA_Environment &env) const
{
  CORBA_ULong temp, mcount;

  // Build the de-encapsulating CDR stream, bypassing the stringent
  // alignment tests (we're a bit looser in what we need here, and we
  // _know_ we're OK).  Then skip the byte order code.
  CDR stream;
  CORBA_TypeCode_ptr tc = 0;

  stream.setup_encapsulation (_buffer, (size_t)_length);

  switch (_kind) 
    {
    case tk_except:
    case tk_struct:		// index from 0
      mcount = member_count (env);		// clears env
      if (env.exception () == 0)
	{
	  // the first time in. Precompute and store types of all members
	  _private_state->tc_member_type_list_ = new CORBA_TypeCode_ptr [mcount];

	  if (_private_state->tc_member_type_list_)
	    {
	      // skip the id, name, and member_count part
	      if (!stream.skip_string ()		// type ID, hidden
		  || !stream.skip_string ()	// typedef name
		  || !stream.get_ulong (temp)) 
		{ 	// member count
		  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
		  return (CORBA_TypeCode_ptr)0;
		} 
	      else 
		{
		  // compute the typecodes for all the members and
		  // return the required one.
		  for (CORBA_ULong i = 0; i < mcount; i++) 
		    {
		      // the ith entry will have the typecode of the ith guy
		      if (!stream.skip_string ()  // skip the name
			  || stream.decode (_tc_CORBA_TypeCode,
					    &_private_state->tc_member_type_list_[i],
					    this, 
					    env) != CORBA_TypeCode::TRAVERSE_CONTINUE) 
			{
			  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
			  return 0;
			}
		    }

		  _private_state->tc_member_type_list_known_ = CORBA_B_TRUE;

		  if (index < mcount)
		    return _private_state->tc_member_type_list_[index];
		  else
		    {
		      env.exception (new CORBA_Bounds ());
		      return (CORBA_TypeCode_ptr)0;
		    }
		}
	    }
	  else  // no memory for the member_list
	    {
	      env.exception (new CORBA_NO_MEMORY (COMPLETED_NO));
	      return (CORBA_TypeCode_ptr)0;
	    }
	}
      else // out of bounds
	{
	  env.exception (new CORBA_Bounds ());
	  return (CORBA_TypeCode_ptr)0;
	}
    case tk_union:            // index from 0
      mcount = member_count (env);		// clears env
      if (env.exception () == 0)
	{
	  // the first time in. Precompute and store types of all members
	  _private_state->tc_member_type_list_ = new CORBA_TypeCode_ptr [mcount];
	  if (_private_state->tc_member_type_list_)
	    {
	      // skip the id, name, and discrimant type part
	      if (!stream.skip_string ()		// type ID, hidden
		  || !stream.skip_string ()	// typedef name
		  || stream.decode (_tc_CORBA_TypeCode,
				    &tc, 
				    this,
				    env) != CORBA_TypeCode::TRAVERSE_CONTINUE)  // TC
		{ 	// member count
		  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
		  return (CORBA_TypeCode_ptr)0;
		} 
	      else if (!stream.get_ulong (temp)	// default used
		       || !stream.get_ulong (temp))	// member count
		{
		  tc->Release ();
		  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
		  return 0;
		}
	      else  
		{
		  CORBA_Long scratch; // always big enough

		  // compute the typecodes for all the members and return the
		  // required one

		  for (CORBA_ULong i = 0; i < temp; i++) 
		    {
		      // the ith entry will have the typecode of the ith guy
		      if (stream.decode (tc, &scratch, this,  env) // member label
			  != CORBA_TypeCode::TRAVERSE_CONTINUE
			  || !stream.skip_string ()  // skip the name
			  || stream.decode (_tc_CORBA_TypeCode,  // get the typecode
					    &_private_state->tc_member_type_list_[i],
					    this, 
					    env) !=
			  CORBA_TypeCode::TRAVERSE_CONTINUE)
			{
			  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
			  return 0;
			}
		    }
		  _private_state->tc_member_type_list_known_ = CORBA_B_TRUE;

		  if (index < mcount)
		    return _private_state->tc_member_type_list_[index];
		  else
		    {
		      env.exception (new CORBA_Bounds ());
		      return (CORBA_TypeCode_ptr)0;
		    }
		}
	    }
	  else  // no memory for the member_list
	    {
	      env.exception (new CORBA_NO_MEMORY (COMPLETED_NO));
	      return (CORBA_TypeCode_ptr)0;
	    }
	}
      else // out of bounds
	{
	  env.exception (new CORBA_Bounds ());
	  return (CORBA_TypeCode_ptr)0;
	}

    default:
      // bad kind
      env.exception (new CORBA_BadKind ());
      return (CORBA_TypeCode_ptr)0;
    }
}

// Return member labels for tk_union typecodes.
CORBA_Any_ptr
CORBA_TypeCode::private_member_label (CORBA_ULong n,
			      CORBA_Environment &env) const
{
  env.clear ();

  // this function is only applicable to the tk_union TC
  if (_kind == tk_union)
    {
      CDR stream;

      stream.setup_encapsulation (_buffer, (size_t)_length);

      // skip ID and name, and then get the discriminant TC
      CORBA_TypeCode_ptr    tc = 0;

      if (!stream.skip_string ()		// type ID, hidden
	  || !stream.skip_string ()	// typedef name
	  || stream.decode (_tc_CORBA_TypeCode, &tc, this,  env)
	  != CORBA_TypeCode::TRAVERSE_CONTINUE) 
	{
	  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	  return 0;
	}

      // skip default used, and get member count
      CORBA_ULong member_count;

      if (!stream.get_ulong (member_count)	// default used
	  || !stream.get_ulong (member_count)) 
	{
	  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	  dmsg ("TypeCode::private_member_label -- error reading from stream");
	  return 0;
	}

      _private_state->tc_member_label_list_ = new CORBA_Any_ptr [member_count];
      if (_private_state->tc_member_label_list_)
	{
	  for (CORBA_ULong i = 0; i < member_count; i++) 
	    {
	      // allocate buffer to hold the member label value
	      void *buf = new CORBA_Octet [tc->size (env)];
	      if (stream.decode (tc, buf, this,  env)
		  != CORBA_TypeCode::TRAVERSE_CONTINUE
		  || !stream.skip_string ()   	    // member name
		  || !skip_typecode (stream)) // member type
		{	    // member TC
		  dmsg1 ("TypeCode::private_member_label -- error getting typecode for member %d",i);
		  delete [] buf;
		  // XXX-Andy: free this list on error
		  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
		  return 0;
		}
	      else
		ACE_NEW_RETURN (_private_state->tc_member_label_list_[i], 
				CORBA_Any (tc, buf, CORBA_B_TRUE),
				0);
	    }
	}
      else
	{
	  env.exception (new CORBA_NO_MEMORY (COMPLETED_NO));
	  return 0;
	}

      _private_state->tc_member_label_list_known_ = CORBA_B_TRUE;
      tc->Release ();

      // If caller asked for the label for a nonexistent member, they get
      // an error report!
      if (n >= member_count)  
	{
	  env.exception (new CORBA_Bounds ());
	  return 0;
	}
      else
	return _private_state->tc_member_label_list_[n];
    }
  else // wrong typecode
    {
      env.exception (new CORBA_BadKind ());
      return 0;
    }
}

CORBA_TypeCode_ptr 
CORBA_TypeCode::private_discriminator_type (CORBA_Environment &env) const
{
  CDR stream;
  
  stream.setup_encapsulation (_buffer, (size_t)_length);

  // skip ID and name, and then get the discriminant TC
  
  if (!stream.skip_string ()		// type ID, hidden
      || !stream.skip_string ()	        // typedef name
      || stream.decode (_tc_CORBA_TypeCode,
			&_private_state->tc_discriminator_type_, this,  env) !=
      CORBA_TypeCode::TRAVERSE_CONTINUE) 
    {
      env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
      return 0;
    }
  else
    {
      _private_state->tc_discriminator_type_known_ = CORBA_B_TRUE;
      return _private_state->tc_discriminator_type_;
    }
}

CORBA_Long
CORBA_TypeCode::private_default_index (CORBA_Environment &env) const
{
  CDR stream;
  CORBA_TypeCode_ptr tc = 0;

  stream.setup_encapsulation (_buffer, (size_t)_length);

  // skip ID and name, and then get the discriminant TC
  
  if (!stream.skip_string ()		// type ID, hidden
      || !stream.skip_string ()	        // typedef name
      || stream.decode (_tc_CORBA_TypeCode,
			&tc, this,  env) != CORBA_TypeCode::TRAVERSE_CONTINUE
      || !stream.get_long (_private_state->tc_default_index_used_))
      
    {
      env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
      return 0;
    }
  else
    {
      _private_state->tc_default_index_used_known_ = CORBA_B_TRUE;
      return _private_state->tc_default_index_used_;
    }
}

CORBA_Long
CORBA_TypeCode::private_length (CORBA_Environment &env) const
{
  CDR stream;
  CORBA_TypeCode_ptr tc = 0;

  stream.setup_encapsulation (_buffer, (size_t)_length);
  switch (_kind)
    {
    case tk_sequence:
    case tk_array:
      {
	// skip the typecode of the element and get the bounds
	if (stream.decode (_tc_CORBA_TypeCode, &tc, this,  env) !=
	    CORBA_TypeCode::TRAVERSE_CONTINUE // skip typecode
	    || !stream.get_ulong (_private_state->tc_length_))
	  {
	    env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
	    return 0;
	  }
	else
	  {
	    _private_state->tc_length_known_ = CORBA_B_TRUE;
	    return _private_state->tc_length_;
	  }
	break;
      case tk_string:
      case tk_wstring:
	{
	  if (stream.get_ulong (_private_state->tc_length_))
	    {
	      _private_state->tc_length_known_ = CORBA_B_TRUE;
	      return _private_state->tc_length_;
	    }
	  else
	    {
	      env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
	      return 0;
	    }
	}
      default:
	env.exception (new CORBA_BadKind);
	return 0;
      }
    }
}

CORBA_TypeCode_ptr
CORBA_TypeCode::private_content_type (CORBA_Environment &env) const
{
  CDR stream;

  stream.setup_encapsulation (_buffer, (size_t) _length);
  switch (_kind)
    {
    case tk_sequence:
    case tk_array:
      {
	// skip the typecode of the element and get the bounds
	if (stream.decode (_tc_CORBA_TypeCode, &_private_state->tc_content_type_,
			   this,  env) != 
	    CORBA_TypeCode::TRAVERSE_CONTINUE) // element type
	  {
	    env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
	    return 0;
	  }
	else
	  {
	    _private_state->tc_content_type_known_ = CORBA_B_TRUE;
	    return _private_state->tc_content_type_;
	  }
	break;
      case tk_alias:
	{
	  if (!stream.skip_string ()  // typeID
	      || !stream.skip_string () // name
	      || stream.decode (_tc_CORBA_TypeCode,
				&_private_state->tc_content_type_, this,  env) != 
	      CORBA_TypeCode::TRAVERSE_CONTINUE)
	    {
	      env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
	      return 0;
	    }
	  else
	    {
	      _private_state->tc_content_type_known_ = CORBA_B_TRUE;
	      return _private_state->tc_content_type_;
	    }
	}
      default:
	env.exception (new CORBA_BadKind);
	return 0;
      }
    }
}

CORBA_ULong
CORBA_TypeCode::private_discrim_pad_size (CORBA_Environment &env)
{
  CDR stream;
  size_t discrim_size,
    overall_align;

  stream.setup_encapsulation (_buffer, (size_t)_length);

 (void) calc_key_union_attributes (&stream, overall_align, discrim_size, env);

  if (env. exception () == 0)
    {
      _private_state->tc_discrim_pad_size_known_ = CORBA_B_TRUE;
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

CORBA_ULong
CORBA_TypeCode::param_count (CORBA_Environment &env) const
{
  env.clear ();

  switch (_kind) 
    {
    default:
      return 0;

    case tk_string:
    case tk_wstring:
      return 1;

    case tk_objref:
    case tk_sequence:
    case tk_array:
      return 2;

    case tk_alias:
      return 3;

    case tk_except:
    case tk_struct:
      {
	CORBA_ULong members;
	CDR stream;

	stream.setup_encapsulation (_buffer, (size_t)_length);

	// skip rest of header (type ID and name) and collect the
	// number of struct members
	if (!stream.skip_string ()	    	    // ID
	    || !stream.skip_string ()  	    // struct name
	    || !stream.get_ulong (members)) 
	  {
	    env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	    return 0;
	  }

	return 3 + 2 * members;
      }
    case tk_enum:
      {
	CORBA_ULong members;
	CDR stream;

	stream.setup_encapsulation (_buffer, (size_t)_length);

	// skip rest of header (type ID and name) and collect the
	// number of struct members
	if (!stream.skip_string ()			// ID
	    || !stream.skip_string ()		// typedef name
	    || !stream.get_ulong (members)) 
	  {
	    env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	    return 0;
	  }

	return 3 + members;
      }
    case tk_union:
      {
	CORBA_ULong members;
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
	    env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	    return 0;
	  }

	return 5 + 3 * members;
      }
    }
}


// Internal hack, used until member_count () and length () are
// implemented.  Doesn't support all the types that those routines
// support.

CORBA_ULong
CORBA_TypeCode::ulong_param (CORBA_ULong n,
			     CORBA_Environment &env) const
{
  CORBA_ULong temp;

  temp = param_count (env);		// clears env
  if (env.exception ())
    return 0;

  if (temp < n) 
    {
      env.exception (new CORBA_Bounds);
      return 0;
    }

  // Get parameters for non-empty typecodes; their parameter lists are
  // encapsulated CDR (for complex ones) or inlined (for simple ones).
  switch (_kind) 
    {
    default:				// most have no long params
      break;

      // Array, sequence ... complex parameter lists
    case tk_array:		// param 1 is an integer
    case tk_sequence:		// ... identical content
      { 
	if (n == 0)
	  break;

	// Build CDR stream for encapsulated params, and skip the
	// typecode up front.
	CDR stream;

	stream.setup_encapsulation (_buffer, (size_t)_length);
	if (!skip_typecode (stream)) 
	  {
	    env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
	    return 0;
	  }

	// Then comes the "bounds" parameter.
	if (!stream.get_ulong (temp))
	  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	return temp;
      }

    // string, wstring ... simple parameter lists, containing just the
    // string bounds (zero indicates unbounded).  Stored specially
    case tk_string:
    case tk_wstring:
      if (n != 0)
	break;
      return _length;
    }
  env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
  return 0;
}

// Internal hack, used until member_type (), discriminator_type (),
// and content_type () are implemented.
//
// NOTE special calling convention for stream.decode () when we're
// potentially deencapsulating an indirected typecode: the "data2"
// value indicates that this typecode is the parent.  See comments at
// stream.decode () for further details.

CORBA_TypeCode_ptr
CORBA_TypeCode::typecode_param (CORBA_ULong n,
				CORBA_Environment &env) const
{
  CORBA_ULong temp;

  temp = param_count (env);		// clears env
  if (env.exception ())
    return 0;

  if (temp < n) 
    {
      env.exception (new CORBA_Bounds);
      return 0;
    }

  // Build the de-encapsulating CDR stream, bypassing the stringent
  // alignment tests (we're a bit looser in what we need here, and we
  // _know_ we're OK).  Then skip the byte order code.

  CDR stream;
  CORBA_TypeCode_ptr tc = 0;

  stream.setup_encapsulation (_buffer, (size_t)_length);

  switch (_kind) 
    {
    default:				// most have no tc params
      break;

    case tk_sequence:		// param 0 is a tc
    case tk_array:
      if (n != 0)
	break;
      if (stream.decode (_tc_CORBA_TypeCode,
			 &tc,
			 this,
			 env) != CORBA_TypeCode::TRAVERSE_CONTINUE) 
	{
	  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	  return 0;
	}
      return tc;

    case tk_alias:		// #1 is a tc
      if (n != 2)
	break;
      if (!stream.skip_string ()		// type ID, hidden
	  || !stream.skip_string ()	// typedef name
	  || stream.decode (_tc_CORBA_TypeCode, &tc, this,  env)
	  != CORBA_TypeCode::TRAVERSE_CONTINUE) 
	{
	  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	  return 0;
	}
      return tc;

    case tk_except:
    case tk_struct:		// #5 and up are tc, index from 0
      if (n < 4 || n & 0x1) 
	{	// tc is at odd number of param list
	  env.exception (new CORBA_Bounds);
	  return 0;
	}

      if (!stream.skip_string ()		// type ID, hidden
	  || !stream.skip_string ()	// typedef name
	  || !stream.get_ulong (temp)) 
	{ 	// member count
	  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	  return 0;
	} 
      else 
	{
	  temp = (n - 3) / 2;

	  // skip member pairs to the one we want
	  for (CORBA_ULong i = 0; i < temp; i++) 
	    {
	      // skip to the member being asked
	      if (!stream.skip_string ()	// member name
		  || !skip_typecode (stream)) 
		{
		  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
		  return 0;
		}
	    }

	  if (!stream.skip_string ()
	      || stream.decode (_tc_CORBA_TypeCode, &tc, this, 
			       env)!= CORBA_TypeCode::TRAVERSE_CONTINUE) 
	    {
	      env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	      return 0;
	    }
	  return tc;
	}

    case tk_union:            // #6 and up are tc, index from 0
      if (n != 2 && (n < 7 || (n - 7) % 3)) 
	{
	  env.exception (new CORBA_Bounds);
	  return 0;
	}

      if (!stream.skip_string ()		// type ID, hidden
	  || !stream.skip_string ()	// typedef name
	  || stream.decode (_tc_CORBA_TypeCode,
			   &tc, this,
			    env) != CORBA_TypeCode::TRAVERSE_CONTINUE)  // TC
	{
	  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	  return 0;
	} 
      else if (!stream.get_ulong (temp)	// default used
		 || !stream.get_ulong (temp))	// member count
	{
	  tc->Release ();
	  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	  return 0;
	}

      if (n == 2)
	return tc;

      temp = (n - 7) / 3;

      // skip to the member requested

      CORBA_Long scratch;		// always big enough

      for (CORBA_ULong i = 0; i < temp; i++) 
	{
	  if (stream.decode (tc, &scratch, this,  env) // member label
	      != CORBA_TypeCode::TRAVERSE_CONTINUE
	      || !stream.skip_string ()		// member name
	      || !skip_typecode (stream)) 
	    {	// member typecode
	      tc->Release ();
	      env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	      return 0;
	    }
	}

      // member label
      if (stream.decode (tc, 
			&scratch, this,
			 env) != CORBA_TypeCode::TRAVERSE_CONTINUE
	  || !stream.skip_string ())		// member name
	{
	  tc->Release ();
	  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	  return 0;
	}
      tc->Release ();

      if (stream.decode (_tc_CORBA_TypeCode,
			&tc, this,
			 env) != CORBA_TypeCode::TRAVERSE_CONTINUE) 
	{
	  env.exception (new CORBA_BAD_TYPECODE (COMPLETED_NO));
	  return 0;
	}
      return tc;
    }

  env.exception (new CORBA_BAD_PARAM (COMPLETED_NO));
  return 0;
}

// skip a typecode encoding in a given CDR stream
// This is just a helper function
CORBA_Boolean 
CORBA_TypeCode::skip_typecode (CDR &stream)
{
  CORBA_ULong kind;
  CORBA_ULong temp;

  if (stream.get_ulong (kind) 
      && (kind < TC_KIND_COUNT || kind == ~0))
    {

      switch (kind) 
	{
	  // Most TypeCodes have empty parameter lists, nothing to skip
	default:
	  break;

	  // Some have single integer parameters, easy to skip.  Some have
	  // preallocated constants that could be used.
	case tk_string:
	case tk_wstring:
	case ~0:
	  return stream.get_ulong (temp);

	  // The rest have "complex" parameter lists that are
	  // encoded as bulk octets ... just skip them.
	case tk_objref:
	case tk_struct:
	case tk_union:
	case tk_enum:
	case tk_sequence:
	case tk_array:
	case tk_alias:
	case tk_except:
	  return stream.get_ulong (temp) != CORBA_B_FALSE
	    && stream.skip_bytes (temp) != CORBA_B_FALSE;
	}

      return CORBA_B_TRUE;
    }
  else
    return CORBA_B_FALSE;
}


