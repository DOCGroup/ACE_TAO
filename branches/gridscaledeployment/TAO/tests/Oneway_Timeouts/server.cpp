// $Id$

#include "TestS.h"

#include "tao/Strategies/advanced_resource.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/TAOA.h"
#include "tao/AnyTypeCode/Any.h"

#include "ace/streams.h"
#include "ace/High_Res_Timer.h"
#include "ace/Reactor.h"

const int TIME_THRESHOLD = 50; //ms

int activate_delay = 0000;
int run_delay = 00;
int request_delay = 00;
int abort_after = 0;
int num_expected = 0;
int elapsed_max = 0;
int elapsed_min = 0;
int first_min = 0;
int first_max = 0;
const ACE_TCHAR *server_ior = ACE_TEXT ("test.ior");

class Tester_i
  : public virtual POA_Tester
  , public virtual ACE_Event_Handler
{
public:
  Tester_i (CORBA::ORB_ptr orb)
    : orb_ (orb)
    , id1_ (0)
    , id2_ (0)
    , count_ (0)
    , failed_ (false)
  {
    this->start_ = ACE_High_Res_Timer::gettimeofday_hr ();
  }

  virtual ~Tester_i ()
  {
  }

  virtual void test (CORBA::Long id)
  {
    // Ignore the extra message that were used for making messages
    // queued for SYNC_DELAYED_BUFFERING test cases.
    if (id == -3)
      return;

    testShared (id);
  }

  virtual CORBA::Long test2 (CORBA::Long id)
  {
    if (id == -2)
      {
        // Special id used to force a connect. Ignore.
        this->start_ = ACE_High_Res_Timer::gettimeofday_hr ();
        return id;
      }
    return testShared (id);
  }

  int testShared (CORBA::Long id)
  {
    ACE_Time_Value now = ACE_High_Res_Timer::gettimeofday_hr ();
    if (id == -1)
      {
        // Signals the end of a test run
        if (num_expected > 0 && count_ != num_expected)
          {
            cerr << "Error: Expected " << num_expected
                 << ", but received " << count_ << endl;
            this->failed_ = true;
          }
        long ms = (last_ - first_).msec ();
        if (elapsed_max > 0 && ms > elapsed_max)
          {
            cerr << "Error: Expected  < " << elapsed_max
                 << "ms, but was " << ms << "ms" << endl;
            this->failed_ = true;
          }
        if (elapsed_min > 0 && ms < elapsed_min)
          {
            cerr << "Error: Expected  > " << elapsed_min
                 << "ms, but was " << ms << "ms" << endl;
            this->failed_ = true;
          }
        ms = (first_ - start_).msec ();
        if (first_max > 0 && ms > first_max)
          {
            cerr << "Error: Expected first < " << first_max
                 << "ms, but was " << ms << "ms" << endl;
            this->failed_ = true;
          }
        if (first_min > 0 && ms < first_min)
          {
            cerr << "Error: Expected first > " << first_min
                 << "ms, but was " << ms << "ms" << endl;
            this->failed_ = true;
          }
        ACE_Time_Value timeout (0, 50 * 1000);
        this->orb_->orb_core ()->reactor ()->schedule_timer (this, 0, timeout);
        return id;
      }


    this->last_ = now;
    if (id == 0)
      {
        this->first_ = now;
      }
    ++this->count_;
    cout << 's' << id << endl;
    if (abort_after > 0 && this->count_ >= abort_after)
      {
        cout << "\nAborting..." << endl;
        ACE_OS::abort ();
      }
    if (request_delay > 0 && id == 0)
      {
        ACE_OS::sleep (ACE_Time_Value (0, 1000 * request_delay));
      }
    return id;
  }

  int handle_timeout (const ACE_Time_Value &, const void *)
  {
    this->orb_->shutdown (0);
    return 0;
  }

  bool failed () const {
    return this->failed_;
  }

private:
  CORBA::ORB_ptr orb_;
  CORBA::Long id1_;
  CORBA::Long id2_;
  int count_;
  bool failed_;
  ACE_Time_Value start_;
  ACE_Time_Value first_;
  ACE_Time_Value last_;
};

#include "tao/Messaging/Messaging.h"
#include "tao/Strategies/advanced_resource.h"

#include "ace/streams.h"
#include "ace/Log_Msg.h"
#include "ace/Arg_Shifter.h"

using namespace CORBA;
using namespace PortableServer;

