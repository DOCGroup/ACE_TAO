// $Id$

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

#include "tao/Typecode.h"
#include "tao/Environment.h"
#include "tao/Any.h"
#include "tao/Exception.h"
#include "tao/CDR_Interpreter.h"
#include "tao/Principal.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/Typecode.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Typecode, "$Id$")

CORBA_TypeCode::Bounds::Bounds (void)
  : CORBA_UserException (CORBA::TypeCode::_tc_Bounds)
{
}

void
CORBA_TypeCode::Bounds::_raise (void)
{
  TAO_RAISE(*this);
}

CORBA_TypeCode::Bounds*
CORBA_TypeCode::Bounds::_narrow (CORBA_Exception *ex)
{
  if (ex->_is_a ("IDL:omg.orb/CORBA/TypeCode/Bounds:1.0"))
    return ACE_dynamic_cast (CORBA_TypeCode::Bounds*, ex);
  return 0;
}

int
CORBA_TypeCode::Bounds::_is_a (const char* interface_id) const
{
  return ((ACE_OS::strcmp (interface_id,
			  "IDL:omg.orb/CORBA/TypeCode/Bounds:1.0") ==
	   0)
	  || CORBA_UserException::_is_a (interface_id));
}

CORBA_TypeCode::BadKind::BadKind (void)
  : CORBA_UserException (CORBA::TypeCode::_tc_BadKind)
{
}

void
CORBA_TypeCode::BadKind::_raise (void)
{
  TAO_RAISE(*this);
}

CORBA_TypeCode::BadKind*
CORBA_TypeCode::BadKind::_narrow (CORBA_Exception *ex)
{
  if (ex->_is_a ("IDL:omg.orb/CORBA/TypeCode/BadKind:1.0"))
    return ACE_dynamic_cast (CORBA_TypeCode::BadKind*, ex);
  return 0;
}

int
CORBA_TypeCode::BadKind::_is_a (const char* interface_id) const
{
  return ((ACE_OS::strcmp (interface_id,
			  "IDL:omg.orb/CORBA/TypeCode/BadKind:1.0") ==
	   0)
	  || CORBA_UserException::_is_a (interface_id));
}

// decreases the refcount and deletes when refcount reaches 0

// Constructor for CONSTANT typecodes with empty parameter lists.
// These are only created once, and those constants are shared.

CORBA_TypeCode::CORBA_TypeCode (CORBA::TCKind kind)
  : length_ (0),
    buffer_ (0),
    byte_order_ (0),
    kind_ (kind),
    parent_ (0),
    tc_base_ (0),
    root_tc_base_ (0),
    refcount_ (1),
    orb_owns_ (1),
    private_state_ (new TC_Private_State (kind)),
    non_aligned_buffer_ (0)
{
  // all these are simple typecodes and the comparison is based solely on
  this->private_state_->tc_size_known_ = 1;
  switch (this->kind_)
    {
    case CORBA::tk_null:
    case CORBA::tk_void:
      this->private_state_->tc_size_ = 0;
      break;
    case CORBA::tk_short:
      this->private_state_->tc_size_ = sizeof (CORBA::Short);
      break;
    case CORBA::tk_ushort:
      this->private_state_->tc_size_ = sizeof (CORBA::UShort);
      break;
    case CORBA::tk_long:
      this->private_state_->tc_size_ = sizeof (CORBA::Long);
      break;
    case CORBA::tk_ulong:
      this->private_state_->tc_size_ = sizeof (CORBA::ULong);
      break;
    case CORBA::tk_float:
      this->private_state_->tc_size_ = sizeof (CORBA::Float);
      break;
    case CORBA::tk_double:
      this->private_state_->tc_size_ = sizeof (CORBA::Double);
      break;
    case CORBA::tk_longlong:
      this->private_state_->tc_size_ = sizeof (CORBA::LongLong);
      break;
    case CORBA::tk_ulonglong:
      this->private_state_->tc_size_ = sizeof (CORBA::ULongLong);
      break;
    case CORBA::tk_longdouble:
      this->private_state_->tc_size_ = sizeof (CORBA::LongDouble);
      break;
    case CORBA::tk_boolean:
      this->private_state_->tc_size_ = sizeof (CORBA::Boolean);
      break;
    case CORBA::tk_octet:
      this->private_state_->tc_size_ = sizeof (CORBA::Octet);
      break;
    case CORBA::tk_char:
      this->private_state_->tc_size_ = sizeof (CORBA::Char);
      break;
    case CORBA::tk_wchar:
      this->private_state_->tc_size_ = sizeof (CORBA::WChar);
      break;
    case CORBA::tk_TypeCode:
      this->private_state_->tc_size_ = sizeof (CORBA::TypeCode_ptr);
      break;
    case CORBA::tk_Principal:
      this->private_state_->tc_size_ = sizeof (CORBA::Principal);
      break;
    case CORBA::tk_any:
      this->private_state_->tc_size_ = sizeof (CORBA::Any);
      break;
    default:
      // we should never be here
      this->private_state_->tc_size_known_ = 0;
      break;
    }
}

// Constructor for all other typecodes, including constants with
// non-empty parameter lists.  See "corba.hh" for details.

CORBA_TypeCode::CORBA_TypeCode (CORBA::TCKind kind,
                                size_t length,
                                const char *buffer,
                                CORBA::Boolean orb_owns_tc,
                                CORBA::ULong size,
                                CORBA::TypeCode_ptr parent)
  //  : length_ (length - 4),
  : length_ (length),
    kind_ (kind),
    parent_ (parent),
    refcount_ (1),
    orb_owns_ (orb_owns_tc),
    private_state_ (new TC_Private_State (kind)),
    non_aligned_buffer_ (0)
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

  if (this->parent_ == 0)
    {
      // Allocate a buffer to hold the encapsulated stream. We
      // allocate extra space since we need a buffer that is aligned
      // on a 4 byte word boundary. As a result, it is quite possible
      // that we may start accessing the buffer from a position
      // shifted to the right in the allocated buffer. As a result,
      // during destruction, we do not want part of the allocated heap
      // to remain dangling. Hence we save a handle to the original
      // allocated buffer.

      // *NOTE* that the buffer parameter is simply our encapsulation. It does
      // not contain our TypeCode::kind () and the length. These are passed as
      // separate parameters. However, in case of indirected typecodes, the
      // offset value for the indirection will effectively point to the tk_kind
      // field in our CDR representation. Hence, we introduce a new field
      // called tc_base_ which represents the start of our CDR
      // representation. The buffer_ data member will point to our
      // encapsulation. 

      // @@ The typecode buffer contain the encapsulation byte order
      // in the first byte...
      const CORBA::Octet *ptr =
        ACE_reinterpret_cast (const CORBA::Octet *,
                              buffer);
      this->byte_order_ = *ptr;

      // allocate a buffer which will accomodate our entire encapsulation plus
      // 4 bytes for our tk_kind value and 4 bytes for our encapsulation
      // length. The extra MAX_ALIGNMENT bytes are necessary to ensure that we
      // will get a properly aligned buffer.

      ACE_NEW (this->non_aligned_buffer_,
               char [this->length_ + 4 + 4 + ACE_CDR::MAX_ALIGNMENT]);

      char* start = ptr_align_binary (this->non_aligned_buffer_,
                                      ACE_CDR::MAX_ALIGNMENT);

      (void) ACE_OS::memcpy (start, &this->kind_, 4);
      (void) ACE_OS::memcpy (start + 4, &this->length_, 4);
      (void) ACE_OS::memcpy (start + 8, buffer, this->length_);
      // we are the topmost level typecode and hence our typecode base is
      // the start whereas the buffer_ which represents the encapsulation is 8
      // bytes ahead of the typecode base
      this->tc_base_ = start;
      // since we do not have any parents, we are the root
      this->root_tc_base_ = start;
      this->buffer_ = start + 4 + 4;
      this->private_state_->tc_size_known_ = 1;
      this->private_state_->tc_size_ = size;
    }
  else
    {
      // We are a child. We do not allocate a new buffer, but share it
      // with our parent. We know that our parent's buffer was
      // properly aligned.
      const CORBA::Octet *ptr =
        ACE_reinterpret_cast (const CORBA::Octet *, buffer);
      this->byte_order_ = *ptr;

      this->buffer_ = buffer;
      // our typecode base is 8 bytes prior to our encapsulation and our root
      // base is the same as that of our parent's
      this->tc_base_ = this->buffer_ - 8;
      this->root_tc_base_ = parent->root_tc_base_;
    }
}

