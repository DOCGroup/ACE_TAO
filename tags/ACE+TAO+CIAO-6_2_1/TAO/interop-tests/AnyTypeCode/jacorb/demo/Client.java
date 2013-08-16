//$Id$
package demo;

import Demo.*;
import org.omg.CORBA.Any;

import java.io.BufferedReader;
import java.io.FileReader;

public class Client
{
  public static void main (String[] args)
    throws Exception
  {
    org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
    String filename = "test.ior";
    if (args.length == 1)
      {
        filename = args[0];
      }
    BufferedReader br = new BufferedReader (new FileReader (filename));

    Demo.ATC_Test atc_test =
      Demo.ATC_TestHelper.narrow (orb.string_to_object (br.readLine ()));

    // struct tests
    do_struct_tests (atc_test);
    do_union_tests (atc_test);
    do_value_tests (atc_test);

    atc_test.shutdown ();

    orb.shutdown (true);
  }

  private static void do_struct_tests (Demo.ATC_Test atc_test)
  {
    System.out.println ("Calling do_struct() with a NestedStruct");

    Demo.BaseStruct foo1 = new Demo.BaseStruct ();
    foo1.str = "BaseStruct 1";

    Demo.BaseStruct foo2 = new Demo.BaseStruct ();
    foo2.str = "BaseStruct 2";

    Demo.BaseStruct foo3 = new Demo.BaseStruct ();
    foo3.str = "BaseStruct 3";

    Demo.NestedStruct bar = new Demo.NestedStruct ();
    bar.foo1 = foo1;
    bar.foo2 = foo2;
    bar.foo3 = foo3;

    Demo.BaseStruct innerBaseStruct1 = new Demo.BaseStruct ();
    innerBaseStruct1.str = "Inner BaseStruct 1";
    Demo.BaseStruct innerBaseStruct2 = new Demo.BaseStruct ();
    innerBaseStruct2.str = "Inner BaseStruct 2";
    Demo.BaseStruct innerBaseStruct3 = new Demo.BaseStruct ();
    innerBaseStruct3.str = "Inner BaseStruct 3";

    Demo.NestedStruct innerNestedStruct = new Demo.NestedStruct ();
    innerNestedStruct.foo1 = innerBaseStruct1;
    innerNestedStruct.foo2 = innerBaseStruct2;
    innerNestedStruct.foo3 = innerBaseStruct3;
    innerNestedStruct.bars = new Demo.NestedStruct[0];

    bar.bars = new Demo.NestedStruct[] {innerNestedStruct};

    Any any = org.omg.CORBA.ORB.init ().create_any ();
    Demo.NestedStructHelper.insert (any, bar);

    System.out.println (atc_test.do_struct (any));
  }

  private static void do_union_tests (Demo.ATC_Test atc_test)
  {
    Any any = unionWithBar1 ();
    System.out.println ("Calling do_union() with a Quux with bar1");
    System.out.println (atc_test.do_union (any));

    any = unionWithBar2();
    System.out.println ("Calling do_union() with a Quux with bar2");
    System.out.println (atc_test.do_union (any));

    any = unionWithDefaultBar();
    System.out.println ("Calling do_union() with a Quux with defaultBar");
    System.out.println (atc_test.do_union (any));

  }

  private static void do_value_tests (Demo.ATC_Test atc_test)
  {
    Demo.BaseValue bval1 = new BaseValueImpl ();
    bval1.str = "BaseValue 1";

    Demo.BaseValue bval2 = new BaseValueImpl ();
    bval2.str = "BaseValue 2";

    Demo.NestedValue nval = new NestedValueImpl ();
    nval.foo1 = bval1;
    nval.foo2 = bval2;

    Any any = org.omg.CORBA.ORB.init ().create_any ();
    Demo.NestedValueHelper.insert (any, nval);

    System.out.println ("Calling do_value() with a NestedValue");
    System.out.println (atc_test.do_value (any));
  }

  private static Any unionWithBar1 ()
  {
    Any any = org.omg.CORBA.ORB.init ().create_any ();

    Demo.BaseUnion bar1 = new Demo.BaseUnion ();
    bar1.str ("BaseUnion 1");

    Demo.NestedUnion quux = new Demo.NestedUnion ();
    quux.bar1(bar1);

    Demo.NestedUnionHelper.insert (any, quux);
    return any;
  }

  private static Any unionWithBar2 ()
  {
    Any any = org.omg.CORBA.ORB.init ().create_any ();

    Demo.BaseUnion bar2 = new Demo.BaseUnion ();
    bar2.l (9765625);

    Demo.NestedUnion quux = new Demo.NestedUnion ();
    quux.bar2 (bar2);

    Demo.NestedUnionHelper.insert (any, quux);
    return any;
  }

  private static Any unionWithDefaultBar ()
  {
    Any any = org.omg.CORBA.ORB.init ().create_any ();

    Demo.BaseUnion defaultBaseUnion = new Demo.BaseUnion ();
    defaultBaseUnion.s ((short) 1024);

    Demo.NestedUnion quux = new Demo.NestedUnion ();
    quux.defaultBar (defaultBaseUnion);

    Demo.NestedUnionHelper.insert (any, quux);
    return any;
  }
}
