#include "StockFactory.h"

#include <cstring>
#include <iostream>

StockFactory::StockFactory (CORBA::ORB_ptr orb, int number)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     rhat_ ("RHAT", "RedHat, Inc.", 210),
     msft_ ("MSFT", "Microsoft, Inc.", 91),
     number_ (number)
{
}

Test::Stock_ptr
StockFactory::get_stock (char const *symbol)
{
  std::cout << "Server Number is " << number_ << std::endl;

  if (std::strcmp (symbol, "RHAT") == 0)
    return this->rhat_._this ();
  else if (std::strcmp (symbol, "MSFT") == 0)
    return this->msft_._this ();

  throw Test::Invalid_Stock_Symbol ();
}

void
StockFactory::shutdown ()
{
  this->orb_->shutdown (false);
}
