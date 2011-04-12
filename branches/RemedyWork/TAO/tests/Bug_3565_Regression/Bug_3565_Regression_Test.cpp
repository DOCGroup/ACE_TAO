// $Id$

#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/ACE.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  int extra_argc = 4;
  int second_extra_argc = 4;
  int largc = argc;
  int second_largc = argc;

  try
    {
      ACE_TCHAR **extra = 0;
      ACE_TCHAR **second_extra = 0;
      ACE_NEW_RETURN (extra, ACE_TCHAR *[extra_argc], -1);
      ACE_NEW_RETURN (second_extra, ACE_TCHAR *[second_extra_argc], -1);
      extra[0] = ACE::strnew (ACE_TEXT ("-ORBGestalt"));
      extra[1] = ACE::strnew (ACE_TEXT ("Local"));
      extra[2] = ACE::strnew (ACE_TEXT ("-ORBSvcConfDirective"));
      extra[3] = ACE::strnew (ACE_TEXT ("static Client_Strategy_Factory '-ORBReplyDispatcherTableSize 333'"));

      second_extra[0] = ACE::strnew (ACE_TEXT ("-ORBGestalt"));
      second_extra[1] = ACE::strnew (ACE_TEXT ("Local"));
      second_extra[2] = ACE::strnew (ACE_TEXT ("-ORBSvcConfDirective"));
      second_extra[3] = ACE::strnew (ACE_TEXT ("static Client_Strategy_Factory '-ORBReplyDispatcherTableSize 666'"));

      //second_extra[2] = ACE::strnew (ACE_TEXT ("-ORBSvcConfDirective \"static Client_Strategy_Factory '-ORBReplyDispatcherTableSize 666'\""));

      ACE_TCHAR **largv = new ACE_TCHAR *[largc+extra_argc];
      for (int i = 0; i < largc; i++)
        largv[i] = argv[i];

      ACE_TCHAR **second_largv = new ACE_TCHAR *[second_largc+second_extra_argc];
      for (int i = 0; i < second_largc; i++)
        second_largv[i] = argv[i];

      for (int i = 0; i < extra_argc; i++)
        {
          largv[argc+i] = extra[i];
        }
      for (int i = 0; i < second_extra_argc; i++)
        {
          second_largv[argc+i] = second_extra[i];
        }

      largc += extra_argc;
      second_largc += second_extra_argc;

      CORBA::ORB_var orb = CORBA::ORB_init (largc, largv, "FirstORB");

      if (orb->orb_core()->client_factory()->reply_dispatcher_table_size () != 333)
        {
          ++result;
        }

      orb->destroy ();
      orb = CORBA::ORB::_nil ();

      CORBA::ORB_var second_orb = CORBA::ORB_init (second_largc, second_largv, "SecondORB");

      if (second_orb->orb_core()->client_factory()->reply_dispatcher_table_size () != 666)
        {
          ++result;
        }

      second_orb->destroy ();
      second_orb = CORBA::ORB::_nil ();

      for (int i = 0; i < extra_argc; i++)
        ACE::strdelete (extra[i]);
      for (int i2 = 0; i2 < second_extra_argc; i2++)
        ACE::strdelete (second_extra[i2]);
      delete [] extra;
      delete [] largv;
      delete [] second_extra;
      delete [] second_largv;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }

  if (result == 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Test passed!")));
  else
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Test failed. Result: %d\n"),
                result));
  return result;
}
