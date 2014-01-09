// $Id$

#include "_pch.h"

#include "bankC.h"

#include <iostream>
#include <stdexcept>
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://server.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

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

  ::CORBA::ValueBase *_copy_value ()
  {
    ::CORBA::ValueBase *ret_val = 0;
    ACE_NEW_THROW_EX (
      ret_val,
      Person_i (
        name (),
        balance ()
      ),
      ::CORBA::NO_MEMORY ()
    );
    return ret_val;
  }

private:
  void debit(CORBA::Long)
  {
    throw std::runtime_error("Not supported.");
  }
};

int ACE_TMAIN (int argc, ACE_TCHAR* argv[]) {

  try {

    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;

    CORBA::Object_var obj = orb->string_to_object(ior);
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
  catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
