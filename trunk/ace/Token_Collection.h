/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ACE
// 
// = FILENAME
//    ACE_Token_Collection.h
//
// = DESCRIPTION
//    The ACE_Token class offers methods for acquiring, renewing,
//    and releasing a synchronization token on a per-token basis.  The
//    ACE_Token_Collection offers an interface for performing
//    operations on groups of tokens as a whole, or on a single token
//    within the collection.
//
//    The atomic group operations are not yet implemented.
//
// = AUTHOR
//    Douglas C. Schmidt (schmidt@cs.wustl.edu) and
//    Tim Harrison (harrison@cs.wustl.edu)
// 
// ============================================================================

#if !defined (ACE_TOKEN_COLLECTION_H)
#define ACE_TOKEN_COLLECTION_H

#include "ace/Map_Manager.h"
#include "ace/Local_Tokens.h"
#include "ace/SString.h"

class ACE_Export ACE_Token_Collection : public ACE_Token_Proxy
  // = TITLE
  //   Allows atomic token group operations AND 
  //   provides a ACE_Token manager interface.
  //
  // = DESCRIPTION
  //   There are two types of operations offered by
  //   ACE_Token_Collection.  The first is atomic operations on
  //   collections of Token_Proxies.  In this respect, the
  //   ACE_Token_Collection can be thought of as a single token
  //   consisting of multiple Token_Proxies.  The second role of the
  //   ACE_Token_Collection is as a ACE_Token manager.
  //   ACE_Token_Collection allows individual operations on single
  //   members of a collection of Token_Proxies.  This provides a
  //   single access point for operations on multiple tokens.

  // = Bugs
  //   Although ACE_Token_Collection inherits from ACE_Token_Proxy, it
  //   can not be including in a collection.  This is because clone()
  //   returns zero for now.