// Destructor.  For "indirected" typecodes and children, the typecode
// reuses the buffer owned by its parent.

CORBA_TypeCode::~CORBA_TypeCode (void)
{
  // Delete the original, possibly nonaligned, buffer.
  if (this->non_aligned_buffer_ != 0)
    {
      delete [] this->non_aligned_buffer_;
      this->non_aligned_buffer_ = 0;
    }
  this->buffer_ = 0;

  // Free up our private state (if any)
  if (this->private_state_)
    {
      delete this->private_state_;
      this->private_state_ = 0;
    }
}

// Returns true if the two unaliased typecodes are equal.
CORBA::Boolean
CORBA_TypeCode::equivalent (CORBA::TypeCode_ptr tc,
			    CORBA::Environment &env) const
{
  const CORBA::TypeCode* rcvr = this;
  CORBA::Boolean status = 0;

  if (this->kind_ == CORBA::tk_alias)
    {
      rcvr = this->content_type (env);
      // @@EXC@@ Is returning FALSE the right choice?
      TAO_CHECK_ENV_RETURN (env, 0);

      status = (rcvr->kind (env) == CORBA::tk_alias);
      TAO_CHECK_ENV_RETURN (env, 0);

      while (status)
        {
          rcvr = rcvr->content_type (env);
          TAO_CHECK_ENV_RETURN (env, 0);

          status = (rcvr->kind (env) == CORBA::tk_alias);
          TAO_CHECK_ENV_RETURN (env, 0);
        }
    }

  status = (tc->kind (env) == CORBA::tk_alias);
  TAO_CHECK_ENV_RETURN (env, 0);

  while (status)
    {
      tc = tc->content_type (env);
      TAO_CHECK_ENV_RETURN (env, 0);

      status = (tc->kind (env) == CORBA::tk_alias);
      TAO_CHECK_ENV_RETURN (env, 0);
    }

  return rcvr->equal (tc, env);
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
        TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, 0);
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
        TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, 0);
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
        TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, 0);
    }
  else
    return this->private_member_label (index, env);
}

// only applicable to CORBA::tk_unions
CORBA::TypeCode_ptr
CORBA_TypeCode::discriminator_type (CORBA::Environment &env) const
{
  if (this->kind_ != CORBA::tk_union)
    TAO_THROW_ENV_RETURN (CORBA::TypeCode::BadKind (), env, (CORBA::TypeCode_ptr)0);

  if (this->private_state_->tc_discriminator_type_known_)
    return this->private_state_->tc_discriminator_type_;
  else
    return this->private_discriminator_type (env);
}

// only applicable to CORBA::tk_unions
CORBA::Long
CORBA_TypeCode::default_index (CORBA::Environment &env) const
{
  if (this->kind_ != CORBA::tk_union)
    TAO_THROW_ENV_RETURN (CORBA::TypeCode::BadKind (), env, 0);

  if (this->private_state_->tc_default_index_used_known_)
    return this->private_state_->tc_default_index_used_;
  else
    return this->private_default_index (env);
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
    TAO_THROW_ENV_RETURN (CORBA::TypeCode::BadKind (), env, 0);
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
    TAO_THROW_ENV_RETURN (CORBA::TypeCode::BadKind (), env, 0);
}

// compute the padded size of the discriminant
CORBA::ULong
CORBA_TypeCode::TAO_discrim_pad_size (CORBA::Environment &env)
{
  if (this->kind_ != CORBA::tk_union)
    TAO_THROW_ENV_RETURN (CORBA::TypeCode::BadKind (), env, 0);

  if (this->private_state_->tc_discrim_pad_size_known_)
    return this->private_state_->tc_discrim_pad_size_;
  else
    return this->private_discrim_pad_size (env);
}

// skip a typecode encoding in a given CDR stream
// This is just a helper function
CORBA::Boolean
CORBA_TypeCode::skip_typecode (TAO_InputCDR &stream)
{
  CORBA::ULong kind;
  CORBA::ULong temp;

  if (stream.read_ulong (kind)
      && (kind < CORBA::TC_KIND_COUNT || kind == ~0u))
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
        case ~0u:
          return stream.read_ulong (temp);

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
          return (stream.read_ulong (temp) != 0
                  && stream.skip_bytes (temp) != 0);
        }

      return 1;
    }
  else
    return 0;
}

