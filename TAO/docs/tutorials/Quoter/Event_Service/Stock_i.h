//
// $Id$
//

#ifndef TAO_TUTORIAL_QUOTER_EVENT_SERVICE_STOCK_I_H
#define TAO_TUTORIAL_QUOTER_EVENT_SERVICE_STOCK_I_H

#include "QuoterS.h"
#include <orbsvcs/CosEventCommS.h>
#include <orbsvcs/CosEventChannelAdminC.h>

class Quoter_Stock_i
  : public virtual POA_Quoter::Modify_Stock
{
public:
  Quoter_Stock_i (const char *symbol,
                  const char *full_name,
                  CORBA::Double price);

  char *symbol ();
  char *full_name ();
  CORBA::Double price ();
  void set_price (CORBA::Double new_price);

  void disconnect_push_supplier (void);

  void connect (CosEventChannelAdmin::SupplierAdmin_ptr supplier_admin);

private:
  Quoter::Event data_;

  CosEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;

  POA_CosEventComm::PushSupplier_tie<Quoter_Stock_i> supplier_personality_;
};

#endif /* TAO_TUTORIAL_QUOTER_EVENT_SERVICE_STOCK_I_H */
