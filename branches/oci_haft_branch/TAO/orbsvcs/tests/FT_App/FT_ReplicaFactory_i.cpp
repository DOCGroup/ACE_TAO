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
#include "orbsvcs/CosNamingC.h"
#include "tao/PortableServer/ORB_Manager.h"
#include "orbsvcs/PortableGroup/PG_Properties_Decoder.h"

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


//////////////////////////////////////////////////////
// FT_ReplicaFactory_i  Construction/destruction

FT_ReplicaFactory_i::FT_ReplicaFactory_i ()
  : ior_output_file_(0)
  , nsName_(0)
  , quitOnIdle_(0)
  , emptySlots_(0)
  , quitRequested_(0)
{
}


FT_ReplicaFactory_i::~FT_ReplicaFactory_i ()
{
  //scope the guard
  {
    InternalGuard guard (internals_);

    // be sure all replicas are gone
    // before this object disappears
    shutdown_i ();
  }
  ACE_DECLARE_NEW_ENV;
  fini (ACE_ENV_SINGLE_ARG_PARAMETER);
}

////////////////////////////////////////////
// FT_ReplicaFactory_i private methods

CORBA::ULong FT_ReplicaFactory_i::allocateId()
{
  CORBA::ULong id = replicas_.size();
  if (emptySlots_ != 0)
  {
    for(CORBA::ULong pos = 0; pos < id; ++pos)
    {
      if (replicas_[pos] == 0)
      {
        id = pos;
      }
    }
  }
  else
  {
    replicas_.push_back(0);
    emptySlots_ += 1;
  }
  return id;
}

void FT_ReplicaFactory_i::shutdown_i()
{
  // assume mutex is locked
  for (size_t nReplica = 0; nReplica < replicas_.size(); ++nReplica)
  {
    FT_TestReplica_i * replica = replicas_[nReplica];
    if (replica != 0)
    {
      replica->requestQuit();
    }
  }
}

int FT_ReplicaFactory_i::write_IOR()
{
  int result = -1;
  FILE* out = ACE_OS::fopen (ior_output_file_, "w");
  if (out)
  {
    ACE_OS::fprintf (out, "%s", static_cast<const char *>(ior_));
    ACE_OS::fclose (out);
    result = 0;
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
      "Open failed for %s\n", ior_output_file_
    ));
  }
  return result;
}

//////////////////////////////////////////////////////
// FT_ReplicaFactory_i public, non-CORBA methods

int FT_ReplicaFactory_i::parse_args (int argc, char * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:q");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'o':
        ior_output_file_ = get_opts.opt_arg ();
        break;
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
                           " -o <iorfile>"
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

const char * FT_ReplicaFactory_i::identity () const
{
  return identity_.c_str();
}

int FT_ReplicaFactory_i::idle (int & result)
{
  ACE_UNUSED_ARG (result);
  int quit = quitRequested_;
  if (!quitRequested_ && replicas_.size() == emptySlots_)
  {
    ACE_ERROR (( LM_ERROR,
      "ReplicaFactory is idle.\n"
      ));
    if (quitOnIdle_)
    {
      ACE_ERROR (( LM_ERROR,
        "ReplicaFactory exits due to quit on idle option.\n"
        ));
      quit = 1;
    }
  }

  return quit;
}


int FT_ReplicaFactory_i::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  if (ior_output_file_ != 0)
  {
    ACE_OS::unlink (ior_output_file_);
    ior_output_file_ = 0;
  }
  if (nsName_ != 0)
  {
    naming_context_->unbind (this_name_
                            ACE_ENV_ARG_PARAMETER);
    nsName_ = 0;
  }
  return 0;
}

