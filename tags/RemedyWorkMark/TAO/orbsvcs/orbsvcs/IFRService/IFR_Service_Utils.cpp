// $Id$

#include "orbsvcs/IFRService/IFR_Service_Utils.h"
#include "orbsvcs/IFRService/IDLType_i.h"
#include "orbsvcs/IFRService/Container_i.h"
#include "orbsvcs/IFRService/Contained_i.h"
#include "orbsvcs/IFRService/ComponentRepository_i.h"
#include "orbsvcs/IFRService/Options.h"
#include "orbsvcs/IFRService/IFR_ComponentsS.h"
#include "orbsvcs/IOR_Multicast.h"
#include "tao/IORTable/IORTable.h"
#include "tao/ORB_Core.h"
#include "tao/default_ports.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/AnyTypeCode/ValueModifierC.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_fcntl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  try
    {
      // Get the POA from the ORB.
      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) IFR_Service::init_with_orb ")
                             ACE_TEXT ("Unable to initialize the POA.\n")),
                            -1);
        }
      PortableServer::POA_var rp =
        PortableServer::POA::_narrow (poa_object.in ());
      return this->init_with_poa (argc, argv, orb, rp.in(), use_multicast_server);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("IFR_Service::init_with_orb");

      throw;
    }
  return 0;
}

