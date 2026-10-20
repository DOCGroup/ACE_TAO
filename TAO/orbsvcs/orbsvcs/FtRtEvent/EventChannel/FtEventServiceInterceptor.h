// -*- C++ -*-

//=============================================================================
/**
 *  @file   FtEventServiceInterceptor.h
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
using CachedRequestInfo = FtRtecEventChannelAdmin::CachedResult;

class CachedRequestTable
{
public:
  CachedRequestTable() = default;

  CachedRequestTable(CachedRequestTable const&) = delete;
  CachedRequestTable(CachedRequestTable&&) = delete;
  CachedRequestTable& operator=(CachedRequestTable const&) = delete;
  CachedRequestTable& operator=(CachedRequestTable&&) = delete;

  bool is_new_request (ACE_CString const& client_id, CORBA::Long retention_id);
  int update (ACE_CString const& client_id,
              CORBA::Long retention_id,
              CORBA::Any const&);
  CORBA::Any get_result (ACE_CString const& client_id);
  void get_state (FtRtecEventChannelAdmin::CachedOptionResults& state);
  void set_state (FtRtecEventChannelAdmin::CachedOptionResults const& state);
private:
  using TableImpl = ACE_Hash_Map_Manager<ACE_CString, CachedRequestInfo, TAO_SYNCH_MUTEX>;
  TableImpl table_;
};


class FtEventServiceInterceptor :
  public PortableInterceptor::ServerRequestInterceptor
{
public:
  FtEventServiceInterceptor ();
  ~FtEventServiceInterceptor () override = default;

  static FtEventServiceInterceptor *instance ();
  char * name () override;

  void destroy () override;

  void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr) override;

  void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_other (PortableInterceptor::ServerRequestInfo_ptr) override;

  void get_state (FtRtecEventChannelAdmin::CachedOptionResults &state);
  void set_state (FtRtecEventChannelAdmin::CachedOptionResults const &state);
private:
  FtEventServiceInterceptor (FtEventServiceInterceptor const &) = delete;
  FtEventServiceInterceptor &operator= (FtEventServiceInterceptor const &) = delete;
  FtEventServiceInterceptor (FtEventServiceInterceptor &&) = delete;
  FtEventServiceInterceptor &operator= (FtEventServiceInterceptor &&) = delete;

  PortableInterceptor::Current_var pic(PortableInterceptor::ServerRequestInfo_ptr ri);

  CORBA::ORB_var orb_;
  CachedRequestTable request_table_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
