// $Id$

#define  ACE_BUILD_SVC_DLL
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Thread_Manager.h"
#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Pipe_Thr_Test : public Benchmark_Performance
{
public:
  virtual int init (int, ACE_TCHAR **);
  virtual int svc (void);

private:
  ACE_HANDLE pipe_handles[2];

  static void *reader (Pipe_Thr_Test *);
};

void *
Pipe_Thr_Test::reader (Pipe_Thr_Test *t)
{
  ACE_HANDLE handle = t->pipe_handles[0];
  int ni = t->thr_id ();
  size_t length = performance_test_options.msg_size ();
  char *to;
  ACE_NEW_RETURN (to, char[length], 0);

  while (ACE_OS::read (handle, to, length) > 0)
    performance_test_options.thr_work_count[ni]++;

  return 0;
}

int
Pipe_Thr_Test::init (int, ACE_TCHAR **)
{
  synch_count = 1;

  if (ACE_OS::pipe (this->pipe_handles) == -1)
    ACE_OS::perror (ACE_TEXT("pipe")), ACE_OS::exit (1);

  if (ACE_Thread_Manager::instance ()->spawn
      (ACE_THR_FUNC (Pipe_Thr_Test::reader),
       (void *) this, performance_test_options.t_flags ()) == -1)
    ACE_OS::perror (ACE_TEXT("thr_create")), ACE_OS::exit (1);

  return 1;
}

int
Pipe_Thr_Test::svc (void)
{
  ssize_t length = performance_test_options.msg_size ();
  ACE_HANDLE handle = this->pipe_handles[1];
  char *from;
  ACE_NEW_RETURN (from, char[length], -1);

  while (!this->done ())
    if (ACE_OS::write (handle, from, length) != length)
      ACE_OS::perror (ACE_TEXT("write"));

  ACE_OS::close (this->pipe_handles[0]);
  ACE_OS::close (this->pipe_handles[1]);

  return 0;
}

ACE_SVC_FACTORY_DECLARE (Pipe_Thr_Test)
ACE_SVC_FACTORY_DEFINE  (Pipe_Thr_Test)

// ACE_Service_Object_Type ptt (&pipe_thr_test, "Pipe_Thr_Test");
#endif /* ACE_HAS_THREADS */
