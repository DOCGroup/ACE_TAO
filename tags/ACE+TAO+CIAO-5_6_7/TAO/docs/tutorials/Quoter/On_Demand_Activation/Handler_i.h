//
// $Id$
//

#ifndef TAO_TUTORIALS_QUOTER_ONDEMAND_CLIENT_HANDLER_I_H
#define TAO_TUTORIALS_QUOTER_ONDEMAND_CLIENT_HANDLER_I_H

#include "QuoterS.h"

class Single_Query_Stock_Handler_i : public POA_Quoter::AMI_Single_Query_StockHandler
{
public:
  Single_Query_Stock_Handler_i (int *response_count);

  void get_price_and_names (CORBA::Double ami_return_val,
                           const char *symbol,
                           const char *full_name);

  void get_price_and_names_excep (::Messaging::ExceptionHolder *);

  void get_symbol(const char *);
  void get_symbol_excep (::Messaging::ExceptionHolder *);

  void get_full_name (const char *);
  void get_full_name_excep (::Messaging::ExceptionHolder *);

  void price (double);
  void price_excep (::Messaging::ExceptionHolder *);

private:

  int *response_count_;
};

#endif /* TAO_TUTORIALS_QUOTER_AMI_CLIENT_HANDLER_I_H */
