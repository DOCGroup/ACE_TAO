//
// $Id$
//

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
#if defined (HPUX) && (ACE_HAS_STANDARD_CPP_LIBRARY == 0)
  string symbol_;
  string full_name_;
#else
  std::string symbol_;
  std::string full_name_;
#endif /* HPUX */
  CORBA::Double price_;
};

#endif /* STOCK_H */
