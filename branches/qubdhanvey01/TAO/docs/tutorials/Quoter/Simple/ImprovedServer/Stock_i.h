//
// $Id$
//

#ifndef TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_I_H
#define TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_I_H

#include "QuoterS.h"
#include <string>

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

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
#if defined (HPUX) && (ACE_HAS_STANDARD_CPP_LIBRARY == 0)
  string symbol_;
  string full_name_;
#else
  std::string symbol_;
  std::string full_name_;
#endif /* HPUX */
  CORBA::Double price_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_I_H */
