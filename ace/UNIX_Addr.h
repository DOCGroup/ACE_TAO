/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    UNIX_Addr.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_UNIX_ADDR_H)
#define ACE_UNIX_ADDR_H
#include "ace/Addr.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

class ACE_Export ACE_UNIX_Addr : public ACE_Addr
  // = TITLE
  //    Defines the ``UNIX domain address family'' address format. 
{
public:
  // = Initialization methods.
  ACE_UNIX_Addr (void);
  // Default constructor.

  ACE_UNIX_Addr (const ACE_UNIX_Addr &sa);
  // Copy constructor.

  ACE_UNIX_Addr (const char rendezvous_point[]);
  // Creates an ACE_UNIX_Addr from a string.

  ACE_UNIX_Addr (const sockaddr_un *, int len);
  // Creates an ACE_INET_Addr from a sockaddr_un structure.

  void set (const ACE_UNIX_Addr &sa);
  // Creates an ACE_UNIX_Addr from another <ACE_UNIX_Addr>.

  void set (const char rendezvous_point[]);
  // Creates an ACE_UNIX_Addr from a string.

  void set (const sockaddr_un *, int len);
  // Creates an ACE_INET_Addr from a sockaddr_un structure.

  virtual void *get_addr (void) const;
  // Return a pointer to the underlying network address.

  virtual void set_addr (void *addr, int len);
  // Set a pointer to the underlying network address.

  virtual int addr_to_string (char addr[], size_t) const;
  // Transform the current address into string format.

  virtual int string_to_addr (const char addr[]);
  // Transform the string into the current addressing format.

  virtual int  operator == (const ACE_Addr &SAP) const;
  // Compare two addresses for equality.

  virtual int  operator != (const ACE_Addr &SAP) const;
  // Compare two addresses for inequality.

  const char *get_path_name (void) const; 
  // Return the path name of the underlying rendezvous point.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  sockaddr_un unix_addr_;
  // Underlying socket address.
};

#if defined (__ACE_INLINE__)
#include "ace/UNIX_Addr.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
#endif /* ACE_UNIX_ADDR_H */
