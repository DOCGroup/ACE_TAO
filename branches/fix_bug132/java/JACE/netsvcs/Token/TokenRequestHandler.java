package JACE.netsvcs.Token;

import java.io.*;
import java.net.SocketException;
import java.util.*;
import JACE.Connection.*;
import JACE.OS.*;
import JACE.netsvcs.Handler;

/**
 * Created by TokenAcceptor to handle token requests.  Delegates to
 * the appropriate LockHandler.  This is fairly robust, and can handle
 * multiple clients and locks (meaning requests can come in to this
 * handle with varying client IDs and token names and still be processed
 * and released appropriately.)  Compatible with the C++ ACE token service.
 *
 *@author Everett Anderson
 */
class TokenRequestHandler extends Handler
{
  /** 
   * Default constructor.
   */
  public TokenRequestHandler() {
    this.clients_ = new Vector (10);
  }
  
  /** 
   * Creates a new TokenRequest instance.
   */
  public Object newRequest ()
  {
    return new TokenRequest ();
  }

  /** 
   * Sends an error message to a client with the TokenReply.EIO
   * errno before abandoning the connection.  This is used when an IO 
   * error occured while receiving the request.
   *
   *@param lastRequest request object to get the arg from
   */
  protected void sendAbortMessage (TokenRequest lastRequest)
  {
    TokenReply reply = new TokenReply (TokenReply.EIO,
				       lastRequest.arg ());
    try {
      reply.streamOutTo (this.peer ().dataOutputStream ());
    } catch (Exception e) {
      // Doesn't matter if there is an error here, we've abandoned
      // the connection.
    }
  }

  /** 
   * Safely shuts down this handler, making sure to release any locks
   * that were touched by clients from this TokenRequestHandler.
   * 
   *@return -1 on failure, 0 on success
   */
  public synchronized int close ()
  {
    // For every client X that has used this handler
    //    for every LockHandler that X has used
    //        release the lock until it fails because X isn't the owner
    //    remove the client entries
    // Call Handler.close ()
    if (!done ()) {
      
      TokenAcceptor parent = (TokenAcceptor) parent ();
      Enumeration clientEnum = clients_.elements ();

      while (clientEnum.hasMoreElements ()) {
	String clientID = (String)clientEnum.nextElement ();
	
	Enumeration handlers = parent.getClientLockHandlers (clientID);
	if (handlers == null)
	  continue;
	
	int num = 0;
	
	while (handlers.hasMoreElements ()) {
	  LockHandler handler = (LockHandler)handlers.nextElement ();
	  
	  handler.abandonLock (clientID);

	  num++;
	}
	
	parent.removeClient (clientID);
      }

      return super.close ();
    }

    return 0;
  }

  /**
   * Read in the given TokenRequest and delegates to the appropriate
   * LockHandler.
   *
   *@see JACE.netsvcs.Handler
   *@param requestObject TokenRequest object to use
   */
  public void processRequest (Object requestObject)
    throws SocketException, EOFException, IOException
  {
    TokenRequest request = (TokenRequest)requestObject;
    TokenAcceptor parent = (TokenAcceptor) parent ();

    try {
      request.streamInFrom (this.peer ().dataInputStream ());
      
      if (!request.tokenName ().equals (lastTokenName_)) {
	// Switched tokens:
	//
	// Either find a handler that's already been made (which would
	// mean this token has been accessed before), or create a new
	// one with a new token
	handler_ = parent.getLockHandler(request.tokenName(),
					 request.tokenType());
	
	if (handler_ == null) {
	  // The client asked for an operation on a type of token
	  // that we don't know about.
	  ACE.ERROR ("Unknown lock type: " + request.tokenType ());
	  TokenReply error = new TokenReply (TokenReply.EINVAL,
					     request.arg ());
	  error.streamOutTo(this.peer ().dataOutputStream ());
	  return;
	}
	
	// Add this LockHandler to the list of those accessed by
	// this clientID
	parent.addClientLockHandler (request.clientID (),
				     handler_);
      }
      
      if (!request.clientID ().equals (lastClientID_)) {
	// Switched clients
	
	if (!clients_.contains (request.clientID ())) 
	  clients_.addElement (request.clientID ());
	
	parent.addClientLockHandler (request.clientID (),
				     handler_);
      }
      
      lastClientID_ = request.clientID ();
      lastTokenName_ = request.tokenName ();
      
      TokenReply reply = handler_.handleRequest(this, request);

      reply.streamOutTo(this.peer ().dataOutputStream ());
      
    } catch (NullPointerException e) {
      sendAbortMessage (request);
      throw e;
    } catch (IOException e) {
      sendAbortMessage (request);
      throw e;
    }
  }

  // List of clientIDs that have been processed by this instance
  // of TokenRequestHandler.  This is useful when abandoning the
  // locks of any clients that have been using this socket.
  private Vector clients_;

  // Name of the last token accessed
  private String lastTokenName_ = null;

  // Last client ID which accessed a token from this handler
  private String lastClientID_ = null;

  // Current LockHandler
  private LockHandler handler_ = null;
}

