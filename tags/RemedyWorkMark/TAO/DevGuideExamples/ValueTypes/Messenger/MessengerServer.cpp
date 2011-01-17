// $Id$

#include "_pch.h"

#include "Messenger_i.h"
#include "Message_i.h"

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

void write_ior(const char* ior) {
  std::ofstream out(ACE_TEXT_ALWAYS_CHAR(ior_output_file));
  out << ior;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      MessageFactory::register_new_factory(* orb.in());

      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

      PortableServer::POAManager_var poaman = poa->the_POAManager();

      PortableServer::Servant_var<Messenger_i> svt = new Messenger_i;

      PortableServer::ObjectId_var id = poa->activate_object(svt.in());
      obj = poa->id_to_reference(id.in());
      CORBA::String_var ior = orb->object_to_string(obj.in());
      write_ior(ior.in());

      std::cout << "Starting server." << std::endl;

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
