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

#include "tao/corba.h"

// @@ This is a botch...
// @@ Can you please explain why?
size_t
calc_key_union_attributes (CDR *stream,
			   size_t &alignment,
			   size_t &size_with_pad,
			   CORBA::Environment &env);

// Constructor for CONSTANT typecodes with empty parameter lists.
// These are only created once, and those constants are shared.

CORBA_TypeCode::CORBA_TypeCode (CORBA::TCKind kind)
  : length_ (0),
    buffer_ (0),
    kind_ (kind),
    parent_ (0),
    refcount_ (1),
    delete_flag_ (CORBA::B_FALSE),
    orb_owns_ (CORBA::B_TRUE),
    private_state_ (new TC_Private_State (kind)),
    non_aligned_buffer_ (0)
{
}

// Constructor for all other typecodes, including constants with
// non-empty parameter lists.  See "corba.hh" for details.

CORBA_TypeCode::CORBA_TypeCode (CORBA::TCKind kind,
				CORBA::ULong length,
				CORBA::Octet *buffer,
				CORBA::Boolean orb_owns_tc,
				CORBA::TypeCode_ptr parent)
  : length_ (length),
    //    buffer_ (buffer),
    kind_ (kind),
    parent_ (parent),
    refcount_ (1),
    delete_flag_ (CORBA::B_FALSE),
    orb_owns_ (orb_owns_tc),
    private_state_ (new TC_Private_State (kind))
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

  // TAO comments:

  // For free standing typecodes, we choose to always make a copy of
  // the buffer passed in. That way, our destructor doesn't have to
  // deal with the case where the buffer was either allocated in which
  // case it must be freed or the case where our buffer just points to
  // the buffer passed in.

  if (!parent_)
    {
      // No parent. We are free standing.
      ptr_arith_t temp;

      // Allocate a buffer to hold the encapsulated stream. We
      // allocate extra space since we need a buffer that is aligned
      // on a 4 byte word boundary. As a result, it is quite possible
      // that we may start accessing the buffer from a position
      // shifted to the right in the allocated buffer. As a result,
      // during destruction, we do not want part of the allocated heap
      // to remain dangling. Hence we save a handle to the original
      // allocated buffer.

      this->non_aligned_buffer_ = new CORBA::Octet [length + 4];

      temp = (ptr_arith_t) non_aligned_buffer_;
      temp += 3;
      temp &= ~0x03;
      this->buffer_ = (CORBA::Octet *) temp;

      (void) ACE_OS::memcpy (this->buffer_, buffer, (size_t) length);

      // The ORB does not own this typecode.
      this->orb_owns_ = CORBA::B_FALSE;
    }
  else
    {
      // We are a child. We do not allocate a new buffer, but share it
      // with our parent. We know that our parent's buffer was
      // properly aligned.
      this->buffer_ = buffer;
    }
}

// Destructor.  For "indirected" typecodes and children, the typecode
// reuses the buffer owned by its parent.

void
CORBA_TypeCode::operator delete (void* p)
{
  CORBA::TypeCode_ptr tc = (CORBA_TypeCode *) p;
  if (!tc->orb_owns_)
    ::delete p;
}

