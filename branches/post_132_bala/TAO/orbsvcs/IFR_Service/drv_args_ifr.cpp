/* -*- C++ -*- */
// $Id$

#include "idl_defines.h"
#include "global_extern.h"
#include "drv_extern_ifr.h"
#include "be_extern.h"
#include "ace/Process.h"

ACE_RCSID (IFR_Service, 
           drv_args_ifr, 
           "$Id$")

extern long DRV_nfiles;
extern const char *DRV_files[];

// Push a file into the list of files to be processed
static void
DRV_push_file (const char *s)
{
  DRV_files[DRV_nfiles++] = s;
}

// Print a usage message and exit.
void
DRV_usage (void)
{
  ACE_DEBUG ((LM_DEBUG,
              idl_global->prog_name (),
              ACE_TEXT (": usage: "),
              idl_global->prog_name (),
              ACE_TEXT (" [flag | file]*\n")));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Legal flags:\n")));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Cw\t\t\tWarning if identifier spellings differ ")
      ACE_TEXT ("only in case (default is error)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Ce\t\t\tError if identifier spellings differ ")
      ACE_TEXT ("only in case (default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -d\t\tOutputs (to stdout) a dump of the AST\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Dname[=value]\t\tdefines name for preprocessor\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -E\t\t\truns preprocessor only, prints on stdout\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Idir\t\t\tincludes dir in search path for preprocessor\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -L\t\t\tEnable locking at the IDL file level\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -r\t\t\tRemove contents of IDL file(s) from repository\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Si\t\t\tSuppress processing of included IDL files\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -t\t\t\tTemporary directory to be used")
      ACE_TEXT (" by the IDL compiler.\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Uname\t\t\tundefines name for preprocessor\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -A...\t\t\tlocal implementation-specific escape\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -u\t\t\tprints usage message and exits\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -v\t\t\ttraces compilation stages\n")
   ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -w\t\t\tsuppresses IDL compiler warning messages\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Yp,path\t\tdefines location of preprocessor\n")
    ));
}

