#include "Controller_exec.h"
#include "ciao/CIAO_common.h"
#include "tao/ORB_Core.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Controller_Impl
    {
      Controller_exec_i::Controller_exec_i (void)
        :sampling_period_ (100)
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
        //        this->poller_->start_controller ();
      }

      void
      Controller_exec_i::stop_controller ()
        throw (::CORBA::SystemException)
      {
        //        this->poller_->stop_controller ();
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
        throw (::CORBA::SystemException,
               ::Components::CCMException)
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
      Controller_exec_i::configuration_complete ()
        throw (::CORBA::SystemException, ::Components::CCMException)
      {
        try
          {
            ACE_DEBUG ((LM_DEBUG, "Trying to contact TM.\n"));
            CIAO::TargetManagerExt_var TM =
              this->context_->get_connection_target_mgr_ext ();

            if (CORBA::is_nil (TM.in ()))
              {
                ACE_ERROR ((LM_ERROR, "Unable to connect to TM!\n"));
                throw CORBA::BAD_INV_ORDER ();
              }
            ACE_DEBUG ((LM_DEBUG, "Done! Connection established with TM!\n"));
            ACE_DEBUG ((LM_DEBUG, "Now creating the poller object.\n"));

            ::CIAO::RACE::Execution_Time_Monitor_var monitor =
                this->context_->get_connection_monitor ();

            if (CORBA::is_nil (monitor.in ()))
              {
                ACE_ERROR ((LM_ERROR, "Unable to connect to Central Monitor!\n"));
                throw CORBA::BAD_INV_ORDER ();
              }


            ACE_Reactor *reactor =
              this->context_->_ciao_the_Container ()->the_ORB ()->orb_core ()->reactor ();

            this->poller_.reset (new Poller (TM.in (),
                                             monitor.in (),
                                             reactor,
                                             this->sampling_period_));
            this->poller_->init ();
            ACE_DEBUG ((LM_DEBUG, "Done creating the poller object.\n"));
          }
        catch (CORBA::Exception & ex)
          {
            ACE_PRINT_EXCEPTION (ex, "(%P|%T|%N|%l) [RACE:] Exception caught "
                                 "in Controller_exec_i::configuration_complete ()");
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
