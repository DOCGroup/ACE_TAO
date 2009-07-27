#ifndef CIAO_DATA_BASE_EXEC_H
#define CIAO_DATA_BASE_EXEC_H

#include "Data_Base_svnt.h"
#include "Data_Base_exec_export.h"
#include "tao/LocalObject.h"
#include "ace/Hash_Map_Manager_T.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Data_Base
    {
      namespace CIDL_Data_Base_Component_Impl
      {

        /// Forward declerations.
        class Query_exec_i;
        class Admin_exec_i;

        typedef ACE_Hash_Map_Iterator_Ex
            <const char*,
             ::CIAO::RACE::OperationalString,
            ACE_Hash<const ACE_TCHAR *>,
            ACE_Equal_To<const ACE_TCHAR *>,
            ACE_Thread_Mutex> DATA_BASE_ITER;


        class DATA_BASE_EXEC_Export Data_Base_Component_exec_i
          : public virtual CIAO_CIAO_RACE_Data_Base_Data_Base_Component_Impl::Data_Base_Component_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          Data_Base_Component_exec_i (void);

          virtual ~Data_Base_Component_exec_i (void);

          virtual ::CIAO::RACE::Data_Base::CCM_Admin_ptr
          get_admin ();

          virtual ::CIAO::RACE::Data_Base::CCM_Query_ptr
          get_query ();


          virtual void
          set_session_context (::Components::SessionContext_ptr ctx);

          virtual void ciao_preactivate ();

          virtual void configuration_complete ();

          virtual void ccm_activate ();

          virtual void ccm_passivate ();

          virtual void ccm_remove ();

          protected:
          CIAO_CIAO_RACE_Data_Base_Data_Base_Component_Impl::Data_Base_Component_Context *context_;

          private:
          ACE_Hash_Map_Manager_Ex
            <const char*,
            ::CIAO::RACE::OperationalString,
            ACE_Hash<const char*>,
            ACE_Equal_To<const char*>,
            ACE_Thread_Mutex> data_base_;

          friend class Query_exec_i;

          friend class Admin_exec_i;
        };

        class DATA_BASE_EXEC_Export Data_Base_Component_Home_exec_i
          : public virtual CIAO_CIAO_RACE_Data_Base_Data_Base_Component_Impl::Data_Base_Component_Home_Exec,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          Data_Base_Component_Home_exec_i (void);
          virtual ~Data_Base_Component_Home_exec_i (void);

          virtual ::Components::EnterpriseComponent_ptr
          create ();
        };

        extern "C" DATA_BASE_EXEC_Export ::Components::HomeExecutorBase_ptr
        create_CIAO_RACE_Data_Base_Data_Base_Component_Home_Impl (void);
      }
    }
  }
}


#endif /* CIAO_DATA_BASE_EXEC_H */
