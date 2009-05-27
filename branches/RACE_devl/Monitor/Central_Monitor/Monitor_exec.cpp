#include "Monitor_exec.h"
#include "ciao/CIAO_common.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Central_Monitor_Impl
    {
      Execution_Time_Monitor_exec_i::Execution_Time_Monitor_exec_i ()
        :MAX_BUFFER_SIZE (BUFSIZ)
      {
      }

      Execution_Time_Monitor_exec_i::~Execution_Time_Monitor_exec_i ()
      {
      }


      void
      Execution_Time_Monitor_exec_i::push_delays (const char * id,
                                                  const Delays & delay)
        throw (CORBA::SystemException)
      {
//         ACE_DEBUG ((LM_DEBUG, "Trying to register e-2-e execution times "
//                     "for %s\n", id));

        switch (this->delay_map_.bind (id, delay))
          {
            /// Created new entry for id.
            case 0:
              {
//                 ACE_DEBUG ((LM_DEBUG, "New entry created in the table.\n"));
                break;
              }
            /// id has been already registered. Therefore, update the
            /// contents.
            case 1:
              {
//                 ACE_DEBUG ((LM_DEBUG, "Existing entry found in the table."
//                             "Updating the contents...."));
                /// Retrieve the existing entry from the table.
                Delays prev_entry;
                this->delay_map_.find (id, prev_entry);

                /// "Merge" the existing entry and the new enty and store it
                /// back intro the table.
                CORBA::ULong prev_len = prev_entry.length ();

                /// If the new length is greater than MAX_BUFFER_SIZE, then
                /// reset the sequence.
                if (prev_len + delay.length () > MAX_BUFFER_SIZE)
                  {
                    prev_entry.length (0);
                  }
                else
                  {
                    prev_entry.length (prev_len + delay.length ());
                    for (CORBA::ULong itr = prev_len;
                         itr < prev_entry.length ();
                         ++itr)
                      {
                        prev_entry [itr] = delay [itr - prev_len];
                      }
                  }
                if (this->delay_map_.rebind (id, prev_entry)  == -1)
                  {
                    ACE_ERROR ((LM_ERROR,
                                "ERROR while updating the contents.\n"));
                    break;
                  }

//                 ACE_DEBUG ((LM_DEBUG, "done!\n"));
                break;
              }


            /// This should never happen! -- Unknown error while accessing
            /// the table.
            default:
              {

                ACE_ERROR ((LM_ERROR, "Unknown error while registering e-2-e "
                            "execution times for %s\n.", id));
                break;
              }

          }
        return;
      }


      ::CIAO::RACE::Delays *
      Execution_Time_Monitor_exec_i::get_delays (const char * id)
        throw (::CORBA::SystemException,
               ::CIAO::RACE::Execution_Time_Monitor::IdNotFound)
      {
        Delays temp_delay;
        //        ACE_DEBUG ((LM_DEBUG, "Searching the table for %s...", id));
        if (this->delay_map_.find (id, temp_delay) != 0)
          {
//             ACE_DEBUG ((LM_DEBUG, "Oops! No entry found!\n"));
            throw ::CIAO::RACE::Execution_Time_Monitor::IdNotFound ();
          }
        //        ACE_DEBUG ((LM_DEBUG, "done! Entry found and returned!\n"));

        /// Remove the entry for id from the table.
        this->delay_map_.unbind (id);

        /// Return the delays associated with id.
        Delays_var ret (temp_delay);
        return ret._retn ();
      }


      Central_Monitor_exec_i::Central_Monitor_exec_i ()
      {
      }

      Central_Monitor_exec_i::~Central_Monitor_exec_i ()
      {
      }

      ::CIAO::RACE::CCM_Execution_Time_Monitor_ptr
      Central_Monitor_exec_i::get_Monitor ()
        throw ( ::CORBA::SystemException)
      {
        return new Execution_Time_Monitor_exec_i ();
      }

      // Operations from Components::SessionComponent

      void
      Central_Monitor_exec_i::set_session_context (
        ::Components::SessionContext_ptr ctx)
        throw (::CORBA::SystemException,
               ::Components::CCMException)
      {
        this->context_ =
          Central_Monitor_Context::_narrow (ctx);

        if (this->context_ == 0)
        {
          ACE_THROW (::CORBA::INTERNAL ());
        }
      }

      void
      Central_Monitor_exec_i::ciao_preactivate ()
        throw (::CORBA::SystemException,
               ::Components::CCMException)
      {
      }

      void
      Central_Monitor_exec_i::configuration_complete ()
        throw (::CORBA::SystemException,
               ::Components::CCMException)
      {
      }

      void
      Central_Monitor_exec_i::ccm_activate ()
        throw (::CORBA::SystemException,
               ::Components::CCMException)
      {
      }

      void
      Central_Monitor_exec_i::ccm_passivate ()
        throw (::CORBA::SystemException,
               ::Components::CCMException)
      {
      }

      void
      Central_Monitor_exec_i::ccm_remove ()
        throw (::CORBA::SystemException,
               ::Components::CCMException)
      {
      }


      Central_Monitor_Home_exec_i::Central_Monitor_Home_exec_i ()
      {
      }

      Central_Monitor_Home_exec_i::~Central_Monitor_Home_exec_i ()
      {
      }

      ::Components::EnterpriseComponent_ptr
      Central_Monitor_Home_exec_i::create ()
        throw (::CORBA::SystemException,
               ::Components::CCMException)
      {
        ::Components::EnterpriseComponent_ptr retval =
          ::Components::EnterpriseComponent::_nil ();

        ACE_NEW_THROW_EX (retval,
                          Central_Monitor_exec_i,
                          ::CORBA::NO_MEMORY ());
        ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());
        return retval;
      }

      extern "C" MONITOR_EXEC_Export ::Components::HomeExecutorBase_ptr
      create_CIAO_RACE_Central_Monitor_Home_Impl (void)
      {
        ::Components::HomeExecutorBase_ptr retval =
          ::Components::HomeExecutorBase::_nil ();

        ACE_NEW_RETURN (retval, Central_Monitor_Home_exec_i,
                        ::Components::HomeExecutorBase::_nil ());
        return retval;
      }
    }
  }
}
