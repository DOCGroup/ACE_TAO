// $Id$

#include "ciao/Container_Base.h"
#include "ace/SString.h"
#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"
#include "ciao/HomeRegistrar_i.h"

char *ior_file_name_ = 0;
char *component_list_ = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':  // get the file name to write to
       ior_file_name_ = get_opts.opt_arg ();
      break;

      case 'i':                 // get
       component_list_ = get_opts.opt_arg ();
      break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "-i <config file>"
                           "-o <ior_output_file>"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
write_IOR(const char *filename
          const char *ior)
{
  if (filename == 0 || ior == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "A valid filename and an IOR string are required for saving IOR\n"),
                      -1);


  FILE* ior_output_file_ =
    ACE_OS::fopen (filename, "w");

  if (ior_output_file_ == NULL)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open <%s> for writing\n", filename),
                      -1);

  ACE_OS::fprintf (ior_output_file_,
                   "%s",
                   ior);

  ACE_OS::fclose (ior_output_file_);

  return 0;
}

int breakdown (char *source,
               int len,
               char *list[])
{
  ACE_Tokenizer line (source);
  line.delimiter_replace ('|', 0);

  int cntr = 0;
  char *p;
  for (p = line.next ();
       p && cntr < len;
       ++cntr, p=line.next ())
    list[cntr] = ACE::strnew (p);

  return cntr;
}

void
install_homes (CIAO::Session_Container &container,
               CORBA::ORB_ptr orb,
               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
  if (component_list_ == 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  FILE* config_file =
    ACE_OS::fopen (component_list_, "r");

  if (config_file)
    {
      ACE_Read_Buffer ior_buffer (config_file);
      char *data = 0;
      while ((data = ior_buffer.read ('\n')) != 0)
        {
          char *items[10];
          auto_ptr<char> an_entry (data);
          int len = breakdown (an_entry.get (),
                               10,
                               items);

          if (len < 7)
            {
              ACE_DEBUG ((LM_DEBUG, "Error parsing configuration file\n"));
              continue;
            }

          // len should be at least such and such long so we have all
          // the information we need.  These items are separate with
          // character '|'.

          // 0 -> DLL path to home executor
          // 1 -> entry point for home executor factory
          // 2 -> DLL path to servant glue code
          // 3 -> entry point for servant glue code factory

          // we are not using these until we support HomeFinder.

          // 4 -> Repository ID for home interface
          // 5 -> Repository ID for managed component
          // 6 -> Canonical Name of home interface.

          Components::CCMHome_var home =
            container.ciao_install_home (items[0],
                                         items[1],
                                         items[2],
                                         items[3]
                                         ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (CORBA::is_nil (home))
            {
              ACE_DEBUG ((LM_DEBUG, "Fail to create %s\n", items[6]));
              continue;
            }
        }
    }
  ACE_OS::fclose (config_file);
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return -1;

      // Get reference to Root POA.
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Activate POA manager
      PortableServer::POAManager_var mgr
        = poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Configuring HomeRegistrar.
      obj = orb->resolve_initial_references ("NameService"
                                             ACE_ENV_ARG_PARAMETER);
      CosNaming::NamingContext_var ns = CosNaming::NamingContext::_narrow (obj);
      if (CORBA::is_nil (ns))
        return -1;

      PortableServer::Servant hr_svt = new CIAO::HomeRegistrar_Impl (ns);
      PortableServer::ObjectId_var hr_oid
        = poa->activate_object (hr_svt
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      obj = poa->id_to_reference (hr_oid
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::HomeRegistrar_var home_registrar =
        CIAO::HomeRegistrar::_narrow (obj
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (home_registrar))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to acquire HomeRegistrar interface\n"), -1);

      CORBA::String_var str = orb->object_to_string (home_registrar.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      write_IOR (str.in ());
      ACE_DEBUG ((LM_INFO, "HomeFinder IOR: %s\n", str.in ()));

      // Start Deployment part

      CIAO::Session_Container container (orb);
      container.init ();

      // install component

      install_homes (container, orb, home_registrar ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // End Deployment part

      ACE_DEBUG ((LM_DEBUG,
                  "Running generic server...\n"));

      // Run the main event loop for the ORB.
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "server::main\t\n");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
