// $Id$

#include "Test_i.h"
#include "ORB_DLL.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Argv_Type_Converter.h"

Server_Worker::Server_Worker ()
  : Abstract_Worker (ACE_TEXT ("test.ior"))
{
}

//
const ACE_TCHAR *
Server_Worker::kind (void) const
{
  return ACE_TEXT ("Server");
}

//
Server_Worker::~Server_Worker (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %@ Server::<dtor>\n", this));
}

//
int
Server_Worker::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"),0);

  for( int c = 0; ((c = get_opts ()) != -1); )
    switch (c)
    {
    case 'o':
      this->ior_file_ = get_opts.opt_arg ();
      break;
    }

  // Indicates successful parsing of the command line
  return 0;
}

//
int
Server_Worker::test_main (int argc, ACE_TCHAR *argv[])
{
  // Making sure there are no stale ior files to confuse a client
  ACE_OS::unlink (ior_file_.c_str ());

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  CORBA::Object_var poa_object =
    orb->resolve_initial_references ("RootPOA");

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in ());

  if (CORBA::is_nil (root_poa.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Panic: nil RootPOA\n")),
                      1);

  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager ();

  if (parse_args (argc, argv) != 0)
    return 1;

  Hello *hello_impl;
  ACE_NEW_RETURN (hello_impl,
                  Hello (orb.in ()),
                  1);

  PortableServer::ServantBase_var owner_transfer (hello_impl);

  PortableServer::ObjectId_var id =
    root_poa->activate_object (hello_impl);

  CORBA::Object_var hello_obj =
    root_poa->id_to_reference (id.in ());

  Test::Hello_var hello =
    Test::Hello::_narrow (hello_obj.in ());

  CORBA::String_var ior =
    orb->object_to_string (hello.in ());

  poa_manager->activate ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Server activated POA manager\n")));

  // Output the IOR to the <ior_output_file>
  FILE *output_file= ACE_OS::fopen (ior_file_.c_str (), "w");
  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Cannot open output file %s for writing IOR: %C"),
                       ior_file_.c_str (),
                       ior.in ()),
                      1);
  ACE_OS::fprintf (output_file, "%s", ior.in ());
  ACE_OS::fclose (output_file);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Server entering the event loop\n")));

  orb->run ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Server exiting the event loop\n")));

  root_poa->destroy (1, 1);

  // During normal test execution the ORB would have been destroyed
  // by a request from the client.

  //  orb->shutdown (0);

  orb->destroy ();

  return 0;
}
