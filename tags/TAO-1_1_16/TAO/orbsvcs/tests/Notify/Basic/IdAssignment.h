/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   LifeCycleTest.h
//
// = DESCRIPTION
//   Test to check if ec, admin  are assigned id's correctly.
//
// = AUTHORS
//   Chanaka Liyanaarachchi <chanaka@ociweb.com> and Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef IDASSIGNMENT_H
#define IDASSIGNMENT_H

#include "orbsvcs/CosNotifyChannelAdminC.h"

class IdAssignment
{
public:
  IdAssignment (void);
  ~IdAssignment ();

  int parse_args(int argc, char *argv[]);
  void init(int argc, char *argv[], CORBA::Environment &ACE_TRY_ENV);
  void run_test(CORBA::Environment &ACE_TRY_ENV);

private:
  CosNotifyChannelAdmin::ChannelID create_ec(CORBA::Environment &ACE_TRY_ENV);
  void destroy_ec(CosNotifyChannelAdmin::ChannelID id,
                  CORBA::Environment &ACE_TRY_ENV);

  CosNotifyChannelAdmin::AdminID create_supplier_admin (CosNotifyChannelAdmin::ChannelID channel_id, CORBA::Environment &ACE_TRY_ENV);
  CosNotifyChannelAdmin::AdminID create_consumer_admin (CosNotifyChannelAdmin::ChannelID channel_id, CORBA::Environment &ACE_TRY_ENV);

  void destroy_consumer_admin (CosNotifyChannelAdmin::ChannelID channel_id, CosNotifyChannelAdmin::AdminID admin_id, CORBA::Environment &ACE_TRY_ENV);
  void destroy_supplier_admin (CosNotifyChannelAdmin::ChannelID channel_id, CosNotifyChannelAdmin::AdminID admin_id, CORBA::Environment &ACE_TRY_ENV);

  // = Data members
  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;

  int iter_;
  // Number of iterations for the test.

  int ec_count_;
  // Numbers of EC's to create.

  int consumer_admin_count_;
  // Number of consumer admins to create per ec.

  int supplier_admin_count_;
  // Number of consumer admins to create per ec.
};

#endif /* IDASSIGNMENT_H */
