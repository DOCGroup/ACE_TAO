#include "ace/Thread_Manager.h"
// $Id$

#include "Options.h"
#include "Benchmark.h"

#if defined (ACE_HAS_THREADS)

extern int synch_count;

class Pipe_Thr_Test : public Benchmark
{
public:
  virtual int init (int, char **);
  virtual int svc (void);

private:
  int pipe_fds[2];

  static void *reader (Pipe_Thr_Test *);
};

void *
Pipe_Thr_Test::reader (Pipe_Thr_Test *t)
{
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ());

  int fd = t->pipe_fds[0];
  int  ni     = t->thr_id ();
  int  length = options.msg_size ();
  char *to    = new char[length];
  int  n;

  while ((n = ACE_OS::read (fd, to, length)) > 0)
    options.thr_work_count[ni]++;

  return 0;
}

int
Pipe_Thr_Test::init (int, char **)
{
  synch_count = 1;

  if (ACE_OS::pipe (this->pipe_fds) == -1)
    ACE_OS::perror ("pipe"), ACE_OS::exit (1);

  if (ACE_Service_Config::thr_mgr ()->spawn 
      (ACE_THR_FUNC (Pipe_Thr_Test::reader),
       (void *) this, options.t_flags ()) == -1)
    ACE_OS::perror ("thr_create"), ACE_OS::exit (1);

  return 1;
}

int
Pipe_Thr_Test::svc (void)
{
  size_t length = options.msg_size ();
  char	 *from  = new char[length];
  int	 fd     = this->pipe_fds[1];

  while (!this->done ())
    if (ACE_OS::write (fd, from, length) != length)
      ACE_OS::perror ("write");

  ACE_OS::close (this->pipe_fds[0]);
  ACE_OS::close (this->pipe_fds[1]);

  return 0;
}

extern "C" ACE_Service_Object *pipe_thr_test (void);

ACE_Service_Object *pipe_thr_test (void)
{
  return new Pipe_Thr_Test;
}

// ACE_Service_Object_Type ptt (&pipe_thr_test, "Pipe_Thr_Test");
#endif /* ACE_HAS_THREADS */
