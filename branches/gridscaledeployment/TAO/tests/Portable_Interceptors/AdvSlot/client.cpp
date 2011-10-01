// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "StateTransferC.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (argc != 2)
      {
        ACE_DEBUG ((LM_ERROR, "Usage: %s <ior>\n", argv[0]));
        return -1;
      }

      CORBA::Object_var obj = orb->string_to_object (argv[1]);
      StateTransfer_var server = StateTransfer::_narrow (obj.in ());

      CORBA::Short n  = server->number ();

      ACE_DEBUG ((LM_ERROR, "Got number %d\n", n));

      server->shutdown ();

    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("ERROR : unexpected CORBA exception caught : ");
      return 1;
    }
  return 0;
}
