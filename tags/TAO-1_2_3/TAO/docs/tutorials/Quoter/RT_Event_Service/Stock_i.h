//
// $Id$
//

#ifndef TAO_TUTORIAL_QUOTER_RT_EVENT_SERVICE_STOCK_I_H
#define TAO_TUTORIAL_QUOTER_RT_EVENT_SERVICE_STOCK_I_H

#include "QuoterS.h"
#include <orbsvcs/RtecEventCommS.h>
#include <orbsvcs/RtecEventChannelAdminC.h>

class Quoter_Stock_i
  : public virtual POA_Quoter::Modify_Stock,
    public virtual PortableServer::RefCountServantBase
{
public:
  Quoter_Stock_i (const char *symbol,
                  const char *full_name,
                  CORBA::Double price);
  ~Quoter_Stock_i (void);

  char *symbol () throw (CORBA::SystemException);
  char *full_name () throw (CORBA::SystemException);
  CORBA::Double price () throw (CORBA::SystemException);
  void set_price (CORBA::Double new_price) throw (CORBA::SystemException);

  void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    throw (CORBA::SystemException);

  void connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin);

private:
  Quoter::Event data_;

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;

  POA_RtecEventComm::PushSupplier_tie<Quoter_Stock_i> &supplier_personality_;
};

#endif /* TAO_TUTORIAL_QUOTER_RT_EVENT_SERVICE_STOCK_I_H */
