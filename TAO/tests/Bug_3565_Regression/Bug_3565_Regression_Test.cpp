#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/ACE.h"
#include <array>
#include <string>
#include <vector>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result {};
  constexpr size_t extra_argc = 4;

  try
    {
      std::array<std::basic_string<ACE_TCHAR>, extra_argc> extra {
        ACE_TEXT ("-ORBGestalt"),
        ACE_TEXT ("Local"),
        ACE_TEXT ("-ORBSvcConfDirective"),
        ACE_TEXT ("static Client_Strategy_Factory '-ORBReplyDispatcherTableSize 333'")
      };
      std::array<std::basic_string<ACE_TCHAR>, extra_argc> second_extra {
        ACE_TEXT ("-ORBGestalt"),
        ACE_TEXT ("Local"),
        ACE_TEXT ("-ORBSvcConfDirective"),
        ACE_TEXT ("static Client_Strategy_Factory '-ORBReplyDispatcherTableSize 666'")
      };

      std::vector<ACE_TCHAR *> largv (argc + extra_argc);
      for (int index {}; index < argc; ++index)
        largv[index] = argv[index];

      std::vector<ACE_TCHAR *> second_largv (argc + extra_argc);
      for (int index {}; index < argc; ++index)
        second_largv[index] = argv[index];

      for (size_t index {}; index < extra_argc; ++index)
        {
          largv[argc + index] = extra[index].data ();
        }
      for (size_t index {}; index < extra_argc; ++index)
        {
          second_largv[argc + index] = second_extra[index].data ();
        }

      int largc = argc + extra_argc;
      int second_largc = argc + extra_argc;

        {
          CORBA::ORB_var const orb = CORBA::ORB_init (largc, largv.data (), "FirstORB");

          if (orb->orb_core ()->client_factory ()->reply_dispatcher_table_size () != 333)
            ++result;

          orb->destroy ();
        }

      {
        CORBA::ORB_var const second_orb = CORBA::ORB_init (second_largc, second_largv.data (), "SecondORB");

        if (second_orb->orb_core ()->client_factory ()->reply_dispatcher_table_size () != 666)
          ++result;

        second_orb->destroy ();
      }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      ++result;
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