// constructor for the private state
TC_Private_State::TC_Private_State (CORBA::TCKind kind)
  : tc_kind_ (kind),
    tc_id_known_ (0),
    tc_name_known_ (0),
    tc_member_count_known_ (0),
    tc_member_type_list_known_ (0),
    tc_member_name_list_known_ (0),
    tc_member_label_list_known_ (0),
    tc_discriminator_type_known_ (0),
    tc_default_index_used_known_ (0),
    tc_length_known_ (0),
    tc_content_type_known_ (0),
    tc_size_known_ (0),
    tc_alignment_known_ (0),
    tc_discrim_pad_size_known_ (0),
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
  // the following just point into the buffer. So we just make it
  // point to 0
  this->tc_id_ = 0;

  if (this->tc_name_ != 0)
    {
      CORBA::string_free (this->tc_name_);
      this->tc_name_ = 0;
    }

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
		              CORBA::string_free (this->tc_member_name_list_ [i]);
		              this->tc_member_name_list_ [i] = 0;
	              }

              delete [] this->tc_member_name_list_;
	            this->tc_member_name_list_ = 0;
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
		              CORBA::string_free (this->tc_member_name_list_ [i]);
		              this->tc_member_name_list_ [i] = 0;
	              }

              delete [] this->tc_member_name_list_;
	            this->tc_member_name_list_ = 0;
            }

          // free up member type list
          if (this->tc_member_type_list_known_)
            {
              for (CORBA::ULong i = 0;
                   i < this->tc_member_count_;
                   i++)
	              {
		              CORBA::release (this->tc_member_type_list_[i]);
	              }

              // Now free up the array.
              delete [] this->tc_member_type_list_;
	            this->tc_member_type_list_ = 0;
            }
          this->tc_member_count_ = 0;
        }
        break;

      case CORBA::tk_sequence:
      case CORBA::tk_array:
      case CORBA::tk_alias:
        // Delete the content type only if it has a parent i.e., if it
        // is not acquired from the pool of constant or predefined
        // typecodes.
        if (this->tc_content_type_known_ && this->tc_content_type_ != 0)
	        {
	          CORBA::release (this->tc_content_type_);
	          this->tc_content_type_ = 0;
	        }
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
		              CORBA::string_free (this->tc_member_name_list_ [i]);
		              this->tc_member_name_list_ [i] = 0;
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
                  CORBA::release (this->tc_member_type_list_[i]);
	              }

              // Now free up the array.
              delete [] this->tc_member_type_list_;
	            this->tc_member_type_list_ = 0;
            }
          if (this->tc_member_label_list_known_)
            {
              for (CORBA::ULong i = 0;
                   i < this->tc_member_count_;
                   i++)
                // Free up the label (Any_ptr).
                delete this->tc_member_label_list_[i];

              delete [] this->tc_member_label_list_;
	            this->tc_member_label_list_ = 0;
            }
          this->tc_member_count_ = 0;

          // Discriminator must come last b/c it will be inside the Any
          // in each element of the label list.
          CORBA::release (this->tc_discriminator_type_);
	        this->tc_discriminator_type_ = 0;
        }
        break;

      default:
        // nothing to do
        break;
    }
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
    case CORBA::tk_ulonglong:
    case CORBA::tk_longdouble:
    case CORBA::tk_boolean:
    case CORBA::tk_octet:
    case CORBA::tk_char:
    case CORBA::tk_wchar:
    case CORBA::tk_TypeCode:
    case CORBA::tk_Principal:
    case CORBA::tk_any:
      // all these are simple typecodes and the comparison is based solely on
      // the kind_ field
      return 1;
    case CORBA::tk_objref:
      return this->private_equal_objref (tc, env);
    case CORBA::tk_struct:
      return this->private_equal_struct (tc, env);
    case CORBA::tk_union:
      return this->private_equal_union (tc, env);
    case CORBA::tk_enum:
      return this->private_equal_enum (tc, env);
    case CORBA::tk_string:
      return this->private_equal_string (tc, env);
    case CORBA::tk_wstring:
      return this->private_equal_wstring (tc, env);
    case CORBA::tk_sequence:
      return this->private_equal_sequence (tc, env);
    case CORBA::tk_array:
      return this->private_equal_array (tc, env);
    case CORBA::tk_alias:
      return this->private_equal_alias (tc, env);
    case CORBA::tk_except:
      return this->private_equal_except (tc, env);
    case ~0u: // indirection
      {
        // indirection offset must be same
        return 1;
      }
      ACE_NOTREACHED (break);
    default:
      return 0;
    }
}

