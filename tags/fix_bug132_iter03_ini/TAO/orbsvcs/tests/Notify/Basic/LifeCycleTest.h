/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   LifeCycleTest.h
//
// = DESCRIPTION
//   Test to check it ec and admin objects are destroyed.
//
// = AUTHOR
//   Chanaka Liyanaarachchi <chanaka@ociweb.com>
//
// ==========================================================================

#ifndef NOTIFY_LIFECYCLETST_H
#define NOTIFY_LIFECYCLETST_H

class LifeCycleTest
{
public:
  LifeCycleTest (void);
  ~LifeCycleTest ();

  void init(int argc, char *argv[], CORBA::Environment &ACE_TRY_ENV);
  void run_test(CORBA::Environment &ACE_TRY_ENV);
  int parse_args(int argc, char *argv[]);

private:
   void create_ec(CORBA::Environment &ACE_TRY_ENV);
   void create_supplier_admin(CORBA::Environment &ACE_TRY_ENV);
   void create_consumer_admin(CORBA::Environment &ACE_TRY_ENV);
   void destroy_ec(CORBA::Environment &ACE_TRY_ENV);
   void destroy_supplier_admin(CORBA::Environment &ACE_TRY_ENV);
   void destroy_consumer_admin(CORBA::Environment &ACE_TRY_ENV);

   CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;
   CosNotifyChannelAdmin::EventChannel_var ec_;
   CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
   CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

  int count_;
  // number of times to create destroy.
};

#endif /* NOTIFY_LIFECYCLETST_H */
