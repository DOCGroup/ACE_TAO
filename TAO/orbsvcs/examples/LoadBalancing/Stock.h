
#ifndef STOCK_H
#define STOCK_H

#include "TestS.h"
#include <string>

class Stock
  : public POA_Test::Stock
{
public:
  Stock (const char *symbol,
         const char *full_name,
         CORBA::Double price);

  char *symbol ();
  char *full_name ();
  CORBA::Double price ();

private:
  std::string symbol_;
  std::string full_name_;
  CORBA::Double price_;
};

#endif /* STOCK_H */
