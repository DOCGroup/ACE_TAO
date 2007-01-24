// $Id$


#include "Test_i.h"
#include "ORB_DLL.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Argv_Type_Converter.h"


ACE_RCSID (Hello,
           server,
           "$Id$")

//
Server_Worker::Server_Worker ()
  : Abstract_Worker ("test.ior")
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
  ACE_Get_Opt get_opts (argc, argv, "o:",0);

  for( int c = 0; ((c = get_opts ()) != -1); )
    switch (c)
    {
    case 'o':
      this->ior_file_ = get_opts.opt_arg ();
      break;
    }

  // Indicates sucessful parsing of the command line
  return 0;
}

//
int
Server_Worker::test_main (int argc, ACE_TCHAR *argv[] ACE_ENV_ARG_DECL)
{
  // Making sure there are no stale ior files to confuse a client
  ACE_OS::unlink (ior_file_.c_str ());

  ACE_Argv_Type_Converter cvt (argc, argv);
  CORBA::ORB_var orb = CORBA::ORB_init (cvt.get_argc (),
                                        cvt.get_ASCII_argv () ACE_ENV_ARG_PARAMETER);

  CORBA::Object_var poa_object =
    orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);

  if (CORBA::is_nil (root_poa.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Panic: nil RootPOA\n")),
                      1);

  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager ();

  if (parse_args (cvt.get_argc (), cvt.get_ASCII_argv ()) != 0)
    return 1;

  Hello *hello_impl;
  ACE_NEW_RETURN (hello_impl,
                  Hello (orb.in ()),
                  1);

  PortableServer::ServantBase_var owner_transfer(hello_impl);

  PortableServer::ObjectId_var id =
    root_poa->activate_object (hello_impl ACE_ENV_ARG_PARAMETER);

  CORBA::Object_var hello_obj =
    root_poa->id_to_reference (id.in ()
                               ACE_ENV_ARG_PARAMETER);

  Test::Hello_var hello =
    Test::Hello::_narrow (hello_obj.in ()
                          ACE_ENV_ARG_PARAMETER);

  CORBA::String_var ior =
    orb->object_to_string (hello.in () ACE_ENV_ARG_PARAMETER);

  poa_manager->activate ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Server activated POA manager\n")));

  // Output the IOR to the <ior_output_file>
  FILE *output_file= ACE_OS::fopen (ior_file_.c_str (), "w");
  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Cannot open output file for writing IOR: %s"),
                       ior_file_.c_str ()),
                      1);
  ACE_OS::fprintf (output_file, "%s", ior.in ());
  ACE_OS::fclose (output_file);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Server entering the event loop\n")));

  orb->run ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Server exiting the event loop\n")));

  root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);

  // During normal test execution the ORB would have been destroyed
  // by a request from the client.

  //  orb->shutdown (0 ACE_ENV_ARG_PARAMETER);

  orb->destroy ();

  return 0;
}
