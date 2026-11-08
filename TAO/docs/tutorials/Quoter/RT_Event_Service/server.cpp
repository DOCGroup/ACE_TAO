#include "Stock_Factory_i.h"

#include "ace/OS_NS_unistd.h"
#include "orbsvcs/CosNamingC.h"

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

      Quoter_Stock_Factory_i stock_factory_i;
      Quoter::Stock_Factory_var stock_factory (stock_factory_i._this ());

      CORBA::Object_var naming_context_object (
        orb->resolve_initial_references ("NameService"));
      CosNaming::NamingContext_var naming_context (
        CosNaming::NamingContext::_narrow (naming_context_object.in ()));

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = "Stock_Factory";
      naming_context->rebind (name, stock_factory.in ());

      name[0].id = "EventService";
      CORBA::Object_var ec_object (naming_context->resolve (name));
      RtecEventChannelAdmin::EventChannel_var ec (
        RtecEventChannelAdmin::EventChannel::_narrow (ec_object.in ()));
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin (ec->for_suppliers ());

      stock_factory_i.load_stock_objects (poa.in (),
                                          poa_manager.in (),
                                          supplier_admin.in ());

      for (int j = 0; j < 1000; ++j)
        {
          for (int i = 1; i < argc; ++i)
            {
              try
                {
                  Quoter::Stock_var stock (
                    stock_factory->get_stock (ACE_TEXT_ALWAYS_CHAR (argv[i])));
                  CORBA::String_var const full_name (stock->full_name ());
                  CORBA::Double const price = stock->price ();
                  Quoter::Modify_Stock_var modify_stock (
                    Quoter::Modify_Stock::_narrow (stock.in ()));
                  modify_stock->set_price (price + 1);

                  std::cout << "Set the price of "
                            << full_name.in ()
                            << " to " << price + 1 << std::endl;
                }
              catch (Quoter::Invalid_Stock_Symbol const &)
                {
                  std::cerr << "Invalid stock symbol <"
                            << argv[i] << ">" << std::endl;
                }

              ACE_Time_Value const tv (0, 500000);
              ACE_OS::sleep (tv);
            }
        }

      stock_factory_i.destroy_stock_objects ();
      poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const &)
    {
      std::cerr << "CORBA exception raised!" << std::endl;
      return 1;
    }

  return 0;
}
