/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    SPIPE.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SPIPE_H)
#define ACE_SPIPE_H

#include "ace/IPC_SAP.h"
#include "ace/SPIPE_Addr.h"

// The following is necessary since many C++ compilers don't support
// typedef'd types inside of classes used as formal template
// arguments... ;-(.  Luckily, using the C++ preprocessor I can hide
// most of this nastiness!

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)
#define ACE_SPIPE_ACCEPTOR ACE_SPIPE_Acceptor
#define ACE_SPIPE_CONNECTOR ACE_SPIPE_Connector
#define ACE_SPIPE_STREAM ACE_SPIPE_Stream
#define ACE_UPIPE_ACCEPTOR ACE_UPIPE_Acceptor
#define ACE_UPIPE_CONNECTOR ACE_UPIPE_Connector
#define ACE_UPIPE_STREAM ACE_UPIPE_Stream
#else /* TEMPLATES are broken (must be a cfront-based compiler...) */
#define ACE_SPIPE_ACCEPTOR ACE_SPIPE_Acceptor, ACE_SPIPE_Addr
#define ACE_SPIPE_CONNECTOR ACE_SPIPE_Connector, ACE_SPIPE_Addr
#define ACE_SPIPE_STREAM ACE_SPIPE_Stream, ACE_SPIPE_Addr
#define ACE_UPIPE_ACCEPTOR ACE_UPIPE_Acceptor, ACE_SPIPE_Addr
#define ACE_UPIPE_CONNECTOR ACE_UPIPE_Connector, ACE_SPIPE_Addr
#define ACE_UPIPE_STREAM ACE_UPIPE_Stream, ACE_SPIPE_Addr
#endif /* ACE_TEMPLATE_TYPEDEFS */

class ACE_Export ACE_SPIPE : public ACE_IPC_SAP
  // = TITLE
  //     Defines the member functions for the base class of the
  //     ACE_SPIPE abstraction. 
{
public:
  int close (void);
  // Close down the STREAM pipe without removing the rendezvous point.

  int remove (void);
  // Close down the STREAM pipe and remove the rendezvous point from
  // the file system.

  int get_local_addr (ACE_SPIPE_Addr &) const;
  // Return the local address of this endpoint.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_SPIPE (void);
  // Ensure that this class is an abstract base class 

  ACE_SPIPE_Addr local_addr_;
  // Our local address.
};

#include "ace/SPIPE.i"
#endif /* ACE_SPIPE_H */