{
public:

  ACE_Token_Collection (int debug = 0,
			const char *name = 0);
  // <debug> print out verbose debugging messages. <name> will give a
  // name to the collection.  Collections don't really need names, but
  // are sometimes useful for debugging.
  
// Collection Management operations

  int insert (ACE_Token_Proxy &token);

  // Insert a Token into the collection.  All ACE_Token type
  // operations performed on the collection will also be performed on
  // the new_proxy until it is removed.  Note that no operations
  // performed prior to the insertion will be performed.  Returns: 0
  // on success, -1 on failure with <errno> == problem.  If a token
  // proxy already exists in the collection with the same name, the
  // insertion will fail.  Also, <token> is copied.  Note that during
  // the copy, client_id's are *not* inherited.  The client ID of the
  // thread using the collection will be used.  Client ID's can be
  // changed explicity on each proxy using is_member.

  int extract (const char *token_name, ACE_Token_Proxy *&proxy);
  // removes the ACE_Token matching the given token_name from the
  // collection.  On success, extract returns 0. On failure
  // (token_name was not in the collection,) extract returns -1.  On
  // success, the state of the token found is copied into proxy.
  // The returned ACE_Token_Proxy* must be deleted by the user.

  ACE_Token_Proxy *is_member (const char *token_name);
  // returns the proxy if true.  0 otherwise.

  int is_member (const ACE_Token_Proxy &token);
  // Is the specified token in the collection?
  // 1, yes.
  // 0, no.

// = Collective operation semantics.

//   For acquire, renew, and release, there are two interfaces.  Once
//   interface allows an operation on a single token in the
//   collection.  The collective interfaces perform atomic operations
//   on the entire collection.  For instance, a collective acquire
//   will perform an acquire for each and every token in the
//   collection or the operation will fail.  Currently, these
//   operations are performed with no ordering heuristics.  That is,
//   the Collection steps through the tokens in the order they were
//   inserted.  For each one it performs the operation (acquire,
//   renew, or release).

  virtual int acquire (int notify = 0,
		       void (*sleep_hook)(void *) = 0,
		       ACE_Synch_Options &options =
		       ACE_Synch_Options::defaults);
  // Acquire "atomically" all resources in the collection.  This is
  // only successfull if all tokens in the collection could be
  // acquired.  options contains the blocking semantics, timeout
  // value, etc.  Returns: 0 on success, -1 on failure with <errno> ==
  // problem.  If and error or deadlock occurs for one of the tokens,
  // all the tokens will be released and the method will return -1.
  // Note that returning on detection of deadlock prevents livelock
  // between competing collections.  If a collection returns after
  // detecting deadlock, it is the application's responsibility to not
  // to blindly loop on the collection::acquire operation.  In other
  // words, once the collection reports deadlock, it is out of our
  // hands.

  virtual int acquire (const char *token_name,
		       int notify = 0,
		       void (*sleep_hook)(void *) = 0,
		       ACE_Synch_Options &options =
		       ACE_Synch_Options::defaults);
  // Acquire the token corresponding to <token_name>.  The other
  // parameters are passed to <token>::acquire.

  virtual int tryacquire (void (*sleep_hook)(void *) = 0);
  // Try to acquire all tokens in collection.

  virtual int tryacquire (const char *token_name,
			  void (*sleep_hook)(void *) = 0);
  // Try to acquire <token_name>.

  virtual int renew (int requeue_position = 0,
		     ACE_Synch_Options &options =
		     ACE_Synch_Options::defaults);
  // Renews "atomically" all resources in the collection.  This is
  // only successfull if all tokens in the collection could be
  // renewed.  options contains the blocking semantics, timeout
  // value, etc.  Returns: 0 on success, -1 on failure with <errno> ==
  // problem.


  virtual int renew (const char *token_name,
		     int requeue_position = 0,
		     ACE_Synch_Options &options =
		     ACE_Synch_Options::defaults);
  // Renew the token corresponding to <token_name>.  The other
  // parameters are passed to <token>::renew.

  virtual int release (ACE_Synch_Options &options =
		       ACE_Synch_Options::defaults);
  // Releases "atomically" all resources in the collection.  This is
  // only successfull if all tokens in the collection could be
  // released.  options contains the blocking semantics, timeout
  // value, etc.  Returns: 0 on success, -1 on failure with <errno> ==
  // problem.


  virtual int release (const char *token_name,
		       ACE_Synch_Options &options =
		       ACE_Synch_Options::defaults);
  // Release the token corresponding to <token_name>.  The other
  // parameters are passed to <token>::release.

  ~ACE_Token_Collection (void);

  void dump (void) const;
  // Dump the state of the class.

  virtual const char *name (void) const;
  // Return the name of the collection.  Not very functionally
  // important, but sometimes a useful debugging tool.

protected:

  typedef ACE_Token_Name TOKEN_NAME;

  typedef ACE_Map_Manager<TOKEN_NAME, ACE_Token_Proxy *, ACE_Null_Mutex>
    COLLECTION;
  // COLLECTION maintains a mapping from token names to ACE_Tokens*

  typedef ACE_Map_Iterator<TOKEN_NAME, ACE_Token_Proxy *, ACE_Null_Mutex>
    COLLECTION_ITERATOR;
  // Allows iterations through collection_

  typedef ACE_Map_Entry<TOKEN_NAME, ACE_Token_Proxy *>
    COLLECTION_ENTRY;
  // Allows iterations through collection_

  COLLECTION collection_;
  // COLLECTION maintains a mapping from token names to ACE_Tokens*.

  int debug_;
  // Whether to print out debug messages or not.

  char name_[ACE_MAXTOKENNAMELEN];
  // Name of the collection.

  // = I'm not sure what these mean, but they have to be defined since they're
  //   pure virtual in ACE_Token_Proxy.
  virtual ACE_Token_Proxy *clone (void) const;
  virtual ACE_Tokens *create_token (const char *name);
};

#if defined (__INLINE__)
#include "ace/Token_Collection.i"
#endif /* __INLINE__ */

#endif /* ACE_TOKEN_COLLECTION_H */
