//$Id$

#include "Sender_exec.h"

#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"

namespace CIDL_Sender_Impl
{
  char*
  Message_Impl::get_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    //ACE_DEBUG ((LM_DEBUG, "Sender sending out message: [%s]\n", component_.message_.in ()));
    return CORBA::string_dup (component_.message_.in ());
  }

  Sender_exec_i::~Sender_exec_i ()
  {
  }

  void
  Sender_exec_i::local_message (const char * local_message
                                ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    message_ = CORBA::string_dup (local_message);
  }

  char *
  Sender_exec_i::local_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::string_dup (message_.in ());
  }

  void
  Sender_exec_i::hertz (CORBA::Long hertz
                        ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    this->hertz_ = hertz;
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::Hertz:%d\n", this->hertz_));
  }

  CORBA::Long
  Sender_exec_i::hertz (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->hertz_;
  }

  ::Hello::COLOR_SELECTION
  Sender_exec_i::color (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // Your code here.
    return this->color_;
  }

  void
  Sender_exec_i::color (
  ::Hello::COLOR_SELECTION color
  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    this->color_ = color;
  }

  Hello::CCM_ReadMessage_ptr
  Sender_exec_i::get_push_message (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec.i::get_push_message called\n "));
    return ( new Message_Impl (*this) );
  }
  
  void Sender_exec_i::start (::CORBA::Long payload,
                             ::CORBA::Long iterations
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                             ACE_THROW_SPEC ((CORBA::SystemException))
  {
    Hello::OctetStream dataload;
    dataload.length (payload);
    Hello::TimeOut_var event = new OBV_Hello::TimeOut (dataload);
    ACE_DEBUG ((LM_DEBUG, "Sender initiates the process.\n"));
    
      //long niterations = this->hertz_;
      ACE_Sample_History history (iterations);
      
      ACE_hrtime_t test_start = ACE_OS::gethrtime ();
      //timespec short_sleep = { 0, 1000000 };
      
      for (int i = 0; i < iterations; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();

          this->context_->push_click_out (event ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_hrtime_t now = ACE_OS::gethrtime ();
          history.sample (now - start);
          //ACE_OS::nanosleep (&short_sleep);
        }

      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      ACE_DEBUG ((LM_DEBUG, "test finished\n"));

      ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      bool do_dump_history = false;
      if (do_dump_history)
        {
          history.dump_samples ("HISTORY", gsf);
        }

      ACE_Basic_Stats stats;
      history.collect_basic_stats (stats);
      stats.dump_results ("Total", gsf);

      ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                             test_end - test_start,
                                             stats.samples_count ());

    ACE_DEBUG ((LM_DEBUG, "My current color is:"));

    switch (this->color_)
      {
      case ::Hello::empty:
        ACE_DEBUG ((LM_DEBUG, "Enum attribute is uninitialized.\n"));
        break;

      case ::Hello::white:
        ACE_DEBUG ((LM_DEBUG, "white\n"));
        break;

      case ::Hello::red:
        ACE_DEBUG ((LM_DEBUG, "red\n"));
        break;

      case ::Hello::yellow:
        ACE_DEBUG ((LM_DEBUG, "yellow\n"));
        break;

      default:
        ACE_DEBUG ((LM_DEBUG, "Unknown color!\n"));

      }
  }

  void
  Sender_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::set_session_context\n"));

    this->context_ =
          Sender_Exec_Context::_narrow (ctx
                                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil (this->context_.in ()))
      {
        ACE_THROW (CORBA::INTERNAL ());
      }
  }

  void
  Sender_exec_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_i::ciao_preactivate\n"));
  }

  void
  Sender_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_i::ccm_activate\n"));
  }

  void
  Sender_exec_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_i::ciao_postactivate\n"));
  }

  void
  Sender_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::ccm_passivate\n"));
  }

  void
  Sender_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::ccm_remove\n"));
  }

  SenderHome_exec_i::SenderHome_exec_i ()
  {
  }

  SenderHome_exec_i::~SenderHome_exec_i ()
  {
  }

  ::Components::EnterpriseComponent_ptr
  SenderHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) creating SenderHome \n"));
    return new Sender_exec_i;
  }

  extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createSenderHome_Impl (void)
  {
    return new SenderHome_exec_i ();
  }
}

