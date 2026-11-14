#include "_pch.h"

#include "bankC.h"
#include "ace/Get_Opt.h"

#include <iostream>
#include <stdexcept>

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://server.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
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
  Person_i (char const *name, CORBA::Long balance)
  {
    this->name (name);
    this->balance (balance);
  }

  ::CORBA::ValueBase *_copy_value () override
  {
    ::CORBA::ValueBase *ret_val {};
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
  void debit (CORBA::Long) override
  {
    throw std::runtime_error ("Not supported.");
  }

public:
  Person_i (Person_i const &) = delete;
  Person_i (Person_i &&) = delete;
  Person_i &operator= (Person_i const &) = delete;
  Person_i &operator= (Person_i &&) = delete;
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  try {
    CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
    Client_Options options;

    if (parse_args (argc, argv, options) != 0)
      return 1;

    CORBA::Object_var const obj = orb->string_to_object (options.ior);
    Transaction_var const trans = Transaction::_narrow (obj.in ());
    if (CORBA::is_nil (trans.in ()))
      throw std::runtime_error ("failed to find a valid Transaction IOR");

    Person_var const p = new Person_i ("TAOUser", 1000);

    char const * const n = p->name ();
    double const bal = p->balance () / 100.0;
    std::cout << "Client: Sending person:" << n
              << " starting_balance:$" << bal
              << std::endl;

    CORBA::Long const b = trans->update (p.in ());

    while (orb->work_pending ()) {
      orb->perform_work ();
    }

    std::cout << "Client: Ending balance: " << b / 100.0 << std::endl;

    orb->destroy ();
  }
  catch (CORBA::Exception const& e)
  {
    std::cerr << e << std::endl;
    return 1;
  }
  catch (std::runtime_error const& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
