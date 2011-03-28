//=============================================================================
/**
 *  @file    demux_test_server.cpp
 *
 *  $Id$
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#include "demux_test_server.h"
#include "tao/debug.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

// Constructor
Demux_Test_Server::Demux_Test_Server (void)
  : argc_ (0),
    argv_ (0),
    num_POAs_ (1),
    num_objs_ (1),
    poa_fp_ (0),
    ior_fp_ (0),
    servant_fp_ (0),
    use_user_id_ (0),
    use_transient_poas_ (0)
{
}

// destructor
Demux_Test_Server::~Demux_Test_Server (void)
{
  ACE_OS::fclose (this->poa_fp_);
}


//
// initialize the Demux_Test_Server
//

int
Demux_Test_Server::init (int argc, ACE_TCHAR *argv [])
{
  ACE_OS::printf ("here\n");

  this->argc_ = argc;
  this->argv_ = argv;

  // Grab the ORB
  try
    {
      // get the underlying ORB
      this->orb_ =
        CORBA::ORB_init (argc, argv);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ORB_init");
      throw;
    }

  // Grab the ROOT POA
  try
    {
      CORBA::Object_var temp;      // holder for the myriad of times we get
                                   // an object which we then have to narrow.
      // Get the Root POA

      temp =
        this->orb_->resolve_initial_references ("RootPOA");
      if (CORBA::is_nil (temp.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to get root poa reference.\n"),
                          1);

      this->root_poa_ =
        PortableServer::POA::_narrow (temp.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("PortableServer::POA::_narrow");
      throw;
    }

  // grab the POA Manager
  try
    {

      this->poa_mgr_ =
        this->root_poa_->the_POAManager ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("RootPOA->the_POAManager");
      throw;
    }

  // now parse the rest of the arguments to determine the POA depth, the number
  // of objects with each POA and other info

  ACE_DEBUG ((LM_DEBUG,
              "Before Parse Args\n"));

  if (this->parse_args () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) Demux_Test_Server::init - "
                       "parse_args failed\n"),
                      -1);

  // init the Policies used by all the POAs
  CORBA::PolicyList policies (2);

  try
    {
      // The id_uniqueness_policy by default is UNIQUE_ID. So each of our servants
      // will have a unique name

      policies.length (2);

      // Choose the ID Policy for servants.

      if (this->use_user_id_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Using the USER_ID policy ...\n"));

          policies[0] =
            this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Using the SYSTEM_ID policy ...\n"));

          policies[0] =
            this->root_poa_->create_id_assignment_policy (PortableServer::SYSTEM_ID);
        }


      // Choose the LifeSpan Policy. Default is PERSISTENT.
      if (this->use_transient_poas_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Using the TRANSIENT Lifespan policy for the POAs\n"));

          policies[1] =
            this->root_poa_->create_lifespan_policy (PortableServer::TRANSIENT);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Using the PERSISTENT Lifespan policy for the POAs\n"));

          policies[1] =
            this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);
        }

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("creating policy");
      throw;
    }

  // now create a POA hierarchy of the desired depth and populate each POA with
  // the specified number of objects. Finally, activate these objects.

  char poa_file [128];

  // open the file that has all the POA names in it
  if ((this->poa_fp_ = ACE_OS::fopen ("poa_names_100.dat", "r")) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Unable to open POA file %s\n", poa_file),
                        -1);
    }

  // Open the file that has the servant names in it.
  if ((this->servant_fp_ = ACE_OS::fopen ("names_file", "r")) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Unable to open POA file %s\n", poa_file),
                        -1);
    }

  // loop indices
  CORBA::ULong i, j;

  PortableServer::POA *prev_poa = this->root_poa_.in ();
  for (i = 0; i < this->num_POAs_; i++)
    {
      char poa_name [128];

      ACE_OS::memset (poa_name, 0, 128);
      int n_matched = fscanf (this->poa_fp_, "%s", poa_name);

      try
        {
          this->child_poa_[i] = prev_poa->create_POA (poa_name,
                                                      this->poa_mgr_.in (),
                                                      policies);
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("create_POA");
          throw;
        }

      for (j = 0; j < this->num_objs_; j++)
        {
          PortableServer::ObjectId_var id;

          if (!use_user_id_)
            {
              // activate the object
              try
                {
                  Demux_Test_i * demux_test_i_ptr = 0;
                  ACE_NEW_RETURN (demux_test_i_ptr,
                                  Demux_Test_i,
                                  -1);
                  // POA will hold the servant
                  PortableServer::ServantBase_var owner (demux_test_i_ptr);

                  //id = this->child_poa_[i]->activate_object (&this->demux_test_[j],
                  id = this->child_poa_[i]->activate_object (demux_test_i_ptr);
                }
              catch (const CORBA::Exception& ex)
                {
                  ex._tao_print_exception ("poa->activate_obj");
                  throw;
                }

              // Get the IOR and output it to the file
              try
                {
                  CORBA::Object_var demux_var = this->child_poa_[i]->id_to_reference (id.in ());

                  CORBA::String_var ior = this->orb_->object_to_string
                    (demux_var.in ());


                  ACE_OS::fprintf (this->ior_fp_, "%s\n", ior.in ());
                }
              catch (const CORBA::Exception& ex)
                {
                  ex._tao_print_exception ("object_to_string");
                  throw;
                }
            }
          else
            {
              // Use the USER_ID policy.

              char servant_name [128];

              try
                {
                  Demux_Test_i * demux_test_i_ptr;
                  ACE_NEW_RETURN (demux_test_i_ptr,
                                  Demux_Test_i (this->child_poa_[i].in ()),
                                  -1);

                  ACE_OS::memset (servant_name, 0, 128);

                  n_matched = fscanf (this->servant_fp_, "%s", servant_name);

                  ACE_DEBUG ((LM_DEBUG,
                              "Activating Servant with Name : %s\n",
                              servant_name));

                  PortableServer::ObjectId_var oid =
                    PortableServer::string_to_ObjectId (servant_name);

                  this->child_poa_[i]->activate_object_with_id (oid.in (),
                                                                demux_test_i_ptr);

                  // Get Object reference for demux_test_i impl object.
                  CORBA::Object_var demux_var = demux_test_i_ptr->_this ();


                  CORBA::String_var ior = this->orb_->object_to_string
                    (demux_var.in ());


                  ACE_OS::fprintf (this->ior_fp_, "%s\n", ior.in ());

                }
              catch (const CORBA::Exception& ex)
                {
                  ex._tao_print_exception ("object_to_string");
                  throw;
                }

            }// end of if (!use_user_id_)

        } // j loop

      prev_poa = this->child_poa_[i].in ();

    } // i loop

  ACE_OS::fclose (this->ior_fp_);
  this->ior_fp_ = 0;
  ACE_OS::fclose (this->servant_fp_);

  // now activate the POAs

  try
    {
      this->poa_mgr_->activate ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("poa_mgr->activate");
      throw;
    }

  // success
  return 0;

}

// parse command line arguments (if any).
int
Demux_Test_Server::parse_args (void)
{

  ACE_Get_Opt get_opts (this->argc_, this->argv_, ACE_TEXT("df:o:p:ut"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'f':
        this->ior_fp_ = ACE_OS::fopen (get_opts.opt_arg (), "w");
        if (this->ior_fp_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.opt_arg ()), -1);
        break;
      case 'o':
        this->num_objs_ = ACE_OS::atoi (get_opts.opt_arg ());
        if (this->num_objs_ > TAO_DEMUX_TEST_MAX_OBJS)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%d exceeds the maximum of "
                               "%d objects per POA\n",
                               this->num_objs_,
                               TAO_DEMUX_TEST_MAX_OBJS),
                              -1);
          }
        break;
      case 'p':
        this->num_POAs_ = ACE_OS::atoi (get_opts.opt_arg ());
        if (this->num_POAs_ > TAO_DEMUX_TEST_MAX_POAS)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%d exceeds the maximum of "
                               "%d POAs\n",
                               this->num_objs_,
                               TAO_DEMUX_TEST_MAX_POAS),
                              -1);
          }
        break;
      case 'u':
        this->use_user_id_ = 1;
        break;
      case 't':
        this->use_transient_poas_ = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o <num objects>]"
                           " [-p <num POAs>]"
                           " [-f <IOR file>]"
                           "\n", this->argv_ [0]),
                          -1);
      }

  if (!this->ior_fp_)
    {
      // open default IOR file
      this->ior_fp_ = ACE_OS::fopen ("ior.dat", "w");
      if (this->ior_fp_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to open file ior.dat for writing\n"), -1);
    }
  return 0;
}

// grab a reference to the naming service so that we can register with it.
int
Demux_Test_Server::init_naming_service (void)
{
  // Initialize the naming services
  if (this->my_name_client_.init (this->orb_.in ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to initialize "
                       "the ACE_Naming_Client.\n"),
                      -1);

  // success
  return 0;
}

// The main program for Demux_Test
int
Demux_Test_Server::run (void)
{
  try
    {
      this->orb_->run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("run failed");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Demux_Test_Server::run - "
                         "Error running the server\n"),
                        -1);
    }

  ACE_TIMEPROBE_PRINT;

  return 0;
}
