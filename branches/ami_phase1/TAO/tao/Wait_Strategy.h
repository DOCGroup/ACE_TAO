// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Wait_Strategy.h
//
// = DESCRIPTION
//     Classes to strategize waiting for reply.
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================


#include "tao/GIOP.h"

class TAO_Request_Mux_Strategy;
class TAO_Transport;

class TAO_Export TAO_Wait_Strategy
{
  // = TITLE
  //
  //    Strategy for waiting for the reply.
  // 
  // = DESCRIPTION
  // 
  
public:
  TAO_Wait_Strategy (TAO_Transport *transport);
  // Constructor.
  
  virtual ~TAO_Wait_Strategy (void);
  // Destructor.
  
  virtual int wait (void) = 0;
  // Base class virtual method.

protected:
  TAO_Transport *transport_;
  // Transport object.
};

class TAO_Export TAO_Wait_On_Reactor : public TAO_Wait_Strategy
{
  // = TITLE
  // 
  //    Wait on the Reactor. Happens in s Single Threaded client
  //    environment. 
  //
  // = DESCRIPTION
  //

public:
  TAO_Wait_On_Reactor (TAO_Transport *transport);
  // Constructor.
  
  virtual ~TAO_Wait_On_Reactor (void);
  // Destructor.
  
  virtual int wait (void);
  // Do the event loop of the Reactor.
};

class TAO_Export TAO_Wait_On_Leader_Follower : public TAO_Wait_Strategy
{
  // = TITLE
  //
  //    Wait according to the Leader-Follower model. Leader does the
  //    event loop of the Reactor and the Followers wait on the
  //    condition variable.
  //    
  // = DESCRIPTION
  // 

public:
  TAO_Wait_On_Leader_Follower (TAO_Transport *transport);
  // Constructor.
  
  virtual ~TAO_Wait_On_Leader_Follower (void);
  // Destructor.
  
  virtual int wait (void);
  // Wait according to the L-F model.
};

class TAO_Export TAO_Wait_On_Read :  public TAO_Wait_Strategy
{
  // = TITLE
  // 
  //    Wait on receiving the reply.
  //
  // = DESCRIPTION
  //
  
public:
  TAO_Wait_On_Read (TAO_Transport *transport);
  // Constructor.

  virtual ~TAO_Wait_On_Read (void);
  // Destructor.

  virtual int wait (void);
  // Wait on the read operation.
};
