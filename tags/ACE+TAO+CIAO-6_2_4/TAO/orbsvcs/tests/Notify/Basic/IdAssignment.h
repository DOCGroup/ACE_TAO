/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   IdAssignment.h
 *
 *  $Id$
 *
 * Test to check if ec, admin  are assigned id's correctly.
 *
 *
 *  @author Chanaka Liyanaarachchi <chanaka@ociweb.com> and Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef IDASSIGNMENT_H
#define IDASSIGNMENT_H

#include "orbsvcs/CosNotifyChannelAdminC.h"

class IdAssignment
{
public:
  IdAssignment (void);
  ~IdAssignment (void);

  int parse_args (int argc,
                  ACE_TCHAR *argv[]);
  void init (int argc,
             ACE_TCHAR *argv[]);
  void run_test (void);

private:
  CosNotifyChannelAdmin::ChannelID create_ec (void);
  void destroy_ec (CosNotifyChannelAdmin::ChannelID id);

  CosNotifyChannelAdmin::AdminID create_supplier_admin (
      CosNotifyChannelAdmin::ChannelID channel_id
    );
  CosNotifyChannelAdmin::AdminID create_consumer_admin (
      CosNotifyChannelAdmin::ChannelID channel_id
    );

  void destroy_consumer_admin (CosNotifyChannelAdmin::ChannelID channel_id,
                               CosNotifyChannelAdmin::AdminID admin_id);
  void destroy_supplier_admin (CosNotifyChannelAdmin::ChannelID channel_id,
                               CosNotifyChannelAdmin::AdminID admin_id);

  bool default_consumer_admin_test (CosNotifyChannelAdmin::ChannelID channel_id);
  bool default_supplier_admin_test (CosNotifyChannelAdmin::ChannelID channel_id);


  // = Data members.
  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;

  /// Number of iterations for the test.
  int iter_;

  /// Numbers of EC's to create.
  int ec_count_;

  /// Number of consumer admins to create per ec.
  int consumer_admin_count_;

  /// Number of consumer admins to create per ec.
  int supplier_admin_count_;
};

#endif /* IDASSIGNMENT_H */
