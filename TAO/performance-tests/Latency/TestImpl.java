//
// $Id$
//

public class TestImpl extends _TestImplBase {
  public TestImpl (org.omg.CORBA.ORB orb) {
    orb_ = orb;
  }
  public long test_method(long stamp) {
    return stamp;
  }
  public void shutdown () {
      // orb_.shutdown ();
  }

  private org.omg.CORBA.ORB orb_;
}
