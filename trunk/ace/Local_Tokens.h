/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Local_Tokens
//
// = AUTHOR
//    Karl-Heinz Dorn (kdorn@erlh.siemens.de)
//    Douglas C. Schmidt (schmidt@cs.wustl.edu)
//    Tim Harrison (harrison@cs.wustl.edu)
//
// = DESCRIPTION
//   This file contains definitions for the following classes:
//
// public:
//   7. ACE_Token_Proxy
//   8. ACE_Null_Token : public ACE_Token_Proxy
//   9. ACE_Local_Mutex : public ACE_Token_Proxy
//   *. ACE_Local_RLock : public ACE_Local_Mutex
//   &. ACE_Local_WLock : public ACE_Local_Mutex
// private:
//   1. ACE_TOKEN_CONST
//   3. ACE_TPQ_Entry
//   b. ACE_TSS_TPQ_Entry
//   c. ACE_TPQ_Iterator
//   4. ACE_Token_Proxy_Queue
//   5. ACE_Tokens
//   6. ACE_Mutex_Token : public ACE_Tokens
//   12. ACE_RW_Token : public ACE_Tokens
//   a. ACE_Token_Name
//   
// ============================================================================

#if !defined (ACE_LOCAL_MUTEX_H)
#define ACE_LOCAL_MUTEX_H

#include "ace/Synch.h"
#include "ace/Containers.h"
#include "ace/Synch_Options.h"
#include "ace/Map_Manager.h"

// 1.
class ACE_Export ACE_TOKEN_CONST
{
  // = TITLE
  //   Not a public interface.
  //   
  // = DESCRIPTION
  //   Constant definitions and typdefs for Token library.  Mostly,
  //   this class is necessary to fight the compiler with order of
  //   declaration errors.
public:
#if defined (ACE_MT_SAFE) 
  // ACE platform supports some form of threading.
  typedef ACE_Condition_Thread_Mutex COND_VAR;
  typedef ACE_Thread_Mutex MUTEX;
  typedef ACE_Thread_Mutex_Guard GUARD;
#else
  typedef ACE_Null_Condition_Mutex COND_VAR;
  typedef ACE_Null_Mutex MUTEX;
  typedef ACE_Null_Mutex_Guard GUARD;
#endif /* ACE_HAS_THREADS */
};

// Forward decl.
class ACE_Token_Proxy;

// 3..
class ACE_Export ACE_TPQ_Entry
  // = TITLE
  //   Not a public interface.
  //   
  // = DESCRIPTION
  //   Token Proxy Queue entry.
  //   Used in the ACE_Token_Proxy_Queue
{
friend class ACE_Token_Manager;
public:
  typedef void (*PTVF) (void *);
 
  ACE_TPQ_Entry (void);
  // Null constructor.

  ACE_TPQ_Entry (const ACE_Token_Proxy *proxy,
		 const char *client_id);
  // Construction.

  ACE_TPQ_Entry (const ACE_TPQ_Entry &rhs);
  // Copy constructor.

  ~ACE_TPQ_Entry (void);
  // Death.

  void operator= (const ACE_TPQ_Entry &rhs);
  // Copy operator use by the queue.

  // = Set/get top of the queue.
  ACE_Token_Proxy *proxy (void) const;
  void proxy (ACE_Token_Proxy *);

  // = Delta/get nesting level of the entry.
  int nesting_level (void) const;
  void nesting_level (int delta);

  // = Set/get client_id of the entry.
  const char *client_id (void) const;
  void client_id (const char *);

  int equal_client_id (const char *id);
  // Returns 1 if <id> == client id.  Does not check for <id> == 0.

  void set (void (*sleep_hook)(void *));
  // One method for arg and sleep_hook.

  // = Set/get sleep hook of the entry.
  void sleep_hook (void (*sh)(void *));
  PTVF sleep_hook (void) const;

  void call_sleep_hook (void);
  // Call the sleep hook function or method passing arg.

  void dump (void) const;
  // Dump the state of the class.

  // = Used to block the thread if an acquire fails with EWOULDBLOCK.
  ACE_TOKEN_CONST::COND_VAR cond_var_;
  ACE_TOKEN_CONST::MUTEX lock_;

  ACE_TPQ_Entry *next_;
  // Pointer to next in list.

  // = Get/set whether this client is blocked waiting for a token.
  int waiting (void) const;
  void waiting (int w);

private:
  int waiting_;
  // This client is waiting for a token.

  ACE_Token_Proxy *proxy_;
  // Proxy.

  int nesting_level_;
  // Nesting level.

  void *arg_;
  // Arg.

  char client_id_[ACE_MAXCLIENTIDLEN];
  // Client id.

  void (*sleep_hook_)(void *);
  // Sleep hook.
};

