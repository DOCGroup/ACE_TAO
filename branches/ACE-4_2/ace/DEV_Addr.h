/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    DEV_Addr.h
//
// = AUTHOR
//    Gerhard Lenzer and Douglas C. Schmidt
// 
// ============================================================================

#if !defined (ACE_DEV_ADDR_H)
#define ACE_DEV_ADDR_H

#include "ace/Addr.h"
#include "ace/ACE.h"

class ACE_Export ACE_DEV_Addr : public ACE_Addr
  // = TITLE
  //    Defines device address family address format. 
{
public:
  // = Initialization methods.
  ACE_DEV_Addr (void);
  // Default constructor.

  ACE_DEV_Addr (const ACE_DEV_Addr &sa);
  // Copy constructor.

  ACE_DEV_Addr (LPCTSTR devname);
  // Create a ACE_DEV_Addr from a device name.

  void set (LPCTSTR devname);
  // Create a ACE_Addr from a ACE_DEV pathname. 

  ACE_DEV_Addr &operator= (const ACE_DEV_Addr &);
  // Assignment operator.

  virtual void *get_addr (void) const;
  // Return a pointer to the address. 

  virtual int addr_to_string (LPTSTR addr, size_t) const;
  // Transform the current address into string format. 

  virtual int operator == (const ACE_Addr &SAP) const;
  // Compare two addresses for equality. 

  virtual int operator != (const ACE_Addr &SAP) const;
  // Compare two addresses for inequality. 

  LPCTSTR get_path_name (void) const; 
  // Return the path name used for the rendezvous point.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  TCHAR devname_[MAXNAMLEN + 1];
  // Name of the device.
};

#if defined (__ACE_INLINE__)
#include "ace/DEV_Addr.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_DEV_ADDR_H */
