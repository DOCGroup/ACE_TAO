//$Id$

#include "../lib/Driver.h"
#include "ace/OS_main.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  TAO_Notify_Tests_Driver driver;

  try
    {
      int result = driver.init (argc, argv);

      if (result == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("Failed to initialize the Notify Testing Driver.\n")),
                            1);

      driver.run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT("Error: \n"));
      return 1;
    }

  return 0;
}
