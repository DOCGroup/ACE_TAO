/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   LifeCycle.h
 *
 *  $Id$
 *
 * Test to check it ec and admin objects are destroyed.
 *
 *
 *  @author Chanaka Liyanaarachchi <chanaka@ociweb.com>
 */
//=============================================================================


#ifndef NOTIFY_LIFECYCLETST_H
#define NOTIFY_LIFECYCLETST_H

class LifeCycle
{
public:
  LifeCycle (void);
  ~LifeCycle (void);

  void init (int argc,
             ACE_TCHAR *argv[]);
  void run_test (void);
  int parse_args (int argc,
                  ACE_TCHAR *argv[]);

private:
   void create_ec (void);
   void create_supplier_admin (void);
   void create_consumer_admin (void);
   void destroy_ec (void);
   void destroy_supplier_admin (void);
   void destroy_consumer_admin (void);

   CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;
   CosNotifyChannelAdmin::EventChannel_var ec_;
   CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
   CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

  /// Number of times to create destroy.
  int count_;
};

#endif /* NOTIFY_LIFECYCLETST_H */
