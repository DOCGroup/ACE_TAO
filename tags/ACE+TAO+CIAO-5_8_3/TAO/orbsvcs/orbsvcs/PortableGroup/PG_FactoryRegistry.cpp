// -*- C++ -*-
// $Id$

#include "orbsvcs/PortableGroup/PG_FactoryRegistry.h"

#include "ace/Get_Opt.h"
#include "ace/Vector_T.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Auto_Ptr.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/PortableServer/POAManagerC.h"
#include "orbsvcs/PortableGroup/PG_Operators.h" // operator == on CosNaming::Name

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Use this macro at the beginning of CORBA methods
// to aid in debugging.
#define METHOD_ENTRY(name)            \
  if (TAO_debug_level <= 6){} else    \
    ACE_DEBUG (( LM_DEBUG,            \
    "Enter %C\n", #name               \
      ))

// Use this macro to return from CORBA methods
// to aid in debugging.  Note that you can specify
// the return value after the macro, for example:
// METHOD_RETURN(Plugh::plover) xyzzy; is equivalent
// to return xyzzy;
// METHOD_RETURN(Plugh::troll); is equivalent to
// return;
// WARNING: THIS GENERATES TWO STATEMENTS!!! THE FOLLOWING
// will not do what you want it to:
//  if (cave_is_closing) METHOD_RETURN(Plugh::pirate) aarrggh;
// Moral:  Always use braces.
#define METHOD_RETURN(name)           \
  if (TAO_debug_level <= 6){} else    \
    ACE_DEBUG (( LM_DEBUG,            \
      "Leave %C\n", #name             \
      ));                             \
  return /* value goes here */

TAO::PG_FactoryRegistry::PG_FactoryRegistry (const char * name)
  : identity_(name)
  , orb_ (0)
  , poa_ (0)
  , object_id_ (0)
  , this_obj_ (0)
  , ior_output_file_(0)
  , ns_name_("")
  , naming_context_(0)
  , this_name_(1)
  , quit_on_idle_(0)
  , quit_state_(LIVE)
  , linger_(0)
{
}

TAO::PG_FactoryRegistry::~PG_FactoryRegistry (void)
{
}

//////////////////////////////////////////////////////
// PG_FactoryRegistry public, non-CORBA methods

int TAO::PG_FactoryRegistry::parse_args (int argc, ACE_TCHAR * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:n:q"));
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'o':
      {
        this->ior_output_file_ = get_opts.opt_arg ();
        break;
      }
      case 'n':
      {
        this->ns_name_ = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg());
        break;
      }
      case 'q':
      {
        this->quit_on_idle_ = 1;
        break;
      }

      case '?':
        // fall thru
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -o <registry ior file>"
                           " -n <name to use to register with name service>"
                           " -q{uit on idle}"
                           "\n",
                           argv [0]),
                          -1);
      break;
    }
  }
  // Indicates successful parsing of the command line
  return 0;
}

const char * TAO::PG_FactoryRegistry::identity () const
{
  return this->identity_.c_str();
}

void TAO::PG_FactoryRegistry::_remove_ref (void)
{
  this->quit_state_ = GONE;
}

int TAO::PG_FactoryRegistry::idle (int & result)
{
  result = 0;
  int quit = 0;
  if (this->quit_state_ == GONE)
  {
    if (linger_ < 2)
    {
      ++linger_;
    }
    else
    {
      quit = 1;
    }
  }
  return quit;
}


int TAO::PG_FactoryRegistry::fini (void)
{
  if (this->ior_output_file_ != 0)
  {
    ACE_OS::unlink (this->ior_output_file_);
    this->ior_output_file_ = 0;
  }
  if (this->ns_name_.length () != 0)
  {
    this->naming_context_->unbind (this_name_);
    this->ns_name_ = "";
  }
  return 0;
}


