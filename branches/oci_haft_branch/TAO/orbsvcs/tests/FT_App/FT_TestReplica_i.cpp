// $Id$
/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_TestReplica_i.cpp
 *
 *  $Id$
 *
 *  Implements CORBA interface TestReplica.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
//

#include "FT_TestReplica_i.h"
#include "FT_ReplicaFactory_i.h"
#include <tao/PortableServer/ORB_Manager.h>
#include <iostream>
#include "FT_TestReplicaC.h"

//////////////////
// TestReplica_i

namespace
{
  /**
   * Endian neutral store of long into indexable object.
   * BUFFER can be sequence of Octet, unsigned char[], etc.
   *
   * TODO: Find this a good home.
   *
   * @param state an object that supports char & operator[] (size_t index);
   * @param offset is the position within state where the first character should be stored.
   * @param value is the data to be inserted into state.
   */
  template<typename BUFFER>
  void storeLong(BUFFER & state, size_t offset, long value)
  {
    state[offset    ] = ACE_static_cast (unsigned char, value >> 24);
    state[offset + 1] = ACE_static_cast (unsigned char, value >> 16);
    state[offset + 2] = ACE_static_cast (unsigned char, value >>  8);
    state[offset + 3] = ACE_static_cast (unsigned char, value      );
  }

  /**
   * Endian neutral load of long from indexable object.
   * BUFFER can be sequence of Octet, unsigned char[], etc.
   *
   * TODO: Find this a good home.
   *
   * @param state an object that supports const char & operator[] (size_t index) const;
   * @param offset is the position within state where the first character can be found
   * @returns value is the data loaded from state.
   */
  template<typename BUFFER>
  long loadLong(const BUFFER & state, size_t offset)
  {
    long result
      = ((state[offset    ] & 0xFF) << 24)
      | ((state[offset + 1] & 0xFF) << 16)
      | ((state[offset + 2] & 0xFF) <<  8)
      | ((state[offset + 3] & 0xFF)      );
    return result;
  }
}

// Macros to simplify suicide.
#define KEVORKIAN(value, method)                                   \
  if (this->death_pending_ == (FT_TEST::TestReplica::value)){            \
    suicide (#value " in method " #method);                        \
    CORBA::OBJECT_NOT_EXIST ex;                                    \
    ACE_THROW(ex);                                                 \
    } else ;

#define KEVORKIAN_DURING(method)                                   \
  if (this->death_pending_ == FT_TEST::TestReplica::BEFORE_STATE_CHANGE  \
    || this->death_pending_ == FT_TEST::TestReplica::BEFORE_REPLICATION  \
    || this->death_pending_ == FT_TEST::TestReplica::BEFORE_REPLY ){     \
    suicide ("read-only method " #method);                         \
    CORBA::OBJECT_NOT_EXIST ex;                                    \
    ACE_THROW(ex);                                                 \
    } else ;


//////////////////////////////////////////////////
// class FT_TestReplica_i construction/destruction

FT_TestReplica_i::FT_TestReplica_i (FT_ReplicaFactory_i * factory, const char * name, unsigned long factory_id)
  : factory_(factory)
  , name_(name)
  , factory_id_(factory_id)
  , death_pending_(FT_TEST::TestReplica::NOT_YET)
  , verbose_(1)
{
//  std::cout << name_.c_str() << '@' << this->factory_->location() << '#' << this->factory_id_ << " Construct" << std::endl;
}

FT_TestReplica_i::~FT_TestReplica_i ()
{
//  std::cout << name_.c_str() << '@' << this->factory_->location() << '#' << this->factory_id_ << " Destruct" << std::endl;
}


void FT_TestReplica_i::suicide(const char * note)
{
  std::cout << name_.c_str() << '@' << this->factory_->location() << '#' << this->factory_id_ << " Simulate fault: " << note << std::endl;

  // Tell the poa we aren't accepting future calls
  this->poa_->deactivate_object (this->object_id_.in ()
                 ACE_ENV_ARG_PARAMETER);
}

/////////////////////////////////////////////////////
// class FT_TestReplica_i public, non-CORBA interface



//static
const char * FT_TestReplica_i::repository_id()
{
  return FT_TEST::_tc_TestReplica->id();
}

int
FT_TestReplica_i::parse_args (int argc, char *argv[])
{
  return 0;
}

//static
const char *
FT_TestReplica_i::usage_options()
{
  return "";
}

unsigned long FT_TestReplica_i::factory_id()const
{
  return this->factory_id_;
}

::PortableServer::POA_ptr FT_TestReplica_i::_default_POA (ACE_ENV_SINGLE_ARG_DECL)
{
  return ::PortableServer::POA::_duplicate(this->poa_ ACE_ENV_ARG_PARAMETER);
}

PortableServer::ObjectId FT_TestReplica_i::object_id()const
{
  return this->object_id_.in();
}


/**
 * Initialize this object.
 * @param orbManager our ORB -- we keep var to it.
 * @return zero for success; nonzero is process return code for failure.
 */
int FT_TestReplica_i::init (CORBA::ORB_var & orb ACE_ENV_ARG_DECL)
{
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

  return 0;
}

void FT_TestReplica_i::_remove_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  //////////////////////////////////////////////////
  // WARNING: The following call invokes fini then deletes this object
  this->factory_->remove_replica(this->factory_id_, this);
}

int FT_TestReplica_i::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  return 0;
}



