/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Naming
 *
 * = FILENAME
 *    NameHandler.java
 *
 *************************************************/
package JACE.netsvcs.Naming;

import java.net.SocketException;
import java.io.*;
import java.util.*;
import JACE.OS.*;
import JACE.Connection.*;
import JACE.Reactor.*;
import JACE.SOCK_SAP.*;
import JACE.netsvcs.Handler;

/**
 * Handlers interaction between a client (NameProxy) and the naming
 * service database.  Created by NameAcceptor to handle requests.
 * <P>
 * In general, the user binds a name to a (value, type) pair.  The type is just
 * treated as just another String (in the C++ version the name and value are
 * arrays of 16 bit data types and the type is an array of 8 bit chars).
 * <P>
 * For this to work in the hash table scheme, the type and value are wrapped in
 * a ValueType class defined as a nested top level class within the
 * NameHandler.
 * <P>
 * This is compatible with the C++ ACE remote name service. 
 *
 *@see JACE.netsvcs.Naming.NameAcceptor
 *@see JACE.netsvcs.Naming.NameRequest
 *@see JACE.netsvcs.Naming.NameReply
 *
 *@author Everett Anderson
 */
public class NameHandler extends Handler
{
  /**
   * Constructor
   *
   * @param mapping     Hash table created in NameAcceptor
   */
  public NameHandler (Hashtable mapping)
  {
    this.mapping_ = mapping;
  }
  
  /**
   * Read in the given NameRequest and calls dispatch.
   */
  public void processRequest (Object request)
    throws SocketException, EOFException, IOException
  {
    NameRequest nameRequest = (NameRequest)request;

    nameRequest.streamInFrom (peer ().dataInputStream ());

    this.dispatch (nameRequest);
  }

  /**
   * Create a new instance of NameRequest.
   */
  public Object newRequest ()
  {
    return new NameRequest ();
  }

  /**
   * This is the point at which a request is sent to the various methods
   * that fulfill it.  It switches on the request type -- bind, 
   * rebind, resolve, etc.
   *
   *@param nameRequest     The request to respond to
   */
  void dispatch (NameRequest nameRequest) throws IOException
  {
    NameAcceptor parent = (NameAcceptor)parent ();

    // Call the various other member functions based on the
    // message type of the request -- bind, rebind, etc.
    switch (nameRequest.requestType()) 
      {
      case NameRequest.BIND:
	this.bind(nameRequest, false);
	parent.modifiedMapping (true);
	break;
      case NameRequest.REBIND:
	this.bind(nameRequest, true);
	parent.modifiedMapping (true);
	break;
      case NameRequest.RESOLVE:
	this.resolve(nameRequest);
	break;
      case NameRequest.UNBIND:
	this.unbind(nameRequest);
	parent.modifiedMapping (true);
	break;
      case NameRequest.LIST_NAMES:
	this.listByName(nameRequest.name(), false);
	break;
      case NameRequest.LIST_VALUES:
	this.listByValue(nameRequest.name(), false);
	break;
      case NameRequest.LIST_TYPES:
	this.listByType(nameRequest.name(), false);
	break;
      case NameRequest.LIST_NAME_ENTRIES:
	this.listByName(nameRequest.name(), true);
	break;
      case NameRequest.LIST_VALUE_ENTRIES:
	this.listByValue(nameRequest.name(), true);
	break;
      case NameRequest.LIST_TYPE_ENTRIES:
	this.listByType(nameRequest.name(), true);
	break;
      default:
	ACE.ERROR("Unknown type: " + nameRequest.requestType());

	// Send a failure message.  This will only work if the other
	// side is expecting something like a NameReply rather than
	// a NameRequest.  It would've been better to have everything
	// use NameRequests to avoid this kind of thing.
	NameReply reply = new NameReply (NameReply.FAILURE, 0);
	reply.streamOutTo(peer ().dataOutputStream ());
	break;
      }

  }

