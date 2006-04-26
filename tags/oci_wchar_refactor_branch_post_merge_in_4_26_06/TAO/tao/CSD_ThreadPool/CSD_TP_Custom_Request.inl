// -*- C++ -
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Custom_Request::TP_Custom_Request
                                 (TP_Custom_Request_Operation* op,
                                  TP_Servant_State*            servant_state)
  : TP_Request(op->servant(),servant_state),
    op_(op, false)
{
}


ACE_INLINE
void
TAO::CSD::TP_Custom_Request::execute_op()
{
  this->op_->execute();

  // Now drop the reference to the custom operation object.
  // This is necessary so that custom operation objects can be created
  // on the stack for synchronous custom requests.  If we do not do this,
  // then there is a race condition which could result in the stack-created
  // custom operation object having a reference count of 2 when it falls
  // out of scope (and destructs).  Our op_ data member would be the one
  // that held the other reference, and when our op_ data member destructs,
  // it attempts to perform a _remove_ref() on the underlying operation
  // object - which has already been destructed!  Thus, we reset the op_
  // data member here to the 'nil' state - causing the _remove_ref() to
  // be performed now.
  this->op_ = 0;
}


ACE_INLINE
void
TAO::CSD::TP_Custom_Request::cancel_op()
{
  this->op_->cancel();

  // See comments in the execute_op() method.
  this->op_ = 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
