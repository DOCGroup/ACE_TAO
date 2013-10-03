// $Id$
// FUZZ: disable check_for_ace_log_categories

#include "DemoC.h"
#include "BaseValueImpl.h"
#include "NestedValueImpl.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"

void unionWithBar1 (CORBA::Any& any)
{
  Demo::BaseUnion bar1;
  bar1.str (CORBA::string_dup ("Bar 1"));
  Demo::NestedUnion any_union;
  any_union.bar1 (bar1);

  any <<= any_union;
}

void unionWithBar2 (CORBA::Any& any)
{
  Demo::BaseUnion bar2;
  bar2.l (9765625);
  Demo::NestedUnion any_union;
  any_union.bar2 (bar2);

  any <<= any_union;
}

void unionWithDefaultBar (CORBA::Any& any)
{
  Demo::BaseUnion defaultBar;
  defaultBar.s (1024);
  Demo::NestedUnion any_union;
  any_union.defaultBar (defaultBar);

  any <<= any_union;
}

const ACE_TCHAR *ior_str = ACE_TEXT ("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior_str = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-k <ior>")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Demo::ATC_Test_var test;
  CORBA::ORB_var orb;
  try
    {
      orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == -1)
        {
          return -1;
        }
      CORBA::Object_var obj =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior_str));
      if  (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("%P could not get ATC_Test IOR.\n")),
                            -1);
        }

      test = Demo::ATC_Test::_narrow (obj.in ());
      if  (CORBA::is_nil (test.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("%P narrow failed.\n")),
                            -1);
        }
    }
  catch  (CORBA::Exception& ex)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%P initialization caught %s.\n"), ex._name()),
                        -1);
    }

  CORBA::Any any;
  CORBA::String_var str;

  // union tests
  try
    {
      unionWithBar1 (any);
      str = test->do_union (any);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%P Union test 1 returned %s\n"), str.in()));

      unionWithBar2 (any);
      str = test->do_union (any);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%P Union test 2 returned %s\n"), str.in()));

      unionWithDefaultBar (any);
      str = test->do_union (any);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%P Union test 3 returned %s\n"), str.in()));
    }
  catch  (CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%P Union test caught %s.\n"), ex._name()));
    }

  // valuetype tests
  try
    {
      Demo_BaseValueImpl* foo1 = new Demo_BaseValueImpl();
      foo1->str(CORBA::string_dup("BaseValue 1"));

      Demo_BaseValueImpl* foo2 = new Demo_BaseValueImpl();
      foo2->str(CORBA::string_dup("BaseValue 2"));

      Demo_NestedValueImpl* bar = new Demo_NestedValueImpl();
      bar->foo1(foo1);
      bar->foo2(foo2);

      any <<= bar;

      str = test->do_value (any);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%P Valuetype test returned %s\n"),
                  str.in()));
    }
  catch  (CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%P Valuetype test caught %s.\n"), ex._name()));
    }

  // struct tests
  try
    {

      Demo::BaseStruct foo1;
      foo1.str = CORBA::string_dup("BaseStruct 1");

      Demo::BaseStruct foo2;
      foo2.str = CORBA::string_dup("BaseStruct 2");

      Demo::BaseStruct foo3;
      foo3.str = CORBA::string_dup("BaseStruct 3");

      Demo::NestedStruct bar;
      bar.foo1 = foo1;
      bar.foo2 = foo2;
      bar.foo3 = foo3;

      Demo::BaseStruct innerBaseStruct1;
      innerBaseStruct1.str = CORBA::string_dup("Inner BaseStruct 1");

      Demo::BaseStruct innerBaseStruct2;
      innerBaseStruct2.str = CORBA::string_dup("Inner BaseStruct 2");

      Demo::BaseStruct innerBaseStruct3;
      innerBaseStruct3.str = CORBA::string_dup("Inner BaseStruct 3");

      Demo::NestedStruct innerBar;
      innerBar.foo1 = innerBaseStruct1;
      innerBar.foo2 = innerBaseStruct2;
      innerBar.foo3 = innerBaseStruct3;

      Demo::NestedSeq bars;
      bars.length(1);
      bars[0] = innerBar;

      bar.bars = bars;

      any <<= bar;

      str = test->do_struct (any);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%P struct test returned %s\n"),
                  str.in()));
    }
  catch  (CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%P struct test caught %s.\n"), ex._name()));
    }

  test->shutdown ();

  return 0;
}
