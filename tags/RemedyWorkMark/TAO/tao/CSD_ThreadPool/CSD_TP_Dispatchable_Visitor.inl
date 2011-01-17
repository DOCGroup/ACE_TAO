// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Dispatchable_Visitor::TP_Dispatchable_Visitor()
{
}


ACE_INLINE
void
TAO::CSD::TP_Dispatchable_Visitor::reset()
{
  // Set the handle to 0 to have it release any request it may currently
  // be referencing.
  this->request_ = 0;
}


ACE_INLINE
TAO::CSD::TP_Request*
TAO::CSD::TP_Dispatchable_Visitor::request()
{
  TP_Request_Handle handle(this->request_.in(), false);
  return handle._retn();
}

TAO_END_VERSIONED_NAMESPACE_DECL
