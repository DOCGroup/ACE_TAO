#include "Stock_Consumer.h"

#include "ace/OS_NS_string.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Event_Utilities.h"
#include "tao/Exception.h"

#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb (CORBA::ORB_init (argc, argv));
      CORBA::Object_var poa_object (orb->resolve_initial_references ("RootPOA"));
      PortableServer::POA_var poa (PortableServer::POA::_narrow (poa_object.in ()));
      PortableServer::POAManager_var poa_manager (poa->the_POAManager ());
      poa_manager->activate ();

      CORBA::Object_var naming_context_object (
        orb->resolve_initial_references ("NameService"));
      CosNaming::NamingContext_var naming_context (
        CosNaming::NamingContext::_narrow (naming_context_object.in ()));

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = "EventService";

      CORBA::Object_var ec_object (naming_context->resolve (name));
      RtecEventChannelAdmin::EventChannel_var ec (
        RtecEventChannelAdmin::EventChannel::_narrow (ec_object.in ()));

      ACE_ConsumerQOS_Factory qos;
      qos.start_disjunction_group ();

      for (int i = 1; i < argc; ++i)
        {
          if (ACE_OS::strlen (argv[i]) < 4)
            continue;

          CORBA::ULong const type =
            static_cast<CORBA::ULong> (static_cast<unsigned char> (argv[i][0])) << 24 |
            static_cast<CORBA::ULong> (static_cast<unsigned char> (argv[i][1])) << 16 |
            static_cast<CORBA::ULong> (static_cast<unsigned char> (argv[i][2])) << 8 |
            static_cast<CORBA::ULong> (static_cast<unsigned char> (argv[i][3]));

          qos.insert_type (type, 0);
        }

      Stock_Consumer stock_consumer_i;
      stock_consumer_i.connect (ec.in (), qos.get_ConsumerQOS ());

      orb->run ();

      stock_consumer_i.disconnect ();
      poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "CORBA exception raised!" << ex << std::endl;
      return 1;
    }

  return 0;
}
