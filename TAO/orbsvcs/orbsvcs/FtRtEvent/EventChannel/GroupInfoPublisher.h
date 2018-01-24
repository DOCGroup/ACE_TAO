// -*- C++ -*-

//=============================================================================
/**
 *  @file   GroupInfoPublisher.h
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef GROUPINFOPUBLISHER_H
#define GROUPINFOPUBLISHER_H
#include "ftrtec_export.h"
#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Vector_T.h"
#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FTEC_Become_Primary_Listener;

class GroupInfoPublisherBase
{
public:
  typedef FtRtecEventChannelAdmin::EventChannelList BackupList;

  struct Info {
    bool primary;
    CORBA::Object_var iogr;
    FtRtecEventChannelAdmin::EventChannel_var successor;
    BackupList backups;
  };

  typedef auto_ptr<Info> Info_ptr;
  friend class ACE_Singleton<GroupInfoPublisherBase, TAO_SYNCH_MUTEX>;

  void subscribe(TAO_FTEC_Become_Primary_Listener* listener);
  void set_naming_context(CosNaming::NamingContext_var naming_context);
  bool is_primary() const;
  CORBA::Object_var group_reference() const;

  FtRtecEventChannelAdmin::EventChannel_var successor() const;

  const BackupList& backups() const;

  Info* setup_info(const FTRT::ManagerInfoList & info_list,
              int my_position,
              CORBA::ULong object_group_ref_version);

  void update_info(Info_ptr& info);

  const PortableServer::ObjectId& object_id() const;
  const CosNaming::Name& name() const;

  void object_id(const char* oid);
  void name(const char* nam);

private:
  GroupInfoPublisherBase();

  CosNaming::NamingContext_var naming_context_;
  typedef ACE_Vector<TAO_FTEC_Become_Primary_Listener*, 2> Subscribers;
  Subscribers  subscribers_;
  PortableServer::ObjectId object_id_;
  CosNaming::Name name_;
  Info_ptr info_;
};

TAO_END_VERSIONED_NAMESPACE_DECL
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_FTRTEC_SINGLETON_DECLARE(ACE_Singleton, GroupInfoPublisherBase, TAO_SYNCH_MUTEX)
typedef ACE_Singleton<GroupInfoPublisherBase, TAO_SYNCH_MUTEX> GroupInfoPublisher;

ACE_END_VERSIONED_NAMESPACE_DECL

#endif
