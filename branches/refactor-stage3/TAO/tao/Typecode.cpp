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
#include "tao/Marshal.h"
#include "tao/CORBA_String.h"
#include "tao/debug.h"
#include "ace/Malloc_Base.h"

#if !defined (__ACE_INLINE__)
# include "tao/Typecode.i"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (tao,
           Typecode,
           "$Id$")


// Typecodes essentially consist of just the CDR octets that get
// marshaled and unmarshaled, and this code knows how to parse those
// octets and answer questions CORBA's TypeCode APIs require.

CORBA::TypeCode::Bounds::Bounds (void)
  : CORBA::UserException ("IDL:omg.org/CORBA/TypeCode/Bounds:1.0",
                          "Bounds")
{
}

CORBA::TypeCode::Bounds*
CORBA::TypeCode::Bounds::_downcast (CORBA::Exception *ex)
{
  if (ex->_is_a ("IDL:omg.org/CORBA/TypeCode/Bounds:1.0"))
    return ACE_dynamic_cast (CORBA::TypeCode::Bounds*, ex);
  return 0;
}

CORBA::Exception *
CORBA::TypeCode::Bounds::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::CORBA::TypeCode::Bounds, 0);
  return retval;
}

CORBA::Exception *
CORBA::TypeCode::Bounds::_tao_duplicate (void) const
{
  CORBA::Exception *result;
  ACE_NEW_RETURN (result,
                  CORBA::TypeCode::Bounds (*this),
                  0);
  return result;
}

void
CORBA::TypeCode::Bounds::_raise (void)
{
  TAO_RAISE(*this);
}

void
CORBA::TypeCode::Bounds::_tao_encode (TAO_OutputCDR &cdr
                                      ACE_ENV_ARG_DECL) const
{
  if (cdr << this->_rep_id ())
    {
      return;
    }

  ACE_THROW (CORBA::MARSHAL ());
}

void
CORBA::TypeCode::Bounds::_tao_decode (TAO_InputCDR &
                                      ACE_ENV_ARG_DECL_NOT_USED)
{
}

// ****************************************************************

CORBA::TypeCode::BadKind::BadKind (void)
  : CORBA::UserException ("IDL:omg.org/CORBA/TypeCode/BadKind:1.0",
                          "BadKind")
{
}

CORBA::TypeCode::BadKind*
CORBA::TypeCode::BadKind::_downcast (CORBA::Exception *ex)
{
  if (ex->_is_a ("IDL:omg.org/CORBA/TypeCode/BadKind:1.0"))
    return ACE_dynamic_cast (CORBA::TypeCode::BadKind*, ex);
  return 0;
}

CORBA::Exception *
CORBA::TypeCode::BadKind::_tao_duplicate (void) const
{
  CORBA::Exception *result;
  ACE_NEW_RETURN (result,
                  CORBA::TypeCode::BadKind (*this),
                  0);
  return result;
}

void
CORBA::TypeCode::BadKind::_raise (void)
{
  TAO_RAISE(*this);
}

void
CORBA::TypeCode::BadKind::_tao_encode (TAO_OutputCDR &cdr
                                       ACE_ENV_ARG_DECL) const
{
  if (cdr << this->_rep_id ())
    {
      return;
    }

  ACE_THROW (CORBA::MARSHAL ());
}

void
CORBA::TypeCode::BadKind::_tao_decode (TAO_InputCDR &
                                       ACE_ENV_ARG_DECL_NOT_USED)
{
}

// ****************************************************************

// Constructor for CONSTANT typecodes with empty parameter lists.
// These are only created once, and those constants are shared.

CORBA::TypeCode::TypeCode (CORBA::TCKind kind)
  : length_ (0),
    buffer_ (0),
    byte_order_ (ACE_CDR_BYTE_ORDER),
    kind_ (kind),
    parent_ (0),
    tc_base_ (0),
    root_tc_base_ (0),
    refcount_ (1),
    orb_owns_ (1),
    private_state_ (new TAO::TC_Private_State (kind)),
    non_aligned_buffer_ (0),
    offset_map_ (0)
{
}

CORBA::TypeCode::TypeCode (CORBA::TCKind kind,
                           size_t length,
                           const char *buffer,
                           CORBA::Boolean orb_owns_tc,
                           CORBA::ULong /* size */,
                           CORBA::TypeCode_ptr parent)
  : length_ (length),
    kind_ (kind),
    parent_ (parent),
    refcount_ (1),
    orb_owns_ (orb_owns_tc),
    private_state_ (new TAO::TC_Private_State (kind)),
    non_aligned_buffer_ (0),
    offset_map_ (0)
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

      // *NOTE* that the buffer parameter is simply our encapsulation. It
      // does not contain our TypeCode::kind () and the length. These are
      // passed as separate parameters. However, in case of indirected
      // offset value for the indirection will effectively point to the
      // typecodes, the tk_kind field in our CDR representation. Hence,
      // we introduce a new field called tc_base_ which represents the
      // start of our CDR representation. The buffer_ data member will
      // point to our encapsulation.

      // @@ The typecode buffer contain the encapsulation byte order
      // in the first byte...
      const CORBA::Octet *ptr =
        ACE_reinterpret_cast (const CORBA::Octet *,
                              buffer);
      this->byte_order_ = *ptr;

      // allocate a buffer which will accomodate our entire encapsulation
      // plus 4 bytes for our tk_kind value and 4 bytes for our encapsulation
      // length. The extra MAX_ALIGNMENT bytes are necessary to ensure
      // that we will get a properly aligned buffer.

      static const size_t lsize = sizeof (CORBA::ULong);
      ACE_NEW (this->non_aligned_buffer_,
               char [this->length_ + lsize + lsize + ACE_CDR::MAX_ALIGNMENT]);

      char* start = ACE_ptr_align_binary (this->non_aligned_buffer_,
                                          ACE_CDR::MAX_ALIGNMENT);

      // length_ is of size_t which, on 64-bit platforms, is 64 bits.
      // The value to be copied is expected to be 32-bit.  We will cast
      // the value down to a CORBA::ULong and copy that.
      CORBA::ULong length = ACE_static_cast (CORBA::ULong, this->length_);

      (void) ACE_OS::memcpy (start, &this->kind_, lsize);
      (void) ACE_OS::memcpy (start + lsize, &length, lsize);
      (void) ACE_OS::memcpy (start + lsize + lsize, buffer, this->length_);
      // we are the topmost level typecode and hence our typecode base is
      // the start whereas the buffer_ which represents the encapsulation
      // is 8 bytes ahead of the typecode base
      this->tc_base_ = start;
      // since we do not have any parents, we are the root
      this->root_tc_base_ = start;
      this->buffer_ = start + lsize + lsize;
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
      // our typecode base is 8 bytes prior to our encapsulation and our
      // root base is the same as that of our parent's
      this->tc_base_ = this->buffer_ - 8;
      this->root_tc_base_ = parent->root_tc_base_;
    }
}

// Destructor.  For "indirected" typecodes and children, the typecode
// reuses the buffer owned by its parent.

CORBA::TypeCode::~TypeCode (void)
{
  // Delete the original, possibly nonaligned, buffer.
  delete [] this->non_aligned_buffer_;
  this->non_aligned_buffer_ = 0;

  this->buffer_ = 0;

  // Free up our private state.
  delete this->private_state_;
  this->private_state_ = 0;

  delete this->offset_map_;
  this->offset_map_ = 0;
}


// Return the i-th member typecode if it exists, else raise an
// exception. Possible exceptions are BadKind and Bounds.
//
// Applicable only to struct, union, and except

CORBA::TypeCode_ptr
CORBA::TypeCode::member_type (CORBA::ULong slot
                              ACE_ENV_ARG_DECL) const
{
  if (this->private_state_->tc_member_count_known_
      && this->private_state_->tc_member_type_list_known_)
    {
      if (slot < this->private_state_->tc_member_count_)
        {
          return
            CORBA::TypeCode::_duplicate (
              this->private_state_->tc_member_type_list_[slot]);
        }
      else
        {
          ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                            CORBA::TypeCode::_nil ());
        }
    }
  else
    {
      return
        CORBA::TypeCode::_duplicate (
          this->private_member_type (slot
                                     ACE_ENV_ARG_PARAMETER));
    }
}

// Applicable only to struct, union, and except

const char *
CORBA::TypeCode::member_name (CORBA::ULong slot
                              ACE_ENV_ARG_DECL) const
{
  if (this->private_state_->tc_member_count_known_
      && this->private_state_->tc_member_name_list_known_)
    {
      if (slot < this->private_state_->tc_member_count_)
        {
          return this->private_state_->tc_member_name_list_[slot];
        }
      else
        {
          ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                            0);
        }
    }
  else
    {
      return this->private_member_name (slot
                                         ACE_ENV_ARG_PARAMETER);
    }
}

// Return the label of the i-th member.  Applicable only to CORBA::tk_union
CORBA::Any_ptr
CORBA::TypeCode::member_label (CORBA::ULong slot
                               ACE_ENV_ARG_DECL) const
{
  CORBA::Any_ptr retval = 0;

  if (this->private_state_->tc_member_count_known_
      && this->private_state_->tc_member_label_list_known_)
    {
      if (slot < this->private_state_->tc_member_count_)
        {
          ACE_NEW_RETURN (
              retval,
              CORBA::Any (*this->private_state_->tc_member_label_list_[slot]),
              0
            );
        }
      else
        {
          ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                            0);
        }
    }
  else
    {
      CORBA::Any_ptr label =
        this->private_member_label (slot
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      ACE_NEW_RETURN (retval,
                      CORBA::Any (*label),
                      0);
    }

  return retval;
}

// only applicable to CORBA::tk_unions
CORBA::TypeCode_ptr
CORBA::TypeCode::discriminator_type (ACE_ENV_SINGLE_ARG_DECL) const
{
  if (this->kind_ != CORBA::tk_union)
    ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                      CORBA::TypeCode::_nil ());

  if (this->private_state_->tc_discriminator_type_known_)
    return
      CORBA::TypeCode::_duplicate (
        this->private_state_->tc_discriminator_type_);

  CORBA::TypeCode_ptr type =
    this->private_discriminator_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return CORBA::TypeCode::_duplicate (type);
}

