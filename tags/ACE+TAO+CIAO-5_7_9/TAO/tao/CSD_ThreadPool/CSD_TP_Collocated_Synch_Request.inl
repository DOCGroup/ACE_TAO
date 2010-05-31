// -*- C++ -*-
//
// $Id$

#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Collocated_Synch_Request::TP_Collocated_Synch_Request
                             (TAO_ServerRequest&              server_request,
                              const PortableServer::ObjectId& object_id,
                              PortableServer::POA_ptr         poa,
                              const char*                     operation,
                              PortableServer::Servant         servant,
                              TP_Servant_State*               servant_state)
  : TP_Corba_Request(object_id,
                     poa,
                     operation,
                     servant,
                     servant_state,
                     server_request),
    exception_(0)
{
}


ACE_INLINE
bool
TAO::CSD::TP_Collocated_Synch_Request::wait(void)
{
  bool dispatched = this->synch_helper_.wait_while_pending();

  if (dispatched)
    {
      // Check to see if the dispatching caused an exception to be raised.
      if (this->exception_ != 0)
        {
          // An exception was raised during the actual dispatching to
          // the servant.  We need to raise the exception to our caller,
          // which is the thread that made the collocated request in the
          // first place.
          CORBA::Exception* ex = this->exception_;
          this->exception_ = 0;

          ACE_Auto_Basic_Ptr<CORBA::Exception> ex_holder(ex);
          ex->_raise ();
        }
    }

  return dispatched;

}

TAO_END_VERSIONED_NAMESPACE_DECL
