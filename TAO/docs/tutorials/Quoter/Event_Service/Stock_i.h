
#ifndef TAO_TUTORIAL_QUOTER_EVENT_SERVICE_STOCK_I_H
#define TAO_TUTORIAL_QUOTER_EVENT_SERVICE_STOCK_I_H

#include "QuoterS.h"
#include <orbsvcs/CosEventCommS.h>
#include <orbsvcs/CosEventChannelAdminC.h>

class Quoter_Stock_i
  : public virtual POA_Quoter::Modify_Stock
{
public:
  Quoter_Stock_i (char const *symbol,
                  char const *full_name,
                  CORBA::Double price);

  char *symbol () override;
  char *full_name () override;
  CORBA::Double price () override;
  void set_price (CORBA::Double new_price) override;

  void disconnect_push_supplier ();

  void connect (CosEventChannelAdmin::SupplierAdmin_ptr supplier_admin);

  Quoter_Stock_i (Quoter_Stock_i const &) = delete;
  Quoter_Stock_i (Quoter_Stock_i &&) = delete;
  Quoter_Stock_i &operator= (Quoter_Stock_i const &) = delete;
  Quoter_Stock_i &operator= (Quoter_Stock_i &&) = delete;

private:
  Quoter::Event data_;

  CosEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;

  POA_CosEventComm::PushSupplier_tie<Quoter_Stock_i> supplier_personality_;
};

#endif /* TAO_TUTORIAL_QUOTER_EVENT_SERVICE_STOCK_I_H */
