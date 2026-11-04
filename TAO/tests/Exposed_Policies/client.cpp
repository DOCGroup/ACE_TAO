// -- App. Specific Include --
#include "CounterC.h"
// #include "util.h"

// -- App. Specific Include --
#include "Policy_Verifier.h"

#include "tao/Strategies/advanced_resource.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int status {};

  try
    {
      Policy_Verifier policy_verifier;

      bool const retval = policy_verifier.init (argc, argv);

      if (retval)
        {
          policy_verifier.run ();
        }
      else
        {
          ++status;
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CORBA Exception Raised");
      ++status;
    }
  return status;
}
