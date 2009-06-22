// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Custom_Request_Operation::TP_Custom_Request_Operation
                                          (PortableServer::Servant servant)
: servant_ (servant)
{
  this->servant_->_add_ref ();
}


ACE_INLINE
void
TAO::CSD::TP_Custom_Request_Operation::execute (void)
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
