//$Id$

////////////////////////////////////////////////////////////////////////
// This files tests the Client exposed policies:
//     - PriorityModelPolicy
//     - PriorityBandedConnectionPolicy
//     - ClientProtocolPolicy
//
// This policies are embedded in the object reference, by writem
// them into the IOR.
//
//
// -- App. Specific Include --
#include "Policy_Tester.h"

#include "tao/Strategies/advanced_resource.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      Policy_Tester policy_tester;

      int result = policy_tester.init (argc, argv);

      if (result != 0)
        return result;

      policy_tester.run ();
    }

  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("CORBA Exception Raised."));
      return 1;
    }


  return 0;
}