namespace {

  void print_usage ()
  {
    //FUZZ: disable check_for_lack_ACE_OS
    cout << "server [-activate_delay ms] [-run_delay ms] [-request_delay ms] "
      "[-abort_after n] [-server_ior <ior>]\n"
      "\t[-expected n=0] [-elapsed_max ms=0] [-elapsed_min ms=0] "
      "[-first_min ms=0]\n"
      "\t[-first_max ms=0]\n"
      "\tactivate_delay Millisecond delay before POAManager::activate.\n"
      "\trun_delay Millisecond delay before ORB::run ().\n"
      "\trequest_delay Millisecond delay within each servant request.\n"
      "\tabort_after abort () after N requests.\n"
      "\tior Server ior.\n" << endl;
    //FUZZ: enable check_for_lack_ACE_OS
  }

  bool parse_command_line (int ac, ACE_TCHAR *av[])
  {
    ACE_Arg_Shifter args (ac, av);
    args.consume_arg ();
    while (args.is_anything_left ())
      {
        if (args.cur_arg_strncasecmp (ACE_TEXT ("-activate_delay")) == 0)
          {
            args.consume_arg ();
            activate_delay = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT ("-run_delay")) == 0)
          {
            args.consume_arg ();
            run_delay = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT ("-request_delay")) == 0)
          {
            args.consume_arg ();
            request_delay = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT ("-expected")) == 0)
          {
            args.consume_arg ();
            num_expected = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT ("-elapsed_max")) == 0)
          {
            args.consume_arg ();
            elapsed_max = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT ("-elapsed_min")) == 0)
          {
            args.consume_arg ();
            elapsed_min = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT ("-first_min")) == 0)
          {
            args.consume_arg ();
            first_min = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT ("-first_max")) == 0)
          {
            args.consume_arg ();
            first_max = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT ("-abort_after")) == 0)
          {
            args.consume_arg ();
            abort_after = ACE_OS::atoi (args.get_current ());
            args.consume_arg ();
          }
        else if (args.cur_arg_strncasecmp (ACE_TEXT ("-server_ior")) == 0)
          {
            args.consume_arg ();
            server_ior = args.get_current ();
            args.consume_arg ();
          }
        else
          {
            cerr << "Error: Unknown argument \""
                 << args.get_current () << "\"" << endl;
            print_usage ();
            return false;
          }
      }
    return true;
  }

  void WriteIOR (const char *ior)
  {
    ofstream out (ACE_TEXT_ALWAYS_CHAR (server_ior));
    out << ior;
  }

  POA_ptr create_poa (ORB_ptr orb)
  {
    PolicyList pols (2);
    Object_var obj = orb->resolve_initial_references ("RootPOA");
    POA_var root = POA::_narrow (obj.in ());
    ACE_ASSERT (! is_nil (root.in ()));
    pols.length (2);
    pols[0] = root->create_id_assignment_policy (PortableServer::USER_ID);
    pols[1] = root->create_lifespan_policy (PortableServer::PERSISTENT);
    POAManager_var man = root->the_POAManager ();
    POA_var poa = root->create_POA ("X", man.in (), pols);
    return poa._retn ();
  }
}

int ACE_TMAIN (int ac, ACE_TCHAR *av[])
{
  try
    {
      ORB_var orb = ORB_init (ac, av);

      if (!parse_command_line (ac, av))
        {
          return 1;
        }

      POA_var poa = create_poa (orb.in ());
      ACE_ASSERT (! is_nil (poa.in ()));

      Tester_i svt (orb.in ());

      ObjectId_var id = string_to_ObjectId ("tester");

      poa->activate_object_with_id (id.in (), &svt);
      Object_var obj = poa->id_to_reference (id.in ());
      String_var ior = orb->object_to_string (obj.in ());
      WriteIOR (ior.in ());

      cout << "Servants registered and activated." << endl;

      if (activate_delay > 0)
        {
          ACE_OS::sleep (ACE_Time_Value (0, activate_delay * 1000));
        }
      POAManager_var man = poa->the_POAManager ();
      man->activate ();

      cout << "POAManager activated." << endl;

      if (run_delay > 0)
        {
          ACE_OS::sleep (ACE_Time_Value (0, run_delay * 1000));
        }
      cout << "Running orb..." << endl;

      orb->run ();

      if (svt.failed ())
        {
          return 1;
        }

      return 0;
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("server:");
    }

  return 1;
}
