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
  ~LifeCycleTest (void);

  void init (int argc, 
             char *argv[] 
             TAO_ENV_ARG_DECL);
  void run_test (TAO_ENV_SINGLE_ARG_DECL);
  int parse_args (int argc, 
                  char *argv[]);

private:
   void create_ec (TAO_ENV_SINGLE_ARG_DECL);
   void create_supplier_admin (TAO_ENV_SINGLE_ARG_DECL);
   void create_consumer_admin (TAO_ENV_SINGLE_ARG_DECL);
   void destroy_ec (TAO_ENV_SINGLE_ARG_DECL);
   void destroy_supplier_admin (TAO_ENV_SINGLE_ARG_DECL);
   void destroy_consumer_admin (TAO_ENV_SINGLE_ARG_DECL);

   CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;
   CosNotifyChannelAdmin::EventChannel_var ec_;
   CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
   CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

  int count_;
  // Number of times to create destroy.
};

#endif /* NOTIFY_LIFECYCLETST_H */
