// $Id$

#define ACE_BUILD_SVC_DLL
#include "ace/Get_Opt.h"
#include "ace/Dynamic_Service.h"
#include "Time_Date.h"

int 
DLL_ORB::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n\trunning ORB event loop\n\n"));

  ACE_TRY_NEW_ENV
    {
      // Run the ORB event loop in its own thread.
      this->orb_manager_.run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      ACE_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      ACE_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

int 
DLL_ORB::init (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV 
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n\tInitialize ORB\n\n"));

      // Initialize the ORB.
      this->orb_manager_.init (argc,
                               argv,
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Become an Active Object so that the ORB
      // will execute in a separate thread.
      return this->activate ();
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("DLL_ORB::init");
      return -1;
    }
  ACE_ENDTRY;
}

int 
DLL_ORB::fini (void)
{
  return 0;
}

int
Time_Date_Servant::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "dn:o:");
  int c = 0;

  this->orb_ = "ORB";

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;
        // Find the ORB in the Service Repository.
      case 'n':
        this->orb_ = get_opts.optarg;
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int 
Time_Date_Servant::init (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV 
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n\tTime_Date servant\n\n"));

      this->parse_args (argc, argv);
 
      DLL_ORB *orb =
        ACE_Dynamic_Service<DLL_ORB>::instance (this->orb_);

      if (orb == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "can't find %s in the Service Repository\n",
                           this->orb_),
                          -1);
 
      CORBA::String_var str = orb->orb_manager_.activate (&servant_, 
                                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;
 
      if (this->ior_output_file_)
        {
          ACE_OS::fprintf (this->ior_output_file_,
                           "%s",
                           str.in ());
          ACE_OS::fclose (this->ior_output_file_);
        }

    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("DLL_ORB::init");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

// The following Factory is used by the <ACE_Service_Config> and
// dll_orb.conf file to dynamically initialize the state of the 
// Time_Date service.
ACE_SVC_FACTORY_DEFINE (DLL_ORB)
ACE_SVC_FACTORY_DEFINE (Time_Date_Servant)
