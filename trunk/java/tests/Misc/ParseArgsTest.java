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
package tests.Misc;

import ACE.OS.*;
import ACE.Misc.*;

public class ParseArgsTest
{
  public static void main (String args[])
  {
    String s;
    GetOpt opt = new GetOpt (args, "p:rs:");
    for (int c; (c = opt.next ()) != -1; )
      {
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