void TAO::PG_FactoryRegistry::init (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
{
  ACE_ASSERT (CORBA::is_nil (this->orb_.in ()));
  ACE_ASSERT (CORBA::is_nil (this->poa_.in ()));
  this->orb_ = CORBA::ORB::_duplicate (orb);
  this->poa_ = PortableServer::POA::_duplicate (poa);
  ACE_ASSERT ( ! CORBA::is_nil (this->orb_.in ()));
  ACE_ASSERT ( ! CORBA::is_nil (this->poa_.in ()));

  // Register with the POA.
  this->object_id_ = this->poa_->activate_object (this);

  // find my identity as a corba object
  this->this_obj_ =
    this->poa_->id_to_reference (object_id_.in ());

  // and create a ior string
  this->ior_ = this->orb_->object_to_string (this->this_obj_.in ());

}

int TAO::PG_FactoryRegistry::init (CORBA::ORB_ptr orb)
{
  int result = 0;

  this->orb_ = CORBA::ORB::_duplicate (orb);

  // Use the ROOT POA for now
  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references (TAO_OBJID_ROOTPOA);

  if (CORBA::is_nil (poa_object.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                      -1);

  // Get the POA object.
  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in ());


  if (CORBA::is_nil (this->poa_.in()))
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to narrow the POA.\n")),
                      -1);
  }

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager ();

  poa_manager->activate ();

  // Register with the POA.
  this->object_id_ = this->poa_->activate_object (this);

  // find my identity as a corba object
  this->this_obj_ =
    this->poa_->id_to_reference (object_id_.in ());


  // and create a ior string
  this->ior_ = this->orb_->object_to_string (this->this_obj_.in ());

  if (this->ior_output_file_ != 0)
  {
    this->identity_ = "file:";
    this->identity_ += ACE_TEXT_ALWAYS_CHAR(this->ior_output_file_);
    result = write_ior_file (this->ior_output_file_,
                             this->ior_.in ());
  }

  if (this->ns_name_.length () != 0)
  {
    this->identity_ = "name:";
    this->identity_ += this->ns_name_;

    CORBA::Object_var naming_obj =
      this->orb_->resolve_initial_references ("NameService");

    if (CORBA::is_nil(naming_obj.in ())){
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%T %n (%P|%t) Unable to find the Naming Service\n"),
                        1);
    }

    this->naming_context_ =
      CosNaming::NamingContext::_narrow (naming_obj.in ());

    this->this_name_.length (1);
    this->this_name_[0].id = CORBA::string_dup (this->ns_name_.c_str ());

    this->naming_context_->rebind (this->this_name_, this->this_obj_.in());
  }

  return result;
}


::PortableGroup::FactoryRegistry_ptr TAO::PG_FactoryRegistry::reference()
{
  // narrow and duplicate
  return ::PortableGroup::FactoryRegistry::_narrow(this->this_obj_.in ());
}


//////////////////////////////////////////
// PG_FactoryRegistry CORBA methods

/* Reference:info
  typedef CosNaming::Name Name;
  typedef Name Location;
  struct FactoryInfo {
    GenericFactory the_factory;
    Location the_location;
    Criteria the_criteria;
  };
  typedef sequence<FactoryInfo> FactoryInfos;
*/

TAO::PG_FactoryRegistry::RoleInfo::RoleInfo(size_t estimated_number_entries)
  : infos_(estimated_number_entries)
{
}

