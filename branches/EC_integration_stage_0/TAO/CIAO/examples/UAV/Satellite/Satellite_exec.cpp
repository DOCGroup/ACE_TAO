// $Id$

#include <fstream>
#include <iostream>
#include "CIAO_common.h"
#include "Satellite_exec.h"

#define DISPLACEMENT 256

/// Default constructor.
MyImpl::Satellite_exec_i::Satellite_exec_i (void)
  : str_ ("BM DEVICE DATA"), counter_ (-1)
{

}

/// Default destructor.
MyImpl::Satellite_exec_i::~Satellite_exec_i ()
{
}

// Operations from event sink(s)

void
MyImpl::Satellite_exec_i::push_uav_ready (BBN_UAV::UAVReady *ev
                   ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  static int count = 0;

  ACE_hrtime_t end = ACE_OS::gethrtime ();
  ACE_DEBUG ((LM_DEBUG, "UAV RTT = %i\n", end - start_));
  this->UAV_times_[count++] = end - this->start_;


}

void
MyImpl::Satellite_exec_i::push_ucav_ready (BBN_UAV::UCAVReady *ev
                   ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  static int count = 0;

  ACE_hrtime_t end = ACE_OS::gethrtime ();
  ACE_DEBUG ((LM_DEBUG, "UCAV RTT = %i\n", end - start_));
  this->UCAV_times_[count++] = end - this->start_;

}

void
MyImpl::Satellite_exec_i::push_battle_ready (BBN_UAV::BattleReady *ev
                   ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{

  ACE_hrtime_t end = ACE_OS::gethrtime ();
  ACE_DEBUG ((LM_DEBUG, "Battle RTT = %i\n", end - start_));

  this->Battle_times_[this->counter_] = end - this->start_;

}

// Operations from supported interface(s)

void
MyImpl::Satellite_exec_i::alert (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  BBN_UAV::TargetLocated_var ev = new OBV_BBN_UAV::TargetLocated ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Satellite, pushing TargetLocated!\n")));

  this->counter_++;
  if (this->counter_ == 1000)
    {

      std::ofstream UAV_out;
      std::ofstream UCAV_out;
      std::ofstream battle_out;

      UAV_out.open ("UAV_stats.dat");
      UCAV_out.open ("UCAV_out.dat");
      battle_out.open ("Battle_stats.dat");

      for (int i = 0; i < 3000; ++i)
        {
           UAV_out << (long)this->UAV_times_[i] << "\t";
        }

      for (int j = 0; j < 3000; ++j)
        {
           UCAV_out << (long)this->UCAV_times_[j] << "\t";
        }

      for (int k = 0; k < 1000; ++k)
        {
           battle_out << (long)this->Battle_times_[k] << "\t";
        }

    }
  else
    {
      this->start_ = ACE_OS::gethrtime ();
      this->context_->push_target_located (ev.in ()
                                           ACE_ENV_ARG_PARAMETER);
    }
}

// Operations from Components::SessionComponent
void
MyImpl::Satellite_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Satellite_exec_i::set_session_context\n"));

  this->context_ =
    BBN_UAV::CCM_Satellite_Context::_narrow (ctx
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::Satellite_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Satellite_exec_i::ccm_activate\n"));
  char *argv[1] = { "Satellite_exec"};

  int argc = sizeof(argv)/sizeof(argv[0]);
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);

  CIAO_REGISTER_VALUE_FACTORY (orb.in(), BBN_UAV::UAVReady_init,
                               BBN_UAV::UAVReady);

  CIAO_REGISTER_VALUE_FACTORY (orb.in(), BBN_UAV::UCAVReady_init,
                               BBN_UAV::UCAVReady);


  CIAO_REGISTER_VALUE_FACTORY (orb.in(), BBN_UAV::BattleReady_init,
                               BBN_UAV::BattleReady);
}

void
MyImpl::Satellite_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Satellite_exec_i::ccm_passivate\n"));
}

void
MyImpl::Satellite_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Satellite_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::SatelliteHome_exec_i::SatelliteHome_exec_i ()
{
}

/// Default dtor.
MyImpl::SatelliteHome_exec_i::~SatelliteHome_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::SatelliteHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::Satellite_exec_i;
}


extern "C" SATELLITE_EXEC_Export ::Components::HomeExecutorBase_ptr
createSatelliteHome_Impl (void)
{
  return new MyImpl::SatelliteHome_exec_i;
}
