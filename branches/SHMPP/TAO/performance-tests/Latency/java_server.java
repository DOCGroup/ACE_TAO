//
// $Id$

import java.io.*;

public class java_server {

  public static void main(String[] args) {
    // Initialize the ORB.
    org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args,null);

    Test test = new TestImpl (orb);

    // Export the newly create object.
    orb.connect(test);

    // Write the IOR to a file
    try {
      FileWriter output = new FileWriter("test.ior");
      output.write(orb.object_to_string(test));
      output.close();
    }
    catch(java.io.IOException e) {
      System.out.println("Exception: " + e);
      System.exit(1);
    }

    System.out.println("The IOR is <"
                       + orb.object_to_string(test)
                       + ">");

    try {
      Thread.currentThread().join();
    }
    catch(InterruptedException e) {
      System.out.println(e);
    }
  }

}
