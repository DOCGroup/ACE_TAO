//
// $Id$
//

#ifndef TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_FACTORY_I_H
#define TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_FACTORY_I_H

#include "QuoterS.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Quoter_Stock_Factory_i : public POA_Quoter::Stock_Factory {
public:
  Quoter_Stock_Factory_i (PortableServer::POA_ptr stock_factory_poa);

  Quoter::Stock_ptr get_stock (const char *symbol)
    throw (Quoter::Invalid_Stock_Symbol);

private:
  PortableServer::POA_var stock_factory_poa_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_FACTORY_I_H */
