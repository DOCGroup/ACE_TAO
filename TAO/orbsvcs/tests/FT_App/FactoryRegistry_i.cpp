// -*- C++ -*-
//
// $Id$

#include "FactoryRegistry_i.h"

#include <ace/Get_Opt.h>
#include <ace/Vector_T.h>
#include <tao/PortableServer/ORB_Manager.h>
#include <orbsvcs/PortableGroup/PG_Operators.h> // Borrow operator == on CosNaming::Name

// Use this macro at the beginning of CORBA methods
// to aid in debugging.
#define METHOD_ENTRY(name)    \
    ACE_DEBUG (( LM_DEBUG,    \
    "Enter %s\n", #name       \
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
#define METHOD_RETURN(name)   \
    ACE_DEBUG (( LM_DEBUG,    \
      "Leave %s\n", #name     \
      ));                     \
    return /* value goes here */

FactoryRegistry_i::FactoryRegistry_i ()
  : ior_output_file_(0)
  , ns_name_(0)
  , quit_on_idle_(0)
  , quit_requested_(0)

{
}

FactoryRegistry_i::~FactoryRegistry_i (void)
{
}

//////////////////////////////////////////////////////
// FactoryRegistry_i public, non-CORBA methods

int FactoryRegistry_i::parse_args (int argc, char * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:q");
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
                           " -o <factory ior file>"
                           " -q{uit on idle}"
                           "\n",
                           argv [0]),
                          -1);
      break;
    }
  }
  // Indicates sucessful parsing of the command line
  return 0;
}

const char * FactoryRegistry_i::identity () const
{
  return this->identity_.c_str();
}

int FactoryRegistry_i::idle (int & result)
{
  result = 0;
  int quit = this->quit_requested_;
  return quit;
}


int FactoryRegistry_i::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->ior_output_file_ != 0)
  {
    ACE_OS::unlink (this->ior_output_file_);
    this->ior_output_file_ = 0;
  }
  if (this->ns_name_ != 0)
  {
    this->naming_context_->unbind (this_name_
                            ACE_ENV_ARG_PARAMETER);
    this->ns_name_ = 0;
  }
  return 0;
}