/////////////////////////////////////////////////////
// class FT_TestReplica_i:  PullMonitorable interface
CORBA::Boolean FT_TestReplica_i::is_alive ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN(DURING_IS_ALIVE, is_alive)
  ACE_ERROR ((LM_ERROR,
    "%s@%s#%d: is_alive: %d\n",
    this->name_.c_str(),
    this->factory_->location(),
    this->factory_id_,
    (this->death_pending_ != FT_TEST::TestReplica::DENY_IS_ALIVE)
    ));

  return this->death_pending_ != FT_TEST::TestReplica::DENY_IS_ALIVE;
}

/////////////////////////////////////////////////////
// class FT_TestReplica_i:  Updateable interface
FT::State * FT_TestReplica_i::get_update ()
  ACE_THROW_SPEC ((CORBA::SystemException, FT::NoUpdateAvailable))
{
  KEVORKIAN(DURING_GET_UPDATE, get_update)
  long counter = load();
  ::FT::State_var vState = new ::FT::State;
  vState->length(sizeof(counter));
  storeLong(vState, 0, counter);
  return vState._retn();
}

void FT_TestReplica_i::set_update (const FT::State & s)
  ACE_THROW_SPEC ((CORBA::SystemException, FT::InvalidUpdate))
{
#if defined(FT_TEST_LACKS_UPDATE)
  ACE_THROW ( FT::InvalidUpdate () );
#else // FT_TEST_LACKS_UPDATE
  KEVORKIAN(BEFORE_SET_UPDATE, set_update)
  long counter = loadLong(s, 0);
  store(counter);
  KEVORKIAN(AFTER_SET_UPDATE, set_update)
#endif // FT_TEST_LACKS_UPDATE
}

/////////////////////////////////////////////////////
// class FT_TestReplica_i:  Checkpointable interface
::FT::State * FT_TestReplica_i::get_state ()
  ACE_THROW_SPEC ((CORBA::SystemException, FT::NoStateAvailable))
{
#if defined(FT_TEST_LACKS_STATE)
  ACE_THROW( FT::NoStateAvailable () );
#else // FT_TEST_LACKS_STATE
  KEVORKIAN(DURING_GET_STATE, get_state)
  long counter = load();
  ::FT::State_var vState = new ::FT::State;
  vState->length(sizeof(counter));
  storeLong(vState, 0, counter);
  return vState._retn();
#endif // FT_TEST_LACKS_STATE
}

void FT_TestReplica_i::set_state (const FT::State & s)
  ACE_THROW_SPEC ((CORBA::SystemException, FT::InvalidState))
{
#if defined(FT_TEST_LACKS_STATE)
  ACE_THROW ( FT::InvalidState () );
#else // FT_TEST_LACKS_STATE
  KEVORKIAN(BEFORE_SET_STATE, set_state)
  long counter = loadLong(s, 0);
  store(counter);
  KEVORKIAN(AFTER_SET_STATE, set_state)
#endif // FT_TEST_LACKS_STATE
}

void FT_TestReplica_i::tao_update_object_group ( PortableGroup::ObjectGroup_ptr iogr
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::NO_IMPLEMENT());
}

