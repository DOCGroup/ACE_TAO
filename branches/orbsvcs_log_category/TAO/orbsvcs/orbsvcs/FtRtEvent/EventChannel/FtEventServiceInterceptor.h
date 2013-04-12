// -*- C++ -*-

//=============================================================================
/**
 *  @file   FtEventServiceInterceptor.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef FTEVENTSERVICEINTERCEPTOR_H
#define FTEVENTSERVICEINTERCEPTOR_H
#include "ace/Hash_Map_Manager_T.h"
#include "ace/SString.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/ORB.h"
#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/PI/PI.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FTEC_Event_Channel_Impl;
typedef FtRtecEventChannelAdmin::CachedResult CachedRequestInfo;

class CachedRequestTable
{
public:
    bool is_new_request(const ACE_CString& client_id, CORBA::Long retention_id);
    int update(const ACE_CString& client_id,
         CORBA::Long retention_id,
         const CORBA::Any&);
    CORBA::Any get_result(const ACE_CString& client_id);
    void get_state(FtRtecEventChannelAdmin::CachedOptionResults& state);
    void set_state(const FtRtecEventChannelAdmin::CachedOptionResults& state);
private:
   typedef ACE_Hash_Map_Manager<ACE_CString, CachedRequestInfo, TAO_SYNCH_MUTEX> TableImpl;
   TableImpl table_;
};


class FtEventServiceInterceptor :
  public PortableInterceptor::ServerRequestInterceptor
{
public:
  FtEventServiceInterceptor();
  ~FtEventServiceInterceptor();

  static FtEventServiceInterceptor* instance();
  virtual char * name (void);

  virtual void destroy (void);

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr);

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr);

  void get_state(FtRtecEventChannelAdmin::CachedOptionResults& state);
  void set_state(const FtRtecEventChannelAdmin::CachedOptionResults& state);
private:
  PortableInterceptor::Current_var pic(PortableInterceptor::ServerRequestInfo_ptr ri);

  CORBA::ORB_var orb_;
  CachedRequestTable request_table_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
