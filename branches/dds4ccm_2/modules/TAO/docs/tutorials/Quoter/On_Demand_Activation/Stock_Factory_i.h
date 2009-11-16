//
// $Id$
//
#ifndef TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_FACTORY_I_H
#define TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_FACTORY_I_H

#include "Stock_i.h"

class Quoter_Stock_Factory_i : public POA_Quoter::Stock_Factory {
public:
  Quoter_Stock_Factory_i ();

  Quoter::Stock_ptr get_stock (const char *symbol);
};

#endif /* TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_FACTORY_I_H */