CORBA_TypeCode::~CORBA_TypeCode (void)
{
  if (this->orb_owns_)
    // we are constants, don't do anything
    return;
  else if (this->parent_) // check if we have a parent
    {
      // We have a parent which means that we were not directly
      // created by IDL compiler generated code, but by the
      // precomputation logic. We should delete ourselves and the
      // subtree below us only if our parent was in the process of
      // deleteing itself
      if (parent_->delete_flag_)
        // Parent is deleting, so we have to go.
	{
          // Set our delete flag to TRUE so that our children (if any)
          // will know that we have initiated our destruction
	  this->delete_flag_ = CORBA::B_TRUE;

          // Delete any private state we have and thus free up the
          // children.
	  delete this->private_state_;

	  // We share the buffer octets of our parent. Hence we don't
	  // deallocate it.
	  this->buffer_ = 0;
	}
      // Else, somebody maliciously tried to delete us, but we won't
      // get deleted.
    }
  else
    {
      // We are free standing (IDL compiler generated code) and are to
      // be deleted.  We indicate to our children that we are getting
      // deleted.
      this->delete_flag_ = CORBA::B_TRUE;

      // Free up our children.
      delete this->private_state_;

      // Delete the original, possibly nonaligned, buffer.
      delete [] this->non_aligned_buffer_;
      this->buffer_ = 0;
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

// Return the i-th member typecode if it exists, else raise an
// exception. Possible exceptions are BadKind and Bounds.
//
// Applicable only to struct, union, and except

CORBA::TypeCode_ptr
CORBA_TypeCode::member_type (CORBA::ULong index,
			     CORBA::Environment &env) const
{
  if (this->private_state_->tc_member_count_known_
      && this->private_state_->tc_member_type_list_known_)
    {
      if (index < this->private_state_->tc_member_count_)
        return this->private_state_->tc_member_type_list_[index];
      else
        {
          env.exception (new CORBA::TypeCode::Bounds ());
          return 0;
        }
    }
  else
    return this->private_member_type (index, env);
}

// Applicable only to struct, union, and except

const char *
CORBA_TypeCode::member_name (CORBA::ULong index,
			     CORBA::Environment &env) const
{
  if (this->private_state_->tc_member_count_known_
      && this->private_state_->tc_member_name_list_known_)
    {
      if (index < this->private_state_->tc_member_count_)
        return this->private_state_->tc_member_name_list_[index];
      else
        {
          env.exception (new CORBA::TypeCode::Bounds ());
          return 0;
        }
    }
  else
    return this->private_member_name (index, env);
}

// Return the label of the i-th member.  Applicable only to CORBA::tk_union
CORBA::Any_ptr
CORBA_TypeCode::member_label (CORBA::ULong index,
                              CORBA::Environment &env) const
{
  if (this->private_state_->tc_member_count_known_
      && this->private_state_->tc_member_label_list_known_)
    {
      if (index < this->private_state_->tc_member_count_)
        return this->private_state_->tc_member_label_list_[index];
      else
        {
          env.exception (new CORBA::TypeCode::Bounds ());
          return 0;
        }
    }
  else
    return this->private_member_label (index, env);
}

// only applicable to CORBA::tk_unions
CORBA::TypeCode_ptr
CORBA_TypeCode::discriminator_type (CORBA::Environment &env) const
{
  if (this->kind_ == CORBA::tk_union)
    {
      if (this->private_state_->tc_discriminator_type_known_)
        return this->private_state_->tc_discriminator_type_;
      else
        return this->private_discriminator_type (env);
    }
  else
    {
      env.exception (new CORBA::TypeCode::BadKind ());
      return (CORBA::TypeCode_ptr)0;
    }
}

// only applicable to CORBA::tk_unions
CORBA::Long
CORBA_TypeCode::default_index (CORBA::Environment &env) const
{
  if (this->kind_ == CORBA::tk_union)
    {
      if (this->private_state_->tc_default_index_used_known_)
        return this->private_state_->tc_default_index_used_;
      else
        return this->private_default_index (env);
    }
  else
    {
      env.exception (new CORBA::TypeCode::BadKind ());
      return 0;
    }
}

// returns the length. Applicable only to string, sequence, and arrays
CORBA::ULong
CORBA_TypeCode::length (CORBA::Environment &env) const
{
  // a switch stmt, unfortunately, doesn't get inlined
  if (this->kind_ == CORBA::tk_sequence
      || this->kind_ == CORBA::tk_array
      || this->kind_ == CORBA::tk_string
      || this->kind_ == CORBA::tk_wstring)
    {
      if (this->private_state_->tc_length_known_)
        return this->private_state_->tc_length_;
      else
        return this->private_length (env);
    }
  else
    {
      env.exception (new CORBA::TypeCode::BadKind ());
      return 0;
    }
}

// returns the typecode. Applicable only to string, sequence, and arrays
CORBA::TypeCode_ptr
CORBA_TypeCode::content_type (CORBA::Environment &env) const
{
  if (this->kind_ == CORBA::tk_sequence
      || this->kind_ == CORBA::tk_array
      || this->kind_ == CORBA::tk_alias)
    {
      if (this->private_state_->tc_content_type_known_)
        return this->private_state_->tc_content_type_;
      else
        return this->private_content_type (env);
    }
  else
    {
      env.exception (new CORBA::TypeCode::BadKind ());
      return 0;
    }
}

// compute the padded size of the discriminant
CORBA::ULong
CORBA_TypeCode::TAO_discrim_pad_size (CORBA::Environment &env)
{
  if (this->kind_ == CORBA::tk_union)
    {
      if (this->private_state_->tc_discrim_pad_size_known_)
        return this->private_state_->tc_discrim_pad_size_;
      else
        return this->private_discrim_pad_size (env);
    }
  else
    {
      env.exception (new CORBA::TypeCode::BadKind ());
      return 0;
    }
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

// constructor for the private state
TC_Private_State::TC_Private_State (CORBA::TCKind kind)
  : tc_kind_ (kind),
    tc_id_known_ (CORBA::B_FALSE),
    tc_name_known_ (CORBA::B_FALSE),
    tc_member_count_known_ (CORBA::B_FALSE),
    tc_member_type_list_known_ (CORBA::B_FALSE),
    tc_member_name_list_known_ (CORBA::B_FALSE),
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
    tc_member_name_list_ (0),
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
  // the following two just point into the buffer. So we just make it point to NUL
  this->tc_id_ = 0;
  this->tc_name_ = 0;

  // determine what kind of children we may have and free the space accordingly
  switch (this->tc_kind_)
    {
    case CORBA::tk_enum:
        // free up the member name list
        if (this->tc_member_name_list_known_)
          {
            for (CORBA::ULong i = 0;
                 i < this->tc_member_count_;
                 i++)
              {
                this->tc_member_name_list_ [i] = 0; // not owned by us
              }
            delete [] this->tc_member_name_list_;
          }
        break;
    case CORBA::tk_struct:
    case CORBA::tk_except:
      {
        // free up the member name list
        if (this->tc_member_name_list_known_)
          {
            for (CORBA::ULong i = 0;
                 i < this->tc_member_count_;
                 i++)
              {
                this->tc_member_name_list_ [i] = 0; // not owned by us
              }
            delete [] this->tc_member_name_list_;
          }

        // free up member type list
        if (this->tc_member_type_list_known_)
          {
            for (CORBA::ULong i = 0;
                 i < this->tc_member_count_;
                 i++)
              {
                // free up the memory allocated for the typecode only if it has a parent
                if (this->tc_member_type_list_[i]->parent_)
                  delete this->tc_member_type_list_[i];
              }
            // now free up the array
            delete [] this->tc_member_type_list_;
          }
        this->tc_member_count_ = 0;
      }
      break;
    case CORBA::tk_sequence:
    case CORBA::tk_array:
    case CORBA::tk_alias:
      // delete the content type only if it has a parent i.e., if it is not
      // acquired from the pool of constant or predefined typecodes
      if (this->tc_content_type_known_)
        if (this->tc_content_type_->parent_)
          delete this->tc_content_type_;
      break;
    case CORBA::tk_union:
      {
        // free up the member name list
        if (this->tc_member_name_list_known_)
          {
            for (CORBA::ULong i = 0;
                 i < this->tc_member_count_;
                 i++)
              {
                this->tc_member_name_list_ [i] = 0; // not owned by us
              }
            delete [] this->tc_member_name_list_;
          }

        // Free up type list, label list, and finally the discriminator
        if (this->tc_member_type_list_known_)
          {
            for (CORBA::ULong i = 0;
                 i < this->tc_member_count_;
                 i++)
              {
                // free up the memory allocated for the typecode if it has a
                // parent that owns it
                if (this->tc_member_type_list_[i]->parent_)
                  delete this->tc_member_type_list_[i];
              }
            // now free up the array
            delete [] this->tc_member_type_list_;
          }
        if (this->tc_member_label_list_known_)
          {
            for (CORBA::ULong i = 0;
                 i < this->tc_member_count_;
                 i++)
              {
                // free up the label (Any_ptr)
                delete this->tc_member_label_list_[i];
              }
            delete [] this->tc_member_label_list_;
          }
        this->tc_member_count_ = 0;
        // Discriminator must come last b/c it will be inside the Any
        // in each element of the label list.
        delete this->tc_discriminator_type_;
      }
      break;
    default:
      // nothing to do
      break;
    }
}

// COM's IUnknown support

// {A201E4C1-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_TypeCode,
0xa201e4c1, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

// COM stuff
u_long
CORBA_TypeCode::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, lock_, 0));

  assert (this != 0);

  if (this->orb_owns_)
    return this->refcount_; // this better be 1
  else if (parent_)
    // we are owned by the parent
    //	  return parent_->Addref ();
    return this->refcount_; // 1
  else
    return this->refcount_++;
}