CORBA::Boolean
CORBA_TypeCode::private_equal_objref (CORBA::TypeCode_ptr tc,
                                      CORBA::Environment &env) const
{
  env.clear ();
  // compare the repoID and name, of which the name is optional as per GIOP
  // spec. However, the repoID is mandatory.
  const char *my_id = this->id (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *tc_id = tc->id (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  if (!ACE_OS::strcmp (my_id, tc_id))
    {
      // same repository IDs. Now check their names
      const char *myname = this->name (env);
      TAO_CHECK_ENV_RETURN (env, 0);

      const char *tcname = tc->name (env);
      TAO_CHECK_ENV_RETURN (env, 0);

      if ((ACE_OS::strlen (myname) > 1) &&
          (ACE_OS::strlen (tcname) > 1))
        {
          // both of them specify names, compare them
          if (!ACE_OS::strcmp (myname, tcname))
            return 1; // success
          else
            return 0; // failed
        }
      return 1; // equal (success)
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
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *tc_id = tc->id (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *my_name = this->name (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *tc_name = tc->name (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  // compare repoIDs if they exist
  if (ACE_OS::strlen (my_id) > 1
      && ACE_OS::strlen (tc_id) > 1
      && ACE_OS::strcmp (my_id, tc_id)) // not same
    return 0;

  // Compare names if they exist.
  if (ACE_OS::strlen (my_name) > 1
      && ACE_OS::strlen (tc_name) > 1
      && ACE_OS::strcmp (my_name, tc_name)) // not same
    return 0;

  // Check if the member count is same.
  CORBA::ULong my_count = this->member_count (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::ULong tc_count = tc->member_count (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  if (my_count != tc_count)
    return 0; // number of members don't match

  for (CORBA::ULong i=0; i < my_count; i++)
    {
      const char *my_member_name =
        this->member_name (i, env);
      TAO_CHECK_ENV_RETURN (env, 0);

      const char *tc_member_name =
        tc->member_name (i, env);
      TAO_CHECK_ENV_RETURN (env, 0);

      if (ACE_OS::strlen (my_member_name) > 1
          && ACE_OS::strlen (tc_member_name) > 1
          && ACE_OS::strcmp (my_member_name, tc_member_name)) // not same
        return 0;

      // now compare the typecodes of the members
      CORBA::TypeCode_ptr my_member_tc = this->member_type (i, env);
      TAO_CHECK_ENV_RETURN (env, 0);

      CORBA::TypeCode_ptr tc_member_tc = tc->member_type (i, env);
      TAO_CHECK_ENV_RETURN (env, 0);

      CORBA::Boolean flag =
        my_member_tc->equal (tc_member_tc, env);
      TAO_CHECK_ENV_RETURN (env, 0);
      if (!flag)
        return 0;
    }

  return 1; // success (equal)
}

CORBA::Boolean
CORBA_TypeCode::private_equal_union (CORBA::TypeCode_ptr tc,
                                     CORBA::Environment &env) const
{
  env.clear ();

  // For unions the repoID and names are optional. However, if
  // provided, we must compare them.
  const char *my_id = this->id (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *tc_id = tc->id (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *my_name = this->name (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *tc_name = tc->name (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  // compare repoIDs if they exist
  if (ACE_OS::strlen (my_id) > 1
      && ACE_OS::strlen (tc_id) > 1
      && ACE_OS::strcmp (my_id, tc_id)) // not same
    return 0;

  // compare names if they exist
  if (ACE_OS::strlen (my_name) > 1
      && ACE_OS::strlen (tc_name) > 1
      && ACE_OS::strcmp (my_name, tc_name)) // not same
    return 0;

  // check if the discriminant type is same
  CORBA::TypeCode_ptr my_discrim = this->discriminator_type (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::TypeCode_ptr tc_discrim = tc->discriminator_type (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  int status = my_discrim->equal (tc_discrim, env);
  TAO_CHECK_ENV_RETURN (env, 0);
  if (!status)
    return 0;

  // check the default used
  CORBA::Long my_default = this->default_index (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::Long tc_default = tc->default_index (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  if (my_default != tc_default)
    return 0;

  // check if the member count is same
  CORBA::ULong my_count = this->member_count (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::ULong tc_count = tc->member_count (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  if (my_count != tc_count)
    return 0; // number of members don't match

  for (CORBA::ULong i=0; i < my_count; i++)
    {
      // First check if labels are same.

      // check if member names are same
      const char *my_member_name = this->member_name (i, env);
      TAO_CHECK_ENV_RETURN (env, 0);

      const char *tc_member_name = tc->member_name (i, env);
      TAO_CHECK_ENV_RETURN (env, 0);

      if (ACE_OS::strlen (my_member_name) > 1 && ACE_OS::strlen
          (tc_member_name) > 1)
        // both specify member names
        if (ACE_OS::strcmp (my_member_name, tc_member_name)) // not same
          return 0;

      // now compare the typecodes of the members
      CORBA::TypeCode_ptr my_member_tc = this->member_type (i, env);
      TAO_CHECK_ENV_RETURN (env, 0);

      CORBA::TypeCode_ptr tc_member_tc = tc->member_type (i, env);
      TAO_CHECK_ENV_RETURN (env, 0);

      CORBA::Boolean flag = my_member_tc->equal (tc_member_tc, env);
      TAO_CHECK_ENV_RETURN (env, 0);
      if (!flag)
        return 0;
    }

  return 1; // success (equal)
}

CORBA::Boolean
CORBA_TypeCode::private_equal_enum (CORBA::TypeCode_ptr tc,
                                    CORBA::Environment &env) const
{
  env.clear ();

  // for enum the repoID and names are optional. However, if provided, we
  // must compare them
  const char *my_id = this->id (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *tc_id = tc->id (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *my_name = this->name (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *tc_name = tc->name (env);
  TAO_CHECK_ENV_RETURN (env, 0);

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
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::ULong tc_count = tc->member_count (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  if (my_count != tc_count)
    return 0; // number of members don't match

  for (CORBA::ULong i=0; i < my_count; i++)
    {
      // now check if the member names are same
      const char *my_member_name = this->member_name (i, env);
      TAO_CHECK_ENV_RETURN (env, 0);

      const char *tc_member_name = tc->member_name (i, env);
      TAO_CHECK_ENV_RETURN (env, 0);

      if (ACE_OS::strlen (my_member_name) > 1 && ACE_OS::strlen
          (tc_member_name) > 1)
        // both specify member names
        if (ACE_OS::strcmp (my_member_name, tc_member_name)) // not same
          return 0;
    }

  return 1; // success (equal)
}

CORBA::Boolean
CORBA_TypeCode::private_equal_string (CORBA::TypeCode_ptr tc,
                                      CORBA::Environment &env) const
{
  // compare the lengths
  CORBA::ULong my_len = this->length (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::ULong tc_len = tc->length (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  return (my_len == tc_len);
}

CORBA::Boolean
CORBA_TypeCode::private_equal_wstring (CORBA::TypeCode_ptr tc,
                                       CORBA::Environment &env) const
{
  // compare the lengths
  CORBA::ULong my_len = this->length (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::ULong tc_len = tc->length (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  return (my_len == tc_len);
}

CORBA::Boolean
CORBA_TypeCode::private_equal_sequence (CORBA::TypeCode_ptr tc,
                                        CORBA::Environment &env) const
{
  // this involves comparing the typecodes of the element type as well as the
  // bounds
  CORBA::TypeCode_ptr my_elem = this->content_type (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::TypeCode_ptr tc_elem = tc->content_type (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  int status = my_elem->equal (tc_elem, env);
  TAO_CHECK_ENV_RETURN (env, 0);
  if (!status)
    return 0;

  // now check if bounds are same
  CORBA::ULong my_len = this->length (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::ULong tc_len = tc->length (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  return (my_len == tc_len);
}

CORBA::Boolean
CORBA_TypeCode::private_equal_array (CORBA::TypeCode_ptr tc,
                                     CORBA::Environment &env) const
{
  // exactly like sequence
  return this->private_equal_sequence (tc, env);
}

CORBA::Boolean
CORBA_TypeCode::private_equal_alias (CORBA::TypeCode_ptr tc,
                                     CORBA::Environment &env) const
{
  env.clear ();

  // for structs the repoID and names are optional. However, if provided, we
  // must compare them
  const char *my_id = this->id (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *tc_id = tc->id (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *my_name = this->name (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *tc_name = tc->name (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  // compare repoIDs if they exist
  if (ACE_OS::strlen (my_id) > 1 && ACE_OS::strlen (tc_id) > 1)
    if (ACE_OS::strcmp (my_id, tc_id)) // not same
      return 0;

  // compare names if they exist
  if (ACE_OS::strlen (my_name) > 1 && ACE_OS::strlen (tc_name) > 1)
    if (ACE_OS::strcmp (my_name, tc_name)) // not same
      return 0;

  // now compare element typecodes
  CORBA::TypeCode_ptr my_elem = this->content_type (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::TypeCode_ptr tc_elem = tc->content_type (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  return my_elem->equal (tc_elem, env);
}

CORBA::Boolean
CORBA_TypeCode::private_equal_except (CORBA::TypeCode_ptr tc,
                                      CORBA::Environment &env) const
{
  // exactly similar to structs, except that the repository ID is mandatory
  env.clear ();

  const char *my_id = this->id (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *tc_id = tc->id (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  if (ACE_OS::strcmp (my_id, tc_id))
    return 0; // failed

  // now compare names. They may be optional
  const char *my_name = this->name (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  const char *tc_name = tc->name (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  // compare names if they exist
  if (ACE_OS::strlen (my_name) > 1 && ACE_OS::strlen (tc_name) > 1)
    if (ACE_OS::strcmp (my_name, tc_name)) // not same
      return 0;

  // check if the member count is same
  CORBA::ULong my_count = this->member_count (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::ULong tc_count = tc->member_count (env);
  TAO_CHECK_ENV_RETURN (env, 0);

  if (my_count != tc_count)
    return 0; // number of members don't match

  for (CORBA::ULong i=0; i < my_count; i++)
    {
      const char *my_member_name = this->member_name (i, env);
      TAO_CHECK_ENV_RETURN (env, 0);

      const char *tc_member_name = tc->member_name (i, env);
      TAO_CHECK_ENV_RETURN (env, 0);

      if (ACE_OS::strlen (my_member_name) > 1 && ACE_OS::strlen
          (tc_member_name) > 1)
        // both specify member names
        if (ACE_OS::strcmp (my_member_name, tc_member_name)) // not same
          return 0;

      // now compare the typecodes of the members
      CORBA::TypeCode_ptr my_member_tc = this->member_type (i, env);
      TAO_CHECK_ENV_RETURN (env, 0);

      CORBA::TypeCode_ptr tc_member_tc = tc->member_type (i, env);
      TAO_CHECK_ENV_RETURN (env, 0);

      CORBA::Boolean flag = my_member_tc->equal (tc_member_tc, env);
      TAO_CHECK_ENV_RETURN (env, 0);
      if (!flag)
        return 0;
    }

  return 1; // success (equal)
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
        // Double checked locking...
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                          this->private_state_->mutex_, 0);
        if (this->private_state_->tc_id_known_)
          return this->private_state_->tc_id_;

        this->private_state_->tc_id_known_ = 1;
        this->private_state_->tc_id_ = (CORBA::String) (buffer_
                                                 + 4    // skip byte order flag
                                                        // and padding
                                                 + 4);  // skip (strlen + 1)
        return this->private_state_->tc_id_; // this is OK because the strings in the
                                       // CDR stream are NULL terminated
      }
    // No other typecodes ever have type IDs
    default:
      TAO_THROW_ENV_RETURN (CORBA::TypeCode::BadKind (), env, 0);
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
      // parameter (number one) a name string encoded per CDR rules.
      // That means we can just return a pointer to that string
      // directly!

    case CORBA::tk_objref:
    case CORBA::tk_struct:
    case CORBA::tk_union:
    case CORBA::tk_enum:
    case CORBA::tk_alias:
    case CORBA::tk_except:
      {
        // Double checked locking...
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                          this->private_state_->mutex_, 0);
        if (this->private_state_->tc_name_known_)
          return this->private_state_->tc_name_;

        // setup an encapsulation.
        TAO_InputCDR stream (this->buffer_ + 4,
                             this->length_ - 4,
                             this->byte_order_);

        // skip the typecode ID
        if (stream.skip_string ())  // ID
          {
            this->private_state_->tc_name_known_ = 1;

            // "Read" the string without copying.
            stream.read_string (this->private_state_->tc_name_);

            return this->private_state_->tc_name_;
          }
        else
          TAO_THROW_ENV_RETURN (CORBA::INTERNAL (), env, (CORBA::String)0);
      }
    // No other typecodes ever have type IDs.
    default:
      TAO_THROW_ENV_RETURN (CORBA::TypeCode::BadKind (), env, (CORBA::String)0);
    }
  ACE_NOTREACHED (return 0);
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
        // Double checked locking...
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                          this->private_state_->mutex_, 0);
        if (this->private_state_->tc_member_count_known_)
          return this->private_state_->tc_member_count_;

        CORBA::ULong members;
        // setup an encapsulation
        TAO_InputCDR stream (this->buffer_+4, this->length_-4,
                             this->byte_order_);

        // skip rest of header (type ID and name) and collect the
        // number of struct members
        if (!stream.skip_string ()          // ID
            || !stream.skip_string ()       // struct name
            || !stream.read_ulong (members))
          TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);

        this->private_state_->tc_member_count_known_ = 1;
        this->private_state_->tc_member_count_ = members;
        return this->private_state_->tc_member_count_;
      }
    case CORBA::tk_union:
      {
        // Double checked locking...
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                          this->private_state_->mutex_, 0);
        if (this->private_state_->tc_member_count_known_)
          return this->private_state_->tc_member_count_;

        CORBA::ULong members;
        // setup an encapsulation
        TAO_InputCDR stream (this->buffer_+4, this->length_-4,
                             this->byte_order_);

        // skip rest of header (type ID, name, etc...) and collect the
        // number of struct members
        if (!stream.skip_string ()              // ID
            || !stream.skip_string ()           // struct name
            || !skip_typecode (stream)          // discriminant TC
            || !stream.read_ulong (members)      // default used
            || !stream.read_ulong (members))     // real member count
          TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);

        this->private_state_->tc_member_count_known_ = 1;
        this->private_state_->tc_member_count_ = members;
        return this->private_state_->tc_member_count_;
      }
    default:
      TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);
    }
  ACE_NOTREACHED (return 0);
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
  TAO_InputCDR stream (this->buffer_+4, this->length_-4,
                       this->byte_order_);
  CORBA::TypeCode_ptr tc = 0;

  switch (kind_)
    {
    case CORBA::tk_except:
    case CORBA::tk_struct:              // index from 0
      mcount = this->member_count (env);                // clears env
      // @@EXC@@ Why are we rethrowing a different exception here?
      // if (env.exception ()) TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, (CORBA::TypeCode_ptr)0);
      TAO_CHECK_ENV_RETURN (env, (CORBA::TypeCode_ptr)0);

      {
        // Double checked locking...
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                          this->private_state_->mutex_, 0);
        if (this->private_state_->tc_member_type_list_known_)
          if (index < mcount)
            return  this->private_state_->tc_member_type_list_[index];
          else
            TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, CORBA::TypeCode::_nil ());

        // the first time in. Precompute and store types of all members

        // Allocate a list to hold the member typecodes
        // @@EXC@@ Need to check for allocation failure here.
        this->private_state_->tc_member_type_list_ = new CORBA::TypeCode_ptr [mcount];

        if (this->private_state_->tc_member_type_list_ == 0)
          TAO_THROW_ENV_RETURN (CORBA::NO_MEMORY (), env, (CORBA::TypeCode_ptr)0);

        // skip the id, name, and member_count part
        if (!stream.skip_string ()        // type ID, hidden
            || !stream.skip_string ()     // typedef name
            || !stream.read_ulong (temp))  // member count
          TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, (CORBA::TypeCode_ptr)0);

        // compute the typecodes for all the members and
        // return the required one.
        for (CORBA::ULong i = 0; i < mcount; i++)
          // the ith entry will have the typecode of the ith guy
          {
            if (!stream.skip_string ())  // skip the name
              TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);
            CORBA::TypeCode::traverse_status status =
              stream.decode (CORBA::_tc_TypeCode,
                             // the typecode will be
                             // retrieved at the i-th
                             // location. The decode
                             // routine will allocate the
                             // storage to hold a
                             // typecode
                             &this->private_state_->
                             tc_member_type_list_[i],
                             this,
                             // pass ourselves since we
                             // will be the parent. This
                             // is the case where the 3rd
                             // parameter is used in a
                             // decode method
                             env);
            TAO_CHECK_ENV_RETURN (env, 0);

            if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
              TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);
          }

        this->private_state_->tc_member_type_list_known_ = 1;

        if (index < mcount)
          return this->private_state_->tc_member_type_list_[index];
        else
          TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, (CORBA::TypeCode_ptr)0);
      }
      ACE_NOTREACHED (break;)

    case CORBA::tk_union:            // index from 0
      mcount = this->member_count (env);                // clears env
      // @@EXC@@ Why are we rethrowing a different exception here?
      // if (env.exception ()) TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, (CORBA::TypeCode_ptr)0);
      TAO_CHECK_ENV_RETURN (env, (CORBA::TypeCode_ptr)0);

      {
        // Double checked locking...
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                          this->private_state_->mutex_, 0);
        if (this->private_state_->tc_member_type_list_known_)
          if (index < mcount)
            return  this->private_state_->tc_member_type_list_[index];
          else
            TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, CORBA::TypeCode::_nil ());

      // the first time in. Precompute and store types of all members
      // @@EXC@@ Need to check allocation failure.
        this->private_state_->tc_member_type_list_ = new CORBA::TypeCode_ptr [mcount];
        if (this->private_state_->tc_member_type_list_ == 0)
          TAO_THROW_ENV_RETURN (CORBA::NO_MEMORY (), env, (CORBA::TypeCode_ptr)0);

      // skip the id, name, and discrimant type part
        if (!stream.skip_string ()        // type ID, hidden
            || !stream.skip_string ()     // typedef name
            || !skip_typecode (stream))   // skip typecode for discriminant
          TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, (CORBA::TypeCode_ptr)0);
        else if (!stream.read_ulong (temp)     // default used
                 || !stream.read_ulong (temp)) // member count
          TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);
        // Always big enough because labels can only be of a
        // few different types of which "long" has the
        // largest size.
        CORBA::Long scratch;

      // get the typecode for the discriminator
        tc = this->discriminator_type (env);
        // compute the typecodes for all the members and return the
        // required one
        TAO_CHECK_ENV_RETURN (env, 0);

        for (CORBA::ULong i = 0; i < mcount; i++)
          // the ith entry will have the typecode of the ith guy
          {
            CORBA::TypeCode::traverse_status status =
              stream.decode (tc, &scratch, this,  env);
            // member label
            TAO_CHECK_ENV_RETURN (env, 0);

            if (status != CORBA::TypeCode::TRAVERSE_CONTINUE
                || !stream.skip_string ())  // skip the name
              TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);

            status =  stream.decode (CORBA::_tc_TypeCode,  // get the typecode
                                     &private_state_->tc_member_type_list_[i],
                                     this,
                                     env);
            TAO_CHECK_ENV_RETURN (env, 0);

            if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
              TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);
          }
        this->private_state_->tc_member_type_list_known_ = 1;

        if (index < mcount)
          return this->private_state_->tc_member_type_list_[index];
        else
          TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, (CORBA::TypeCode_ptr)0);
      }
      ACE_NOTREACHED (break);

    default:
      // bad kind
      TAO_THROW_ENV_RETURN (CORBA::TypeCode::BadKind (), env, (CORBA::TypeCode_ptr)0);
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
  TAO_InputCDR stream (this->buffer_+4, this->length_-4,
                       this->byte_order_);
  CORBA::TypeCode_ptr tc = 0;

  switch (kind_)
    {
    case CORBA::tk_enum:
      mcount = this->member_count (env);                // clears env
      // out of bounds
      // if (env.exception ()) TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, (char *)0);
      TAO_CHECK_ENV_RETURN (env, (char *)0);

      {
        // Double checked locking...
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                          this->private_state_->mutex_, 0);
        if (this->private_state_->tc_member_name_list_known_)
          if (index < mcount)
            return this->private_state_->tc_member_name_list_[index];
          else
            TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, 0);

      // the first time in. Precompute and store names of all members
      // Allocate a list to hold the member names
        this->private_state_->tc_member_name_list_ = new char* [mcount];

        if (this->private_state_->tc_member_name_list_ == 0)
          // no memory for the member_list
          TAO_THROW_ENV_RETURN (CORBA::NO_MEMORY (), env, (char *)0);

        // skip the id, name, and member_count part
        if (!stream.skip_string ()     // type ID, hidden
            || !stream.skip_string ()     // enum name
            || !stream.read_ulong (temp)) // member count
          TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, (char *)0);

      // compute the typecodes for all the members and
      // return the required one.
        for (CORBA::ULong i = 0; i < mcount; i++)
          // now skip this name
          if (!stream.read_string (this->private_state_->tc_member_name_list_ [i]))
            TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, (char *)0);

        this->private_state_->tc_member_name_list_known_ = 1;

        if (index < mcount)
          return this->private_state_->tc_member_name_list_[index];
        else
          TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, (char *)0);
      }
      ACE_NOTREACHED (break);

    case CORBA::tk_except:
    case CORBA::tk_struct:              // index from 0
      mcount = this->member_count (env);                // clears env
      // out of bounds
      // if (env.exception ()) TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, (char *)0);
      TAO_CHECK_ENV_RETURN (env, (char *)0);

      {
        // Double checked locking...
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                          this->private_state_->mutex_, 0);
        if (this->private_state_->tc_member_name_list_known_)
          if (index < mcount)
            return this->private_state_->tc_member_name_list_[index];
          else
            TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, 0);

      // the first time in. Precompute and store names of all members
      // Allocate a list to hold the member names
        this->private_state_->tc_member_name_list_ = new char* [mcount];

        if (this->private_state_->tc_member_name_list_ == 0)
          // no memory for the member_list
          TAO_THROW_ENV_RETURN (CORBA::NO_MEMORY (), env, (char *)0);

        // skip the id, name, and member_count part
        if (!stream.skip_string ()     // type ID, hidden
            || !stream.skip_string ()     // struct/except name
            || !stream.read_ulong (temp)) // member count
          TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, (char *)0);

          // compute the typecodes for all the members and
          // return the required one.
        for (CORBA::ULong i = 0; i < mcount; i++)
          if (!stream.read_string (this->private_state_->tc_member_name_list_ [i])
              || !skip_typecode (stream))
            TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);

        this->private_state_->tc_member_name_list_known_ = 1;

        if (index < mcount)
          return this->private_state_->tc_member_name_list_[index];
        else
          TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, (char *)0);
      }
      ACE_NOTREACHED (break;)

    case CORBA::tk_union:            // index from 0
      mcount = this->member_count (env);                // clears env
      // out of bounds
      // if (env.exception ()) TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, (char *)0);
      TAO_CHECK_ENV_RETURN (env, (char *)0);

      {
        // Double checked locking...
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                          this->private_state_->mutex_, 0);
        if (this->private_state_->tc_member_name_list_known_)
          if (index < mcount)
            return this->private_state_->tc_member_name_list_[index];
          else
            TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, 0);

      // the first time in. Precompute and store names of all members
      // Allocate a list to hold the member names
        this->private_state_->tc_member_name_list_ = new char* [mcount];

        if (this->private_state_->tc_member_name_list_ == 0)
          // no memory for the member_list
          TAO_THROW_ENV_RETURN (CORBA::NO_MEMORY (), env, 0);

        // skip the id, name, and discrimant type part
        if (!stream.skip_string ()        // type ID, hidden
            || !stream.skip_string ()     // typedef name
            || !skip_typecode (stream))   // skip typecode for discriminant
          TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, (char *)0);
        else if (!stream.read_ulong (temp)     // default used
                 || !stream.read_ulong (temp)) // member count
          TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);
        else
          {
            CORBA::Long scratch; // always big enough because labels can
            // only be of a few different types of
            // which "long" has the largest size

            // get the typecode for the discriminator
            tc = this->discriminator_type (env);
            // compute the name for all the members and return the
            // required one
            TAO_CHECK_ENV_RETURN (env, 0);

            for (CORBA::ULong i = 0; i < mcount; i++)
              {
                // the ith entry will have the name of the ith member
                CORBA::TypeCode::traverse_status status =
                  stream.decode (tc, &scratch, this,  env); // member label
                TAO_CHECK_ENV_RETURN (env, 0);

                if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
                  TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);

                // skip typecode for member
                if (!stream.read_string (this->private_state_->tc_member_name_list_ [i])
                    || (!skip_typecode (stream))) // skip typecode
                  TAO_THROW_ENV_RETURN ( CORBA::BAD_TYPECODE (), env, 0);
              }

            this->private_state_->tc_member_name_list_known_ = 1;

            if (index < mcount)
              return this->private_state_->tc_member_name_list_[index];
            else
              TAO_THROW_ENV_RETURN (CORBA::TypeCode::Bounds (), env, (char *)0);
          }
      }
      ACE_NOTREACHED (break;)

    default:
      // bad kind
      TAO_THROW_ENV_RETURN (CORBA::TypeCode::BadKind (), env, (char *)0);
    }
  ACE_NOTREACHED (return (char *)0;)
}

// Return member labels for CORBA::tk_union typecodes.
CORBA::Any_ptr
CORBA_TypeCode::private_member_label (CORBA::ULong n,
                                      CORBA::Environment &ACE_TRY_ENV) const
{
  // this function is only applicable to the CORBA::tk_union TC
  if (this->kind_ != CORBA::tk_union)
    ACE_THROW_RETURN (CORBA::TypeCode::BadKind (), 0);

  TAO_InputCDR stream (this->buffer_+4,
                       this->length_-4,
                       this->byte_order_);

  // skip ID and name
  if (!stream.skip_string ()       // type ID, hidden
      || !stream.skip_string ()    // typedef name
      || !skip_typecode (stream))  // skip discriminant typecode
    ACE_THROW_RETURN (CORBA::TypeCode::BadKind (), 0);

      // skip default used, and get member count
  CORBA::ULong member_count;

  if (!stream.read_ulong (member_count)      // default used
      || !stream.read_ulong (member_count))  // member count
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TypeCode::private_member_label -- "
                  "error reading from stream"));
    ACE_THROW_RETURN (CORBA::BAD_TYPECODE (), 0);
    }

  // Double checked locking...
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                    this->private_state_->mutex_, 0);
  if (this->private_state_->tc_member_label_list_known_)
    if (n < member_count)
      return this->private_state_->tc_member_label_list_[n];
    else
      ACE_THROW_RETURN (CORBA::TypeCode::Bounds (), 0);

  // member labels are of type Any. However, the actual types are
  // restricted to simple types


  // @@EXC@@ We should use Auto_Ptr_Array to make this exception
  // safe.
  CORBA::Any_ptr* label_list;
  ACE_NEW_THROW_EX (label_list,
                    CORBA::Any_ptr [member_count],
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  // get the discriminant TC
  CORBA::TypeCode_ptr tc = this->discriminator_type (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  for (CORBA::ULong i = 0; i < member_count; i++)
    {
      // Create an any from the portion of the CDR stream created
      // above.....

      // @@ This code assumes that the stream is a single message
      //    block, this is perfectly OK [the stream is created from a
      //    single buffer], but we may need to change this is the
      //    stream creation changes. [Carlos]
      TAO_InputCDR temp (stream);

      char *begin = stream.rd_ptr ();
      int retval = temp.skip (tc, ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
        return 0;

      char* end = temp.rd_ptr ();

      // We need to allocate more memory than in the original
      // stream, first to guarantee that the buffer is aligned in
      // memory and next because the realignment may introduce
      // extra padding. 2*MAX_ALIGNMENT should be enough.
      // @@EXC@@ This doesn't seem to be exception safe.
      TAO_OutputCDR out (end - begin + 2 * ACE_CDR::MAX_ALIGNMENT,
                         ACE_CDR_BYTE_ORDER,
                         ACE_Allocator::instance (),
                         ACE_Allocator::instance ());

      retval = out.append (tc, &stream, ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
      if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
        return 0;

      // @@EXC@@ Need to check memory allocation failure.
      ACE_NEW_THROW_EX (label_list[i],
                        CORBA::Any (tc, 0, out.begin ()),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      if (stream.skip_string () == 0
          || this->skip_typecode (stream) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        "TypeCode::private_member_label "
                        "error getting typecode for member %d\n",
                        i));
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                            0);
        }
    }

  this->private_state_->tc_member_label_list_ = label_list;
  this->private_state_->tc_member_label_list_known_ = 1;

  // If caller asked for the label for a nonexistent member, they get
  // an error report!
  if (n >= member_count)
    ACE_THROW_RETURN (CORBA::TypeCode::Bounds (), 0);

  return this->private_state_->tc_member_label_list_[n];
}

CORBA::TypeCode_ptr
CORBA_TypeCode::private_discriminator_type (CORBA::Environment &env) const
{
  // Double checked locking...
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                    this->private_state_->mutex_, 0);
  if (this->private_state_->tc_discriminator_type_known_)
    return this->private_state_->tc_discriminator_type_;

  TAO_InputCDR stream (this->buffer_+4, this->length_-4,
                       this->byte_order_);

  // skip ID and name, and then get the discriminant TC

  if (!stream.skip_string () // type ID, hidden
      || !stream.skip_string () // typedef name
      || stream.decode (CORBA::_tc_TypeCode,
                        &this->private_state_->tc_discriminator_type_, this,
                        env) != CORBA::TypeCode::TRAVERSE_CONTINUE)
    TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);

  this->private_state_->tc_discriminator_type_known_ = 1;
  return this->private_state_->tc_discriminator_type_;
}

