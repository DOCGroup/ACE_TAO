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

void Options::destroy (void)
{
  delete instance_;
  instance_ = 0;
}

void
Options::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("n:l:u:ifdsTNa"));

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
      case 'f':
        this->find_ = true;
        break;
    case 's':
        this->shutdown_ = true;
        break;
      case 'n':
        this->name_ = get_opt.opt_arg ();
        break;
      case 'l':
        this->path_ = get_opt.opt_arg ();
        break;
      case 'u':
        this->uuid_ = get_opt.opt_arg ();
        break;
    case 'N':
    this->all_names_ = true;
    break;
    case 'T':
      this->all_types_ = true;
    break;
    case 'a':
      this->names_by_type_ = true;
    break;
        // Usage fallthrough.
      default:
      this->usage ();
      }

  if ((this->name_ == "")
    && (this->shutdown_ == false)
    && (this->uuid_ == "")
    && (this->all_names_ == false)
    && (this->all_types_ == false))
    this->usage ();
  else if (this->name_ != "")
  {
  if (!(this->install_ || this->find_ || this->delete_))
    this->usage ();
    else if (this->install_ && this->path_ == "")
    this->usage ();
  }
  else if (this->uuid_ != "")
  {
    if (!this->find_ && !this->names_by_type_)
      this->usage ();
  }
}

void Options::usage (void)
{
  ACE_DEBUG ((LM_DEBUG, "OPTIONS: -s <shutdown> -n <:name> [-i <install> -l <:path>] \
              [-d <delete>] [-f <find>] [-u <:uuid> [-a <names by type>] ] \
              [-N <all names>] [-T <all types>]\n"));
    ACE_OS::exit (1);
}


Options::Options (void)
  : name_ (""),
    uuid_ (""),
  path_ (""),
  delete_ (false),
  install_ (false),
  find_ (false),
  all_names_ (false),
  all_types_ (false),
  names_by_type_ (false),
  shutdown_ (false)
{
}
