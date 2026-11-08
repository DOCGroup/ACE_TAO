
#ifndef TAO_TUTORIAL_QUOTER_RT_EVENT_SERVICE_STOCK_I_H
#define TAO_TUTORIAL_QUOTER_RT_EVENT_SERVICE_STOCK_I_H

#include "QuoterS.h"
#include <orbsvcs/RtecEventCommS.h>
#include <orbsvcs/RtecEventChannelAdminC.h>

#include <string>

class Quoter_Stock_i
  : public virtual POA_Quoter::Modify_Stock
{
public:
  Quoter_Stock_i (std::string const &symbol,
                  std::string const &full_name,
                  CORBA::Double price);
  ~Quoter_Stock_i () override;

  Quoter_Stock_i (Quoter_Stock_i const &) = delete;
  Quoter_Stock_i (Quoter_Stock_i &&) = delete;
  Quoter_Stock_i &operator= (Quoter_Stock_i const &) = delete;
  Quoter_Stock_i &operator= (Quoter_Stock_i &&) = delete;

  char *symbol () override;
  char *full_name () override;
  CORBA::Double price () override;
  void set_price (CORBA::Double new_price) override;

  void disconnect_push_supplier ();

  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin);

private:
  Quoter::Event data_ {};

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_ {};

  // The CORBA tie manages its own lifetime through _add_ref/_remove_ref.
  POA_RtecEventComm::PushSupplier_tie<Quoter_Stock_i> &supplier_personality_;
};

#endif /* TAO_TUTORIAL_QUOTER_RT_EVENT_SERVICE_STOCK_I_H */
