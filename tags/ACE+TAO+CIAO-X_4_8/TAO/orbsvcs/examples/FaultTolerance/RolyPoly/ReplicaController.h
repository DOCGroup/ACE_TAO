// file      : RolyPoly/ReplicaController.h
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef REPLICA_CONTROLLER_H
#define REPLICA_CONTROLLER_H

#include "ace/TMCast/GroupFwd.hpp"

#include "tao/LocalObject.h"
#include "tao/PortableInterceptorC.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/PortableServer/PortableServer.h"

#include "Log.h"

// State management
//
//

PortableInterceptor::SlotId
state_slot_id ();

void
state_slot_id (PortableInterceptor::SlotId slot_id);

// ReplicaController
//
//

class ReplicaController
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
public:
  virtual
  ~ReplicaController ();

  ReplicaController (CORBA::ORB_ptr orb);

public:
  virtual char *
  name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void
  destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
  virtual void
  tao_ft_interception_point (
    PortableInterceptor::ServerRequestInfo_ptr ri,
    CORBA::OctetSeq_out ocs
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));
#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

  virtual void
  receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void
  receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void
  send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void
  send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void
  send_other (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

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

  typedef
  Log<RecordId, CORBA::OctetSeq_var>
  Log_;

  Log_ log_;
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  auto_ptr<ACE_TMCast::Group> group_;
};

#endif  /* REPLICA_CONTROLLER_H */
