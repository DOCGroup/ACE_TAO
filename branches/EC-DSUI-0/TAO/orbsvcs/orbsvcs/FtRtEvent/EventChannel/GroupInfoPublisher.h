// -*- C++ -*-

//=============================================================================
/**
 *  @file   GroupInfoPublisher.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef GROUPINFOPUBLISHER_H
#define GROUPINFOPUBLISHER_H

#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "tao/PortableServer/PortableServerC.h"
#include "ace/Vector_T.h"
#include "ace/Singleton.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_FTEC_Become_Primary_Listener;

class GroupInfoPublisherBase
{
public:
  friend class ACE_Singleton<GroupInfoPublisherBase, ACE_Thread_Mutex>;

  void subscribe(TAO_FTEC_Become_Primary_Listener* listener);
  void set_naming_context(CosNaming::NamingContext_var naming_context);
  bool is_primary() const;
  CORBA::Object_var group_reference() const;

  FtRtecEventChannelAdmin::EventChannel_var successor() const;

  typedef FtRtecEventChannelAdmin::EventChannelList BackupList;
  const BackupList& backups() const;

  void update(const FTRT::ManagerInfoList & info_list,
              int my_position
              ACE_ENV_ARG_DECL);

  const PortableServer::ObjectId& object_id() const;
  const CosNaming::Name& name() const;

  void object_id(const char* oid);
  void name(const char* nam);

private:
  GroupInfoPublisherBase();

  bool primary_;
  CosNaming::NamingContext_var naming_context_;
  CORBA::Object_var iogr_;
  FtRtecEventChannelAdmin::EventChannel_var successor_;
  BackupList backups_;
  typedef ACE_Vector<TAO_FTEC_Become_Primary_Listener*, 2> Subscribers;
  Subscribers  subscribers_;
  PortableServer::ObjectId object_id_;
  CosNaming::Name name_;
};

typedef ACE_Singleton<GroupInfoPublisherBase, ACE_Thread_Mutex> GroupInfoPublisher;
#endif
