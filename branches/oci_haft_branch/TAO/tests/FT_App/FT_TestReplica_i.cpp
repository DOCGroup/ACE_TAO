// $Id$

#include "FT_TestReplica_i.h"

//////////////////
// TestReplica_i

namespace
{
  /**
   * Endian neutral store of long into sequence of Octet.
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
   * Endian neutral load of long from sequence of Octet.
   */
  template<typename BUFFER>
  long loadLong(const BUFFER & state, size_t offset)
  {
    long result
      = (state[offset    ] << 24)
      | (state[offset + 1] << 16)
      | (state[offset + 2] <<  8)
      | (state[offset + 3]      );
    return result;
  }
}

// A macro to simplify suicide.
// exit code 0 tells test drivers "I meant to do that!"
#define KEVORKIAN(value) if (death_pending_ == (FT_TEST::TestReplica::value)){this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);} else ;

FT_TestReplica_i::FT_TestReplica_i (CORBA::ORB_var & orb)
  : counter_(0)
  , orb_(orb)
  , death_pending_(FT_TEST::TestReplica::NOT_YET)
{
  load();
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
  return death_pending_ != FT_TEST::TestReplica::DENY_IS_ALIVE;
}

FT::State * FT_TestReplica_i::get_update ()
  ACE_THROW_SPEC ((CORBA::SystemException, FT::NoUpdateAvailable))
{
#if defined(FT_TEST_LACKS_UPDATE)
  ACE_THROW( FT::NoUpdateAvailable () );
#else // FT_TEST_LACKS_UPDATE
  KEVORKIAN(DURING_GET_UPDATE)
  load();
  ::FT::State_var vState = new ::FT::State;
  vState->length(sizeof(long));
  storeLong(vState, 0, counter_);
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
  counter_ = loadLong(s, 0);
  store();
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
  ::FT::State_var vState = new ::FT::State;
  vState->length(sizeof(long));
  load();
  storeLong(vState, 0, counter_);
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
  counter_ = loadLong(s, 0);
  store();
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
  counter_ = value;
  store();
  KEVORKIAN(AFTER_SET)
}

CORBA::Long FT_TestReplica_i::increment (CORBA::Long delta
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN(BEFORE_INCREMENT)
  counter_ += delta;
  store();
  KEVORKIAN(AFTER_INCREMENT)
  return counter_;
}

CORBA::Long FT_TestReplica_i::get (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN(DURING_GET)
  return counter_;
}

CORBA::Long FT_TestReplica_i::counter (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN(DURING_ATTRIBUTE_GET)
  return counter_;
}

void FT_TestReplica_i::counter (CORBA::Long counter
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  KEVORKIAN(BEFORE_ATTRIBUTE_SET)
  counter_ = counter;
  store();
  KEVORKIAN(AFTER_ATTRIBUTE_SET)
}

void FT_TestReplica_i::die (FT_TEST::TestReplica::Bane  when
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  death_pending_ = when;
  KEVORKIAN(RIGHT_NOW)
}

void FT_TestReplica_i::store()
{
  FILE * f = ACE_OS::fopen("Persistent.dat", "w");
  if(f != 0)
  {
    char buffer[sizeof(long)];
    storeLong(buffer, 0, counter_);
    ACE_OS::fwrite(buffer, 1, sizeof(long), f);
    ACE_OS::fclose(f);
  }

}

void FT_TestReplica_i::load()
{
  counter_ = 0;
  FILE * f = ACE_OS::fopen("Persistent.dat","r");
  if(f != 0)
  {
    char buffer[sizeof(long)];
    ACE_OS::fread(buffer, 1, sizeof(long), f);
    ACE_OS::fclose(f);
    counter_ = loadLong(buffer, 0);
  }
}

