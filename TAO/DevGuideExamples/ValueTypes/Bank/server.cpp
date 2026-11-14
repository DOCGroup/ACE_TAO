#include "_pch.h"

#include "bankS.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/AnyTypeCode/TypeCode.h"

#include <iostream>
#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {ACE_TEXT ("server.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

class Person_i
  : public virtual OBV_Person
  , public virtual CORBA::DefaultValueRefCountBase
{
public:
  Person_i () = default;

  ::CORBA::ValueBase *_copy_value () override
  {
    ::CORBA::ValueBase *ret_val {};
    ACE_NEW_THROW_EX (
      ret_val,
      Person_i (),
      ::CORBA::NO_MEMORY ()
    );
    return ret_val;
  }

  void debit (CORBA::Long amount) override
  {
    CORBA::Long current_balance = balance ();
    current_balance -= amount;
    balance (current_balance);
  }

  Person_i (Person_i const &) = delete;
  Person_i (Person_i &&) = delete;
  Person_i &operator= (Person_i const &) = delete;
  Person_i &operator= (Person_i &&) = delete;
};

class PersonFactory
  : public virtual Person_init
{
public:
  PersonFactory () = default;

  PersonFactory (PersonFactory const &) = delete;
  PersonFactory (PersonFactory &&) = delete;
  PersonFactory &operator= (PersonFactory const &) = delete;
  PersonFactory &operator= (PersonFactory &&) = delete;

private:
  Person *create (char const *name) override
  {
    Person_i * const p = new Person_i;
    p->name (name);
    return p;
  }

  CORBA::ValueBase *create_for_unmarshal () override
  {
    // It doesn't matter what values we construct it with
    // because they will be overwritten with the demarshaled values.
    return new Person_i;
  }
};

class Transaction_i
  : public virtual POA_Transaction
{
public:
  Transaction_i () = default;

  CORBA::Long update (Person *person) override
  {
    ACE_DEBUG ((LM_DEBUG,
      "Server: Subtracting $1.50 from person:%s starting balance:$%.2f\n",
      person->name (),
      person->balance () / 100.0));
    person->debit (150);
    return person->balance ();
  }

  Transaction_i (Transaction_i const &) = delete;
  Transaction_i (Transaction_i &&) = delete;
  Transaction_i &operator= (Transaction_i const &) = delete;
  Transaction_i &operator= (Transaction_i &&) = delete;
};

int write_ior (ACE_TCHAR const *filename, char const *ior)
{
  std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
    ACE_OS::fopen (filename, "w"), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                       filename),
                      -1);

  if (ACE_OS::fprintf (output_file.get (), "%s", ior) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot write IOR to output file: %s"),
                       filename),
                      -1);

  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      Server_Options options;

      if (parse_args (argc, argv, options) != 0)
        return 1;

      {
        CORBA::ValueFactoryBase_var const factory = new PersonFactory;
        CORBA::String_var const id = _tc_Person->id ();
        orb->register_value_factory (id.in (), factory.in ());
      }

      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var const poaman = poa->the_POAManager ();

      PortableServer::Servant_var<Transaction_i> const svt = new Transaction_i;

      {
        PortableServer::ObjectId_var const id = poa->activate_object (svt.in ());
        obj = poa->id_to_reference (id.in ());
        CORBA::String_var const ior = orb->object_to_string (obj.in ());
        if (write_ior (options.ior_output_file, ior.in ()) != 0)
          return 1;
      }

      std::cout << "Server: Running..." << std::endl;

      poaman->activate ();

      orb->run ();

      poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