int
TAO_IFR_Server::init_with_poa (int argc,
                               ACE_TCHAR *argv [],
                               CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr rp,
                               int use_multicast_server)
{
  try
    {
      // Duplicate the ORB.
      this->orb_ = CORBA::ORB::_duplicate (orb);
      // Duplicate the provided poa as the new root.
      this->root_poa_ = PortableServer::POA::_duplicate (rp);

      int retval = OPTIONS::instance ()->parse_args (argc,
                                                    argv);

      if (retval != 0)
        {
          return retval;
        }

      retval = this->create_poa ();

      if (retval != 0)
        {
          return retval;
        }

      retval = this->open_config ();

      if (retval != 0)
        {
          return retval;
        }

      retval = this->create_repository ();

      if (retval != 0)
        {
          return retval;
        }

      if (use_multicast_server
          || OPTIONS::instance()->support_multicast_discovery ())
        {
          retval =
            this->init_multicast_server ();

          if (retval != 0)
            {
              return retval;
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("IFR_Service::init");

      throw;
    }
  return 0;
}

int
TAO_IFR_Server::fini (void)
{
  try
    {
      this->root_poa_->destroy (1,
                                1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_IFR_Server::fini");
      throw;
    }
  return 0;
}

int
TAO_IFR_Server::create_poa (void)
{
  PortableServer::POAManager_var poa_manager =
    this->root_poa_->the_POAManager ();

  poa_manager->activate ();

  CORBA::PolicyList policies (5);
  policies.length (5);

  // ID Assignment Policy.
  policies[0] =
    this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);

  // Lifespan Policy.
  policies[1] =
    this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);

  // Request Processing Policy.
  policies[2] =
    this->root_poa_->create_request_processing_policy (
        PortableServer::USE_DEFAULT_SERVANT
      );

  // Servant Retention Policy.
  policies[3] =
    this->root_poa_->create_servant_retention_policy (
        PortableServer::NON_RETAIN
      );

  // Id Uniqueness Policy.
  policies[4] =
    this->root_poa_->create_id_uniqueness_policy (
        PortableServer::MULTIPLE_ID
      );

  this->repo_poa_ =
    this->root_poa_->create_POA ("repoPOA",
                                 poa_manager.in (),
                                 policies);

  policies[0]->destroy ();

  return 0;
}

int
TAO_IFR_Server::open_config (void)
{
  if (OPTIONS::instance ()->using_registry ())
    {
#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_REGISTRY)
      HKEY root =
        ACE_Configuration_Win32Registry::resolve_key (
            HKEY_LOCAL_MACHINE,
            "Software\\TAO\\IFR"
          );

      ACE_NEW_THROW_EX (this->config_,
                        ACE_Configuration_Win32Registry (root),
                        CORBA::NO_MEMORY ());
#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_REGISTRY */

      return 0;
    }
  else
    {
      ACE_Configuration_Heap *heap = 0;
      ACE_NEW_THROW_EX (heap,
                        ACE_Configuration_Heap,
                        CORBA::NO_MEMORY ());

      if (OPTIONS::instance ()->persistent ())
        {
          const char *filename = OPTIONS::instance ()->persistent_file ();

          if (heap->open (filename))
            {
              delete heap;
              heap = 0;

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
TAO_IFR_Server::create_repository (void)
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

  PortableServer::ServantBase_var tie_safety (impl_tie);
  safety.release ();

  this->repo_poa_->set_servant (impl_tie);

  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId ("");

  CORBA::Object_var obj =
    this->repo_poa_->create_reference_with_id (
        oid.in (),
        "IDL:omg.org/CORBA/ComponentIR/ComponentRepository:1.0"
      );

  CORBA::Repository_ptr repo_ref =
    CORBA::Repository::_narrow (obj.in ());

  // Initialize the repository.
  int status = impl->repo_init (repo_ref,
                                this->repo_poa_);

  if (status != 0)
    {
      return -1;
    }

  // Save and output the IOR string.
  this->ifr_ior_ =
    this->orb_->object_to_string (repo_ref);


  CORBA::Object_var table_object =
    this->orb_->resolve_initial_references ("IORTable");

  IORTable::Table_var adapter =
    IORTable::Table::_narrow (table_object.in ());

  if (CORBA::is_nil (adapter.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Nil IORTable\n"), -1);
    }
  else
    {
      adapter->bind ("InterfaceRepository",
                     this->ifr_ior_.in ());
    }

  // Add the repository to the ORB's table of initialized object references.
  this->orb_->register_initial_reference ("InterfaceRepository",
                                          repo_ref);

  // Write our IOR to a file.

  FILE *output_file_ =
    ACE_OS::fopen (OPTIONS::instance ()->ior_output_file (),
                   ACE_TEXT ("w"));

  if (output_file_ == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO_IFR_Server::create_repository - ")
                         ACE_TEXT ("can't open IOR output ")
                         ACE_TEXT ("file for writing\n")),
                        -1);
    }

  ACE_OS::fprintf (output_file_,
                   "%s\n",
                   this->ifr_ior_.in ());

  ACE_OS::fclose (output_file_);

  return 0;
}

// Install ior multicast handler.
int
TAO_IFR_Server::init_multicast_server (void)
{
#if defined (ACE_HAS_IP_MULTICAST)
  // Get reactor instance from TAO.
  ACE_Reactor *reactor = this->orb_->orb_core ()->reactor ();

  // See if the -ORBMulticastDiscoveryEndpoint option was specified.
  ACE_CString mde (
      TAO_ORB_Core_instance ()->orb_params ()->mcast_discovery_endpoint ()
    );

  // First, see if the user has given us a multicast port number
  // on the command-line;
  u_short port =
    TAO_ORB_Core_instance ()->orb_params ()->service_port (
                                                 TAO::MCAST_INTERFACEREPOSERVICE
                                               );

  if (port == 0)
    {
      // Check environment var. for multicast port.
      const char *port_number =
        ACE_OS::getenv ("InterfaceRepoServicePort");

      if (port_number != 0)
        port = static_cast<u_short> (ACE_OS::atoi (port_number));
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
  )
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
      switch (contained_kind)
      {
        case CORBA::dk_Provides:
        case CORBA::dk_Uses:
        case CORBA::dk_Emits:
        case CORBA::dk_Publishes:
        case CORBA::dk_Consumes:
        case CORBA::dk_Attribute:
          break;
        default:
          error_flag = 1;
          break;
      }
      break;
    default:
      break;
  }

  if (error_flag == 1)
    {
      throw
        CORBA::BAD_PARAM (CORBA::OMGVMCID | 4,
                          CORBA::COMPLETED_NO);
    }
}

void
TAO_IFR_Service_Utils::pre_exist (
    const char *id,
    name_clash_checker checker,
    ACE_Configuration_Section_Key &key,
    TAO_Repository_i *repo,
    CORBA::DefinitionKind kind
  )
{
  TAO_IFR_Service_Utils::id_exists (id,
                                    repo);

  TAO_IFR_Service_Utils::name_exists (checker,
                                      key,
                                      repo,
                                      kind);
}

void
TAO_IFR_Service_Utils::id_exists (const char *id,
                                  TAO_Repository_i *repo)
{
  // The repo id must not already exist.
  ACE_TString holder;

  if (repo->config ()->get_string_value (repo->repo_ids_key (),
                                         id,
                                         holder)
       == 0)
    {
      throw
        CORBA::BAD_PARAM (CORBA::OMGVMCID | 2,
                          CORBA::COMPLETED_NO);
    }
}

void
TAO_IFR_Service_Utils::name_exists (
    name_clash_checker checker,
    ACE_Configuration_Section_Key &key,
    TAO_Repository_i *repo,
    CORBA::DefinitionKind kind
  )
{
  int status = 0;
  ACE_TString section_name;
  u_int count = 0;
  char *stringified = 0;

  // Check the members defined elsewhere, if any.
  ACE_Configuration_Section_Key refs_key;
  status =
    repo->config ()->open_section (key,
                                   "refs",
                                   0,
                                   refs_key);

  if (status == 0)
    {
      repo->config ()->get_integer_value (refs_key,
                                          "count",
                                          count);

      for (CORBA::ULong i = 0; i < count; ++i)
        {
          ACE_Configuration_Section_Key member_key;
          stringified = TAO_IFR_Service_Utils::int_to_string (i);

          repo->config ()->open_section (refs_key,
                                         stringified,
                                         0,
                                         member_key);

          ACE_TString member_name;
          repo->config ()->get_string_value (member_key,
                                             "name",
                                             member_name);

          if ((*checker) (member_name.fast_rep ()) != 0)
            {
              throw
                CORBA::BAD_PARAM (CORBA::OMGVMCID | 3,
                                  CORBA::COMPLETED_NO);
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
      repo->config ()->get_integer_value (defns_key,
                                          "count",
                                          count);

      for (CORBA::ULong i = 0; i < count; ++i)
        {
          ACE_Configuration_Section_Key defn_key;
          stringified = TAO_IFR_Service_Utils::int_to_string (i);

          repo->config ()->open_section (defns_key,
                                         stringified,
                                         0,
                                         defn_key);

          ACE_TString defn_name;
          repo->config ()->get_string_value (defn_key,
                                             "name",
                                             defn_name);

          if ((*checker) (defn_name.fast_rep ()) != 0)
            {
              throw CORBA::BAD_PARAM (
                CORBA::OMGVMCID | 3,
                CORBA::COMPLETED_NO);
            }
        }
    }

  if (kind == CORBA::dk_Interface
      || kind == CORBA::dk_Component)
    {
      TAO_IFR_Service_Utils::check_subsection (checker,
                                               "attrs",
                                               repo->config (),
                                               key);
    }

  if (kind == CORBA::dk_Interface)
    {
      TAO_IFR_Service_Utils::check_subsection (checker,
                                               "ops",
                                               repo->config (),
                                               key);
    }

  if (kind == CORBA::dk_Component)
    {
      TAO_IFR_Service_Utils::check_subsection (checker,
                                               "provides",
                                               repo->config (),
                                               key);

      TAO_IFR_Service_Utils::check_subsection (checker,
                                               "uses",
                                               repo->config (),
                                               key);

      TAO_IFR_Service_Utils::check_subsection (checker,
                                               "emits",
                                               repo->config (),
                                               key);

      TAO_IFR_Service_Utils::check_subsection (checker,
                                               "publishes",
                                               repo->config (),
                                               key);

      TAO_IFR_Service_Utils::check_subsection (checker,
                                               "consumes",
                                               repo->config (),
                                               key);
    }
}

void
TAO_IFR_Service_Utils::check_subsection (
    TAO_IFR_Service_Utils::name_clash_checker checker,
    const char *sub_section,
    ACE_Configuration *config,
    ACE_Configuration_Section_Key &key
  )
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
          throw CORBA::BAD_PARAM (3, CORBA::COMPLETED_NO);
          return;
        }
    }
}

void
TAO_IFR_Service_Utils::valid_creation (
    CORBA::DefinitionKind container_kind,
    CORBA::DefinitionKind contained_kind,
    const char *id,
    name_clash_checker checker,
    ACE_Configuration_Section_Key &key,
    TAO_Repository_i *repo
  )
{
  TAO_IFR_Service_Utils::valid_container (container_kind,
                                          contained_kind);

  /// IDL modules can be reopened, and thus pre-exist.
  if (contained_kind == CORBA::dk_Module)
    {
      return;
    }

  TAO_IFR_Service_Utils::pre_exist (id,
                                    checker,
                                    key,
                                    repo,
                                    container_kind);
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
                                         repo);

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
TAO_IFR_Service_Utils::set_exceptions (
    ACE_Configuration *config,
    ACE_Configuration_Section_Key &key,
    const char *sub_section,
    const CORBA::ExceptionDefSeq &exceptions
  )
{
  CORBA::ULong length = exceptions.length ();

  if (length == 0)
    {
      return;
    }

  // Create new subsection because we are either creating a new entry
  // or are calling from a mutator which has already removed the old one.
  ACE_Configuration_Section_Key excepts_key;
  config->open_section (key,
                        sub_section,
                        1,
                        excepts_key);

  config->set_integer_value (excepts_key,
                             "count",
                             length);

  char *type_path = 0;

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      type_path =
        TAO_IFR_Service_Utils::reference_to_path (exceptions[i]);

      char *stringified = TAO_IFR_Service_Utils::int_to_string (i);
      config->set_string_value (excepts_key,
                                stringified,
                                type_path);
    }
}

