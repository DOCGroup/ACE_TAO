//$Id$


/**
 * @file controller.cpp
 *
 * This program interact with a TSEC_CheckPoint component, using its supported
 * interface "ISessionService"
 */


//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------

#include <ace/streams.h>
#include <ace/Get_Opt.h>

#include "TSEC_CheckPointC.h"


//-------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------

#define SEPARATION_LINE "----------------------------------------" \
                        "----------------------------------------\n"

#define TESTCASE_NOT_DEFINED             0
#define TESTCASE_ACQUIRE_RELEASE_SESSION 1
#define TESTCASE_CREATE_DESTROY_SESSION  2


//-------------------------------------------------------------------
// Statics
//-------------------------------------------------------------------

static int              _testcase           = TESTCASE_NOT_DEFINED;
static int              _loop               = 0;
static int              _ident              = 1;
static const ACE_TCHAR* _sessionService_ior = 0;


//-------------------------------------------------------------------
// Function
//-------------------------------------------------------------------

int
parse_args
(
  int   argc,
  ACE_TCHAR* argv[]
)
{
  ACE_Get_Opt get_opts( argc, argv, "i:k:l:t:" );
  int c;

  while( ( c = get_opts() ) != -1 )
  {
    switch( c )
    {
      case 'i':
         _ident = ACE_OS::atoi( get_opts.opt_arg() );
         break;

      case 'k':
        _sessionService_ior = get_opts.opt_arg();
        break;

      case 'l':
        _loop = ACE_OS::atoi( get_opts.opt_arg() );
        break;

      case 't':
        _testcase = ACE_OS::atoi( get_opts.opt_arg() );
        break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN( ( LM_ERROR,
                            "usage:  %s\n"
                            "-i <Ident> (default is 1)\n"
                            "-k <TSEC_CheckPoint IOR> (default is file://TSEC_CheckPoint.ior)\n"
                            "-t <testcase number> (default is 1)\n"
                            "\n",
                            argv[0] ),
                            -1 );
      break;
    }
  }

  if ( _sessionService_ior == 0 )
  {
    _sessionService_ior = ACE_TEXT ("file://TSEC_CheckPoint.ior");
  }

  if( _testcase == 0 )
  {
    _testcase = TESTCASE_ACQUIRE_RELEASE_SESSION;
  }

  return 0;
}


//-------------------------------------------------------------------
// Function
//-------------------------------------------------------------------

int
ACE_TMAIN
(
  int   argc,
  ACE_TCHAR* argv[]
)
{
  try
  {
    // Initialize orb
    CORBA::ORB_var orb =
                        CORBA::ORB_init (argc, argv);

    if( parse_args (argc, argv) != 0 )
    {
      return -1;
    }

    CORBA::Object_var obj = orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (_sessionService_ior));

    ENW::ISessionService_var sessionService =
               ENW::ISessionService::_narrow (obj.in () );

    if( CORBA::is_nil( sessionService.in() ) )
    {
      ACE_ERROR_RETURN( ( LM_ERROR,
                          "Unable to acquire 'SessionService' objref\n" ),
                          -1 );
    }

    ACE_DEBUG( ( LM_DEBUG, SEPARATION_LINE ) );

    for( int i = 0; i < _loop; ++i )
    {
      switch( _testcase )
      {
        case TESTCASE_ACQUIRE_RELEASE_SESSION:
          {
            ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) Executing testcase acquire release session...[running]\n" ) );

            ENW::TSession_var session = sessionService->acquireSession( _ident );
            //CORBA::Long ident = session->getIdent();
            sessionService->releaseSession( _ident, session.in () );

            ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) Executing testcase acquire release session...[ready]\n" ) );
          }

          break;

        case TESTCASE_CREATE_DESTROY_SESSION:
          {
            ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) Executing testcase create destroy session...[running]\n" ) );

            ENW::TSession_var session = sessionService->createSession();
            CORBA::Long ident = session->getIdent();
            ACE_DEBUG(( LM_DEBUG, "(%P|%t@%T) Ident: %d\n", ident ) );

            if( !sessionService->destroySession( session.in () ) )
            {
              ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) Executing testcase create destroy session...[ERROR]\n" ) );
            }

            ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) Executing testcase create destroy session...[ready]\n" ) );
          }

          break;

        default:
          ACE_DEBUG( ( LM_DEBUG, "(%P|%t@%T) Testcase is undefined!\n" ) );
          break;

      }
    }

    orb->destroy( );
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Who is the culprit \n");
    cerr << "Uncaught CORBA exception" << endl;

    return 1;
  }

  return 0;
}

