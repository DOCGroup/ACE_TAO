/*************************************************
 *
 * = PACKAGE
 *    JACE.OS
 *
 * = FILENAME
 *    OS.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.OS;

import java.util.StringTokenizer;

/**
 * <hr>
 * <h2>SYNOPSIS</h2>
 *<blockquote>
 *     Methods to extend the capabilities of the Java runtime system.
 *</blockquote>
 *
 * <h2>DESCRIPTION</h2>
 *<blockquote>
 * This non-instantiable class contains little <q>utility functions</q>
 * that should have been in Java to begin with :-)
 *</blockquote>
 */
public class OS
{
  /**
   * Create an array of Strings from a single String using <delim> as
   * the delimiter.
   *@param args the String to break up to make an array of Strings
   *@param delim the delimeter to use to break the String up
   *@return an array containing the original String broken up
   */
  public static String [] createStringArray (String args, String delim)
  {
    // First determine the number of arguments
    int count = 0;
    StringTokenizer tokens = new StringTokenizer (args, delim);
    while (tokens.hasMoreTokens ())
      {
	tokens.nextToken ();
	count++;
      }
    if (count == 0)
      return null;
    
    // Create argument array
    String [] argArray = new String [count];
    int index = 0;
    tokens = new StringTokenizer (args, " ");
    while (tokens.hasMoreTokens ())
      {
	argArray [index] = tokens.nextToken ();
	index++;
      }

    // Assert index == count
    if (index != count)
      return null;
    else
      return argArray;    
  }

  // Default private constructor to avoid instantiation
  private OS ()
  {
  }
}
