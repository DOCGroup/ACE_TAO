//
// $Id$
//

import java.io.*;

public class java_client {

  public static void main(String[] args) {
    // Initialize the ORB.
    org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);

    // Read the AccountManager's IOR from a file
    Test test = null;
    try {
      LineNumberReader input =
	new LineNumberReader(new FileReader("test.ior"));
      org.omg.CORBA.Object object = orb.string_to_object(input.readLine());
      test = TestHelper.narrow(object);
    }
    catch(java.io.IOException e) {
      System.out.println("Exception: " + e);
      System.exit(1);
    }

    // use args[0] as the number of iterations.
    Long value = new Long (args.length > 0 ? args[0] : "10000");

    long iterations = value.longValue ();

    // Request the account manager to open a named account.
    for (int i = 0; i != iterations; ++i) {
       test.test_method (0);
    }
    test.shutdown ();
  }

}