// ************************************************************

// b..
#if defined (ACE_NO_TSS_TOKENS)
typedef ACE_TPQ_Entry ACE_TPQ_ENTRY;
#else
typedef ACE_TSS<ACE_TPQ_Entry> ACE_TPQ_ENTRY;
#endif /* ACE_NO_TSS_TOKENS */

class ACE_Export ACE_TSS_TPQ_Entry : public ACE_TPQ_ENTRY
  // = TITLE
  //    Not a public interface.
  // = DESCRIPTION
  //    ACE_TSS_TPQ_Entry
{
public:
  ACE_TSS_TPQ_Entry (const ACE_Token_Proxy *proxy,
		     const char *client_id);
  // These are passed to the constructor of ACE_TPQ_Entry in
  // make_TSS_TYPE

  virtual ACE_TPQ_Entry *make_TSS_TYPE (void) const;
  // Allows us to pass args to the construction of the TSS object.

  operator ACE_TPQ_Entry *(void);
  // Operator overloading and inheritence don't mix.

  void dump (void) const;
  // Dump the state of the class.

#if defined (ACE_NO_TSS_TOKENS)
  ACE_TSS_TPQ_Entry *operator-> (void)
    {
      return this;
    }
#endif

private:
  ACE_TSS_TPQ_Entry (const ACE_TSS_TPQ_Entry &);
  void operator= (const ACE_TSS_TPQ_Entry &);
  // Private: should not be used

  // = These are passed to the constructor of ACE_TPQ_Entry in
  // make_TSS_TYPE
  const ACE_Token_Proxy *proxy_;
  // Proxy.
  const char *client_id_;
  // Client_id.
};

// ************************************************************

class ACE_Token_Proxy_Queue;

// c..
class ACE_Export ACE_TPQ_Iterator
  // = TITLE
  //   Not a public interface.
  //   
  // = DESCRIPTION
  //   Iterates through ACE_Token_Proxy_Queues.
{
public:
  ACE_TPQ_Iterator (ACE_Token_Proxy_Queue &q);
  // Construction.

  int next (ACE_TPQ_Entry *&next_item);
  // Pass back the <next_item>. 

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  void advance (void);
  // Move forward by one element in the queue.

  void dump (void) const;
  // Dump the state of an object.

private:
  ACE_TPQ_Entry *current_;
};

// 4..
class ACE_Export ACE_Token_Proxy_Queue
  // = TITLE
  //   Not a public interface.
  //   
  // = DESCRIPTION
  //   Token waiter list.
  //   This queue holds all the token proxies waiting for ownership of
  //   a token.  Along with the proxy reference, it also stores the
  //   nesting level, client id, and a magic cookie from the proxy.
  //   This queue stores the ACE_TPQ_Entries by pointer values.  It
  //   DOES NOT make copies.  Thus, the user is responsible to ensure
  //   that the TPQ's stick around.  This is motivated by the need to
  //   reduce dynamic memory allocation.
{
  friend class ACE_TPQ_Iterator;
public:
  ACE_Token_Proxy_Queue (void);
  // Construction.

  void enqueue (ACE_TPQ_Entry* new_entry,
		int position);
  // Enqueue a proxy, nesting level, client_id, and a magic cookie at
  // the given position in the list.  If the position is -1, we
  // enqueue at the end of the list (I think).

  const ACE_TPQ_Entry* head (void);
  // Top of the queue.

//  int member (const char *id);
  // Is this id in the waiter list?

  void dequeue (void);
  // Remove the top waiter.

  void remove (const ACE_TPQ_Entry *remove_me);
  // Remove the waiter whose proxy ref matches remove_me.

  int size (void);
  // The number of waiters.

  void dump (void) const;
  // Dump the state of the class.

protected:
  ACE_TPQ_Entry *head_;
  // Head.
  ACE_TPQ_Entry *tail_;
  // Tail.
  int size_;
  // Size.
};