int FactoryRegistry_i::init (CORBA::ORB_var & orb  ACE_ENV_ARG_DECL)
{
  int result = 0;

  this->orb_ = orb;

  // Use the ROOT POA for now
  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references (TAO_OBJID_ROOTPOA
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (poa_object.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                      -1);

  // Get the POA object.
  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in ()
                                  ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil(this->poa_))
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to narrow the POA.\n")),
                      -1);
  }

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  // Register with the POA.

  this->object_id_ = this->poa_->activate_object (this ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  // find my IOR

  CORBA::Object_var obj =
    this->poa_->id_to_reference (object_id_.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  this->ior_ = this->orb_->object_to_string (obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;


  if (this->ior_output_file_ != 0)
  {
    this->identity_ = "file:";
    this->identity_ += this->ior_output_file_;
    result = write_ior_file (this->ior_output_file_, this->ior_);
  }
  else
  {
    // if no IOR file specified,
    // then always try to register with name service
    this->ns_name_ = "FactoryRegistry";
  }

  if (this->ns_name_ != 0)
  {
    this->identity_ = "name:";
    this->identity_ += this->ns_name_;

    CORBA::Object_var naming_obj =
      this->orb_->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (CORBA::is_nil(naming_obj.in ())){
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%T %n (%P|%t) Unable to find the Naming Service\n"),
                        1);
    }

    this->naming_context_ =
      CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this->this_name_.length (1);
    this->this_name_[0].id = CORBA::string_dup (this->ns_name_);

    this->naming_context_->rebind (this->this_name_, _this()
                            ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }

  return result;
}

//////////////////////////////////////////
// FactoryRegistry_i CORBA methods

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


void FactoryRegistry_i::register_factory (
    const char * type_id,
    const PortableGroup::FactoryInfo & factory_info
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException, PortableGroup::MemberAlreadyPresent))
{
  METHOD_ENTRY(FactoryRegistry_i::register_factory);

  PortableGroup::FactoryInfos * infos;
  if (this->registry_.find(type_id, infos) != 0)
  {
    ACE_DEBUG(( LM_DEBUG,
      "FactoryRegistry: adding new type: %s\n", type_id
      ));
    // Note the 5.  It's a guess about the number of factories
    // that might exist for any particular type of object.
    // todo: make it a parameter.
    ACE_NEW_THROW_EX (infos, PortableGroup::FactoryInfos(5),
      CORBA::NO_MEMORY());
    this->registry_.bind(type_id, infos);
  }
  // at this point infos points to the infos structure
  // for this type..
  size_t length = infos->length();
  for (size_t nInfo = 0; nInfo < length; ++nInfo)
  {
    PortableGroup::FactoryInfo & info = (*infos)[nInfo];
    if (info.the_location == factory_info.the_location)
    {
      ACE_ERROR(( LM_ERROR,
        "Attempt to register duplicate location %s for type: %s\n" ,
          ACE_static_cast(const char *, info.the_location[0].id),
          type_id));
      ACE_THROW (PortableGroup::MemberAlreadyPresent() );
    }
  }

  infos->length(length + 1);
  (*infos)[length] = factory_info;

  ACE_DEBUG(( LM_DEBUG,
    "FactoryRegistry: Added factory: %s[%d] at %s\n",
      type_id,
      ACE_static_cast(int,length + 1),
      ACE_static_cast(const char *, factory_info.the_location[0].id)
    ));

  METHOD_RETURN(FactoryRegistry_i::register_factory);
}

void FactoryRegistry_i::unregister_factory (
    const char * type_id,
    const PortableGroup::Location & location
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException, PortableGroup::MemberNotFound))
{
  METHOD_ENTRY(FactoryRegistry_i::unregister_factory);

  PortableGroup::FactoryInfos * infos;
  if (this->registry_.find(type_id, infos) == 0)
  {
    // at this point infos points to the infos structure
    // for this type..

    int found = 0;
    size_t length = infos->length();
    for (size_t nInfo = 0; !found && nInfo < length; ++nInfo)
    {
      PortableGroup::FactoryInfo & info = (*infos)[nInfo];
      if (info.the_location == location)
      {
        found = 1;

        ACE_ERROR(( LM_INFO,
          "Unregistering  factory %s at location %s\n",
            type_id,
            ACE_static_cast(const char *, location[0].id)
          ));
        if (length > 1)
        {
          while (nInfo + 1 < length)
          {
            (*infos)[nInfo] = (*infos)[nInfo + 1];
            nInfo += 1;
          }
          infos->length(nInfo);
        }
        else
        {
          assert ( length == 1 );
          this->registry_.unbind (type_id);
          delete infos;
        }
      }
    }
  }
  else
  {
    ACE_ERROR(( LM_ERROR,
      "Attempt to unregister factory for unknown type %s\n", type_id
      ));
    ACE_THROW ( PortableGroup::MemberNotFound() );
    infos->length(0);
    this->registry_.bind(type_id, infos);
  }

  if (registry_.current_size() == 0)
  {
    ACE_ERROR(( LM_INFO,
      "FactoryRegistry is idle\n"
      ));
    this->quit_requested_ |= this->quit_on_idle_;
  }

  METHOD_RETURN(FactoryRegistry_i::unregister_factory);
}

void FactoryRegistry_i::unregister_factory_by_type (
    const char * type_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_ENTRY(FactoryRegistry_i::unregister_factory_by_type);
  PortableGroup::FactoryInfos * infos;
  if (this->registry_.unbind(type_id, infos) )
  {
    ACE_DEBUG(( LM_DEBUG,
      "Unregistering all factories for type %s\n", type_id
      ));
    // delete the entire set of factories for this location.
    delete infos;
  }
  else
  {
    ACE_ERROR(( LM_INFO,
      "Info: unregister_factory_by_type: unknown type: %s\n", type_id
      ));
  }

  if (registry_.current_size() == 0)
  {
    ACE_ERROR(( LM_INFO,
      "FactoryRegistry is idle\n"
      ));
    this->quit_requested_ |= this->quit_on_idle_;
  }

  METHOD_RETURN(FactoryRegistry_i::unregister_factory_by_type);
}

