// $Id$

#include "_pch.h"

#include "bankS.h"

#include "tao/AnyTypeCode/TypeCode.h"
#include "ace/OS_NS_stdio.h"
#include <iostream>
#include <fstream>
#include <fstream>
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("server.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
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
  void debit(CORBA::Long amt)
  {
    CORBA::Long tmp = balance();
    tmp -= amt;
    balance(tmp);
  }
};

class PersonFactory
  : public virtual Person_init
{
  Person* create(const char* name)
  {
    Person_i* p = new Person_i;
    p->name(name);
    return p;
  }

  CORBA::ValueBase * create_for_unmarshal()
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
  CORBA::Long update(Person* p)
  {
    ACE_DEBUG((LM_DEBUG,
      "Server: Subtracting $1.50 from person:%s starting balance:$%.2f\n",
      p->name(),
      p->balance() / 100.0));
    p->debit(150);
    return p->balance();
  }
};

void write_ior(const char* ior) {
  std::ofstream out(ACE_TEXT_ALWAYS_CHAR(ior_output_file));
  out << ior;
}

int ACE_TMAIN (int argc, ACE_TCHAR* argv[]) {
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      {
        CORBA::ValueFactoryBase_var factory = new PersonFactory;
        CORBA::String_var id = _tc_Person->id();
        orb->register_value_factory(id.in(), factory.in());
      }

      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

      PortableServer::POAManager_var poaman = poa->the_POAManager();

      PortableServer::Servant_var<Transaction_i> svt = new Transaction_i;

      {
        PortableServer::ObjectId_var id = poa->activate_object(svt.in());
        obj = poa->id_to_reference(id.in());
        CORBA::String_var ior = orb->object_to_string(obj.in());
        write_ior(ior.in());
      }

      std::cout << "Server: Running..." << std::endl;

      poaman->activate();

      orb->run();

      poa->destroy(true, true);
      orb->destroy();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
