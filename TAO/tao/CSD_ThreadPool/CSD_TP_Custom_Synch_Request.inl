// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Custom_Synch_Request::TP_Custom_Synch_Request
                              (TP_Custom_Request_Operation* op,
                               TP_Servant_State* servant_state)
  : TP_Custom_Request(op,servant_state)
{
}


ACE_INLINE
bool
TAO::CSD::TP_Custom_Synch_Request::wait()
{
  return this->synch_helper_.wait_while_pending();
}

TAO_END_VERSIONED_NAMESPACE_DECL
