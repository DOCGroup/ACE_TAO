// $Id$

#include "FT_TestReplica_i.h"

//////////////////
// TestReplica_i

namespace
{
  /**
   * Endian neutral store of long into indexable object.
   * BUFFER can be sequence of Octet, unsigned char[], etc.
   *
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

// A macro to simplify suicide.
// exit code 0 tells test drivers "I meant to do that!"
#define KEVORKIAN(value)                                  \
  if (death_pending_ == (FT_TEST::TestReplica::value)){   \
    std::cout << "FT Replica" << identity_ << ": Simulated fault " #value << std::endl;  \
    exit(0);                                              \
    } else ;

FT_TestReplica_i::FT_TestReplica_i (CORBA::ORB_var & orb, int identity)
  : orb_(orb)
  , identity_(identity)
  , death_pending_(FT_TEST::TestReplica::NOT_YET)
  , verbose_(1)
{
}

FT_TestReplica_i::~FT_TestReplica_i ()
{
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

CORBA::Boolean FT_TestReplica_i::is_alive ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN(DURING_IS_ALIVE)
  ACE_ERROR ((LM_ERROR,
    "FT Replica%d: is_alive: %d\n",
    identity_,
    (death_pending_ != FT_TEST::TestReplica::DENY_IS_ALIVE)
    ));

  return death_pending_ != FT_TEST::TestReplica::DENY_IS_ALIVE;
}

FT::State * FT_TestReplica_i::get_update ()
  ACE_THROW_SPEC ((CORBA::SystemException, FT::NoUpdateAvailable))
{
#if defined(FT_TEST_LACKS_UPDATE)
  ACE_THROW( FT::NoUpdateAvailable () );
#else // FT_TEST_LACKS_UPDATE
  KEVORKIAN(DURING_GET_UPDATE)
  long counter = load();
  ::FT::State_var vState = new ::FT::State;
  vState->length(sizeof(counter));
  storeLong(vState, 0, counter);
  return vState._retn();
#endif // FT_TEST_LACKS_UPDATE
}

void FT_TestReplica_i::set_update (const FT::State & s)
  ACE_THROW_SPEC ((CORBA::SystemException, FT::InvalidUpdate))
{
#if defined(FT_TEST_LACKS_UPDATE)
  ACE_THROW ( FT::InvalidUpdate () );
#else // FT_TEST_LACKS_UPDATE
  KEVORKIAN(BEFORE_SET_UPDATE)
  long counter = loadLong(s, 0);
  store(counter);
  KEVORKIAN(AFTER_SET_UPDATE)
#endif // FT_TEST_LACKS_UPDATE
}

::FT::State * FT_TestReplica_i::get_state ()
  ACE_THROW_SPEC ((CORBA::SystemException, FT::NoStateAvailable))
{
#if defined(FT_TEST_LACKS_STATE)
  ACE_THROW( FT::NoStateAvailable () );
#else // FT_TEST_LACKS_STATE
  KEVORKIAN(DURING_GET_STATE)
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
  KEVORKIAN(BEFORE_SET_STATE)
  long counter = loadLong(s, 0);
  store(counter);
  KEVORKIAN(AFTER_SET_STATE)
#endif // FT_TEST_LACKS_STATE
}

//////////////////////////////
// implement FT_TEST::Replica

void FT_TestReplica_i::set (CORBA::Long value
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN(BEFORE_SET)
  long counter = value;
  store(counter);
  KEVORKIAN(AFTER_SET)
}

CORBA::Long FT_TestReplica_i::increment (CORBA::Long delta
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN(BEFORE_INCREMENT)
  long counter = load ();
  counter += delta;
  store (counter);
  KEVORKIAN(AFTER_INCREMENT)
  return counter;
}

CORBA::Long FT_TestReplica_i::get (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN(DURING_GET)
  long counter = load ();
  return counter;
}

CORBA::Long FT_TestReplica_i::counter (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN(DURING_ATTRIBUTE_GET)
  long counter = load ();
  return counter;
}

void FT_TestReplica_i::counter (CORBA::Long counter
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN(BEFORE_ATTRIBUTE_SET)
  store (counter);
  KEVORKIAN(AFTER_ATTRIBUTE_SET)
}

void FT_TestReplica_i::die (FT_TEST::TestReplica::Bane  when
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  death_pending_ = when;
  KEVORKIAN(RIGHT_NOW)
}

void FT_TestReplica_i::shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
//  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
  death_pending_ = FT_TEST::TestReplica::WHILE_IDLE;
}

int FT_TestReplica_i::idle (int & result)
{
  int quit = 0;
  if (death_pending_ == FT_TEST::TestReplica::WHILE_IDLE)
  {
    std::cout << "FT Replica" << identity_ << ": Simulated fault WHILE_IDLE" << std::endl;
    result = 0;
    quit = 1;
  }
  return quit;
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
      std::cout << "FT Replica" << identity_ << ": " << counter << std::endl;
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

