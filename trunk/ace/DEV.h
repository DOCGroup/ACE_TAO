/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    DEV.h
//
// = AUTHOR
//    Gerhard Lenzer
// 
// ============================================================================

#if !defined (ACE_DEV_H)
#define ACE_DEV_H

#include "ace/IO_SAP.h"
#include "ace/DEV_Addr.h"

// The following is necessary since many C++ compilers don't support
// typedef'd types inside of classes used as formal template
// arguments... ;-(.  Luckily, using the C++ preprocessor I can hide
// most of this nastiness!

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)
#define ACE_DEV_CONNECTOR ACE_DEV_Connector
#define ACE_DEV_STREAM ACE_DEV_Stream
#else /* TEMPLATES are broken (must be a cfront-based compiler...) */
#define ACE_DEV_CONNECTOR ACE_DEV_Connector, ACE_DEV_Addr
#define ACE_DEV_STREAM ACE_DEV_Stream, ACE_DEV_Addr
#endif /* ACE_TEMPLATE_TYPEDEFS */

class ACE_Export ACE_DEV : public ACE_IO_SAP
  // = TITLE
  //     Defines the member functions for the base class of the
  //     ACE_DEV abstraction. 
{
public:
  int close (void);
  // Close down the DEVICE

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  // = The following two methods are no-ops to keep the
  // <ACE_Connector> happy.
  int get_local_addr (ACE_DEV_Addr &) const;
  // Return the local endpoint address.

  int get_remote_addr (ACE_DEV_Addr &) const;
  // Return the address of the remotely connected peer (if there is
  // one).

  int disable (int signum) const ;
  // Disable signal <signum>
  // This is here to prevent Win32 from
  // disabling SPIPE using socket calls

protected:
  ACE_DEV (void);
  // Ensure that this class is an abstract base class 

  ACE_DEV_Addr addr_;
  // Address of device we are connected to.
};

#include "ace/DEV.i"

#endif /* ACE_DEV_H */
