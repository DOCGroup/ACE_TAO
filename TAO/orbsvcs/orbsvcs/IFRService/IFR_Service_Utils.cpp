// $Id$

#include "IFR_Service_Utils.h"
#include "IDLType_i.h"
#include "Container_i.h"
#include "Contained_i.h"
#include "orbsvcs/orbsvcs/IFRService/ComponentRepository_i.h"
#include "orbsvcs/orbsvcs/IFRService/Options.h"
#include "orbsvcs/orbsvcs/IFRService/IFR_ComponentsS.h"
#include "orbsvcs/IOR_Multicast.h"
#include "tao/IORTable/IORTable.h"
#include "tao/ORB_Core.h"
#include "tao/default_ports.h"
#include "tao/PortableServer/POA.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (IFR_Service,
           IFR_Service,
           "$Id$")
           
TAO_Repository_i *TAO_IFR_Service_Utils::repo_ = 0;

TAO_IFR_Server::TAO_IFR_Server (void)
  : //servant_locator_impl_ (0),
    ior_multicast_ (0),
    config_ (0)
{
}

TAO_IFR_Server::~TAO_IFR_Server (void)
{
  // Get reactor instance from TAO.
  ACE_Reactor *reactor = this->orb_->orb_core ()->reactor ();

  if ( this->ior_multicast_ )
  {
    // Remove event handler for the ior multicast.
    if (reactor->remove_handler (this->ior_multicast_,
                                 ACE_Event_Handler::READ_MASK)
         == -1)
      {
        ACE_DEBUG ((
            LM_DEBUG,
            ACE_TEXT ("Interface Repository: cannot remove handler\n")
          ));
      }
  }

  delete this->config_;
  delete this->ior_multicast_;
}