// 5..
class ACE_Export ACE_Tokens
  // = TITLE
  //   Not a public interface.
  //   
  // = DESCRIPTION
  //   Abstract representation of ACE tokens.
  //   Currently, I don't see a reason for providing an abstract
  //   interface at this level of the library.  As of yet, no one uses
  //   ACE_Tokens derivatives through this abstract interface except
  //   for Token_Manager.  It only uses the statistical methods which
  //   are shared by all Tokens.  For that reason, it still makes
  //   since to have a common base class.  However, acquire, renew,
  //   and release do not need to have matching interfaces throughout
  //   all Tokens.

  // = EXTENDING TOKENS
  //   To add a new type of token (e.g. semaphore), this class must be
  //   subtyped to define the new semantics.  See ACE_Token_Manager
  //   for details.
{
public:

  ACE_Tokens (void);
  // Null constructor.

  virtual int acquire (ACE_TPQ_Entry *caller,
		       int ignore_deadlock,
		       int notify) = 0;
  // No implementation.

  virtual int tryacquire (ACE_TPQ_Entry *caller) = 0;
  // No implementation.

  virtual int renew (ACE_TPQ_Entry *caller,
		     int requeue_position) = 0;
  // No implementation.

  virtual int release (ACE_TPQ_Entry *caller) = 0;
  // No implementation.

  void make_owner (ACE_TPQ_Entry *caller);
  // Move the caller to the front of the waiter list.  This is for use
  // with remote mutexes and shadow mutexes.

  void remove (ACE_TPQ_Entry *caller);
  // Remove the caller from the waiter list.

  // = Accessor methods.

  typedef ACE_Unbounded_Stack<ACE_TPQ_Entry *> OWNER_STACK;
  // Stack of owners.

  virtual int owners (OWNER_STACK &o, const char *id) = 0;
  // Returns a stack of the current owners.  Returns -1 on error, 0 on
  // success.  If <id> is non-zero, returns 1 if id is an owner.

  virtual int is_waiting_for (const char *id) = 0;
  // Returns 1 if <id> is waiting for this token.  0 otherwise.

  virtual int is_owner (const char *id) = 0;
  // Returns 1 if <id> is an owner of this token.  0 otherwise.

  virtual ACE_Token_Proxy_Queue *waiters (void);
  // Return the queue of waiters.

  virtual int no_of_waiters (void);
  // Return the number of proxies that are currently waiting to get
  // the token.

  const char *owner_id (void);
  // The current owner.

  const char* name (void);
  // Token name.

  // = Reference counting.  These are only called by the
  // Token_Manager. 
  void inc_reference (void);
  int dec_reference (void);

  void dump (void) const;
  // Dump the state of the class.

  enum TOKEN_TYPES { MUTEX, RWLOCK };
  // These are the Token types supported by the library at ship time.
  // There is no restriction on the number of Token types added by
  // "3rd parties."  These are only necessary for the Token Server.

  virtual int type (void) const = 0;
  // Provides a manual RTTI mechanism.  This method is used only by
  // ACE_Token_Request so that the type of a token can be sent to a
  // remote Token Server.

  // = The following methods allow the deadlock detection algorithm to
  // check if this token has been visited.

  void visit (int v);
  // Mark or unmark the token as visited.

  int visited (void);
  // Check if the token has been visited.

  ACE_TPQ_Entry *owner (void);
  // All the data of the current owner.

protected:

  int visited_;
  // For the deadlock detection algorithm.

  int reference_count_;
  // Reference count.

  ACE_Token_Proxy_Queue waiters_;
  // List of client's owning and waiting the token.

  char token_name_[ACE_MAXTOKENNAMELEN];
  // Name of token.
};

class ACE_Local_Mutex;