// only applicable to CORBA::tk_unions
CORBA::Long
CORBA::TypeCode::default_index (ACE_ENV_SINGLE_ARG_DECL) const
{
  if (this->kind_ != CORBA::tk_union)
    ACE_THROW_RETURN (CORBA::TypeCode::BadKind (), 0);

  if (this->private_state_->tc_default_index_used_known_)
    return this->private_state_->tc_default_index_used_;
  else
    return this->private_default_index (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// returns the length. Applicable only to string, sequence, and arrays
CORBA::ULong
CORBA::TypeCode::length (ACE_ENV_SINGLE_ARG_DECL) const
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
        return this->private_length (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  else
    ACE_THROW_RETURN (CORBA::TypeCode::BadKind (), 0);
}

// returns the typecode. Applicable only to string, sequence, and arrays
CORBA::TypeCode_ptr
CORBA::TypeCode::content_type (ACE_ENV_SINGLE_ARG_DECL) const
{
  if (this->kind_ == CORBA::tk_sequence
      || this->kind_ == CORBA::tk_array
      || this->kind_ == CORBA::tk_alias)
    {
      if (this->private_state_->tc_content_type_known_)
        return CORBA::TypeCode::_duplicate (
                   this->private_state_->tc_content_type_
                 );
      else
        {
          CORBA::TypeCode_ptr tmp =
            this->private_content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

          return CORBA::TypeCode::_duplicate (tmp);
        }
    }
  else
    {
      ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                        0);
    }

}

CORBA::Visibility
CORBA::TypeCode::member_visibility (CORBA::ULong slot
                                    ACE_ENV_ARG_DECL) const
{
  if (this->private_state_->tc_member_count_known_
      && this->private_state_->tc_member_visibility_list_known_)
    {
      if (slot < this->private_state_->tc_member_count_)
        {
          return this->private_state_->tc_member_visibility_list_[slot];
        }
      else
        {
          ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                            0);
        }
    }
  else
    {
      CORBA::Visibility v =
        this->private_member_visibility (slot
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::PRIVATE_MEMBER);

      return v;
    }
}

CORBA::ValueModifier
CORBA::TypeCode::type_modifier (ACE_ENV_SINGLE_ARG_DECL) const
{
  if (this->private_state_->tc_type_modifier_known_)
    {
      return this->private_state_->tc_type_modifier_;
    }
  else
    {
      CORBA::ValueModifier vm =
        this->private_type_modifier (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::VM_NONE);

      return vm;
    }
}

CORBA::TypeCode_ptr
CORBA::TypeCode::concrete_base_type (ACE_ENV_SINGLE_ARG_DECL) const
{
  if (this->private_state_->tc_concrete_base_type_known_)
    {
      return CORBA::TypeCode::_duplicate (
          this->private_state_->tc_concrete_base_type_
          );
    }
  else
    {
      CORBA::TypeCode_ptr tmp =
        this->private_concrete_base_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      return CORBA::TypeCode::_duplicate (tmp);
    }
}

// skip a typecode encoding in a given CDR stream
// This is just a helper function
CORBA::Boolean
CORBA::TypeCode::skip_typecode (TAO_InputCDR &stream)
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
          return (stream.read_ulong (temp) != 0);

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
        case CORBA::tk_value:
          return (stream.read_ulong (temp) != 0
                  && stream.skip_bytes (temp) != 0);
        }

      return 1;
    }
  else
    return 0;
}

// constructor for the private state
TAO::TC_Private_State::TC_Private_State (CORBA::TCKind kind)
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
    tc_discrim_pad_size_known_ (0),
    tc_member_visibility_list_known_ (0),
    tc_type_modifier_known_ (0),
    tc_concrete_base_type_known_ (0),
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
    tc_member_visibility_list_ (0),
    tc_type_modifier_ (0),
    tc_concrete_base_type_ (0)
{
}

