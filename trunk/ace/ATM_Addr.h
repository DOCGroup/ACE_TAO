/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    ATM_Addr.h
//
// = AUTHOR
//    Joe Hoffert <joeh@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_ATM_ADDR_H
#define ACE_ATM_ADDR_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Addr.h"

#if !defined (ACE_HAS_FORE_ATM_XTI)
typedef int ATMSAPAddress;
#endif /* ACE_HAS_FORE_ATM_XTI */

class ACE_Export ACE_ATM_Addr : public ACE_Addr
{
  // = TITLE
  //    Defines the ATM domain address family address format.
public:
  // Constants used for ATM options
  static const long LINE_RATE;
  static const int OPT_FLAGS_CPID;
  static const int OPT_FLAGS_PMP;
  static const int DEFAULT_SELECTOR;

  // = Initialization methods.
  ACE_ATM_Addr (unsigned char selector = DEFAULT_SELECTOR);
  // Default constructor.

  ACE_ATM_Addr (const ACE_ATM_Addr &,
                unsigned char selector = DEFAULT_SELECTOR);
  // Copy constructor.

  ACE_ATM_Addr (const ATMSAPAddress *,
                unsigned char selector = DEFAULT_SELECTOR);
  // Creates an <ACE_ATM_Addr> from an ATMSAPAddress structure. This
  // is vendor specific (FORE systems). May need to change when other
  // vendors are supported.

  ACE_ATM_Addr (const ASYS_TCHAR sap[],
                unsigned char selector = DEFAULT_SELECTOR);
  // Initializes an <ACE_ATM_Addr> from the <sap> which can be
  // "atm-address" (e.g.,
  // "47.0005.80.ffe100.0000.f20f.2200.0020480694f9.00") or "hostname"
  // (e.g., "frisbee.cs.wustl.edu").

  ~ACE_ATM_Addr (void);
  // Default dtor.

  // = Initialization methods (useful after object construction).
  void init (unsigned char selector = DEFAULT_SELECTOR);
  // Default initialization for non-address values (e.g.,
  // t_atm_sap_addr.SVE_tag_addr, t_atm_sap_addr.SVE_tag_selector)

  int set (const ACE_ATM_Addr &,
           unsigned char selector = DEFAULT_SELECTOR);
  // Initializes from another <ACE_ATM_Addr>.

  int set (const ATMSAPAddress *,
           unsigned char selector = DEFAULT_SELECTOR);
  // Initializes an <ACE_ATM_Addr> from an ATMSAPAddress
  // structure. This is vendor specific (FORE systems). May need to
  // change when other vendors are supported.

  int set (const ASYS_TCHAR sap[],
           unsigned char selector = DEFAULT_SELECTOR);
  // Initializes an <ACE_ATM_Addr> from the <sap> which can be
  // "atm-address" (e.g.,
  // "47.0005.80.ffe100.0000.f20f.2200.0020480694f9.00") or "hostname"
  // (e.g., "frisbee.cs.wustl.edu").

  virtual int string_to_addr (const ASYS_TCHAR sap[]);
  // Initializes an <ACE_ATM_Addr> from the <sap> which can be
  // "atm-address" (e.g.,
  // "47.0005.80.ffe100.0000.f20f.2200.0020480694f9.00") or "hostname"
  // (e.g., "frisbee.cs.wustl.edu").

  virtual int addr_to_string (ASYS_TCHAR addr[], 
                              size_t addrlen) const;
  // Return the character representation of the ATM address (e.g.,
  // "47.0005.80.ffe100.0000.f20f.2200.0020480694f9.00") storing it in
  // the <addr> (which is assumed to be <addrlen> bytes long). This
  // version is reentrant.  Returns -1 if the <addrlen> of the <addr>
  // is too small, else 0.

  const ASYS_TCHAR *addr_to_string (void) const;
  // Return the character representation of the ATM address (e.g.,
  // "47.0005.80.ffe100.0000.f20f.2200.0020480694f9.00"). Returns -1
  // if the <size> of the <buffer> is too small, else 0.(This version
  // is non-reentrant since it returns a pointer to a static data
  // area.)

  virtual void *get_addr (void) const;
  // Return a pointer to the underlying network address.

  virtual void set_addr (void *, int);
  // Set a pointer to the address.

  unsigned char get_selector (void) const;
  // Return the selector for network address.

  void set_selector (unsigned char);
  // Set the selector for the network address.

  int operator == (const ACE_ATM_Addr &SAP) const;
  // Compare two addresses for equality.  The addresses are considered
  // equal if they contain the same ATM address.  Q: Is there any
  // other check for equality needed for ATM?

  int operator != (const ACE_ATM_Addr &SAP) const;
  // Compare two addresses for inequality.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  static char *construct_options (ACE_HANDLE fd,
                                  int qos_kb,
                                  int flags,
                                  long *optsize);
  // Construct options for ATM connections

protected:
  static int get_local_address (ACE_HANDLE handle,
                                u_char addr[]);
  // Get the local ATM address.

private:
  ATMSAPAddress atm_addr_;
  // Underlying representation - this may be very
  // vendor-implementation specific. Other vendors (besides FORE) may
  // name and define this structure differently. We can work around
  // that problem when we run into other vendors supporting XTI on top
  // of ATM. Is this class specific to XTI?  Not sure.
};

#if defined (__ACE_INLINE__)
#include "ace/ATM_Addr.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_ATM_ADDR_H */
