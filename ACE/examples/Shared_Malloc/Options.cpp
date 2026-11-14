#include "ace/Get_Opt.h"
#include "Options.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdlib.h"

Options *
Options::instance ()
{
  static Options instance;
  return &instance;
}

std::string const &
Options::program_name () const
{
  return this->program_name_;
}

std::string const &
Options::slave_name () const
{
  return this->slave_name_;
}

bool
Options::debug () const
{
  return this->debug_;
}

bool
Options::exec_slave () const
{
  return this->exec_slave_;
}

size_t
Options::iteration_count () const
{
  return this->iteration_count_;
}

bool
Options::use_sbrk () const
{
  return this->use_sbrk_;
}

size_t
Options::max_msg_size () const
{
  return this->max_msg_size_;
}

size_t
Options::spawn_count () const
{
  return this->spawn_count_;
}

bool
Options::spawn_threads () const
{
  return this->spawn_threads_;
}

bool
Options::use_mmap () const
{
  return this->use_mmap_;
}

bool
Options::use_shmem () const
{
  return this->use_shmem_;
}

bool
Options::child () const
{
  return this->child_;
}

// Explain usage and exit.

void
Options::print_usage_and_die ()
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("usage: %n")
              ACE_TEXT ("\n[-d] (run in debugging mode)\n")
              ACE_TEXT ("[-e] (use exec(2) in addition to fork(2))\n")
              ACE_TEXT ("[-l] (use C++ new operator rather than sbrk(2)\n")
              ACE_TEXT ("[-L max_msg_size]\n")
              ACE_TEXT ("[-m] (use mmap rather than SysV shared memory)\n")
              ACE_TEXT ("[-p] (use processes rather than threads)\n")
              ACE_TEXT ("[-s] (use SysV shared memory rather than mmap)\n")
              ACE_TEXT ("[-t number of threads or processes to spawn]\n")
              ACE_TEXT ("[-T] (enable tracking)\n")
              ACE_TEXT ("[-n iteration_count]\n")));
  //FUZZ: enable check_for_lack_ACE_OS

  ACE_OS::exit (1);
  /* NOTREACHED */
}

void
Options::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("dehlL:mn:pst:T"));

  this->program_name_ = ACE_TEXT_ALWAYS_CHAR (argv[0]);
  ACE_LOG_MSG->open (argv[0]);

  // Put in a special-case check for child process.
  if (this->program_name_ == this->slave_name_)
    {
      this->child_ = true;
      this->use_mmap_ = true;
    }

  for (int c {};
       (c = get_opt ()) != -1;
       )
    {
      switch (c)
        {
        case 'd':
          this->debug_ = true;
          break;
        case 'e':
          this->exec_slave_ = true;
          break;
        case 'h':
          this->print_usage_and_die ();
          /* NOTREACHED */
          break;
        case 'l':
          this->use_sbrk_ = false;
          break;
        case 'L':
          this->max_msg_size_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'm':
          this->use_mmap_ = true;
          break;
        case 'n':
          this->iteration_count_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'p': // Spawn processes rather than threads
          this->spawn_threads_ = false;
          break;
        case 's':
          // Use System V shared memory...
          this->use_shmem_ = true;
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
  if (this->exec_slave_ && !this->spawn_threads_)
    this->use_mmap_ = true;
}
