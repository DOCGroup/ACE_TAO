// $Id$

#include "Foo_Bar.h"
#include "ace/Time_Value.h"
#include "ace/Get_Opt.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Foo_Bar *foobar_impl;
      ACE_NEW_RETURN (foobar_impl,
                      Foo_Bar (orb.in ()),
                      1);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (foobar_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Foo_var foo =
        Test::Foo::_narrow (object.in ());

      poa_manager->activate ();

      // Dont unscope it or move it elsewhere.. It is here with a
      // purpose. If you dont understand this, please re-read the
      // README file.
      {
        PortableServer::ServantBase_var owner_transfer (foobar_impl);
      }

      ACE_Time_Value tv (10,
                         0);

      // Just run the ORB for a minute..
      orb->run (tv);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) server - shutting down the ORB\n"));

      orb->shutdown (1);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Finished shutting down the ORB\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Test successful..\n"));
  return 0;
}
