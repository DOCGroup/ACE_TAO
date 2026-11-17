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
#include "ace/OS_NS_strings.h"
#include "ace/Tokenizer_T.h"

#include "Invocation.h"
#include "PeerObject.h"
#include "PeerProcess.h"
#include "Session.h"
#include "Log.h"

#include <cstring>
#include <fstream>
#include <string>

void
parse_filename (Session &session, char * buffer)
{
  Log log(session);
  if (std::strchr (buffer, '=') == nullptr)
    {
      log.process_file (ACE_TEXT_CHAR_TO_TCHAR(buffer));
    }
  else
    {
      ACE_Tokenizer_T<char> tokens(buffer);
      tokens.delimiter_replace('=', 0);
      char * const alias = tokens.next();
      char * const filename = tokens.next();
      if (alias == nullptr || filename == nullptr)
        {
          ACE_ERROR ((LM_ERROR, "invalid alias=filename input\n"));
          return;
        }
      ACE_TString const converted_filename = ACE_TEXT_CHAR_TO_TCHAR(filename);
      log.process_file (converted_filename.c_str(), alias);
    }
}

bool
manifest_value_available (std::string const &line, std::string::size_type offset)
{
  if (line.size () <= offset)
    {
      ACE_ERROR ((LM_ERROR, "manifest option is missing its value\n"));
      return false;
    }
  return true;
}

void
parse_manifest (Session &session, ACE_TCHAR const *filename)
{
  // get list of logs, aliases, and other config from file
  std::ifstream strm (ACE_TEXT_ALWAYS_CHAR (filename));
  if (!strm.is_open())
    {
      ACE_DEBUG ((LM_DEBUG,"cannot open manifest file %C\n", filename));
      return;
    }
  std::string line;
  while (std::getline (strm, line))
    {
      if (line.empty () || line[0] == '#')
        {
          continue;
        }
      if (line[0] == '-')
        {
          if (line.size () < 2)
            {
              ACE_ERROR ((LM_ERROR, "invalid manifest option\n"));
              continue;
            }
          if (line[1] == 'o')
            {
              if (!manifest_value_available (line, 3))
                continue;
              if (session.has_dir())
                {
                  ACE_ERROR ((LM_ERROR,
                              "supply either output file "
                              "or directory but not both\n"));
                  ACE_OS::exit (0);
                }
              session.outfile (line.data () + 3);
              continue;
            }
          if (line[1] == 'd')
            {
              if (session.has_outfile())
                {
                  ACE_ERROR ((LM_ERROR,
                              "supply either output file "
                              "or directory but not both\n"));
                  ACE_OS::exit (0);
                }
              if (line.size () > 2 && line[2] == 'd')
                {
                  if (!manifest_value_available (line, 4))
                    continue;
                  session.make_dir (line.data () + 4, true);
                }
              else
                {
                  if (!manifest_value_available (line, 3))
                    continue;
                  session.make_dir (line.data () + 3, false);
                }
              continue;
            }
          if (line[1] == 't')
            {
              if (!manifest_value_available (line, 3))
                continue;
              Session::set_tao_version (ACE_TEXT_CHAR_TO_TCHAR (line.data () + 3));
              continue;
            }
          if (line[1] == 'a')
            {
              if (!manifest_value_available (line, 3))
                continue;
              session.alternate_address (line.data () + 3);
              continue;
            }
          if (line[1] == 'p')
            {
              if (!manifest_value_available (line, 3))
                continue;
              session.default_service (line.data () + 3);
              continue;
            }
        }
      parse_filename (session, line.data ());
    }
}

