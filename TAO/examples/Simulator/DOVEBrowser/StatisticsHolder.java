/**
<p>
<ul>
<li> <b>Java Class</b> StatisticsHolder
<li> <b>Source File</b> StatisticsHolder.java
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
final public class StatisticsHolder implements org.omg.CORBA.portable.Streamable {
  public Statistics value;
  public StatisticsHolder() {
  }
  public StatisticsHolder(Statistics value) {
    this.value = value;
  }
  public void _read(org.omg.CORBA.portable.InputStream input) {
    value = StatisticsHelper.read(input);
  }
  public void _write(org.omg.CORBA.portable.OutputStream output) {
    StatisticsHelper.write(output, value);
  }
  public org.omg.CORBA.TypeCode _type() {
    return StatisticsHelper.type();
  }
}
