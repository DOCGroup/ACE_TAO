
#ifndef TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_I_H
#define TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_I_H

#include "QuoterS.h"
#include <string>

class Quoter_Stock_i : public POA_Quoter::Single_Query_Stock {
public:
  Quoter_Stock_i (char const *symbol,
                  char const *full_name,
                  CORBA::Double price);

  char *symbol () override;
  char *full_name () override;
  CORBA::Double price () override;
  CORBA::Double get_price_and_names (CORBA::String_out symbol,
                                     CORBA::String_out full_name) override;

  Quoter_Stock_i (Quoter_Stock_i const &) = delete;
  Quoter_Stock_i (Quoter_Stock_i &&) = delete;
  Quoter_Stock_i &operator= (Quoter_Stock_i const &) = delete;
  Quoter_Stock_i &operator= (Quoter_Stock_i &&) = delete;

private:
  std::string symbol_;
  std::string full_name_;
  CORBA::Double price_;
};

#endif /* TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_I_H */
