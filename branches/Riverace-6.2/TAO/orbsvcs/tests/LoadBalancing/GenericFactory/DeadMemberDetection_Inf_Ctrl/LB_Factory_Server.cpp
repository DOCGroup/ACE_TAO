// $Id$

#include "LB_Factory_Server.h"
#include "Simple.h"
#include "Factory.h"

#include "TestC.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR* factory1_file = ACE_TEXT ("factory1.ior");
const ACE_TCHAR* factory2_file = ACE_TEXT ("factory2.ior");
const ACE_TCHAR* factory3_file = ACE_TEXT ("factory3.ior");
const ACE_TCHAR* direct_obj2_file = ACE_TEXT ("direct_obj2.ior");

int pong_delay = 0;

LB_Factory_Server::LB_Factory_Server (int argc, ACE_TCHAR ** argv)
  : argc_ (argc)
  , argv_ (argv)
  , server_id_ (0)
{
}

CORBA::ORB_ptr
LB_Factory_Server::orb (void)
{
  return this->orb_.in ();
}

int
LB_Factory_Server::run (void)
{
  try
    {
      this->orb_->run ();
      //ACE_Time_Value period (0, 10);
      //while (1)
      //  {
      //    this->orb_->perform_work (&period);
      //  }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught while running LB_Factory_Server\n");
      return -1;
    }
  return 1;

}


int
LB_Factory_Server::destroy (void)
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
  return 1;

}

int
LB_Factory_Server::write_ior_to_file (const char *ior, const char * ior_file)
{
  FILE *output_file =
    ACE_OS::fopen (ior_file, "w");

  if (output_file == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Cannot open output file %s for writing IOR:", ior_file));
      return -1;
    }

  ACE_OS::fprintf (output_file, "%s", ior);
  ACE_OS::fclose (output_file);
  return 0;
}

int
LB_Factory_Server::parse_args (int& argc, ACE_TCHAR **& argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("n:o:j:l:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        this->server_id_ = ACE_OS::atoi(get_opts.opt_arg());
        break;
      case 'o':
        this->factory_ior_file_ = get_opts.opt_arg ();
        break;
      case 'l':
        pong_delay = ACE_OS::atoi(get_opts.opt_arg());
        break;
      case 'j':
        direct_obj2_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-n <server_id> -o <factory_ior_file> -l <pong_delay>"
                           "\n",
                           argv [0]),
                          -1);
      }


  // Indicates successful parsing of the command line
  return 0;
}

int
LB_Factory_Server::init (void)
{
  try
    {
      // Initialise the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_, this->argv_);

      this->parse_args (this->argc_, this->argv_);

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

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
                           " (%P|%t) Unable to get Load Manager Reference\n"),
                          1);


      Factory* servant = new Factory(server_id_);
      PortableServer::ServantBase_var owner_transfer(servant);
      obj = servant->_this ();
      PortableGroup::GenericFactory_var factory_obj
        = PortableGroup::GenericFactory::_narrow (obj.in ());
      CORBA::String_var ior =
        this->orb_->object_to_string (factory_obj.in ());

      this->write_ior_to_file (ior.in (),
        ACE_TEXT_ALWAYS_CHAR (this->factory_ior_file_.c_str ()));

      if (this->server_id_ == 2)
      {
        Simple *simple_servant;

        ACE_NEW_THROW_EX (simple_servant,
          Simple (this->server_id_),
          CORBA::NO_MEMORY ());

        PortableServer::ServantBase_var simple_safe_servant = simple_servant;

        CORBA::Object_var obj = simple_servant->_this ();
        CORBA::String_var ior = this->orb_->object_to_string (obj.in ());
        this->write_ior_to_file (ior.in (),
          ACE_TEXT_ALWAYS_CHAR (direct_obj2_file));
      }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception raised in LB_Factory_Server::init()");
      return -1;
    }

  return 1;

}

