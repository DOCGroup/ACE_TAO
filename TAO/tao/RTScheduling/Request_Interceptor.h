// -*- C++ -*-
//
#ifndef TAO_RTSCHEDULING_REQUEST_INTERCEPTOR_H
#define TAO_RTSCHEDULING_REQUEST_INTERCEPTOR_H

#include "tao/RTScheduling/rtscheduler_export.h"
#include "tao/RTScheduling/RTScheduler.h"
#include "ace/Atomic_Op.h"
#include "tao/RTScheduling/Current.h"
#include "tao/LocalObject.h"

#include "tao/PI_Server/PI_Server.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

extern std::atomic<long> server_guid_counter;

class TAO_RTScheduler_Export Client_Interceptor:
  public PortableInterceptor::ClientRequestInterceptor,
  public ::CORBA::LocalObject
{
public:
  //  Client_Interceptor (RTScheduling::Current_ptr current);
  void send_request (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  char* name () override;

  void destroy () override;

  static const IOP::ServiceId SchedulingInfo;
};

class TAO_RTScheduler_Export Server_Interceptor :
  public PortableInterceptor::ServerRequestInterceptor,
  public ::CORBA::LocalObject
{
public:
  Server_Interceptor (TAO_RTScheduler_Current_ptr current);

  void receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_other (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  char* name () override;

  void destroy () override;

  static const IOP::ServiceId SchedulingInfo;

 private:
  TAO_RTScheduler_Current_var current_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_RTSCHEDULING_REQUEST_INTERCEPTOR_H */
