//
// $Id$
//

#ifndef TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_FACTORY_I_H
#define TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_FACTORY_I_H

#include "QuoterS.h"

class Quoter_Stock_Factory_i : public POA_Quoter::Stock_Factory {
public:
  Quoter_Stock_Factory_i (PortableServer::POA_ptr stock_factory_poa);

  Quoter::Stock_ptr get_stock (const char *symbol)
    throw (Quoter::Invalid_Stock_Symbol);

private:
  PortableServer::POA_var stock_factory_poa_;
};

#endif /* TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_FACTORY_I_H */

