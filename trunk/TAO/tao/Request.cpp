// $Id$

#include "tao/Request.h"
#include "tao/Sequence_T.h"
#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "tao/Object.h"
#include "tao/Stub.h"

#if !defined (__ACE_INLINE__)
# include "tao/Request.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Request, "$Id$")

void
CORBA::release (CORBA::Request_ptr req)
{
  if (req)
    req->_decr_refcnt ();
}

CORBA::ULong
CORBA_Request::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_, 0);
  return refcount_++;
}

CORBA::ULong
CORBA_Request::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

CORBA_Request*
CORBA_Request::_nil (void)
{
  return 0;
}

// Reference counting for DII Request object

// DII Request class implementation

CORBA_Request::CORBA_Request (CORBA::Object_ptr obj,
                              const CORBA::Char *op,
                              CORBA::NVList_ptr args,
                              CORBA::NamedValue_ptr result,
                              CORBA::Flags flags,
                              CORBA::Environment &TAO_IN_ENV)
  : args_ (args),
    result_ (result),
    flags_ (flags),
    env_ (TAO_IN_ENV),
    contexts_ (0),
    ctx_ (0),
    refcount_ (1)
{
  target_ = CORBA::Object::_duplicate (obj);
  opname_ = CORBA::string_copy (op);
}

CORBA_Request::CORBA_Request (CORBA::Object_ptr obj,
                              const CORBA::Char *op,
                              CORBA::Environment &TAO_IN_ENV)
  : flags_ (0),
    env_ (TAO_IN_ENV),
    contexts_ (0),
    ctx_ (0),
    refcount_ (1)
{
  target_ = CORBA::Object::_duplicate (obj);
  opname_ = CORBA::string_copy (op);

  ACE_NEW (args_, CORBA::NVList);
  ACE_NEW (result_, CORBA::NamedValue);
}

CORBA_Request::~CORBA_Request (void)
{
  assert (refcount_ == 0);

  CORBA::release (this->target_);
  CORBA::string_free ((CORBA::String) this->opname_);
  this->opname_ = 0;
  CORBA::release (this->args_);
  CORBA::release (this->result_);
}

// The public DII interfaces:  normal and oneway calls.
//
// NOTE that using DII, programmers can get the special behaviour of
// discarding the response for normal calls.  This doesn't change the
// semantics of any OMG-IDL interface, it just streamlines control
// flow in some exotic situations.

void
CORBA_Request::invoke (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Stub *stub = this->target_->_stubobj ();

  stub->do_dynamic_call ((char *) opname_,
                         1,
                         args_,
                         result_,
                         flags_,
                         exceptions_,
                         ACE_TRY_ENV);
}

void
CORBA_Request::send_oneway (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Stub *stub = this->target_->_stubobj ();

  stub->do_dynamic_call ((char *) opname_,
                         0,
                         args_,
                         result_,
                         flags_,
                         exceptions_,
                         ACE_TRY_ENV);
}

