
#ifndef TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_I_H
#define TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_I_H

#include "QuoterS.h"
#include <string>

class Quoter_Stock_i : public POA_Quoter::Stock {
public:
  Quoter_Stock_i (char const *symbol,
                  char const *full_name,
                  CORBA::Double price);

  Quoter_Stock_i (Quoter_Stock_i const &) = delete;
  Quoter_Stock_i &operator= (Quoter_Stock_i const &) = delete;
  Quoter_Stock_i (Quoter_Stock_i &&) = delete;
  Quoter_Stock_i &operator= (Quoter_Stock_i &&) = delete;

  char *symbol () override;
  char *full_name () override;
  CORBA::Double price () override;
  Quoter::Stock::StockHistory *history () override;

  static void set_default_POA (PortableServer::POA_ptr poa)
  {
    Quoter_Stock_i::_poa = PortableServer::POA::_duplicate (poa);
  }
  PortableServer::POA_ptr _default_POA () override
  {
    return PortableServer::POA::_duplicate (Quoter_Stock_i::_poa.in ());
  }

private:
  std::string symbol_;
  std::string full_name_;
  CORBA::Double price_;

  static PortableServer::POA_var _poa;
};

#endif /* TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_I_H */