//////////////////////////////
// implement FT_TEST::Replica

void FT_TestReplica_i::set (CORBA::Long value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN(BEFORE_STATE_CHANGE, set)
  long counter = value;
  store(counter);
  KEVORKIAN(BEFORE_REPLY, set)
}

CORBA::Long FT_TestReplica_i::increment (CORBA::Long delta
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN(BEFORE_STATE_CHANGE, increment)
  long counter = load ();
  counter += delta;
  store (counter);
  KEVORKIAN(BEFORE_REPLY, increment)
  return counter;
}

CORBA::Long FT_TestReplica_i::get (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN_DURING(get)
  long counter = load ();
  return counter;
}

CORBA::Long FT_TestReplica_i::counter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN_DURING([get]counter)
  long counter = load ();
  return counter;
}

void FT_TestReplica_i::counter (CORBA::Long counter
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN(BEFORE_STATE_CHANGE, [set]counter)
  store (counter);
  KEVORKIAN(BEFORE_REPLY, [set]counter)
}

void FT_TestReplica_i::die (FT_TEST::TestReplica::Bane  when
      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  std::cout << name_.c_str() << '@' << this->factory_->location() << '#' << this->factory_id_ << " Received death threat: " << when << std::endl;

  this->death_pending_ = when;
  KEVORKIAN(RIGHT_NOW, die)
}

void FT_TestReplica_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  std::cout << name_.c_str() << '@' << this->factory_->location() << '#' << this->factory_id_ << " Shut down requested" << std::endl;
  this->death_pending_ = FT_TEST::TestReplica::CLEAN_EXIT;
}

//////////////////////////////////////////////
// FT_TestReplica_i public non-CORBA interface
int FT_TestReplica_i::idle (int & result)
{
  int quit = 0;
  if (this->death_pending_ == FT_TEST::TestReplica::WHILE_IDLE)
  {
    ACE_ERROR ((LM_ERROR,
      "%s@%s#%d: Simulated fault WHILE_IDLE",
      this->name_.c_str(),
      this->factory_->location(),
      ACE_static_cast(int, this->factory_id_ )
      ));
    this->poa_->deactivate_object (this->object_id_.in ()
                 ACE_ENV_ARG_PARAMETER);
    result = 0;
    quit = 1;
  }
  else if (this->death_pending_ == FT_TEST::TestReplica::CLEAN_EXIT)
  {
    this->poa_->deactivate_object (this->object_id_.in ()
                 ACE_ENV_ARG_PARAMETER);
    result = 0;
    quit = 1;
  }
  return quit;
}

void FT_TestReplica_i::request_quit()
{
  this->death_pending_ = FT_TEST::TestReplica::WHILE_IDLE;
}


void FT_TestReplica_i::store(long counter)
{
  FILE * f = ACE_OS::fopen("Persistent.dat", "w");
  if(f != 0)
  {
    unsigned char buffer[sizeof(long)];
    storeLong(buffer, 0, counter);
    ACE_OS::fwrite(buffer, 1, sizeof(long), f);
    ACE_OS::fclose(f);
    if (this->verbose_)
    {
      std::cout << name_.c_str() << '@' << this->factory_->location() << '#' << this->factory_id_ << ": " << counter << std::endl;
    }
  }
}

long FT_TestReplica_i::load ()
{
  long counter = 0;
  FILE * f = ACE_OS::fopen("Persistent.dat","r");
  if(f != 0)
  {
    unsigned char buffer[sizeof(long)];
    ACE_OS::fread(buffer, 1, sizeof(long), f);
    ACE_OS::fclose(f);
    counter = loadLong(buffer, 0);
  }
  return counter;
}

///////////////////////////////////
// Template instantiation for
// competence-challenged compilers.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template void storeLong(::FT::State_var & state, size_t offset, long value);
  template long loadLong(const ::FT::State_var & state, size_t offset);

  template void storeLong(unsigned char * & state, size_t offset, long value);
  template long loadLong(const unsigned char * & state, size_t offset);

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate void storeLong(::FT::State_var & state, size_t offset, long value)
# pragma long loadLong(const ::FT::State_var & state, size_t offset)

# pragma instantiate void storeLong(unsigned char * & state, size_t offset, long value)
# pragma long loadLong(const unsigned char * & state, size_t offset)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