// Destructor for the private state. In effect, this cleans up all the
// children and the subtree we hold.
TAO::TC_Private_State::~TC_Private_State (void)
{
  // The following just point into the buffer. So we just make it
  // point to 0.
  this->tc_id_ = 0;

  if (this->tc_name_ != 0)
    {
      CORBA::string_free (this->tc_name_);
      this->tc_name_ = 0;
    }

  // Determine what kind of children we may have and free the space
  // accordingly.
  switch (this->tc_kind_)
    {
      case CORBA::tk_enum:

        // free up the member name list
        if (this->tc_member_name_list_known_)
          {
            for (CORBA::ULong i = 0; i < this->tc_member_count_; ++i)
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

        // Free up the member name list
        if (this->tc_member_name_list_known_)
          {
            for (CORBA::ULong i = 0; i < this->tc_member_count_; ++i)
              {
                CORBA::string_free (this->tc_member_name_list_ [i]);
                this->tc_member_name_list_ [i] = 0;
              }

            delete [] this->tc_member_name_list_;
            this->tc_member_name_list_ = 0;
          }

        // Free up member type list
        if (this->tc_member_type_list_known_)
          {
            for (CORBA::ULong i = 0; i < this->tc_member_count_; ++i)
              {
                CORBA::release (this->tc_member_type_list_[i]);
              }

            // Now free up the array.
            delete [] this->tc_member_type_list_;
            this->tc_member_type_list_ = 0;
          }

        this->tc_member_count_ = 0;

        break;

      case CORBA::tk_sequence:
      case CORBA::tk_array:
      case CORBA::tk_alias:
        // Delete the content type only if it has a parent i.e., if it
        // is not acquired from the pool of constant or predefined
        // TypeCodes.
        if (this->tc_content_type_known_ && this->tc_content_type_ != 0)
          {
            CORBA::release (this->tc_content_type_);
            this->tc_content_type_ = 0;
          }

        break;

      case CORBA::tk_union:
        {
          // Free up the member name list.
          if (this->tc_member_name_list_known_)
            {
              for (CORBA::ULong i = 0; i < this->tc_member_count_; ++i)
                {
                  CORBA::string_free (this->tc_member_name_list_ [i]);
                  this->tc_member_name_list_ [i] = 0;
                }

              delete [] this->tc_member_name_list_;
            }

          // Free up type list, label list, and finally the
          // discriminator.
          if (this->tc_member_type_list_known_)
            {
              for (CORBA::ULong i = 0; i < this->tc_member_count_; ++i)
                {
                  CORBA::release (this->tc_member_type_list_[i]);
                }

              // Now free up the array.
              delete [] this->tc_member_type_list_;
              this->tc_member_type_list_ = 0;
            }

          if (this->tc_member_label_list_known_)
            {
              for (CORBA::ULong i = 0; i < this->tc_member_count_; ++i)
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

      case CORBA::tk_value:
        // Free up the member name list.
        if (this->tc_member_name_list_known_)
          {
            for (CORBA::ULong i = 0; i < this->tc_member_count_; ++i)
              {
                CORBA::string_free (this->tc_member_name_list_ [i]);
                this->tc_member_name_list_ [i] = 0;
              }

            delete [] this->tc_member_name_list_;
            this->tc_member_name_list_ = 0;
          }

        // Free up member type list.
        if (this->tc_member_type_list_known_)
          {
            for (CORBA::ULong i = 0; i < this->tc_member_count_; ++i)
              {
                CORBA::release (this->tc_member_type_list_[i]);
              }

            // Now free up the array.
            delete [] this->tc_member_type_list_;
            this->tc_member_type_list_ = 0;
          }

        this->tc_member_count_ = 0;

        // Free up the member visibility list.
        if (this->tc_member_visibility_list_known_)
          {
            delete [] this->tc_member_visibility_list_;
            this->tc_member_visibility_list_ = 0;
          }

        // Free up concrete base value typecode.
        if (this->tc_concrete_base_type_known_)
          {
            CORBA::release (this->tc_concrete_base_type_);
            this->tc_concrete_base_type_ = 0;
          }

        break;

    default:
      break;
    }
}

// Point of recursion for equal() and equivalent().
CORBA::Boolean
CORBA::TypeCode::equ_common (CORBA::TypeCode_ptr tc,
                             CORBA::Boolean equiv_only
                             ACE_ENV_ARG_DECL) const
{
  // This will catch applications that haven't initialized an ORB.
  if (CORBA::is_nil (tc))
    {
      return 0;
    }

  // Are the two pointers the same?
  if (this == tc)
    {
      return 1;
    }

  if (equiv_only)
    {
      CORBA::TypeCode_var rcvr =
        CORBA::TypeCode::_duplicate (ACE_const_cast (CORBA::TypeCode *,
                                                     this));

      CORBA::Boolean status = (this->kind_ == CORBA::tk_alias);

      while (status)
        {
          rcvr = rcvr->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          status =
            (rcvr->kind (ACE_ENV_SINGLE_ARG_PARAMETER) == CORBA::tk_alias);
          ACE_CHECK_RETURN (0);
        }

      CORBA::TCKind kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
      status = (kind == CORBA::tk_alias);

      // Added by Bala to check for leaks as content_type duplicates the
      // pointers
      CORBA::TypeCode_var tcvar =
        CORBA::TypeCode::_duplicate (ACE_const_cast (CORBA::TypeCode *,
                                                     tc));

      while (status)
        {
          tcvar = tcvar->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          kind = tcvar->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
          status = (kind == CORBA::tk_alias);
        }

      kind = rcvr->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
      CORBA::TCKind other_kind = tcvar->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (kind != other_kind)
        {
          return 0;
        }

      // Typecode kinds are same.
      return rcvr->private_equal (tcvar.in (),
                                  equiv_only
                                  ACE_ENV_ARG_PARAMETER);
    }

  CORBA::TCKind kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (this->kind_ != kind)
    {
      return 0;
    }

  // typecode kinds are same
  return this->private_equal (tc,
                              equiv_only
                              ACE_ENV_ARG_PARAMETER);
}

// check if typecodes are equal. Equality is based on a mix of structural
// and name equivalence i.e., if names are provided, we also check for name
// equivalence, else resort simply to structural equivalence.
CORBA::Boolean
CORBA::TypeCode::private_equal (CORBA::TypeCode_ptr tc,
                                CORBA::Boolean equiv_only
                                ACE_ENV_ARG_DECL) const
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
      // all these are simple typecodes and the comparison is based
      // solely on the kind_ field
      return 1;
    case CORBA::tk_component:
    case CORBA::tk_home:
    case CORBA::tk_objref:
    case CORBA::tk_abstract_interface:
    case CORBA::tk_local_interface:
      return this->private_equal_objref (tc,
                                         equiv_only
                                         ACE_ENV_ARG_PARAMETER);
    case CORBA::tk_struct:
      return this->private_equal_struct (tc,
                                         equiv_only
                                         ACE_ENV_ARG_PARAMETER);
    case CORBA::tk_union:
      return this->private_equal_union (tc,
                                        equiv_only
                                        ACE_ENV_ARG_PARAMETER);
    case CORBA::tk_enum:
      return this->private_equal_enum (tc,
                                       equiv_only
                                       ACE_ENV_ARG_PARAMETER);
    case CORBA::tk_string:
    case CORBA::tk_wstring:
      return this->private_equal_string (tc,
                                         equiv_only
                                         ACE_ENV_ARG_PARAMETER);
    case CORBA::tk_sequence:
      return this->private_equal_sequence (tc,
                                           equiv_only
                                           ACE_ENV_ARG_PARAMETER);
    case CORBA::tk_array:
      return this->private_equal_array (tc,
                                        equiv_only
                                        ACE_ENV_ARG_PARAMETER);
    case CORBA::tk_value_box:
    case CORBA::tk_alias:
      return this->private_equal_alias (tc,
                                        equiv_only
                                        ACE_ENV_ARG_PARAMETER);
    case CORBA::tk_except:
      return this->private_equal_except (tc,
                                         equiv_only
                                         ACE_ENV_ARG_PARAMETER);
    case CORBA::tk_value:
    case CORBA::tk_event:
      return this->private_equal_valuetype (tc,
                                            equiv_only
                                            ACE_ENV_ARG_PARAMETER);
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
CORBA::TypeCode::private_equal_objref (
    CORBA::TypeCode_ptr tc,
    CORBA::Boolean equiv_only
    ACE_ENV_ARG_DECL
  ) const
{
  // Compare the repoID and name, of which the name is optional as per GIOP
  // spec. However, the repoID is mandatory.
  const char *my_id = this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  const char *tc_id = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (ACE_OS::strcmp (my_id, tc_id) == 0)
    {
      // Equality of repoIDs is sufficient for equivalence.
      if (equiv_only)
        {
          return 1;
        }
      // Name check is skipped by equivalent().
      else
        {
          // same repository IDs. Now check their names
          const char *myname = this->name (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          const char *tcname = tc->name (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          if ((ACE_OS::strlen (myname) > 1)
              && (ACE_OS::strlen (tcname) > 1))
            {
              // Both of them specify names, compare them
              if (ACE_OS::strcmp (myname, tcname) == 0)
                {
                  return 1; // success
                }
              else
                {
                  return 0; // failed
                }
            }

          return 1; // success
        }
    }

  return 0; // failed
}

CORBA::Boolean
CORBA::TypeCode::private_equal_struct (
    CORBA::TypeCode_ptr tc,
    CORBA::Boolean equiv_only
    ACE_ENV_ARG_DECL
  ) const
{
  // For structs the repoID and names are optional. However, if provided,
  // we must compare them
  const char *my_id = this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  const char *tc_id = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // compare repoIDs if they exist
  if (ACE_OS::strlen (my_id) > 1 && ACE_OS::strlen (tc_id) > 1)
    {
      if (ACE_OS::strcmp (my_id, tc_id)) // not same
        {
          return 0;
        }
      else
        {
          // Equality of repoIDs is sufficient for equivalence.
          if (equiv_only)
            {
              return 1;
            }
        }
    }

  // Skipped by equivalent().
  if (!equiv_only)
    {
      const char *my_name = this->name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      const char *tc_name = tc->name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // Compare names if they exist.
      if (ACE_OS::strlen (my_name) > 1
          && ACE_OS::strlen (tc_name) > 1
          && ACE_OS::strcmp (my_name, tc_name) != 0) // not same
        {
          return 0;
        }
    }

  // Check if the member count is same.
  CORBA::ULong my_count = this->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong tc_count = tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (my_count != tc_count)
    {
      return 0; // number of members don't match
    }

  // The checks below tell if we have a recursive struct.
  CORBA::TypeCode_ptr par = this->parent_;

  if (par != 0)
    {
      if (this->tc_base_ == this->root_tc_base_)
        {
          return 1;
        }

      CORBA::TypeCode_ptr tc_par = tc->parent_;

      if (tc_par)
        {
          CORBA::TypeCode_ptr gpar = par->parent_;
          CORBA::TypeCode_ptr tc_gpar = tc_par->parent_;

          if (gpar != 0
              && tc_gpar != 0
              && this->tc_base_ == gpar->tc_base_
              && tc->tc_base_ == tc_gpar->tc_base_)
            {
              return 1;
            }
        }
    }

  for (CORBA::ULong i = 0; i < my_count; ++i)
    {
      // Skipped by equivalent().
      if (!equiv_only)
        {
          const char *my_member_name =
            this->member_name (i ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          const char *tc_member_name =
            tc->member_name (i ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          if (ACE_OS::strlen (my_member_name) > 1
              && ACE_OS::strlen (tc_member_name) > 1
              && ACE_OS::strcmp (my_member_name, tc_member_name) != 0)
            {
              return 0;
            }
        }

      // now compare the typecodes of the members
      CORBA::TypeCode_var my_member_tc =
        this->member_type (i
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::TypeCode_var tc_member_tc =
        tc->member_type (i
                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // One of our members may be recursive, but not through us.
      if (my_member_tc->parent_ != 0
          && my_member_tc->parent_ == tc_member_tc->parent_
          && my_member_tc->tc_base_ == tc_member_tc->tc_base_
          && my_member_tc->root_tc_base_ == tc_member_tc->root_tc_base_)
        {
          continue;
        }

      CORBA::Boolean flag =
        my_member_tc->equ_common (tc_member_tc.in (),
                                  equiv_only
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (!flag)
        {
          return 0;
        }
    }

  return 1; // success
}

CORBA::Boolean
CORBA::TypeCode::private_equal_union (CORBA::TypeCode_ptr tc,
                                      CORBA::Boolean equiv_only
                                      ACE_ENV_ARG_DECL) const
{
  // For unions the repoID and names are optional. However, if provided, we
  // must compare them
  const char *my_id = this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  const char *tc_id = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // compare repoIDs if they exist
  if (ACE_OS::strlen (my_id) > 1 && ACE_OS::strlen (tc_id) > 1)
    {
      if (ACE_OS::strcmp (my_id, tc_id)) // not same
        {
          return 0;
        }
      else
        {
          // Equality of repoIDs is sufficient for equivalence.
          if (equiv_only)
            {
              return 1;
            }
        }
    }

  // Skipped by equivalent().
  if (!equiv_only)
    {
      const char *my_name = this->name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      const char *tc_name = tc->name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // Compare names if they exist.
      if (ACE_OS::strlen (my_name) > 1
          && ACE_OS::strlen (tc_name) > 1
          && ACE_OS::strcmp (my_name, tc_name) != 0) // not same
        {
          return 0;
        }
    }

  // check if the discriminant type is same
  CORBA::TypeCode_var my_discrim =
    this->discriminator_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::TypeCode_var tc_discrim =
    tc->discriminator_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Boolean status = my_discrim->equ_common (tc_discrim.in (),
                                                  equiv_only
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (status == 0)
    {
      return 0;
    }

  // Check the default used.
  CORBA::Long my_default = this->default_index (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Long tc_default = tc->default_index (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (my_default != tc_default)
    {
      return 0;
    }

  // check if the member count is same
  CORBA::ULong my_count = this->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong tc_count = tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (my_count != tc_count)
    {
      return 0;
    }

  // The checks below indicate that we have a recursive union.
  CORBA::TypeCode_ptr par = this->parent_;

  if (par != 0)
    {
      if (this->tc_base_ == this->root_tc_base_)
        {
          return 1;
        }

      CORBA::TypeCode_ptr tc_par = tc->parent_;

      if (tc_par)
        {
          CORBA::TypeCode_ptr gpar = par->parent_;
          CORBA::TypeCode_ptr tc_gpar = tc_par->parent_;

          if (gpar != 0
              && tc_gpar != 0
              && this->tc_base_ == gpar->tc_base_
              && tc->tc_base_ == tc_gpar->tc_base_)
            {
              return 1;
            }
        }
    }

  for (CORBA::ULong i = 0; i < my_count; ++i)
    {
      // First check if labels are same.

      // Check if member names are same - skipped by equivalent().
      if (!equiv_only)
        {
          const char *my_member_name =
            this->member_name (i
                               ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          const char *tc_member_name =
            tc->member_name (i
                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          if (ACE_OS::strlen (my_member_name) > 1
              && ACE_OS::strlen (tc_member_name) > 1
              && ACE_OS::strcmp (my_member_name,
                                 tc_member_name)) // not same
            {
              return 0;
            }
        }

      // now compare the typecodes of the members
      CORBA::TypeCode_var my_member_tc =
        this->member_type (i
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::TypeCode_var tc_member_tc =
        tc->member_type (i
                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // One of our members may be recursive, but not through us.
      if (my_member_tc->parent_ != 0
          && my_member_tc->parent_ == tc_member_tc->parent_
          && my_member_tc->tc_base_ == tc_member_tc->tc_base_
          && my_member_tc->root_tc_base_ == tc_member_tc->root_tc_base_)
        {
          continue;
        }

      CORBA::Boolean flag = my_member_tc->equ_common (tc_member_tc.in (),
                                                      equiv_only
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (flag == 0)
        {
          return 0;
        }
    }

  return 1; // success
}

CORBA::Boolean
CORBA::TypeCode::private_equal_enum (CORBA::TypeCode_ptr tc,
                                     CORBA::Boolean equiv_only
                                     ACE_ENV_ARG_DECL) const
{
  // For enums the repoID and names are optional. However, if
  // provided, we must compare them.
  const char *my_id = this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  const char *tc_id = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // compare repoIDs if they exist
  if (ACE_OS::strlen (my_id) > 1 && ACE_OS::strlen (tc_id) > 1)
    {
      if (ACE_OS::strcmp (my_id, tc_id)) // not same
        {
          return 0;
        }
      else
        {
          // Equality of repoIDs is sufficient for equivalence.
          if (equiv_only)
            {
              return 1;
            }
        }
    }

  // Skipped by equivalent().
  if (!equiv_only)
    {
      const char *my_name = this->name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      const char *tc_name = tc->name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // Compare names if they exist.
      if (ACE_OS::strlen (my_name) > 1
          && ACE_OS::strlen (tc_name) > 1
          && ACE_OS::strcmp (my_name, tc_name)) // not same
        {
          return 0;
        }
    }

  // Check if the member count is same.
  CORBA::ULong my_count = this->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong tc_count = tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (my_count != tc_count)
    {
      return 0;
    }

  for (CORBA::ULong i=0; i < my_count; ++i)
    {
      // Check if member names are same - skipped by equivalent().
      if (!equiv_only)
        {
          const char *my_member_name =
            this->member_name (i
                               ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          const char *tc_member_name =
            tc->member_name (i
                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          if (ACE_OS::strlen (my_member_name) > 1
              && ACE_OS::strlen (tc_member_name) > 1
              && ACE_OS::strcmp (my_member_name, tc_member_name) != 0)
            {
              return 0;
            }
        }
    }

  return 1; // success
}

CORBA::Boolean
CORBA::TypeCode::private_equal_string (
    CORBA::TypeCode_ptr tc,
    CORBA::Boolean /* equiv_only */
    ACE_ENV_ARG_DECL
  ) const
{
  // compare the lengths
  CORBA::ULong my_len = this->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong tc_len = tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return (my_len == tc_len);
}

CORBA::Boolean
CORBA::TypeCode::private_equal_wstring (
    CORBA::TypeCode_ptr tc,
    CORBA::Boolean /* equiv_only */
    ACE_ENV_ARG_DECL
  ) const
{
  // compare the lengths
  CORBA::ULong my_len = this->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong tc_len = tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return (my_len == tc_len);
}

CORBA::Boolean
CORBA::TypeCode::private_equal_sequence (
    CORBA::TypeCode_ptr tc,
    CORBA::Boolean equiv_only
    ACE_ENV_ARG_DECL
  ) const
{
  // this involves comparing the typecodes of the element type as well
  // as the bounds
  CORBA::TypeCode_var my_elem =
    this->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::TypeCode_var tc_elem =
    tc->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Boolean status = my_elem->equ_common (tc_elem.in (),
                                               equiv_only
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (status == 0)
    {
      return 0;
    }

  // Now check if bounds are same.
  CORBA::ULong my_len = this->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong tc_len = tc->length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return (my_len == tc_len);
}

CORBA::Boolean
CORBA::TypeCode::private_equal_array (CORBA::TypeCode_ptr tc,
                                      CORBA::Boolean equiv_only
                                      ACE_ENV_ARG_DECL) const
{
  // Exactly like sequence
  return this->private_equal_sequence (tc,
                                       equiv_only
                                       ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
CORBA::TypeCode::private_equal_alias (CORBA::TypeCode_ptr tc,
                                      CORBA::Boolean equiv_only
                                      ACE_ENV_ARG_DECL) const
{
  // For structs the repoID and names are optional. However, if
  // provided, we must compare them.
  const char *my_id = this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  const char *tc_id = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  const char *my_name = this->name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  const char *tc_name = tc->name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Compare repository IDs if they exist.
  if (ACE_OS::strlen (my_id) > 1 && ACE_OS::strlen (tc_id) > 1)
    {
      if (ACE_OS::strcmp (my_id, tc_id) != 0) // not same
        {
          return 0;
        }
    }

  // Compare names if they exist.
  if (ACE_OS::strlen (my_name) > 1 && ACE_OS::strlen (tc_name) > 1)
    {
      if (ACE_OS::strcmp (my_name, tc_name) != 0) // not same
        {
          return 0;
        }
    }

  // Now compare element TypeCodes.
  CORBA::TypeCode_var my_elem =
    this->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::TypeCode_var tc_elem =
    tc->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return my_elem->equ_common (tc_elem.in (),
                              equiv_only
                              ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
CORBA::TypeCode::private_equal_except (CORBA::TypeCode_ptr tc,
                                       CORBA::Boolean equiv_only
                                       ACE_ENV_ARG_DECL) const
{
  // Exactly similar to structs, except that the repository ID is
  // mandatory.

  const char *my_id = this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  const char *tc_id = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (ACE_OS::strcmp (my_id, tc_id) != 0)
    {
      return 0; // failed
    }
  else
    {
      // Equality of repoIDs is sufficient for equivalence.
      if (equiv_only)
        {
          return 1;
        }
    }

  // Now compare names. They may be optional.
  const char *my_name = this->name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  const char *tc_name = tc->name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Compare names if they exist.
  if (ACE_OS::strlen (my_name) > 1
      && ACE_OS::strlen (tc_name) > 1
      && ACE_OS::strcmp (my_name, tc_name) != 0) // not same
    {
      return 0;
    }

  // Check if the member count is same.
  CORBA::ULong my_count = this->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong tc_count = tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (my_count != tc_count)
    {
      return 0; // number of members don't match
    }

  for (CORBA::ULong i = 0; i < my_count; ++i)
    {
      // Check if member names are same.
      const char *my_member_name =
        this->member_name (i
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      const char *tc_member_name =
        tc->member_name (i
                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (ACE_OS::strlen (my_member_name) > 1
          && ACE_OS::strlen (tc_member_name) > 1
          && ACE_OS::strcmp (my_member_name, tc_member_name) != 0)
        {
          return 0;
        }

      // Now compare the typecodes of the members.
      CORBA::TypeCode_var my_member_tc =
        this->member_type (i ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::TypeCode_var tc_member_tc =
        tc->member_type (i ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::Boolean flag = my_member_tc->equ_common (tc_member_tc.in (),
                                                      equiv_only
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (flag == 0)
        {
          return 0;
        }
    }

  return 1; // success
}

//@@ boris: This version is incomplete...
CORBA::Boolean
CORBA::TypeCode::private_equal_valuetype (
    CORBA::TypeCode_ptr tc,
    CORBA::Boolean equiv_only
    ACE_ENV_ARG_DECL
  ) const
{
  // Compare the repoID and name, of which the name is optional as per GIOP
  // spec. However, the repoID is mandatory.
  const char *my_id = this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  const char *tc_id = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (ACE_OS::strcmp (my_id, tc_id) != 0)
    {
      return 0;
    }
  else
    {
      // Equality of repoIDs is sufficient for equivalence.
      if (equiv_only)
        {
          return 1;
        }
    }

  // Now check their names.
  const char *myname = this->name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  const char *tcname = tc->name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (ACE_OS::strlen (myname) > 1
      && ACE_OS::strlen (tcname) > 1
      && ACE_OS::strcmp (myname, tcname) != 0)
    {
      return 0; // failed
    }

  // Compare value modifiers.
  CORBA::ValueModifier my_vm =
    this->type_modifier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ValueModifier tc_vm =
    tc->type_modifier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (my_vm != tc_vm)
    {
      return 0;
    }

  // Check if the member count is same.
  CORBA::ULong my_count = this->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong tc_count = tc->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (my_count != tc_count)
    {
      return 0; // number of members don't match
    }

  // The checks below indicate that we have a recursive union.
  CORBA::TypeCode_ptr par = this->parent_;

  if (par != 0)
    {
      if (this->tc_base_ == this->root_tc_base_)
        {
          return 1;
        }

      CORBA::TypeCode_ptr tc_par = tc->parent_;

      if (tc_par)
        {
          CORBA::TypeCode_ptr gpar = par->parent_;
          CORBA::TypeCode_ptr tc_gpar = tc_par->parent_;

          if (gpar != 0
              && tc_gpar != 0
              && this->tc_base_ == gpar->tc_base_
              && tc->tc_base_ == tc_gpar->tc_base_)
            {
              return 1;
            }
        }
    }

  for (CORBA::ULong i = 0; i < my_count; ++i)
    {
      // Check if the member visibilities are the same.
      CORBA::Visibility my_vs =
        this->member_visibility (i
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::Visibility tc_vs =
        tc->member_visibility (i
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (my_vs != tc_vs)
        {
          return 0;
        }

      // Check if the (optional) member names are same.
      const char *my_member_name =
        this->member_name (i
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      const char *tc_member_name =
        tc->member_name (i
                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (ACE_OS::strlen (my_member_name) > 1
          && ACE_OS::strlen (tc_member_name) > 1
          && ACE_OS::strcmp (my_member_name, tc_member_name) != 0)
        {
          return 0;
        }

      // Now compare the typecodes of the members.
      CORBA::TypeCode_var my_member_tc =
        this->member_type (i
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::TypeCode_var tc_member_tc =
        tc->member_type (i
                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::Boolean flag = my_member_tc->equ_common (tc_member_tc.in (),
                                                      equiv_only
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (flag == 0)
        {
          return 0;
        }
    }

  return 1;
}


// Return the type ID (RepositoryId) for the TypeCode; it may be empty.
//
// NOTE the string returned here is owned by the typecode!!
//
// Valid only for objref, struct, union, enum, alias, and except. Raises
// BadKind exception for the rest of the cases.
const char *
CORBA::TypeCode::private_id (ACE_ENV_SINGLE_ARG_DECL) const
{
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
    case CORBA::tk_value:
    case CORBA::tk_component:
    case CORBA::tk_home:
    case CORBA::tk_event:
      {
        // Double checked locking...
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                          guard,
                          this->private_state_->mutex_,
                          0);

        if (this->private_state_->tc_id_known_)
          {
            return this->private_state_->tc_id_;
          }

        this->private_state_->tc_id_known_ = 1;
        this->private_state_->tc_id_ = (ACE_const_cast (char *, buffer_)
                                        + 4    // skip byte order
                                        // flag and padding
                                        + 4);  // skip (strlen + 1)
        return this->private_state_->tc_id_; // this is OK because
                                             // strings in the CDR stream
                                             // are NULL terminated
      }
    // No other typecodes ever have type IDs
    default:
      ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                        0);
    }
}

// return the name. The string is owned by the typecode
const char *
CORBA::TypeCode::private_name (ACE_ENV_SINGLE_ARG_DECL) const
{
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
    case CORBA::tk_value:
    case CORBA::tk_component:
    case CORBA::tk_home:
    case CORBA::tk_event:
      {
        // Double checked locking...
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                          guard,
                          this->private_state_->mutex_,
                          0);

        if (this->private_state_->tc_name_known_)
          {
            return this->private_state_->tc_name_;
          }

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
          {
            ACE_THROW_RETURN (CORBA::INTERNAL (),
                              0);
          }
      }
    // No other typecodes ever have type IDs.
    default:
      ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                        0);
    }
}

// Return the number of members defined by this typecode
//
// Applicable to struct, union, enum, alias, and except
// For the rest of the cases, raises the BadKind exception.

CORBA::ULong
CORBA::TypeCode::private_member_count (ACE_ENV_SINGLE_ARG_DECL) const
{
  // Double checked locking...
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->private_state_->mutex_,
                    0);

  CORBA::ULong members;
  // setup an encapsulation
  TAO_InputCDR stream (this->buffer_+4,
                       this->length_-4,
                       this->byte_order_);

  switch (kind_)
    {
    case CORBA::tk_alias:
    case CORBA::tk_value_box:
      return 1;
    case CORBA::tk_enum:
    case CORBA::tk_except:
    case CORBA::tk_struct:
      {
        if (this->private_state_->tc_member_count_known_)
          {
            return this->private_state_->tc_member_count_;
          }

        // Skip rest of header (type ID and name) and collect the
        // number of struct members.
        if (!stream.skip_string ()          // ID
            || !stream.skip_string ()       // struct name
            || !stream.read_ulong (members))
          {
            ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                              0);
          }

        this->private_state_->tc_member_count_known_ = 1;
        this->private_state_->tc_member_count_ = members;
        return this->private_state_->tc_member_count_;
      }
    case CORBA::tk_union:
      {
        if (this->private_state_->tc_member_count_known_)
          {
            return this->private_state_->tc_member_count_;
          }

        // Skip rest of header (type ID, name, etc...) and collect the
        // number of struct members.
        if (!stream.skip_string ()                    // ID
            || !stream.skip_string ()                 // struct name
            || !this->skip_typecode (stream)          // discriminant TC
            || !stream.read_ulong (members)           // default used
            || !stream.read_ulong (members))          // real member count
          {
            ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                              0);
          }

        this->private_state_->tc_member_count_known_ = 1;
        this->private_state_->tc_member_count_ = members;
        return this->private_state_->tc_member_count_;
      }
    case CORBA::tk_value:
    case CORBA::tk_event:
      {
        if (this->private_state_->tc_member_count_known_)
          {
            return this->private_state_->tc_member_count_;
          }

        // skip rest of header (type ID, name, etc) and collect the
        // number of value members
        CORBA::ULong tc_kind_holder;
        CORBA::TCKind tc_kind;

        if (!stream.skip_string ()          // ID
            || !stream.skip_string ()       // Name
            || !stream.skip_ulong ()        // ValueModifier
            || !stream.read_ulong (tc_kind_holder)) // Base's TCKind
          {
            ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                              0);
          }

        tc_kind = ACE_static_cast (CORBA::TCKind,
                                   tc_kind_holder);

        // The tc_kind can be either tk_null or tk_value.
        // In the latter case we should skip encapsulation or
        // indirection - whatever comes in.
        CORBA::ULong encap_length;

        if (tc_kind != CORBA::tk_null)
          {
            if (!stream.read_ulong (encap_length))
              {
                ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                                  0);
              }

            if (tc_kind_holder != 0xffffffff) // check for indirection
              {
                if (!stream.skip_bytes (encap_length))
                  {
                    ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                                      0);
                  }
              }
          }

        // Now read member count.
        if (!stream.read_ulong (members))
          {
            ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                              0);
          }

        this->private_state_->tc_member_count_known_ = 1;
        this->private_state_->tc_member_count_ = members;
        return this->private_state_->tc_member_count_;
      }
    default:
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                        0);
    }
}

//
// Applicable only to struct, union, except, value and event.
//

CORBA::TypeCode_ptr
CORBA::TypeCode::private_member_type (CORBA::ULong slot
                                      ACE_ENV_ARG_DECL) const
{
  CORBA::ULong temp = 0;
  CORBA::ULong i = 0;
  CORBA::ULong mcount = this->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  // Build the de-encapsulating CDR stream, bypassing the stringent
  // alignment tests (we're a bit looser in what we need here, and we
  // _know_ we're OK).  Then skip the byte order code.
  TAO_InputCDR stream (this->buffer_+4,
                       this->length_-4,
                       this->byte_order_);
  CORBA::TypeCode_var tc;
  CORBA::TypeCode_ptr disc_tc = CORBA::TypeCode::_nil ();

  // Double checked locking...
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->private_state_->mutex_,
                    CORBA::TypeCode::_nil ());

  switch (kind_)
    {
    case CORBA::tk_except:
    case CORBA::tk_struct:
      if (this->private_state_->tc_member_type_list_known_)
        {
          if (slot < mcount)
            {
              return this->private_state_->tc_member_type_list_[slot];
            }
          else
            {
              ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                                CORBA::TypeCode::_nil ());
            }
        }

      // the first time in. Precompute and store types of all members

      // Allocate a list to hold the member typecodes
      ACE_NEW_THROW_EX (this->private_state_->tc_member_type_list_,
                        CORBA::TypeCode_ptr[mcount],
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      // skip the id, name, and member_count part
      if (!stream.skip_string ()        // type ID, hidden
          || !stream.skip_string ()     // typedef name
          || !stream.read_ulong (temp))  // member count
        {
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                            CORBA::TypeCode::_nil ());
        }

      // compute the typecodes for all the members and
      // return the required one.
      for (i = 0; i < mcount; ++i)
        // the ith entry will have the typecode of the ith guy
        {
          if (!stream.skip_string ())  // skip the name
            {
              ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                                CORBA::TypeCode::_nil ());
            }

          CORBA::TypeCode_ptr& member_type =
            this->private_state_->tc_member_type_list_[i];
          CORBA::TypeCode::_tao_decode (this,
                                       stream,
                                       member_type
                                       ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
        }

      this->private_state_->tc_member_type_list_known_ = 1;

      if (slot < mcount)
        {
          return this->private_state_->tc_member_type_list_[slot];
        }
      else
        {
          ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                            CORBA::TypeCode::_nil ());
        }
    case CORBA::tk_union:
      if (this->private_state_->tc_member_type_list_known_)
        {
          if (slot < mcount)
            {
              return  this->private_state_->tc_member_type_list_[slot];
            }
          else
            {
              ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                                CORBA::TypeCode::_nil ());
            }
        }

      // the first time in. Precompute and store types of all members
      ACE_NEW_THROW_EX (this->private_state_->tc_member_type_list_,
                        CORBA::TypeCode_ptr[mcount],
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      // skip the id, name, and discrimant type part
      if (!stream.skip_string ()              // type ID, hidden
          || !stream.skip_string ()           // typedef name
          || !this->skip_typecode (stream))   // skip discriminant typecode
        {
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                            CORBA::TypeCode::_nil ());
        }
      else if (!stream.read_ulong (temp)      // default used
               || !stream.read_ulong (temp))  // member count
        {
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                            CORBA::TypeCode::_nil ());
        }

      // Get the typecode for the discriminator.
      disc_tc =
        this->private_discriminator_type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      // Compute the typecodes for all the members and return the
      // required one.
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      for (i = 0; i < mcount; ++i)
        // The ith entry will have the typecode of the ith guy.
        {
          // Skip member label.
          CORBA::TypeCode::traverse_status status =
            TAO_Marshal_Object::perform_skip (disc_tc,
                                              &stream
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

          if (status != CORBA::TypeCode::TRAVERSE_CONTINUE
              || !stream.skip_string ())  // skip the name
            {
              ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                                CORBA::TypeCode::_nil ());
            }

          CORBA::TypeCode_ptr& member_type =
            this->private_state_->tc_member_type_list_[i];
          CORBA::TypeCode::_tao_decode (this,
                                       stream,
                                       member_type
                                       ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
        }

      this->private_state_->tc_member_type_list_known_ = 1;

      if (slot < mcount)
        {
          return this->private_state_->tc_member_type_list_[slot];
        }
      else
        {
          ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                            CORBA::TypeCode::_nil ());
        }
    case CORBA::tk_value:
    case CORBA::tk_event:
      if (this->private_state_->tc_member_type_list_known_)
        {
          if (slot < mcount)
            {
              return  this->private_state_->tc_member_type_list_[slot];
            }
          else
            {
              ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                                CORBA::TypeCode::_nil ());
            }
        }

      // the first time in. Precompute and store types of all members

      // Allocate a list to hold the member typecodes
      ACE_NEW_THROW_EX (this->private_state_->tc_member_type_list_,
                        CORBA::TypeCode_ptr [mcount],
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      // skip rest of header (type ID, name, etc) and collect the
      // number of value members
      CORBA::ULong tc_kind_holder;
      CORBA::TCKind tc_kind;

      if (!stream.skip_string ()          // ID
          || !stream.skip_string ()       // Name
          || !stream.skip_ulong ()        // ValueModifier
          || !stream.read_ulong (tc_kind_holder)) // Base's TCKind
        {
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                            CORBA::TypeCode::_nil ());
        }

      tc_kind = ACE_static_cast (CORBA::TCKind, tc_kind_holder);

      // The tc_kind can be either tk_null or tk_value.
      // In the latter case we should skip encapsulation or
      // indirection - whatever comes in.
      CORBA::ULong encap_length;

      if (tc_kind != CORBA::tk_null)
        {
          if (!stream.read_ulong (encap_length))
            {
              ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                                CORBA::TypeCode::_nil ());
            }

          if (tc_kind_holder != 0xffffffff) // check for indirection
            {
              if (!stream.skip_bytes (encap_length))
                {
                  ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                                    CORBA::TypeCode::_nil ());
                }
            }
        }

      // Now skip member count.
      if (!stream.skip_ulong ())
        {
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                            CORBA::TypeCode::_nil ());
        }

      // Compute the typecodes for all the members and
      // return the required one.
      for (i = 0; i < mcount; ++i)
        // The ith entry will have the typecode of the ith guy.
        {
          if (!stream.skip_string ())  // skip the name
            {
              ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                                CORBA::TypeCode::_nil ());
            }

          CORBA::TypeCode_ptr& member_type =
            this->private_state_->tc_member_type_list_[i];
          CORBA::TypeCode::_tao_decode (this,
                                       stream,
                                       member_type
                                       ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

          if (!stream.skip_ulong ())  // skip member visibility
            {
              ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                                CORBA::TypeCode::_nil ());
            }
        }

      this->private_state_->tc_member_type_list_known_ = 1;

      if (slot < mcount)
        {
          return this->private_state_->tc_member_type_list_[slot];
        }
      else
        {
          ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                            CORBA::TypeCode::_nil ());
        }
    default:
      // Bad kind.
      ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                        CORBA::TypeCode::_nil ());
    }
}

// Return the name for the nth member
// Applicable only to CORBA::tk_struct, CORBA::tk_union, CORBA::tk_enum,
// CORBA::tk_except, tk_value and tk_event.
const char *
CORBA::TypeCode::private_member_name (CORBA::ULong slot
                                      ACE_ENV_ARG_DECL) const
{
  CORBA::ULong temp, mcount;

  // Build the de-encapsulating CDR stream, bypassing the stringent
  // alignment tests (we're a bit looser in what we need here, and we
  // _know_ we're OK).  Then skip the byte order code.
  TAO_InputCDR stream (this->buffer_+4,
                       this->length_-4,
                       this->byte_order_);

  mcount = this->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  // out of bounds
  ACE_CHECK_RETURN (0);

  // Double checked locking...
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->private_state_->mutex_,
                    0);

  if (this->private_state_->tc_member_name_list_known_)
    {
      if (slot < mcount)
        {
          return this->private_state_->tc_member_name_list_[slot];
        }
      else
        {
          ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                            0);
        }
    }

  // the first time in. Precompute and store names of all members
  // Allocate a list to hold the member names
  ACE_NEW_THROW_EX (this->private_state_->tc_member_name_list_,
                    char* [mcount],
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  switch (kind_)
    {
    case CORBA::tk_enum:
      {
        // skip the id, name, and member_count part
        if (!stream.skip_string ()     // type ID, hidden
            || !stream.skip_string ()     // enum name
            || !stream.read_ulong (temp)) // member count
          {
            ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                              0);
          }

        // compute the typecodes for all the members and
        // return the required one.
        for (CORBA::ULong i = 0; i < mcount; ++i)
          {
          // Now read this name.
          if (!stream.read_string (
                      this->private_state_->tc_member_name_list_ [i]
                      ))
            {
              ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                                0);
            }
          }

        this->private_state_->tc_member_name_list_known_ = 1;

        if (slot < mcount)
          {
            return this->private_state_->tc_member_name_list_[slot];
          }
        else
          {
            ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                              0);
          }
      }
    case CORBA::tk_except:
    case CORBA::tk_struct:
      {
        // Skip the id, name, and member_count part.
        if (!stream.skip_string ()        // type ID, hidden
            || !stream.skip_string ()     // struct/except name
            || !stream.read_ulong (temp)) // member count
          {
            ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                              0);
          }

        // Computes the typecodes for all the members and
        // return the required one.
        for (CORBA::ULong i = 0; i < mcount; ++i)
          {
            if (!stream.read_string (
                     this->private_state_->tc_member_name_list_[i]
                   )
                || !this->skip_typecode (stream))
              {
                ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                                  0);
              }
          }

        this->private_state_->tc_member_name_list_known_ = 1;

        if (slot < mcount)
          {
            return this->private_state_->tc_member_name_list_[slot];
          }
        else
          {
            ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                              0);
          }

      }
    case CORBA::tk_union:            // index from 0
      {
        // skip the id, name, and discrimant type part
        if (!stream.skip_string ()             // type ID, hidden
            || !stream.skip_string ()          // typedef name
            || !this->skip_typecode (stream))  // skip discriminant typecode
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (), (char *)0);
        else if (!stream.read_ulong (temp)     // default used
                 || !stream.read_ulong (temp)) // member count
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (), 0);
        else
          {
            // get the typecode for the discriminator
            CORBA::TypeCode_ptr disc_tc =
              this->private_discriminator_type_i (
                ACE_ENV_SINGLE_ARG_PARAMETER);
            // Compute the name for all the members and return the
            // required one.
            ACE_CHECK_RETURN (0);

            for (CORBA::ULong i = 0; i < mcount; ++i)
              {
                // The ith entry will have the name of the ith member.
                CORBA::TypeCode::traverse_status status =
                  TAO_Marshal_Object::perform_skip (disc_tc,
                                                    &stream
                                                    ACE_ENV_ARG_PARAMETER);
                ACE_CHECK_RETURN (0);

                if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
                  ACE_THROW_RETURN (CORBA::BAD_TYPECODE (), 0);

                // skip typecode for member
                if (!stream.read_string (
                         this->private_state_->tc_member_name_list_ [i]
                       )
                    || (!this->skip_typecode (stream))) // skip typecode
                  ACE_THROW_RETURN ( CORBA::BAD_TYPECODE (), 0);
              }

            this->private_state_->tc_member_name_list_known_ = 1;

            if (slot < mcount)
              {
                return this->private_state_->tc_member_name_list_[slot];
              }
            else
              {
                ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                                  0);
              }
          }
      }
    case CORBA::tk_value:
    case CORBA::tk_event:
      {
        // Skip rest of header (type ID, name, etc) and skip the
        // number of value members.
        CORBA::ULong tc_kind_holder;
        CORBA::TCKind tc_kind;

        if (!stream.skip_string ()          // ID
            || !stream.skip_string ()       // Name
            || !stream.skip_ulong ()        // ValueModifier
            || !stream.read_ulong (tc_kind_holder)) // Base's TCKind
          {
            ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                              0);
          }

        tc_kind = ACE_static_cast (CORBA::TCKind, tc_kind_holder);

        // The tc_kind can be either tk_null or tk_value.
        // In the latter case we should skip encapsulation or
        // indirection - whatever comes in.
        CORBA::ULong encap_length;

        if (tc_kind != CORBA::tk_null)
          {
            if (!stream.read_ulong (encap_length))
              {
                ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                                  0);
              }

            if (tc_kind_holder != 0xffffffff) // check for indirection
              {
                if (!stream.skip_bytes (encap_length))
                  {
                    ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                                      0);
                  }
              }
          }

        // Now skip member count.
        if (!stream.skip_ulong ())
          {
            ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                              0);
          }

        // Compute the names for all the members, skip the other
        // member parameters and return the required name.
        for (CORBA::ULong i = 0; i < mcount; ++i)
          {
            if (!stream.read_string (
                     this->private_state_->tc_member_name_list_[i]
                   )
                || !this->skip_typecode (stream)    // member typecode
                || !stream.skip_ulong ())           // member visibility
              {
                ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                                  0);
              }
          }

        this->private_state_->tc_member_name_list_known_ = 1;

        if (slot < mcount)
          {
            return this->private_state_->tc_member_name_list_[slot];
          }
        else
          {
            ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                              0);
          }
      }
    default:
      // Bad kind.
      ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                        0);
    }
}

