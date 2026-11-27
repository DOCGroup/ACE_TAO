#ifndef TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_FACTORY_I_H
#define TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_FACTORY_I_H

#include "Stock_i.h"

class Quoter_Stock_Factory_i : public POA_Quoter::Stock_Factory {
public:
  Quoter_Stock_Factory_i () = default;

  Quoter::Stock_ptr get_stock (char const *symbol) override;

  Quoter_Stock_Factory_i (Quoter_Stock_Factory_i const &) = delete;
  Quoter_Stock_Factory_i (Quoter_Stock_Factory_i &&) = delete;
  Quoter_Stock_Factory_i &operator= (Quoter_Stock_Factory_i const &) = delete;
  Quoter_Stock_Factory_i &operator= (Quoter_Stock_Factory_i &&) = delete;
};

#endif /* TAO_TUTORIAL_QUOTER_SIMPLE_SERVER_STOCK_FACTORY_I_H */

