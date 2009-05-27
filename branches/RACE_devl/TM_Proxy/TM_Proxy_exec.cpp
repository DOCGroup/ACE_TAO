// $Id$
#include "TM_Proxy_exec.h"
#include "ciao/CIAO_common.h"
#include "tao/ORB_Core.h"
#include "orbsvcs/CosNamingC.h"
#include <sstream>
#include "Utility.h"
#include "SystemSnapshot.h"

#include "ciao/Containers/Container_Base.h"

namespace CIAO
{
  namespace RACE
  {
    namespace TM_Proxy
    {
      namespace CIDL_TM_Proxy_Component_Impl
      {
        TM_Proxy_Component_exec_i::TM_Proxy_Component_exec_i (void)
          : logger_ ("TM_Proxy.log")
        {
        }

        TM_Proxy_Component_exec_i::~TM_Proxy_Component_exec_i (void)
        {
        }
        // Attribute operations.
        char *
          TM_Proxy_Component_exec_i::TM_Name ()
        {
          return CORBA::string_dup (this->TM_Name_.in ());
        }

        void
          TM_Proxy_Component_exec_i::TM_Name (const char * name )
        {
          this->TM_Name_ = CORBA::string_dup (name);
        }

        // Port operations.

        ::CIAO::RACE::TM_Proxy::CCM_SystemSnapshot_ptr
          TM_Proxy_Component_exec_i::get_snapshot ()
        {
          // Your code here.
          return new SystemSnapshot_exec_i
            (this->TM_Daemon_.in (), this->logger_);
        }

        ::CIAO::RACE::TM_Proxy::CCM_Utility_ptr
          TM_Proxy_Component_exec_i::get_utils ()
        {
          // Your code here.
          return new Utility_exec_i (this->TM_Daemon_.in(), this->logger_);
        }

        ::CIAO::RACE::TM_Proxy::CCM_ResourceManager_ptr
          TM_Proxy_Component_exec_i::get_manager ()
        {
          // Your code here.
          return ::CIAO::RACE::TM_Proxy::CCM_ResourceManager::_nil ();
        }

        // Operations from Components::SessionComponent

        void
          TM_Proxy_Component_exec_i::set_session_context (
          ::Components::SessionContext_ptr ctx)
        {
          this->context_ =
            TM_Proxy_Component_Context::_narrow (ctx);
          if (CORBA::is_nil (this->context_))
          {
            throw ::CORBA::INTERNAL ();
          }
        }

        void
          TM_Proxy_Component_exec_i::ciao_preactivate ()
        {
          // Your code here.
        }

        void
          TM_Proxy_Component_exec_i::configuration_complete ()
        {
          std::stringstream msg;
          msg << "TM_Proxy_Component_exec_i::"
                 "configuration_complete (): Trying to obtain "
                 "the object reference of the TM\n";
          try
          {
            ::CORBA::ORB_ptr orb =
              dynamic_cast<CIAO::Container_i *>  (this->context_->_ciao_the_Container())->the_ORB();

            /// Trying to get the object reference from the Naming Service.
            CORBA::Object_var naming_obj =
              orb->resolve_initial_references ("NameService");
            if (CORBA::is_nil (naming_obj.in ()))
            {
              msg << "Unable to resolve the Name Service.\n";
              this->logger_.log (msg.str());
              return;
            }

            CosNaming::NamingContext_var namingContext =
              CosNaming::NamingContext::_narrow (naming_obj.in ());
            CosNaming::Name name (1);
            name.length (1);
            name[0].id = CORBA::string_dup (this->TM_Name_.in ());
            CORBA::Object_var TM = namingContext->resolve (name);
            msg << "Now trying to resolve \""
                << this->TM_Name_.in () << "\" from the naming service...";
            this->TM_Daemon_ =
              ::CIAO::TM_Daemon::Daemon::_narrow (TM.in ());

            if (CORBA::is_nil (this->TM_Daemon_.in ()))
            {
              msg << "\nObject reference of TM_Daemon is nil!\n";

            }
            msg << "... done!\n"
                << "Successfully obtained the object reference of the TM\n";
          }
          catch (CORBA::Exception &ex)
          {
            msg <<  "Exception caught\n" << ex._info ().c_str();
            msg <<  "\nUnable to resolve reference to "
                << this->TM_Name_.in () << "\n";
          }
          this->logger_.log (msg.str());
          return;
        }

        void
          TM_Proxy_Component_exec_i::ccm_activate ()
        {
          // Your code here.
        }

        void
          TM_Proxy_Component_exec_i::ccm_passivate ()
        {
          // Your code here.
        }

        void
          TM_Proxy_Component_exec_i::ccm_remove ()
        {
          // Your code here.
        }

        //==================================================================
        // Home Executor Implementation Class:   TM_Proxy_Component_Home_exec_i
        //==================================================================

        TM_Proxy_Component_Home_exec_i::TM_Proxy_Component_Home_exec_i (void)
        {
        }

        TM_Proxy_Component_Home_exec_i::~TM_Proxy_Component_Home_exec_i (void)
        {
        }

        ::Components::EnterpriseComponent_ptr
          TM_Proxy_Component_Home_exec_i::create ()
        {
          ::Components::EnterpriseComponent_ptr retval =
            ::Components::EnterpriseComponent::_nil ();

          ACE_NEW_THROW_EX (
            retval,
            TM_Proxy_Component_exec_i,
            ::CORBA::NO_MEMORY ());

          return retval;
        }

        extern "C" TM_PROXY_EXEC_Export ::Components::HomeExecutorBase_ptr
          create_CIAO_RACE_TM_Proxy_TM_Proxy_Component_Home_Impl (void)
        {
          ::Components::HomeExecutorBase_ptr retval =
            ::Components::HomeExecutorBase::_nil ();

          ACE_NEW_RETURN (
            retval,
            TM_Proxy_Component_Home_exec_i,
            ::Components::HomeExecutorBase::_nil ());

          return retval;
        }
      }
    }
  }
}
