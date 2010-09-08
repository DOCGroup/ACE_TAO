// $Id$

#include "airplane_server_i.h"

#include "tao/IORTable/IORTable.h"
#include "tao/ImR_Client/ImR_Client.h"
#include "tao/debug.h"
#include "tao/PortableServer/Root_POA.h"

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_time.h"

// The server name of the Aiprlane Server
const char SERVER_NAME[] = "airplane_server";

Airplane_Server_i::Airplane_Server_i (void)
  : server_impl_ (0),
    ior_output_file_ (0)
{
  // Nothing
}

int
Airplane_Server_i::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, ACE_TEXT("do:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.opt_arg (), "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.opt_arg ()), -1);
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
Airplane_Server_i::init (int argc, ACE_TCHAR** argv)
{
  // Since the Implementation Repository keys off of the POA name, we need
  // to use the SERVER_NAME as the POA's name.
  const char *poa_name = SERVER_NAME;

  try
    {
      // Initialize the ORB
      this->orb_ = CORBA::ORB_init (argc, argv);

      // Save pointers to the command line arguments
      this->argc_ = argc;
      this->argv_ = argv;

      // Now check the arguments for our options
      int retval = this->parse_args ();

      if (retval != 0)
        return retval;

      // Get the POA from the ORB.
      CORBA::Object_var obj =
        this->orb_->resolve_initial_references ("RootPOA");
      ACE_ASSERT(! CORBA::is_nil (obj.in ()));

      // Narrow the object to a POA.
      root_poa_ = PortableServer::POA::_narrow (obj.in ());

      // Get the POA_Manager.
      this->poa_manager_ = this->root_poa_->the_POAManager ();

      // We now need to create a POA with the persistent and user_id policies,
      // since they are need for use with the Implementation Repository.

      CORBA::PolicyList policies (2);
      policies.length (2);

      policies[0] =
        this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);

      policies[1] =
        this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);

      this->airplane_poa_ =
        this->root_poa_->create_POA (poa_name,
                              this->poa_manager_.in (),
                              policies);

      // Creation of the new POA is over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy ();
        }

      ACE_NEW_RETURN (this->server_impl_, Airplane_i, -1);

      PortableServer::ObjectId_var server_id =
        PortableServer::string_to_ObjectId ("server");

      this->airplane_poa_->activate_object_with_id (server_id.in (),
                                                    this->server_impl_);

      obj = this->airplane_poa_->id_to_reference (server_id.in ());
      CORBA::String_var ior =
        this->orb_->object_to_string (obj.in ());
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "The ImRified IOR is: <%s>\n", ior.in ()));

      TAO_Root_POA* tmp_poa = dynamic_cast<TAO_Root_POA*>(airplane_poa_.in());
      obj = tmp_poa->id_to_reference_i (server_id.in (), false);
      CORBA::String_var plain_ior =
        this->orb_->object_to_string (obj.in ());
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "The plain IOR is: <%s>\n", plain_ior.in ()));

      // Note : The IORTable will only be used for those clients who try to
      // invoke indirectly using a simple object_key reference
      // like "corbaloc::localhost:8888/airplane_server".
      obj = this->orb_->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (obj.in ());
      ACE_ASSERT(! CORBA::is_nil (adapter.in ()));
      adapter->bind (poa_name, plain_ior.in ());

      this->poa_manager_->activate ();

      if (this->ior_output_file_)
        {
          ACE_OS::fprintf (this->ior_output_file_, "%s", ior.in ());
          ACE_OS::fclose (this->ior_output_file_);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Airplane_Server_i::init");
      throw;
    }


  return 0;
}

int
Airplane_Server_i::run (void)
{
  try
    {
      ACE_Time_Value tv(60);
      ACE_Time_Value tvStart = ACE_OS::gettimeofday();

      this->orb_->run (tv);

      ACE_Time_Value tvEnd = ACE_OS::gettimeofday();

      this->root_poa_->destroy(1, 1);
      this->orb_->destroy();

      if (tvEnd - tvStart > tv - ACE_Time_Value(5))
        return 1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Airplane_Server_i::run");
      throw;
    }


  return 0;
}

Airplane_Server_i::~Airplane_Server_i (void)
{
  delete this->server_impl_;
}
