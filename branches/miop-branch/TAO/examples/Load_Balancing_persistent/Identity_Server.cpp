// $Id$

#include "Identity_Server.h"
#include "Identity_i.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"

Identity_Server::Identity_Server (void)
  : group_factory_ior_ (0),
    random_objects_ (5),
    rr_objects_ (5)
{
}

int
Identity_Server::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "di:a:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'i': // ior of the <Object_Group_Factory> object.
        this->group_factory_ior_ = get_opts.opt_arg ();
        break;
      case 'a': // number of objects to create/register with the random group.
        random_objects_ = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'o': // number of objects to create/register with round
                //robin group.
        rr_objects_ = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-i] <Object_Group_Factory_ior>"
                           " [-a] <number_of_objects_for_random_group>"
                           " [-o] <number_of_objects_for_rr_group>"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
Identity_Server::init (int argc,
                       char* argv[])
{
  int result;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      result = this->orb_manager_.init (argc,
                                        argv
                                        TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (result == -1)
        return result;

      CORBA::PolicyList policies (2);
      policies.length (2);

      // Lifespan policy
      policies[0] =
        this->orb_manager_.root_poa()->create_lifespan_policy (PortableServer::PERSISTENT
                                                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policies[1] =
        this->orb_manager_.root_poa()->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION
                                                                          TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->persistent_POA_  =
        this->orb_manager_.root_poa()->create_POA ("persistent_server",
                                                   this->orb_manager_.poa_manager (),
                                                   policies
                                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      // Destroy policy objects
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          policies[i]->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }


    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Identity_Server::init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Identity_Server::register_groups (TAO_ENV_SINGLE_ARG_DECL)
{



  // Contact the <Object_Group_Factory> to create 2
  // <Object_Group>s, one random and one rr.
  CORBA::ORB_var orb = orb_manager_.orb ();
  CORBA::Object_var obj =
    orb->string_to_object (this->group_factory_ior_
                           TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  Load_Balancer::Object_Group_Factory_var factory =
    Load_Balancer::Object_Group_Factory::_narrow (obj.in ()
                                                  TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (factory.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Identity_Server::init: "
                       "problems using the factory ior\n"),
                      -1);


  // Unbind the previously registered random group.
  ACE_TRY_EX (UNBIND_RANDOM)
    {
      factory->unbind_random ("Random group"
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (UNBIND_RANDOM);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%N | %l) <Unbind> harmless here \n"));
    }
  ACE_ENDTRY;

  // Unbind the previously registered round robin group
  ACE_TRY_EX (UNBIND_ROUND)
    {
      factory->unbind_round_robin ("Round Robin group"
                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (UNBIND_ROUND);
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%N | %l) <Unbind> harmless here \n"));
    }
  ACE_ENDTRY;


  // We want to make two groups Random & Round Robin.
  Load_Balancer::Object_Group_var random_group =
    factory->make_random ("Random group"
                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  Load_Balancer::Object_Group_var rr_group =
    factory->make_round_robin ("Round Robin group"
                               TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);


  // Create the requested number of <Identity> objects, and
  // register them with the random and round robin
  // <Object_Group>s.
  this->create_objects (random_objects_,
                        random_group.in ()
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);


  this->create_objects (rr_objects_,
                        rr_group.in ()
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

void
Identity_Server::create_objects (size_t number_of_objects,
                                 Load_Balancer::Object_Group_ptr group
                                 TAO_ENV_ARG_DECL)
{
  // Create the specified number of servants, and register each one
  // with the provided <Object_Group>.
  for (size_t i = 0; i < number_of_objects; ++i)
    {
      // Create an id for this servant.
      char id[BUFSIZ];
      ACE_OS::sprintf (id,
                       "Identity object " ACE_SIZE_T_FORMAT_SPECIFIER,
                       i);

      // Create and activate a servant.
      Identity_i * identity_servant;
      ACE_NEW_THROW_EX (identity_servant,
                        Identity_i (id, this->persistent_POA_.in ()),
                        CORBA::NO_MEMORY ());
      ACE_CHECK;

      PortableServer::ServantBase_var s = identity_servant;
      this->orb_manager_.activate_poa_manager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Object_var obj = identity_servant->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      Load_Balancer::Member member;
      member.id = CORBA::string_dup (id);
      member.obj =
        this->orb_manager_.orb ()->object_to_string (obj.in ()
                                                     TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Do an unbind and then bind
      ACE_TRY_EX (UNBIND)
        {
          group->unbind (id TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (UNBIND);
        }
      ACE_CATCHANY
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%N | %l) Harmless here \n"));
        }
      ACE_ENDTRY;

      // Bind the servant in the random <Object_Group>.
      group->bind (member TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

    }
}

int
 Identity_Server::run (TAO_ENV_SINGLE_ARG_DECL)
{
  int result;

  result = this->orb_manager_.run (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return result;
}

Identity_Server::~Identity_Server (void)
{
}

int
main (int argc, char *argv[])
{
  int result = 0;
  Identity_Server server;

  if (server.init (argc, argv) == -1)
    return 1;

  // Check the non-ORB arguments.
  if (server.parse_args (argc, argv) == -1)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      result = server.register_groups (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      result = server.run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Identity_Server");
      return 1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (1);

  if (result == -1)
    return 1;
  else
    return 0;
}
