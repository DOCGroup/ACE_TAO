// $Id$

#define  ACE_BUILD_SVC_DLL
#include "ace/Thread_Manager.h"
#include "Options.h"
#include "Benchmark.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Pipe_Thr_Test : public Benchmark
{
public:
  virtual int init (int, char **);
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
  size_t length = options.msg_size ();
  char *to; 
  ACE_NEW_RETURN (to, char[length], 0);

  while (ACE_OS::read (handle, to, length) > 0)
    options.thr_work_count[ni]++;

  return 0;
}

int
Pipe_Thr_Test::init (int, char **)
{
  synch_count = 1;

  if (ACE_OS::pipe (this->pipe_handles) == -1)
    ACE_OS::perror ("pipe"), ACE_OS::exit (1);

  if (ACE_Thread_Manager::instance ()->spawn 
      (ACE_THR_FUNC (Pipe_Thr_Test::reader),
       (void *) this, options.t_flags ()) == -1)
    ACE_OS::perror ("thr_create"), ACE_OS::exit (1);

  return 1;
}

int
Pipe_Thr_Test::svc (void)
{
  ssize_t length = options.msg_size ();
  ACE_HANDLE handle = this->pipe_handles[1];
  char *from;
  ACE_NEW_RETURN (from, char[length], -1);

  while (!this->done ())
    if (ACE_OS::write (handle, from, length) != length)
      ACE_OS::perror ("write");

  ACE_OS::close (this->pipe_handles[0]);
  ACE_OS::close (this->pipe_handles[1]);

  return 0;
}

ACE_SVC_FACTORY_DECLARE (Pipe_Thr_Test)
ACE_SVC_FACTORY_DEFINE  (Pipe_Thr_Test)

// ACE_Service_Object_Type ptt (&pipe_thr_test, "Pipe_Thr_Test");
#endif /* ACE_HAS_THREADS */
