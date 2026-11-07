/* Test wether list_initial_services returns an ObjectURL or an ObjectId as
 * it should be when an ORBInitRef is set.
 *
 * launch test like this:
 *  <execname> -ORBInitRef MyObjectId=file://myobj.ior
 */

#include <cstring>

#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/ObjectIdListC.h"

int
ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  int retcode {1};

  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::ORB_ObjectIdList_var const listsrv = orb->list_initial_services ();
      CORBA::ULong const service_count = listsrv->length ();
      for (CORBA::ULong index {}; index < service_count; ++index)
        if (std::strcmp (listsrv[index], "MyObjectId") == 0)
          retcode = 0;

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return retcode;
}
