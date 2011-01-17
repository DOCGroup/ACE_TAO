//
// $Id$
//

#include "Handler_i.h"
#include "ace/streams.h"

Single_Query_Stock_Handler_i::
    Single_Query_Stock_Handler_i (int *response_count)
    : response_count_ (response_count)
{
}

void
Single_Query_Stock_Handler_i::get_price_and_names (CORBA::Double ami_return_val,
                                                  const char *symbol,
                                                  const char *full_name)
{
  cout << "The price of one stock in \""
       << full_name << "\" (" << symbol << ") is "
       << ami_return_val << endl;
  (*this->response_count_)++;
}

void
Single_Query_Stock_Handler_i::
    get_price_and_names_excep (::Messaging::ExceptionHolder *)
{
  // We ignore the exception, but this counts as a reponse, otherwise
  // the application would not finish.
  cerr << "Exception raised while querying stock price"
       << endl;
  (*this->response_count_)++;
}


void
Single_Query_Stock_Handler_i::get_symbol (const char *)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
Single_Query_Stock_Handler_i::
    get_symbol_excep (::Messaging::ExceptionHolder *)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
Single_Query_Stock_Handler_i::get_full_name (const char *)
{
    throw CORBA::NO_IMPLEMENT ();
}

void
Single_Query_Stock_Handler_i::
   get_full_name_excep (::Messaging::ExceptionHolder *)
{
    throw CORBA::NO_IMPLEMENT ();
}

void
Single_Query_Stock_Handler_i::price (double)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
Single_Query_Stock_Handler_i::
    price_excep (::Messaging::ExceptionHolder *)
{
    throw CORBA::NO_IMPLEMENT ();
}