// Return member labels for CORBA::tk_union typecodes.
CORBA::Any_ptr
CORBA::TypeCode::private_member_label (CORBA::ULong n
                                       ACE_ENV_ARG_DECL) const
{
  // this function is only applicable to the CORBA::tk_union TC
  if (this->kind_ != CORBA::tk_union)
    {
      ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                        0);
    }

  TAO_InputCDR stream (this->buffer_+4,
                       this->length_-4,
                       this->byte_order_);

  // skip ID and name
  if (!stream.skip_string ()       // type ID, hidden
      || !stream.skip_string ()    // typedef name
      || !this->skip_typecode (stream))  // skip discriminant typecode
    {
      ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                        0);
    }

      // skip default used, and get member count
  CORBA::ULong member_count;

  if (!stream.read_ulong (member_count)      // default used
      || !stream.read_ulong (member_count))  // member count
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TypeCode::private_member_label -- ")
                  ACE_TEXT ("error reading from stream")));
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                        0);
    }

  // Double checked locking...
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->private_state_->mutex_,
                    0);

  if (this->private_state_->tc_member_label_list_known_)
    {
      if (n < member_count)
        {
          return this->private_state_->tc_member_label_list_[n];
        }
      else
        {
          ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                            0);
        }
    }

  // MJember labels are of type Any. However, the actual types are
  // restricted to simple types

  // @@EXC@@ We should use Auto_Ptr_Array to make this exception
  // safe.
  CORBA::Any_ptr* label_list;
  ACE_NEW_THROW_EX (label_list,
                    CORBA::Any_ptr [member_count],
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  // get the discriminant TC
  CORBA::TypeCode_ptr disc_tc =
    this->private_discriminator_type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  for (CORBA::ULong i = 0; i < member_count; ++i)
    {
      // Create an any from the portion of the CDR stream created
      // above.....

      // @@ This code assumes that the stream is a single message
      //    block, this is perfectly OK [the stream is created from a
      //    single buffer], but we may need to change this if the
      //    stream creation changes. [Carlos]
      TAO_InputCDR temp (stream);

      char *begin = stream.rd_ptr ();
      int retval =
        TAO_Marshal_Object::perform_skip (disc_tc,
                                          &temp
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          return 0;
        }

      char* end = temp.rd_ptr ();

      // We need to allocate more memory than in the original
      // stream, first to guarantee that the buffer is aligned in
      // memory and next because the realignment may introduce
      // extra padding. 2*MAX_ALIGNMENT should be enough.
      // @@EXC@@ This doesn't seem to be exception safe.
      TAO_OutputCDR out (end - begin + 2 * ACE_CDR::MAX_ALIGNMENT,
                         ACE_CDR_BYTE_ORDER,
                         ACE_Allocator::instance (),
                         ACE_Allocator::instance (),
                         ACE_Allocator::instance ());

      CORBA::TypeCode_ptr label_tc;

      const CORBA::ULong slot =
        ACE_static_cast (
          CORBA::ULong,
          this->private_default_index_i (ACE_ENV_SINGLE_ARG_PARAMETER));
      ACE_CHECK_RETURN (0);

      // If we are computing the label for the default index,
      // the label must contain an octet value of 0.
      if (i == slot)
        {
          label_tc = CORBA::_tc_octet;

          if (out.write_octet (ACE_static_cast (CORBA::Octet, 0)) == 0)
            {
              return 0;
            }

          retval =
            TAO_Marshal_Object::perform_skip (disc_tc,
                                              &stream
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
            {
              return 0;
            }
        }
      else
        {
          label_tc = disc_tc;

          retval =
            TAO_Marshal_Object::perform_append (label_tc,
                                                &stream,
                                                &out
                                                ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          if (retval != CORBA::TypeCode::TRAVERSE_CONTINUE)
            {
              return 0;
            }
        }

      TAO::Unknown_IDL_Type *impl = 0;
      ACE_NEW_THROW_EX (impl,
                        TAO::Unknown_IDL_Type (
                            CORBA::TypeCode::_duplicate (label_tc),
                            out.begin (),
                            ACE_CDR_BYTE_ORDER,
                            1
                          ),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      ACE_NEW_THROW_EX (label_list[i],
                        CORBA::Any,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      label_list[i]->replace (impl);

      if (stream.skip_string () == 0
          || this->skip_typecode (stream) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("TypeCode::private_member_label ")
                        ACE_TEXT ("error getting typecode for member %d\n"),
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
    {
      ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                        0);
    }

  return this->private_state_->tc_member_label_list_[n];
}

CORBA::TypeCode_ptr
CORBA::TypeCode::private_discriminator_type (
    ACE_ENV_SINGLE_ARG_DECL
  ) const
{
  // Double checked locking...
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->private_state_->mutex_,
                    0);

  return this->private_discriminator_type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::private_discriminator_type_i (
    ACE_ENV_SINGLE_ARG_DECL
  ) const
{
  if (this->private_state_->tc_discriminator_type_known_)
    {
      return this->private_state_->tc_discriminator_type_;
    }

  TAO_InputCDR stream (this->buffer_+4,
                       this->length_-4,
                       this->byte_order_);

  // skip ID and name, and then get the discriminant TC

  if (!stream.skip_string ()      // type ID, hidden
      || !stream.skip_string ())  // typedef name
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                        0);
    }

  CORBA::TypeCode::_tao_decode (this,
                               stream,
                               this->private_state_->tc_discriminator_type_
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  this->private_state_->tc_discriminator_type_known_ = 1;
  return this->private_state_->tc_discriminator_type_;
}

CORBA::Long
CORBA::TypeCode::private_default_index (
    ACE_ENV_SINGLE_ARG_DECL
  ) const
{
  // Double checked locking...
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->private_state_->mutex_,
                    0);

  return this->private_default_index_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Long
CORBA::TypeCode::private_default_index_i (
    ACE_ENV_SINGLE_ARG_DECL
  ) const
{
  if (this->private_state_->tc_default_index_used_known_)
    {
      return this->private_state_->tc_default_index_used_;
    }

  TAO_InputCDR stream (this->buffer_+4,
                       this->length_-4,
                       this->byte_order_);

  // skip ID and name, and then get the discriminant TC

  if (!stream.skip_string ()                  // type ID, hidden
      || !stream.skip_string ()               // typedef name
      || !this->skip_typecode (stream)        // skip discriminant
      || !stream.read_long (this->private_state_->tc_default_index_used_))
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                        0);
    }

  this->private_state_->tc_default_index_used_known_ = 1;
  return this->private_state_->tc_default_index_used_;
}

CORBA::Long
CORBA::TypeCode::private_length (ACE_ENV_SINGLE_ARG_DECL) const
{
  TAO_InputCDR stream (this->buffer_+4,
                       this->length_-4,
                       this->byte_order_);
  switch (this->kind_)
    {
    case CORBA::tk_sequence:
    case CORBA::tk_array:
      {
        // Double checked locking...
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                          guard,
                          this->private_state_->mutex_,
                          0);

        if (this->private_state_->tc_length_known_)
          {
            return this->private_state_->tc_length_;
          }

        // Skip the typecode of the element and get the bounds
        if (!this->skip_typecode (stream) // skip typecode
            || !stream.read_ulong (this->private_state_->tc_length_))
          {
            ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                              0);
          }
        else
          {
            this->private_state_->tc_length_known_ = 1;
            return this->private_state_->tc_length_;
          }
      }
    case CORBA::tk_string:
    case CORBA::tk_wstring:
      {
        // Double checked locking...
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                          guard,
                          this->private_state_->mutex_,
                          0);

        if (this->private_state_->tc_length_known_)
          {
            return this->private_state_->tc_length_;
          }

        if (stream.read_ulong (this->private_state_->tc_length_))
          {
            this->private_state_->tc_length_known_ = 1;
            return this->private_state_->tc_length_;
          }
        else
          {
            ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                              0);
          }
      }
    default:
      ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                        0);
    }
}

