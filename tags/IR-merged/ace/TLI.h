/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    TLI.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_TLI_H
#define ACE_TLI_H

#include "ace/IPC_SAP.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Addr.h"

#if defined (ACE_HAS_TLI)

// There's not a universal device name for TLI devices.  If the platform
// needs something other than /dev/tcp, it needs to be set up in the config.h
// file as ACE_TLI_TCP_DEVICE.
#ifndef ACE_TLI_TCP_DEVICE
#define ACE_TLI_TCP_DEVICE "/dev/tcp"
#endif

// There's not a universal device name for XTI/ATM devices.  If the platform
// needs something other than /dev/xtisvc0, it needs to be set up in the
// config.h file as ACE_XTI_ATM_DEVICE. This may be FORE vendor specific and
// there may be no good default.
#ifndef ACE_XTI_ATM_DEVICE
#define ACE_XTI_ATM_DEVICE "/dev/xtisvc0"
#endif

class ACE_Export ACE_TLI : public ACE_IPC_SAP
{
  // = TITLE
  //     Defines the member functions for the base class of the
  //     ACE_TLI  abstraction.
public:
  // = Initialization and termination methods.
  ACE_HANDLE open (const char device[],
                   int oflag = O_RDWR,
                   struct t_info *info = 0);
  // Initialize a TLI endpoint.

  int close (void);
  // Close a TLI endpoint and release resources.

  int set_option (int level, int option, void *optval, int optlen);
  // Set underlying protocol options.

  int get_option (int level, int option, void *optval, int &optlen);
  // Get underlying protocol options.

  // = Calls to underlying TLI operations.
  int look (void) const;
  int rcvdis (struct t_discon * = 0) const;
  int snddis (struct t_call * = 0) const;
  int sndrel (void) const;
  int rcvrel (void) const;

  int get_local_addr (ACE_Addr &) const;
  // Return our local endpoint address.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Ensure we are an abstract class.
  ACE_TLI (void);
  // Default constructor.
  ~ACE_TLI (void);
  // Destructor.

  ACE_TLI (const char device[], int oflag = O_RDWR, struct t_info *info = 0);
  // Initialize a TLI endpoint.

private:
#if defined (ACE_HAS_SVR4_TLI)
  // Insane TLI option management.
  struct t_optmgmt so_opt_req;
  struct t_optmgmt so_opt_ret;
#endif /* ACE_HAS_SVR4_TLI */
};

#include "ace/TLI.i"

#endif /* ACE_HAS_TLI */
#endif /* ACE_TLI_H */