  /**
   * 
   * Bind a name and a (value, type) pair.  All this data is given in the
   * NameRequest from the client.  Returns a NameReply back to the client
   * with either Reply.SUCCESS or Reply.FAILURE as the type.
   *
   *@param request        NameRequest given by the client
   *@param rebind         Is this a rebind or not?
   */
  void bind (NameRequest request, 
	     boolean rebind) throws IOException
  {
    // The hash table entries consists of (String name, ValueType data) 
    // pairs, so create the appropriate ValueType
    ValueType vt = new ValueType(request.type(),
				 request.value());

    // Reply to tell sender of success or failure
    NameReply reply = new NameReply();

    // If it's a rebind request, overwrite the old entry.  If the key doesn't
    // exist, add it.  If it does exist and it's not a bind request, return
    // a failure code via a NameReply.
    if ((rebind) || (!this.mapping_.containsKey(request.name()))) {
      
      ACE.DEBUG ("Binding: " + request.name() + " and " + vt.value_);
      
      // Add/Update the entry in the hash table
      this.mapping_.put(request.name(), vt);
      
      // Set the reply code to success
      reply.type(NameReply.SUCCESS);
      
    } else {
      
      ACE.DEBUG ("Key " + request.name() + " already exists");
      
      // Set reply code to failure
      reply.type(NameReply.FAILURE);
      
      // reply error code unused as far as I know
    }
	
    reply.streamOutTo(peer ().dataOutputStream ());
  }

  /**
   * Given a name, this looks up and returns the type and value.  This is
   * done by sending back a full NameRequest with the correct info.  If
   * there is a problem, an "empty" NameRequest is returned -- it has no
   * name, type, or value fields.
   *
   *@param request     NameRequest sent by the client (has the name to lookup)
   */
  void resolve (NameRequest request) throws IOException
  {
    // A NameRequest is also used in response
    NameRequest result;

    // If the requested name is in the hash table, return the data
    if (this.mapping_.containsKey(request.name())) {

      // Get the data pair based on the name
      ValueType vt = (ValueType)this.mapping_.get(request.name());

      ACE.DEBUG("Good resolve: " + vt.value_);

      // Fill the reply structure 
      result = new NameRequest(NameRequest.RESOLVE,
			       null,
			       vt.value_,
			       vt.type_,
			       null);

    } else {

      // Otherwise return a null response 
      result = new NameRequest(NameRequest.RESOLVE,
			       null,
			       null,
			       null,
			       null);

    }

    result.streamOutTo (peer ().dataOutputStream ());
  }

  /**
   * 
   * Given a name, remove its entry in the mapping.  Returns a NameReply
   * to the client with NameReply.SUCCESS or NameReply.FAILURE.
   *
   *@param request    NameRequest from the client (has the name to remove)
   */
  void unbind (NameRequest request) throws IOException
  {
    NameReply reply = new NameReply();

    // If the given key isn't in the table, return an error
    // Otherwise remove it.  Uses a NameReply to respond.
    if (!this.mapping_.containsKey(request.name())) 
      reply.type(NameReply.FAILURE);
    else {
      this.mapping_.remove(request.name());
      reply.type(NameReply.SUCCESS);
    }

    // Send the reply out to the socket
    reply.streamOutTo (peer ().dataOutputStream ());
  }

  /**
   *
   * Given a pattern string (given in NameRequest's name field), this 
   * finds all the entries in the mapping which have a name that begins with
   * the string.  Each one is sent back separately via a NameRequest, and this
   * sequence is followed by a blank NameRequest.
   *
   *@param pattern           Pattern to find (what result names should 
   *                         begin with)
   *@param completeLookup    Should the value and type be returned as well?
   */
  void listByName (String pattern, 
		   boolean completeLookup) throws IOException
  {
    // Get a listing of all the keys in the hash table
    Enumeration enum = this.mapping_.keys();

    // References used in the loop
    String name;
    ValueType vt;

    // A NameRequest is used to return each item corresponding to the pattern.
    NameRequest result = 
      new NameRequest((completeLookup ? NameRequest.LIST_NAMES :
		       NameRequest.LIST_NAME_ENTRIES),
		      null,
		      null,
		      null,
		      null);

    // Keep ourselves safe from null pointer exceptions
    if (pattern == null)
      pattern = new String("");
      
    // Scan through all the elements
    while (enum.hasMoreElements()) {

      // Get a key
      name = (String)enum.nextElement();

      // Does it fit the pattern?
      if (name.startsWith(pattern)) {

	// Set the result name
	result.name(name);

	// Only make another hash table request if the user
	// wants all the data
	if (completeLookup) {

	  // Get data from the hash table
	  vt = (ValueType)mapping_.get(name);

	  // Set the rest of the data
	  result.type(vt.type_);
	  result.value(vt.value_);
	}
	  
	// Send it to the socket
	result.streamOutTo (peer ().dataOutputStream ());
      }
    }
    
    // Send final null message
    result.name(null);
    result.type(null);
    result.value(null);
    result.requestType(NameRequest.MAX_ENUM);
    result.streamOutTo (peer ().dataOutputStream ());
  }