void
CORBA_Request::send_deferred (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

void
CORBA_Request::get_response (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

CORBA::Boolean
CORBA_Request::poll_response (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                    0);
}


//  constructor.
CORBA_ORB_RequestSeq::CORBA_ORB_RequestSeq (CORBA::ULong max)
  : TAO_Unbounded_Pseudo_Sequence <CORBA_Request> (max)
{
  // no-op
}

CORBA_ORB_RequestSeq::CORBA_ORB_RequestSeq (const CORBA_ORB_RequestSeq &rhs)
  : TAO_Unbounded_Pseudo_Sequence <CORBA_Request> (rhs)
{
  // no-op
}

CORBA_ORB_RequestSeq::CORBA_ORB_RequestSeq (CORBA::ULong max,
                                            CORBA::ULong length,
                                            CORBA_Request **data,
                                            CORBA::Boolean release)
  : TAO_Unbounded_Pseudo_Sequence <CORBA_Request> (max,
                                                   length,
                                                   data,
                                                   release)
{
  // no-op
}


CORBA_ORB_RequestSeq::CORBA_ORB_RequestSeq (void)
{
  // no-op
}

/*
// Constructor using a maximum length value.
CORBA_ORB_RequestSeq::CORBA_ORB_RequestSeq (CORBA::ULong maximum)
  : TAO_Unbounded_Base_Sequence (maximum, allocbuf (maximum))
{
}

CORBA_ORB_RequestSeq::CORBA_ORB_RequestSeq (CORBA::ULong maximum,
                                            CORBA::ULong length,
                                            CORBA::Request_ptr *data,
                                            CORBA::Boolean release)
  : TAO_Unbounded_Base_Sequence (maximum, length, data, release)
{
}

CORBA_ORB_RequestSeq::CORBA_ORB_RequestSeq (const CORBA_ORB_RequestSeq &rhs)
  : TAO_Unbounded_Base_Sequence (rhs)
{
  CORBA::Request_ptr *tmp1 = allocbuf (this->maximum_);
  CORBA::Request_ptr * const tmp2 =
    ACE_reinterpret_cast (CORBA::Request_ptr * ACE_CAST_CONST,
                          rhs.buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp1[i] = tmp2[i];

  this->buffer_ = tmp1;
}

CORBA_ORB_RequestSeq &
CORBA_ORB_RequestSeq::operator= (const CORBA_ORB_RequestSeq &rhs)
{
  if (this == &rhs)
    return *this;

  if (this->release_)
    {
      if (this->maximum_ < rhs.maximum_)
        {
          // free the old buffer
          CORBA::Request_ptr *tmp =
            ACE_reinterpret_cast (CORBA::Request_ptr *,
                                  this->buffer_);
          freebuf (tmp);
          this->buffer_ = allocbuf (rhs.maximum_);
        }
    }
  else
    this->buffer_ = allocbuf (rhs.maximum_);

  TAO_Unbounded_Base_Sequence::operator= (rhs);

  CORBA::Request_ptr *tmp1 =
    ACE_reinterpret_cast (CORBA::Request_ptr *,
                          this->buffer_);
  CORBA::Request_ptr * const tmp2 =
    ACE_reinterpret_cast (CORBA::Request_ptr * ACE_CAST_CONST,
                          rhs.buffer_);

  for (CORBA::ULong i = 0; i < this->length_; ++i)
    tmp1[i] = tmp2[i];

  return *this;
}

CORBA_ORB_RequestSeq::~CORBA_ORB_RequestSeq (void)
{
  this->_deallocate_buffer ();
}

CORBA::Request_ptr
CORBA_ORB_RequestSeq::operator[] (CORBA::ULong i)
{
  if (i >= this->maximum_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) CORBA_ORB_RequestSeq %p\n",
                       "operator[] - subscript out of range"),
                      0);

  CORBA::Request_ptr *tmp =
    ACE_reinterpret_cast (CORBA::Request_ptr *,
                          this->buffer_);
  return tmp[i];
}

const CORBA::Request*
CORBA_ORB_RequestSeq::operator[] (CORBA::ULong i) const
{
  if (i >= this->maximum_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) CORBA_ORB_RequestSeq %p\n",
                       "operator[] - subscript out of range"),
                      0);

  CORBA::Request_ptr * const tmp =
    ACE_reinterpret_cast (CORBA::Request_ptr * ACE_CAST_CONST,
                          this->buffer_);

  return tmp[i];
}

CORBA::Request_ptr *
CORBA_ORB_RequestSeq::allocbuf (CORBA::ULong size)
{
  return new CORBA::Request_ptr[size];
}

void
CORBA_ORB_RequestSeq::freebuf (CORBA::Request_ptr *buffer)
{
  delete [] buffer;
}

void
CORBA_ORB_RequestSeq::_allocate_buffer (CORBA::ULong length)
{
  CORBA::Request_ptr * tmp = allocbuf (length);

  if (this->buffer_ != 0)
    {
      CORBA::Request_ptr *old =
        ACE_reinterpret_cast (CORBA::Request_ptr *,
                              this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        tmp[i] = old[i];

      if (this->release_)
        freebuf (old);
    }

  this->buffer_ = tmp;
}

void
CORBA_ORB_RequestSeq::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;

  CORBA::Request_ptr *tmp =
    ACE_reinterpret_cast (CORBA::Request_ptr *,
                          this->buffer_);

  freebuf (tmp);

  this->buffer_ = 0;
}

CORBA::Request_ptr *
CORBA_ORB_RequestSeq::get_buffer (CORBA::Boolean orphan)
{
  CORBA::Request_ptr *result = 0;

  if (orphan == 0)
    {
      // We retain ownership.
      if (this->buffer_ == 0)
        {
          result = allocbuf (this->length_);
          this->buffer_ = result;
        }
      else
        {
          result = ACE_reinterpret_cast (CORBA::Request_ptr *,
                                         this->buffer_);
        }
    }
  else // if (orphan == 1)
    {
      if (this->release_ != 0)
        {
          // We set the state back to default and relinquish
          // ownership.
          result = ACE_reinterpret_cast(CORBA::Request_ptr *,
                                        this->buffer_);
          this->maximum_ = 0;
          this->length_ = 0;
          this->buffer_ = 0;
          this->release_ = 0;
        }
    }
  return result;
}

const CORBA::Request_ptr *
CORBA_ORB_RequestSeq::get_buffer (void) const
{
  return ACE_reinterpret_cast (const CORBA::Request_ptr * ACE_CAST_CONST,
                               this->buffer_);
}

void
CORBA_ORB_RequestSeq::replace (CORBA::ULong max,
                               CORBA::ULong length,
                               CORBA::Request_ptr *data,
                               CORBA::Boolean release)
{
  this->maximum_ = max;
  this->length_ = length;

  if (this->buffer_ && this->release_ == 1)
    {
      CORBA::Request_ptr *tmp =
        ACE_reinterpret_cast(CORBA::Request_ptr *,
                             this->buffer_);
      freebuf (tmp);
    }

  this->buffer_ = data;
  this->release_ = release;
}
*/
#endif /* TAO_HAS_MINIMUM_CORBA */


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Unbounded_Pseudo_Sequence<CORBA_Request>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Unbounded_Pseudo_Sequence<CORBA_Request>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