CORBA::TypeCode_ptr
TAO_IFR_Service_Utils::gen_valuetype_tc_r (
      ACE_Configuration_Section_Key &key,
      TAO_Repository_i *repo
    )
{
  ACE_TString name;
  repo->config ()->get_string_value (key,
                                     "name",
                                     name);
  ACE_TString id;
  repo->config ()->get_string_value (key,
                                     "id",
                                     id);
  CORBA::ValueModifier tm = CORBA::VM_NONE;
  CORBA::ULong is_it = 0;

  repo->config ()->get_integer_value (key,
                                      "is_abstract",
                                      is_it);

  if (is_it)
    {
      tm = CORBA::VM_ABSTRACT;
    }
   else
    {
      repo->config ()->get_integer_value (key,
                                          "is_custom",
                                          is_it);

      if (is_it)
        {
          tm = CORBA::VM_CUSTOM;
        }
      else
        {
          repo->config ()->get_integer_value (key,
                                              "is_truncatable",
                                              is_it);

          if (is_it)
            {
              tm = CORBA::VM_TRUNCATABLE;
            }
        }
    }

  ACE_TString base_path;
  int status =
    repo->config ()->get_string_value (key,
                                       "base_value",
                                       base_path);
  CORBA::TypeCode_var base_tc = CORBA::TypeCode::_nil ();

  if (status == 0)
    {
      ACE_Configuration_Section_Key base_key;
      repo->config ()->expand_path (repo->root_key (),
                                    base_path,
                                    base_key,
                                    0);
      base_tc =
        TAO_IFR_Service_Utils::gen_valuetype_tc_r (base_key,
                                                   repo);
    }

  CORBA::ValueMemberSeq vm_seq;
  vm_seq.length (0);
  TAO_IFR_Service_Utils::fill_valuemember_seq (vm_seq,
                                               key,
                                               repo);

  return
    repo->tc_factory ()->create_value_tc (id.c_str (),
                                          name.c_str (),
                                          tm,
                                          base_tc.in (),
                                          vm_seq);
}

