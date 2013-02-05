// $Id$

// Utility to traverse and simplify verbose log files.
//
// The goal is to take a collection of log files and extract details
// related to connections, objects, and invocations to separate out
// perhaps multiple processes, or at least threads to get a sense of
// invocation lifecycle.
//
// Ideally a collection of files could be used so that invocations
// that traverse many processes could be tracked.

#include "ace/OS_NS_stdio.h"
#include "ace/Log_Msg.h"
#include "ace/streams.h"
#include "ace/OS_NS_strings.h"
#include "ace/Tokenizer_T.h"

#include "Invocation.h"
#include "PeerObject.h"
#include "PeerProcess.h"
#include "Session.h"
#include "Log.h"

ACE_TCHAR *outfile = 0;

void
parse_filename (Session &session, char * buffer)
{
  Log log(session);
  if (ACE_OS::strchr(buffer,'=') == 0)
    {
      log.process_file (ACE_TEXT_CHAR_TO_TCHAR(buffer));
    }
  else
    {
      ACE_Tokenizer_T<char> tokens(buffer);
      tokens.delimiter_replace('=', 0);
      char *alias = tokens.next();
      ACE_TString filename = ACE_TEXT_CHAR_TO_TCHAR(tokens.next());
      log.process_file (filename.c_str(), alias);
    }
}

void
parse_manifest (Session &session, ACE_TCHAR *filename)
{
  // get list of logs, aliases, and other config from file
  ifstream strm (ACE_TEXT_ALWAYS_CHAR (filename));
  if (!strm.is_open())
    {
      ACE_DEBUG ((LM_DEBUG,"cannot open manifest file %C\n", filename));
      return;
    }
  char buffer[500];
  while (!strm.eof())
    {
      strm.getline(buffer,500);
      if (buffer[0] == '\0' ||
          buffer[0] == '#')
        {
          continue;
        }
      if (buffer[0] == '-')
        {
          if (buffer[1] == 'o')
            {
              if (session.has_dir())
                {
                  ACE_ERROR ((LM_ERROR,
                              "supply either output file "
                              "or directory but not both\n"));
                  ACE_OS::exit (0);
                }
              session.outfile (buffer+3);
              continue;
            }
          if (buffer[1] == 'd')
            {
              if (session.has_outfile())
                {
                  ACE_ERROR ((LM_ERROR,
                              "supply either output file "
                              "or directory but not both\n"));
                  ACE_OS::exit (0);
                }
              session.make_dir (buffer+3);
              continue;
            }
          if (buffer[1] == 't')
            {
              Session::set_tao_version (ACE_TEXT_CHAR_TO_TCHAR (buffer + 3));
              continue;
            }
          if (buffer[1] == 'a')
            {
              session.alternate_address(buffer+3);
              continue;
            }
          if (buffer[1] == 'p')
            {
              session.default_service (buffer+3);
              continue;
            }
        }
      parse_filename (session, buffer);
    }
}

void
print_help (void)
{
  ACE_DEBUG ((LM_DEBUG, "tao_logWalker recongizes the following arguments\n"));
  ACE_DEBUG ((LM_DEBUG, "-o <filename> - write all output to specified file\n"));
  ACE_DEBUG ((LM_DEBUG, "-d <directory> - create separate output files, one per log, and put them in specified directory.\n   Either -o or -d may be set but not both. Default output to stdout.\n"));
  ACE_DEBUG ((LM_DEBUG, "-m <manifest> - Take inputs from named manifest file\n"));
  ACE_DEBUG ((LM_DEBUG, "-t <1.5 .. 2.0>  - set source TAO version, default 2.0\n"));
  ACE_DEBUG ((LM_DEBUG, "-a <name=address> - bind an alias to a host address.\n   Repeat as many times as necessary.\n"));
  ACE_DEBUG ((LM_DEBUG, "-p <service=address> - bind a service such as Naming to a specific endpoint address\n"));
}

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  if (argc < 2)
    {
      ACE_ERROR ((LM_ERROR,
                  " At least one log file must be specified\n"));
      return 0;
    }
  Session session;
  for (int i = 1; i < argc; i++)
    {
      if (ACE_OS::strcasecmp (argv[i], ACE_TEXT("-o")) == 0)
        {
          if (session.has_dir())
            ACE_ERROR_RETURN ((LM_ERROR,
                               "supply either output file "
                               "or directory but not both\n"), 0);
          session.outfile(ACE_TEXT_ALWAYS_CHAR(argv[++i]));
          continue;
        }
      if (ACE_OS::strcasecmp (argv[i], ACE_TEXT("-d")) == 0)
        {
          if (session.has_outfile())
            ACE_ERROR_RETURN ((LM_ERROR,
                               "supply either output file "
                               "or directory but not both\n"), 0);
          session.make_dir (ACE_TEXT_ALWAYS_CHAR(argv[++i]));
          continue;
        }
      if (ACE_OS::strcasecmp (argv[i], ACE_TEXT("-m")) == 0)
        {
          parse_manifest (session, argv[++i]);
          continue;
        }
      if (ACE_OS::strcasecmp (argv[i], ACE_TEXT("-t")) == 0)
        {
          if (Session::set_tao_version (argv[++i]))
            continue;
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT("TAO version must be 1.5, 1.6, 1.7, 1.8, or 2.0 \n")), 0);
        }
      if (ACE_OS::strcasecmp (argv[i], ACE_TEXT("-a")) == 0)
        {
          session.alternate_address (ACE_TEXT_ALWAYS_CHAR (argv[++i]));
          continue;
        }
      if (ACE_OS::strcasecmp (argv[i], ACE_TEXT("-p")) == 0)
        {
          session.default_service (ACE_TEXT_ALWAYS_CHAR (argv[++i]));
          continue;
        }
      if (argv[i][0] == ACE_TEXT('-'))
        {
          print_help ();
          return 0;
        }

      Log log(session);

      parse_filename (session, ACE_TEXT_ALWAYS_CHAR (argv[i]));
    }

  session.reconcile();

  session.dump();

  return 0;
}