int FT_ReplicaFactory_i::init (TAO_ORB_Manager & orbManager
  ACE_ENV_ARG_DECL)
{
  int result = 0;
  orb_ = orbManager.orb();
  // todo: improve poa handling -- 
  //   -- allow separate poas for the factory and replicas
  poa_ = orbManager.root_poa();

  // Register with the ORB.
  ior_ = orbManager.activate (this
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (ior_output_file_ != 0)
  {
    identity_ = "file:";
    identity_ += ior_output_file_;
    result = write_IOR();
  }
  else
  {
    // if no IOR file specified,
    // then always try to register with name service
    nsName_ = "FT_ReplicaFactory";
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

void FT_ReplicaFactory_i::removeReplica(CORBA::ULong id, FT_TestReplica_i * replica)
{
  InternalGuard guard (internals_);
  if (id < replicas_.size())
  {
    if(replicas_[id] == replica)
    {
      delete replicas_[id];
      replicas_[id] = 0;
      emptySlots_ += 1;
    }
    else
    {
      ACE_ERROR (( LM_ERROR,
        "Remove replica %d mismatch.\n",
        ACE_static_cast(int, id)
        ));
    }
  }
  else
  {
    ACE_ERROR (( LM_ERROR,
      "Attempt to remove invalid replica %d. Limit %d.\n",
      ACE_static_cast(int, id),
      ACE_static_cast(int, replicas_.size())
      ));
  }
}

//////////////////////////////////////////
// FT_ReplicaFactory_i CORBA methods

CORBA::Object_ptr FT_ReplicaFactory_i::create_object (
    const char * type_id,
    const FT::Criteria & the_criteria,
    FT::GenericFactory::FactoryCreationId_out factory_creation_id
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
  InternalGuard guard (internals_);

  ::TAO_PG::Properties_Decoder decoder (the_criteria);

  // boolean, becomes true if a required parameter is missing
  int missingParameter = 0;
  const char * missingParameterName = 0;

  CORBA::Long initialValue = 0;
  if (! ::TAO_PG::find (decoder, "INITIAL_VALUE", initialValue) )
  {
    // not required.  Otherwise:
    // missingParameter = 1;
    // missingParameterName = "INITIAL_VALUE";
  }

  if (missingParameter)
  {
    ACE_ERROR ((LM_ERROR,
      "Throwing 'InvalidCriteria' due to missing %s\n",
      missingParameterName
      ));
    ACE_THROW ( PortableGroup::InvalidCriteria() );
  }

  CORBA::ULong replicaId = allocateId();


  // NOTE: ACE_NEW is incompatable with ACE_Auto_Basic_Ptr
  // so create a bare pointer first.
  FT_TestReplica_i * pFTReplica = 0;

  ACE_NEW_NORETURN(pFTReplica, FT_TestReplica_i(
    this,
    replicaId    
    ));
  if (pFTReplica == 0)
  {
    ACE_ERROR ((LM_ERROR,
      "New Replica_i returned NULL.  Throwing ObjectNotCreated.\n"
      ));
    ACE_THROW ( PortableGroup::ObjectNotCreated() );
  }
  ACE_Auto_Basic_Ptr<FT_TestReplica_i> replica(pFTReplica);

  ACE_NEW_NORETURN ( factory_creation_id,
    PortableGroup::GenericFactory::FactoryCreationId);
  if (factory_creation_id.ptr() == 0)
  {
    ACE_ERROR ((LM_ERROR,
      "New factory_creation_id returned NULL.  Throwing ObjectNotCreated.\n"
      ));

    ACE_THROW ( PortableGroup::ObjectNotCreated() );
  }

  // assign id and capture replica object before
  // activating it so it will be there when remove is called
  (*factory_creation_id) <<= replicaId;
  replicas_.push_back(replica.release());

  PortableServer::ObjectId_var id =
    poa_->activate_object (pFTReplica
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Object_var obj =
    this->poa_->id_to_reference (id.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  METHOD_RETURN(FT_ReplicaFactory_i::create_object)
    obj._retn();
}

void FT_ReplicaFactory_i::delete_object (
    const FT::GenericFactory::FactoryCreationId & factory_creation_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , PortableGroup::ObjectNotFound
  ))
{
  METHOD_ENTRY(FT_ReplicaFactory_i::delete_object);

  InternalGuard guard (internals_);

  CORBA::ULong replicaId;
  factory_creation_id >>= replicaId;
  if (replicaId < replicas_.size())
  {
    if(replicas_[replicaId] != 0)
    {
      replicas_[replicaId]->requestQuit();
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

CORBA::Boolean FT_ReplicaFactory_i::is_alive (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_RETURN(FT_ReplicaFactory_i::is_alive)
    1;
}

void FT_ReplicaFactory_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  METHOD_ENTRY(FT_FaultDetectorFactory_i::shutdown);
  InternalGuard guard (internals_);
  shutdown_i ();
  quitRequested_ = 1;
  METHOD_RETURN(FT_FaultDetectorFactory_i::shutdown);
}


///////////////////////////////////
// Template instantiation for
// competence-challenged compilers.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template ACE_Vector<FT_TestReplica_i *>;
  template ACE_Guard<ACE_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Vector<FT_TestReplica_i *>
# pragma ACE_Guard<ACE_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

