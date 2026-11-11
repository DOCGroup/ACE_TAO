#include "TestClient.h"
#include "MessengerC.h"

#include "tao/ORB_Constants.h"

#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Get_Opt.h"

#include <ctime>
#include <cstdlib>
#include <memory>

TestClient::TestClient (CORBA::ORB_ptr orb, int argc, ACE_TCHAR *argv[])
  : orb_ (CORBA::ORB::_duplicate (orb))
{
  parseCommands (argc, argv);
}

int TestClient::parseCommands (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("s:t:i:r:x:e:z:k:"));
  int c {};
  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
    case 's':
      options_.startup_pause = ACE_OS::atoi (get_opts.opt_arg ());
      break;

    case 't':
      options_.thread_count = ACE_OS::atoi (get_opts.opt_arg ());
      break;

    case 'i':
      options_.iterations = ACE_OS::atoi (get_opts.opt_arg ());
      break;

    case 'r':
      {
        ACE_TCHAR const *opt = get_opts.opt_arg ();
        if (opt[0] == 'r') { options_.random_requests = true; opt++; }
        options_.request_count = ACE_OS::atoi (opt);
        break;
      }

    case 'x':
      {
        ACE_TCHAR const *opt = get_opts.opt_arg ();
        options_.shutdown_orb = (opt && opt[0] != '0');
        break;
      }

    case 'e':
      {
        ACE_TCHAR const *opt = get_opts.opt_arg ();
        while (opt && *opt != '\0')
        {
          if (*opt == 'h') options_.expect_holding = true;
          else if (*opt == 'n') options_.expect_no_profile = true;
          opt++;
        }
        break;
      }

    case 'z':
      options_.pause_type = get_opts.opt_arg ()[0];
      break;

    case 'k':
      options_.ior_file = get_opts.opt_arg ();
      break;

    case '?':
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("usage:  %s\n")
                         ACE_TEXT ("\t-s <startup pause in milliseconds>\n")
                         ACE_TEXT ("\t-t <number of threads>\n")
                         ACE_TEXT ("\t-i <number of iterations per thread>\n")
                         ACE_TEXT ("\t-r <r><number of requests per string_to_object>\n")
                         ACE_TEXT ("\t-x shutdown server orb at end of a string_to_object\n")
                         ACE_TEXT ("\t-e <h><n> 'catch holding or no profile exceptions'\n\n"),
                         argv[0]),
                        -1);
    }
  }
  return 0;
}

void TestClient::pause (int milliseconds)
{
  if (milliseconds > 0)
  {
    if (options_.pause_type == 's')
    {
      ACE_OS::sleep (ACE_Time_Value (0, milliseconds * 1000));
    }
    else if (options_.pause_type == 'r')
    {
      ACE_Time_Value tv (0, milliseconds * 1000);
      orb_->run (tv);
    }
  }
}

bool TestClient::run ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting Client.\n")));
  pause (options_.startup_pause);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("* Client started.\n")));

  if (!buildIORList ())
    return false;

  if (this->activate (THR_NEW_LWP, options_.thread_count) == -1)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("activate failed")));
    return false;
  }

  this->wait ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("* Client ended.\n")));
  return true;
}

// Read in the stringified object references into an array
// Warning: The file may contain many separate IORs separated by linefeeds.
bool TestClient::buildIORList ()
{
  FILE *ior_file = ACE_OS::fopen (options_.ior_file.c_str (), ACE_TEXT ("r"));
  if (ior_file == nullptr)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Fail to open %s\n"), options_.ior_file.c_str ()));
      return false;
    }

  std::unique_ptr<FILE, int (*) (FILE *)> const ior_file_owner (ior_file, ACE_OS::fclose);
  ACE_TString ior;
  while (getline (ior_file_owner.get (), ior) != EOF)
    {
      if (ior.length () > 0)
        iors_.push_back (ior);
      else
        break;
    }
  return true;
}

int TestClient::svc ()
{
  // Every invocation of svc increases the thread count.
  int const thread_num = ++instance_;
  std::size_t const vec_size = iors_.size ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Client Thread started (%d.%d.%B.%d)\n"),
              thread_num, options_.iterations, vec_size, options_.request_count));

  int i {};
  std::size_t obj_iter {};
  int request_iter {};

  ACE_TString current_ior;

  std::srand (static_cast<unsigned int> (std::time (nullptr)));

  try
    {
      int holding_count {};
      int no_profile_count {};

      for (i = 1; i <= options_.iterations; ++i)
        {
          for (obj_iter = 1; obj_iter <= vec_size; ++obj_iter)
            {
              request_iter = -1;
              current_ior = iors_[obj_iter - 1];
              CORBA::Object_var obj = orb_->string_to_object (current_ior.c_str ());
              if (!CORBA::is_nil (obj.in ()))
                {
                  request_iter = 0;
                  Messenger_var test = Messenger::_narrow (obj.in ());

                  if (!CORBA::is_nil (test.in ()))
                    {
                      int const new_req_count (
                        !options_.random_requests ? options_.request_count :
                        static_cast<int> (((static_cast<double> (std::rand ()) /
                                            static_cast<double> (RAND_MAX)) *
                                           static_cast<double> (options_.request_count - 1)) + .5) + 1);
                      for (request_iter = 1; request_iter <= new_req_count; ++request_iter)
                        {
                          try
                            {
                              test->send_message (
                                thread_num, i, static_cast<CORBA::Long> (obj_iter), request_iter);
                            }
                          catch (CORBA::SystemException const &ex)
                            {
                              if (options_.expect_holding && ex.minor () == TAO_POA_HOLDING)
                                {
                                  ACE_ERROR ((LM_ERROR,
                                              ACE_TEXT ("Caught expected holding exception "
                                                        "with (%d.%B.%d)\n"),
                                              thread_num, obj_iter, request_iter));
                                  ++holding_count;
                                }
                              else
                                {
                                  throw;
                                }
                              if (options_.expect_no_profile &&
                                  ex.minor () == TAO_INVOCATION_SEND_REQUEST_MINOR_CODE)
                                {
                                  ACE_ERROR ((LM_ERROR,
                                              ACE_TEXT ("Caught expected no-profile exception "
                                                        "with (%d.%B.%d)\n"),
                                              thread_num, obj_iter, request_iter));
                                  ++no_profile_count;
                                }
                              else
                                {
                                  throw;
                                }
                            }
                        }
                      if (options_.shutdown_orb)
                        test->shutdownOrb ();
                    }
                }
            }
        }

      if (holding_count > 0)
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client thread %d received %d holding error(s).\n"),
                    thread_num, holding_count));

      if (no_profile_count > 0)
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client thread %d received %d no profile error(s).\n"),
                    thread_num, no_profile_count));

      return 0;
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("CORBA client error with (%d.%d.%B.%d):%s\n"),
                  thread_num, i, obj_iter, request_iter, current_ior.c_str ()));
      ex._tao_print_exception (ACE_TEXT (""));
    }
  return 1;
}
