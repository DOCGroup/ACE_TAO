/**
<p>
<ul>
<li> <b>Java Class</b> Statistics
<li> <b>Source File</b> Statistics.java
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
final public class Statistics {
  public double utilization;
  public double overhead;
  public int arrival_time;
  public int deadline_time;
  public int completion_time;
  public int computation_time;
  public Statistics() {
  }
  public Statistics(
    double utilization,
    double overhead,
    int arrival_time,
    int deadline_time,
    int completion_time,
    int computation_time
  ) {
    this.utilization = utilization;
    this.overhead = overhead;
    this.arrival_time = arrival_time;
    this.deadline_time = deadline_time;
    this.completion_time = completion_time;
    this.computation_time = computation_time;
  }
  public java.lang.String toString() {
    org.omg.CORBA.Any any = org.omg.CORBA.ORB.init().create_any();
    StatisticsHelper.insert(any, this);
    return any.toString();
  }
}