CORBA::Long
CORBA_TypeCode::private_default_index (CORBA::Environment &env) const
{
  // Double checked locking...
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                    this->private_state_->mutex_, 0);
  if (this->private_state_->tc_default_index_used_known_)
    return this->private_state_->tc_default_index_used_;

  TAO_InputCDR stream (this->buffer_+4, this->length_-4,
                       this->byte_order_);

  // skip ID and name, and then get the discriminant TC

  if (!stream.skip_string ()            // type ID, hidden
      || !stream.skip_string ()         // typedef name
      || !skip_typecode (stream)        // skip discriminant
      || !stream.read_long (this->private_state_->tc_default_index_used_))
    TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);

  this->private_state_->tc_default_index_used_known_ = 1;
  return this->private_state_->tc_default_index_used_;
}

CORBA::Long
CORBA_TypeCode::private_length (CORBA::Environment &env) const
{
  TAO_InputCDR stream (this->buffer_+4, this->length_-4,
                       this->byte_order_);
  switch (this->kind_)
    {
    case CORBA::tk_sequence:
    case CORBA::tk_array:
      {
        // Double checked locking...
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                          this->private_state_->mutex_, 0);
        if (this->private_state_->tc_length_known_)
          return this->private_state_->tc_length_;

        // skip the typecode of the element and get the bounds
        if (!skip_typecode (stream) // skip typecode
            || !stream.read_ulong (this->private_state_->tc_length_))
          TAO_THROW_ENV_RETURN (CORBA::BAD_PARAM (), env, 0);
        else
          {
            this->private_state_->tc_length_known_ = 1;
            return this->private_state_->tc_length_;
          }
      }
      ACE_NOTREACHED (break);

    case CORBA::tk_string:
    case CORBA::tk_wstring:
      {
        // Double checked locking...
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                          this->private_state_->mutex_, 0);
        if (this->private_state_->tc_length_known_)
          return this->private_state_->tc_length_;

        if (stream.read_ulong (this->private_state_->tc_length_))
          {
            this->private_state_->tc_length_known_ = 1;
            return this->private_state_->tc_length_;
          }
        else
          TAO_THROW_ENV_RETURN (CORBA::BAD_PARAM (), env, 0);
      }
      ACE_NOTREACHED (break);

    default:
      TAO_THROW_ENV_RETURN (CORBA::TypeCode::BadKind (), env, 0);
    }
  ACE_NOTREACHED (return 0);
}

