// ============================================================================
//
// = PACKAGE
//    tests.Misc
// 
// = FILENAME
//    ParseArgsTest.java
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================
package JACE.tests.Misc;

import JACE.OS.*;
import JACE.Misc.*;

public class ParseArgsTest
{
  public static void main (String args[])
  {
    ACE.enableDebugging ();

    String s;
    GetOpt opt = new GetOpt (args, "p:rs:", true);
    for (int c; (c = opt.next ()) != -1; )
      {
	System.out.println ("c is " + (char)c);
	switch (c)
	  {
	  case 'p':
	    s = opt.optarg ();
	    ACE.DEBUG ("Option <p> selected with argument " + s);
	    break;
	  case 'r':
	    ACE.DEBUG ("Option <r> selected");
	    break;
	  case 's':
	    s = opt.optarg ();
	    ACE.DEBUG ("Option <s> selected with argument " + s);
	    break;
	  default:
	    ACE.DEBUG ("Usage: java tests.Misc.ParseArgsTest [-p arg1] [-r] [-s arg2]");
	    break;
	  }
      }
  }
}
