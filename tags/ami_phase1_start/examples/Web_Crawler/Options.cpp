// $Id$

#include "ace/Get_Opt.h"
#include "URL_Addr.h"
#include "Options.h"

ACE_RCSID(Web_Crawler, Options, "$Id$")

int 
Options::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt getopt (argc, argv, ASYS_TEXT ("df:h:i:l:rt:u:vo:p:"));

  ACE_LOG_MSG->open (argv[0]);

  this->hostname_ = "tao.cs.wustl.edu";
  this->uri_ = "index.html";//"~kirthika/auto_purge_client.html";
  this->recurse_ = 0;
  this->debug_ = 0;
  this->timeout_.sec (ACE_DEFAULT_TIMEOUT);
  this->url_filter_ = 0;
  this->verbose_ = 0;
  this->order_ = "FIFO";
  this->port_no_ = ACE_DEFAULT_HTTP_PORT;
  this->handle_limit_ = 64;
  // The default is to make this limit as large as possible.
  //  int handle_limit = 10;

  for (int c;
       (c = getopt ()) != EOF;
       )
    switch (c)
      {
      case 'd':
        this->debug_ = 1;
        break;
      case 'f':
        this->url_filter_ = getopt.optarg;
        break;
      case 'h':
        this->hostname_ = getopt.optarg;
        break;
      case 'i':
        this->uri_ = getopt.optarg;
        break;
      case 'l':
        this->handle_limit_ = ACE_OS::atoi (getopt.optarg);
        break;
      case 'r':
        this->recurse_ = 1;
        break;
      case 't':
        this->timeout_.sec (ACE_OS::atoi (getopt.optarg));
        break;
      case 'u':
        {
          this->hostname_ = getopt.optarg;
          char *s = ACE_OS::strchr (getopt.optarg, '/');
          if (s != 0)
            {
              this->uri_ = s + 1;
              *s = '\0';
            }
          else
            ACE_ERROR ((LM_ERROR,
                        "invalid URL %s\n",
                        getopt.optarg));
        }
        break;
      case 'v':
        this->verbose_ = 1;
        break;
      case 'o':
        {
        this->order_ = getopt.optarg;
        }
        break;
      case 'p':
        this->port_no_ = ACE_OS::atoi (getopt.optarg);
        break;
      default:
        ACE_ERROR ((LM_ERROR,
                    "usage: %n [-d] [-f filter] [-h hostname]"
                    " [-l handle-limit] [-r] [-t timeout] [-u URI]"
                    " [-v]\n%a",
                    1));

        /* NOTREACHED */
      }

  // Don't bother checking the return value since this is just a
  // "hint" and isn't portable to all OS platforms.
  
  //   ACE::set_handle_limit (handle_limit);
  /* ACE_DEBUG ((LM_DEBUG, "Changing rlimit\n"));
  struct rlimit rl;
  if (getrlimit (RLIMIT_NOFILE, &rl) == -1)
    cout << "getrlimit: errno = "<< errno <<endl;
  
  rl.rlim_cur = handle_limit;
  if (setrlimit (RLIMIT_NOFILE, &rl) == -1)
   cout << "setrlimit: errno = "<< errno <<endl;
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p \n"),
                       -1);
  if (getrlimit (RLIMIT_NOFILE, &rl) == -1)
    cout << "getrlimit: errno = "<< errno <<endl;
  else 
    cout << "limit "<< rl.rlim_cur<<endl;
  ACE_DEBUG ((LM_DEBUG, "Changed rlimit\n")); */
  return 0;
}

int 
Options::port_no (void) const
{
  return this->port_no_;
}

int
Options::recurse (void) const
{
  return this->recurse_;
}

const ACE_Time_Value *
Options::timeout (void) const
{
  return &this->timeout_;
}

int
Options::debug (void) const
{
  return this->debug_;
}

int
Options::verbose (void) const
{
  return this->verbose_;
}

LPCTSTR 
Options::order (void) const
{
  return this->order_;
}
LPCTSTR
Options::hostname (void) const
{
  return this->hostname_;
}

LPCTSTR
Options::path_name (void) const
{
  return this->uri_;
}

LPCTSTR
Options::url_filter (void) const
{
  return this->url_filter_;
}

Command_Processor *
Options::command_processor (void) const
{
  return this->command_processor_;
}

void
Options::command_processor (Command_Processor *cp)
{
  this->command_processor_ = cp;
}

URL_Visitor *
Options::visitor (void) const
{
  return this->visitor_;
}

void
Options::visitor (URL_Visitor *v)
{
  this->visitor_ = v;
}

int 
Options::handle_limit (void)
{
  return this->handle_limit_;
}
