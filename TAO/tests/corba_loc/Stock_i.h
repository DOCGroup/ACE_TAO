//
// $Id$
//

#ifndef TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_I_H
#define TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_I_H

#include "QuoterS.h"
#include <string>

class Quoter_Stock_i : public POA_Quoter::Stock {
public:
  Quoter_Stock_i (const char *symbol,
                  const char *full_name,
                  CORBA::Double price,
                  CORBA::Environment & =
                  TAO_default_environment());

  char *symbol (CORBA::Environment & = TAO_default_environment()) throw (CORBA::SystemException);
  char *full_name (CORBA::Environment & = TAO_default_environment()) throw (CORBA::SystemException);
  CORBA::Double price (CORBA::Environment & = TAO_default_environment()) throw (CORBA::SystemException);

private:
  std::string symbol_;
  std::string full_name_;
  CORBA::Double price_;
};

#endif /* TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_I_H */
