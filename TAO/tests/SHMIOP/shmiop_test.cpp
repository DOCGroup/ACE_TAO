#include "TestS.h"

#include "tao/Strategies/advanced_resource.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Task.h"

#include <memory>
#include <string>

struct Options
{
  ACE_TCHAR const *server_ior_file {};
  ACE_TCHAR const *client_ior_file {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT ("s:c:"));
  int option {};

  while ((option = opts ()) != -1)
    switch (option)
      {
      case 's':
        options.server_ior_file = opts.opt_arg ();
        break;
      case 'c':
        options.client_ior_file = opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s [-s <server IOR file>] "
                                     "[-c <client IOR file>]\n"),
                           argv [0]),
                          -1);
      }

  return 0;
}

class Servant : public virtual POA_Test::Hello
{
public:
  explicit Servant (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  Servant (Servant const &) = delete;
  Servant &operator= (Servant const &) = delete;
  Servant (Servant &&) = delete;
  Servant &operator= (Servant &&) = delete;

  char *get_string () override
  {
    CORBA::String_var str ("hello");
    return str._retn ();
  }

  void shutdown () override
  {
    this->orb_->shutdown (false);
  }

private:
  CORBA::ORB_var const orb_;
};

class ORB_Task : public ACE_Task_Base
{
public:
  explicit ORB_Task (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  ORB_Task (ORB_Task const &) = delete;
  ORB_Task &operator= (ORB_Task const &) = delete;
  ORB_Task (ORB_Task &&) = delete;
  ORB_Task &operator= (ORB_Task &&) = delete;

  int svc () override
  {
    try
      {
        this->orb_->run ();
        return 0;
      }
    catch (CORBA::Exception const &ex)
      {
        ex._tao_print_exception ("Exception caught from ORB::run:");
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ERROR unknown exception caught from ORB::run\n")));
      }
    return 1;
  }

private:
  CORBA::ORB_var const orb_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb (CORBA::ORB_init (argc, argv));
      Options options {};

      if (parse_args (argc, argv, options) != 0)
        return 1;

      Servant servant (orb.in ());
      ORB_Task task (orb.in ());

      if (options.server_ior_file != nullptr)
        {
          CORBA::Object_var const object (
            orb->resolve_initial_references ("RootPOA"));
          PortableServer::POA_var const poa (
            PortableServer::POA::_narrow (object.in ()));
          PortableServer::POAManager_var const manager (poa->the_POAManager ());
          manager->activate ();

          Test::Hello_var const servant_object (servant._this ());
          CORBA::String_var const ior (orb->object_to_string (servant_object.in ()));
          std::unique_ptr<FILE, int (*) (FILE *)> ior_file (
            ACE_OS::fopen (options.server_ior_file, ACE_TEXT ("w")),
            ACE_OS::fclose);
          if (!ior_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Cannot open server IOR file %s: %p\n"),
                               options.server_ior_file),
                              1);
          if (ACE_OS::fputs (ior.in (), ior_file.get ()) == EOF)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Cannot write server IOR file %s: %p\n"),
                               options.server_ior_file),
                              1);

          if (options.client_ior_file == nullptr &&
              task.activate (THR_NEW_LWP | THR_JOINABLE) != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Cannot activate ORB task\n")),
                              1);
        }

      if (options.client_ior_file != nullptr)
        {
          std::string ior ("file://");
          ior += ACE_TEXT_ALWAYS_CHAR (options.client_ior_file);
          CORBA::Object_var const object (orb->string_to_object (ior.c_str ()));
          Test::Hello_var const hello (Test::Hello::_narrow (object.in ()));
          CORBA::String_var const result (hello->get_string ());
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) - returned <%C>\n"),
                      result.in ()));
          hello->shutdown ();
        }
      else if (options.server_ior_file != nullptr)
        {
          if (task.wait () != 0)
            return 1;
        }

      orb->destroy ();
      return 0;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ERROR unknown exception caught in main\n")));
    }
  return 1;
}
