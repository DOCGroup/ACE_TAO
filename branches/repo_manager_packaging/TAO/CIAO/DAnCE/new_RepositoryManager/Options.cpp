// $Id$

// Options.cpp,v Stoyan

#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "Options.h"

Options *Options::instance_ = 0;

Options *
Options::instance (void)
{

  if (Options::instance_ == 0) 
    Options::instance_ = new Options;

  return Options::instance_;
}

void
Options::parse_args (int argc, ACE_TCHAR *argv[])
{
	ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("n:l:a:idps"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'i':
        this->install_ = true;
        break;
      case 'd':
        this->delete_ = true;
        break;
      case 'p':
        this->plan_ = true;
        break;
	  case 's':
        this->shutdown_ = true;
        break;
      case 'n':
        this->package_ = get_opt.opt_arg ();
        break;
      case 'l':
        this->local_path_ = get_opt.opt_arg ();
        break;
      case 'a':
        this->artifact_ = get_opt.opt_arg ();
        break;
        // Usage fallthrough.
      default:
		  this->usage ();
		  
      }

  if ((this->package_ == "") && (this->shutdown_ == false)) 
	  this->usage ();
  if (this->package_ != "")
  {
	if (!(this->install_ || this->plan_ || this->delete_ || (this->artifact_ != "")))
	  this->usage ();
    else if (this->install_ && this->local_path_ == "")
	  this->usage ();
  }
}

void Options::usage (void)
{
	ACE_DEBUG ((LM_DEBUG, "OPTIONS: -s <shutdown> -n <:package> [-i <install> -l <:local_path>] \
		[-d <delete>] [-p <plan>] [-a <artifact>]\n"));
    ACE_OS::exit (1);
}


Options::Options (void) 
  : package_ (""),
	local_path_ (""),
	artifact_ (""),
	delete_ (false),
	install_ (false),
	plan_ (false),
	shutdown_ (false)
{
}
