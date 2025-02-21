
#ifndef TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_I_H
#define TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_I_H

#include "QuoterS.h"
#include <string>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Quoter_Stock_i
  : public virtual POA_Quoter::Stock
{
public:
  Quoter_Stock_i (const char *symbol,
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

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_I_H */