void TAO::PG_FactoryRegistry::register_factory (
    const char * role,
    const char * type_id,
    const PortableGroup::FactoryInfo & factory_info)
{
  METHOD_ENTRY(TAO::PG_FactoryRegistry::register_factory);

  RoleInfo * role_info = 0;
  auto_ptr<RoleInfo> safe_entry;
  if (this->registry_.find(role, role_info) != 0)
    {
      ACE_DEBUG(( LM_DEBUG,
                  "%s: adding new role: %s:%s\n",
                  this->identity_.c_str(), role, type_id));

      // Note the 5.  It's a guess about the number of factories
      // that might exist for any particular role object.
      // todo: make it a parameter.
      ACE_NEW_THROW_EX (role_info,
                        RoleInfo(5),
                        CORBA::NO_MEMORY());

      ACE_AUTO_PTR_RESET (safe_entry, role_info, RoleInfo);
      role_info->type_id_ = type_id;
    }
  else
    {
      if (role_info->type_id_ != type_id)
        {
          throw PortableGroup::TypeConflict();
        }
    }

  PortableGroup::FactoryInfos & infos = role_info->infos_;;
  size_t length = infos.length();
  for (size_t nInfo = 0; nInfo < length; ++nInfo)
    {
      PortableGroup::FactoryInfo & info = infos[nInfo];
      if (info.the_location == factory_info.the_location)
        {
          ACE_ERROR(( LM_ERROR,
                      "%s: Attempt to register duplicate location %s for role: %s\n" ,
                      this->identity_.c_str(),
                      static_cast<const char *> (info.the_location[0].id),
          role));
      throw PortableGroup::MemberAlreadyPresent();
    }
  }

  infos.length(length + 1);
  infos[length] = factory_info;

  if (safe_entry.get() != 0)
  {
    this->registry_.bind(role, safe_entry.release());
  }

  ACE_DEBUG(( LM_DEBUG,
    "%s: Added factory: [%d] %s@%s\n",
      this->identity_.c_str(),
      static_cast<int> (length + 1),
      role,
      static_cast<const char *> (factory_info.the_location[0].id)
    ));

  METHOD_RETURN(TAO::PG_FactoryRegistry::register_factory);
}

void TAO::PG_FactoryRegistry::unregister_factory (
    const char * role,
    const PortableGroup::Location & location)
{
  METHOD_ENTRY(TAO::PG_FactoryRegistry::unregister_factory);

  RoleInfo * role_info = 0;
  if (this->registry_.find(role, role_info) == 0)
  {
    PortableGroup::FactoryInfos & infos = role_info->infos_;
    int found = 0;
    size_t length = infos.length();
    for (size_t nInfo = 0; !found && nInfo < length; ++nInfo)
    {
      PortableGroup::FactoryInfo & info = infos[nInfo];
      if (info.the_location == location)
      {
        found = 1;

        ACE_ERROR(( LM_INFO,
          "%s: Unregistering  factory %s@%s\n",
            this->identity_.c_str(),
            role,
            static_cast<const char *> (location[0].id)
          ));
        if (length > 1)
        {
          // if this is not the last entry
          if (nInfo + 1 < length)
          {
            // move last entry into newly-emptied slot
            infos[nInfo] = infos[length - 1];
            nInfo = length -1;
          }
          infos.length(nInfo);
        }
        else
        {
          ACE_ASSERT ( length == 1 );
          if (this->registry_.unbind (role) == 0)
          {
            ACE_DEBUG(( LM_INFO,
              "%s: No more factories registered for %s\n",
              this->identity_.c_str(),
              role
              ));
            delete role_info;
          }
          else
          {
            ACE_ERROR ((LM_ERROR,
              "%s: LOGIC ERROR AT " __FILE__ " (%d): Entry to be deleted disappeared\n",
              this->identity_.c_str(),
              __LINE__));
          }
        }
      }
    }
  }
  else
  {
    ACE_ERROR(( LM_ERROR,
      "%s, Attempt to unregister factory for unknown role %s\n",
      this->identity_.c_str(),
      role
      ));
    throw PortableGroup::MemberNotFound();
  }

  //////////////////////
  // request complete
  // check quit-on-idle
  if (registry_.current_size() == 0 && quit_state_ == LIVE)
  {
    ACE_ERROR(( LM_INFO,
      "%s is idle\n",
      identity()
      ));
    if (quit_on_idle_)
    {
        this->poa_->deactivate_object (this->object_id_.in ());
        quit_state_ = DEACTIVATED;
    }
  }

  METHOD_RETURN(TAO::PG_FactoryRegistry::unregister_factory);
}

