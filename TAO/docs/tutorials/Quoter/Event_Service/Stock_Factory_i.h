//
// $Id$
//

#ifndef TAO_TUTORIAL_QUOTER_EVENT_SERVICE_STOCK_FACTORY_I_H
#define TAO_TUTORIAL_QUOTER_EVENT_SERVICE_STOCK_FACTORY_I_H

#include "QuoterS.h"
#include <orbsvcs/CosEventChannelAdminS.h>

class Quoter_Stock_Factory_i : public POA_Quoter::Stock_Factory {
public:
  Quoter_Stock_Factory_i ();

  void load_stock_objects (PortableServer::POA_ptr poa,
                           PortableServer::POAManager_ptr poa_manager,
                           CosEventChannelAdmin::SupplierAdmin_ptr supplier_admin);
  void destroy_stock_objects ();

  Quoter::Stock_ptr get_stock (const char *symbol)
    throw (Quoter::Invalid_Stock_Symbol);

private:
  PortableServer::POA_var stock_factory_poa_;
};

#endif /* TAO_TUTORIAL_QUOTER_EVENT_SERVICE_STOCK_FACTORY_I_H */
