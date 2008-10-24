// $Id$

// Options.cpp,v Stoyan

#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "Options.h"


bool
Options::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("ov:n:l:u:t:icfdsTNa"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'o':
        this->write_to_ior_ = true;
        this->register_with_ns_ = false;
        break;
      case 'v':
        this->write_to_ior_ = false;
        this->register_with_ns_ = true;
        this->repoman_name_ = get_opt.opt_arg ();
        break;
      case 'i':
        this->install_ = true;
        break;
      case 'c':
        this->create_ = true;
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
      case 't':
        this->names_by_type_ = true;
        this->type_ = get_opt.opt_arg ();
        break;
        // Usage fallthrough.
      default:
        this->usage ();
        return false;
      }

  if ((this->name_ == "")
    && (this->shutdown_ == false)
    && (this->uuid_ == "")
    && (this->all_names_ == false)
    && (this->all_types_ == false)
    && (this->names_by_type_ == false))
  {
    this->usage ();
    return false;
  }
  else if (this->name_ != "")
  {
    if (!(this->install_ || this->create_ || this->find_ || this->delete_))
    {
      this->usage ();
      return false;
    }
    else if (this->install_ && this->path_ == "")
    {
      this->usage ();
      return false;
    }
    else if (this->create_ && this->path_ == "")
    {
      this->usage ();
      return false;
    }
  }
  else if (this->uuid_ != "")
  {
    if (!this->find_)
    {
      this->usage ();
      return false;
    }
  }

  return true;
}

/// @todo Exit is not nice, return -1 so that the caller can do something and
/// we don't exit abruptly
void Options::usage (void)
{
  DANCE_INFO (("OPTIONS: \n\
  -o <using ior file> \n\
  -v <: name of naming service> \n\
  -s <shutdown> \n\
  -n <:name> \n\
  [-i <install> -l <:path>] \n\
  [-c <create> -l <:path>] \n\
  [-d <delete>] \n\
  [-f <find>] \n\
  -u <:uuid> \n\
  [-f <find>] \n\
  -a <names by type> \n\
  [-t <:type>] \n\
  -N <all names> \n\
  -T <all types> \n"));
}

Options::Options (void)
  : name_ (""),
    uuid_ (""),
    type_ (""),
    path_ (""),
    delete_ (false),
    install_ (false),
    create_ (false),
    find_ (false),
    all_names_ (false),
    all_types_ (false),
    names_by_type_ (false),
    shutdown_ (false),
    register_with_ns_ (false),
    write_to_ior_ (true),
    repoman_name_ ("")
{
}
