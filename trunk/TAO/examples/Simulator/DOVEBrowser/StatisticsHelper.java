/**
<p>
<ul>
<li> <b>Java Class</b> StatisticsHelper
<li> <b>Source File</b> StatisticsHelper.java
<li> <b>IDL Source File</b> NewNavWeap.idl
<li> <b>IDL Absolute Name</b> ::Statistics
<li> <b>Repository Identifier</b> IDL:Statistics:1.0
</ul>
<b>IDL definition:</b>
<pre>
    struct Statistics {
      double utilization;
      double overhead;
      unsigned long arrival_time;
      unsigned long deadline_time;
      unsigned long completion_time;
      unsigned long computation_time;
    };
</pre>
</p>
*/
abstract public class StatisticsHelper {
  private static org.omg.CORBA.ORB _orb() {
    return org.omg.CORBA.ORB.init();
  }
  public static Statistics read(org.omg.CORBA.portable.InputStream _input) {
    Statistics result = new Statistics();
    result.utilization = _input.read_double();
    result.overhead = _input.read_double();
    result.arrival_time = _input.read_ulong();
    result.deadline_time = _input.read_ulong();
    result.completion_time = _input.read_ulong();
    result.computation_time = _input.read_ulong();
    return result;
  }
  public static void write(org.omg.CORBA.portable.OutputStream _output, Statistics value) {
    _output.write_double(value.utilization);
    _output.write_double(value.overhead);
    _output.write_ulong(value.arrival_time);
    _output.write_ulong(value.deadline_time);
    _output.write_ulong(value.completion_time);
    _output.write_ulong(value.computation_time);
  }
  public static void insert(org.omg.CORBA.Any any, Statistics value) {
    org.omg.CORBA.portable.OutputStream output = any.create_output_stream();
    write(output, value);
    any.read_value(output.create_input_stream(), type());
  }
  public static Statistics extract(org.omg.CORBA.Any any) {
    if(!any.type().equal(type())) {
      throw new org.omg.CORBA.BAD_TYPECODE();
    }
    return read(any.create_input_stream());
  }
  private static org.omg.CORBA.TypeCode _type;
  public static org.omg.CORBA.TypeCode type() {
    if(_type == null) {
      org.omg.CORBA.StructMember[] members = new org.omg.CORBA.StructMember[6];
      members[0] = new org.omg.CORBA.StructMember("utilization", _orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_double), null);
      members[1] = new org.omg.CORBA.StructMember("overhead", _orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_double), null);
      members[2] = new org.omg.CORBA.StructMember("arrival_time", _orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_ulong), null);
      members[3] = new org.omg.CORBA.StructMember("deadline_time", _orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_ulong), null);
      members[4] = new org.omg.CORBA.StructMember("completion_time", _orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_ulong), null);
      members[5] = new org.omg.CORBA.StructMember("computation_time", _orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_ulong), null);
      _type = _orb().create_struct_tc(id(), "Statistics", members);
    }
    return _type;
  }
  public static java.lang.String id() {
    return "IDL:Statistics:1.0";
  }
}