  /**
   *
   * Given a pattern string (given in NameRequest's name field), this 
   * finds all the entries in the mapping which have a type that begins with
   * the string.  Each one is sent back separately via a NameRequest, and this
   * sequence is followed by a blank NameRequest.
   *
   *@param pattern           Pattern to find (what result types should 
   *                         begin with)
   *@param completeLookup    Should the value be returned as well?  This is 
   *                         only used to decide between LIST_TYPES and 
   *                         LIST_TYPE_ENTRIES since we might as well send 
   *                         back both if we look them up together.
   */
  void listByType (String pattern, 
		   boolean completeLookup) throws IOException
  {
    // Get a listing of all the keys in the hash table
    Enumeration enum = this.mapping_.keys();

    // References used in the loop
    String name;
    ValueType vt;

    // A NameRequest is used to return each item corresponding to the pattern.
    NameRequest result = 
      new NameRequest((completeLookup ? NameRequest.LIST_TYPES :
		       NameRequest.LIST_TYPE_ENTRIES),
		      null,
		      null,
		      null,
		      null);
    // Keep ourselves safe from null pointer exceptions
    if (pattern == null)
      pattern = new String("");

      // Scan through all the elements
    while (enum.hasMoreElements()) {

      // Get a key
      name = (String)enum.nextElement();

      // Have to get all the data for this entry to compare
      vt = (ValueType)mapping_.get(name);

      // Does it fit the pattern?
      if (vt.type_ != null)
	if (vt.type_.startsWith(pattern)) {

	  // Set the result values
	  result.name(name);
	  result.type(vt.type_);
	  result.value(vt.value_);
      
	  // Send it out to the socket
	  result.streamOutTo (peer ().dataOutputStream ());
	}
    }

    // Send final null message
    result.name(null);
    result.type(null);
    result.value(null);
    result.requestType(NameRequest.MAX_ENUM);
    result.streamOutTo (peer ().dataOutputStream ());
  }
  /**
   *
   * Given a pattern string (given in NameRequest's name field), this 
   * finds all the entries in the mapping which have a value that begins with
   * the string.  Each one is sent back separately via a NameRequest, and this
   * sequence is followed by a blank NameRequest.
   *
   *@param pattern           Pattern to find (what result values should 
   *                         begin with)
   *@param completeLookup    Should the value be returned as well?  This is 
   *                         only used to decide between LIST_TYPES and 
   *                         LIST_TYPE_ENTRIES since we might as well send 
   *                         back both if we look them up together.
   */
  void listByValue (String pattern, 
		    boolean completeLookup) throws IOException
  {
    // Get a listing of all the keys in the hash table
    Enumeration enum = this.mapping_.keys();

    // References used in the loop
    String name;
    ValueType vt;

    // A NameRequest is used to return each item corresponding to the pattern.
    NameRequest result = 
      new NameRequest((completeLookup ? NameRequest.LIST_VALUES :
		       NameRequest.LIST_VALUE_ENTRIES),
		      null,
		      null,
		      null,
		      null);
    // Keep ourselves safe from null pointer exceptions
    if (pattern == null)
      pattern = new String("");

      // Scan through all the elements
    while (enum.hasMoreElements()) {

      // Get a key
      name = (String)enum.nextElement();

      // Have to get all the data for this entry to compare
      vt = (ValueType)mapping_.get(name);

      // Does it fit the pattern?
      if (vt.value_ != null)
	if (vt.value_.startsWith(pattern)) {

	  // Set the result values
	  result.name(name);
	  result.type(vt.type_);
	  result.value(vt.value_);
      
	  // Send it out to the socket
	  result.streamOutTo (peer ().dataOutputStream ());
	}
    }

    // Send final null message
    result.name(null);
    result.type(null);
    result.value(null);
    result.requestType(NameRequest.MAX_ENUM);
    result.streamOutTo (peer ().dataOutputStream ());
  }

  // References to the hash table and the timer queue
  private Hashtable mapping_;

  /**
   * A simple wrapper to keep the type and value together in
   * the hash table.
   */
  static class ValueType implements Serializable
  {
    /**
     * Constructor
     *
     *@param type   Type string to include
     *@param value  Value string to include
     */
    ValueType(String type, String value)
    { this.type_ = type; this.value_ = value; }

    public String type_;
    public String value_;
  }
}