CORBA::TypeCode_ptr
CORBA_TypeCode::private_content_type (CORBA::Environment &env) const
{
  TAO_InputCDR stream (this->buffer_+4, this->length_-4,
                       this->byte_order_);
  CORBA::TypeCode::traverse_status status = CORBA::TypeCode::TRAVERSE_STOP;

  switch (kind_)
    {
    case CORBA::tk_sequence:
    case CORBA::tk_array:
      {
        // Double checked locking...
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                          this->private_state_->mutex_, 0);
        if (this->private_state_->tc_content_type_known_)
          return this->private_state_->tc_content_type_;

        // retrieve the content type
        status = stream.decode (CORBA::_tc_TypeCode,
                           &this->private_state_->tc_content_type_,
                           this,  env);
        TAO_CHECK_ENV_RETURN (env, 0);

        if (status != CORBA::TypeCode::TRAVERSE_CONTINUE) // element type
          TAO_THROW_ENV_RETURN (CORBA::BAD_PARAM (), env, 0);
        else
          {
            this->private_state_->tc_content_type_known_ = 1;
            return this->private_state_->tc_content_type_;
          }
      }
      ACE_NOTREACHED (break);

    case CORBA::tk_alias:
      {
        // Double checked locking...
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                          this->private_state_->mutex_, 0);
        if (this->private_state_->tc_content_type_known_)
          return this->private_state_->tc_content_type_;

        if (!stream.skip_string ()  // typeID
            || !stream.skip_string ()) // name
          TAO_THROW_ENV_RETURN (CORBA::BAD_PARAM (), env, 0);

        status = stream.decode (CORBA::_tc_TypeCode,
                                &this->private_state_->tc_content_type_, this,
                                env);
        TAO_CHECK_ENV_RETURN (env, 0);

        if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
          TAO_THROW_ENV_RETURN (CORBA::BAD_PARAM (), env, 0);
        else
          {
            this->private_state_->tc_content_type_known_ = 1;
            return this->private_state_->tc_content_type_;
          }
      }
      /*NOTREACHED*/

    default:
      TAO_THROW_ENV_RETURN (CORBA::TypeCode::BadKind (), env, 0);
    }
  ACE_NOTREACHED (return 0);
}

