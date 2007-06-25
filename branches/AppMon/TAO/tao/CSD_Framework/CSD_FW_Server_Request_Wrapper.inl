// -*- C++ -*-
//
// $Id$

#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::FW_Server_Request_Wrapper::FW_Server_Request_Wrapper
                                          (TAO_ServerRequest& server_request)
  : is_clone_(false),
    request_(&server_request)
{
}


ACE_INLINE
void
TAO::CSD::FW_Server_Request_Wrapper::clone()
{
  // Only clone the TAO_Server_Request object if we have not performed the
  // clone already.  This really should only be called once, but this code
  // makes sure that we do not introduce a leak.
  if (!this->is_clone_)
    {
      this->request_ = this->clone (this->request_);
      this->is_clone_ = true;
    }
}


ACE_INLINE
void
TAO::CSD::FW_Server_Request_Wrapper::cancel()
{
  // We only need to handle remote requests that are expecting a reply.
  if (!this->request_->collocated()       &&
      !this->request_->sync_with_server() &&
      this->request_->response_expected() &&
      !this->request_->deferred_reply())
    {
          CORBA::NO_IMPLEMENT ex;
          this->request_->tao_send_reply_exception(ex);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