int
TAO_IFR_Server::init_with_orb (int argc,
                               ACE_TCHAR *argv [],
                               CORBA::ORB_ptr orb,
                               int use_multicast_server)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Duplicate the ORB.
      this->orb_ = CORBA::ORB::_duplicate (orb);

      // Get the POA from the ORB.
      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_LIB_TEXT ("(%P|%t) Unable to initialize the POA.\n")
            ),
            -1
          );
        }
      this->root_poa_ =
        PortableServer::POA::_narrow (poa_object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int retval = OPTIONS::instance ()->parse_args (argc,
                                                    argv);

      if (retval != 0)
        {
          return retval;
        }

      retval = this->create_poa (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (retval != 0)
        {
          return retval;
        }

      retval = this->open_config (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (retval != 0)
        {
          return retval;
        }

      retval = this->create_repository (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (retval != 0)
        {
          return retval;
        }

      if (use_multicast_server)
        {
          retval = this->init_multicast_server (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (retval != 0)
            {
              return retval;
            }
        }
      ACE_DEBUG ((LM_DEBUG,
                  "The IFR IOR is: <%s>\n",
                  this->ifr_ior_.in ()));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "IFR_Service::init");

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
TAO_IFR_Server::fini (void)
{
  ACE_TRY_NEW_ENV
    {
      this->root_poa_->destroy (1,
                                1
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO_IFR_Server::fini");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  return 0;
}

int
TAO_IFR_Server::create_poa (ACE_ENV_SINGLE_ARG_DECL)
{
  PortableServer::POAManager_var poa_manager =
    this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::PolicyList policies (5);
  policies.length (5);

  // ID Assignment Policy.
  policies[0] =
    this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID 
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Lifespan Policy.
  policies[1] =
    this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT 
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1); 

  // Request Processing Policy.
  policies[2] =
    this->root_poa_->create_request_processing_policy (
        PortableServer::USE_DEFAULT_SERVANT 
        ACE_ENV_ARG_PARAMETER
      );
  ACE_CHECK_RETURN (-1);

  // Servant Retention Policy.
  policies[3] =
    this->root_poa_->create_servant_retention_policy (
        PortableServer::NON_RETAIN 
        ACE_ENV_ARG_PARAMETER
      );
  ACE_CHECK_RETURN (-1);

  // Id Uniqueness Policy.
  policies[4] =
    this->root_poa_->create_id_uniqueness_policy (
        PortableServer::MULTIPLE_ID 
        ACE_ENV_ARG_PARAMETER
      );
  ACE_CHECK_RETURN (-1);

  this->repo_poa_ =
    this->root_poa_->create_POA ("repoPOA",
                                 poa_manager.in (),
                                 policies
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  policies[0]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
TAO_IFR_Server::open_config (ACE_ENV_SINGLE_ARG_DECL)
{
  if (OPTIONS::instance ()->using_registry ())
    {
#if defined (ACE_WIN32)
      HKEY root =
        ACE_Configuration_Win32Registry::resolve_key (
            HKEY_LOCAL_MACHINE,
            "Software\\TAO\\IFR"
          );

      ACE_NEW_THROW_EX (this->config_,
                        ACE_Configuration_Win32Registry (root),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (-1);
#endif /* ACE_WIN32 */

      return 0;
    }
  else
    {
      ACE_Configuration_Heap *heap = 0;
      ACE_NEW_THROW_EX (heap,
                        ACE_Configuration_Heap,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (-1);

      if (OPTIONS::instance ()->persistent ())
        {
          const char *filename = OPTIONS::instance ()->persistent_file ();

          if (heap->open (filename))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("Error:: Opening persistent heap file '%s'\n"),
                  filename
                ),
                -1
              );
            }
        }
      else
        {
          heap->open ();
        }

      this->config_ = heap;

      return 0;
    }
}

int
TAO_IFR_Server::create_repository (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_ComponentRepository_i *impl = 0;
  ACE_NEW_THROW_EX (
      impl,
      TAO_ComponentRepository_i (
          this->orb_.in (),
          this->root_poa_,
          this->config_
        ),
      CORBA::NO_MEMORY ()
    );
  ACE_CHECK_RETURN (-1);

  auto_ptr<TAO_ComponentRepository_i> safety (impl);
  TAO_IFR_Service_Utils::repo_ = impl;

  POA_CORBA::ComponentIR::Repository_tie<TAO_ComponentRepository_i> *impl_tie
    = 0;

  ACE_NEW_THROW_EX (
      impl_tie,
      POA_CORBA::ComponentIR::Repository_tie<TAO_ComponentRepository_i> (
          impl,
          this->repo_poa_,
          1
        ),
      CORBA::NO_MEMORY ()
    );
  ACE_CHECK_RETURN (-1);

  PortableServer::ServantBase_var tie_safety (impl_tie);
  safety.release ();

  this->repo_poa_->set_servant (impl_tie
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  PortableServer::ObjectId_var oid = 
    PortableServer::string_to_ObjectId ("");

  CORBA::Object_var obj =
    this->repo_poa_->create_reference_with_id (
        oid.in (),
        "IDL:omg.org/CORBA/ComponentIR/ComponentRepository:1.0"
        ACE_ENV_ARG_PARAMETER
      );
  ACE_CHECK_RETURN (-1);

  CORBA::Repository_ptr repo_ref =
    CORBA::Repository::_narrow (obj.in ()
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Initialize the repository.
  int status = impl->repo_init (repo_ref,
                                this->repo_poa_
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (status != 0)
    {
      return -1;
    }

  // Save and output the IOR string.
  this->ifr_ior_ =
    this->orb_->object_to_string (repo_ref
                                  ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);

  CORBA::Object_var table_object =
    this->orb_->resolve_initial_references ("IORTable"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  IORTable::Table_var adapter =
    IORTable::Table::_narrow (table_object.in ()
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (adapter.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Nil IORTable\n"), -1);
    }
  else
    {
      adapter->bind ("InterfaceRepository",
                     this->ifr_ior_.in ()
                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  // Add the repository to the ORB's table of initialized object references.
  this->orb_->register_initial_reference ("InterfaceRepository", 
                                          repo_ref 
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

  FILE *output_file_ =
    ACE_OS::fopen (OPTIONS::instance()->ior_output_file (),
                   "w");

  ACE_OS::fprintf (output_file_,
                   "%s",
                   this->ifr_ior_.in ());

  ACE_OS::fclose (output_file_);

  return 0;
}

// Install ior multicast handler.
int
TAO_IFR_Server::init_multicast_server (ACE_ENV_SINGLE_ARG_DECL)
{
  // Get reactor instance from TAO.
  ACE_Reactor *reactor = this->orb_->orb_core ()->reactor ();

#if defined (ACE_HAS_IP_MULTICAST)

  // See if the -ORBMulticastDiscoveryEndpoint option was specified.
  ACE_CString mde (
      TAO_ORB_Core_instance ()->orb_params ()->mcast_discovery_endpoint ()
    );

  // First, see if the user has given us a multicast port number
  // on the command-line;
  u_short port =
    TAO_ORB_Core_instance ()->orb_params ()->service_port (
                                                 INTERFACEREPOSERVICE
                                               );

  if (port == 0)
    {
      // Check environment var. for multicast port.
      const char *port_number =
        ACE_OS::getenv ("InterfaceRepoServicePort");

      if (port_number != 0)
        port = ACE_OS::atoi (port_number);
    }

  // Port wasn't specified on the command-line or in environment -
  // use the default.
  if (port == 0)
    port = TAO_DEFAULT_INTERFACEREPO_SERVER_REQUEST_PORT;

  // Instantiate a handler which will handle client requests for
  // the IFR ior, received on the multicast port.
  ACE_NEW_THROW_EX (this->ior_multicast_,
                    TAO_IOR_Multicast (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (-1);

  if (mde.length () != 0)
    {
      if (this->ior_multicast_->init (this->ifr_ior_.in (),
                                      mde.c_str (),
                                      TAO_SERVICEID_INTERFACEREPOSERVICE)
           == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("Interface Repository: cannot initialize ")
              ACE_TEXT ("multicast event handler\n")
            ),
            -1
          );
        }
    }
  else
    {
      if (this->ior_multicast_->init (this->ifr_ior_.in (),
                                      port,
                                      ACE_DEFAULT_MULTICAST_ADDR,
                                      TAO_SERVICEID_INTERFACEREPOSERVICE)
           == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("Interface Repository: cannot initialize ")
              ACE_TEXT ("multicast event handler\n")
            ),
            -1
          );
        }
    }

  // Register event handler for the ior multicast.
  if (reactor->register_handler (this->ior_multicast_,
                                 ACE_Event_Handler::READ_MASK)
       == -1)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("Interface Repository: cannot register ")
          ACE_TEXT ("multicast event handler\n")
        ),
        -1
      );
    }

#endif /* ACE_HAS_IP_MULTICAST */

  return 0;
}

TAO_IFR_Service_Utils::TAO_IFR_Service_Utils (void)
{
}

TAO_IFR_Service_Utils::~TAO_IFR_Service_Utils (void)
{
}

ACE_Configuration_Section_Key
TAO_IFR_Service_Utils::tmp_key_;

char *
TAO_IFR_Service_Utils::int_to_string (CORBA::ULong number)
{
  static char hex_string[9];
  ACE_OS::sprintf (hex_string, 
                   "%8.8X", 
                   number);
  hex_string[8] = '\0';
  return hex_string;
}

void
TAO_IFR_Service_Utils::valid_container (
    CORBA::DefinitionKind container_kind,
    CORBA::DefinitionKind contained_kind
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int error_flag = 0;

  switch (container_kind)
  {
    case CORBA::dk_Repository:
    case CORBA::dk_Module:
      break;
    case CORBA::dk_Exception:
    case CORBA::dk_Struct:
    case CORBA::dk_Union:
      switch (contained_kind)
      {
        case CORBA::dk_Struct:
        case CORBA::dk_Union:
        case CORBA::dk_Enum:
          break;
        default:
          error_flag = 1;
          break;
      }
      break;
    case CORBA::dk_Value:
    case CORBA::dk_Interface:
    case CORBA::dk_AbstractInterface:
    case CORBA::dk_LocalInterface:
    case CORBA::dk_Home:
      switch (contained_kind)
      {
        case CORBA::dk_Module:
        case CORBA::dk_Interface:
        case CORBA::dk_Value:
          error_flag = 1;
          break;
        default:
          break;
      }
      break;
    case CORBA::dk_Component:
      error_flag = 1;
      break;
    default:
      break;
  }

  if (error_flag == 1)
    {
      ACE_THROW (CORBA::BAD_PARAM (4,
                                   CORBA::COMPLETED_NO));
    }
}

void
TAO_IFR_Service_Utils::pre_exist (
    const char *id,
    TAO_IFR_Service_Utils::name_clash_checker checker,
    ACE_Configuration_Section_Key &key,
    TAO_Repository_i *repo,
    CORBA::DefinitionKind kind
    ACE_ENV_ARG_DECL
  )
{
  TAO_IFR_Service_Utils::id_exists (id,
                                    repo
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_IFR_Service_Utils::name_exists (checker,
                                      key,
                                      repo,
                                      kind
                                      ACE_ENV_ARG_PARAMETER);
}

void
TAO_IFR_Service_Utils::id_exists (const char *id,
                                  TAO_Repository_i *repo
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // The repo id must not already exist.
  ACE_TString holder;
  if (repo->config ()->get_string_value (repo->repo_ids_key (),
                                         id,
                                         holder)
       == 0)
    {
      ACE_THROW (CORBA::BAD_PARAM (2,
                                   CORBA::COMPLETED_NO));
    }
}

void
TAO_IFR_Service_Utils::name_exists (
    TAO_IFR_Service_Utils::name_clash_checker checker,
    ACE_Configuration_Section_Key &key,
    TAO_Repository_i *repo,
    CORBA::DefinitionKind kind
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int index = 0;
  int status = 0;
  ACE_TString section_name;

  // Check the members defined elsewhere, if any.
  ACE_Configuration_Section_Key refs_key;
  status =
    repo->config ()->open_section (key,
                                   "refs",
                                   0,
                                   refs_key);

  if (status == 0)
    {
      while (repo->config ()->enumerate_sections (refs_key,
                                                  index++,
                                                  section_name)
              == 0)
        {
          ACE_Configuration_Section_Key member_key;
          repo->config ()->open_section (refs_key,
                                         section_name.c_str (),
                                         0,
                                         member_key);

          ACE_TString member_name;
          repo->config ()->get_string_value (member_key,
                                             "name",
                                             member_name);

          if ((*checker) (member_name.fast_rep ()) != 0)
            {
              ACE_THROW (CORBA::BAD_PARAM (3,
                                           CORBA::COMPLETED_NO));
            }
        }
    }

  // Check the members defined in this scope, if any.
  ACE_Configuration_Section_Key defns_key;
  status = 
    repo->config ()->open_section (key,
                                   "defns",
                                   0,
                                   defns_key);
  if (status == 0)
    {
      index = 0;

      while (repo->config ()->enumerate_sections (defns_key,
                                                  index++,
                                                  section_name)
              == 0)
        {
          ACE_Configuration_Section_Key defn_key;
          repo->config ()->open_section (defns_key,
                                         section_name.c_str (),
                                         0,
                                         defn_key);

          ACE_TString defn_name;
          repo->config ()->get_string_value (defn_key,
                                             "name",
                                             defn_name);

          if ((*checker) (defn_name.fast_rep ()) != 0)
            {
              ACE_THROW (CORBA::BAD_PARAM (3,
                                           CORBA::COMPLETED_NO));
            }
        }
    }

  if (kind == CORBA::dk_Interface
      || kind == CORBA::dk_Component)
    {
      TAO_IFR_Service_Utils::check_subsection (checker,
                                               "attrs",
                                               repo->config (),
                                               key
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (kind == CORBA::dk_Interface)
    {
      TAO_IFR_Service_Utils::check_subsection (checker,
                                               "ops",
                                               repo->config (),
                                               key
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (kind == CORBA::dk_Component)
    {
      TAO_IFR_Service_Utils::check_subsection (checker,
                                               "provides",
                                               repo->config (),
                                               key
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      TAO_IFR_Service_Utils::check_subsection (checker,
                                               "uses",
                                               repo->config (),
                                               key
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      TAO_IFR_Service_Utils::check_subsection (checker,
                                               "emits",
                                               repo->config (),
                                               key
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      TAO_IFR_Service_Utils::check_subsection (checker,
                                               "publishes",
                                               repo->config (),
                                               key
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      TAO_IFR_Service_Utils::check_subsection (checker,
                                               "consumes",
                                               repo->config (),
                                               key
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_IFR_Service_Utils::check_subsection (
    TAO_IFR_Service_Utils::name_clash_checker checker,
    const char *sub_section,
    ACE_Configuration *config,
    ACE_Configuration_Section_Key &key
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Configuration_Section_Key sub_key;
  int status =
    config->open_section (key,
                          sub_section,
                          0,
                          sub_key);

  if (status != 0)
    {
      return;
    }

  CORBA::ULong count = 0;
  config->get_integer_value (sub_key,
                             "count",
                             count);

  ACE_Configuration_Section_Key entry_key;
  ACE_TString entry_name;
  char *stringified = 0;

  for (CORBA::ULong i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      config->open_section (sub_key,
                            stringified,
                            0,
                            entry_key);
      config->get_string_value (entry_key,
                                "name",
                                entry_name);

      if ((*checker) (entry_name.fast_rep ()) != 0)
        {
          ACE_THROW (CORBA::BAD_PARAM (5,
                                       CORBA::COMPLETED_NO));
          return;
        }
    }
}

void
TAO_IFR_Service_Utils::valid_creation (
    CORBA::DefinitionKind container_kind,
    CORBA::DefinitionKind contained_kind,
    const char *id,
    TAO_IFR_Service_Utils::name_clash_checker checker,
    ACE_Configuration_Section_Key &key,
    TAO_Repository_i *repo
    ACE_ENV_ARG_DECL
  )
{
  TAO_IFR_Service_Utils::valid_container (container_kind,
                                          contained_kind
                                          ACE_ENV_ARG_PARAMETER);

  TAO_IFR_Service_Utils::pre_exist (id,
                                    checker,
                                    key,
                                    repo,
                                    container_kind
                                    ACE_ENV_ARG_PARAMETER);
}

ACE_TString
TAO_IFR_Service_Utils::create_common (
    CORBA::DefinitionKind container_kind,
    CORBA::DefinitionKind contained_kind,
    ACE_Configuration_Section_Key container_key,
    ACE_Configuration_Section_Key &new_key,
    TAO_Repository_i *repo,
    const char *id,
    const char *name,
    TAO_IFR_Service_Utils::name_clash_checker checker,
    const char *version,
    const char *sub_section_name
    ACE_ENV_ARG_DECL
  )
{
  ACE_TString path;

  // No need for a return value, every error we check for will throw
  // one of the BAD_PARAM versions if it is discovered.
  TAO_IFR_Service_Utils::valid_creation (container_kind,
                                         contained_kind,
                                         id,
                                         checker,
                                         container_key,
                                         repo
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (path);

  ACE_Configuration *config = repo->config ();

  // Create new section, or open if it already exists.
  ACE_Configuration_Section_Key sub_key;
  config->open_section (container_key,
                        sub_section_name,
                        1,
                        sub_key);

  u_int defn_count = 0;

  // If we have a count, it gets set, if not, it stays 0.
  config->get_integer_value (sub_key,
                             "count",
                             defn_count);

  char *section_name = 
    TAO_IFR_Service_Utils::int_to_string (defn_count);
  config->open_section (sub_key,
                        section_name,
                        1,
                        new_key);

  // Increment the count.
  config->set_integer_value (sub_key,
                             "count",
                             defn_count + 1);

  config->get_integer_value (sub_key,
                             "count",
                             defn_count);

  // Set the name attribute.
  config->set_string_value (new_key,
                            "name",
                            name);

  // Set the id attribute.
  config->set_string_value (new_key,
                            "id",
                            id);

  // Set the version attribute.
  config->set_string_value (new_key,
                            "version",
                            version);

  // Set the definition kind.
  config->set_integer_value (new_key,
                             "def_kind",
                             contained_kind);

  // Get the container's absolute name, append the new name,
  // and set it in the new section.
  ACE_TString absolute_name;
  config->get_string_value (container_key,
                            "absolute_name",
                            absolute_name);

  absolute_name += "::";
  absolute_name += name;

  config->set_string_value (new_key,
                            "absolute_name",
                            absolute_name);

  // Get the container's path.
  ACE_TString container_id;
  config->get_string_value (container_key,
                            "id",
                            container_id);

  config->set_string_value (new_key,
                            "container_id",
                            container_id);

  if (container_id == "") // This Container is the Repository.
    {
      path = "";
    }
  else
    {
      config->get_string_value (repo->repo_ids_key (),
                                container_id.c_str (),
                                path);

      path += '\\';
    }

  path += sub_section_name;
  path += '\\';
  path += section_name;

  // Store our path under our global repo id for fast lookup.
  config->set_string_value (repo->repo_ids_key (),
                            id,
                            path);

  return path;
}

void
TAO_IFR_Service_Utils::set_initializers (
    const CORBA::InitializerSeq &initializers,
    ACE_Configuration *config,
    ACE_Configuration_Section_Key &key
  )
{
  CORBA::ULong length = initializers.length ();

  if (length == 0)
    {
      return;
    }
    
  ACE_Configuration_Section_Key initializers_key;
  config->open_section (key,
                        "initializers",
                        1,
                        initializers_key);
  config->set_integer_value (initializers_key,
                             "count",
                             length);

  CORBA::ULong arg_count = 0;
  char *arg_path = 0;
  ACE_Configuration_Section_Key initializer_key;
  ACE_Configuration_Section_Key params_key;
  ACE_Configuration_Section_Key arg_key;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      config->open_section (initializers_key,
                            stringified,
                            1,
                            initializer_key);
      config->set_string_value (initializer_key,
                                "name",
                                initializers[i].name.in ());

      arg_count = initializers[i].members.length ();

      if (arg_count > 0)
        {
          config->open_section (initializer_key,
                                "params",
                                1,
                                params_key);
          config->set_integer_value (params_key,
                                     "count",
                                     arg_count);

          for (CORBA::ULong j = 0; j < arg_count; ++j)
            {
              char *stringified = 
                TAO_IFR_Service_Utils::int_to_string (j);
              config->open_section (params_key,
                                    stringified,
                                    1,
                                    arg_key);
              config->set_string_value (
                  arg_key,
                  "arg_name",
                  initializers[i].members[j].name.in ()
                );
              arg_path = 
                TAO_IFR_Service_Utils::reference_to_path (
                    initializers[i].members[j].type_def.in ()
                  );
              config->set_string_value (arg_key,
                                        "arg_path",
                                        arg_path);
            }
        }
    }
}

char *
TAO_IFR_Service_Utils::reference_to_path (CORBA::IRObject_ptr obj)
{
  PortableServer::ObjectId object_id;
  TAO::ObjectKey object_key = 
    obj->_stubobj ()->profile_in_use ()->object_key ();
  int status = TAO_POA::parse_ir_object_key (object_key,
                                             object_id);
  if (status != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "reference_to_path - parse_ir_object_key failed\n"));
      return 0;
    }

  return TAO_IFR_Service_Utils::oid_to_string (object_id);
}

CORBA::DefinitionKind
TAO_IFR_Service_Utils::path_to_def_kind (ACE_TString &path,
                                         TAO_Repository_i *repo)
{
  int status = 
    repo->config ()->expand_path (repo->root_key (),
                                  path,
                                  TAO_IFR_Service_Utils::tmp_key_,
                                  0);

  if (status != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "path_to_def_kind - bad path: '%s'\n",
                  path.c_str ()));
      return CORBA::dk_none;
    }

  u_int kind = 0;
  repo->config ()->get_integer_value (TAO_IFR_Service_Utils::tmp_key_,
                                      "def_kind",
                                      kind);
  return ACE_static_cast (CORBA::DefinitionKind,
                          kind);
}

CORBA::DefinitionKind 
TAO_IFR_Service_Utils::reference_to_def_kind (CORBA::IRObject_ptr obj,
                                              TAO_Repository_i *repo)
{
  ACE_TString path (TAO_IFR_Service_Utils::reference_to_path (obj));
  return TAO_IFR_Service_Utils::path_to_def_kind (path, repo);
}

TAO_IDLType_i *
TAO_IFR_Service_Utils::path_to_idltype (ACE_TString &path,
                                        TAO_Repository_i *repo)
{
  CORBA::DefinitionKind def_kind = 
    TAO_IFR_Service_Utils::path_to_def_kind (path, repo);
  TAO_IDLType_i *retval = repo->select_idltype (def_kind);

  if (retval == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "path_to_idltype - not an IDLType: '%s'\n",
                  path.c_str ()));
      return 0;
    }

  retval->section_key (TAO_IFR_Service_Utils::tmp_key_);
  return retval;
}

TAO_Contained_i *
TAO_IFR_Service_Utils::path_to_contained (ACE_TString &path,
                                          TAO_Repository_i *repo)
{
  CORBA::DefinitionKind def_kind = 
    TAO_IFR_Service_Utils::path_to_def_kind (path, repo);
  TAO_Contained_i *retval = repo->select_contained (def_kind);

  if (retval == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "path_to_contained - not a contained type: '%s'\n",
                  path.c_str ()));
      return 0;
    }

  retval->section_key (TAO_IFR_Service_Utils::tmp_key_);
  return retval;
}

TAO_Container_i *
TAO_IFR_Service_Utils::path_to_container (ACE_TString &path,
                                          TAO_Repository_i *repo)
{
  CORBA::DefinitionKind def_kind = 
    TAO_IFR_Service_Utils::path_to_def_kind (path, repo);
  TAO_Container_i *retval = repo->select_container (def_kind);

  if (retval != 0)
    {
      retval->section_key (TAO_IFR_Service_Utils::tmp_key_);
    }

  return retval;
}

CORBA::Object_ptr
TAO_IFR_Service_Utils::path_to_ir_object (ACE_TString &path,
                                          TAO_Repository_i *repo
                                          ACE_ENV_ARG_DECL)
{
  CORBA::DefinitionKind def_kind = 
    TAO_IFR_Service_Utils::path_to_def_kind (path, repo);

  return TAO_IFR_Service_Utils::create_objref (def_kind,
                                               path.c_str (),
                                               repo
                                               ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_IFR_Service_Utils::create_objref (CORBA::DefinitionKind def_kind,
                                      const char *obj_id,
                                      TAO_Repository_i *repo
                                      ACE_ENV_ARG_DECL)
{
  ACE_TString repo_id ("IDL:omg.org/");

  switch (def_kind)
  {
    case CORBA::dk_Attribute:
      repo_id += "CORBA/ExtAttributeDef:";
      break;
    case CORBA::dk_Constant:
      repo_id += "CORBA/ConstantDef:";
      break;
    case CORBA::dk_Exception:
      repo_id += "CORBA/ExceptionDef:";
      break;
    case CORBA::dk_Interface:
      repo_id += "CORBA/ExtInterfaceDef:";
      break;
    case CORBA::dk_AbstractInterface:
      repo_id += "CORBA/ExtAbstractInterfaceDef:";
      break;
    case CORBA::dk_LocalInterface:
      repo_id += "CORBA/ExtLocalInterfaceDef:";
      break;
    case CORBA::dk_Module:
      repo_id += "CORBA/ComponentIR/ModuleDef:";
      break;
    case CORBA::dk_Operation:
      repo_id += "CORBA/OperationDef:";
      break;
    case CORBA::dk_Typedef:
      repo_id += "CORBA/TypedefDef:";
      break;
    case CORBA::dk_Alias:
      repo_id += "CORBA/AliasDef:";
      break;
    case CORBA::dk_Struct:
      repo_id += "CORBA/StructDef:";
      break;
    case CORBA::dk_Union:
      repo_id += "CORBA/UnionDef:";
      break;
    case CORBA::dk_Enum:
      repo_id += "CORBA/EnumDef:";
      break;
    case CORBA::dk_Primitive:
      repo_id += "CORBA/PrimitiveDef:";
      break;
    case CORBA::dk_String:
      repo_id += "CORBA/StringDef:";
      break;
    case CORBA::dk_Sequence:
      repo_id += "CORBA/SequenceDef:";
      break;
    case CORBA::dk_Array:
      repo_id += "CORBA/ArrayDef:";
      break;
    case CORBA::dk_Wstring:
      repo_id += "CORBA/WstringDef:";
      break;
    case CORBA::dk_Fixed:
      repo_id += "CORBA/FixedDef:";
      break;
    case CORBA::dk_Value:
      repo_id += "CORBA/ExtValueDef:";
      break;
    case CORBA::dk_ValueBox:
      repo_id += "CORBA/ValueBoxDef:";
      break;
    case CORBA::dk_ValueMember:
      repo_id += "CORBA/ValueMemberDef:";
      break;
    case CORBA::dk_Native:
      repo_id += "CORBA/NativeDef:";
      break;
    case CORBA::dk_Component:
      repo_id += "CORBA/ComponentIR/ComponentDef:";
      break;
    case CORBA::dk_Home:
      repo_id += "CORBA/ComponentIR/HomeDef:";
      break;
    case CORBA::dk_Factory:
      repo_id += "CORBA/ComponentIR/FactoryDef:";
      break;
    case CORBA::dk_Finder:
      repo_id += "CORBA/ComponentIR/FinderDef:";
      break;
    case CORBA::dk_Event:
      repo_id += "CORBA/ComponentIR/EventDef:";
      break;
    case CORBA::dk_Emits:
      repo_id += "CORBA/ComponentIR/EmitsDef:";
      break;
    case CORBA::dk_Publishes:
      repo_id += "CORBA/ComponentIR/PublishesDef:";
      break;
    case CORBA::dk_Consumes:
      repo_id += "CORBA/ComponentIR/ConsumesDef:";
      break;
    case CORBA::dk_Provides:
      repo_id += "CORBA/ComponentIR/ProvidesDef:";
      break;
    case CORBA::dk_Uses:
      repo_id += "CORBA/ComponentIR/UsesDef:";
      break;
    default:
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), 
                        CORBA::Object::_nil ());
  }

  repo_id += "1.0";

  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId (obj_id);

  PortableServer::POA_ptr poa = repo->select_poa (def_kind);

  return poa->create_reference_with_id (oid.in (),
                                        repo_id.c_str ()
                                        ACE_ENV_ARG_PARAMETER);
}

char *
TAO_IFR_Service_Utils::oid_to_string (PortableServer::ObjectId &oid)
{
  static char oid_string[2 * 1024];
  CORBA::ULong length = oid.length ();

  // Copy the data.
  ACE_OS::memcpy (oid_string, 
                  oid.get_buffer (), 
                  length);

  // Null terminate the string.
  oid_string[length] = '\0';
  return oid_string;
}

