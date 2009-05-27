#ifndef CIAO_CONTROLLER_EXEC_H
#define CIAO_CONTROLLER_EXEC_H

#include "Controller_svnt.h"
#include "Poller.h"
#include "ace/Auto_Ptr.h"
#include "Descriptor.h"

#include "Controller_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Controller_Impl
    {

      class CONTROLLER_EXEC_Export Controller_exec_i
      : public virtual Controller_Exec,
      public virtual TAO_Local_RefCounted_Object
      {
      public:
        Controller_exec_i (void);

        virtual ~Controller_exec_i (void);

        // Supported operations.
        virtual void start_poller ()
        throw (::CORBA::SystemException);

        virtual void stop_poller ()
        throw (::CORBA::SystemException);

        virtual void start_controller ()
        throw (::CORBA::SystemException);

        virtual void stop_controller ()
        throw (::CORBA::SystemException);

        // Attribute operations.
        virtual ::CORBA::Long
          sampling_period ()
          throw (CORBA::SystemException);

        virtual void
          sampling_period (::CORBA::Long period)
        throw (CORBA::SystemException);

        // Facet
        virtual ::CIAO::RACE::CCM_Descriptors_ptr
          get_descriptors ()
          throw (::CORBA::SystemException);

        virtual void
        set_session_context (::Components::SessionContext_ptr ctx)
        throw (::CORBA::SystemException, ::Components::CCMException);

        virtual void
        ciao_preactivate ()
        throw (::CORBA::SystemException, ::Components::CCMException);

        virtual void
        configuration_complete ()
        throw (::CORBA::SystemException, ::Components::CCMException);

        virtual void
        ccm_activate ()
        throw (::CORBA::SystemException, ::Components::CCMException);

        virtual void
        ccm_passivate ()
        throw (::CORBA::SystemException, ::Components::CCMException);

        virtual void
        ccm_remove ()
        throw (::CORBA::SystemException, ::Components::CCMException);

        protected:
        Controller_Context *context_;

        auto_ptr <Poller> poller_;

        CORBA::Long sampling_period_;

      };

      class CONTROLLER_EXEC_Export Controller_Home_exec_i
      : public virtual Controller_Home_Exec,
      public virtual TAO_Local_RefCounted_Object
      {
        public:
        Controller_Home_exec_i (void);
        virtual ~Controller_Home_exec_i (void);

        virtual ::Components::EnterpriseComponent_ptr
        create ()
        throw (::CORBA::SystemException, ::Components::CCMException);
      };

      extern "C" CONTROLLER_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Controller_Home_Impl (void);
    }
  }
}

#endif /* CIAO_CONTROLLER_EXEC_H */
