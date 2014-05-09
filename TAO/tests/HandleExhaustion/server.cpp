// $Id$

#include "TestS.h"
#include "ace/Get_Opt.h"
#include "ace/Handle_Set.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_resource.h"
#include "ace/streams.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("server.ior");

class Test_i: public virtual POA_Test
{
public:
  Test_i (CORBA::ORB_ptr orb)
   : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  void simple (void)
  {
  }

  void shutdown (void)
  {
    this->orb_->shutdown ();
  }

private:
  CORBA::ORB_var orb_;
};

class Descriptors
{
public:
  Descriptors (void)
   : slast_ (ACE_INVALID_HANDLE),
     last_ (ACE_INVALID_HANDLE),
     ok_ (false)
  {
  }

  int allow_accepts (void)
  {
    ACE_OS::close(this->slast_);
    ACE_OS::close(this->last_);
    this->ok_ = true;
    return 0;
  }

  void leak (const ACE_TCHAR* file)
  {
    for (size_t i = 0; i < 0xffff; i++)
      {
        ACE_HANDLE h = ACE_OS::open (file, O_RDONLY);
        if (h == ACE_INVALID_HANDLE)
          {
            cout << "Server: last handle encounterd at i = " << i << endl;
            return;
          }

        // Save the last two file handles so that they can be closed later
        // on.  We need two for this test to work with SHMIOP.
        this->slast_ = this->last_;
        this->last_ = h;
      }
    cout << "Server: Descriptors::leak did not saturate fdset" << endl;
  }

  bool ok (void) const
  {
    return this->ok_ && this->last_ != ACE_INVALID_HANDLE;
  }

private:
  ACE_HANDLE slast_;
  ACE_HANDLE last_;
  bool ok_;
};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
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
    rlimit rlim;
    if (ACE_OS::getrlimit(RLIMIT_NOFILE, &rlim) == 0)
      {
        if (rlim.rlim_cur < static_cast<rlim_t> (ACE_DEFAULT_SELECT_REACTOR_SIZE) &&
            rlim.rlim_max > static_cast<rlim_t> (ACE_DEFAULT_SELECT_REACTOR_SIZE))
          {
            rlim.rlim_cur = ACE_DEFAULT_SELECT_REACTOR_SIZE;
            rlim.rlim_max = ACE_DEFAULT_SELECT_REACTOR_SIZE;
            ACE_OS::setrlimit(RLIMIT_NOFILE, &rlim);
          }
      }
#endif /* !ACE_LACKS_RLIMIT && RLIMIT_NOFILE */

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      if (parse_args (argc, argv) != 0)
        return 1;

      Test_i* test_i;
      ACE_NEW_RETURN (test_i,
                      Test_i (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(test_i);

      PortableServer::ObjectId_var id = root_poa->activate_object (test_i);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test_var test = Test::_narrow (object.in ());

      CORBA::String_var ior = orb->object_to_string (test.in ());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file %s for writing IOR: %C\n",
                           ior_output_file,
                           ior.in ()),
                           1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();
      poa_manager->activate ();

      Descriptors descriptors;
      descriptors.leak (
#ifdef _WRS_KERNEL
                        "server.out");
#else
                        argv[0]);
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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