CORBA::ULong
CORBA_TypeCode::private_discrim_pad_size (CORBA::Environment &env)
{
  // Double checked locking...
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                    this->private_state_->mutex_, 0);
  if (this->private_state_->tc_discrim_pad_size_known_)
    return this->private_state_->tc_discrim_pad_size_;

  TAO_InputCDR stream (this->buffer_+4, this->length_-4,
                       this->byte_order_);

  size_t discrim_size;
  size_t overall_align;

 (void) TAO_CDR_Interpreter::calc_key_union_attributes (&stream,
                                                         overall_align,
                                                         discrim_size,
                                                         env);
 TAO_CHECK_ENV_RETURN (env, 0);

 this->private_state_->tc_discrim_pad_size_known_ = 1;
 this->private_state_->tc_discrim_pad_size_ = discrim_size;
 return discrim_size;
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
        TAO_InputCDR stream (this->buffer_+4, this->length_-4,
                             this->byte_order_);

        // skip rest of header (type ID and name) and collect the
        // number of struct members
        if (!stream.skip_string ()                  // ID
            || !stream.skip_string ()       // struct name
            || !stream.read_ulong (members))
          TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);

        return 3 + 2 * members;
      }

    case CORBA::tk_enum:
      {
        CORBA::ULong members;
        TAO_InputCDR stream (this->buffer_+4, this->length_-4,
                             this->byte_order_);

        // skip rest of header (type ID and name) and collect the
        // number of struct members
        if (!stream.skip_string ()                      // ID
            || !stream.skip_string ()           // typedef name
            || !stream.read_ulong (members))
          TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);

        return 3 + members;
      }

    case CORBA::tk_union:
      {
        CORBA::ULong members;
        TAO_InputCDR stream (this->buffer_+4, this->length_-4,
                             this->byte_order_);

        // skip rest of header (type ID, name, etc...) and collect the
        // number of struct members
        if (!stream.skip_string ()                      // ID
            || !stream.skip_string ()           // struct name
            || !skip_typecode (stream)          // discriminant TC
            || !stream.read_ulong (members)      // default used
            || !stream.read_ulong (members))     // real member count
          TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);

        return 5 + 3 * members;
      }
    }
}

