// $Id$

#include "InheritedOpC.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb;
  Outer::Derived_var target;

  try
    {
      orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var tmp = orb->string_to_object (ior);

      target = Outer::Derived::_narrow (tmp.in ());

      if (CORBA::is_nil (target.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Outer::Derived ")
                             ACE_TEXT ("reference <%s>\n"),
                             ior),
                            1);
        }
        
      target->BaseOp ();
      target->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::BAD_OPERATION&)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("client - target operation ")
                  ACE_TEXT ("BaseOp returned BAD_OPERATION\n")));
      target->shutdown ();
      orb->destroy  ();
      return 1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client - Exception caught:");
      target->shutdown ();
      orb->destroy  ();
      return 1;
    }

  return 0;
}
