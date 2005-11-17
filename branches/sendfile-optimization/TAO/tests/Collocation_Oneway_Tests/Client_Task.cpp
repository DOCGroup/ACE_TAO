//
// $Id$
//

#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "Client_Task.h"
#include "TestC.h"

ACE_RCSID(Collocation_Oneway_Tests, Client_Task, "$Id$")

Client_Task::Client_Task (const char *ior,
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
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var tmp =
        this->corb_->string_to_object (input_
				       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
        Messaging::SYNC_SCOPE_POLICY_TYPE, anyPolicy ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var tmpGenericVar =
        tmpVar->_set_policy_overrides(polList,CORBA::ADD_OVERRIDE ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
        hello->get_string (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%s>\n",
		  the_string.in ()));

      hello->onewayTest(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - returned from onewayTest() call \n"));

      hello->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
			   "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;

}
