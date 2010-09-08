// $Id$

#include "AnyC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - test started.\n"));

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      // Create and register factories.

      OBV_AnyTest::VA_init *va_factory = 0;
      ACE_NEW_RETURN (va_factory,
                      OBV_AnyTest::VA_init,
                      1); // supplied by mapping

      orb->register_value_factory (va_factory->tao_repository_id (),
                                   va_factory);
      va_factory->_remove_ref (); // release ownership


      OBV_AnyTest::VB_init *vb_factory = 0;
      ACE_NEW_RETURN (vb_factory,
                      OBV_AnyTest::VB_init,
                      1); // supplied by mapping

      orb->register_value_factory (vb_factory->tao_repository_id (),
                                   vb_factory);
      vb_factory->_remove_ref (); // release ownership

      // Do local test

      OBV_AnyTest::VA_var va1, va2;
      ACE_NEW_RETURN (va1.inout (), OBV_OBV_AnyTest::VA, 1);
      ACE_NEW_RETURN (va2.inout (), OBV_OBV_AnyTest::VA, 1);

      const CORBA::ULong magic = 3145;

      va1->id (magic);
      va2->id (magic);

      CORBA::Any a1, a2;

      // Test both copying and non-copying version of operator<<=
      a1 <<= va1.in ();

      OBV_AnyTest::VA *pva = va2._retn();
      a2 <<= &pva;

      OBV_AnyTest::VA* dst = 0;

      if (!(a1 >>= dst) || dst->id () != magic)
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "(%P|%t) client - test failed.\n"),
                            1);
        }

      if (!(a2 >>= dst) || dst->id () != magic)
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "(%P|%t) client - test failed.\n"),
                            1);
        }


      // It should be possible to extract to a base type
      OBV_AnyTest::VB_var vb1;
      ACE_NEW_RETURN (vb1.inout (), OBV_OBV_AnyTest::VB, 1);
      vb1->id (magic);

      a1 <<= vb1.in ();
      CORBA::ValueBase_var target;
      if (!(a1 >>= CORBA::Any::to_value(target.out())))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "(%P|%t) client - base extraction test failed.\n"),
                            1);
        }
      dst = OBV_AnyTest::VA::_downcast(target.in());
      if (dst == 0 || dst->id() != magic)
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "(%P|%t) client - base extraction test failed.\n"),
                            1);
        }


      // Now do remote test

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      OBV_AnyTest::Test_var test =
        OBV_AnyTest::Test::_narrow(tmp.in ());

      if (CORBA::is_nil (test.in ()))
      {
        ACE_ERROR_RETURN ((LM_DEBUG,
                         "Nil OBV_AnyTest::Test reference <%s>\n",
                         ior),
                        1);
      }


      // STEP 1.
      CORBA::Any_var result = test->get_something (
          0);

      if (!(result.inout () >>= dst) || dst->id () != magic)
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "(%P|%t) client - test 1 failed.\n"),
                            1);
        }

      // STEP 2.
      OBV_AnyTest::VB* dst_vb = 0;
      result = test->get_something (
          1);

      if (!(result.inout () >>= dst_vb) || dst_vb->id () != magic)
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "(%P|%t) client - test 2 failed.\n"),
                            1);
        }

      // STEP 3. A sanity check demonstrating base-type pointer to
      // derived type allowed.
      OBV_AnyTest::VA_var dst_va = test->get_vb();
      if (dst_va->id () != magic)
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "(%P|%t) client - test 3 failed.\n"),
                            1);
        }

#if !defined (TAO_HAS_OPTIMIZED_VALUETYPE_MARSHALING)
      // @@ There's still a problem here with the optimized valuetype
      // marshaling and passing values through anys. The problem is
      // that while the Any in fact contains all of the required type
      // information, there is no way to share that with the
      // ValueBase::_tao_unmarshal_pre() which needs the type info in
      // order to select the appropriate value factory.

      // STEP 4. get a VB, but extract to a VA*.
      result = test->get_something (
          1);

      if (!(result.inout () >>= CORBA::Any::to_value(target.out())))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "(%P|%t) client - test 4 extraction failed.\n"),
                            1);
        }
      dst_va = OBV_AnyTest::VA::_downcast(target._retn());
      if (dst_va == 0 || dst_va->id() != magic)
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "(%P|%t) client -  test 4 failed.\n"),
                            1);
        }
#endif /* TAO_HAS_OPTIMIZED_VALUETYPE_MARSHALING */

      test->shutdown ();

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - test finished.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in client:");
      return 1;
    }

  return 0;
}