void FactoryRegistry_i::unregister_factory_by_location (
    const PortableGroup::Location & location
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_ENTRY(FactoryRegistry_i::unregister_factory_by_location);

  ACE_Vector<ACE_CString> hitList;

  // iterate through the registery
  for (RegistryType_Iterator it = this->registry_.begin();
       it != this->registry_.end();
       ++it)
  {
    RegistryType_Entry & entry = *it;
    PortableGroup::FactoryInfos * infos = entry.int_id_;

    int found = 0;
    size_t length = infos->length();
    for (size_t nInfo = 0; !found && nInfo < length; ++nInfo)
    {
      PortableGroup::FactoryInfo & info = (*infos)[nInfo];
      if (info.the_location == location)
      {
        found = 1;
        if (length > 1)
        {
          while (nInfo + 1 < length)
          {
            infos[nInfo] = infos[nInfo + 1];
            nInfo += 1;
          }
          infos->length(nInfo);
        }
        else
        {
          assert ( length == 1 );
          // remember entries to be deleted
          hitList.push_back(entry.ext_id_);
        }
      }
    }
  }

  // now remove any types that became empty

  for (size_t nHit = 0; nHit < hitList.size(); ++nHit)
  {
    PortableGroup::FactoryInfos * infos;
    if (this->registry_.unbind(hitList[nHit], infos) )
    {
      delete infos;
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
        "LOGIC ERROR AT " __FILE__ " (%d): Entry to be deleted disappeared\n", __LINE__));
    }
  }
  //////////////////////////
  // If all types are gone...
  if (registry_.current_size() == 0)
  {
    ACE_ERROR(( LM_INFO,
      "FactoryRegistry is idle\n"
      ));
    this->quit_requested_ |= this->quit_on_idle_;
  }


  METHOD_RETURN(FactoryRegistry_i::unregister_factory_by_location);
}

::PortableGroup::FactoryInfos * FactoryRegistry_i::list_factories_by_type (
    const char * type_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_ENTRY(FactoryRegistry_i::list_factories_by_type);

  PortableGroup::FactoryInfos_var infos;
  ACE_NEW_THROW_EX (infos, ::PortableGroup::FactoryInfos(),
    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));

  PortableGroup::FactoryInfos * typeInfos;
  if (this->registry_.find(type_id, typeInfos) == 0)
  {
    (*infos) = (*typeInfos);
  }
  else
  {
    ACE_ERROR(( LM_INFO,
      "Info: list_factories_by_type: unknown type %s\n", type_id
      ));
  }
  METHOD_RETURN(FactoryRegistry_i::list_factories_by_type) infos._retn();
}

::PortableGroup::FactoryInfos * FactoryRegistry_i::list_factories_by_location (
    const PortableGroup::Location & location
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_ENTRY(FactoryRegistry_i::list_factories_by_location);
  ::PortableGroup::FactoryInfos_var infos;
  ACE_NEW_THROW_EX (infos, ::PortableGroup::FactoryInfos(this->registry_.current_size()),
    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));

  size_t count = 0;

  // iterate through the registery
  for (RegistryType_Iterator it = this->registry_.begin();
       it != this->registry_.end();
       ++it)
  {
    RegistryType_Entry & entry = *it;
    PortableGroup::FactoryInfos * infos = entry.int_id_;

    // iterate through the entry for this type
    int found = 0;
    size_t length = infos->length();
    for (size_t nInfo = 0; !found && nInfo < length; ++nInfo)
    {
      PortableGroup::FactoryInfo & info = (*infos)[nInfo];
      if (info.the_location == location)
      {
        found = 1;
        count += 1;
        infos->length(count);
        (*infos)[count-1] = info;
      }
    }
  }

  METHOD_RETURN(FactoryRegistry_i::list_factories_by_location) infos._retn();
}

//////////////////////////////
// Implementation methods

int FactoryRegistry_i::write_ior_file(const char * outputFile, const char * ior)
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Hash_Map_Manager <ACE_CString, PortableGroup::FactoryInfos *, MapMutex>;
  template class ACE_Hash_Map_Entry <ACE_CString, PortableGroup::FactoryInfos *>;
  template class ACE_Hash_Map_Iterator <ACE_CString, PortableGroup::FactoryInfos *, MapMutex>;
  template class ACE_Vector<ACE_CString>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate ACE_Hash_Map_Manager <ACE_CString, PortableGroup::FactoryInfos *, MapMutex>
# pragma instantiate ACE_Hash_Map_Entry <ACE_CString, PortableGroup::FactoryInfos *>
# pragma instantiate ACE_Hash_Map_Iterator <ACE_CString, PortableGroup::FactoryInfos *, MapMutex>
# pragma instantiate ACE_Vector<ACE_CString>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