void
print_help ()
{
  ACE_DEBUG ((LM_DEBUG, "tao_logWalker recongizes the following arguments\n"));
  ACE_DEBUG ((LM_DEBUG, "-o <filename> - write all output to specified file\n"));
  ACE_DEBUG ((LM_DEBUG, "-d[d] <directory> - create separate output files, one per log, and put them in specified directory.\n   Either -o or -d may be set but not both. Default output to stdout.\n   Use -dd to further split thread and peer process details onto multiple files."));
  ACE_DEBUG ((LM_DEBUG, "-m <manifest> - Take inputs from named manifest file\n"));
  ACE_DEBUG ((LM_DEBUG, "-t <1.5 .. 2.0>  - set source TAO version, default 2.0\n"));
  ACE_DEBUG ((LM_DEBUG, "-a <name=address> - bind an alias to a host address.\n   Repeat as many times as necessary.\n"));
  ACE_DEBUG ((LM_DEBUG, "-p <service=address> - bind a service such as Naming to a specific peer endpoint address\n"));
  ACE_DEBUG ((LM_DEBUG, "[alias=]filename - provide a source file with an altenate name"));
}

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  if (argc < 2)
    {
      ACE_ERROR ((LM_ERROR,
                  " At least one log file must be specified\n"));
      return 1;
    }
  Session session;
  for (int i = 1; i < argc; ++i)
    {
      if (ACE_OS::strcasecmp (argv[i], ACE_TEXT("-o")) == 0)
        {
          if (session.has_dir())
            ACE_ERROR_RETURN ((LM_ERROR,
                               "supply either output file "
                               "or directory but not both\n"), 1);
          if (i + 1 >= argc)
            ACE_ERROR_RETURN ((LM_ERROR, "-o requires an output filename\n"), 1);
          session.outfile(ACE_TEXT_ALWAYS_CHAR(argv[++i]));
          continue;
        }
      if (ACE_OS::strcasecmp (argv[i], ACE_TEXT("-d")) == 0 ||
          ACE_OS::strcasecmp (argv[i], ACE_TEXT("-dd")) == 0)
        {
          if (session.has_outfile())
            ACE_ERROR_RETURN ((LM_ERROR,
                               "supply either output file "
                               "or directory but not both\n"), 1);
          if (i + 1 >= argc)
            ACE_ERROR_RETURN ((LM_ERROR, "-d and -dd require an output directory\n"), 1);
          bool const split = ACE_OS::strcasecmp (argv[i], ACE_TEXT("-dd")) == 0;
          session.make_dir (ACE_TEXT_ALWAYS_CHAR(argv[++i]), split);
          continue;
        }
      if (ACE_OS::strcasecmp (argv[i], ACE_TEXT("-m")) == 0)
        {
          if (i + 1 >= argc)
            ACE_ERROR_RETURN ((LM_ERROR, "-m requires a manifest filename\n"), 1);
          parse_manifest (session, argv[++i]);
          continue;
        }
      if (ACE_OS::strcasecmp (argv[i], ACE_TEXT("-t")) == 0)
        {
          if (i + 1 >= argc)
            ACE_ERROR_RETURN ((LM_ERROR, "-t requires a TAO version\n"), 1);
          if (Session::set_tao_version (argv[++i]))
            continue;
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT("TAO version must be 1.5, 1.6, 1.7, 1.8, 2.0, or 2.2\n")), 1);
        }
      if (ACE_OS::strcasecmp (argv[i], ACE_TEXT("-a")) == 0)
        {
          if (i + 1 >= argc)
            ACE_ERROR_RETURN ((LM_ERROR, "-a requires an alias and address\n"), 1);
          session.alternate_address (ACE_TEXT_ALWAYS_CHAR (argv[++i]));
          continue;
        }
      if (ACE_OS::strcasecmp (argv[i], ACE_TEXT("-p")) == 0)
        {
          if (i + 1 >= argc)
            ACE_ERROR_RETURN ((LM_ERROR, "-p requires a service and address\n"), 1);
          session.default_service (ACE_TEXT_ALWAYS_CHAR (argv[++i]));
          continue;
        }
      if (argv[i][0] == ACE_TEXT('-'))
        {
          print_help ();
          return 1;
        }

      Log log(session);

      parse_filename (session, ACE_TEXT_ALWAYS_CHAR (argv[i]));
    }

  session.reconcile();

  session.dump();

  return 0;
}