// COM stuff
u_long
CORBA_TypeCode::Release (void)
{
  // This code is subtle since we need to make sure that we don't try
  // to release the lock after we've deleted this...
  ACE_MT (this->lock_.acquire ());

  ACE_ASSERT (this != 0);

  u_long result;

  if (this->orb_owns_)
    result = this->refcount_; // 1
  else if (this->parent_)
    // return parent_->Release ();
    result = this->refcount_; // 1
  else
    {
      result = --this->refcount_;
      ACE_MT (this->lock_.release ());

      if (result == 0)
        delete this;

      return result;
    }

  ACE_MT (this->lock_.release ());
  return result;
}

// COM stuff
TAO_HRESULT
CORBA_TypeCode::QueryInterface (REFIID riid,
				void **ppv)
{
  *ppv = 0;

  if (IID_CORBA_TypeCode == riid || IID_TAO_IUnknown == riid)
    *ppv = this;

  if (*ppv == 0)
    return ResultFromScode (TAO_E_NOINTERFACE);

 (void) AddRef ();
  return TAO_NOERROR;
}

// check if typecodes are equal. Equality is based on a mix of structural and
// name equivalence i.e., if names are provided, we also check for name
// equivalence, else resort simply to structural equivalence.
CORBA::Boolean
CORBA_TypeCode::private_equal (CORBA::TypeCode_ptr tc,
			       CORBA::Environment &env) const
{
  // We come in here only if the typecode kinds of both are same
  // Handle each complex typecode separately.
  switch (this->kind_)
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
      // the kind_ field
      return CORBA::B_TRUE;
    case CORBA::tk_objref:
      this->private_equal_objref (tc, env);
    case CORBA::tk_struct:
      this->private_equal_struct (tc, env);
    case CORBA::tk_union:
      this->private_equal_union (tc, env);
    case CORBA::tk_enum:
      this->private_equal_enum (tc, env);
    case CORBA::tk_string:
      this->private_equal_string (tc, env);
    case CORBA::tk_wstring:
      this->private_equal_wstring (tc, env);
    case CORBA::tk_sequence:
      this->private_equal_sequence (tc, env);
    case CORBA::tk_array:
      this->private_equal_array (tc, env);
    case CORBA::tk_alias:
      this->private_equal_alias (tc, env);
    case CORBA::tk_except:
      this->private_equal_except (tc, env);
    default:
      return CORBA::B_TRUE;
    }
}