void TAO::PG_FactoryRegistry::unregister_factory_by_role (
    const char * role
  )
{
  METHOD_ENTRY(TAO::PG_FactoryRegistry::unregister_factory_by_role);

  RoleInfo * role_info = 0;
  if (this->registry_.unbind(role, role_info) == 0)
  {
    ACE_DEBUG(( LM_DEBUG,
      "%s: Unregistering all factories for role %s\n",
      this->identity_.c_str(),
      role
      ));
    // delete the entire set of factories for this location.
    delete role_info;
  }
  else
  {
    ACE_ERROR(( LM_INFO,
      "%s: Unregister_factory_by_role: unknown role: %s\n",
      this->identity_.c_str(),
      role
      ));
  }

  /////////////////////
  // Function complete
  // check quit options
  if (registry_.current_size() == 0 && quit_state_ == LIVE)
  {
    ACE_ERROR(( LM_INFO,
      "%s is idle\n",
      identity()
      ));
    if (quit_on_idle_)
    {
        this->poa_->deactivate_object (this->object_id_.in ());
        quit_state_ = DEACTIVATED;
    }
  }

  METHOD_RETURN(TAO::PG_FactoryRegistry::unregister_factory_by_role);
}

void TAO::PG_FactoryRegistry::unregister_factory_by_location (
    const PortableGroup::Location & location)
{
  METHOD_ENTRY(TAO::PG_FactoryRegistry::unregister_factory_by_location);

  ////////////////////////////////////////////
  // a vector of roles that need to be deleted.
  ACE_Vector<ACE_CString> emptyRoles;

  // iterate through the registery
  for (RegistryType_Iterator it = this->registry_.begin();
       it != this->registry_.end();
       ++it)
  {
    RegistryType_Entry & entry = *it;
    ACE_CString & role = entry.ext_id_;
    RoleInfo * role_info =  entry.int_id_;

    PortableGroup::FactoryInfos & infos = role_info->infos_;
    // ACE_ERROR((LM_INFO,  "unregister_factory_by_location: Checking role %s\n", role.c_str()  ));

    int found = 0;
    size_t length = infos.length();
    for (size_t nInfo = 0; !found && nInfo < length; ++nInfo)
    {
      PortableGroup::FactoryInfo & info = infos[nInfo];
      if (info.the_location == location)
      {

        ACE_ERROR((LM_INFO,
          "%s: Unregister_factory_by_location: Removing: [%d] %s@%s\n",
          this->identity_.c_str(),
          static_cast<int> (nInfo),
          role.c_str(),
          static_cast<const char *> (location[0].id)
          ));
        found = 1;
        if (length > 1)
        {
          while (nInfo + 1 < length)
          {
            ACE_ERROR((LM_INFO,
              "%s: Unregister_factory_by_location: Move: [%d] %s to [%d]\n",
              this->identity_.c_str(),
              (int)nInfo + 1, role.c_str(), (int)nInfo
              ));
            infos[nInfo] = infos[nInfo + 1];
            nInfo += 1;
          }
          ACE_ERROR((LM_INFO,
            "%s: unregister_factory_by_location: New length [%d] %s\n",
            this->identity_.c_str(),
            (int)nInfo, role.c_str()
            ));
          infos.length(nInfo);
        }
        else
        {
          ACE_ERROR((LM_INFO,
            "%s: Removed all entries for %s\n",
            this->identity_.c_str(),
            role.c_str()
            ));
          ACE_ASSERT ( length == 1 );
          // remember entries to be deleted
          emptyRoles.push_back(entry.ext_id_);
        }
      }
    }
  }

  // now remove any roles that became empty

  for (size_t nRole = 0; nRole < emptyRoles.size(); ++nRole)
  {
    ACE_ERROR((LM_INFO,
      "%s: Remove role %s\n",
      this->identity_.c_str(),
      emptyRoles[nRole].c_str()
      ));
    RoleInfo * role_info;
    if (this->registry_.unbind(emptyRoles[nRole], role_info) == 0)
    {
      delete role_info;
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
        "%s: LOGIC ERROR AT " __FILE__ " (%d): Role to be deleted disappeared\n",
        this->identity_.c_str(),
        __LINE__));
    }
  }
  //////////////////////////
  // If all types are gone...
  if (registry_.current_size() == 0 && quit_state_ == LIVE)
  {
    ACE_ERROR(( LM_INFO,
      "%s is idle\n",
      identity()
      ));
    if (quit_on_idle_)
    {
        this->poa_->deactivate_object (this->object_id_.in ());
        quit_state_ = DEACTIVATED;
    }
  }

  METHOD_RETURN(TAO::PG_FactoryRegistry::unregister_factory_by_location);
}

