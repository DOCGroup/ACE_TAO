// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Custom_Request_Operation::TP_Custom_Request_Operation
                                          (PortableServer::Servant servant)
: servant_ (servant)
{
  // This try-catch block is not really necessary for current implementation
  // since the _add_ref does not throw exception, but we have to add it to
  // satisfy the non-exception builds. If _add_ref really throws an exception
  // then this constructor needs deal with the exception.
  try
  {
    this->servant_->_add_ref ();
  }
  catch (...)
  {
  }
}


ACE_INLINE
void
TAO::CSD::TP_Custom_Request_Operation::execute()
{
  this->execute_i();
}


ACE_INLINE
void
TAO::CSD::TP_Custom_Request_Operation::cancel()
{
  this->cancel_i();
}


ACE_INLINE
PortableServer::Servant
TAO::CSD::TP_Custom_Request_Operation::servant()
{
  return this->servant_.in();
}

TAO_END_VERSIONED_NAMESPACE_DECL
