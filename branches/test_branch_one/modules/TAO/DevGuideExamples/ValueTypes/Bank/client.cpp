// $Id$

#include "_pch.h"

#include "bankC.h"

#include <iostream>
#include <stdexcept>


// Note : We use a completely different implementation
// of the Person valuetype for the client side, just to
// show that this is possible.
class Person_i
  : public virtual OBV_Person
  , public virtual CORBA::DefaultValueRefCountBase
{
public:
  Person_i(const char* n, CORBA::Long bal)
  {
    name(n);
    balance(bal);
  }
private:
  void debit(CORBA::Long)
  {
    throw std::runtime_error("Not supported.");
  }
};

const char* server_ior = "file://server.ior";


int ACE_TMAIN (int ac, ACE_TCHAR* av[]) {

  try {

    CORBA::ORB_var orb = CORBA::ORB_init(ac, av);

    CORBA::Object_var obj = orb->string_to_object(server_ior);
    Transaction_var trans = Transaction::_narrow(obj.in());
    if (CORBA::is_nil(trans.in()))
      throw std::runtime_error("failed to find a valid Transaction IOR");

    Person_var p = new Person_i("TAOUser", 1000);

    const char* n = p->name();
    double bal = p->balance() / 100.0;
    std::cout << "Client: Sending person:" << n
              << " starting_balance:$" << bal
              << std::endl;

    CORBA::Long b = trans->update(p.in());

    while (orb->work_pending()) {
      orb->perform_work();
    }

    std::cout << "Client: Ending balance: " << b/100.0 << std::endl;

    orb->destroy();

  } catch(const CORBA::Exception& e) {
    std::cerr << e << std::endl;
    return 1;
  }

  return 0;
}
