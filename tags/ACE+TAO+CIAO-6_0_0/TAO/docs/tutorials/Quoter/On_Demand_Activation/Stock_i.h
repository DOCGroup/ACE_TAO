//
// $Id$
//

#ifndef TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_I_H
#define TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_I_H

#include "QuoterS.h"
#include <string>

class Quoter_Stock_i : public POA_Quoter::Single_Query_Stock {
public:
  Quoter_Stock_i (const char *symbol,
                  const char *full_name,
                  CORBA::Double price);

  char *symbol ();
  char *full_name ();
  CORBA::Double price ();
  CORBA::Double get_price_and_names (CORBA::String_out symbol,
                                     CORBA::String_out full_name);

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

#endif /* TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_I_H */