CORBA::TypeCode_ptr
CORBA::TypeCode::private_content_type (ACE_ENV_SINGLE_ARG_DECL) const
{
  TAO_InputCDR stream (this->buffer_+4,
                       this->length_-4,
                       this->byte_order_);
  switch (kind_)
    {
    case CORBA::tk_sequence:
    case CORBA::tk_array:
      {
        // Double checked locking...
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                          guard,
                          this->private_state_->mutex_,
                          0);

        if (this->private_state_->tc_content_type_known_)
          {
            return this->private_state_->tc_content_type_;
          }

        // retrieve the content type
        CORBA::TypeCode::_tao_decode (this,
                                     stream,
                                     this->private_state_->tc_content_type_
                                     ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        this->private_state_->tc_content_type_known_ = 1;
        return this->private_state_->tc_content_type_;
      }
    case CORBA::tk_alias:
    case CORBA::tk_value_box:
      {
        // Double checked locking...
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                          guard,
                          this->private_state_->mutex_,
                          0);

        if (this->private_state_->tc_content_type_known_)
          {
            return this->private_state_->tc_content_type_;
          }

        if (!stream.skip_string ()  // typeID
            || !stream.skip_string ()) // name
          {
            ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                              0);
          }

        CORBA::TypeCode::_tao_decode (this,
                                     stream,
                                     this->private_state_->tc_content_type_
                                     ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        this->private_state_->tc_content_type_known_ = 1;
        return this->private_state_->tc_content_type_;
      }
    default:
      ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                        0);
    }
}

