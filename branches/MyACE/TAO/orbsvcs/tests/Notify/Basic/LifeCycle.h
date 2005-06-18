/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = FILENAME
//   LifeCycle.h
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

class LifeCycle
{
public:
  LifeCycle (void);
  ~LifeCycle (void);

  void init (int argc,
             char *argv[]
             ACE_ENV_ARG_DECL);
  void run_test (ACE_ENV_SINGLE_ARG_DECL);
  int parse_args (int argc,
                  char *argv[]);

private:
   void create_ec (ACE_ENV_SINGLE_ARG_DECL);
   void create_supplier_admin (ACE_ENV_SINGLE_ARG_DECL);
   void create_consumer_admin (ACE_ENV_SINGLE_ARG_DECL);
   void destroy_ec (ACE_ENV_SINGLE_ARG_DECL);
   void destroy_supplier_admin (ACE_ENV_SINGLE_ARG_DECL);
   void destroy_consumer_admin (ACE_ENV_SINGLE_ARG_DECL);

   CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;
   CosNotifyChannelAdmin::EventChannel_var ec_;
   CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
   CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

  int count_;
  // Number of times to create destroy.
};

#endif /* NOTIFY_LIFECYCLETST_H */
