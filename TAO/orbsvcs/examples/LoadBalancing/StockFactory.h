
#ifndef STOCKFACTORY_H
#define STOCKFACTORY_H

#include "TestS.h"
#include "Stock.h"

class StockFactory
  : public POA_Test::StockFactory
{
public:
  StockFactory (CORBA::ORB_ptr orb, int number);

  StockFactory (StockFactory const &) = delete;
  StockFactory (StockFactory &&) = delete;
  StockFactory &operator= (StockFactory const &) = delete;
  StockFactory &operator= (StockFactory &&) = delete;

  Test::Stock_ptr get_stock (char const *symbol) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
  Stock rhat_;
  Stock msft_;
  int number_;
};

#endif /* STOCKFACTORY_H */
