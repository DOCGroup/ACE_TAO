//
// $Id$
//

#include "StockFactory.h"
#include "ace/streams.h"

StockFactory::StockFactory (CORBA::ORB_ptr orb, int number)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     rhat_ ("RHAT", "RedHat, Inc.", 210),
     msft_ ("MSFT", "Microsoft, Inc.", 91),
     number_ (number)
{
}

Test::Stock_ptr
StockFactory::get_stock (const char *symbol)
    throw (Test::Invalid_Stock_Symbol)
{
  cout << "Server Number is " << number_ << endl;
  if (strcmp (symbol, "RHAT") == 0) {
    return this->rhat_._this ();
  } else if (strcmp (symbol, "MSFT") == 0) {
    return this->msft_._this ();
  }
  throw Test::Invalid_Stock_Symbol ();
}

void
StockFactory::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
