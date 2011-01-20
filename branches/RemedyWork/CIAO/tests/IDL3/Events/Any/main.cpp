// $Id$

#include "EventAnyC.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"

void
insert_into_any (CORBA::Any& any, Components::EventBase* vb)
{
  any <<= vb;
}

void
debug_msg (const char *msg)
{
  ACE_ERROR ((LM_ERROR, "Error: %s\n", msg));
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      StringMsg_init *StringMsg_factory = new StringMsg_init;

      orb->register_value_factory (StringMsg_factory->tao_repository_id (),
                                   StringMsg_factory);
      StringMsg_factory->_remove_ref (); // release ownership

      const char *test_str = "a message";

      StringMsg_var ev = new OBV_StringMsg;
      ev->str_msg (test_str);

      CORBA::Any in_any;
      insert_into_any (in_any, ev.in ());

      TAO_OutputCDR out;
      CORBA::Boolean good = out << in_any;

      if (!good)
        {
          debug_msg ("Any marshaling failed");
          return -1;
        }

      TAO_InputCDR in (out);
      CORBA::Any out_any;
      good = in >> out_any;

      if (!good)
        {
          debug_msg ("Any demarshaling failed");
          return -1;
        }

      StringMsg *ev_out = 0;
      good = out_any >>= ev_out;

      if (!good)
        {
          debug_msg ("Any extraction failed");
          return -1;
        }

      const char *result_str = ev_out->str_msg ();

      if (result_str == 0 || ACE_OS::strcmp (result_str, test_str) != 0)
        {
          debug_msg ("Extracted member string null or incorrect");
          return -1;
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("caught exception:");
      return 1;
    }

  return 0;
}