CORBA::Visibility
CORBA::TypeCode::private_member_visibility (CORBA::ULong slot
                                           ACE_ENV_ARG_DECL) const
{
  if (this->kind_ != CORBA::tk_value
      && this->kind_ != CORBA::tk_event)
    {
      ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                        CORBA::PRIVATE_MEMBER);
    }

  CORBA::ULong mcount = this->member_count (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::PRIVATE_MEMBER);

  if (slot >= mcount)
    {
      ACE_THROW_RETURN (CORBA::TypeCode::Bounds (),
                        CORBA::PRIVATE_MEMBER);
    }

  // Double checked locking...
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->private_state_->mutex_,
                    0);

  if (this->private_state_->tc_member_visibility_list_known_)
    {
      return this->private_state_->tc_member_visibility_list_[slot];
    }

  // Set up an encapsulation.
  TAO_InputCDR stream (this->buffer_+4,
                       this->length_-4,
                       this->byte_order_);

  // Skip rest of header (type ID, name, etc) and collect the
  // number of value members.
  if (!stream.skip_string ()              // ID
      || !stream.skip_string ()           // Name
      || !stream.skip_ulong ()            // ValueModifier
      || !this->skip_typecode (stream)    // Concrete base typecode
      || !stream.skip_ulong ())           // Member count
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                        0);
    }

  ACE_NEW_THROW_EX (this->private_state_->tc_member_visibility_list_,
                    CORBA::Visibility[mcount],
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::PRIVATE_MEMBER);

  CORBA::Visibility tmp = CORBA::PRIVATE_MEMBER;

  for (CORBA::ULong i = 0; i < mcount; ++i)
    {
      if (!stream.skip_string ()              // Member name
          || !this->skip_typecode (stream)    // Member TypeCode
          || !stream.read_short (tmp))        // Read the member visibility
        {
          ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                            0);
        }

      this->private_state_->tc_member_visibility_list_[i] = tmp;
    }

  this->private_state_->tc_member_visibility_list_known_ = 1;
  return this->private_state_->tc_member_visibility_list_[slot];
}

