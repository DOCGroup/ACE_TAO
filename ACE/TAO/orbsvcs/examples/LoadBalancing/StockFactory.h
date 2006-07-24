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

  Test::Stock_ptr get_stock (const char *symbol)
    throw (Test::Invalid_Stock_Symbol);

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  Stock rhat_;
  Stock msft_;
  int number_;
};

#endif /* STOCKFACTORY_H */