CORBA::Boolean
CORBA_TypeCode::private_equal_objref (CORBA::TypeCode_ptr tc,
                                      CORBA::Environment &env) const
{
  env.clear ();
  // compare the repoID and name, of which the name is optional as per GIOP
  // spec
  if (!ACE_OS::strcmp (this->id (env), tc->id (env)))
    {
      // same repository IDs. Now check their names
      const char *myname = this->name (env);
      if (env.exception ())
        return 0;
      const char *tcname = tc->name (env);
      if (env.exception ())
        return 0;
      if ((ACE_OS::strlen (myname) > 1) &&
          (ACE_OS::strlen (tcname) > 1))
        {
          // both of them specify names, compare them
          if (!ACE_OS::strcmp (myname, tcname))
            return 1; // success
          else
            return 0; // failed
        }
      return 1; // equal
    }
  return 0; // failed
}

CORBA::Boolean
CORBA_TypeCode::private_equal_struct (CORBA::TypeCode_ptr tc,
                                      CORBA::Environment &env) const
{
  env.clear ();

  // for structs the repoID and names are optional. However, if provided, we
  // must compare them
  const char *my_id = this->id (env);
  if (env.exception ())
    return 0;
  const char *tc_id = tc->id (env);
  if (env.exception ())
    return 0;
  const char *my_name = this->id (env);
  if (env.exception ())
    return 0;
  const char *tc_name = tc->id (env);
  if (env.exception ())
    return 0;

  // compare repoIDs if they exist
  if (ACE_OS::strlen (my_id) > 1 && ACE_OS::strlen (tc_id) > 1)
    if (ACE_OS::strcmp (my_id, tc_id)) // not same
      return 0;

  // compare names if they exist
  if (ACE_OS::strlen (my_name) > 1 && ACE_OS::strlen (tc_name) > 1)
    if (ACE_OS::strcmp (my_name, tc_name)) // not same
      return 0;

  // check if the member count is same
  CORBA::ULong my_count = this->member_count (env);
  if (env.exception ())
    return 0;
  CORBA::ULong tc_count = tc->member_count (env);
  if (env.exception ())
    return 0;

  if (my_count != tc_count)
    return 0; // number of members don't match

  for (CORBA::ULong i=0; i < my_count; i++)
    {
      const char *my_member_name = this->member_name (i, env);
      if (env.exception ())
        return 0;

      const char *tc_member_name = this->member_name (i, env);
      if (env.exception ())
        return 0;

      if (ACE_OS::strlen (my_member_name) > 1 && ACE_OS::strlen
          (tc_member_name) > 1)
        if (ACE_OS::strcmp (my_member_name, tc_member_name)) // not same
          return 0;

      CORBA::TypeCode_ptr my_member_tc = this->member_type (i, env);
      if (env.exception ())
        return 0;

      CORBA::TypeCode_ptr tc_member_tc = tc->member_type (i, env);
      if (env.exception ())
        return 0;

      CORBA::Boolean flag = my_member_tc->equal (tc_member_tc, env);
      if (!flag || env.exception ())
        return 0;
    }

  return 1;
}

CORBA::Boolean
CORBA_TypeCode::private_equal_union (CORBA::TypeCode_ptr tc,
                                     CORBA::Environment &env) const
{
  ACE_UNUSED_ARG (tc);
  ACE_UNUSED_ARG (env);
  return 1;
}