::PortableGroup::FactoryInfos * TAO::PG_FactoryRegistry::list_factories_by_role (
    const char * role,
    CORBA::String_out type_id)
{
  METHOD_ENTRY(TAO::PG_FactoryRegistry::list_factories_by_role);

  // allocate stucture to be returned.
  PortableGroup::FactoryInfos_var result = 0;
  ACE_NEW_THROW_EX (result, ::PortableGroup::FactoryInfos(),
    CORBA::NO_MEMORY (TAO::VMCID, CORBA::COMPLETED_NO));


  RoleInfo * role_info = 0;
  if (this->registry_.find(role, role_info) == 0)
  {
    type_id =  CORBA::string_dup(role_info->type_id_.c_str());
    (*result) = role_info->infos_;
  }
  else
  {
    type_id = CORBA::string_dup("");
    ACE_ERROR(( LM_INFO,
      "%s: list_factories_by_role: unknown role %s\n",
      this->identity_.c_str(),
      role
      ));
  }
  METHOD_RETURN(TAO::PG_FactoryRegistry::list_factories_by_role) result._retn();
}

::PortableGroup::FactoryInfos * TAO::PG_FactoryRegistry::list_factories_by_location (
    const PortableGroup::Location & location
  )
{
  METHOD_ENTRY(TAO::PG_FactoryRegistry::list_factories_by_location);
  ::PortableGroup::FactoryInfos_var result;
  ACE_NEW_THROW_EX (result, ::PortableGroup::FactoryInfos(this->registry_.current_size()),
    CORBA::NO_MEMORY (TAO::VMCID, CORBA::COMPLETED_NO));


  size_t result_length = 0;

  // iterate through the registery
  for (RegistryType_Iterator it = this->registry_.begin();
       it != this->registry_.end();
       ++it)
  {
    RegistryType_Entry & entry = *it;
    RoleInfo * role_info = entry.int_id_;

    PortableGroup::FactoryInfos & found_infos = role_info->infos_;
    // iterate through the entry for this type
    int found = 0;
    size_t length = found_infos.length();
    for (size_t nInfo = 0; !found && nInfo < length; ++nInfo)
    {
      PortableGroup::FactoryInfo & info = found_infos[nInfo];
      if (info.the_location == location)
      {
        found = 1;
        result_length += 1;
        result->length(result_length);
        (*result)[result_length-1] = info;
      }
    }
  }

  METHOD_RETURN(TAO::PG_FactoryRegistry::list_factories_by_location) result._retn();
}

//////////////////////////////
// Implementation methods

int TAO::PG_FactoryRegistry::write_ior_file(const ACE_TCHAR * outputFile, const char * ior)
{
  int result = -1;
  FILE* out = ACE_OS::fopen (outputFile, "w");
  if (out)
  {
    ACE_OS::fprintf (out, "%s", ior);
    ACE_OS::fclose (out);
    result = 0;
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
      "Open failed for %s\n", outputFile
    ));
  }
  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
