/*************************************************
 *
 * = PACKAGE
 *    ACE.Misc
 *
 * = FILENAME
 *    GetOpt.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package ACE.Misc;

import java.io.*;
import java.util.Hashtable;
import java.util.StringTokenizer;

/**
 * <hr>
 * <h2>SYNOPSIS</h2>
 *<blockquote>
 *     Iterator for parsing command-line arguments.
 *</blockquote>
 *
 * <h2>DESCRIPTION</h2>
 *<blockquote>
 *     This version of `get_opt' appears to the caller like standard
 *     Unix `get_opt' but it behaves differently for the user, since
 *     it allows the user to intersperse the options with the other
 *     arguments.
 * 
 * <p> As `get_opt' works, it permutes the elements of `argv' so that,
 *     when it is done, all the options precede everything else. Thus
 *     all application programs are extended to handle flexible argument
 *     order.
 *</blockquote>
 *
 */
public class GetOpt
{
  /** 
   * Constructor
   *@param args command line arguments
   *@param optstring string containing the legitimate option
   * characters. A colon in optstring means that the previous character
   * is an option that wants an argument which is then taken from the
   * rest of the current args-element. Here is an example of what
   * optstring might look like: "c:dP:p".
   */
  public GetOpt (String[] args, String optstring)
  {
    // Cache the arguments
    this.args_ = args;
    this.hasArg_ = false;

    // Build the arg hashtable
    this.buildArgTable (optstring);
  }
  
  /** 
   * Scan elements specified in optstring for next option flag.
   *@return The character corresponding to the next flag.
   */
  public int next ()
  {
    if (this.args_ == null)
      return -1;

    if (this.index_ < this.args_.length) 
      {
	String arg = this.args_[this.index_++];

	// Make sure flag starts with "-"
	if (!arg.startsWith ("-"))
	  return -1;
	
	// Check if there is more than one character specified as flag
	if (arg.length () > 2)
	  return -1;
	
	// So far so good
	// Check if the flag is in the arg_table and if it is get the
	// associated binding.
	Character c = (Character) this.argTable_.get (new Character (arg.charAt (1)));
	if (c == null)
	  return -1;

	if (c.charValue () == '#')
	  {
	    this.hasArg_ = false;
	    return arg.charAt (1);
	  }
	else if (c.charValue () == ':')
	  {
	    this.hasArg_ = true;
	    return arg.charAt (1);
	  }
	else   // This should not happen
	  return -1; 
      }
    return -1;
  }
  
  /**
   * Get the argument (if any) associated with the flag.
   *@return the argument associated with the flag.
   */
  public String optarg ()
  {
    if (this.hasArg_)
      return this.args_[this.index_++];
    else
      return null;
  }
  
  // Build the argument table
  private void buildArgTable (String s)
  {
    this.argTable_ = new Hashtable ();
    StringTokenizer tokens = new StringTokenizer (s, ":");
    while (tokens.hasMoreTokens ())
      {
	// Get the next token
	String t = tokens.nextToken ();

	// First add all flags except the one with ":" after it
	// Note "#" is an arbitrary character we use to distinguish
	// the two cases
	for (int i = 0; i < t.length () - 1; i++)
	  this.argTable_.put (new Character (t.charAt (i)),
			       new Character ('#'));

	// Now Add the flag just before ":" to the arg_table
	this.argTable_.put (new Character (t.charAt (t.length () - 1)),
			     new Character (':'));
      }
  }
  
  private String [] args_;
  // Copy of the args passed in
  
  private boolean hasArg_;
  // Indicator that the flag has an argument following it

  private int index_;
  // Index into the array of arguments

  private Hashtable argTable_;
  // Table of flags that take arguments after them
}