void
TAO_IFR_Service_Utils::fill_valuemember_seq (
    CORBA::ValueMemberSeq &vm_seq,
    ACE_Configuration_Section_Key &key,
    TAO_Repository_i *repo
  )
{
  ACE_Configuration_Section_Key members_key;
  int status =
    repo->config ()->open_section (key,
                                   "members",
                                   0,
                                   members_key);

  if (status != 0)
    {
      vm_seq.length (0);
      return;
    }

  CORBA::ULong count = 0;
  repo->config ()->get_integer_value (members_key,
                                      "count",
                                      count);
  vm_seq.length (count);
  char *stringified = 0;
  ACE_Configuration_Section_Key member_key, type_key;
  ACE_TString holder;
  CORBA::ULong access = 0;

  for (CORBA::ULong i = 0; i < count; ++i)
    {
      stringified = TAO_IFR_Service_Utils::int_to_string (i);
      repo->config ()->open_section (members_key,
                                     stringified,
                                     0,
                                     member_key);
      repo->config ()->get_string_value (member_key,
                                         "name",
                                         holder);
      vm_seq[i].name = holder.fast_rep ();
      repo->config ()->get_string_value (member_key,
                                         "id",
                                         holder);
      vm_seq[i].id = holder.fast_rep ();
      repo->config ()->get_string_value (key,
                                         "id",
                                         holder);
      vm_seq[i].defined_in = holder.fast_rep ();
      repo->config ()->get_string_value (member_key,
                                         "version",
                                         holder);
      vm_seq[i].version = holder.fast_rep ();
      repo->config ()->get_string_value (member_key,
                                         "type_path",
                                         holder);
      TAO_IDLType_i *impl =
        TAO_IFR_Service_Utils::path_to_idltype (holder,
                                                repo);
      vm_seq[i].type = impl->type_i ();

      repo->config ()->get_integer_value (member_key,
                                          "access",
                                          access);
      vm_seq[i].access = static_cast<CORBA::Visibility> (access);
    }
}

