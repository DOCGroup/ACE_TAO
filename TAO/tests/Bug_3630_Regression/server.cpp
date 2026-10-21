#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include <array>
#include <vector>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result {};
  constexpr size_t extra_argc = 2;

  try
    {
      std::array<ACE_TCHAR *, extra_argc> extra {};
      extra[0] = ACE::strnew (ACE_TEXT ("-ORBSvcConf"));
      extra[1] = ACE::strnew (ACE_TEXT ("TAO Conf File.conf"));

      std::vector<ACE_TCHAR *> largv (argc + extra_argc);
      for (int i = 0; i < argc; ++i)
        largv[i] = argv[i];


      for (size_t i = 0; i < extra_argc; ++i)
        largv[argc + i] = extra[i];

      int largc = argc + extra_argc;

      CORBA::ORB_var orb = CORBA::ORB_init (largc, largv.data (), "FirstORB");
      if (CORBA::is_nil (orb.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Test failed. CORBA::ORB_init returned NIL ORB\n"),
                      1));

          for (ACE_TCHAR *arg : extra)
            ACE::strdelete (arg);

          return -1;
        }

      orb->destroy ();
      orb = CORBA::ORB::_nil ();

      for (ACE_TCHAR *arg : extra)
        ACE::strdelete (arg);


      std::array<ACE_TCHAR *, extra_argc> second_extra {};
      second_extra[0] = ACE::strnew (ACE_TEXT ("-ORBSvcConf"));
      second_extra[1] = ACE::strnew (ACE_TEXT ("TAO_Conf_File.conf"));
      std::vector<ACE_TCHAR *> second_largv (argc + extra_argc);
      for (int i = 0; i < argc; ++i)
        second_largv[i] = argv[i];
      for (size_t i = 0; i < extra_argc; ++i)
        second_largv[argc + i] = second_extra[i];

      int second_largc = argc + extra_argc;

      CORBA::ORB_var second_orb = CORBA::ORB_init (second_largc, second_largv.data (), "SecondORB");
      if (CORBA::is_nil (second_orb.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Test failed. CORBA::ORB_init returned NIL ORB\n"),
                      1));

          for (ACE_TCHAR *arg : second_extra)
            ACE::strdelete (arg);

          return -1;
        }

      second_orb->destroy ();
      second_orb = CORBA::ORB::_nil ();

      for (ACE_TCHAR *arg : second_extra)
        ACE::strdelete (arg);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }

  if (result == 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Test passed!\n")));
  else
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Test failed. Result: %d\n"),
                result));
  return result;
}
