// $Id$

#include "ace/Get_Opt.h"
#include "Options.h"
#include "ace/Log_Msg.h"

ACE_RCSID(Shared_Malloc, Options, "$Id$")

// Static Singleton instance.
Options *Options::instance_ = 0;

Options *
Options::instance (void)
{
  if (Options::instance_ == 0)
    Options::instance_ = new Options ();

  return Options::instance_;
}

char *
Options::program_name (void)
{ 
  return this->program_name_; 
}

const char *
Options::slave_name (void)
{ 
  return this->slave_name_; 
}

int 
Options::debug (void)
{ 
  return this->debug_; 
}

int 
Options::exec_slave (void)
{ 
  return this->exec_slave_; 
}

size_t 
Options::iteration_count (void)
{ 
  return this->iteration_count_; 
}

int 
Options::use_sbrk (void)
{ 
  return this->use_sbrk_; 
}

size_t 
Options::max_msg_size (void)
{ 
  return this->max_msg_size_; 
}

size_t 
Options::spawn_count (void)
{ 
  return this->spawn_count_; 
}

int 
Options::spawn_threads (void)
{ 
  return this->spawn_threads_; 
}

int 
Options::use_mmap (void)
{ 
  return this->use_mmap_; 
}

int 
Options::use_shmem (void)
{ 
  return this->use_shmem_; 
}

int 
Options::child (void)
{ 
  return this->child_; 
}

// Explain usage and exit.

void 
Options::print_usage_and_die (void)
{
  ACE_ERROR ((LM_ERROR, "usage: %n"
       "\n[-d] (run in debugging mode)\n"
       "[-e] (use exec(2) in addition to fork(2))\n"
       "[-l] (use C++ new operator rather than sbrk(2)\n"
       "[-L max_msg_size]\n"
       "[-m] (use mmap rather than SysV shared memory)\n"
       "[-p] (use processes rather than threads)\n"
       "[-s] (use SysV shared memory rather than mmap)\n"
       "[-t number of threads or processes to spawn]\n"
       "[-T] (enable tracking)\n"
       "[-n iteration_count]\n"));
  ACE_OS::exit (1);
  /* NOTREACHED */
}

Options::Options (void)
  : slave_name_ ("slave"),
    debug_ (0),
    exec_slave_ (0),
    iteration_count_ (100),
    use_sbrk_ (0),
    use_shmem_ (0),
    max_msg_size_ (127),
    spawn_count_ (1),
    spawn_threads_ (1),
    use_mmap_ (0),
    child_ (0)
{
}

void
Options::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "dehlL:mn:pst:T");

  this->program_name_ = argv[0];
  ACE_LOG_MSG->open (this->program_name_);

  // Put in a special-case check for child process.
  if (ACE_OS::strcmp (this->program_name_, slave_name_) == 0)
    {
      this->child_ = 1;
      this->use_mmap_ = 1;
    }

  for (int c;
       (c = get_opt ()) != -1;
       )
    {
      switch (c)
	{
	case 'd':
	  this->debug_ = 1;
	  break;
	case 'e':
	  this->exec_slave_ = 1;
	  break;
	case 'h':
	  this->print_usage_and_die ();
	  /* NOTREACHED */
	  break;
	case 'l':
	  this->use_sbrk_ = 0;
	  break;
	case 'L':
	  this->max_msg_size_ = ACE_OS::atoi (get_opt.opt_arg ());
	  break;
	case 'm':
	  this->use_mmap_ = 1;
	  break;
	case 'n':
	  this->iteration_count_ = ACE_OS::atoi (get_opt.opt_arg ());
	  break;
	case 'p': // Spawn processes rather than threads
	  this->spawn_threads_ = 0;
	  break;
	case 's':
	  // Use System V shared memory...
	  this->use_shmem_ = 1;
	  break;
	case 't':
	  this->spawn_count_ = ACE_OS::atoi (get_opt.opt_arg ());
	  break;
	case 'T':
#if defined (ACE_HAS_TRACE)
          ACE_Trace::start_tracing ();
#endif /* ACE_HAS_TRACE */
	  break;
	default:
	  this->print_usage_and_die ();
	  /* NOTREACHED */
	  break;
	}
    }

  // Switch to using MMAP when the -p and -e options are enabled.
  if (this->exec_slave_ == 1 && this->spawn_threads_ == 0)
    this->use_mmap_ = 1;
}
