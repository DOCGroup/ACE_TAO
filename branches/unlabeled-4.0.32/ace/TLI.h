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

#if !defined (ACE_TLI_H)
#define ACE_TLI_H

#include "ace/IPC_SAP.h"
#include "ace/Addr.h"

// The following is necessary since many C++ compilers don't support
// typedef'd types inside of classes used as formal template
// arguments... ;-(.  Luckily, using the C++ preprocessor I can hide
// most of this nastiness!

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)
#define ACE_TLI_ACCEPTOR ACE_TLI_Acceptor
#define ACE_TLI_CONNECTOR ACE_TLI_Connector
#define ACE_TLI_STREAM ACE_TLI_Stream
#else /* TEMPLATES are broken (must be a cfront-based compiler...) */
#define ACE_TLI_ACCEPTOR ACE_TLI_Acceptor, ACE_INET_Addr
#define ACE_TLI_CONNECTOR ACE_TLI_Connector, ACE_INET_Addr
#define ACE_TLI_STREAM ACE_TLI_Stream, ACE_INET_Addr
#endif /* ACE_TEMPLATE_TYPEDEFS */

#if defined (ACE_HAS_TLI)
class ACE_Export ACE_TLI : public ACE_IPC_SAP
  // = TITLE
  //     Defines the member functions for the base class of the
  //     ACE_TLI  abstraction. 
{
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
