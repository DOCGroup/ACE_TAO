/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    FILE_Addr.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_FILE_ADDR_H)
#define ACE_FILE_ADDR_H

#include "ace/Addr.h"
#include "ace/ACE.h"

class ACE_Export ACE_FILE_Addr : public ACE_Addr
  // = TITLE
  //    Defines the FILE address family address format. 
{
public:
  // = Initialization methods.
  ACE_FILE_Addr (void);
  // Default constructor.

  ACE_FILE_Addr (const ACE_FILE_Addr &sa);
  // Copy constructor.

  ACE_FILE_Addr (LPCTSTR filename);
  // Create a ACE_FILE_Addr from a pathname. 

  void set (LPCTSTR filename);
  // Create a ACE_FILE_Addr from a pathname. 

  ACE_FILE_Addr &operator= (const ACE_FILE_Addr &);
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
  TCHAR filename_[MAXNAMLEN + 1];
  // Name of the file.
};

#if defined (__ACE_INLINE__)
#include "ace/FILE_Addr.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_FILE_ADDR_H */
