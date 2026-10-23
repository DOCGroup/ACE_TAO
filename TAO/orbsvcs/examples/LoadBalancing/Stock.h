
#ifndef STOCK_H
#define STOCK_H

#include "TestS.h"
#include <string>

class Stock
  : public POA_Test::Stock
{
public:
  Stock (char const *symbol,
         char const *full_name,
         CORBA::Double price);

  Stock (Stock const &) = delete;
  Stock (Stock &&) = delete;
  Stock &operator= (Stock const &) = delete;
  Stock &operator= (Stock &&) = delete;

  char *symbol () override;
  char *full_name () override;
  CORBA::Double price () override;

private:
  std::string symbol_;
  std::string full_name_;
  CORBA::Double price_;
};

#endif /* STOCK_H */
