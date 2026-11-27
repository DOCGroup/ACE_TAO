#ifndef NOTIFY_PUSH_SUPPLIER_H
#define NOTIFY_PUSH_SUPPLIER_H

#include "Notify_StructuredPushSupplier.h"

class Notify_Push_Supplier : public TAO_Notify_Tests_StructuredPushSupplier
{
public:
  Notify_Push_Supplier () = default;
  Notify_Push_Supplier (Notify_Push_Supplier const&) = delete;
  Notify_Push_Supplier (Notify_Push_Supplier&&) = delete;
  Notify_Push_Supplier& operator= (Notify_Push_Supplier const&) = delete;
  Notify_Push_Supplier& operator= (Notify_Push_Supplier&&) = delete;

  void _connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin,
                 CosNotifyChannelAdmin::EventChannel_ptr notify_channel,
                 bool use_filter);
};

#endif /* NOTIFY_PUSH_SUPPLIER_H */