char *
TAO_IFR_Service_Utils::reference_to_path (CORBA::IRObject_ptr obj)
{
  if (CORBA::is_nil (obj))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "reference_to_path - null object; check the include "
                  "order of IDL\n"));

      throw CORBA::INTF_REPOS ();
    }

  PortableServer::ObjectId object_id;
  TAO::ObjectKey object_key =
    obj->_stubobj ()->profile_in_use ()->object_key ();
  int status = TAO_Root_POA::parse_ir_object_key (object_key,
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
  return static_cast<CORBA::DefinitionKind> (kind);
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
                                          TAO_Repository_i *repo)
{
  CORBA::DefinitionKind def_kind =
    TAO_IFR_Service_Utils::path_to_def_kind (path, repo);

  return TAO_IFR_Service_Utils::create_objref (def_kind,
                                               path.c_str (),
                                               repo);
}

CORBA::Object_ptr
TAO_IFR_Service_Utils::create_objref (CORBA::DefinitionKind def_kind,
                                      const char *obj_id,
                                      TAO_Repository_i *repo)
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
      throw CORBA::OBJECT_NOT_EXIST ();
  }

  repo_id += "1.0";

  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId (obj_id);

  PortableServer::POA_ptr poa = repo->select_poa (def_kind);

  return poa->create_reference_with_id (oid.in (),
                                        repo_id.c_str ());
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

TAO_END_VERSIONED_NAMESPACE_DECL
