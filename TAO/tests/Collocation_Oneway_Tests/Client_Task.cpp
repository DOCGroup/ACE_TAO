
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "Client_Task.h"
#include "TestC.h"

Client_Task::Client_Task (ACE_TCHAR const *ior,
                          CORBA::ORB_ptr corb,
                          Client_Task::SyncMode sync_mode,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , input_ (ior)
  , corb_ (CORBA::ORB::_duplicate (corb))
  , sync_mode_ (sync_mode)
{
}

int
Client_Task::svc ()
{
  try
    {
      CORBA::Object_var const tmp {this->corb_->string_to_object (this->input_.c_str ())};

      Test::Hello_var hello {Test::Hello::_narrow (tmp.in ())};

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Test::Hello reference <%s>\n"),
                             this->input_.c_str ()),
                           1);
        }

      Test::Hello_var const tmp_var {Test::Hello::_duplicate (hello.in ())};
      CORBA::Any any_policy {};

      Messaging::SyncScope selected_scope {Messaging::SYNC_NONE};

      if (this->sync_mode_ == SyncMode::transport)
        {
          selected_scope = Messaging::SYNC_WITH_TRANSPORT;
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - SYNC_WITH_TRANSPORT collocated oneway test\n"));
        }
      else if (this->sync_mode_ == SyncMode::server)
        {
          selected_scope = Messaging::SYNC_WITH_SERVER;
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - SYNC_WITH_SERVER collocated oneway test\n"));
        }
      else if (this->sync_mode_ == SyncMode::target)
        {
          selected_scope = Messaging::SYNC_WITH_TARGET;
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - SYNC_WITH_TARGET collocated oneway test\n"));
        }
      else if (this->sync_mode_ == SyncMode::none)
        {
          selected_scope = Messaging::SYNC_NONE;
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - SYNC_NONE collocated oneway test\n"));
        }

      any_policy <<= selected_scope;
      CORBA::PolicyList policy_list {};
      policy_list.length (1);
      policy_list[0] = this->corb_->create_policy (
        Messaging::SYNC_SCOPE_POLICY_TYPE, any_policy);

      CORBA::Object_var const tmp_generic_var {
        tmp_var->_set_policy_overrides (policy_list, CORBA::ADD_OVERRIDE)};

      // do unchecked narrow because ORB's not activated. Otherwise get TRANSIENT exception
      hello = Test::Hello::_narrow (tmp_generic_var.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Test::Hello reference <%s>\n"),
                             this->input_.c_str ()),
                           1);
        }

      CORBA::String_var const the_string {hello->get_string ()};

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string.in ()));

      hello->onewayTest ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - returned from onewayTest() call\n"));

      hello->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