CORBA::Any_ptr
CORBA_TypeCode::parameter (const CORBA::Long index,
                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                    0);
}

// Tell user the size of an instance of the data type described by
// this typecode ... typically used to allocate memory.

size_t
CORBA::TypeCode::private_size (CORBA::Environment &env)
{
  if (kind_ >= CORBA::TC_KIND_COUNT)
    TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);

  env.clear ();

  // Double checked locking...
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                    this->private_state_->mutex_, 0);
  if (this->private_state_->tc_size_known_)
    return this->private_state_->tc_size_;

  if (TAO_CDR_Interpreter::table_[kind_].calc_ == 0)
    {
      private_state_->tc_size_known_ = 1;
      private_state_->tc_size_ =
        TAO_CDR_Interpreter::table_[kind_].size_;
      return private_state_->tc_size_;
    }

  size_t alignment;
  TAO_InputCDR stream (this->buffer_+4, this->length_-4,
                       this->byte_order_);

  private_state_->tc_size_known_ = 1;
  private_state_->tc_size_ =
    TAO_CDR_Interpreter::table_[kind_].calc_ (&stream, alignment, env);
  return private_state_->tc_size_;
}

// Tell user the alignment restriction for the data type described by
// an instance of this data type.  Rarely used; provided for
// completeness.

size_t
CORBA::TypeCode::private_alignment (CORBA::Environment &env)
{
  env.clear ();

  if (kind_ >= CORBA::TC_KIND_COUNT)
    TAO_THROW_ENV_RETURN (CORBA::BAD_TYPECODE (), env, 0);

  // Double checked locking...
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard,
                    this->private_state_->mutex_, 0);
  if (this->private_state_->tc_alignment_known_)
    return this->private_state_->tc_alignment_;

  if (TAO_CDR_Interpreter::table_[kind_].calc_ == 0)
    {
      private_state_->tc_alignment_known_ = 1;
      private_state_->tc_alignment_ =
        TAO_CDR_Interpreter::table_[kind_].alignment_;
      return private_state_->tc_alignment_;
    }

  size_t alignment;
  TAO_InputCDR stream (this->buffer_+4, this->length_-4,
                       this->byte_order_);

  (void) TAO_CDR_Interpreter::table_[kind_].calc_ (&stream,
                                                    alignment,
                                                    env);
  TAO_CHECK_ENV_RETURN (env, 0);

  private_state_->tc_alignment_known_ = 1;
  private_state_->tc_alignment_ = alignment;
  return alignment;
}
