// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Cancel_Visitor::TP_Cancel_Visitor()
{
}


ACE_INLINE
TAO::CSD::TP_Cancel_Visitor::TP_Cancel_Visitor(PortableServer::Servant servant)
  : servant_(servant)
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

TAO_END_VERSIONED_NAMESPACE_DECL