// 6..
class ACE_Export ACE_Mutex_Token : public ACE_Tokens
  // = TITLE
  //    Not a public interface.
  //
  // = DESCRIPTION 
  //    Class that acquires, renews, and releases a process-local
  //    synchronization token.
  //    This class is a more general-purpose synchronization mechanism
  //    than SunOS 5.x mutexes.  For example, it implements "recursive
  //    mutex" semantics, where a thread that owns the token can
  //    reacquire it without deadlocking.  In addition, threads that are
  //    blocked awaiting the token are serviced in strict FIFO order as
  //    other threads release the token (SunOS 5.x mutexes don't strictly
  //    enforce an acquisition order).
{
public:
  ACE_Mutex_Token (const char* name);
  // life

  virtual ~ACE_Mutex_Token (void);
  // death

  // = Synchronization operations.
  // With acquire, renew, and release, the caller must be specified so
  // that multiple proxies (e.g. ACE_Local_Mutex) can use the same
  // token.

  virtual int acquire (ACE_TPQ_Entry *caller,
		       int ignore_deadlock,
		       int notify);
  // Returns 0 on success, -1 on failure with ACE_LOG_MSG->errnum() as
  // the reason.  If errnum == EWOULDBLOCK, and notify == 1,
  // ACE_Token_Proxy::sleep_hook() has been called on the current owner
  // of the token.  If ignore_deadlock is passed as 1 and errnum ==
  // EDEADLK, then deadlock was detected via ace_token_manager.

  virtual int tryacquire (ACE_TPQ_Entry *caller);
  // same as acquire, but fails if would block

  virtual int renew (ACE_TPQ_Entry *caller,
		     int requeue_position);
  // An optimized method that efficiently reacquires the token if no
  // other threads are waiting.  This is useful for situations where
  // you don't want to degrade the quality of service if there are
  // other threads waiting to get the token.  If <requeue_position> ==
  // -1 and there are other threads waiting to obtain the token we are
  // queued at the end of the list of waiters.  If <requeue_position>
  // > -1 then it indicates how many entries to skip over before
  // inserting our thread into the list of waiters (e.g.,
  // <requeue_position> == 0 means "insert at front of the queue").
  // Renew has the rather odd semantics such that if there are other
  // waiting threads it will give up the token even if the
  // nesting_level_ > 1.  I'm not sure if this is really the right
  // thing to do (since it makes it possible for shared data to be
  // changed unexpectedly) so use with caution...
  // Returns 0 on success, -1 on failure with ACE_LOG_MSG->errnum() as
  // the reason.  If errnum == EWOULDBLOCK, and notify == 1,
  // ACE_Token_Proxy::sleep_hook() has been called on the current owner
  // of the token.

  virtual int release (ACE_TPQ_Entry *caller);
  // Relinquish the token.  If there are any waiters then the next one
  // in line gets it.  If the caller is not the owner, caller is
  // removed from the waiter list.

  void dump (void) const;
  // Dump the state of the class.

  virtual int type (void) const;
  // Returns ACE_Tokens::MUTEX.

  virtual int owners (OWNER_STACK &o, const char *id);
  // Returns a stack of the current owners.  Returns -1 on error, 0 on
  // success.  If <id> is non-zero, returns 1 if id is an owner.

  virtual int is_waiting_for (const char *id);
  // Returns 1 if <id> is waiting for this token.  0 otherwise.

  virtual int is_owner (const char *id);
  // Returns 1 if <id> is an owner of this token.  0 otherwise.

private:
  ACE_TOKEN_CONST::MUTEX lock_;
  // ACE_Mutex_Token used to lock internal data structures.
};

