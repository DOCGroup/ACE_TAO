/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    ServantMain.cpp
 *
 *  $Id$
 *
 *  Implements a generic object that acts as "main" for a CORBA server.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
//
#ifndef TAO_UTILS_SERVANTMAIN_T_CPP
#define TAO_UTILS_SERVANTMAIN_T_CPP

#include "ServantMain.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_RCSID(Utils, ServantMain, "$Id$")

#include <ace/Argv_Type_Converter.h>
#include <tao/PortableServer/ORB_Manager.h>

template <typename SERVANT>
TAO::Utils::ServantMain<SERVANT>::ServantMain(const char * name)
  : name_(name)
{
}

template <typename SERVANT>
TAO::Utils::ServantMain<SERVANT>::~ServantMain()
{
}

template <typename SERVANT>
int TAO::Utils::ServantMain<SERVANT>::Main (int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  // hide unicode if necessary.
  ACE_Argv_Type_Converter command_line(argc, argv);

  char ** asciiArgv = command_line.get_ASCII_argv();

  ACE_TRY_NEW_ENV
  {
    // Create an object that manages all the
    // details of being a server.
    TAO_ORB_Manager orbManager;
    result = orbManager.init (argc, asciiArgv
        ACE_ENV_ARG_PARAMETER);
    if(result == 0)
    {
      ACE_CHECK_RETURN (-1);

      // create an instance of the servant and give it a
      // chance at the arguments.
      SERVANT servant;
      result = servant.parse_args (argc, asciiArgv);
      if (result == 0)
      {
        //////////////////////////////////
        // let the servant register itself
        result = servant.init(orbManager);
        if (result == 0)
        {
          ACE_ERROR ((LM_INFO,
            "%n\n%T: %s Ready %s\n", name_, servant.identity()
            ));

          //////////////////////////////////
          // Run the event loop for the ORB.
          // Initial run to initialize the orb
          ACE_Time_Value tv(1,0);
          result = orbManager.run (tv
            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          // now run event loop
          CORBA::ORB_var orb = orbManager.orb();
          int quit = 0;
          while (result == 0 && ! quit )
          {
            ACE_Time_Value work_tv(1,0);
            orb->perform_work(work_tv
              ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
            quit = servant.idle(result);
          }
          ACE_ERROR ((LM_DEBUG,
            "call fini"
            ));
          servant.fini(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_ERROR ((LM_DEBUG,
            "return fini"
            ));
          orb->shutdown(0 ACE_ENV_ARG_PARAMETER);
          ACE_ERROR ((LM_INFO,
            "%n\n%T: %s Terminated normally. %s\n", name_, servant.identity()
            ));
        }
        else
        {
          ACE_ERROR ((LM_ERROR,
            "%n\n%T: %s registration failed: %p\n", name_
            ));
          result = -1;
        }
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
          "%n\n%T: %s: ORB manager init failed\n", name_
        ));
        result = -1;
      }
    }
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
      name_);
    result = -1;
  }
  ACE_ENDTRY;
  return result;
}

#endif //TAO_UTILS_SERVANTMAIN_T_CPP
