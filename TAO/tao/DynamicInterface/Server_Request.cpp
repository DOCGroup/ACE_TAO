// $Id$

// Implementation of the Dynamic Server Skeleton Interface.

#include "Server_Request.h"
#include "tao/NVList.h"
#include "tao/GIOP_Utils.h"
#include "tao/Marshal.h"

#if !defined (__ACE_INLINE__)
# include "Server_Request.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(DynamicInterface, Server_Request, "$Id$")

// Reference counting for DSI ServerRequest object.

CORBA::ULong
CORBA_ServerRequest::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    ace_mon,
                    this->lock_,
                    0);

  return this->refcount_++;
}

CORBA::ULong
CORBA_ServerRequest::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                      ace_mon,
                      this->lock_,
                      0);

    this->refcount_--;

    if (this->refcount_ != 0)
      {
        return this->refcount_;
      }
  }

  delete this;
  return 0;
}

CORBA_ServerRequest::CORBA_ServerRequest (
    TAO_ServerRequest &orb_server_request
  )
  : lazy_evaluation_ (0),
    ctx_ (CORBA::Context::_nil ()),
    params_ (CORBA::NVList::_nil ()),
    retval_ (0),
    exception_ (0),
    refcount_ (1),
    orb_server_request_ (orb_server_request)
{
  this->orb_server_request_.is_dsi ();
}

CORBA_ServerRequest::~CORBA_ServerRequest (void)
{
  if (this->params_ != 0)
    {
      CORBA::release (this->params_);
    }

  delete this->retval_;
  delete this->exception_;
}

// Unmarshal in/inout params, and set up to marshal the appropriate
// inout/out/return values later on.
void
CORBA_ServerRequest::arguments (CORBA::NVList_ptr &list,
                                CORBA::Environment &ACE_TRY_ENV)
{
  // Save params for later use when marshaling the reply.
  this->params_ = list;

  this->params_->_tao_incoming_cdr (this->orb_server_request_.incoming (),
                                    CORBA::ARG_IN | CORBA::ARG_INOUT,
                                    this->lazy_evaluation_,
                                    ACE_TRY_ENV);

  // Pass this alignment back to the TAO_ServerRequest.
  this->orb_server_request_.dsi_nvlist_align (
                                this->params_->_tao_target_alignment ()
                              );
}

// Store the result value.  There's either an exception, or a result,
// but not both of them.  Results (and exceptions) can be reported
// only after the parameter list has been provided (maybe empty).
void
CORBA_ServerRequest::set_result (const CORBA::Any &value,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  // Setting a result when another result already exists or if an exception
  // exists is an error.
  if (this->retval_ != 0 || this->exception_ != 0)
    {
      ACE_THROW (CORBA::BAD_INV_ORDER ());
    }

  ACE_NEW_THROW_EX (this->retval_,
                    CORBA::Any (value),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;
}

// Store the exception value.
void
CORBA_ServerRequest::set_exception (const CORBA::Any &value,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  if (this->retval_ != 0 || this->exception_ != 0)
    {
      ACE_THROW (CORBA::BAD_INV_ORDER ());
    }

  ACE_NEW_THROW_EX (this->exception_,
                    CORBA::Any (value),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  this->orb_server_request_.exception_type (TAO_GIOP_USER_EXCEPTION);

  if (value.value ())
    {
      // @@ TODO - Change this to use <<=, now that we have it
      // for CORBA_Exception.
      CORBA_Exception* x = (CORBA_Exception*)value.value ();

      if (CORBA_SystemException::_downcast (x) != 0)
        {
          this->orb_server_request_.exception_type (
                                        TAO_GIOP_SYSTEM_EXCEPTION
                                      );
        }
    }
}

// This method will be utilized by the DSI servant to marshal outgoing
// parameters.
void
CORBA_ServerRequest::dsi_marshal (CORBA::Environment &ACE_TRY_ENV)
{
  // NOTE: if "env" is set, it takes precedence over exceptions
  // reported using the mechanism of the ServerRequest.  Only system
  // exceptions are reported that way ...
  //
  // XXX Exception reporting is ambiguous; it can be cleaner than
  // this.  With both language-mapped and dynamic/explicit reporting
  // mechanisms, one of must be tested "first" ... so an exception
  // reported using the other mechanism could be "lost".  Perhaps only
  // the language mapped one should be used for system exceptions.

  // If there wasn't any exception, we proceed.
  if (this->orb_server_request_.exception_type () == TAO_GIOP_NO_EXCEPTION 
      && CORBA::is_nil (this->orb_server_request_.forward_location ()))
    {
      // Send the return value, if any.
      if (this->retval_ != 0)
        {
          if (this->retval_->any_owns_data ())
            {
              this->retval_->_tao_encode (
                                 this->orb_server_request_.outgoing (),
                                 this->orb_server_request_.orb_core (),
                                 ACE_TRY_ENV
                               );
              ACE_CHECK;
            }
          else
            {
              CORBA::TypeCode_var tc = this->retval_->type ();

              TAO_InputCDR cdr (this->retval_->_tao_get_cdr (),
                                this->retval_->_tao_byte_order ());

              (void) TAO_Marshal_Object::perform_append (
                         tc.in (),
                         &cdr,
                         &this->orb_server_request_.outgoing (),
                         ACE_TRY_ENV
                       );
              ACE_CHECK;
            }
        }

      // Send the "inout" and "out" parameters.
      if (this->params_ != 0)
        {
          this->params_->_tao_encode (this->orb_server_request_.outgoing (),
                                      this->orb_server_request_.orb_core (),
                                      CORBA::ARG_INOUT | CORBA::ARG_OUT,
                                      ACE_TRY_ENV);
          ACE_CHECK;
        }
    }
}

