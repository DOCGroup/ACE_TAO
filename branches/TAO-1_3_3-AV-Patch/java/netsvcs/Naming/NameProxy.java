/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Naming
 *
 * = FILENAME
 *    NameProxy.java
 *
 * This is a proxy which clients can use to interact with the naming service.  They
 * open a SOCKStream to the service, and can then call simple bind and resolve
 * methods.
 *
 *@see netsvcs.Naming.NameAcceptor
 *@see netsvcs.Naming.NameHandler
 *
 *@author Everett Anderson
 *
 *************************************************/
package netsvcs.Naming;

import java.io.*;
import java.net.*;
import java.util.*;
import JACE.OS.*;
import JACE.SOCK_SAP.*;

public class NameProxy
{
  /**
   * Constructor
   *
   *@param socket    A SOCKStream already connected to the naming service
   */
  public NameProxy(SOCKStream socket)
    {
      this.socket_ = socket;
    }

  /**
   * Attempt to bind the given data pair
   * @param name      Name/key
   * @param value     Value to bind 
   *
   * @return   True iff bind is successful
   */
  public boolean bind(String name, String value) throws IOException
    {
      return this.bind(name, value, null, false);
    }

  /**
   * Attempt to bind the given data triplet
   * @param name      Name/key
   * @param value     Value to bind 
   * @param type      Type to bind (another string)
   *
   * @return   True iff the bind was successful
   */
  public boolean bind(String name, String value, String type) throws IOException
    {
      return this.bind(name, value, type, false);
    }

  /**
   * The most generic of the bind methods.  Allows factoring out of common code.  Not public.
   */
  boolean bind (String name, String value, String type, boolean rebind) throws IOException
    {
      // Create a new NameRequest with the desired info
      NameRequest request = new NameRequest(rebind ? NameRequest.REBIND : NameRequest.BIND,
					    name,
					    value,
					    type,
					    null);

      // Send it to the naming service
      request.streamOutTo(this.socket_);

      // Create a reply
      NameReply reply = new NameReply();

      // Get the status of the bind from the naming service
      reply.streamInFrom(this.socket_);
      
      // Return true on success
      return (reply.type() == NameReply.SUCCESS ? true : false);
    }

  /**
   * Rebind a name and a value
   * @param name     Name/key
   * @param value    Bound value
   *
   * @return     True if the rebind was successful
   */
  public boolean rebind (String name, String value) throws IOException
    {
      return this.bind(name, value, null, true);
    }

  /** 
   * Rebind a name, value, and type
   * @param name     Name/key
   * @param value    Bound value
   * @param type     Bound type
   *
   * @return    True if rebind was successful
   */
  public boolean rebind (String name, String value, String type) throws IOException
    {
      return this.bind(name, value, type, true);
    }
  /**
   * Look up information bound to the given key/name.
   *
   * @param  name     Name/key
   *
   * @return     Vector with three elements:
   *                 0   Name/key
   *                 1   Value
   *                 2   Type
   */
  public Vector resolve (String name) throws IOException
    {
      // Create a new NameRequest with the name & request type
      NameRequest request = new NameRequest(NameRequest.RESOLVE,
					    name,
					    null,
					    null,
					    null);

      // Send it to the naming service
      request.streamOutTo(this.socket_);

      // Get a response (hopefully with the value and type)
      request.streamInFrom(this.socket_);

      // Dump the result into a vector
      Vector result = new Vector();

      result.addElement(request.name());
      result.addElement(request.value());
      result.addElement(request.type());

      // Cut it down to the size we need
      result.trimToSize();

      return result;
    }

  /**
   * Remove the entry in the mapping corresponding to the given name/key.
   *
   * @param      name      Name/key
   *
   * @return     True if the unbind was successful
   */
  public boolean unbind (String name) throws IOException
    {
      NameRequest request = new NameRequest(NameRequest.UNBIND,
					    name,
					    null,
					    null,
					    null);
      // Send the request to the naming service
      request.streamOutTo(this.socket_);

      NameReply reply = new NameReply();

      // Get reply
      reply.streamInFrom(this.socket_);

      return (reply.type() == NameReply.SUCCESS ? true : false);
    }

  /**
   * Return a vector that's a list of names (Strings) that begin with
   * the given pattern
   * @param    pattern      Search pattern
   * @return   Vector       List of names
   */
  public Vector listNames (String pattern) throws IOException
    {
      return this.requestSimpleList(pattern, NameRequest.LIST_NAMES);
    }

