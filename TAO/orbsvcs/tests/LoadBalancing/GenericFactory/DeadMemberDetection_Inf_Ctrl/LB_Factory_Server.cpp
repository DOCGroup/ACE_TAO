#include "LB_Factory_Server.h"
#include "Simple.h"
#include "Factory.h"

#include "TestC.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"
#include <cstdio>
#include <memory>

LB_Factory_Server::LB_Factory_Server (int argc, ACE_TCHAR ** argv)
  : argc_ (argc),
    argv_ (argv)
{
}

int
LB_Factory_Server::run ()
{
  try
    {
      this->orb_->run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught while running LB_Factory_Server\n");
      return -1;
    }
  return 0;
}


int
LB_Factory_Server::destroy ()
{
  try
    {
      this->lm_->delete_object (this->fcid_.in ());

      this->root_poa_->destroy (1, 1);

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Exception caught while destroying LB_Factory_Server\n");
      return -1;
    }
  return 0;
}

int
LB_Factory_Server::write_ior_to_file (char const *ior, ACE_TCHAR const *ior_file)
{
  using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;
  File_Ptr output_file (ACE_OS::fopen (ior_file, ACE_TEXT ("w")), ACE_OS::fclose);

  if (!output_file)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Cannot open output file for writing IOR\n")));
      return -1;
    }

  std::fprintf (output_file.get (), "%s", ior);
  return 0;
}

int
LB_Factory_Server::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("n:o:j:l:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        this->options_.server_id = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'o':
        this->options_.factory_ior_file = get_opts.opt_arg ();
        break;
      case 'l':
        this->options_.pong_delay = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'j':
        this->options_.direct_object_ior_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -n <server_id> -o <factory_ior_file> "
                                     "-l <pong_delay> -j <direct_object_ior_file>\n"),
                           argv[0]),
                          -1);
      }


  // Indicates successful parsing of the command line
  return 0;
}

int
LB_Factory_Server::init ()
{
  try
    {
      // Initialise the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_, this->argv_);

      if (this->parse_args (this->argc_, this->argv_) != 0)
        return -1;

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Unable to initialize the POA.\n")),
                          -1);

      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager ();

      poa_manager->activate ();

      CORBA::Object_var obj =
        this->orb_->resolve_initial_references ("LoadManager");

      this->lm_ =
        CosLoadBalancing::LoadManager::_narrow (obj.in ());

      if (CORBA::is_nil (this->lm_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Unable to get Load Manager Reference\n")),
                          -1);

      Factory *servant = new Factory (this->options_.server_id,
                                      this->options_.pong_delay);
      PortableServer::ServantBase_var owner_transfer (servant);
      obj = servant->_this ();
      PortableGroup::GenericFactory_var factory_obj =
        PortableGroup::GenericFactory::_narrow (obj.in ());
      CORBA::String_var ior =
        this->orb_->object_to_string (factory_obj.in ());

      if (this->write_ior_to_file (ior.in (),
                                   this->options_.factory_ior_file.c_str ()) != 0)
        return -1;

      if (this->options_.server_id == 2)
        {
          Simple *simple_servant {};

          ACE_NEW_THROW_EX (simple_servant,
                            Simple (this->options_.server_id, this->options_.pong_delay),
                            CORBA::NO_MEMORY ());

          PortableServer::ServantBase_var simple_safe_servant = simple_servant;

          CORBA::Object_var obj = simple_servant->_this ();
          CORBA::String_var ior = this->orb_->object_to_string (obj.in ());
          if (this->write_ior_to_file (ior.in (),
                                       this->options_.direct_object_ior_file.c_str ()) != 0)
            return -1;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception raised in LB_Factory_Server::init()");
      return -1;
    }

  return 0;
}

