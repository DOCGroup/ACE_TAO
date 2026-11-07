#define  ACE_BUILD_SVC_DLL
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"

#include <memory>

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Pipe_Proc_Test : public Benchmark_Performance
{
public:
  int init (int, ACE_TCHAR **) override;
  int svc () override;

private:
  ACE_HANDLE pipe_handles[2] {};

  void reader (ACE_HANDLE handle);
};

int
Pipe_Proc_Test::init (int, ACE_TCHAR **)
{
  synch_count = 1;

  if (ACE_OS::pipe (this->pipe_handles) == -1)
    ACE_OS::perror (ACE_TEXT("pipe")), ACE_OS::exit (1);

  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_OS::perror (ACE_TEXT("fork")), ACE_OS::exit (1);
    case 0:
      this->reader (pipe_handles[0]);
      /* NOTREACHED */
      break;
    default:
      break;
    }
  return 1;
}

void
Pipe_Proc_Test::reader (ACE_HANDLE handle)
{
  int const ni = this->thr_id ();
  int const length = performance_test_options.msg_size ();
  char *buffer {};

  ACE_NEW (buffer, char[length]);
  std::unique_ptr<char[]> to {buffer};

  while (ACE_OS::read (handle, to.get (), length) > 0)
    performance_test_options.thr_work_count[ni]++;
}


int
Pipe_Proc_Test::svc ()
{
  ssize_t length = performance_test_options.msg_size ();
  int const ni = this->thr_id ();
  ACE_HANDLE handle = this->pipe_handles[1];
  char *buffer {};

  ACE_NEW_RETURN (buffer, char[length], -1);
  std::unique_ptr<char[]> from {buffer};

  while (!this->done ())
    if (ACE_OS::write (handle, from.get (), length) == length)
      performance_test_options.thr_work_count[ni]++;
    else
      ACE_OS::perror (ACE_TEXT("write"));

  ACE_OS::close (this->pipe_handles[0]);
  ACE_OS::close (this->pipe_handles[1]);
  return 0;
}

ACE_SVC_FACTORY_DECLARE (Pipe_Proc_Test)
ACE_SVC_FACTORY_DEFINE  (Pipe_Proc_Test)

// ACE_Service_Object_Type ppt (&pipe_proc_test, "Pipe_Proc_Test");
#endif /* ACE_HAS_THREADS */
