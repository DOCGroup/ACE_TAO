/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE
//
// = FILENAME
//    CLASSIX_Addr.h
//
// = AUTHOR
//    Nokia Telecommunications
//
// ============================================================================

#ifndef ACE_CLASSIX_ADDR_H
#include "ace/pre.h"
#define ACE_CLASSIX_ADDR_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Addr.h"

#include <ipc/chIpc.h>
#include <ace/CLASSIX/CLASSIX_OS.h>

class ACE_Export ACE_CLASSIX_Addr : public ACE_Addr
{
  // = TITLE
  //    Defines the ClassiX IPC address format.
  //
  // = DESCRIPTION
  //   This class defines basic interfaces for "ACE-like" address for
  //   Chorus port.
public:
  enum
  {
    ACE_CLASSIX_ADDR_UNKNOWN = ACE_INVALID_HANDLE, /* K_NONEPORT */
    ACE_CLASSIX_ADDR_DEFAULT = K_DEFAULTPORT,
    AF_CLASSIX               = AF_MAX + 1
  };

  enum Addr_Type
  {
    PORT,                       // use Peer_Port
    GROUP,                      // use Peer_Group
    STAMP,                      // use peer_Stamp
    DYNAMIC,                    // use Peer_Group
    UNDEFINED
  };


  /* -----------------------------------------------------*/
  // = INITIALIZATION
  ACE_CLASSIX_Addr(int /* size of the underlying address structure*/);
  virtual ~ACE_CLASSIX_Addr (void);


  /* -----------------------------------------------------*/
  // = Direct initialization methods (useful after the object has been
  // constructed).
  // Returns 0 on success, -1 otherwise.
  //

  /* -----------------------------------------------------*/
  // = ACCESS
  //
  virtual const KnUniqueId& get_id (void) const;
  // Returns a reference to the unique identifier

  virtual ACE_HANDLE get_port_number(void) const;
  virtual ACE_HANDLE get_handle(void) const;
  // Returns the local port no( can be used as an ACE_HANDLE)

  virtual int is_configured(void) const;
  // Returns 1, if address information is proper; Returns 0 otherwise
  virtual ACE_CLASSIX_Addr::Addr_Type is_type(void) const;
  // returns the type of the address

  // = HELPER
  virtual void dump(void) const;

  ACE_ALLOC_HOOK_DECLARE;
  // Declares the dynamic allocation hooks.

private:
};

#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Addr.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_CLASSIX_ADDR_H */