// 12..
class ACE_Export ACE_RW_Token : public ACE_Tokens
  // = TITLE
  //    Not a public interface.
  //
  // = DESCRIPTION 
  //    Class that acquires, renews, and releases a process-local
  //    synchronization token.
  //    This class is a more general-purpose synchronization mechanism
  //    than SunOS 5.x mutexes.  For example, it implements "recursive
  //    mutex" semantics, where a thread that owns the token can
  //    reacquire it without deadlocking.  In addition, threads that are
  //    blocked awaiting the token are serviced in strict FIFO order as
  //    other threads release the token (SunOS 5.x mutexes don't strictly
  //    enforce an acquisition order).
{
public:
  ACE_RW_Token (const char* name);
  // Life.

  virtual ~ACE_RW_Token (void);
  // Death.

  // = Synchronization operations.
  // With acquire, renew, and release, the caller must be specified so
  // that multiple proxies (e.g. ACE_Local_Mutex) can use the same
  // token.

  virtual int acquire (ACE_TPQ_Entry *caller,
		       int ignore_deadlock,
		       int notify);
  // Returns 0 on success, -1 on failure with ACE_LOG_MSG->errnum() as
  // the reason.  If errnum == EWOULDBLOCK, and notify == 1,
  // ACE_Token_Proxy::sleep_hook() has been called on the current owner
  // of the token.  If ignore_deadlock is passed as 1 and errnum ==
  // EDEADLK, then deadlock was detected via ace_token_manager.

  virtual int tryacquire (ACE_TPQ_Entry *caller);
  // same as acquire except fails on would block

  virtual int renew (ACE_TPQ_Entry *caller,
		     int requeue_position);
  // An optimized method that efficiently reacquires the token if no
  // other threads are waiting.  This is useful for situations where
  // you don't want to degrade the quality of service if there are
  // other threads waiting to get the token.  If <requeue_position> ==
  // -1 and there are other threads waiting to obtain the token we are
  // queued at the end of the list of waiters.  If <requeue_position>
  // > -1 then it indicates how many entries to skip over before
  // inserting our thread into the list of waiters (e.g.,
  // <requeue_position> == 0 means "insert at front of the queue").
  // Renew has the rather odd semantics such that if there are other
  // waiting threads it will give up the token even if the
  // nesting_level_ > 1.  I'm not sure if this is really the right
  // thing to do (since it makes it possible for shared data to be
  // changed unexpectedly) so use with caution...
  // Returns 0 on success, -1 on failure with ACE_LOG_MSG->errnum() as
  // the reason.  If errnum == EWOULDBLOCK, and notify == 1,
  // ACE_Token_Proxy::sleep_hook() has been called on the current owner
  // of the token.

  virtual int release (ACE_TPQ_Entry *caller);
  // Relinquish the token.  If there are any waiters then the next one
  // in line gets it.  If the caller is not the owner, caller is
  // removed from the waiter list.

  void dump (void) const;
  // Dump the state of the class.

  enum PROXY_TYPE { READER, WRITER };
  // These are the types that proxies can be.

  virtual int type (void) const;
  // Returns READER or WRITER.

  virtual int owners (OWNER_STACK &o, const char *id);
  // Returns a stack of the current owners.  Returns -1 on error, 0 on
  // success.  If <id> is non-zero, returns 1 if id is an owner.

  virtual int is_waiting_for (const char *id);
  // Returns 1 if <id> is waiting for this token.  0 otherwise.

  virtual int is_owner (const char *id);
  // Returns 1 if <id> is an owner of this token.  0 otherwise.

protected:
  int num_writers_;
  // the number of waiting writers.

  ACE_TOKEN_CONST::MUTEX lock_;
  // ACE_Mutex_Token used to lock internal data structures.

  void notify_new_owner (ACE_TPQ_Entry *caller);
  // Sets the new owner.
};

// a..
class ACE_Token_Name
  // = TITLE
  //   Allows Token_Manger to identify tokens.
  //
  // = DESCRIPTION
  //   For now, this is just a string.  We need a string class anyway
  //   to use in ACE_Map_Manager.  Having this class (instead of
  //   SString) allows us to easily change if needed.  For instance,
  //   we may choose to identify tokens by name and *type* in the
  //   future.
{
public:
  ACE_Token_Name (const char *token_name = 0);
  // Construction.

  ACE_Token_Name (const ACE_Token_Name &rhs);
  // Copy construction.

  virtual ~ACE_Token_Name (void);
  // Death.

  void operator= (const ACE_Token_Name &rhs);
  // Copy.

  int operator== (const ACE_Token_Name &rhs) const;
  // Comparison.

  const char *name (void) const;
  // Token name.

  void name (const char *new_name);
  // Token name.

  void dump (void) const;
  // Dump the state of the class.

private:
  char token_name_[ACE_MAXTOKENNAMELEN];
  // Name of the token.
};


