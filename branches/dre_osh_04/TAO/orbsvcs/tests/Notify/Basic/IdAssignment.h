/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   IdAssignment.h
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
  ~IdAssignment (void);

  int parse_args (int argc,
                  char *argv[]);
  void init (int argc,
             char *argv[]
             ACE_ENV_ARG_DECL);
  void run_test (ACE_ENV_SINGLE_ARG_DECL);

private:
  CosNotifyChannelAdmin::ChannelID create_ec (ACE_ENV_SINGLE_ARG_DECL);
  void destroy_ec (CosNotifyChannelAdmin::ChannelID id
                   ACE_ENV_ARG_DECL);

  CosNotifyChannelAdmin::AdminID create_supplier_admin (
      CosNotifyChannelAdmin::ChannelID channel_id
      ACE_ENV_ARG_DECL
    );
  CosNotifyChannelAdmin::AdminID create_consumer_admin (
      CosNotifyChannelAdmin::ChannelID channel_id
      ACE_ENV_ARG_DECL
    );

  void destroy_consumer_admin (CosNotifyChannelAdmin::ChannelID channel_id,
                               CosNotifyChannelAdmin::AdminID admin_id
                               ACE_ENV_ARG_DECL);
  void destroy_supplier_admin (CosNotifyChannelAdmin::ChannelID channel_id,
                               CosNotifyChannelAdmin::AdminID admin_id
                               ACE_ENV_ARG_DECL);

  // = Data members.
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