CORBA::ValueModifier
CORBA::TypeCode::private_type_modifier (ACE_ENV_SINGLE_ARG_DECL) const
{
  if (this->kind_ != CORBA::tk_value
      && this->kind_ != CORBA::tk_event)
    {
      ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                        0);
    }

  // Double checked locking...
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->private_state_->mutex_,
                    0);

  if (this->private_state_->tc_type_modifier_known_)
    {
      return this->private_state_->tc_type_modifier_;
    }

  // Set up an encapsulation.
  TAO_InputCDR stream (this->buffer_+4,
                       this->length_-4,
                       this->byte_order_);

  // Skip rest of header (type ID, name, etc) and collect the
  // number of value members.
  if (!stream.skip_string ()          // ID
      || !stream.skip_string ())      // Name
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                        0);
    }

  if (!stream.read_short (this->private_state_->tc_type_modifier_))
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                        0);
    }

  this->private_state_->tc_type_modifier_known_ = 1;
  return this->private_state_->tc_type_modifier_;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::private_concrete_base_type (ACE_ENV_SINGLE_ARG_DECL) const
{
  if (this->kind_ != CORBA::tk_value
      && this->kind_ != CORBA::tk_event)
    {
      ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                        CORBA::TypeCode::_nil ());
    }

  // Double checked locking...
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->private_state_->mutex_,
                    CORBA::TypeCode::_nil ());

  if (this->private_state_->tc_concrete_base_type_known_)
    {
      return this->private_state_->tc_concrete_base_type_;
    }

  // Set up an encapsulation.
  TAO_InputCDR stream (this->buffer_+4,
                       this->length_-4,
                       this->byte_order_);

  // Skip rest of header (type ID, name, etc) and collect the
  // number of value members.
  if (!stream.skip_string ()          // ID
      || !stream.skip_string ()       // Name
      || !stream.skip_ulong ())       // ValueModifier
    {
      ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                        CORBA::TypeCode::_nil ());
    }

  // Retrieve the concrete base typecode.
  CORBA::TypeCode::_tao_decode (this,
                               stream,
                               this->private_state_->tc_concrete_base_type_
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  this->private_state_->tc_concrete_base_type_known_ = 1;
  return this->private_state_->tc_concrete_base_type_;
}

// ****************************************************************

