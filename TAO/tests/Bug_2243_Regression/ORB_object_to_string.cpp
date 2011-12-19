// -*- C++ -*-
// $Id$
// Adapted by Jules Colding <colding@omesc.com>

#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/SystemException.h"

#include "ace/Log_Msg.h"

// Valid test IOR.
// Do not attempt to narrow the object represented by this IOR, nor
// should you modify the IOR unless you replace it with another
// valid one!
static const char IOR[] =
"IOR:010000001600000049444c3a43756269745f466163746f72793a312e30000000010000000000000090000000010102cd14000000616e647572696c2e6563652e7563692e6564750057fecdcd2d00000014010f004e5550000000130000000001000000006368696c645f706f61000000000001000000666163746f7279cdcdcd03000000000000000800000001cdcdcd004f4154010000001400000001cdcdcd01000100000000000901010000000000004f41540400000001cd0000";

static const char ORBIT2_IOR[] =
"IOR:010000002600000049444c3a6f6d632e6272757475732f4252555455532f427275747573436865636b3a312e30000000040000000054424f580000000101020005000000554e495800000000100000006f6d632d322e6f6d6573632e636f6d00290000002f746d702f6f726269742d65766f2f6c696e632d313534642d302d343561373635656463306432640000000000000000000000004000000001010200100000006f6d632d322e6f6d6573632e636f6d003c8000001c00000000000000c080313dc3caa1c3462024c0964c4d0501000000e1b8086800000000caaedfba5400000001010200290000002f746d702f6f726269742d65766f2f6c696e632d313534642d302d34356137363565646330643264000000001c00000000000000c080313dc3caa1c3462024c0964c4d0501000000e1b8086801000000480000000100000002000000050000001c00000000000000c080313dc3caa1c3462024c0964c4d0501000000e1b8086801000000140000000100000001000105000000000901010000000000";

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // --------------------------------------------------------
      // Create an object (but don't narrow() it) so we can test
      // that we can stringify it later
      // --------------------------------------------------------

      CORBA::Object_var object = orb->string_to_object (IOR);
      CORBA::String_var object_ref_str = orb->object_to_string (object.in());
      if (0 == object_ref_str.in())
        {
          ACE_DEBUG ((LM_ERROR,
                      "\n"
                      "ORB_object_to_string test failed with TAO test IOR.\n"));
          return 1;
        }
      else
        {
          ACE_DEBUG ((LM_INFO,
                      "\n"
                      "ORB_object_to_string test passed with TAO test IOR."));
        }

      object = orb->string_to_object (ORBIT2_IOR);
      object_ref_str = orb->object_to_string (object.in());
      if (0 == object_ref_str.in())
        {
          ACE_DEBUG ((LM_ERROR,
                      "\n"
                      "ORB_object_to_string test failed with ORBit2 test IOR.\n"));
          return 1;
        }
      else
        {
          ACE_DEBUG ((LM_INFO,
                      "\n"
                      "ORB_object_to_string test passed with ORBit2 test IOR."));
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      ACE_DEBUG ((LM_ERROR,
                  "\n"
                  "ORB_object_to_string test failed.\n"));
      return 1;
    }

  ACE_DEBUG ((LM_INFO,
              "\n"
              "ORB_object_to_string test completed successfully.\n"));

  return 0;
}
