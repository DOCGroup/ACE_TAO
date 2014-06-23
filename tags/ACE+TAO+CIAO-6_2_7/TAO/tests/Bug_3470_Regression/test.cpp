/* Test wether list_initial_services returns an ObjectURL or an ObjectId as
 * it should be when an ORBInitRef is set.
 *
 * launch test like this:
 *  <execname> -ORBInitRef MyObjectId=file://myobj.ior
 *
 * $Id$
 */

#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/ObjectIdListC.h"

int ACE_TMAIN( int argc, ACE_TCHAR * argv[] )
{
  int retcode = 1;

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::ORB_ObjectIdList_var listsrv = orb->list_initial_services();
      for (CORBA::ULong index = 0 ; index < listsrv->length() ; index++ )
        if ( ACE_OS::strcmp(listsrv[index],"MyObjectId") == 0)
          retcode = 0;

      orb->destroy();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return retcode;
}
