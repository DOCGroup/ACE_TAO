//
// $Id$
//

#ifndef STOCKFACTORY_H
#define STOCKFACTORY_H

#include "TestS.h"
#include "Stock.h"

class StockFactory
  : public POA_Test::StockFactory
{
public:
  StockFactory (CORBA::ORB_ptr orb, int number);

  Test::Stock_ptr get_stock (const char *symbol);

  virtual void shutdown (void);

private:
  CORBA::ORB_var orb_;
  Stock rhat_;
  Stock msft_;
  int number_;
};

#endif /* STOCKFACTORY_H */
