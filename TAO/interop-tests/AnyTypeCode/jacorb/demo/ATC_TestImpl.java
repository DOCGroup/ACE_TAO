package demo;

import Demo.*;
import org.omg.CORBA.ORB;
import org.omg.CORBA.Any;
import org.omg.CORBA.TCKind;

import java.util.Arrays;

public class ATC_TestImpl extends Demo.ATC_TestPOA
{
  org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init();

  public ATC_TestImpl(){}

  public void shutdown ()
  {
    orb.shutdown (false);
  }

  public java.lang.String do_struct (Any a)
  {
    String result = "<empty>";
    try
      {
        int kind =  a.type().kind().value();

        switch( kind )
          {
          case TCKind._tk_struct:
            if (Demo.NestedStructHelper.type().equivalent(a.type()))
              {
                Demo.NestedStruct bar = Demo.NestedStructHelper.extract(a);
                return dumpNestedStruct(bar);
              }
            break;
          case TCKind._tk_any:
            System.out.println("Any.");
            Any inner_any = a.extract_any();
            result = do_struct(inner_any);
            break;
          default:
            System.out.println("Unknown, kind " + a.type().kind().value());
          }
      }
    catch ( Exception e )
      {
        e.printStackTrace();
      }
    System.out.println(result);
    return result;
  }

  public java.lang.String do_union (Any a)
  {
    String result = "<empty>";
    try
      {
        int kind =  a.type().kind().value();

        switch( kind )
          {
          case TCKind._tk_union:
            if (Demo.NestedUnionHelper.type().equivalent(a.type()))
              {
                Demo.NestedUnion quux = Demo.NestedUnionHelper.extract(a);
                return dumpNestedUnion(quux);
              }
            break;
          case TCKind._tk_any:
            System.out.println("Any.");
            Any inner_any = a.extract_any();
            result = do_union(inner_any);
            break;
          default:
            System.out.println("Unknown, kind " + a.type().kind().value());
          }
      }
    catch ( Exception e )
      {
        e.printStackTrace();
      }
    System.out.println(result);
    return result;
  }

  public java.lang.String do_value (Any a)
  {
    String result = "<empty>";
    try
      {
        int kind =  a.type().kind().value();

        switch( kind )
          {
          case TCKind._tk_value:
            if (Demo.NestedValueHelper.type().equivalent(a.type()))
              {
                Demo.NestedValue bar = Demo.NestedValueHelper.extract(a);
                return dumpNestedValue(bar);
              }
            break;
          case TCKind._tk_any:
            System.out.println ("Any.");
            Any inner_any = a.extract_any();
            result = do_value (inner_any);
            break;
          default:
            System.out.println("Unknown, kind " + a.type().kind().value());
          }
      }
    catch ( Exception e )
      {
        e.printStackTrace();
      }
    System.out.println(result);
    return result;
  }

  private String dumpNestedStruct(Demo.NestedStruct bar)
  {
    return bar.getClass().getName() + "[" +
      "foo1=" + dumpBaseStruct(bar.foo1) +
      ",foo2=" + dumpBaseStruct(bar.foo2) +
      ",foo3=" + dumpBaseStruct(bar.foo3) +
      ",barz=" + dumpNestedSeq(bar.bars);
  }

  private String dumpNestedSeq(Demo.NestedStruct[] bars)
  {
    StringBuilder sb = new StringBuilder("[");
    for (Demo.NestedStruct bar : bars) {
      sb.append(dumpNestedStruct(bar)).append(",");
    }
    sb.append("]");
    return sb.toString();
  }

  private String dumpBaseStruct(Demo.BaseStruct foo)
  {
    return foo.getClass().getName() + "[str=" + foo.str + "]";
  }

  private String dumpNestedUnion(Demo.NestedUnion quux)
  {
    StringBuilder sb = new StringBuilder(quux.getClass().getName())
      .append("[discriminator=")
      .append(quux.discriminator());
    switch (quux.discriminator()) {
    case 1:
      sb.append(",bar1=")
        .append(dumpBaseUnion(quux.bar1()))
        .append("]");
      break;
    case 2:
      sb.append(",bar2=")
        .append(dumpBaseUnion(quux.bar2()))
        .append("]");
      break;
    default:
      sb.append(",defaultBaz=")
        .append(dumpBaseUnion(quux.defaultBar()))
        .append("]");
      break;
    }
    return sb.append("]").toString();
  }

  private String dumpBaseUnion (Demo.BaseUnion bar)
  {
    StringBuilder sb = new StringBuilder(bar.getClass().getName())
      .append("[discriminator=")
      .append(bar.discriminator());
    switch (bar.discriminator())
      {
      case 1:
        sb.append(",str=")
          .append(bar.str())
          .append("]");
        break;
      case 2:
        sb.append(",l=")
          .append(bar.l())
          .append("]");
        break;
      default:
        sb.append(",s=")
          .append(bar.s())
          .append("]");
        break;
      }
    return sb.append("]").toString();
  }

  private String dumpNestedValue(Demo.NestedValue bar)
  {
    StringBuilder sb = new StringBuilder(bar.getClass().getName())
      .append("[foo1=")
      .append(dumpBaseValue(bar.foo1))
      .append(",foo2=")
      .append(dumpBaseValue(bar.foo2))
      .append("]");
    return sb.toString();
  }

  private Object dumpBaseValue(Demo.BaseValue foo)
  {
    StringBuilder sb = new StringBuilder(foo.getClass().getName())
      .append("[str=")
      .append(foo.str)
      .append("]");
    return sb.toString();
  }

}
