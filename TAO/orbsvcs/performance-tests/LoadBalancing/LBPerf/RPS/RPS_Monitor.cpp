// $Id$

#include "RPS_Monitor.h"
#include "ServerRequestInterceptor.h"
#include "ace/UUID.h"
#include "ace/OS_NS_sys_time.h"
#include "tao/ORB_Constants.h"

RPS_Monitor::RPS_Monitor (ServerRequestInterceptor * interceptor)
  : location_ (1),
    interceptor_ (interceptor),
    last_time_ (ACE_OS::gettimeofday ()),
    lock_ ()
{
  this->location_.length (1);

  ACE_Utils::UUID_GENERATOR::instance ()->init ();

  ACE_Utils::UUID uuid;
  ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (uuid);

  this->location_[0].id = CORBA::string_dup (uuid.to_string ()->c_str ());
  this->location_[0].kind = CORBA::string_dup ("UUID");
}

RPS_Monitor::~RPS_Monitor (void)
{
}

CosLoadBalancing::Location *
RPS_Monitor::the_location (void)
{
  CosLoadBalancing::Location * location;
  ACE_NEW_THROW_EX (location,
                    CosLoadBalancing::Location (this->location_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return location;
}

CosLoadBalancing::LoadList *
RPS_Monitor::loads (void)
{
  const ACE_Time_Value current_time = ACE_OS::gettimeofday ();

  ACE_Time_Value elapsed_time;

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, monitor, this->lock_, 0);

    elapsed_time = current_time - this->last_time_;
    this->last_time_ = current_time;
  }

  const CORBA::Float request_count =
    static_cast<CORBA::Float> (this->interceptor_->request_count ());

  CosLoadBalancing::LoadList * tmp;
  ACE_NEW_THROW_EX (tmp,
                    CosLoadBalancing::LoadList (1),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  CosLoadBalancing::LoadList_var load_list = tmp;

  load_list->length (1);

  load_list[0].id = CosLoadBalancing::RequestsPerSecond;

  if (elapsed_time == ACE_Time_Value::zero)
    load_list[0].value = 0;
  else
    load_list[0].value = request_count / elapsed_time.msec () * 1000;

  // Strictly for debugging or
  ACE_DEBUG ((LM_DEBUG, "%f\n", load_list[0].value));

  return load_list._retn ();
}