CORBA::Boolean
CORBA_TypeCode::private_equal_enum (CORBA::TypeCode_ptr tc,
                                    CORBA::Environment &env) const
{
  ACE_UNUSED_ARG (tc);
  ACE_UNUSED_ARG (env);
  return 1;
}

CORBA::Boolean
CORBA_TypeCode::private_equal_string (CORBA::TypeCode_ptr tc,
                                      CORBA::Environment &env) const
{
  ACE_UNUSED_ARG (tc);
  ACE_UNUSED_ARG (env);
  return 1;
}

CORBA::Boolean
CORBA_TypeCode::private_equal_wstring (CORBA::TypeCode_ptr tc,
                                       CORBA::Environment &env) const
{
  ACE_UNUSED_ARG (tc);
  ACE_UNUSED_ARG (env);
  return 1;
}

CORBA::Boolean
CORBA_TypeCode::private_equal_sequence (CORBA::TypeCode_ptr tc,
                                        CORBA::Environment &env) const
{
  ACE_UNUSED_ARG (tc);
  ACE_UNUSED_ARG (env);
  return 1;
}

CORBA::Boolean
CORBA_TypeCode::private_equal_array (CORBA::TypeCode_ptr tc,
                                     CORBA::Environment &env) const
{
  ACE_UNUSED_ARG (tc);
  ACE_UNUSED_ARG (env);
  return 1;
}

CORBA::Boolean
CORBA_TypeCode::private_equal_alias (CORBA::TypeCode_ptr tc,
                                     CORBA::Environment &env) const
{
  ACE_UNUSED_ARG (tc);
  ACE_UNUSED_ARG (env);
  return 1;
}

CORBA::Boolean
CORBA_TypeCode::private_equal_except (CORBA::TypeCode_ptr tc,
                                      CORBA::Environment &env) const
{
  ACE_UNUSED_ARG (tc);
  ACE_UNUSED_ARG (env);
  return 1;
}

// Return the type ID (RepositoryId) for the TypeCode; it may be empty.
//
// NOTE the string returned here is owned by the typecode!!
//
// Valid only for objref, struct, union, enum, alias, and except. Raises
// BadKind exception for the rest of the cases.
const char *
CORBA_TypeCode::private_id (CORBA::Environment &env) const
{
  env.clear ();

  switch (this->kind_)
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
	this->private_state_->tc_id_known_ = CORBA::B_TRUE;
	this->private_state_->tc_id_ = (CORBA::String) (buffer_
						 + 4	// skip byte order flag
							// and padding
						 + 4);	// skip (strlen + 1)
	return this->private_state_->tc_id_; // this is OK because the strings in the
				       // CDR stream are NULL terminated
      }
    // No other typecodes ever have type IDs
    default:
      env.exception (new CORBA::TypeCode::BadKind ());
      return 0;
    }
}

