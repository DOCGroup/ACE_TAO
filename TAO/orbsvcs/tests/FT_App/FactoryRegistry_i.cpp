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
  : iorOutputFile(0)
  , nsName_(0)
  , quitOnIdle_(0)
  , quitRequested_(0)

{
}

FactoryRegistry_i::~FactoryRegistry_i (void)
{
}

//////////////////////////////////////////////////////
// FactoryRegistry_i public, non-CORBA methods

int FactoryRegistry_i::parse_args (int argc, char * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:t:r:q");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'o':
      {
        iorOutputFile = get_opts.opt_arg ();
        break;
      }
      case 'q':
      {
        quitOnIdle_ = 1;
        break;
      }

      case '?':
        // fall thru
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -o <factory ior file>"
                           " -t <test replica ior file>"
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
  return identity_.c_str();
}

int FactoryRegistry_i::idle (int & result)
{
  result = 0;
  int quit = quitRequested_;
  return quit;
}


int FactoryRegistry_i::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  if (iorOutputFile != 0)
  {
    ACE_OS::unlink (iorOutputFile);
    iorOutputFile = 0;
  }
  if (nsName_ != 0)
  {
    naming_context_->unbind (this_name_
                            ACE_ENV_ARG_PARAMETER);
    nsName_ = 0;
  }
  return 0;
}

int FactoryRegistry_i::init (TAO_ORB_Manager & orbManager
  ACE_ENV_ARG_DECL)
{
  int result = 0;

  orbManager_ = & orbManager;
  orb_ = orbManager.orb();

  // Register with the ORB.
  ior_ = orbManager.activate (this
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (iorOutputFile != 0)
  {
    identity_ = "file:";
    identity_ += iorOutputFile;
    result = writeIOR (iorOutputFile, ior_);
  }
  else
  {
    // if no IOR file specified,
    // then always try to register with name service
    nsName_ = "FactoryRegistry";
  }

  if (nsName_ != 0)
  {
    identity_ = "name:";
    identity_ += nsName_;

    CORBA::Object_var naming_obj =
      orb_->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (CORBA::is_nil(naming_obj.in ())){
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%T %n (%P|%t) Unable to find the Naming Service\n"),
                        1);
    }

    naming_context_ =
      CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this_name_.length (1);
    this_name_[0].id = CORBA::string_dup (nsName_);

    naming_context_->rebind (this_name_, _this()
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
  if (registry_.find(type_id, infos) )
  {
    ACE_DEBUG(( LM_DEBUG,
      "register_factory found infos for %s", type_id
      ));
  }
  else
  {
    ACE_DEBUG(( LM_DEBUG,
      "register_factory: no infos for %s", type_id
      ));
    // Note the 5.  It's a guess about the number of factories
    // that might exist for any particular type of object.
    // todo: make it a parameter.
    ACE_NEW_NORETURN (infos, PortableGroup::FactoryInfos(5));
    if (infos == 0)
    {
      ACE_ERROR(( LM_ERROR,
        "Can't allocate infos for type: %s\n" , type_id));
      ACE_THROW (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
        CORBA::COMPLETED_NO));
    }
    registry_.bind(type_id, infos);
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
        "Found duplicate location for type: %s\n" , type_id));
      ACE_THROW (PortableGroup::MemberAlreadyPresent() );
    }
  }

  infos->length(length + 1);
  (*infos)[length] = factory_info;

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
  if (registry_.find(type_id, infos) )
  {
    ACE_DEBUG(( LM_DEBUG,
      "register_factory found infos for %s", type_id
      ));
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
          registry_.unbind (type_id);
          delete infos;
        }
      }
    }
  }
  else
  {
    ACE_ERROR(( LM_ERROR,
      "nregister_factory: no infos for %s", type_id
      ));
    ACE_THROW ( PortableGroup::MemberNotFound() );
    infos->length(0);
    registry_.bind(type_id, infos);
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
  if (registry_.unbind(type_id, infos) )
  {
    ACE_DEBUG(( LM_DEBUG,
      "unregister_factory_by_type found infos for %s", type_id
      ));
    // delete the entire set of factories for this location.
    delete infos;
  }
  else
  {
    ACE_ERROR(( LM_INFO,
      "Info: unregister_factory_by_type: no infos for %s", type_id
      ));
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
  for (RegistryType_Iterator it = registry_.begin();
       it != registry_.end();
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
    if (registry_.unbind(hitList[nHit], infos) )
    {
      delete infos;
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
        "LOGIC ERROR AT " __FILE__ " (%d): Entry to be deleted disappeared", __LINE__));
    }
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
  ACE_NEW_NORETURN(infos, ::PortableGroup::FactoryInfos() );
  if (infos.ptr() == 0)
  {
    ACE_ERROR(( LM_ERROR,
      "list_factories_by_type: Can't allocate infos for type: %s\n" , type_id));
    ACE_THROW (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
      CORBA::COMPLETED_NO));
  }

  PortableGroup::FactoryInfos * typeInfos;
  if (registry_.unbind(type_id, typeInfos) )
  {
    ACE_DEBUG(( LM_DEBUG,
      "unregister_factory_by_type found infos for %s", type_id
      ));
    (*infos) = (*typeInfos);
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
  ACE_NEW_NORETURN(infos, ::PortableGroup::FactoryInfos(registry_.current_size()) );
  if (infos.ptr() == 0)
  {
    ACE_ERROR(( LM_ERROR,
      "list_factories_by_location: Can't allocate infos\n"));
    ACE_THROW (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
      CORBA::COMPLETED_NO));
  }

  size_t count = 0;

  // iterate through the registery
  for (RegistryType_Iterator it = registry_.begin();
       it != registry_.end();
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

int FactoryRegistry_i::writeIOR(const char * outputFile, const char * ior)
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
