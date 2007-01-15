/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_ReplicaFactory_i.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#include "FT_ReplicaFactory_i.h"
#include "FT_TestReplica_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/PortableGroupC.h"
#include "orbsvcs/PortableGroup/PG_Property_Set.h"

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


static const char * criterion_initial_value = "INITIAL_VALUE";

//////////////////////////////////////////////////////
// FT_ReplicaFactory_i  Construction/destruction

FT_ReplicaFactory_i::FT_ReplicaFactory_i ()
  : internals_ ()
  , orb_ (CORBA::ORB::_nil ())
  , poa_ (PortableServer::POA::_nil ())
  , object_id_ ()
  , ior_ ()
  , ior_output_file_ (0)
  , identity_ ()
  , have_replication_manager_(0)
  , replication_manager_(0)
  , factory_registry_ior_(0)
  , factory_registry_ (0)
  , registered_(0)
  , test_output_file_(0)
  , ns_name_(0)
  , naming_context_ (CosNaming::NamingContext::_nil ())
  , this_name_ ()
  , roles_ ()
  , location_ ("unknown")
  , quit_on_idle_ (0)
  , unregister_by_location_ (0)
  , replicas_ ()
  , empty_slots_(0)
  , quit_requested_(0)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  char const * repo_id =
    FT_TEST::_tc_TestReplica->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "TestReplica type_id: %s\n",
              repo_id));

//  ACE_DEBUG((LM_DEBUG, "Hobbit type_id: %s\n", FT_TEST::_tc_Hobbit->id() ));
//  ACE_DEBUG((LM_DEBUG, "Elf type_id: %s\n", FT_TEST::_tc_Elf->id() ));
//  ACE_DEBUG((LM_DEBUG, "Human type_id: %s\n", FT_TEST::_tc_Human->id() ));
}


FT_ReplicaFactory_i::~FT_ReplicaFactory_i ()
{
  //scope the guard
  {
    InternalGuard guard (this->internals_);

    // be sure all replicas are gone
    // before this object disappears
    shutdown_i ();
  }
}

////////////////////////////////////////////
// FT_ReplicaFactory_i private methods

CORBA::ULong FT_ReplicaFactory_i::allocate_id()
{
  // assume mutex is locked
  CORBA::ULong id = this->replicas_.size();
  if (this->empty_slots_ != 0)
  {
    for(CORBA::ULong pos = 0; pos < id; ++pos)
    {
      if (this->replicas_[pos] == 0)
      {
        id = pos;
      }
    }
  }
  else
  {
    this->replicas_.push_back(0);
    this->empty_slots_ += 1;
  }
  return id;
}

void FT_ReplicaFactory_i::shutdown_i()
{
  // assume mutex is locked
  for (size_t nReplica = 0; nReplica < this->replicas_.size(); ++nReplica)
  {
    FT_TestReplica_i * replica = this->replicas_[nReplica];
    if (replica != 0)
    {
      replica->request_quit();
    }
  }
}

int FT_ReplicaFactory_i::write_ior(const char * outputFile, const char * ior)
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

//////////////////////////////////////////////////////
// FT_ReplicaFactory_i public, non-CORBA methods

int FT_ReplicaFactory_i::parse_args (int argc, char * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:n:f:i:l:t:qu");
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
        this->ns_name_ = get_opts.opt_arg ();
        break;
      }
      case 'f':
      {
        this->factory_registry_ior_ = get_opts.opt_arg ();
        break;
      }
      case 'i':
      {
        this->roles_.push_back(get_opts.opt_arg ());
        break;
      }
      case 'l':
      {
        this->location_ = get_opts.opt_arg ();
        break;
      }
      case 'q':
      {
        this->quit_on_idle_ = 1;
        break;
      }
      case 'u':
      {
        this->unregister_by_location_ = 1;
        break;
      }

      case 't':
      {
        this->test_output_file_ = get_opts.opt_arg ();
        break;
      }

      case '?':
        // fall thru
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s \n"
                           " -o <factory ior file>\n"
                           " -n <naming service registration name>\n"
                           " -f <factory registry ior file>\n"
                           " -i <registration: role>\n"
                           " -l <registration: location>\n"
                           " -t <test replica ior file>\n"
                           " -u{nregister by location}\n"
                           " -q{uit on idle}\n",
                           argv [0]),
                          -1);
      break;
    }
  }
  // Indicates sucessful parsing of the command line
  return 0;
}

const char * FT_ReplicaFactory_i::location () const
{
  return this->location_;
}

const char * FT_ReplicaFactory_i::identity () const
{
  return this->identity_.c_str();
}

