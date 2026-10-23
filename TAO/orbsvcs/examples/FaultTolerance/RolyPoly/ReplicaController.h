// file      : RolyPoly/ReplicaController.h
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#ifndef REPLICA_CONTROLLER_H
#define REPLICA_CONTROLLER_H

#include "ace/TMCast/GroupFwd.hpp"

#include "tao/LocalObject.h"
#include "tao/PortableInterceptorC.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/PortableServer/PortableServer.h"

#include "Log.h"
#include <memory>

// State management
PortableInterceptor::SlotId
state_slot_id ();

void
state_slot_id (PortableInterceptor::SlotId slot_id);

// ReplicaController
class ReplicaController
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
public:
  ~ReplicaController () override = default;

  ReplicaController (CORBA::ORB_ptr orb);

public:
  char *name () override;

  void destroy () override;

#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
  void
  tao_ft_interception_point (
    PortableInterceptor::ServerRequestInfo_ptr ri,
    CORBA::OctetSeq_out ocs) override;
#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

  void
  receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void
  receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void
  send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void
  send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void
  send_other (
    PortableInterceptor::ServerRequestInfo_ptr ri) override;

private:
  static ACE_THR_FUNC_RETURN
  listener_thunk (void* p);

  void
  listener ();

  PortableServer::POA_ptr
  resolve_poa (PortableInterceptor::AdapterName const& name);

private:
  class RecordId
  {
  public:
    RecordId (char const* client_id, CORBA::Long retention_id)
        : client_id_ (CORBA::string_dup (client_id)),
          retention_id_ (retention_id)
    {
    }

    friend bool
    operator< (RecordId const& a, RecordId const& b)
    {
      int r (ACE_OS::strcmp (a.client_id_.in (), b.client_id_.in ()));

      return (r < 0) || (r == 0 && a.retention_id_ < b.retention_id_);
    }

  private:
    CORBA::String_var client_id_;
    CORBA::Long retention_id_;
  };

  using Log_ = Log<RecordId, CORBA::OctetSeq_var>;

  Log_ log_;
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  std::unique_ptr<ACE_TMCast::Group> group_;
};

#endif  /* REPLICA_CONTROLLER_H */