  /**
   * Return a vector that's a list of types (Strings) that begin with
   * the given pattern
   * @param    pattern      Search pattern
   * @return   Vector       List of types
   */
  public Vector listTypes (String pattern) throws IOException
    {
      return this.requestSimpleList(pattern, NameRequest.LIST_TYPES);
    }

  /**
   * Return a vector that's a list of values (Strings) that begin with
   * the given pattern
   * @param    pattern      Search pattern
   * @return   Vector       List of values
   */
  public Vector listValues (String pattern) throws IOException
    {
      return this.requestSimpleList(pattern, NameRequest.LIST_VALUES);
    }

  /**
   * Non-public generic list gathering method
   */
  Vector requestSimpleList (String pattern, int type) throws IOException
    {
      // Make request for a list of the given type
      NameRequest request = new NameRequest(type,
					    pattern,
					    null,
					    null,
					    null);
      request.streamOutTo(this.socket_);

      // Allocate and reuse the DIS here rather than each time we call
      // streamInFrom
      DataInputStream dis = new DataInputStream(this.socket_.inputStream());

      request.streamInFrom(dis);
      Vector result = new Vector();

      // Add elements until there's a null message with the MAX_ENUM
      // request type
      while (request.requestType() != NameRequest.MAX_ENUM) {
	if (type == NameRequest.LIST_NAMES)
	  result.addElement(new String(request.name()));
	else
	if (type == NameRequest.LIST_VALUES)
	  result.addElement(new String(request.value()));
	else
	  result.addElement(new String(request.type()));

	request.streamInFrom(dis);
      }

      // Adjust the vector to the minimal size
      result.trimToSize();

      return result;
    }

  /**
   * Get a vector with the entire data set for entries whose name begins with
   * the given pattern.  Each element in the vector is another vector
   * with the following layout:
   *               0     Name/key
   *               1     Value
   *               2     Type
   *
   * @param    pattern     Search pattern
   * @return   Vector of vectors
   */
  public Vector listNameEntries (String pattern) throws IOException
    {
      return this.requestComplexList(pattern, NameRequest.LIST_NAME_ENTRIES);
    }

  /**
   * Get a vector with the entire data set for entries whose value begins with
   * the given pattern.  Each element in the vector is another vector
   * with the following layout:
   *               0     Name/key
   *               1     Value
   *               2     Type
   *
   * @param    pattern     Search pattern
   * @return   Vector of vectors
   */
  public Vector listValueEntries (String pattern) throws IOException
    {
      return this.requestComplexList(pattern, NameRequest.LIST_VALUE_ENTRIES);
    }

  /**
   * Get a vector with the entire data set for entries whose type begins with
   * the given pattern.  Each element in the vector is another vector
   * with the following layout:
   *               0     Name/key
   *               1     Value
   *               2     Type
   *
   * @param    pattern     Search pattern
   * @return   Vector of vectors
   */

  public Vector listTypeEntries (String pattern) throws IOException
    {
      return this.requestComplexList(pattern, NameRequest.LIST_TYPE_ENTRIES);
    }

  /**
   * Non-public generic method for getting a a vector of vectors with the 
   * entire data set for entries fitting the given pattern.
   */
  Vector requestComplexList (String pattern, int type) throws IOException
    {
      // Create request with desired type
      NameRequest request = new NameRequest(type,
					    pattern,
					    null,
					    null,
					    null);
      // Send it to the naming service
      request.streamOutTo(this.socket_);

      // Allocate the DIS here and reuse
      DataInputStream dis = new DataInputStream(this.socket_.inputStream());

      // Get the first response
      request.streamInFrom(dis);
      Vector result = new Vector();

      // Loop while we don't see a null response with the MAX_ENUM request type
      while (request.requestType() != NameRequest.MAX_ENUM) {
	Vector entry = new Vector();

	// Create an element in the main vector
	entry.addElement(request.name());
	entry.addElement(request.value());
	entry.addElement(request.type());
	entry.trimToSize();
	
	// Add it to the result
	result.addElement(entry);

	// Get another NameRequest
	request.streamInFrom(dis);
      }

      result.trimToSize();

      return result;
    }

  // The SOCKStream used to communication with the service
  SOCKStream socket_;
};


  
  
  
      
  
