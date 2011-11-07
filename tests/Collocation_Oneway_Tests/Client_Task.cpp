//
// $Id$
//

#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "Client_Task.h"
#include "TestC.h"

Client_Task::Client_Task (const ACE_TCHAR *ior,
                          CORBA::ORB_ptr corb,
                          Client_Task::ClientSyncModeEnum syncMode,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
    , input_ (ior)
    , corb_ (CORBA::ORB::_duplicate (corb))
    , syncMode_ (syncMode)

{
}

int
Client_Task::svc (void)
{
  try
    {
      CORBA::Object_var tmp =
        this->corb_->string_to_object (input_);

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             input_),
                             1);
        }

      Test::Hello_var tmpVar = hello;
      CORBA::Any anyPolicy;

      Messaging::SyncScope selectedScope = Messaging::SYNC_NONE; // default

      if(syncMode_ == Client_Task::MODE_TRANSPORT) {
          selectedScope = Messaging::SYNC_WITH_TRANSPORT;
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - SYNC_WITH_TRANSPORT collocated oneway test\n"));
      }
      else if(syncMode_ == Client_Task::MODE_SERVER) {
          selectedScope = Messaging::SYNC_WITH_SERVER;
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - SYNC_WITH_SERVER collocated oneway test\n"));
      }
      else if(syncMode_ == Client_Task::MODE_TARGET) {
          selectedScope = Messaging::SYNC_WITH_TARGET;
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - SYNC_WITH_TARGET collocated oneway test\n"));
      }
      else if(syncMode_ == Client_Task::MODE_NONE) {
          selectedScope = Messaging::SYNC_NONE;
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - SYNC_NONE collocated oneway test\n"));
      }

      anyPolicy <<= selectedScope;
      CORBA::PolicyList polList;
      polList.length(1);
      polList[0] = this->corb_->create_policy(
        Messaging::SYNC_SCOPE_POLICY_TYPE, anyPolicy);

      CORBA::Object_var tmpGenericVar =
        tmpVar->_set_policy_overrides(polList,CORBA::ADD_OVERRIDE);

      // do unchecked narrow because ORB's not activated. Otherwise get TRANSIENT exception
      hello = Test::Hello::_narrow(tmpGenericVar.in());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             input_),
                             1);
        }

      CORBA::String_var the_string =
        hello->get_string ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string.in ()));

      hello->onewayTest();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - returned from onewayTest() call\n"));

      hello->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;

}