// 7..
class ACE_Export ACE_Token_Proxy
  // = TITLE
  //   Abstract representation of ACE tokens.
  //   
  // = DESCRIPTION
  //   Interface for all Tokens in ACE.  This class implements the
  //   synchronization needed for tokens (condition variables etc.)
  //   The algorithms for the operations (acquire, release, etc.)
  //   operate on the generic ACE_Tokens interface.  Thus, the _type_
  //   of token (mutex, rwlock) can be set at construction of
  //   ACE_Token_Proxy.  You can use all Tokens in ACE through the
  //   ACE_Token_Proxy by passing the proper values at construction.
  //   Alternatively, there are class definitions which "know" how to
  //   do this (ACE_Local_Mutex, ACE_Local_RLock, ACE_Local_WLock).

  // = EXTENDING TOKENS
  //   To add a new type of token (e.g. semaphore), this class is not
  //   changed.  See ACE_Token_Manager for details.

  // = RESTRICTIONS
  //   Tokens (e.g. ACE_Mutex_Token) assume that it can always call
  //   ACE_Token_Proxy::token_acquired () on a new token owner.  This
  //   is not a problem for synchronous use of token proxies (that is,
  //   when acquires block until successful.)  However, for
  //   implementations of the Token Server, which may use asynch
  //   operations, the proxy can not go away after an acquire until
  //   the token is acquired.  This is not really a problem, but
  //   should be understood.
{
friend class ACE_Token_Manager;
friend class ACE_Token_Invariant_Manager; // For testing.
public:

  // Initialization and termination methods.
  ACE_Token_Proxy (void);
  // Construction.

  virtual ~ACE_Token_Proxy (void);
  // Death.

  virtual int open (const char *name,
		    int ignore_deadlock = 0,
		    int debug = 0);
  // <name> is the string uniquely identifying the token.
  // <ignore_deadlock> can be 1 to disable deadlock notifications.
  // <debug> prints debug messages.

  // = The following methods have implementations which are
  // independent of the token semantics (mutex, rwlock, etc.)  They
  // forward operations to the underlying token and perform the
  // necessary blocking semantics for operations (condition variables
  // etc.)  This allows reuse of the blocking code as well as having
  // multiple proxies to the same token.

  virtual int acquire (int notify = 0,
		       void (*sleep_hook)(void *) = 0,
		       ACE_Synch_Options &options =
		       ACE_Synch_Options::defaults);
  // Calls acquire on the token.  Blocks the calling thread if would
  // block.

  virtual int renew (int requeue_position = -1,
		     ACE_Synch_Options &options =
		     ACE_Synch_Options::defaults);
  // Calls renew on the token.  Blocks the calling thread if would
  // block.

  virtual int tryacquire (void (*sleep_hook)(void *) = 0);
  // Calls renew on the token.

  virtual int release (ACE_Synch_Options &options =
		       ACE_Synch_Options::defaults);
  // Calls release on the token.

  virtual int remove (ACE_Synch_Options &options =
		      ACE_Synch_Options::defaults);
  // Calls remove on the token.

  // = Utility methods.

  virtual const char *client_id (void) const;
  // Get the client id of the proxy.  This is implemented as
  // thread-specific data.
  
  virtual void client_id (const char *client_id);
  // Set the client_id for the calling thread.  I strongly recommend
  // that this not be used unless you really know what you're doing.
  // I use this in the Token Server, and it caused many headaches.

  virtual const char *name (void) const;
  // Return the name of the token.  This is important for use within
  // the token servers (local and remote) as well as with token
  // collections.  So, all derivations of ACE_Token_Proxy must be able to
  // stringify some name.  The name must uniquely identify a token.
  // So, for instance, the token within the reactor should probably be
  // called "Reactor Token."

  virtual void sleep_hook (void);
  // This should really be called someone_waiting ().
  // This is called by ACE_Token_xx's when another proxy enters the
  // waiting list and requests that the current token holder be notified.

  virtual void token_acquired (ACE_TPQ_Entry *);
  // This is called when a queued (waiting) proxy is removed from the
  // waiters list and given the token.

  virtual const char *owner_id (void);
  // the client id of the current token holder
  
  virtual ACE_Token_Proxy *clone (void) const = 0;
  // Return a dynamically allocated clone of the derived class.

  void dump (void) const;
  // Dump the state of the class.

  virtual int type (void) const;
  // This method can be used be Tokens (e.g. Readers/Writer Tokens) to
  // distinguish between Proxy types.  For instance a Reader proxy
  // should return a different type value than a Writer proxy.  The
  // default implementation returns 0.

protected:
  ACE_Token_Proxy (const ACE_Token_Proxy &);
  // Duplication.

  int ignore_deadlock_;
  // If this is set, we ignore deadlock.

  int debug_;
  // Print a bunch of debug messages.

  ACE_Tokens *token_;
  // Reference to the actual logical token.  Many ACE_Local_Mutex
  // proxies can reference the same ACE_Mutex_Token.

  int handle_options (ACE_Synch_Options &options, 
		      ACE_TOKEN_CONST::COND_VAR &cv);
  // Handles cond_var waits.

  ACE_TSS_TPQ_Entry waiter_;
  // Waiter info used for asynchronous transactions.

  virtual ACE_Tokens *create_token (const char *name) = 0;
  // Make the correct type of ACE_Tokens.  This is called by the Token
  // Manager.
};

