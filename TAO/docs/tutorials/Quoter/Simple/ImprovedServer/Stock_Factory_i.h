
#ifndef TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_FACTORY_I_H
#define TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_FACTORY_I_H

#include "QuoterS.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Quoter_Stock_Factory_i : public POA_Quoter::Stock_Factory {
public:
  explicit Quoter_Stock_Factory_i (PortableServer::POA_ptr stock_factory_poa);

  Quoter::Stock_ptr get_stock (char const *symbol) override;

  Quoter_Stock_Factory_i (Quoter_Stock_Factory_i const &) = delete;
  Quoter_Stock_Factory_i (Quoter_Stock_Factory_i &&) = delete;
  Quoter_Stock_Factory_i &operator= (Quoter_Stock_Factory_i const &) = delete;
  Quoter_Stock_Factory_i &operator= (Quoter_Stock_Factory_i &&) = delete;

private:
  PortableServer::POA_var stock_factory_poa_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_TUTORIAL_QUOTER_SIMPLE_IMPROVEDSERVER_STOCK_FACTORY_I_H */