int FT_ReplicaFactory_i::idle (int & result ACE_ENV_ARG_DECL)
{
  result = 0;
  size_t replicaCount = this->replicas_.size();
  if (replicaCount != this->empty_slots_)
  {
    for (size_t nReplica = 0; result == 0 && nReplica < replicaCount; ++nReplica)
    {
      FT_TestReplica_i * replica = this->replicas_[nReplica];
      if (replica != 0)
      {
        // give the replica's idle processing a change
        // ignore the return status (the replica should shut itself down
        // unless result is non-zero.
        // non-zero result means panic.
        replica->idle(result ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
      }
    }
  }

  int quit = (this->quit_requested_ || result != 0);
  if (!quit && this->replicas_.size() == this->empty_slots_)
  {
/*  if you re-enable this, add some kind of throttle to avoid noise.
    ACE_ERROR (( LM_ERROR,
      "ReplicaFactory is idle.\n"
      ));
*/
    if (this->quit_on_idle_ && this->empty_slots_ != 0)
    {
      ACE_ERROR (( LM_ERROR,
        "%s exits due to quit on idle option.\n",
        identity()
        ));
      quit = 1;
    }
  }

  return quit;
}



int FT_ReplicaFactory_i::init (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL)
{
  int result = 0;

  this->orb_ = CORBA::ORB::_duplicate (orb);

  // Use the ROOT POA for now
  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references (TAO_OBJID_ROOTPOA
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (poa_object.in ()))
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                      -1);
  }

  // Get the POA object.
  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in ()
                                  ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);
  if (CORBA::is_nil(this->poa_.in ()))
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to narrow the POA.\n")),
                      -1);
  }

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Register with the POA.

  this->object_id_ = this->poa_->activate_object (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var this_obj =
    this->poa_->id_to_reference (object_id_.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->ior_ = this->orb_->object_to_string (this_obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (this->factory_registry_ior_ != 0)
  {
    if (ACE_OS::strcmp (this->factory_registry_ior_, "none") != 0)
    {
      CORBA::Object_var reg_obj = this->orb_->string_to_object(factory_registry_ior_
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      this->factory_registry_ = ::PortableGroup::FactoryRegistry::_narrow(reg_obj.in ());
      if (CORBA::is_nil(this->factory_registry_.in ()))
      {
        ACE_ERROR (( LM_ERROR,
           "Can't resolve Factory Registry IOR %s\n",
           this->factory_registry_ior_
           ));
        result = -1;
      }
    }
  }
  else // no -f option.  Try RIR(RM)
  {
    ///////////////////////////////
    // Find the ReplicationManager
    ACE_TRY_NEW_ENV
    {
      CORBA::Object_var rm_obj = orb->resolve_initial_references("ReplicationManager" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->replication_manager_ = ::FT::ReplicationManager::_narrow(rm_obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (!CORBA::is_nil (replication_manager_.in ()))
      {
        this->have_replication_manager_ = 1;
        // empty criteria
        ::PortableGroup::Criteria criteria;
        this->factory_registry_ = this->replication_manager_->get_factory_registry(criteria  ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
        if (CORBA::is_nil (this->factory_registry_.in ()))
        {
          ACE_ERROR ((LM_ERROR,"ReplicaFactory: ReplicationManager failed to return FactoryRegistry.  Factory will not be registered.\n" ));
        }
      }
      else
      {
        this->factory_registry_ =  ::PortableGroup::FactoryRegistry::_narrow(rm_obj.in()  ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
        if (!CORBA::is_nil(this->factory_registry_.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,"Found a FactoryRegistry DBA ReplicationManager\n" ));
        }
        else
        {
          ACE_ERROR ((LM_ERROR,"ReplicaFactory: Can't resolve ReplicationManager.\n" ));
        }
      }
    }
    ACE_CATCHANY
    {
      if (this->test_output_file_ == 0) // ignore if this is a test run
      {
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
          "ReplicaFactory: Exception resolving ReplicationManager. Factory will not be registered.\n" );
      }
    }
    ACE_ENDTRY;

  }

  if ( ! CORBA::is_nil (this->factory_registry_.in ()))
  {
    size_t roleCount = roles_.size();
    for (size_t nRole = 0; nRole < roleCount; ++nRole)
    {
      const char * roleName = this->roles_[nRole].c_str();

      PortableGroup::FactoryInfo info;
      info.the_factory = ::PortableGroup::GenericFactory::_narrow(this_obj.in ());
      info.the_location.length(1);
      info.the_location[0].id = CORBA::string_dup(this->location_);
      info.the_criteria.length(1);
      info.the_criteria[0].nam.length(1);
      info.the_criteria[0].nam[0].id = CORBA::string_dup(PortableGroup::role_criterion);
      info.the_criteria[0].val <<= CORBA::string_dup(roleName);

      ACE_ERROR (( LM_INFO,
         "Factory: %s@%s registering with factory registry\n",
         roleName,
         location_
         ));

      char const * replica_repository_id =
        FT_TEST::_tc_TestReplica->id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      this->factory_registry_->register_factory(
        roleName,
        replica_repository_id,
        info
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }
    this->registered_ = 1;
  }

  int identified = 0; // bool

  if (this->roles_.size() > 0)
  {
    this->identity_ = "Factory";
    if (this->location_ != 0)
    {
      this->identity_ += "@";
      this->identity_ += this->location_;
    }
    identified = 1;
  }

  if (this->ior_output_file_ != 0)
  {
    if (!identified)
    {
      this->identity_ = "file:";
      this->identity_ += this->ior_output_file_;
      // note: don't set identified--ns identity overrides file identitiy
    }
    result = write_ior (this->ior_output_file_, this->ior_. in ());
  }
  else
  {
    if (this->registered_)
    {
      // if we didn't register with a FactoryRegistry
      // and no IOR file specified,
      // then always try to register with name service
      this->ns_name_ = "FT_ReplicaFactory";
    }
  }

  if (this->ns_name_ != 0)
  {
    if (!identified)
    {
      this->identity_ = "name:";
      this->identity_ += this->ns_name_;
    }

    CORBA::Object_var naming_obj =
      this->orb_->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    if (CORBA::is_nil(naming_obj.in ())){
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%T %n (%P|%t) Unable to find the Naming Service\n"),
                        1);
    }

    this->naming_context_ =
      CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    this->this_name_.length (1);
    this->this_name_[0].id = CORBA::string_dup (this->ns_name_);

    this->naming_context_->rebind (this->this_name_, this_obj.in()  // CORBA::Object::_duplicate(this_obj)
                            ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
  }

  // if we're testing.  Create a replica at startup time
  if (this->test_output_file_ != 0)
  {
    // shouldn't be necessary, but create_replica assumes this
    InternalGuard guard (this->internals_);
    FT_TestReplica_i * replica = create_replica ("test" ACE_ENV_ARG_PARAMETER);

    PortableServer::POA_var poa = replica->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    ::CORBA::Object_var replica_obj = poa->servant_to_reference(replica ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    ::CORBA::String_var replicaIOR = this->orb_->object_to_string(replica_obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    write_ior (this->test_output_file_, replicaIOR.in ());
  }

  return result;
}

int FT_ReplicaFactory_i::fini (ACE_ENV_SINGLE_ARG_DECL)
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
  ACE_CHECK_RETURN (-1);
    this->ns_name_ = 0;
  }

  if (registered_)
  {
    registered_ = 0;

    if (this->unregister_by_location_)
    {
      ACE_ERROR (( LM_INFO,
         "%s: unregistering all factories at %s\n",
         identity(),
         location_
         ));

      PortableGroup::Location location(1);
      location.length(1);
      location[0].id = CORBA::string_dup(location_);
      this->factory_registry_->unregister_factory_by_location (
              location
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }
    else
    {
      size_t roleCount = roles_.size();
      for (size_t nRole = 0; nRole < roleCount; ++nRole)
      {
        const char * roleName = this->roles_[nRole].c_str();
        ACE_ERROR (( LM_INFO,
           "Factory for: %s@%s unregistering from factory registry\n",
           roleName,
           location_
           ));

        PortableGroup::Location location(1);
        location.length(1);
        location[0].id = CORBA::string_dup(location_);
        this->factory_registry_->unregister_factory (
                roleName,
                location
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (-1);
      }
    }
  }

  return 0;
}


void FT_ReplicaFactory_i::remove_replica(CORBA::ULong id, FT_TestReplica_i * replica ACE_ENV_ARG_DECL)
{
  InternalGuard guard (this->internals_);
  if (id < this->replicas_.size())
  {
    if(this->replicas_[id] == replica)
    {
      replica->fini(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      delete replica;
      this->replicas_[id] = 0;
      this->empty_slots_ += 1;
    }
    else
    {
      ACE_ERROR (( LM_ERROR,
        "Remove replica %d mismatch.\n",
        static_cast<int> (id)
        ));
    }
  }
  else
  {
    ACE_ERROR (( LM_ERROR,
      "Attempt to remove invalid replica %d. Limit %d.\n",
      static_cast<int> (id),
      static_cast<int> (this->replicas_.size())
      ));
  }
}

//////////////////////////////////////////
// FT_ReplicaFactory_i CORBA methods

CORBA::Object_ptr FT_ReplicaFactory_i::create_object (
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , PortableGroup::NoFactory
    , PortableGroup::ObjectNotCreated
    , PortableGroup::InvalidCriteria
    , PortableGroup::InvalidProperty
    , PortableGroup::CannotMeetCriteria
  ))
{
  METHOD_ENTRY(FT_ReplicaFactory_i::create_object);
  ACE_UNUSED_ARG (type_id);
  InternalGuard guard (this->internals_);

  ::TAO::PG_Property_Set decoder (the_criteria);

  // boolean, becomes true if a required parameter is missing
  int missingParameter = 0;
  const char * missingParameterName = 0;

  CORBA::Long initialValue = 0;
  if (! ::TAO::find (decoder, criterion_initial_value, initialValue) )
  {
    // not required.  Otherwise:
    // missingParameter = 1;
    // missingParameterName = criterion_initial_value;
  }

  const char * role = "replica";
  if (! ::TAO::find (decoder, PortableGroup::role_criterion, role) )
  {
    ACE_ERROR((LM_INFO,
      "Property \"%s\" not found?\n", PortableGroup::role_criterion
      ));
    // not required.  Otherwise:
    // missingParameter = 1;
    // missingParameterName = PortableGroup::role_criterion;
  }

  if (missingParameter)
  {
    ACE_ERROR ((LM_ERROR,
      "Throwing 'InvalidCriteria' due to missing %s\n",
      missingParameterName
      ));
    ACE_THROW_RETURN ( PortableGroup::InvalidCriteria(), 0);
  }

  FT_TestReplica_i * replica = create_replica(role ACE_ENV_ARG_PARAMETER);
  if (replica == 0)
  {
    ACE_ERROR ((LM_ERROR,
      "New Replica_i returned NULL.  Throwing ObjectNotCreated.\n"
      ));
    ACE_THROW_RETURN ( PortableGroup::ObjectNotCreated(), 0);
  }

  ACE_NEW_THROW_EX ( factory_creation_id,
    PortableGroup::GenericFactory::FactoryCreationId,
    PortableGroup::ObjectNotCreated());
  CORBA::ULong factory_id = replica->factory_id();
  (*factory_creation_id) <<= factory_id;

  ACE_ERROR ((LM_INFO,
    "Created %s@%s#%d.\n", role, this->location_, static_cast<int> (factory_id)
    ));


  ::CORBA::Object_ptr replica_obj =
    replica->_default_POA(ACE_ENV_SINGLE_ARG_PARAMETER)->servant_to_reference(replica);
  METHOD_RETURN(FT_ReplicaFactory_i::create_object) replica_obj->_duplicate(replica_obj);
}

FT_TestReplica_i * FT_ReplicaFactory_i::create_replica(const char * name ACE_ENV_ARG_DECL)
{
  // assume mutex is locked
  CORBA::ULong factoryId = allocate_id();

  FT_TestReplica_i * pFTReplica = 0;

  ACE_NEW_NORETURN(pFTReplica, FT_TestReplica_i(
    this,
    name,
    factoryId
    ));

  this->replicas_[factoryId] = pFTReplica;
  this->empty_slots_ -= 1;

  pFTReplica->init (this->orb_ ACE_ENV_ARG_PARAMETER);
  return pFTReplica;
}

void FT_ReplicaFactory_i::delete_object (
    const PortableGroup::GenericFactory::FactoryCreationId & factory_creation_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , PortableGroup::ObjectNotFound
  ))
{
  METHOD_ENTRY(FT_ReplicaFactory_i::delete_object);

  InternalGuard guard (this->internals_);

  CORBA::ULong factoryId;
  factory_creation_id >>= factoryId;
  if (factoryId < this->replicas_.size())
  {
    if(this->replicas_[factoryId] != 0)
    {
      this->replicas_[factoryId]->request_quit();
    }
    else
    {
      ACE_THROW(::PortableGroup::ObjectNotFound());
    }
  }
  else
  {
    ACE_THROW(::PortableGroup::ObjectNotFound());
  }
  METHOD_RETURN(FT_ReplicaFactory_i::delete_object);
}

CORBA::Boolean FT_ReplicaFactory_i::is_alive (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_RETURN(FT_ReplicaFactory_i::is_alive)
    1;
}

void FT_ReplicaFactory_i::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  METHOD_ENTRY(FT_FaultDetectorFactory_i::shutdown);
  InternalGuard guard (this->internals_);
  shutdown_i ();
  this->quit_requested_ = 1;
  METHOD_RETURN(FT_FaultDetectorFactory_i::shutdown);
}
