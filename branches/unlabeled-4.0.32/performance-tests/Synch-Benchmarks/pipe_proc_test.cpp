#include "Options.h"
// @(#)pipe_proc_test.cpp	1.1	10/18/96

#include "Benchmark.h"

#if defined (ACE_HAS_THREADS)

extern int buffer;
extern int synch_count;

class Pipe_Proc_Test : public Benchmark
{
public:
  int init (int, char **);
  virtual int svc (void);

private:
  int pipe_fds[2];

  void reader (int fd);
};

int
Pipe_Proc_Test::init (int, char **)
{
  synch_count = 1;

  if (ACE_OS::pipe (this->pipe_fds) == -1)
    ACE_OS::perror ("pipe"), ACE_OS::exit (1);

  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_OS::perror ("fork"), ACE_OS::exit (1);
    case 0:
      this->reader (pipe_fds[0]);
      /* NOTREACHED */
      break;
    default:
      break;
    }
  return 1;
}

void
Pipe_Proc_Test::reader (int fd)
{
  int  ni     = this->thr_id ();
  int  length = options.msg_size ();
  char *to    = new char[length];
  int  n;

  while ((n = ACE_OS::read (fd, to, length)) > 0)
    options.thr_work_count[ni]++;
}


int
Pipe_Proc_Test::svc (void)
{
  size_t length = options.msg_size ();
  char	 *from  = new char[length];
  int	 ni     = this->thr_id ();
  int	 fd	= this->pipe_fds[1];

  while (!this->done ())
    if (ACE_OS::write (fd, from, length) == length)
      options.thr_work_count[ni]++;
    else
      ACE_OS::perror ("write");
    
  ACE_OS::close (this->pipe_fds[0]);
  ACE_OS::close (this->pipe_fds[1]);
  return 0;
}

extern "C" ACE_Service_Object *pipe_proc_test (void);

ACE_Service_Object *pipe_proc_test (void)
{
  return new Pipe_Proc_Test;
}

// ACE_Service_Object_Type ppt (&pipe_proc_test, "Pipe_Proc_Test");
#endif /* ACE_HAS_THREADS */
