// Controller_exec.cpp,v 1.1 2006/01/23 16:59:41 nshankar Exp
#include "Controller_exec.h"
#include "ciao/CIAO_common.h"
#include "Config_Handlers/DnC_Dump.h"
#include "ciao/CIAO_common.h"
namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Controller_Impl
    {
      Controller_exec_i::Controller_exec_i (void)
        :sampling_period_ (10)
      {
      }

      Controller_exec_i::~Controller_exec_i (void)
      {
      }

      CIAO::RACE::CCM_Descriptors_ptr
      Controller_exec_i::get_descriptors ()
        throw (::CORBA::SystemException)
      {
        return new Descriptor (this->poller_.get ());

      }

      void
      Controller_exec_i::start_poller ()
        throw (::CORBA::SystemException)
      {
        this->poller_->start_poller ();
      }

      void
      Controller_exec_i::stop_poller ()
        throw (::CORBA::SystemException)
      {
        this->poller_->stop_poller ();
      }

            void
      Controller_exec_i::start_controller ()
        throw (::CORBA::SystemException)
      {
        this->poller_->start_controller ();
      }

      void
      Controller_exec_i::stop_controller ()
        throw (::CORBA::SystemException)
      {
        this->poller_->stop_controller ();
      }

      ::CORBA::Long
      Controller_exec_i::sampling_period ()
        throw (CORBA::SystemException)
      {
        return this->sampling_period_;
      }

      void
      Controller_exec_i::sampling_period
      (::CORBA::Long period)
        throw (CORBA::SystemException)
      {
        this->sampling_period_ = period;
      }

      void
      Controller_exec_i::set_session_context
      (::Components::SessionContext_ptr ctx)
        throw (::CORBA::SystemException, ::Components::CCMException)
      {
        this->context_ =
        Controller_Context::_narrow (ctx);

        if (this->context_ == 0)
        {
          throw CORBA::INTERNAL ();
        }
      }

      void
      Controller_exec_i::ciao_preactivate ()
        throw (::CORBA::SystemException, ::Components::CCMException)
      {
      }

      void
      Controller_exec_i::ciao_postactivate ()
        throw (::CORBA::SystemException, ::Components::CCMException)
      {
        try
          {
            if (CIAO::debug_level () > 5 )
              {
                ACE_DEBUG ((LM_DEBUG,
                            "RACE %N %l "
                            "Trying to contact the BDC..."));
              }
            CUTS::Path_Measurement_var path =
              this->context_->get_connection_online_measurements ();

            if (CORBA::is_nil (path.in ()))
              {
                ACE_DEBUG ((LM_DEBUG,
                            "RACE %N %l"
                            "Unable to connect to BDC!\n"));
                //throw CORBA::BAD_INV_ORDER ();
              }

            CUTS::BDC_Control_Handle_var control =
              this->context_->get_connection_controls ();

            if (CORBA::is_nil (control.in ()))
              {
                ACE_DEBUG ((LM_DEBUG,
                            "RACE %N %l "
                            "Unable to connect to BDC!\n"));
                //throw CORBA::BAD_INV_ORDER ();
              }
            if (CIAO::debug_level () > 5 )
              {
                ACE_DEBUG ((LM_DEBUG, "Done! Connection established!\n"));
                ACE_DEBUG ((LM_DEBUG,
                            "RACE %N %l",
                            "Trying to contact TM.\n"));
              }
            CIAO::TargetManagerExt_var TM =
              this->context_->get_connection_target_mgr_ext ();

            if (CORBA::is_nil (TM.in ()))
              {
                ACE_DEBUG ((LM_ERROR,
                            "RACE %N %l "
                            "Unable to connect to TM!\n"));
                //throw CORBA::BAD_INV_ORDER ();
              }
            if (CIAO::debug_level () > 5 )
              {
                ACE_DEBUG ((LM_DEBUG,
                            "RACE %N %l "
                            "Now creating the poller object..."));
              }
            this->poller_.reset (new Poller (TM.in (), path.in (),
                                             control.in (),
                                             this->sampling_period_));
            this->poller_->init ();
            if (CIAO::debug_level () > 5 )
              {
                ACE_DEBUG ((LM_DEBUG, "done.\n"));
              }
          }
        catch (CORBA::Exception & ex)
          {
            ACE_PRINT_EXCEPTION (ex, "(%P|%T|%N|%l) [RACE:] Exception caught "
                                 "in Controller_exec_i::ciao_postactivate ()");
          }

      }


      void
      Controller_exec_i::ccm_activate ()
        throw (::CORBA::SystemException, ::Components::CCMException)
      {
      }

      void
      Controller_exec_i::ccm_passivate ()
        throw (::CORBA::SystemException, ::Components::CCMException)
      {
      }


      void
      Controller_exec_i::ccm_remove ()
      throw (::CORBA::SystemException, ::Components::CCMException)
      {
      }

      //==================================================================
      // Home Executor Implementation Class:   Controller_Home_exec_i
      //==================================================================

      Controller_Home_exec_i::Controller_Home_exec_i (void)
      {
      }

      Controller_Home_exec_i::~Controller_Home_exec_i (void)
      {
      }

      ::Components::EnterpriseComponent_ptr
      Controller_Home_exec_i::create ()
      throw (::CORBA::SystemException, ::Components::CCMException)
      {
        ::Components::EnterpriseComponent_ptr retval =
        ::Components::EnterpriseComponent::_nil ();

        ACE_NEW_THROW_EX (retval, Controller_exec_i, CORBA::NO_MEMORY ());
        ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());

        return retval;
      }

      extern "C" CONTROLLER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Controller_Home_Impl (void)
      {
        ::Components::HomeExecutorBase_ptr retval =
        ::Components::HomeExecutorBase::_nil ();

        ACE_NEW_RETURN (retval,
                        Controller_Home_exec_i,
                        ::Components::HomeExecutorBase::_nil ());
        return retval;
      }
    }
  }
}
