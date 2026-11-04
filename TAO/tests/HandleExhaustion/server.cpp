#include "TestS.h"
#include "ace/Get_Opt.h"
#include "ace/Handle_Set.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_resource.h"
#include "ace/streams.h"
#include <array>
#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file {ACE_TEXT ("server.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s -o <iorfile>\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

class Test_i : public virtual POA_Test
{
public:
  explicit Test_i (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  Test_i (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i &operator= (Test_i &&) = delete;

  void simple () override
  {
  }

  void shutdown () override
  {
    this->orb_->shutdown ();
  }

private:
  CORBA::ORB_var orb_ {};
};

class Descriptors
{
public:
  Descriptors ()
  {
    for (size_t i {}; i < this->openfds_.size (); ++i)
      {
        this->openfds_[i] = ACE_INVALID_HANDLE;
      }
  }

  Descriptors (Descriptors const &) = delete;
  Descriptors (Descriptors &&) = delete;
  Descriptors &operator= (Descriptors const &) = delete;
  Descriptors &operator= (Descriptors &&) = delete;

  void allow_accepts ()
  {
    cout << "Server: closing " << (this->max_close_ - this->min_close_) + 1
         << " fds" << endl;
    for (size_t i = this->min_close_; i <= this->max_close_; ++i)
      {
        ACE_OS::close (this->openfds_[i]);
      }
  }

  void leak (ACE_TCHAR const *file)
  {
    for (size_t i {}; i < this->openfds_.size (); ++i)
      {
        this->openfds_[i] = ACE_OS::open (file, O_RDONLY);
        if (i == 0)
          {
#if defined (ACE_WIN32)
            // the test is not valid on windows so just wing this value
            this->min_close_ = 1000;
#else
            this->min_close_ = (ACE_DEFAULT_SELECT_REACTOR_SIZE - 2) -
              (this->openfds_[i] - 1);
#endif /* ACE_WIN32 */
            cout << "Server: first leaked handle is " << this->openfds_[i]
                 << " min_close is " << this->min_close_ << endl;
          }
        this->max_close_ = i;
        if (this->openfds_[i] == ACE_INVALID_HANDLE)
          {
            cout << "Server: last handle encounterd at i = " << i << endl;
            this->ok_ = true;
            return;
          }
      }
    cout << "Server: Descriptors::leak did not saturate fdset" << endl;
  }

  bool ok () const
  {
    return this->ok_;
  }

private:
  std::array<ACE_HANDLE, 0xffff> openfds_ {};
  size_t min_close_ {};
  size_t max_close_ {};
  bool ok_ {};
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
#if defined (ACE_WIN32)
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  cout << "HandleExhaustion test not available on Windows" << endl;
#else
  try
    {
#if !defined (ACE_LACKS_RLIMIT) && defined (RLIMIT_NOFILE)
    // We must make sure that our file descriptor limit does not exceed
    // the size allowed (in the fd set) by the reactor.  If it does, this
    // test will fail (in a different way than expected) which is a
    // completely different bug than this test is designed to address
    // (see Bug #3326).
    //
    // We must also make sure that this happens before creating the first
    // ORB.  Otherwise, the reactor will be created with a maximum size of
    // the current rlimit for file desriptors (which will later on be
    // increased).
    rlimit rlim {};
    if (ACE_OS::getrlimit(RLIMIT_NOFILE, &rlim) == 0)
      {
        cout << "server evaluating rlimit, cur = "
             << rlim.rlim_cur << " max = " << rlim.rlim_max
             << " reactor max = "
             << ACE_DEFAULT_SELECT_REACTOR_SIZE << endl;
        if (rlim.rlim_cur < static_cast<rlim_t> (ACE_DEFAULT_SELECT_REACTOR_SIZE) &&
            rlim.rlim_max > static_cast<rlim_t> (ACE_DEFAULT_SELECT_REACTOR_SIZE))
          {
            rlim.rlim_cur = ACE_DEFAULT_SELECT_REACTOR_SIZE;
            rlim.rlim_max = ACE_DEFAULT_SELECT_REACTOR_SIZE;
            ACE_OS::setrlimit(RLIMIT_NOFILE, &rlim);
            cout << "server set rlimit_nofile" << endl;
          }
      }
#else
    cout << "server does not support setting rlimit, reactor max = "
         << ACE_DEFAULT_SELECT_REACTOR_SIZE << endl;
#endif /* !ACE_LACKS_RLIMIT && RLIMIT_NOFILE */

      Server_Options options;
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      Test_i *test_i {};
      // ServantBase_var takes ownership of this reference-counted TAO servant.
      ACE_NEW_RETURN (test_i,
                      Test_i (orb.in ()),
                      1);
      PortableServer::ServantBase_var const owner_transfer (test_i);

      PortableServer::ObjectId_var const id = root_poa->activate_object (test_i);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Test_var const test = Test::_narrow (object.in ());

      CORBA::String_var const ior = orb->object_to_string (test.in ());

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")),
          ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file %s for writing IOR: %C\n",
                             ACE_TEXT_ALWAYS_CHAR (options.ior_output_file.c_str ()),
                             ior.in ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();
      poa_manager->activate ();

      Descriptors descriptors;
      descriptors.leak (
#ifdef _WRS_KERNEL
                        "server.out");
#else
                        argv [0]);
#endif

      ACE_Time_Value tv (10);
      orb->run (tv);

      cout << "Server: closing some fds" << endl;

      descriptors.allow_accepts ();
      orb->run ();
      orb->destroy ();

      if (!descriptors.ok ())
        {
          cout << "Server: the accept error never occurred" << endl;
          ACE_ERROR_RETURN ((LM_ERROR, "The accept error never occurred\n"), 1);
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }
#endif /* ACE_WIN32 */
  return 0;
}
