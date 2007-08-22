//
// $Id$
//

#ifndef TAO_TUTORIAL_QUOTER_RT_EVENT_SERVICE_STOCK_I_H
#define TAO_TUTORIAL_QUOTER_RT_EVENT_SERVICE_STOCK_I_H

#include "QuoterS.h"
#include <orbsvcs/RtecEventCommS.h>
#include <orbsvcs/RtecEventChannelAdminC.h>

class Quoter_Stock_i
  : public virtual POA_Quoter::Modify_Stock
{
public:
  Quoter_Stock_i (const char *symbol,
                  const char *full_name,
                  CORBA::Double price);
  ~Quoter_Stock_i (void);

  char *symbol ();
  char *full_name ();
  CORBA::Double price ();
  void set_price (CORBA::Double new_price);

  void disconnect_push_supplier (void);

  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin);

private:
  Quoter::Event data_;

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;

  POA_RtecEventComm::PushSupplier_tie<Quoter_Stock_i> &supplier_personality_;
};

#endif /* TAO_TUTORIAL_QUOTER_RT_EVENT_SERVICE_STOCK_I_H */
