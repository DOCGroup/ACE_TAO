// $Id$

#include "tao/ORB.h"
#include "tao/ORB_Core.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  int extra_argc = 2;
  int second_extra_argc = 2;
  int largc = argc;
  int second_largc = argc;

  try
    {
      ACE_TCHAR **extra = 0;
      ACE_NEW_RETURN (extra, ACE_TCHAR *[extra_argc], -1);
      extra[0] = ACE::strnew (ACE_TEXT ("-ORBSvcConf"));
      extra[1] = ACE::strnew (ACE_TEXT ("TAO Conf File.conf"));

      ACE_TCHAR **largv = new ACE_TCHAR *[largc+extra_argc];
      for (int i = 0; i < largc; i++)
        largv[i] = argv[i];


      for (int i = 0; i < extra_argc; i++)
        largv[argc+i] = extra[i];

      largc += extra_argc;

      CORBA::ORB_var orb = CORBA::ORB_init (largc, largv, "FirstORB");
       if(CORBA::is_nil(orb.in ()))
       {
          ACE_ERROR((LM_ERROR,ACE_TEXT ("Test failed. CORBA::ORB_init returned NIL ORB\n"),1));

          for (int i = 0; i < extra_argc; i++)
            ACE::strdelete (extra[i]);

          delete [] extra;
          delete [] largv;

          return -1;
     }

      orb->destroy ();
      orb = CORBA::ORB::_nil ();

      for (int i = 0; i < extra_argc; i++)
        ACE::strdelete (extra[i]);
      delete [] extra;
      delete [] largv;


      ACE_TCHAR **second_extra = 0;
      ACE_NEW_RETURN (second_extra, ACE_TCHAR *[second_extra_argc], -1);
      second_extra[0] = ACE::strnew (ACE_TEXT ("-ORBSvcConf"));
      second_extra[1] = ACE::strnew (ACE_TEXT ("TAO_Conf_File.conf"));
      ACE_TCHAR **second_largv = new ACE_TCHAR *[second_largc+second_extra_argc];
      for (int i = 0; i < second_largc; i++)
        second_largv[i] = argv[i];
      for (int i = 0; i < second_extra_argc; i++)
        second_largv[second_largc + i] = second_extra[i];

      second_largc += second_extra_argc;

      CORBA::ORB_var second_orb = CORBA::ORB_init (second_largc, second_largv, "SecondORB");
       if(CORBA::is_nil(second_orb.in ()))
       {
          ACE_ERROR((LM_ERROR,ACE_TEXT ("Test failed. CORBA::ORB_init returned NIL ORB\n"),1));

          for (int i = 0; i < extra_argc; i++)
            ACE::strdelete (second_extra[i]);

          delete [] second_extra;
          delete [] second_largv;

          return -1;
     }

      second_orb->destroy ();
      second_orb = CORBA::ORB::_nil ();

      for (int i2 = 0; i2 < second_extra_argc; i2++)
        ACE::strdelete (second_extra[i2]);

      delete [] second_extra;
      delete [] second_largv;
    }
  catch (const CORBA::Exception& ex)
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
