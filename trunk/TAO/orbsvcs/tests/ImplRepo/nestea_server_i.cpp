// $Id$

#include "nestea_server_i.h"

#include "tao/IORTable/IORTable.h"
#include "tao/ImR_Client/ImR_Client.h"
#include "tao/debug.h"
#include "tao/PortableServer/Root_POA.h"

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_stdio.h"



// The file to save the persistent state to.
const ACE_TCHAR NESTEA_DATA_FILENAME[] = ACE_TEXT("nestea.dat");

// The server name of the Nestea Server
const char SERVER_NAME[] = "nestea_server";

const int SELF_DESTRUCT_SECS = 8; // Must coordinate with run_test.pl

Nestea_Server_i::Nestea_Server_i (const char * /*filename*/)
  : server_impl_ (0),
    ior_output_file_ (0)
{
  // Nothing
}

Nestea_Server_i::~Nestea_Server_i (void)
{
  delete this->server_impl_;
}

int
Nestea_Server_i::parse_args (void)
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
                           " [-r]"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}


// The init() method does quite a few things.
//
// - Initialize the ORB
// - Create a persistent POA for the server
// - Activate the POA Manager
// - Activate the servant under the POA
// - Uses the IR helper class to alter the object
// - Creates an IOR from the servant and outputs it to a file

static void printEnvVars() {
  char* useimr = ACE_OS::getenv("TAO_USE_IMR");
  char* ior = ACE_OS::getenv("ImplRepoServiceIOR");
  ACE_OS::printf("nestea_server: TAO_USE_IMR=%s\n", useimr != 0 ? useimr : "<null>");
  ACE_OS::printf("nestea_server: ImplRepoServiceIOR=%s\n", ior != 0 ? ior : "<null>");
}

int
Nestea_Server_i::init (int argc, ACE_TCHAR** argv)
{
  printEnvVars();
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

      this->root_poa_ = PortableServer::POA::_narrow (obj.in ());

      this->poa_manager_ = this->root_poa_->the_POAManager ();

      // We now need to create a POA with the persistent and user_id policies,
      // since they are need for use with the Implementation Repository.

      CORBA::PolicyList policies (2);
      policies.length (2);

      policies[0] =
        this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);

      policies[1] =
        this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);

      this->nestea_poa_ =
        this->root_poa_->create_POA (poa_name,
                              this->poa_manager_.in (),
                              policies);

      // Creation of the new POA is over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0; i < policies.length (); ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy ();
        }

      ACE_NEW_RETURN (this->server_impl_,
                      Nestea_i (orb_.in(), NESTEA_DATA_FILENAME),
                      -1);

      PortableServer::ObjectId_var server_id =
        PortableServer::string_to_ObjectId ("server");

      this->nestea_poa_->activate_object_with_id (server_id.in (),
                                                  this->server_impl_);

      obj = this->nestea_poa_->id_to_reference (server_id.in ());
      CORBA::String_var ior =
        this->orb_->object_to_string (obj.in ());
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "The IOR is: <%s>\n", ior.in ()));

      TAO_Root_POA* tmp_poa = dynamic_cast<TAO_Root_POA*>(nestea_poa_.in());
      obj = tmp_poa->id_to_reference_i (server_id.in (), false);
      CORBA::String_var rawior =
        this->orb_->object_to_string (obj.in ());

      obj = this->orb_->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (obj.in ());
      ACE_ASSERT(! CORBA::is_nil (adapter.in ()));

      adapter->bind (poa_name, rawior.in());

      this->poa_manager_->activate ();

      if (this->ior_output_file_)
        {
          ACE_OS::fprintf (this->ior_output_file_, "%s", ior.in ());
          ACE_OS::fclose (this->ior_output_file_);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Nestea_i::init");
      throw;
    }


  return 0;
}

int
Nestea_Server_i::run (void)
{
  int status = 0;

  try
    {
      ACE_Time_Value tv(SELF_DESTRUCT_SECS);

      this->orb_->run (tv);

      this->root_poa_->destroy(1, 1);
      this->orb_->destroy();
    }
  catch (const CORBA::Exception& ex)
    {
      status = -1;
      ex._tao_print_exception ("Nestea_i::run");
      throw;
    }

  return status;
}