// 8..
class ACE_Export ACE_Null_Token : public ACE_Token_Proxy
  // = TITLE
  //   No op class for nonthreaded platform protocols.
{
public:
  ACE_Null_Token (void) {};
  // Construction.

  virtual int acquire (int /* notify */ = 0,
		       void (* /* sleep_hook */ )(void *) = 0,
		       ACE_Synch_Options & /* options */ =
		       ACE_Synch_Options::defaults) { return 0; }
  // Acquire.
		       
  virtual int renew (int /* requeue_position */ = -1,
		     ACE_Synch_Options & /* options */ =
		     ACE_Synch_Options::defaults) { return 0; }
  // Renew.

  virtual int tryacquire (void (* /* sleep_hook */)(void *) = 0) { return 0; }
  // Try acquire.

  virtual int release (ACE_Synch_Options & /* options */ =
		       ACE_Synch_Options::defaults) { return 0; }
  // Release.

  virtual int remove (ACE_Synch_Options & /* options */ =
		      ACE_Synch_Options::defaults) { return 0; }
  // Remove.

  virtual ACE_Token_Proxy *clone (void) const { return new ACE_Null_Token; }
  // Return a dynamically allocated clone of the derived class.

  void dump (void) const;
  // Dump the state of the class.

  virtual ACE_Tokens *create_token (const char *) { return 0; }
  // Do not allow the Token Manager to create us.
};

// 9..
class ACE_Export ACE_Local_Mutex : public ACE_Token_Proxy
  // = TITLE
  //    Class that acquires, renews, and releases a synchronization
  //    token local to the process. 
  //
  // = DESCRIPTION 
  //    This class is a more general-purpose synchronization mechanism
  //    than SunOS 5.x mutexes.  For example, it implements "recursive
  //    mutex" semantics, where a thread that owns the token can
  //    reacquire it without deadlocking.  In addition, threads that
  //    are blocked awaiting the token are serviced in strict FIFO
  //    order as other threads release the token (SunOS 5.x mutexes
  //    don't strictly enforce an acquisition order).  Lastly,
  //    ACE_Local_Mutex performs deadlock detection on acquire calls.
  //
  // = Synchronization operations.
  //   The interfaces for acquire, tryacquire, renew, release,
  //   etc. are defined in ACE_Token_Proxy.  The semantics for
  //   ACE_Local_Mutex are that of a mutex.
{
public:
  ACE_Local_Mutex (const char *token_name = 0,
		   int ignore_deadlock = 0,
		   int debug = 0);
  // <token_name> uniquely id's the token.
  // <ignore_deadlock> will allow deadlock to occur (useful for
  // testing).  <debug> prints a bunch of messages.

  void dump (void) const;
  // Dump the state of the class.

  virtual ACE_Token_Proxy *clone (void) const;
  // Return deep copy.

protected:
  virtual ACE_Tokens *create_token (const char *name);
  // Return a new ACE_Local_Mutex.
};

