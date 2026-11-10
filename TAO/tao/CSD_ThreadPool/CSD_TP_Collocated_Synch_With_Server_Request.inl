// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Collocated_Synch_With_Server_Request::TP_Collocated_Synch_With_Server_Request
                             (TAO_ServerRequest&              server_request,
                              PortableServer::ObjectId const &object_id,
                              PortableServer::POA_ptr         poa,
                              char const *                    operation,
                              PortableServer::Servant         servant,
                              TP_Servant_State*               servant_state)
  : TP_Corba_Request(object_id,
                     poa,
                     operation,
                     servant,
                     servant_state,
                     server_request)
{
}


ACE_INLINE
bool
TAO::CSD::TP_Collocated_Synch_With_Server_Request::wait()
{
  return this->synch_helper_.wait_while_pending();
}

TAO_END_VERSIONED_NAMESPACE_DECL
