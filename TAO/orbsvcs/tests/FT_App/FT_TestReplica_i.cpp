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
#include "tao/ORB_Constants.h"
#include "FT_TestReplicaC.h"
// FUZZ: disable check_for_streams_include
#include "ace/streams.h"
#include "ace/OS_NS_stdio.h"

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
    state[offset    ] = static_cast<unsigned char> (value >> 24);
    state[offset + 1] = static_cast<unsigned char> (value >> 16);
    state[offset + 2] = static_cast<unsigned char> (value >>  8);
    state[offset + 3] = static_cast<unsigned char> (value      );
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

// NO_RESPONSE ->no reinvocation

#define FAULT_CODE CORBA::TRANSIENT

// Macros to simplify suicide.
#define KEVORKIAN(value, method)                                   \
  if (this->death_pending_ == (FT_TEST::TestReplica::value)){      \
    suicide (#value " in method " #method);                        \
    throw FAULT_CODE (                                             \
             CORBA::SystemException::_tao_minor_code (             \
               TAO::VMCID,                                         \
               EFAULT),                                            \
             CORBA::COMPLETED_NO);                                 \
    }

#define KEVORKIAN_DURING(method)                                   \
  if (this->death_pending_ == FT_TEST::TestReplica::BEFORE_REPLY ){\
    suicide ("read-only method " #method);                         \
    throw FAULT_CODE (                                             \
             CORBA::SystemException::_tao_minor_code (             \
               TAO::VMCID,                                         \
               EFAULT),                                            \
             CORBA::COMPLETED_NO);                                 \
    }

#define KEVORKIAN_RETURN(value, method, result)                    \
  if (this->death_pending_ == (FT_TEST::TestReplica::value)){      \
    suicide (#value " in method " #method);                        \
    throw FAULT_CODE (                                             \
        CORBA::SystemException::_tao_minor_code (                  \
              TAO::VMCID,                                          \
              EFAULT),                                             \
              CORBA::COMPLETED_NO);                                \
    }

#define KEVORKIAN_DURING_RETURN(method, result)                    \
  if (this->death_pending_ == FT_TEST::TestReplica::BEFORE_REPLY ){\
    suicide ("read-only method " #method);                         \
    throw FAULT_CODE (                                             \
        CORBA::SystemException::_tao_minor_code (                  \
             TAO::VMCID,                                           \
             EFAULT),                                              \
     CORBA::COMPLETED_NO);                                         \
    }


//////////////////////////////////////////////////
// class FT_TestReplica_i construction/destruction

FT_TestReplica_i::FT_TestReplica_i (FT_ReplicaFactory_i * factory, const char * name, unsigned long factory_id)
  : death_pending_ (FT_TEST::TestReplica::NOT_YET)
  , verbose_ (1)
  , name_ (name)
  , factory_id_ (factory_id)
  , factory_ (factory)
  , orb_ (0)
  , poa_ (0)
  , object_id_ (0)
  , name_persistent_storage
{
//  cout << name_.c_str() << '@' << this->factory_->location() << '#' << this->factory_id_ << " Construct" << endl;
}

FT_TestReplica_i::~FT_TestReplica_i ()
{
//  cout << name_.c_str() << '@' << this->factory_->location() << '#' << this->factory_id_ << " Destruct" << endl;
}


void FT_TestReplica_i::suicide(const char * note)
{
  ACE_OS::fprintf (stdout, "%s@%s#%lu Simulate FAULT_CODE fault: %s\n",
                   name_.c_str(), this->factory_->location(), this->factory_id_, note);

  // Tell the poa we aren't accepting future calls
  this->poa_->deactivate_object (this->object_id_.in ());
}

/////////////////////////////////////////////////////
// class FT_TestReplica_i public, non-CORBA interface



//static
const char * FT_TestReplica_i::repository_id()
{

  const char * id =
    FT_TEST::_tc_TestReplica->id();

  return id;
}

int
FT_TestReplica_i::parse_args (int argc, ACE_TCHAR *argv[])
{
    ACE_UNUSED_ARG (argc);
    ACE_UNUSED_ARG (argv);
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

::PortableServer::POA_ptr FT_TestReplica_i::_default_POA (void)
{
  return ::PortableServer::POA::_duplicate(this->poa_.in ());
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
int FT_TestReplica_i::init (CORBA::ORB_var & orb, const ACE_TCHAR* file_persistent)
{
  this->orb_ = orb;

  this->name_persistent_storage = file_persistent;

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


  if (CORBA::is_nil(this->poa_.in ()))
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

  return 0;
}

void FT_TestReplica_i::_remove_ref (void)
{
  //////////////////////////////////////////////////
  // WARNING: The following call invokes fini then deletes this object
  this->factory_->remove_replica(this->factory_id_, this);
}

int FT_TestReplica_i::fini (void)
{
  return 0;
}



/////////////////////////////////////////////////////
// class FT_TestReplica_i:  PullMonitorable interface
CORBA::Boolean FT_TestReplica_i::is_alive (void)
{
  KEVORKIAN_RETURN(DURING_IS_ALIVE, is_alive, 0)
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
FT::State * FT_TestReplica_i::get_update (void)
{
  KEVORKIAN_RETURN(DURING_GET_UPDATE, get_update, 0)
  long counter = load();
  ::FT::State_var vState = new ::FT::State;
  vState->length(sizeof(counter));
  storeLong(vState, 0, counter);
  return vState._retn();
}

void FT_TestReplica_i::set_update (const FT::State & s)
{
#if defined(FT_TEST_LACKS_UPDATE)
  throw FT::InvalidUpdate ();
#else // FT_TEST_LACKS_UPDATE
  KEVORKIAN(BEFORE_SET_UPDATE, set_update)
  long counter = loadLong<FT::State>(s, 0);
  store(counter);
  KEVORKIAN(AFTER_SET_UPDATE, set_update)
#endif // FT_TEST_LACKS_UPDATE
}

/////////////////////////////////////////////////////
// class FT_TestReplica_i:  Checkpointable interface
::FT::State * FT_TestReplica_i::get_state (void)
{
#if defined(FT_TEST_LACKS_STATE)
  throw FT::NoStateAvailable ();
#else // FT_TEST_LACKS_STATE
  KEVORKIAN_RETURN(DURING_GET_STATE, get_state, 0)
  long counter = load();
  ::FT::State_var vState = new ::FT::State;
  vState->length(sizeof(counter));
  storeLong(vState, 0, counter);
  return vState._retn();
#endif // FT_TEST_LACKS_STATE
}

void FT_TestReplica_i::set_state (const FT::State & s)
{
#if defined(FT_TEST_LACKS_STATE)
  throw FT::InvalidState ();
#else // FT_TEST_LACKS_STATE
  KEVORKIAN(BEFORE_SET_STATE, set_state)
  long counter = loadLong<FT::State>(s, 0);
  store(counter);
  KEVORKIAN(AFTER_SET_STATE, set_state)
#endif // FT_TEST_LACKS_STATE
}

void FT_TestReplica_i::tao_update_object_group (
    const char * iogr,
    PortableGroup::ObjectGroupRefVersion version,
    CORBA::Boolean is_primary
  )
{
  ACE_UNUSED_ARG (iogr);
  ACE_UNUSED_ARG (version);
  ACE_UNUSED_ARG (is_primary);

  throw CORBA::NO_IMPLEMENT();
}

//////////////////////////////
// implement FT_TEST::Replica

void FT_TestReplica_i::set (CORBA::Long value)
{
  KEVORKIAN(BEFORE_STATE_CHANGE, set)
  long counter = value;
  store(counter);
  KEVORKIAN(BEFORE_REPLY, set)
}

CORBA::Long FT_TestReplica_i::increment (CORBA::Long delta)
{
  KEVORKIAN_RETURN(BEFORE_STATE_CHANGE, increment, 0)
  long counter = load ();
  counter += delta;
  store (counter);
  KEVORKIAN_RETURN(BEFORE_REPLY, increment, 0)
  return counter;
}

CORBA::Long FT_TestReplica_i::get (void)
{
  KEVORKIAN_DURING_RETURN(get, 0)
  long counter = load ();
  return counter;
}

CORBA::Long FT_TestReplica_i::counter (void)
{
  KEVORKIAN_DURING_RETURN([get]counter, 0)
  long counter = load ();
  return counter;
}

void FT_TestReplica_i::counter (CORBA::Long counter)
{
  KEVORKIAN(BEFORE_STATE_CHANGE, [set]counter)
  store (counter);
  KEVORKIAN(BEFORE_REPLY, [set]counter)
}

void FT_TestReplica_i::die (FT_TEST::TestReplica::Bane  when)
{
  ACE_OS::fprintf (stdout, "%s@%s#%lu Received death threat: %d\n",
                   name_.c_str(), this->factory_->location(), this->factory_id_, when);

  this->death_pending_ = when;
  KEVORKIAN(RIGHT_NOW, die)
}

void FT_TestReplica_i::shutdown (void)
{
  ACE_OS::fprintf (stdout, "%s@%s#%lu Shut down requested\n",
                   name_.c_str(), this->factory_->location(), this->factory_id_);
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
      static_cast<int> (this->factory_id_ )
      ));
    this->poa_->deactivate_object (this->object_id_.in ());
    result = 0;
    quit = 1;
  }
  else if (this->death_pending_ == FT_TEST::TestReplica::CLEAN_EXIT)
  {
    this->poa_->deactivate_object (this->object_id_.in ());
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
  FILE * f = ACE_OS::fopen(this->name_persistent_storage, "w");
  if(f != 0)
  {
    unsigned char* buffer = 0;
    ACE_NEW (buffer, unsigned char [sizeof(long)]);
    storeLong(buffer, 0, counter);
    ACE_OS::fwrite(buffer, 1, sizeof(long), f);
    ACE_OS::fclose(f);
    if (this->verbose_)
    {
      ACE_OS::fprintf (stdout, "%s@%s#%lu :%ld\n",
                       name_.c_str(), this->factory_->location(), this->factory_id_, counter);
    }
    delete[] buffer;
    buffer = 0;
  }
}

long FT_TestReplica_i::load ()
{
  long counter = 0;
  FILE * f = ACE_OS::fopen(this->name_persistent_storage,"r");
  if(f != 0)
  {
    unsigned char buffer[sizeof(long)];
    ACE_OS::fread(buffer, 1, sizeof(long), f);
    ACE_OS::fclose(f);
    counter = loadLong<unsigned char *>(buffer, 0);
  }
  return counter;
}
