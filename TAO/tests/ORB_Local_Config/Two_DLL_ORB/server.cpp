#include "Test_i.h"
#include "ORB_DLL.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

#include <memory>

Server_Worker::Server_Worker ()
  : Abstract_Worker (ACE_TEXT ("test.ior"))
{
}

//
ACE_TCHAR const *
Server_Worker::kind () const
{
  return ACE_TEXT ("Server");
}

Server_Worker::~Server_Worker ()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %@ Server::<dtor>\n", this));
}

int
Server_Worker::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"), 0);

  int option {};
  while ((option = get_opts ()) != -1)
    if (option == 'o')
      this->ior_file_ = get_opts.opt_arg ();

  // Indicates successful parsing of the command line
  return 0;
}

int
Server_Worker::test_main (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Making sure there are no stale ior files to confuse a client
      ACE_OS::unlink (this->ior_file_.c_str ());

      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) Panic: nil RootPOA\n")),
                          1);

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (this->parse_args (argc, argv) != 0)
        return 1;

      Hello *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);

      PortableServer::ServantBase_var const owner_transfer (hello_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (hello_impl);

      CORBA::Object_var const hello_obj =
        root_poa->id_to_reference (id.in ());

      Test::Hello_var const hello =
        Test::Hello::_narrow (hello_obj.in ());

      CORBA::String_var const ior =
        orb->object_to_string (hello.in ());

      poa_manager->activate ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Server activated POA manager\n")));

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (this->ior_file_.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) Cannot open output file %s for writing IOR: %C"),
                             this->ior_file_.c_str (),
                             ior.in ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Server entering the event loop\n")));

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Server exiting the event loop\n")));

      root_poa->destroy (true, true);

      // During normal test execution the ORB would have been destroyed
      // by a request from the client.

      //  orb->shutdown (false);

      orb->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Server_Worker::test_main");
      return 1;
    }

  return 0;
}