// *.
class ACE_Export ACE_Local_RLock : public ACE_Token_Proxy
// = TITLE
//    Class that acquires, renews, and releases a readers lock that
//    is local to the process.
//
// = DESCRIPTION 
//    This class implements the reader interface to canonical
//    readers/writer locks.  Multiple readers can hold the lock
//    simultaneously when no writers have the lock.  Alternatively,
//    when a writer holds the lock, no other participants (readers or
//    writers) may hold the lock.  This class is a more
//    general-purpose synchronization mechanism than SunOS 5.x RLocks.
//    For example, it implements "recursive RLock" semantics, where a
//    thread that owns the token can reacquire it without deadlocking.
//    In addition, threads that are blocked awaiting the token are
//    serviced in strict FIFO order as other threads release the token
//    (SunOS 5.x RLockes don't strictly enforce an acquisition
//    order).
//
// = Synchronization operations.
//    The interfaces for acquire, tryacquire, renew, release, etc. are
//    defined in ACE_Token_Proxy.  The semantics for ACE_Local_RLock
//    are that of a readers/writers lock.  Acquire for this class
//    implies a reader acquisition.  That is, multiple clients may
//    acquire a lock for read only.
{
public:
  // = Initialization and termination.

  ACE_Local_RLock (const char *token_name = 0,
		   int ignore_deadlock = 0,
		   int debug = 0);
  // <token_name> uniquely id's the token.
  // <ignore_deadlock> will allow deadlock to occur (useful for
  // testing).  <debug> prints a bunch of messages.

  void dump (void) const;
  // Dump the state of the class.

  virtual int type (void) const;
  // Returns ACE_RW_Token::RLOCK.

  virtual ACE_Token_Proxy *clone (void) const;
  // Return deep copy.

protected:
  virtual ACE_Tokens *create_token (const char *name);
  // Return a new ACE_Local_Mutex.
};

// *.
class ACE_Export ACE_Local_WLock : public ACE_Token_Proxy
// = TITLE
//    Class that acquires, renews, and releases a writer lock that
//    is local to the process.
//
// = DESCRIPTION 
//    This class implements the writer interface to canonical
//    readers/writer locks. Multiple readers can hold the lock
//    simultaneously when no writers have the lock.  Alternatively,
//    when a writer holds the lock, no other participants (readers or
//    writers) may hold the lock.  This class is a more
//    general-purpose synchronization mechanism than SunOS 5.x WLock.
//    For example, it implements "recursive WLock" semantics, where a
//    thread that owns the token can reacquire it without deadlocking.
//    In addition, threads that are blocked awaiting the token are
//    serviced in strict FIFO order as other threads release the token
//    (SunOS 5.x WLocks don't strictly enforce an acquisition order).
//
// = Synchronization operations.
//   The interfaces for acquire, tryacquire, renew, release,
//   etc. are defined in ACE_Token_Proxy.  The semantics for
//   ACE_Local_WLock are that of a readers/writers lock.  Acquire
//   for this class implies a writer acquisition.  That is, only one
//   client may hold the lock for writing.
{
public:
  // = Initialization and termination.

  ACE_Local_WLock (const char *token_name = 0,
		   int ignore_deadlock = 0,
		   int debug = 0);
  // <token_name> uniquely id's the token.
  // <ignore_deadlock> will allow deadlock to occur (useful for
  // testing).  <debug> prints a bunch of messages.

  void dump (void) const;
  // Dump the state of the class.

  virtual int type (void) const;
  // Returns ACE_RW_Token::WLOCK.

  virtual ACE_Token_Proxy *clone (void) const;
  // Return deep copy.

protected:
  ACE_Tokens *create_token (const char *name);
  // Return a new ACE_Local_Mutex.
};

#if defined (__ACE_INLINE__)
#include "ace/Local_Tokens.i"
#endif /* __ACE_INLINE__ */
#endif /* ACE_LOCAL_MUTEX_H */
