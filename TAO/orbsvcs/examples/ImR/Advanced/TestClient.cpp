//$Id$
#include "TestClient.h"
#include "MessengerC.h"

#include "tao/ORB_Constants.h"

#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Get_Opt.h"

TestClient::TestClient (CORBA::ORB_ptr orb, int argc, ACE_TCHAR *argv[])
: orb_(CORBA::ORB::_duplicate(orb))
, pauseType_('s')
, startupPause_(0)
, threadCount_(0)
, instance_(0)
, iterations_(0)
, requestCount_(0)
, randomRequests_(false)
, shutdownOrb_(false)
, expectHolding_(false)
, expectNoProfile_(false)
, iorFile_(ACE_TEXT("imr_test.ior"))
{
  parseCommands (argc, argv);
}

TestClient::~TestClient()
{
}

int TestClient::parseCommands (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("s:t:i:r:x:e:z:k:"));
  int c;
  while ((c = get_opts()) != -1)
  {
    switch (c)
    {
    case 's':
      startupPause_ = ACE_OS::atoi(get_opts.opt_arg());
      break;

    case 't':
      threadCount_ = ACE_OS::atoi(get_opts.opt_arg());
      break;

    case 'i':
      iterations_ = ACE_OS::atoi(get_opts.opt_arg());
      break;

    case 'r':
      {
        const ACE_TCHAR* opt = get_opts.opt_arg();
        if (opt[0] == 'r') { randomRequests_ = true; opt++; }
        requestCount_ = ACE_OS::atoi(opt);
        break;
      }

    case 'x':
      {
        const ACE_TCHAR* opt = get_opts.opt_arg();
        shutdownOrb_ = (opt && opt[0] != '0');
        break;
      }

    case 'e':
      {
        const ACE_TCHAR* opt = get_opts.opt_arg();
        while (opt && *opt != '\0')
        {
          if (*opt == 'h') expectHolding_ = true;
          else if (*opt == 'n') expectNoProfile_ = true;
          opt++;
        }
        break;
      }

    case 'z':
      pauseType_ = get_opts.opt_arg()[0];
      break;

    case 'k':
      iorFile_ = get_opts.opt_arg();
      break;

    case '?':
    default:
      ACE_ERROR_RETURN((LM_ERROR,
        "usage:  %s\n"
        "\t-s <startup pause in milliseconds>\n"
        "\t-t <number of threads>\n"
        "\t-i <number of iterations per thread>\n"
        "\t-r <r><number of requests per string_to_object>\n"
        "\t-x 'shutdown server orb at end of a string_to_object'\n"
        "\t-e <h><n> 'catch holding or no profile exceptions'\n"
        "\n",
        argv[0]),
        -1);
    }
  }
  return 0;
}

void TestClient::pause(int milliseconds)
{
  if (milliseconds > 0)
  {
    if (pauseType_ == 's')
    {
      ACE_OS::sleep(ACE_Time_Value(0, milliseconds * 1000));
    }
    else if (pauseType_ == 'r')
    {
      ACE_Time_Value tv(0, milliseconds * 1000);
      orb_->run(tv);
    }
  }
}

void TestClient::run()
{
  ACE_DEBUG((LM_DEBUG, "Starting Client.\n"));
  pause(startupPause_);
  ACE_DEBUG((LM_DEBUG, "* Client started.\n"));

  buildIORList();

  if (this->activate(THR_NEW_LWP, threadCount_) == -1)
  {
    ACE_ERROR((LM_ERROR, "%p\n", "activate failed"));
  }

  this->wait();

  ACE_DEBUG((LM_DEBUG, "* Client ended.\n"));

}

// Read in the stringified object references into an array
// Warning: The file may contain many separate IORs separated by linefeeds.
void TestClient::buildIORList()
{
  FILE* iorFile = ACE_OS::fopen (iorFile_, "r");
  if ( iorFile == 0 )
    ACE_ERROR ((LM_ERROR, "Fail to open %s\n", iorFile_));

  ACE_TString ior;
  while (getline(iorFile, ior) != EOF )
  {
    if (ior.length() > 0)
      iors_.push_back(ior);
    else
      break;
  }
}

int TestClient::svc()
{
  // Every invocation of svc increates the thread count
  instance_++;
  int threadNum = instance_;
  size_t vec_size = iors_.size();

  ACE_DEBUG((LM_DEBUG, "* Client Thread started (%d.%d.%d.%d)\n",
             threadNum, iterations_, vec_size, requestCount_));

  int     i           = 0;
  size_t  objIter     = 0;
  int     requestIter = 0;

  ACE_TString currentIOR;

  ACE_OS::srand(static_cast<u_int> (ACE_OS::time()));

  try
  {
    int holdingCount    = 0;
    int noProfileCount  = 0;

    // For each iteration
    for (i = 1; i <= iterations_; i++)
    {
      // For each object reference read from file
      for (objIter = 1; objIter <= vec_size; objIter++)
      {
        requestIter = -1;
        // Get a imr_test_var
        currentIOR = iors_[objIter - 1];
        CORBA::Object_var obj = orb_->string_to_object(currentIOR.c_str());
        if (CORBA::is_nil(obj.in()) == false)
        {
          requestIter = 0;
          Messenger_var test = Messenger::_narrow(obj.in());

          if (CORBA::is_nil(test.in()) == false)
          {
            // Calculate the number of requests
            int newReqCount (randomRequests_ == false ? requestCount_ :
            (int)((((double)ACE_OS::rand() / (double)RAND_MAX) * (double)(requestCount_ - 1)) + .5) + 1);
            // For each request
            for (requestIter = 1; requestIter <= newReqCount;  requestIter++)
            {
              try
              {
                test->send_message(threadNum, i, objIter, requestIter);
              }
              catch (CORBA::SystemException& ex)
              {
                // If these exceptions are expected record the number of instances, otherwise rethrow
                if (expectHolding_ == true && ex.minor() == TAO_POA_HOLDING)
                {
                   ACE_ERROR((LM_ERROR, "Caught expected holding exception with (%d.%d.%d)\n",
                     threadNum, objIter, requestIter));
                  holdingCount++;
                }
                else
                {
                  throw;
                }
                if (expectNoProfile_ == true
                  && ex.minor() == TAO_INVOCATION_SEND_REQUEST_MINOR_CODE)
                {
                   ACE_ERROR((LM_ERROR, "Caught expected holding exception with (%d.%d.%d)\n",
                     threadNum, objIter, requestIter));
                  noProfileCount++;
                }
                else
                {
                  throw;
                }
              } // catch
            } // for request
            // We are done with our non-nil narrowed obj ref
            if (shutdownOrb_ == true) test->shutdownOrb();
          } // if narrow
        } // if obj
      } // for obj
    } // for iter
    // Report expected exceptions
    if (holdingCount > 0)
    {
      ACE_DEBUG((LM_DEBUG,"Client thread %d received %d holding error(s).\n",
                 threadNum, holdingCount));
    }

    if (noProfileCount > 0)
    {
      ACE_DEBUG((LM_DEBUG,"Client thread %d received %d no profile error(s).\n",
                 threadNum, noProfileCount));
    }

    return 0;
  } // try
  catch (CORBA::Exception& ex)
  {
    ACE_ERROR((LM_ERROR,"CORBA client error with (%d.%d.%d.%d):%s\n",
               threadNum, i, objIter, requestIter, currentIOR.c_str()));
    ex._tao_print_exception ("");
  }
  return 1;
}
