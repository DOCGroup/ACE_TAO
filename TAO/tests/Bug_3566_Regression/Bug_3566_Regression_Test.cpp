#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/ACE.h"
#include <array>
#include <vector>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  constexpr size_t extra_argc = 4;
  constexpr size_t second_extra_argc = 3;

  try
    {
      std::array<ACE_TCHAR *, extra_argc> extra {};
      std::array<ACE_TCHAR *, second_extra_argc> second_extra {};
      extra[0] = ACE::strnew (ACE_TEXT ("-ORBGestalt"));
      extra[1] = ACE::strnew (ACE_TEXT ("Local"));
      extra[2] = ACE::strnew (ACE_TEXT ("-ORBSvcConfDirective"));
      extra[3] = ACE::strnew (ACE_TEXT ("static Client_Strategy_Factory '-ORBReplyDispatcherTableSize 333'"));

      second_extra[0] = ACE::strnew (ACE_TEXT ("-ORBGestalt"));
      second_extra[1] = ACE::strnew (ACE_TEXT ("Local"));
      second_extra[2] = ACE::strnew (ACE_TEXT ("-ORBSvcConfDirective \"static Client_Strategy_Factory '-ORBReplyDispatcherTableSize 666'\""));

      std::vector<ACE_TCHAR *> largv (argc + extra_argc);
      for (int i = 0; i < argc; ++i)
        largv[i] = argv[i];

      std::vector<ACE_TCHAR *> second_largv (argc + second_extra_argc);
      for (int i = 0; i < argc; ++i)
        second_largv[i] = argv[i];

      for (size_t i = 0; i < extra_argc; ++i)
        {
          largv[argc+i] = extra[i];
        }
      for (size_t i = 0; i < second_extra_argc; ++i)
        {
          second_largv[argc+i] = second_extra[i];
        }

      int largc = argc + extra_argc;
      int second_largc = argc + second_extra_argc;

      CORBA::ORB_var orb = CORBA::ORB_init (largc, largv.data (), "FirstORB");

      if (orb->orb_core ()->client_factory ()->reply_dispatcher_table_size () != 333)
        {
          ++result;
        }

      orb->destroy ();
      orb = CORBA::ORB::_nil ();

      CORBA::ORB_var second_orb = CORBA::ORB_init (second_largc, second_largv.data (), "SecondORB");

      if (second_orb->orb_core ()->client_factory ()->reply_dispatcher_table_size () != 666)
        {
          ++result;
        }

      second_orb->destroy ();
      second_orb = CORBA::ORB::_nil ();

      for (ACE_TCHAR *arg : extra)
        ACE::strdelete (arg);
      for (ACE_TCHAR *arg : second_extra)
        ACE::strdelete (arg);
    }
  catch (CORBA::Exception const &ex)
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
