/*************************************************
 *
 * = PACKAGE
 *    JACE.ServiceConfigurator
 *
 * = FILENAME
 *    ClassReader.java
 *
 *@author Everett Anderson
 *
 *@see JACE.ServiceConfigurator
 *@see JACE.ServiceConfigurator.ServiceLoader
 *
 *************************************************/
package JACE.ServiceConfigurator;

import JACE.OS.*;
import java.io.*;
import java.util.*;

/**
 * <hr>
 * <p><b>TITLE</b><br>
 *    Given a classfile name, this determines the qualified class name
 *    of what's inside (pacakage and class name together).  It is necessary
 *    when users want to dynamically load classes, but don't provide the
 *    full name in svc.conf.
 */

public class ClassReader 
{
  // Types found in classfiles
  public static final byte CONSTANT_Class = 7;
  public static final byte CONSTANT_Fieldref = 9;
  public static final byte CONSTANT_Methodref = 10;
  public static final byte CONSTANT_InterfaceMethodref = 11;
  public static final byte CONSTANT_String = 8;
  public static final byte CONSTANT_Integer = 3;
  public static final byte CONSTANT_Float = 4;
  public static final byte CONSTANT_Long = 5;
  public static final byte CONSTANT_Double = 6;
  public static final byte CONSTANT_NameAndType = 12;
  public static final byte CONSTANT_Utf8 = 1;

  /** Returns a string that is the qualified class name of the
   *  given file.  It returns null if there are any problems.
   *
   *@param   fileName   File to examine
   *
   *@return  String that's the qualified class name of the
   *         given file (null on error)
   */
  public static String getClassName(String fileName)
  {
    try { 

      // Open the .class file
      FileInputStream fis = new FileInputStream(fileName); 

      DataInputStream dis = new DataInputStream(fis);

      // Skip magic number and version numbers
      dis.skipBytes(8);

      // Find out how many entries are in the constant pool table
      int count = dis.readUnsignedShort();

      // Hash table to hold the text entries (possibilities for
      // the class name)
      Hashtable ht = new Hashtable();

      // Vector that holds the index of the class name for each
      // class record (the vector is indexed by placement in the
      // table)
      Vector vt = new Vector(count);
      Integer noValue = new Integer(-1);
      int type;

      // Scan through all the entries
      for (int i = 1; i < count; i++) {

	// Read what type of entry this is
	type = dis.readUnsignedByte();

	switch (type) {

	  case CONSTANT_Class:
	    // Save the index of the class name
	    vt.addElement(new Integer(dis.readUnsignedShort()));
	    break;
	  case CONSTANT_Fieldref:
	  case CONSTANT_Methodref:
	  case CONSTANT_InterfaceMethodref:
 	  case CONSTANT_Integer:
	  case CONSTANT_Float:
  	  case CONSTANT_NameAndType:
	    // Skip the data
	    vt.addElement(noValue);
	    dis.skipBytes(4);
	    break;
	  case CONSTANT_String:
	    // Skip the data
	    vt.addElement(noValue);
	    dis.skipBytes(2);
	    break;
	  case CONSTANT_Long:
	  case CONSTANT_Double:
	    // Skip the data
	    vt.addElement(noValue);
	    dis.skipBytes(8);
	    // These take up two spots in the table
	    i++;
	    break;
	  case CONSTANT_Utf8:
	    vt.addElement(noValue);
	    // Save the text in the hash table
	    ht.put(new Integer(i), new String(dis.readUTF()));
	    break;
 	  default:
	    ACE.DEBUG("Unknown type: " + type);
	    break;

	}

      }

      // Skip the access flags
      dis.skipBytes(2);

      // Get index in table of this class
      int classIdx = dis.readUnsignedShort();

      // Get index in the table of the name of this class
      Integer idx = (Integer)vt.elementAt(classIdx - 1);

      // Put the result in period separated notation 
      String result = new String(((String)ht.get(idx)).replace('/', '.'));

      return result;

    } catch (IOException e) {
      ACE.ERROR("" + e);
      return null;
    }
  }
}
