// -*- C++ -*-

#include "LB_RPMS_Monitor.h"
#include "LB_RPMS_Monitor_ORBInitializer.h"


ACE_RCSID (LoadBalancing,
           LB_RPMS_Monitor,
           "$Id$")

TAO_LB_RPMS_Monitor::TAO_LB_RPMS_Monitor (void)
  : interceptor_ (0)
{
}

void
TAO_LB_RPMS_Monitor::init (CORBA::Environment &ACE_TRY_ENV)
{
  // Possibly no CORBA exceptions instantiated yet so don't attempt
  // to throw one if allocation fails.

  TAO_LB_RPMS_Monitor_Interceptor *& interceptor =
    this->interceptor_.out ();

  ACE_NEW (interceptor,
           TAO_LB_RPMS_Monitor_Interceptor (this->object_group_.in ()));

  TAO_LB_RPMS_Monitor_ORBInitializer *initializer = 0;
  ACE_NEW (temp_initializer,
           TAO_LB_RPMS_Monitor_ORBInitializer (this->interceptor_.in ()));

  PortableInterceptor::ORBInitializer_var orb_initializer =
    temp_initializer;

  PortableInterceptor::register_orb_initializer (initializer.in (),
                                                 ACE_TRY_ENV);
  ACE_CHECK;
}

LoadBalancing::LoadList *
TAO_LB_RPMS_Monitor::current_load (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Construct the LoadList here instead of inside the interceptor to
  // maximize throughput in a multithreaded server replica by
  // preventing two allocations from being added to the critical path
  // of the client request.
  //
  // This optimization won't make a difference on single-threaded
  // server replicas.

  LoadBalancing::LoadList *tmp_loads = 0;
  ACE_NEW_THROW_EX (tmp_loads,
                    LoadBalancing::LoadList,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  LoadBalancing::LoadList_var loads = tmps_loads;

  loads->length (1);

  // The LoadId should be unique within the location this monitor
  // resides.
  loads[0].identifier = 0; // @todo Use a symbolic constant instead.

  loads[0].value = this->interceptor_->current_load ();

  return loads._retn ();
}

void
TAO_LB_RPMS_Monitor::register_redirect (const char *type_id,
                                        CORBA::Object_ptr redirect_to,
                                        CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->interceptor_.register_redirect (type_id,
                                        redirect_to,
                                        ACE_TRY_ENV);
}

void
TAO_LB_RPMS_Monitor::remove_redirect (const char *type_id,
                                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->interceptor_.remove_redirect (type_id,
                                      ACE_TRY_ENV);
}