// return the name. The string is owned by the typecode
const char *
CORBA_TypeCode::private_name (CORBA::Environment &env) const
{
  env.clear ();

  switch (this->kind_)
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

	stream.setup_encapsulation (this->buffer_, (size_t) length_);

	// skip the typecode ID
	if (stream.skip_string ())  // ID
	  {
	    this->private_state_->tc_name_known_ = CORBA::B_TRUE;

	    // skip past the length field.
	    this->private_state_->tc_name_ = (CORBA::String) (stream.next +
                                                              CDR::LONG_SIZE);

	    return this->private_state_->tc_name_;
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

  switch (kind_)
    {
    case CORBA::tk_alias:
      return 1;

    case CORBA::tk_enum:
    case CORBA::tk_except:
    case CORBA::tk_struct:
      {
	CORBA::ULong members;
	CDR stream;

	stream.setup_encapsulation (this->buffer_, (size_t) this->length_);

	// skip rest of header (type ID and name) and collect the
	// number of struct members
	if (!stream.skip_string ()	    	    // ID
	    || !stream.skip_string ()  	    // struct name
	    || !stream.get_ulong (members))
	  {
	    env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
	    return 0;
	  }

	this->private_state_->tc_member_count_known_ = CORBA::B_TRUE;
	this->private_state_->tc_member_count_ = members;
	return this->private_state_->tc_member_count_;
      }
    case CORBA::tk_union:
      {
	CORBA::ULong members;
	CDR stream;

	stream.setup_encapsulation (this->buffer_, (size_t) length_);

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

	this->private_state_->tc_member_count_known_ = CORBA::B_TRUE;
	this->private_state_->tc_member_count_ = members;
	return this->private_state_->tc_member_count_;
      }
    default:
      env.exception (new CORBA::TypeCode::BadKind ());
      return 0;
    }
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

  stream.setup_encapsulation (this->buffer_, (size_t) this->length_);

  switch (kind_)
    {
    case CORBA::tk_except:
    case CORBA::tk_struct:		// index from 0
      mcount = this->member_count (env);		// clears env
      if (env.exception () == 0)
	{
	  // the first time in. Precompute and store types of all members

	  // Allocate a list to hold the member typecodes
	  this->private_state_->tc_member_type_list_ = new CORBA::TypeCode_ptr [mcount];

	  if (this->private_state_->tc_member_type_list_)
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
					    &this->private_state_->
                                            tc_member_type_list_[i],
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

		  this->private_state_->tc_member_type_list_known_ = CORBA::B_TRUE;

		  if (index < mcount)
		    return this->private_state_->tc_member_type_list_[index];
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
      break;
    case CORBA::tk_union:            // index from 0
      mcount = this->member_count (env);		// clears env
      if (env.exception () == 0)
	{
	  // the first time in. Precompute and store types of all members
	  this->private_state_->tc_member_type_list_ = new CORBA::TypeCode_ptr [mcount];
	  if (this->private_state_->tc_member_type_list_)
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
		  tc = this->discriminator_type (env);
		  // compute the typecodes for all the members and return the
		  // required one

		  for (CORBA::ULong i = 0; i < mcount; i++)
		    {
		      // the ith entry will have the typecode of the ith guy
		      if (stream.decode (tc, &scratch, this,  env) // member label
			  != CORBA::TypeCode::TRAVERSE_CONTINUE
			  || !stream.skip_string ()  // skip the name
			  || stream.decode (CORBA::_tc_TypeCode,  // get the typecode
					    &private_state_->tc_member_type_list_[i],
					    this,
					    env) !=
			  CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  env.exception (new CORBA::BAD_TYPECODE
                                         (CORBA::COMPLETED_NO));
			  return 0;
			}
		    }
		  this->private_state_->tc_member_type_list_known_ = CORBA::B_TRUE;

		  if (index < mcount)
		    return this->private_state_->tc_member_type_list_[index];
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

// Return the name for the nth member
// Applicable only to CORBA::tk_struct, CORBA::tk_union, CORBA::tk_enum, and
// CORBA::tk_except
const char *
CORBA_TypeCode::private_member_name (CORBA::ULong index,
                                     CORBA::Environment &env) const
{
  CORBA::ULong temp, mcount;

  // Build the de-encapsulating CDR stream, bypassing the stringent
  // alignment tests (we're a bit looser in what we need here, and we
  // _know_ we're OK).  Then skip the byte order code.
  CDR stream;
  CORBA::TypeCode_ptr tc = 0;

  stream.setup_encapsulation (this->buffer_, (size_t) this->length_);

  switch (kind_)
    {
    case CORBA::tk_enum:
      mcount = this->member_count (env);		// clears env
      if (env.exception () == 0)
	{
	  // the first time in. Precompute and store names of all members
	  // Allocate a list to hold the member names
	  this->private_state_->tc_member_name_list_ = new char* [mcount];

	  if (this->private_state_->tc_member_name_list_)
	    {
	      // skip the id, name, and member_count part
	      if (!stream.skip_string ()	// type ID, hidden
		  || !stream.skip_string ()	// typedef name
		  || !stream.get_ulong (temp))  // member count
		{
		  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
		  return (char *)0;
		}
	      else
		{
		  // compute the typecodes for all the members and
		  // return the required one.
		  for (CORBA::ULong i = 0; i < mcount; i++)
		    {
		      // the ith entry will have the name of the ith member
                      this->private_state_->tc_member_name_list_ [i] = (char *)
                        (stream.next + CDR::LONG_SIZE); // just point ot it
		    }

		  this->private_state_->tc_member_name_list_known_ = CORBA::B_TRUE;

		  if (index < mcount)
		    return this->private_state_->tc_member_name_list_[index];
		  else
		    {
		      env.exception (new CORBA::TypeCode::Bounds ());
		      return (char *)0;
		    }
		}
	    }
	  else  // no memory for the member_list
	    {
	      env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
	      return (char *)0;
	    }
	}
      else // out of bounds
	{
	  env.exception (new CORBA::TypeCode::Bounds ());
	  return (char *)0;
	}
      break;
    case CORBA::tk_except:
    case CORBA::tk_struct:		// index from 0
      mcount = this->member_count (env);		// clears env
      if (env.exception () == 0)
	{
	  // the first time in. Precompute and store names of all members
	  // Allocate a list to hold the member names
	  this->private_state_->tc_member_name_list_ = new char* [mcount];

	  if (this->private_state_->tc_member_name_list_)
	    {
	      // skip the id, name, and member_count part
	      if (!stream.skip_string ()	// type ID, hidden
		  || !stream.skip_string ()	// typedef name
		  || !stream.get_ulong (temp))  // member count
		{
		  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
		  return (char *)0;
		}
	      else
		{
		  // compute the typecodes for all the members and
		  // return the required one.
		  for (CORBA::ULong i = 0; i < mcount; i++)
		    {
		      // the ith entry will have the name of the ith member
                      this->private_state_->tc_member_name_list_ [i] = (char *)
                        (stream.next + CDR::LONG_SIZE); // just point ot it
		      if (!skip_typecode (stream)  // skip the typecode
                          != CORBA::TypeCode::TRAVERSE_CONTINUE)
			{
			  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
			  return 0;
			}
		    }

		  this->private_state_->tc_member_name_list_known_ = CORBA::B_TRUE;

		  if (index < mcount)
		    return this->private_state_->tc_member_name_list_[index];
		  else
		    {
		      env.exception (new CORBA::TypeCode::Bounds ());
		      return (char *)0;
		    }
		}
	    }
	  else  // no memory for the member_list
	    {
	      env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
	      return (char *)0;
	    }
	}
      else // out of bounds
	{
	  env.exception (new CORBA::TypeCode::Bounds ());
	  return (char *)0;
	}
      break;
    case CORBA::tk_union:            // index from 0
      mcount = this->member_count (env);		// clears env
      if (env.exception () == 0)
	{
	  // the first time in. Precompute and store names of all members
	  // Allocate a list to hold the member names
	  this->private_state_->tc_member_name_list_ = new char* [mcount];

	  if (this->private_state_->tc_member_name_list_)
	    {
	      // skip the id, name, and discrimant type part
	      if (!stream.skip_string ()	// type ID, hidden
		  || !stream.skip_string ()	// typedef name
		  || !skip_typecode (stream))   // skip typecode for discriminant
		{
		  env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
		  return (char *)0;
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
		  tc = this->discriminator_type (env);
		  // compute the name for all the members and return the
		  // required one

		  for (CORBA::ULong i = 0; i < mcount; i++)
		    {
		      // the ith entry will have the name of the ith member
		      if (stream.decode (tc, &scratch, this,  env) // member label
			  != CORBA::TypeCode::TRAVERSE_CONTINUE)
                        {
			  env.exception (new CORBA::BAD_TYPECODE
                                         (CORBA::COMPLETED_NO));
			  return 0;
                        }
                      this->private_state_->tc_member_name_list_ [i] = (char *)
                        (stream.next + CDR::LONG_SIZE);
                      // skip typecode for member
                      if (!skip_typecode (stream))
			{
			  env.exception (new CORBA::BAD_TYPECODE
                                         (CORBA::COMPLETED_NO));
			  return 0;
			}
		    }
		  this->private_state_->tc_member_name_list_known_ = CORBA::B_TRUE;

		  if (index < mcount)
		    return this->private_state_->tc_member_name_list_[index];
		  else
		    {
		      env.exception (new CORBA::TypeCode::Bounds ());
		      return (char *)0;
		    }
		}
	    }
	  else  // no memory for the member_list
	    {
	      env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
	      return (char *)0;
	    }
	}
      else // out of bounds
	{
	  env.exception (new CORBA::TypeCode::Bounds ());
	  return (char *)0;
	}
      break;
    default:
      // bad kind
      env.exception (new CORBA::TypeCode::BadKind ());
      return (char *)0;
    }
  return (char *)0;
}

// Return member labels for CORBA::tk_union typecodes.
CORBA::Any_ptr
CORBA_TypeCode::private_member_label (CORBA::ULong n,
                                      CORBA::Environment &env) const
{
  env.clear ();

  // this function is only applicable to the CORBA::tk_union TC
  if (this->kind_ == CORBA::tk_union)
    {
      CDR stream;

      stream.setup_encapsulation (this->buffer_, (size_t) this->length_);

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
      this->private_state_->tc_member_label_list_ = new CORBA::Any_ptr [member_count];
      if (this->private_state_->tc_member_label_list_)
	{
	  tc = this->discriminator_type (env); // retrieve the discriminator
                                               // type as this decides what the
                                               // label is
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
		  this->private_state_->tc_member_label_list_[i] = new
                    CORBA::Any (tc, buf, CORBA::B_TRUE);
		}
	    }
	}
      else
	{
	  env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
	  return 0;
	}

      this->private_state_->tc_member_label_list_known_ = CORBA::B_TRUE;

      // If caller asked for the label for a nonexistent member, they get
      // an error report!
      if (n >= member_count)
	{
	  env.exception (new CORBA::TypeCode::Bounds ());
	  return 0;
	}
      else
	return this->private_state_->tc_member_label_list_[n];
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

  stream.setup_encapsulation (this->buffer_, (size_t) this->length_);

  // skip ID and name, and then get the discriminant TC

  if (!stream.skip_string ()		// type ID, hidden
      || !stream.skip_string ()	        // typedef name
      || stream.decode (CORBA::_tc_TypeCode,
			&this->private_state_->tc_discriminator_type_, this,
                        env) != CORBA::TypeCode::TRAVERSE_CONTINUE)
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
      return 0;
    }
  else
    {
      this->private_state_->tc_discriminator_type_known_ = CORBA::B_TRUE;
      return this->private_state_->tc_discriminator_type_;
    }
}

CORBA::Long
CORBA_TypeCode::private_default_index (CORBA::Environment &env) const
{
  CDR stream;

  stream.setup_encapsulation (this->buffer_, (size_t) this->length_);

  // skip ID and name, and then get the discriminant TC

  if (!stream.skip_string ()		// type ID, hidden
      || !stream.skip_string ()	        // typedef name
      || !skip_typecode (stream)        // skip discriminant
      || !stream.get_long (this->private_state_->tc_default_index_used_))
    {
      env.exception (new CORBA::BAD_TYPECODE (CORBA::COMPLETED_NO));
      return 0;
    }
  else
    {
      this->private_state_->tc_default_index_used_known_ = CORBA::B_TRUE;
      return this->private_state_->tc_default_index_used_;
    }
}

CORBA::Long
CORBA_TypeCode::private_length (CORBA::Environment &env) const
{
  CDR stream;
  stream.setup_encapsulation (this->buffer_, (size_t) this->length_);
  switch (this->kind_)
    {
    case CORBA::tk_sequence:
    case CORBA::tk_array:
      {
	// skip the typecode of the element and get the bounds
	if (!skip_typecode (stream) // skip typecode
	    || !stream.get_ulong (this->private_state_->tc_length_))
	  {
	    env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
	    return 0;
	  }
	else
	  {
	    this->private_state_->tc_length_known_ = CORBA::B_TRUE;
	    return this->private_state_->tc_length_;
	  }
      case CORBA::tk_string:
      case CORBA::tk_wstring:
	{
	  if (stream.get_ulong (this->private_state_->tc_length_))
	    {
	      this->private_state_->tc_length_known_ = CORBA::B_TRUE;
	      return this->private_state_->tc_length_;
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

  stream.setup_encapsulation (this->buffer_, (size_t) this->length_);
  switch (kind_)
    {
    case CORBA::tk_sequence:
    case CORBA::tk_array:
      {
	// retrieve the content type
	if (stream.decode (CORBA::_tc_TypeCode,
                           &this->private_state_->tc_content_type_,
			   this,  env) !=
	    CORBA::TypeCode::TRAVERSE_CONTINUE) // element type
	  {
	    env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
	    return 0;
	  }
	else
	  {
	    this->private_state_->tc_content_type_known_ = CORBA::B_TRUE;
	    return this->private_state_->tc_content_type_;
	  }
      case CORBA::tk_alias:
	{
	  if (!stream.skip_string ()  // typeID
	      || !stream.skip_string () // name
	      || stream.decode (CORBA::_tc_TypeCode,
				&this->private_state_->tc_content_type_, this,
                                env) != CORBA::TypeCode::TRAVERSE_CONTINUE)
	    {
	      env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
	      return 0;
	    }
	  else
	    {
	      this->private_state_->tc_content_type_known_ = CORBA::B_TRUE;
	      return this->private_state_->tc_content_type_;
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

  stream.setup_encapsulation (this->buffer_, (size_t) this->length_);

 (void) calc_key_union_attributes (&stream, overall_align, discrim_size, env);

  if (env. exception () == 0)
    {
      this->private_state_->tc_discrim_pad_size_known_ = CORBA::B_TRUE;
      this->private_state_->tc_discrim_pad_size_ = discrim_size;
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

  switch (this->kind_)
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

	stream.setup_encapsulation (this->buffer_, (size_t) this->length_);

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

	stream.setup_encapsulation (this->buffer_, (size_t) this->length_);

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

	stream.setup_encapsulation (this->buffer_, (size_t) this->length_);

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

  temp = this->param_count (env);		// clears env
  if (env.exception ())
    return 0;

  if (temp < n)
    {
      env.exception (new CORBA::TypeCode::Bounds);
      return 0;
    }

  // Get parameters for non-empty typecodes; their parameter lists are
  // encapsulated CDR (for complex ones) or inlined (for simple ones).
  switch (kind_)
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

	stream.setup_encapsulation (this->buffer_, (size_t) this->length_);
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
      return this->length_;
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

  temp = this->param_count (env);		// clears env
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

  stream.setup_encapsulation (this->buffer_, (size_t) this->length_);

  switch (this->kind_)
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
