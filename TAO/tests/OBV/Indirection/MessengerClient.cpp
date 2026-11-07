#include "MessengerC.h"
#include "Factory.h"
#include <iostream>

constexpr char server_ior[] = "file://server.ior";
using namespace demo::value::idl;
constexpr char IOR[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

int
ACE_TMAIN (int ac, ACE_TCHAR *av[])
{
  try {
    CORBA::ORB_var const orb = CORBA::ORB_init (ac, av);

    NodeFactory::register_new_factory (*orb.in ());
    BoxedValueFactory::register_new_factory (*orb.in ());
    BaseValueFactory::register_new_factory (*orb.in ());
    TValueFactory::register_new_factory (*orb.in ());
    ConfigValueFactory::register_new_factory (*orb.in ());

    CORBA::Object_var const obj = orb->string_to_object (server_ior);
    ValueServer_var const tst = ValueServer::_narrow (obj.in ());
    ACE_ASSERT (!CORBA::is_nil (tst.in ()));

    // Invoke operations and print the results.
    boxedLong_var const p1 (new boxedLong (774));
    boxedLong_var const p2 (new boxedLong (775));
    boxedString_var const s1 (new boxedString ("hello"));
    boxedString_var const s2 (new boxedString ("world"));
    boxedValue_var const b (new OBV_demo::value::idl::boxedValue);
    b->b1 (p1.in ());
    b->b2 (p2.in ());
    boxedValue_var const bb (new OBV_demo::value::idl::boxedValue);
    bb->b1 (p1.in ());
    bb->b2 (p1.in ());

    CORBA::String_var result (tst->receive_boxedvalue (b.in ()));
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t)Passing two boxed values in one valuetype: %s\n",
                result.in ()));
    result = tst->receive_boxedvalue (bb.in ());
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t)Passing one boxed value twice in one valuetype: %s\n",
                result.in ()));
    result = tst->receive_long (p1.in (), p2.in ());
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing two integers: %s\n", result.in ()));
    result = tst->receive_long (p1.in (), p1.in ());
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing one integer twice: %s\n", result.in ()));
    result = tst->receive_string (s1.in (), s2.in ());
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing two strings: %s\n", result.in ()));
    result = tst->receive_string (s1.in (), nullptr);
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing null: %s\n", result.in ()));

    Node_var const n4 (new OBV_demo::value::idl::Node (4, nullptr));
    Node_var const n3 (new OBV_demo::value::idl::Node (3, n4.in ()));
    Node_var const n2 (new OBV_demo::value::idl::Node (2, n3.in ()));
    Node_var const n1 (new OBV_demo::value::idl::Node (1, n2.in ()));

    n4->next (n1.in ());
    result = tst->receive_list (n1.in ());
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing a list structure: %s\n", result.in ()));
    // Break the cycle so the valuetype reference counts can reach zero.
    n4->next (nullptr);

    TValue_var t (new OBV_demo::value::idl::TValue);
    t->data (20);
    t->basic_data (10);

    result = tst->receive_truncatable (t.inout ());
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing inout truncatable: %s\n", result.in ()));
    if (t->data () != 21 || t->basic_data () != 11)
      {
        std::cerr << "Received incorrect truncatable data" << std::endl;
        return 1;
      }

    CORBA::ULong const sz = 50;
    ::demo::value::idl::ConfigValues configs (sz);
    configs.length (sz);
    for (CORBA::ULong i = 0; i < sz; ++i)
      {
        ConfigValue_var config (new ConfigValueImpl ("IOR", IOR));
        // The sequence element adopts the new reference.
        configs[i] = config._retn ();
      }

    result = tst->receive_sequence (configs);
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)Passing sequence: %s\n", result.in ()));

    while (orb->work_pending ())
      {
        orb->perform_work ();
      }

    orb->destroy ();
  }
  catch (CORBA::Exception const& e)
  {
    std::cerr << e << std::endl;
    return 1;
  }

  return 0;
}
