// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Demux
//
// = FILENAME
//    demux_test_server.cpp
//
// = AUTHOR
//
//    Aniruddha Gokhale
//
// ============================================================================

#include "demux_test_server.h"
#include "tao/debug.h"

ACE_RCSID(CodeGen, demux_test_server, "$Id$")

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
  ACE_OS::fclose (this->ior_fp_);
}


//
// initialize the Demux_Test_Server
//

int
Demux_Test_Server::init (int argc, char *argv []
                         ACE_ENV_ARG_DECL)
{
  printf ("here\n");

  this->argc_ = argc;
  this->argv_ = argv;

  // Grab the ORB
  ACE_TRY_EX(GET_ORB)
    {
      // get the underlying ORB
      this->orb_ =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX(GET_ORB);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ORB_init");
      ACE_RE_THROW_EX (GET_ORB);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  // Grab the ROOT POA
  ACE_TRY_EX (GET_ROOT_POA)
    {
      CORBA::Object_var temp;      // holder for the myriad of times we get
                                   // an object which we then have to narrow.
      // Get the Root POA

      temp =
        this->orb_->resolve_initial_references ("RootPOA"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX(GET_ROOT_POA);
      if (CORBA::is_nil (temp.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to get root poa reference.\n"),
                          1);

      this->root_poa_ =
        PortableServer::POA::_narrow (temp.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (GET_ROOT_POA);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "PortableServer::POA::_narrow");
      ACE_RE_THROW_EX (GET_ROOT_POA);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  // grab the POA Manager
  ACE_TRY_EX (GET_POA_MGR)
    {

      this->poa_mgr_ =
        this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (GET_POA_MGR);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "RootPOA->the_POAManager");
      ACE_RE_THROW_EX (GET_POA_MGR);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

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

  ACE_TRY_EX (POLICY)
    {
      // The id_uniqueness_policy by default is UNIQUE_ID. So each of our servants
      // will have a unique name

      policies.length (2);

      // Choose the ID Policy for servants.

      if (this->use_user_id_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Using the USER_ID policy ... \n"));

          policies[0] =
            this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID
                                                          ACE_ENV_ARG_PARAMETER);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Using the SYSTEM_ID policy ... \n"));

          policies[0] =
            this->root_poa_->create_id_assignment_policy (PortableServer::SYSTEM_ID
                                                          ACE_ENV_ARG_PARAMETER);
        }

      ACE_TRY_CHECK_EX (POLICY);

      // Choose the LifeSpan Policy. Default is PERSISTENT.
      if (this->use_transient_poas_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Using the TRANSIENT Lifespan policy for the POAs\n"));

          policies[1] =
            this->root_poa_->create_lifespan_policy (PortableServer::TRANSIENT
                                                     ACE_ENV_ARG_PARAMETER);
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Using the PERSISTENT Lifespan policy for the POAs\n"));

          policies[1] =
            this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT
                                                     ACE_ENV_ARG_PARAMETER);
        }

      ACE_TRY_CHECK_EX (POLICY);

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "creating policy");
      ACE_RE_THROW_EX (POLICY);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

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
      (void) fscanf (this->poa_fp_, "%s", poa_name);

      ACE_TRY_EX (CREATE_POA)
        {
          this->child_poa_[i] = prev_poa->create_POA (poa_name,
                                                      this->poa_mgr_.in (),
                                                      policies
                                                      ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (CREATE_POA);
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "create_POA");
          ACE_RE_THROW_EX (CREATE_POA);
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (-1);

      for (j = 0; j < this->num_objs_; j++)
        {
          PortableServer::ObjectId_var id;

          if (!use_user_id_)
            {
              // activate the object
              ACE_TRY_EX (ACTIVATE_OBJ)
                {
                  Demux_Test_i * demux_test_i_ptr;
                  ACE_NEW_RETURN (demux_test_i_ptr,
                                  Demux_Test_i,
                                  -1);

                  //id = this->child_poa_[i]->activate_object (&this->demux_test_[j],
                  id = this->child_poa_[i]->activate_object (demux_test_i_ptr
                                                             ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK_EX (ACTIVATE_OBJ);
                }
              ACE_CATCHANY
                {
                  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                       "poa->activate_obj");
                  ACE_RE_THROW_EX (ACTIVATE_OBJ);
                }
              ACE_ENDTRY;
              ACE_CHECK_RETURN (-1);

              // Get the IOR and output it to the file
              ACE_TRY_EX (IOR)
                {
                  CORBA::Object_var demux_var = this->child_poa_[i]->id_to_reference (id.in ()
                                                                                      ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK_EX (IOR);

                  CORBA::String_var ior = this->orb_->object_to_string
                    (demux_var.in () ACE_ENV_ARG_PARAMETER);

                  ACE_TRY_CHECK_EX (IOR);

                  ACE_OS::fprintf (this->ior_fp_, "%s\n", ior.in ());
                }
              ACE_CATCHANY
                {
                  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                       "object_to_string");
                  ACE_RE_THROW_EX (IOR);
                }
              ACE_ENDTRY;
              ACE_CHECK_RETURN (-1);
            }
          else
            {
              // Use the USER_ID policy.

              char servant_name [128];

              ACE_TRY
                {
                  Demux_Test_i * demux_test_i_ptr;
                  ACE_NEW_RETURN (demux_test_i_ptr,
                                  Demux_Test_i (this->child_poa_[i].in ()),
                                  -1);

                  ACE_OS::memset (servant_name, 0, 128);

                  (void) fscanf (this->servant_fp_, "%s", servant_name);

                  ACE_DEBUG ((LM_DEBUG,
                              "Activating Servant with Name : %s\n",
                              servant_name));

                  PortableServer::ObjectId_var oid =
                    PortableServer::string_to_ObjectId (servant_name);

                  this->child_poa_[i]->activate_object_with_id (oid.in (),
                                                                demux_test_i_ptr
                                                                ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  // Get Object reference for demux_test_i impl object.
                  CORBA::Object_var demux_var = demux_test_i_ptr->_this (ACE_ENV_SINGLE_ARG_PARAMETER);

                  ACE_TRY_CHECK;

                  CORBA::String_var ior = this->orb_->object_to_string
                    (demux_var.in () ACE_ENV_ARG_PARAMETER);

                  ACE_TRY_CHECK;

                  ACE_OS::fprintf (this->ior_fp_, "%s\n", ior.in ());

                }
              ACE_CATCHANY
                {
                  ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                       "object_to_string");
                  ACE_RE_THROW;
                }
              ACE_ENDTRY;
              ACE_CHECK_RETURN (-1);

            }// end of if (!use_user_id_)

        } // j loop

      prev_poa = this->child_poa_[i].in ();

    } // i loop

  ACE_OS::fclose (this->ior_fp_);
  ACE_OS::fclose (this->servant_fp_);

  // now activate the POAs

  ACE_TRY_EX (ACTIVATE)
    {
      this->poa_mgr_->activate (ACE_ENV_SINGLE_ARG_PARAMETER);

      ACE_TRY_CHECK_EX (ACTIVATE);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "poa_mgr->activate");
      ACE_RE_THROW_EX (ACTIVATE);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  // success
  return 0;

}

// parse command line arguments (if any).
int
Demux_Test_Server::parse_args (void)
{

  ACE_Get_Opt get_opts (this->argc_, this->argv_, "df:o:p:ut");
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
                       "the ACE_Naming_Client. \n"),
                      -1);

  // success
  return 0;
}

// The main program for Demux_Test
int
Demux_Test_Server::run (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
    {
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "run failed");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Demux_Test_Server::run - "
                         "Error running the server\n"),
                        -1);
    }
  ACE_ENDTRY;

  ACE_TIMEPROBE_PRINT;

  return 0;
}

#if defined (ACE_ENABLE_TIMEPROBES)
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Function_Timeprobe<ACE_Timeprobe<ACE_Null_Mutex> >;
template class ACE_Unbounded_Set<ACE_Event_Descriptions>;
template class ACE_Unbounded_Set_Iterator<ACE_Event_Descriptions>;
template class ACE_Node<ACE_Event_Descriptions>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Function_Timeprobe<ACE_Timeprobe<ACE_Null_Mutex> >
#pragma instantiate ACE_Unbounded_Set<ACE_Event_Descriptions>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_Event_Descriptions>
#pragma instantiate ACE_Node<ACE_Event_Descriptions>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_ENABLE_TIMEPROBES */
