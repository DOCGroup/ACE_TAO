// $Id$
#ifndef NOTIFY_PUSH_SUPPLIER_H
#define NOTIFY_PUSH_SUPPLIER_H

#include "Notify_StructuredPushSupplier.h"


class Notify_Push_Supplier: public TAO_Notify_Tests_StructuredPushSupplier
{
public:
  void _connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin,
                CosNotifyChannelAdmin::EventChannel_ptr notify_channel,
                bool useFilter);
};

#endif /* NOTIFY_PUSH_SUPPLIER_H */
