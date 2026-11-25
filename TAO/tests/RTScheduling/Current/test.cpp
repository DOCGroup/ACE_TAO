#include "Thread_Task.h"
#include "../Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "tao/RTScheduling/Current.h"
#include "tao/RTScheduling/Request_Interceptor.h"
#include "tao/PI_Server/ServerRequestInfo.h"
#include "tao/ORB_Core.h"
#include "tao/TAO_Server_Request.h"
#include "tao/CDR.h"

#include <memory>

class Test_Scheduler : public TAO_Scheduler
{
public:
  explicit Test_Scheduler (CORBA::ORB_ptr orb) : TAO_Scheduler (orb) {}

  void receive_request (PortableInterceptor::ServerRequestInfo_ptr,
                        RTScheduling::Current::IdType_out guid,
                        CORBA::String_out,
                        CORBA::Policy_out,
                        CORBA::Policy_out) override
  {
    if (this->guid_length_ >= 0)
      {
        RTScheduling::Current::IdType *value = new RTScheduling::Current::IdType;
        value->length (static_cast<CORBA::ULong> (this->guid_length_));
        guid = value;
      }
  }

  int guid_length_ {-1};
};

class Test_Request_Info : public TAO::ServerRequestInfo
{
public:
  explicit Test_Request_Info (TAO_ServerRequest &request)
    : TAO::ServerRequestInfo (request, nullptr, 0, nullptr, nullptr, 0) {}

  IOP::ServiceContext *get_request_service_context (IOP::ServiceId id) override
  {
    IOP::ServiceContext *context = new IOP::ServiceContext;
    context->context_id = id;
    return context;
  }
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv);

      CORBA::Object_var manager_obj = orb->resolve_initial_references ("RTSchedulerManager");

      TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj.in ());

      Test_Scheduler scheduler (orb.in ());

      manager->rtscheduler (std::addressof (scheduler));

      CORBA::Object_var current_object =
        orb->resolve_initial_references ("RTScheduler_Current");
      TAO_RTScheduler_Current *current =
        dynamic_cast<TAO_RTScheduler_Current *> (current_object.in ());
      if (current == nullptr)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "RTScheduler_Current has the wrong type\n"), 1);
        }
      TAO_InputCDR input (size_t {0});
      TAO_OutputCDR output;
      TAO_ServerRequest request (nullptr, input, output, nullptr, orb->orb_core ());
      Test_Request_Info request_info (request);
      Server_Interceptor interceptor (current);
      int const invalid_lengths[] = {-1, 0, 1};
      for (int const length : invalid_lengths)
        {
          scheduler.guid_length_ = length;
          interceptor.receive_request (std::addressof (request_info));
          if (current->implementation () != nullptr)
            {
              ACE_ERROR_RETURN ((LM_ERROR, "Invalid GUID installed a current\n"), 1);
            }
        }

      // Cancellation must not read past short GUIDs or overwrite its local
      // numeric buffer for GUIDs larger than size_t.
      DT_Hash_Map hash;
      CORBA::ULong const lengths[] = {0, 1, sizeof (size_t) + 1};
      for (CORBA::ULong const length : lengths)
        {
          RTScheduling::Current::IdType guid;
          guid.length (length);
          for (CORBA::ULong i {}; i < length; ++i)
            {
              guid[i] = 1;
            }
          TAO_RTScheduler_Current_i *test_current =
            new TAO_RTScheduler_Current_i (orb->orb_core (), std::addressof (hash));
          test_current->id (guid);
          bool cancelled = false;
          try
            {
              // cancel_thread deletes the current before throwing.
              test_current->cancel_thread ();
            }
          catch (CORBA::THREAD_CANCELLED const &)
            {
              cancelled = true;
            }
          if (!cancelled)
            {
              ACE_ERROR_RETURN ((LM_ERROR, "Cancellation did not throw\n"), 1);
            }
        }

      Thread_Task task (orb.in ());

      task.activate_task (4);

      orb->run ();

      // Wait until all threads are ready.
      ACE_Thread_Manager::instance ()->wait ();

      orb->destroy ();
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
