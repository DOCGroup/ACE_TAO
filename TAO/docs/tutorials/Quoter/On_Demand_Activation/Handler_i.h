
#ifndef TAO_TUTORIALS_QUOTER_ONDEMAND_CLIENT_HANDLER_I_H
#define TAO_TUTORIALS_QUOTER_ONDEMAND_CLIENT_HANDLER_I_H

#include "QuoterS.h"

class Single_Query_Stock_Handler_i : public POA_Quoter::AMI_Single_Query_StockHandler
{
public:
  explicit Single_Query_Stock_Handler_i (int *response_count);

  void get_price_and_names (CORBA::Double ami_return_val,
                           char const *symbol,
                           char const *full_name) override;

  void get_price_and_names_excep (::Messaging::ExceptionHolder *) override;

  void get_symbol (char const *) override;
  void get_symbol_excep (::Messaging::ExceptionHolder *) override;

  void get_full_name (char const *) override;
  void get_full_name_excep (::Messaging::ExceptionHolder *) override;

  void price (double) override;
  void price_excep (::Messaging::ExceptionHolder *) override;

  Single_Query_Stock_Handler_i (Single_Query_Stock_Handler_i const &) = delete;
  Single_Query_Stock_Handler_i (Single_Query_Stock_Handler_i &&) = delete;
  Single_Query_Stock_Handler_i &operator= (Single_Query_Stock_Handler_i const &) = delete;
  Single_Query_Stock_Handler_i &operator= (Single_Query_Stock_Handler_i &&) = delete;

private:
  int *response_count_;
};

#endif /* TAO_TUTORIALS_QUOTER_AMI_CLIENT_HANDLER_I_H */
