//
// $Id$
//

#ifndef TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_I_H
#define TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_I_H

#include "QuoterS.h"
#include <string>

class Quoter_Stock_i
  : public virtual POA_Quoter::Stock,
    public virtual PortableServer::RefCountServantBase
{
public:
  Quoter_Stock_i (const char *symbol,
                  const char *full_name,
                  CORBA::Double price);

  char *symbol () throw (CORBA::SystemException);
  char *full_name () throw (CORBA::SystemException);
  CORBA::Double price () throw (CORBA::SystemException);

private:
  std::string symbol_;
  std::string full_name_;
  CORBA::Double price_;
};

#endif /* TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_I_H */

