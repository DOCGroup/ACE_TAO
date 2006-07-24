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
  ACE_TRY_NEW_ENV
  {
    this->servant_->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHALL
  {
  }
  ACE_ENDTRY;
}

TAO_END_VERSIONED_NAMESPACE_DECL