void
CORBA::TypeCode::_tao_decode (const CORBA::TypeCode *parent,
                             TAO_InputCDR &cdr,
                             CORBA::TypeCode *&x
                             ACE_ENV_ARG_DECL)
{
  x = 0;
  CORBA::ULong kind;

  if (!cdr.read_ulong (kind))
    {
      ACE_THROW (CORBA::BAD_TYPECODE ());
    }

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

    0, // CORBA::_tc_Object ... type ID is CORBA::Object
    0, // CORBA::tk_struct
    0, // CORBA::tk_union
    0, // CORBA::tk_enum
    0, // CORBA::_tc_string ... unbounded
    0, // CORBA::tk_sequence
    0, // CORBA::tk_array
    0, // CORBA::tk_alias
    0, // CORBA::tk_except

    CORBA::_tc_longlong,
    CORBA::_tc_ulonglong,
    CORBA::_tc_longdouble,
    CORBA::_tc_wchar,
    0,  // CORBA::_tc_wstring ... unbounded
    0,  // CORBA::tk_fixed @@ boris: This is unsupported but I need next value
    0,  // CORBA::tk_value
  };

  if (kind < CORBA::TC_KIND_COUNT && tc_consts [kind] != 0)
    {
      // Easy case, the CDR contains no more data, and we can simply
      // duplicate one of the TypeCode constants...
      x = CORBA::TypeCode::_duplicate (tc_consts [kind]);
      return;
    }

  if (kind != ~0u && kind >= CORBA::TC_KIND_COUNT)
    {
      // Invalid kind.... cannot even determine what portion of the
      // CDR corresponds to the typecode....
      ACE_THROW (CORBA::BAD_TYPECODE ());
    }

  if (kind == ~0u)
    {
      // Get the long indicating the encapsulation offset,
      // then set up indirection stream that's like "stream"
      // but has space enough only for the typecode and the
      // length for the encapsulated parameters.
      //
      // The offset must be negative
      CORBA::Long offset;

      if (!cdr.read_long (offset) || offset >= 0)
        {
          // Since indirected typecodes cannot occur at the
          // topmost level, they can occur starting only at the
          // second and subsequent levels. This means that a
          // normal encoding of that typecode occurred somewhere
          // before in the stream. As a result the offset field
          // must always be negative. See the CORBA spec for details.
          ACE_THROW (CORBA::BAD_TYPECODE ());
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

      //                TAO_InputCDR indir_stream (*stream, 8, offset
      //                - 4);
      ACE_Message_Block *mb = (ACE_Message_Block *)cdr.start ();
      TAO_InputCDR indir_stream (mb->rd_ptr () + offset - 4,
                                 -1 * (offset - 4),
                                 cdr.byte_order ());

      if (!indir_stream.good_bit ())
        {
          ACE_THROW (CORBA::BAD_TYPECODE ());
        }

      // Get "kind" and length of target typecode
      //
      // XXX this currently assumes the TCKind to which we
      // indirect is the same byte order as the "parent"
      // typecode -- not the right assumption; see how the
      // TypeCode interpreter does it.

      CORBA::ULong indir_kind = 0;
      CORBA::ULong indir_len = 0;

      if (!indir_stream.read_ulong (indir_kind)
          || indir_kind >= CORBA::TC_KIND_COUNT // no double indirections
          || !indir_stream.read_ulong (indir_len))
        {
          ACE_THROW (CORBA::BAD_TYPECODE ());
        }

      // Now construct indirected typecode.  This shares the
      // typecode octets with the "parent" typecode,
      // increasing the amount of memory sharing and
      // reducing the cost of getting typecodes.
      ACE_NEW (x,
               CORBA::TypeCode ((CORBA::TCKind) indir_kind,
                                indir_len,
                                indir_stream.rd_ptr(),
                                0,
                                0,
                                // @@ TODO
                                // Here we lose the parent
                                // typecode...
                                ACE_const_cast(CORBA::TypeCode*,parent)));
      return;
    }

  // The other cases....
  switch (kind)
    {
    default:
      ACE_THROW (CORBA::INTERNAL ());

      // Some have "simple" parameter lists ... some of these
      // also have preallocated constants that could be used.
    case CORBA::tk_string:
    case CORBA::tk_wstring:
      {
        CORBA::ULong bound;

        if (!cdr.read_ulong (bound))
          {
            ACE_THROW (CORBA::BAD_TYPECODE ());
          }

        if (bound == 0)
          {
            // unbounded string. Let us reuse the ORB owned
            // _tc_string or _tc_wstring
            if (kind == CORBA::tk_string)
              {
                x = CORBA::TypeCode::_duplicate (CORBA::_tc_string);
              }
            else
              {
                x = CORBA::TypeCode::_duplicate (CORBA::_tc_wstring);
              }
          }
        else
          {
            // bounded string. Create a TypeCode. If it is does not
            // have a parent, then the application must free it.

            // allocate a new TypeCode

            // This may produce a memory leak, because
            // callers are sloppy about removing these
            // objects.
            CORBA::Long _oc_bounded_string [] =
            {TAO_ENCAP_BYTE_ORDER, 0};
            // Bounded string. Save the bounds
            _oc_bounded_string [1] = (CORBA::Long) bound;
            ACE_NEW (x,
                     CORBA::TypeCode (
                        ACE_static_cast(CORBA::TCKind, kind),
                        8,
                        ACE_reinterpret_cast(char*,_oc_bounded_string),
                        0,
                        sizeof (CORBA::String_var),
                        0
                      ));
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
    case CORBA::tk_value:
      {
        CORBA::ULong length;

        // get the encapsulation length
        if (!cdr.read_ulong (length))
          {
            ACE_THROW (CORBA::BAD_TYPECODE ());
          }

        // If the length is greater that the containing CDR stream
        // that is an error...
        if (length > cdr.length ())
          {
            ACE_THROW (CORBA::BAD_TYPECODE ());
          }

        // create a new typecode
        ACE_NEW (x,
                 CORBA::TypeCode ((CORBA::TCKind) kind,
                                  length,
                                  cdr.rd_ptr (),
                                  0,
                                  0,
                                  ACE_const_cast (CORBA::TypeCode*,parent)));
        // skip length number of bytes in the stream, otherwise we may
        // leave the stream in an undefined state
        (void) cdr.skip_bytes (length);
      }
      break;
    } // end of switch
}

// ****************************************************************

CORBA::TypeCode_ptr
CORBA::TypeCode::unalias (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::TCKind kind = this->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  CORBA::TypeCode_var tmp = CORBA::TypeCode::_duplicate (this);

  while (kind == CORBA::tk_alias)
    {
      tmp = tmp->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

      kind = tmp->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());
    }

  return tmp._retn ();
}

// ****************************************************************

void
CORBA::TypeCode::_tao_any_destructor (void *x)
{
  CORBA::TypeCode_ptr tmp = ACE_static_cast (CORBA::TypeCode_ptr, x);
  CORBA::release (tmp);
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
CORBA::TypeCode::param_count (ACE_ENV_SINGLE_ARG_DECL) const
{
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
        TAO_InputCDR stream (this->buffer_+4,
                             this->length_-4,
                             this->byte_order_);

        // skip rest of header (type ID and name) and collect the
        // number of struct members
        if (!stream.skip_string ()                  // ID
            || !stream.skip_string ()       // struct name
            || !stream.read_ulong (members))
          {
            ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                              0);
          }

        return 3 + 2 * members;
      }
    case CORBA::tk_enum:
      {
        CORBA::ULong members;
        TAO_InputCDR stream (this->buffer_+4,
                             this->length_-4,
                             this->byte_order_);

        // skip rest of header (type ID and name) and collect the
        // number of struct members
        if (!stream.skip_string ()                      // ID
            || !stream.skip_string ()           // typedef name
            || !stream.read_ulong (members))
          {
            ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                              0);
          }

        return 3 + members;
      }
    case CORBA::tk_union:
      {
        CORBA::ULong members;
        TAO_InputCDR stream (this->buffer_+4,
                             this->length_-4,
                             this->byte_order_);

        // skip rest of header (type ID, name, etc...) and collect the
        // number of struct members
        if (!stream.skip_string ()                    // ID
            || !stream.skip_string ()                 // struct name
            || !this->skip_typecode (stream)          // discriminant TC
            || !stream.read_ulong (members)           // default used
            || !stream.read_ulong (members))          // real member count
          {
            ACE_THROW_RETURN (CORBA::BAD_TYPECODE (),
                              0);
          }

        return 5 + 3 * members;
      }
    }
}

CORBA::Any_ptr
CORBA::TypeCode::parameter (const CORBA::Long /* slot */
                           ACE_ENV_ARG_DECL)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                    0);
}

// ****************************************************************

CORBA::Boolean
operator<< (TAO_OutputCDR& cdr, const CORBA::TypeCode *x)
{
  if (x == 0)
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                        CORBA::COMPLETED_MAYBE),
                        0);
    }

  CORBA::ULong kind =
    x->kind_;

  if (!cdr.write_ulong (kind))
    {
      return 0;
    }

  switch (kind)
    {
    default:
      break;

    // Indirected typecodes can occur at "top level" like
    // this only when constructing a recursive typecode in the
    // TypeCodeFactory. The check for non-null offset map suffices.
    case ~0u:
      if (x->offset_map () == 0)
        {
          return 0;
        }

      if (!cdr.write_octet_array ((CORBA::Octet*)x->buffer_,
                                  4))
        {
          return 0;
        }

      break;

      // A few have "simple" parameter lists
    case CORBA::tk_string:
    case CORBA::tk_wstring:
      {
        ACE_TRY_NEW_ENV
          {
            CORBA::ULong length =
              x->length (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            if (!cdr.write_ulong (length))
              {
                return 0;
              }
          }
        ACE_CATCHANY
          {
            return 0;
          }
        ACE_ENDTRY;
      }
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
    case CORBA::tk_value:
      {
        CORBA::ULong xlen = ACE_static_cast (CORBA::ULong, x->length_);
        if (!cdr.write_ulong (xlen)
            || !cdr.write_octet_array ((CORBA::Octet*)x->buffer_,
                                       xlen))
          {
            return 0;
          }
      }
      break;
    }

  return 1;
}

CORBA::Boolean
operator>> (TAO_InputCDR& cdr, CORBA::TypeCode *&x)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::TypeCode::_tao_decode (0,
                                    cdr,
                                    x
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  return 1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Pseudo_Object_Manager<CORBA::TypeCode,
                                         CORBA::TypeCode_var>;

template class ACE_Hash_Map_Entry<const char *,
                                  ACE_Unbounded_Queue<CORBA::Long> *>;
template class ACE_Hash_Map_Manager_Ex<const char *,
                                       ACE_Unbounded_Queue<CORBA::Long> *,
                                       ACE_Hash<const char *>,
                                       ACE_Equal_To<const char *>,
                                       ACE_Null_Mutex>;
template class 
  ACE_Hash_Map_Iterator_Base_Ex<const char *,
                                ACE_Unbounded_Queue<CORBA::Long> *,
                                ACE_Hash<const char *>,
                                ACE_Equal_To<const char *>,
                                ACE_Null_Mutex>;
template class 
  ACE_Hash_Map_Reverse_Iterator_Ex<const char *,
                                   ACE_Unbounded_Queue<CORBA::Long> *,
                                   ACE_Hash<const char *>,
                                   ACE_Equal_To<const char *>,
                                   ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<const char *,
                                        ACE_Unbounded_Queue<CORBA::Long> *,
                                        ACE_Hash<const char *>,
                                        ACE_Equal_To<const char *>,
                                        ACE_Null_Mutex>;

template class TAO_Pseudo_Var_T<CORBA::TypeCode>;
template class TAO_Pseudo_Out_T<CORBA::TypeCode, CORBA::TypeCode_var>;

template class TAO::Arg_Traits<CORBA::TypeCode>;
template class TAO::Object_Arg_Traits_T<CORBA::TypeCode_ptr,
                                        CORBA::TypeCode_var,
                                        CORBA::TypeCode_out>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Pseudo_Object_Manager<CORBA::TypeCode, \
                                              CORBA::TypeCode_var>

#pragma instantiate ACE_Hash_Map_Entry<const char *, \
                                       ACE_Unbounded_Queue<CORBA::Long> *>
#pragma instantiate \
  ACE_Hash_Map_Manager_Ex<const char *, \
                          ACE_Unbounded_Queue<CORBA::Long> *, \
                          ACE_Hash<const char *>, \
                          ACE_Equal_To<const char *>, \
                          ACE_Null_Mutex>
#pragma instantiate \
  ACE_Hash_Map_Iterator_Base_Ex<const char *, \
                                ACE_Unbounded_Queue<CORBA::Long> *, \
                                ACE_Hash<const char *>, \
                                ACE_Equal_To<const char *>, \
                                ACE_Nullv_Mutex>
#pragma instantiate \
  ACE_Hash_Map_Reverse_Iterator_Ex<const char *, \
                                   ACE_Unbounded_Queue<CORBA::Long> *, \
                                   ACE_Hash<const char *>, \
                                   ACE_Equal_To<const char *>, \
                                   ACE_Null_Mutex>
#pragma instantiate \
  ACE_Hash_Map_Iterator_Ex<const char *, \
                           ACE_Unbounded_Queue<CORBA::Long> *, \
                           ACE_Hash<const char *>, \
                           ACE_Equal_To<const char *>, \
                           ACE_Null_Mutex>

#pragma instantiate TAO_Pseudo_Var_T<CORBA::TypeCode>
#pragma instantiate TAO_Pseudo_Out_T<CORBA::TypeCode, CORBA::TypeCode_var>

#pragma instantiate TAO::Arg_Traits<CORBA::TypeCode>
#pragma instantiate TAO::Object_Arg_Traits_T<CORBA::TypeCode_ptr, \
                                             CORBA::TypeCode_var, \
                                             CORBA::TypeCode_out>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
