// ============================================================================
// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This program tests both the short and long option support in
//    <ACE_Get_Opt>, and demonstrates how to use it.
//
// = AUTHOR
//    Don Hinton <dhinton@dresystems.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/RAW_Socket.h"


#define EXCEPTION_RETURN(expression, message)\
do{\
    if(expression)\
    {\
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (message)), 1);\
}\
}while(0)


/*
 * This is the heart of the test. It test the api of RAW socket class
 *
 * It returns 0 for success and 1 for error so we can keep track of the
 * total error count.
 */


static int
run_option_test ()
{
 
  ACE_DEBUG ((LM_INFO, "%s begin to run ...\n", __func__));
  
  ACE_INET_Addr   addr;
  ACE_INET_Addr (12711, "127.0.0.1");
  ACE_RAW_SOCKET  rawSocket(addr);

  EXCEPTION_RETURN(rawSocket.get_handle() == ACE_INVALID_HANDLE, "  can not bind the addr\n");

  int rc = rawSocket.enable(ACE_NONBLOCK);

  EXCEPTION_RETURN(rc < 0, "  can not bind the addr\n");


  int optval = 0, optlen = sizeof(optval);
  rc = rawSocket.get_option(SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
  
  EXCEPTION_RETURN(rc < 0, "  get SO_RCVBUF in failure\n");

  optlen  = sizeof(optval);
  int new_optval = optval << 1;
  rc = rawSocket.set_option(SOL_SOCKET, SO_RCVBUF, &new_optval, sizeof(new_optval));

  EXCEPTION_RETURN(rc < 0, "  set SO_RCVBUF new value in failure\n");

  new_optval = 0;
  optlen  = sizeof(new_optval);

  rawSocket.get_option(SOL_SOCKET, SO_RCVBUF, &new_optval, &optlen);

  EXCEPTION_RETURN(new_optval < optval, "  set SO_RCVBUF with no effect\n");

  ACE_DEBUG ((LM_INFO, "old optval: %d, new optval ...\n", optval, new_optval));

  return 1;
}



int
run_main (int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("RAW_Socket_Test"));
  ACE_UNUSED_ARG (argv);
  int retval = 0;
 

  // Run the tests for each type of ordering.
  retval = run_option_test ();
  

  ACE_END_TEST;

  return retval;
}
