/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    ClassNameGenerator.java
 * 
 * Given a locator string which may specify a class name, this generates as many possible locations for that
 * class file as possible.  This is needed when trying to infer a class name from a C++ DLL line in the
 * svc.conf file.  Implements the Enumeration interface.  A variety of questionable tactics are
 * used.
 *
 *@author Everett Anderson
 *
 *************************************************/
package JACE.ServiceConfigurator;

import java.io.*;
import java.util.*;
import JACE.OS.*;

public class ClassNameGenerator implements Enumeration
{
  /**
   * Constructor
   * @param locator    Locator string -- approximate class name
   */
  public ClassNameGenerator (String locator)
  {
    this.locator_ = locator;
    this.current_ = 0;
    this.suggestions_ = new Vector();
  
    this.makeSuggestions();
  }

  /** 
   * More unseen possibilities?
   */
  public boolean hasMoreElements() 
  {
    return this.current_ < this.suggestions_.size();
  }

  /**
   * Get the next string
   */
  public Object nextElement()
  {
    return this.suggestions_.elementAt(this.current_++);
  }

  /**
   * Take away things underscores, adjust capitalization.  This
   * makes a name more like a Java class name
   */
  String makeJavaName(String oldName)
  {
    StringTokenizer tok = new StringTokenizer(oldName, "_");
    String result = new String();

    while (tok.hasMoreTokens()) {
      String token = tok.nextToken();
      
      if (token.length() > 0) {

	result += (Character.toUpperCase(token.charAt(0)) + token.substring(1));
      }
    }

    return result;
  }


  /**
   * Generate the suggestions
   */
  void makeSuggestions() 
  {
    String slash = new String(System.getProperty("file.separator"));

    // Discard things like .shobj
    if ((this.locator_.charAt(0) == '.') && (this.locator_.charAt(1) != '.')) {
      int firstslash = this.locator_.indexOf(slash);
      if (firstslash > 0) 
	this.locator_ = this.locator_.substring(firstslash + 1);
    }

    // Find the last directory separator and the colon (inbetween
    // these two is the last directory name, and by our convention
    // this should be the class name)
    int lastSlash = this.locator_.lastIndexOf(slash);
    int colon = this.locator_.lastIndexOf(":");

    ACE.DEBUG("Working with " + this.locator_ + " " + 
		       lastSlash + " " + colon);

    // Best bet first:
    // Locator string is a path, and the last directory name is the class
    // name
    String lastDir = new String(this.locator_.substring(lastSlash + 1,
                                                        colon));

    // Save any other path info for later tries
    String path = null;
    if (lastSlash >= 0) 
      path = this.locator_.substring(0, lastSlash + 1);
  
    int tryNum = 0;

    if (lastDir.length() > 0) {

      // Add this try
      this.suggestions_.addElement(lastDir);

      if (path != null)
	this.suggestions_.addElement(path + lastDir);

      // Second try:
      // Same as the first, but changed to the Java convention for class names:
      // Begins with a capital letter, capital letters for new words rather
      // than underscores.

      String try2 = this.makeJavaName(lastDir);

      this.suggestions_.addElement(try2);
      if (path != null)
	this.suggestions_.addElement(path + try2);
    }

    // Third try:
    // After the colon, the name of the method that created instances of the
    // class in C++ may contain the class name.  Maybe in the format of
    // make_classname, etc.

    String lastWord = new String(this.locator_.substring(colon + 1));

    if (lastWord.length() > 0) {

      // Try the word after the colon, too
      this.suggestions_.addElement(lastWord);
      if (path != null)
	this.suggestions_.addElement(path + lastWord);

      String try2 = this.makeJavaName(lastWord);
      
      this.suggestions_.addElement(try2);
      if (path != null)
	this.suggestions_.addElement(path + try2);

      int make = lastWord.indexOf("make");
      if (make >= 0) {

	make += 4;
	String makeTry = new String(lastWord.substring(make));

	if (makeTry.length() > 0) {

	  this.suggestions_.addElement(makeTry);
	  if (path != null)
	    this.suggestions_.addElement(path + makeTry);

	  String try3 = this.makeJavaName(makeTry);

	  this.suggestions_.addElement(try3);
	  if (path != null)
	    this.suggestions_.addElement(path + try3);
	}
      }
    }

  }

  String locator_;
  Vector suggestions_;

  int current_;
};


