//
// $Id$
//

#include "Handler_i.h"
#include <iostream>

Single_Query_Stock_Handler_i::
    Single_Query_Stock_Handler_i (int *response_count)
    : response_count_ (response_count)
{
}

void
Single_Query_Stock_Handler_i::get_price_and_names (CORBA::Double ami_return_val,
                                                  const char *symbol,
                                                  const char *full_name)
  throw (CORBA::SystemException)
{
  std::cout << "The price of one stock in \""
            << full_name << "\" (" << symbol << ") is "
            << ami_return_val << std::endl;
  (*this->response_count_)++;
}

void
Single_Query_Stock_Handler_i::
    get_price_and_names_excep (Quoter::AMI_Single_Query_StockExceptionHolder_ptr )
{
  // We ignore the exception, but this counts as a reponse, otherwise
  // the application would not finish.
  std::cerr << "Exception raised while querying stock price"
            << std::endl;
  (*this->response_count_)++;
}
