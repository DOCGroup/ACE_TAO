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
    state[offset    ] = static_cast<unsigned char>(value >> 24);
    state[offset + 1] = static_cast<unsigned char>(value >> 16);
    state[offset + 2] = static_cast<unsigned char>(value >>  8);
    state[offset + 3] = static_cast<unsigned char>(value      );
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
#define KEVORKIAN(value, method)                                  \
  if (death_pending_ == (FT_TEST::TestReplica::value)){           \
    suicide (#value " in method " #method);                       \
    return;                                                       \
    } else ;

#define KEVORKIAN_R(value, method, result)                        \
  if (death_pending_ == (FT_TEST::TestReplica::value)){           \
    suicide (#value " in method " #method);                       \
    return result;                                                       \
    } else ;

#define KEVORKIAN_DURING(method)                                   \
  if (death_pending_ == FT_TEST::TestReplica::BEFORE_STATE_CHANGE  \
    || death_pending_ == FT_TEST::TestReplica::BEFORE_REPLICATION  \
    || death_pending_ == FT_TEST::TestReplica::BEFORE_REPLY ){     \
    suicide ("read-only method " #method);                         \
    return;                                                        \
    } else ;

#define KEVORKIAN_R_DURING(method, result)                         \
  if (death_pending_ == FT_TEST::TestReplica::BEFORE_STATE_CHANGE  \
    || death_pending_ == FT_TEST::TestReplica::BEFORE_REPLICATION  \
    || death_pending_ == FT_TEST::TestReplica::BEFORE_REPLY ){     \
    suicide ("read-only method " #method);                         \
    return result;                                                        \
    } else ;


//////////////////////////////////////////////////
// class FT_TestReplica_i construction/destruction

FT_TestReplica_i::FT_TestReplica_i (FT_ReplicaFactory_i * factory, long factoryId)
  : factory_(factory)
  , factoryId_(factoryId)
  , death_pending_(FT_TEST::TestReplica::NOT_YET)
  , verbose_(1)
{
}

FT_TestReplica_i::~FT_TestReplica_i ()
{
}


void FT_TestReplica_i::suicide(const char * note)
{
  std::cout << "Replica # Simulate fault: " << factoryId_ << ": " << note << std::endl;
  ////////////////////////////////////////
  // WARNING: The following call deletes this object and
  // deactivates the servant!  With luck it'll zap the replica
  // not the entire process.
  factory_->removeReplica(factoryId_, this);
}

/////////////////////////////////////////////////////
// class FT_TestReplica_i public, non-CORBA interface
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

long FT_TestReplica_i::factoryId()const
{
  return factoryId_;
}

::FT_TEST::TestReplica_ptr FT_TestReplica_i::objectReference()
{
  PortableServer::POA_var poa = orbManager_->root_poa();
  ::CORBA::Object_ptr obj = poa->servant_to_reference(this);
  return ::FT_TEST::TestReplica::_narrow(obj);
}

::PortableServer::ObjectId * FT_TestReplica_i::objectId()
{
  PortableServer::POA_var poa = orbManager_->root_poa();
  ::PortableServer::ObjectId * oid = poa->servant_to_id(this);
  return oid;
}

char * FT_TestReplica_i::IOR()
{
  PortableServer::POA_var poa = orbManager_->root_poa();
  ::CORBA::Object_var obj = poa->servant_to_reference(this);
  ::CORBA::ORB_var orb = orbManager_->orb();
  return orb->object_to_string(obj);
}



/**
 * Initialize this object.
 * @param orbManager our ORB -- we keep var to it.
 * @return zero for success; nonzero is process return code for failure.
 */
int FT_TestReplica_i::init (TAO_ORB_Manager & orbManager ACE_ENV_ARG_DECL)
{
  orbManager_ = & orbManager;
  objectId_ = orbManager.activate (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

/*
  CORBA::Object_var obj =
    this->poa_->id_to_reference (id.in ()
                                 ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);
*/

  return 0;
}

int FT_TestReplica_i::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  orbManager_->deactivate(objectId_ ACE_ENV_ARG_PARAMETER);
  return 0;
}



/////////////////////////////////////////////////////
// class FT_TestReplica_i:  PullMonitorable interface
CORBA::Boolean FT_TestReplica_i::is_alive ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN_R(DURING_IS_ALIVE, is_alive, 0)
  ACE_ERROR ((LM_ERROR,
    "FT Replica%d: is_alive: %d\n",
    factoryId_,
    (death_pending_ != FT_TEST::TestReplica::DENY_IS_ALIVE)
    ));

  return death_pending_ != FT_TEST::TestReplica::DENY_IS_ALIVE;
}

/////////////////////////////////////////////////////
// class FT_TestReplica_i:  Updateable interface
FT::State * FT_TestReplica_i::get_update ()
  ACE_THROW_SPEC ((CORBA::SystemException, FT::NoUpdateAvailable))
{
  KEVORKIAN_R(DURING_GET_UPDATE, get_update, new ::FT::State)
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
  KEVORKIAN_R(DURING_GET_STATE, get_state, new ::FT::State)
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
  KEVORKIAN_R(BEFORE_STATE_CHANGE, increment, 0)
  long counter = load ();
  counter += delta;
  store (counter);
  KEVORKIAN_R(BEFORE_REPLY, increment, 0)
  return counter;
}

CORBA::Long FT_TestReplica_i::get (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN_R_DURING(get, 0)
  long counter = load ();
  return counter;
}

CORBA::Long FT_TestReplica_i::counter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN_R_DURING([get]counter, 0)
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
  death_pending_ = when;
  KEVORKIAN(RIGHT_NOW, die)
}

void FT_TestReplica_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  death_pending_ = FT_TEST::TestReplica::CLEAN_EXIT;
}

//////////////////////////////////////////////
// FT_TestReplica_i public non-CORBA interface
int FT_TestReplica_i::idle (int & result)
{
  int quit = 0;
  if (death_pending_ == FT_TEST::TestReplica::WHILE_IDLE)
  {
    ACE_ERROR ((LM_ERROR,
      "FT Replica%d: Simulated fault WHILE_IDLE",
      ACE_static_cast(int, factoryId_ )
      ));
    result = 0;
    quit = 1;
  }
  else if (death_pending_ == FT_TEST::TestReplica::CLEAN_EXIT)
  {
    result = 0;
    quit = 1;
  }
  return quit;
}

void FT_TestReplica_i::requestQuit()
{
  death_pending_ = FT_TEST::TestReplica::WHILE_IDLE;
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
    if (verbose_)
    {
      std::cout << "FT Replica" << factoryId_ << ": " << counter << std::endl;
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