// Parse arguments on command line
void
DRV_parse_args (int ac, char *av[])
{
  char *buffer = 0;
  char *s = 0;

  DRV_cpp_init ();
  idl_global->set_prog_name (av[0]);

  for (int i = 1; i < ac; i++)
    {
      if (av[i][0] == '-')
        {
          idl_global->append_idl_flag (av[i]);

          switch (av[i][1])
            {
            case 0:
              DRV_push_file ("standard input");
              break;
            case 'A':
              if (av[i][2] == '\0')
                {
                  if (i < ac - 1)
                    {
                      i++;
                      s = av[i];
                    }
                  else
                    {
                      ACE_OS::exit (99);
                    }
                }
              else
                {
                  s = av[i] + 2;
                }

              ACE_OS::strcat (idl_global->local_escapes (), s);
              ACE_OS::strcat (idl_global->local_escapes (), " ");
              break;
            case 'D':
            case 'U':
            case 'I':
              if (av[i][2] == '\0')
                {
                  if (i < ac - 1)
                    {
                      idl_global->append_idl_flag (av[i+1]);

                      ACE_NEW (buffer,
                               char[ACE_OS::strlen (av[i]) 
                                    + ACE_OS::strlen (av[i + 1]) 
                                    + 2]);

                      ACE_OS::sprintf (buffer, 
                                       "%s%s", 
                                       av[i], 
                                       av[i+1]);

                      DRV_cpp_putarg (buffer);
                      i++;
                    }
                  else
                    {
                      ACE_ERROR ((
                          LM_ERROR,
                          ACE_TEXT ("IDL: I don't understand")
                          ACE_TEXT (" the '%s' option\n"),
                          av[i]
                        ));

                      ACE_OS::exit (99);
                    }
                }
              else
                {
                  DRV_cpp_putarg (av[i]);
                }
              break;
            case 'E':
              idl_global->set_compile_flags (idl_global->compile_flags () 
                                             | IDL_CF_ONLY_PREPROC);
              break;
            case 'L':
              be_global->enable_locking (I_TRUE);
              break;
            case 'd':
              idl_global->set_compile_flags (idl_global->compile_flags ()
                                             | IDL_CF_DUMP_AST);
              break;
              // Temp directory for the IDL compiler to keep its files.
            case 'r':
              be_global->removing (I_TRUE);
              break;
            case 't':
              idl_global->append_idl_flag (av[i+1]);
              idl_global->temp_dir (av[i+1]);
              i++;
              break;
            case 'u':
              idl_global->set_compile_flags (idl_global->compile_flags ()
                                             | IDL_CF_ONLY_USAGE);
              break;
            case 'v':
              idl_global->set_compile_flags (idl_global->compile_flags ()
                                             | IDL_CF_INFORMATIVE);
              break;
            case 'w':
              idl_global->set_compile_flags (idl_global->compile_flags ()
                                             | IDL_CF_NOWARNINGS);
              break;
            case 'S':
              // Suppress ...
              if (av[i][2] == 'i')
                {
                  // ... processing of included IDL files.
                  be_global->do_included_files (0);
                }
              else
                {
                  ACE_ERROR ((
                      LM_ERROR,
                      ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
                      av[i]
                    ));

                  ACE_OS::exit (99);
                }
              break;
            case 'Y':
              if (av[i][2] == '\0')
                {
                  if (i < ac - 1)
                    {
                      i++;
                      s = av[i];
                    }
                  else
                    {
                      ACE_ERROR ((
                          LM_ERROR,
                          ACE_TEXT ("IDL: I don't understand")
                          ACE_TEXT (" the '%s' option\n"),
                          av[i]
                        ));

                      ACE_OS::exit (99);
                    }
                }
              else
                {
                  s = av[i] + 2;
                }

              switch (*s)
                {
                case 'p':
                  if (*(s + 1) == ',')
                    {
                      idl_global->set_cpp_location (s + 2);
                      DRV_cpp_new_location (s + 2);
                    }
                  break;
                default:
                  break;
                }
              break;
            default:
              ACE_ERROR ((
                  LM_ERROR,
                  ACE_TEXT ("IDL: I don't understand the '%s' option\n"),
                  av[i]
                ));

              idl_global->set_compile_flags (idl_global->compile_flags () 
                                             | IDL_CF_ONLY_USAGE);
              break;
            }
        }
      else
        {
          DRV_push_file (av[i]);
        }
    }

  // Make sure the temporary file directory is valid.
  if (idl_global->temp_dir () == 0)
    {
      char tmpdir[MAXPATHLEN + 1];

      if (ACE::get_temp_dir (tmpdir, MAXPATHLEN) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Temporary path too long, ")
                      ACE_TEXT ("defaulting to current directory\n")));

          ACE_OS::strcpy (tmpdir, ".");
        }

#if defined (ACE_MVS)
      if (ACE_OS::access (tmpdir, F_OK) == -1
          || ACE_OS::access (tmpdir, R_OK) == -1
          || ACE_OS::access (tmpdir, W_OK) == -1)
#else
      if (ACE_OS::access (tmpdir, F_OK | R_OK | W_OK) == -1)
#endif /* ACE_MVS */
        {
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("Can't access temporary directory ("),
              tmpdir,
              ACE_TEXT ("), using current directory for temp files.\n")
            ));

          ACE_OS::strcpy (tmpdir, ".");
#if defined (ACE_MVS)
          if (ACE_OS::access (tmpdir, F_OK) == -1
              || ACE_OS::access (tmpdir, R_OK) == -1
              || ACE_OS::access (tmpdir, W_OK) == -1)
#else
          if (ACE_OS::access (tmpdir, F_OK | R_OK | W_OK) == -1)
#endif /* ACE_MVS */
            {
              ACE_ERROR ((
                  LM_ERROR,
                  "%s%s\n",
                  ACE_TEXT ("Error: Can't access temporary directory ("),
                  tmpdir
                ));

              ACE_OS::exit (99);
            }
        }

      idl_global->temp_dir (tmpdir);
    }
}

