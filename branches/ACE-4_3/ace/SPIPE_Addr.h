/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    SPIPE_Addr.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SPIPE_ADDR_H)
#define ACE_SPIPE_ADDR_H
#include "ace/Addr.h"
#include "ace/ACE.h"

class ACE_Export ACE_SPIPE_Addr : public ACE_Addr
  // = TITLE
  //    Defines the SVR4 STREAM pipe address family address format. 
{
public:
  // = Initialization methods.
  ACE_SPIPE_Addr (void);
  // Default constructor.

  ACE_SPIPE_Addr (const ACE_SPIPE_Addr &sa);
  // Copy constructor.

  ACE_SPIPE_Addr (LPCTSTR rendezvous_point, gid_t = 0, uid_t = 0);
  // Create a ACE_SPIPE_Addr from a rendezvous point in the file system.

  int set (LPCTSTR rendezvous_point, gid_t = 0, uid_t = 0);
  // Create a ACE_SPIPE_Addr from a rendezvous point in the file system.

  virtual void *get_addr (void) const;
  // Return a pointer to the address.

  virtual void set_addr (void *addr, int len);
  // Set a pointer to the underlying network address.

#if defined (UNICODE)
  virtual int addr_to_string (wchar_t *addr, size_t) const;
  // Transform the current address into string format. 

  virtual int string_to_addr (const wchar_t *addr);
  // Transform the string into the current addressing format.
#endif /* UNICODE */

  virtual int addr_to_string (char *addr, size_t) const;
  // Transform the current address into string format. 

  virtual int string_to_addr (const char *addr);
  // Transform the string into the current addressing format.

  // = Equality/inequality tests
  virtual int operator == (const ACE_Addr &SAP) const;
  // Check for equality.

  virtual int operator != (const ACE_Addr &SAP) const;
  // Check for inequality

  // = SPIPE-specific address operations
  LPCTSTR get_path_name (void) const; 
  // Pathname of rendezvous point in file system.

  uid_t user_id (void) const;
  // Get user id.
  void user_id (uid_t uid);
  // Set user id.

  void group_id (gid_t gid);
  // Set group ids.
  gid_t group_id (void) const;
  // Get group ids.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  // = This struct contains security attributes.
  struct 
  {
    gid_t gid_;
    // Group id.

    uid_t uid_;
    // User id.

    TCHAR rendezvous_[MAXNAMLEN + 1];
    // Pathname in the file system.

  } SPIPE_addr_;
  // Contents of an SPIPE address.
};

#if defined (__ACE_INLINE__)
#include "ace/SPIPE_Addr.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SPIPE_ADDR_H */
