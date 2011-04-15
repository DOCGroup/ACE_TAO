// $Id$

#include "MessengerC.h"
#include "Factory.h"
#include <iostream>

const char* server_ior = "file://server.ior";
using namespace demo::value::idl;
const char* IOR = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

int ACE_TMAIN (int ac, ACE_TCHAR* av[]) {

  try {

    CORBA::ORB_var orb = CORBA::ORB_init(ac, av);

    NodeFactory::register_new_factory(* orb.in());
    BoxedValueFactory::register_new_factory(* orb.in());
    BaseValueFactory::register_new_factory(* orb.in());
    TValueFactory::register_new_factory(* orb.in());
    ConfigValueFactory::register_new_factory(* orb.in());

    CORBA::Object_var obj = orb->string_to_object(server_ior);
    ValueServer_var tst = ValueServer::_narrow(obj.in());
    ACE_ASSERT(! CORBA::is_nil(tst.in()));

    // invoke operations and print the results
    boxedLong* p1 = new boxedLong (774);
    boxedLong* p2 = new boxedLong (775);
    boxedString* s1 = new boxedString ("hello");
    boxedString* s2 = new boxedString ("world");
    boxedString* null = 0;
    boxedValue* b = new OBV_demo::value::idl::boxedValue ();
    b->b1 (p1);
    b->b2 (p2);
    boxedValue* bb = new OBV_demo::value::idl::boxedValue ();
    bb->b1 (p1);
    bb->b2 (p1);

    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing two boxed values in one valuetype: %s\n",
      tst->receive_boxedvalue (b)));
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing one boxed values twice in one valuetype: %s\n",
      tst->receive_boxedvalue (bb)));
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing two integers: %s\n",
      tst->receive_long (p1, p2)));
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing one integer twice: %s\n",
      tst->receive_long (p1, p1)));
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing two strings: %s\n",
      tst->receive_string (s1, s2)));
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing null: %s\n",
      tst->receive_string (s1, null)));

    Node* n4 = new OBV_demo::value::idl::Node (4, 0);
    Node* n3 = new OBV_demo::value::idl::Node (3, n4);
    Node* n2 = new OBV_demo::value::idl::Node (2, n3);
    Node* n1 = new OBV_demo::value::idl::Node (1, n2);

    n4->next(n1);

    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing a list structure: %s\n",
      tst->receive_list (n1)));

#if 1
    TValue* t = new OBV_demo::value::idl::TValue ();
    t->data (20);
    t->basic_data (10);

    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing inout truncatable: %s\n",
      tst->receive_truncatable (t)));
    if (t->data () != 21 || t->basic_data () != 11)
    {
      std::cerr << "Received incorrect truncatable data" << std::endl;
      return 1;
    }
#endif

    const CORBA::ULong sz = 50;
    ::demo::value::idl::ConfigValues configs (sz);
    configs.length (sz);
    for (CORBA::ULong i = 0; i < sz; ++i)
    {
      configs[i] = new ConfigValueImpl ("IOR", IOR);
    }

    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing sequence: %s\n",
      tst->receive_sequence (configs)));

    while (orb->work_pending()) {
      orb->perform_work();
    }

    orb->destroy();

  } catch(const CORBA::Exception& e) {
    std::cerr << e << std::endl;
    return 1;
  }

  return 0;
}
