// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    ATM_Params.h
//
// = AUTHOR
//    Joe Hoffert <joeh@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_ATM_PARAMS_H
#define ACE_ATM_PARAMS_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_ATM)

#if defined (ACE_HAS_FORE_ATM_XTI)
#include "ace/TLI.h"
#define ATM_PROTOCOL_DEFAULT 0
typedef struct t_info Param_Info;
typedef struct netbuf Param_Udata;
#elif defined (ACE_HAS_FORE_ATM_WS2)
#include "ace/SOCK.h"
#define ATM_PROTOCOL_DEFAULT ATMPROTO_AAL5
#define ACE_XTI_ATM_DEVICE ""
typedef int Param_Info;
typedef int Param_Udata;
#else
#define ACE_XTI_ATM_DEVICE ""
typedef int Param_Info;
typedef int Param_Udata;
#endif /* ACE_HAS_FORE_ATM_XTI && ACE_HAS_FORE_ATM_WS2 */

class ACE_Export ACE_ATM_Params
{
  // = TITLE
  //   Wrapper class that simplifies the information passed to the ATM
  //   enabled <ACE_ATM_Connector> class.
public:
  ACE_ATM_Params (int rw_flag = 1,
                  const char device[] = ACE_XTI_ATM_DEVICE,
                  Param_Info *info = 0,
                  Param_Udata *udata = 0,
                  int oflag = O_RDWR,
                  int protocol_family = AF_ATM,
                  int protocol = ATM_PROTOCOL_DEFAULT,
                  int type = SOCK_RAW,
                  ACE_Protocol_Info *protocol_info = 0,
                  ACE_SOCK_GROUP g = 0,
                  u_long flags 
                    = ACE_FLAG_MULTIPOINT_C_ROOT 
                    | ACE_FLAG_MULTIPOINT_D_ROOT, // connector by default
                  int reuse_addr = 0);
  // Initialize the data members.  This class combines options from
  // ACE_SOCK_Connector (<protocol_family>, <protocol>, <type>, 
  // <protocol_info>, <group>, and <flags>) and
  // ACE_TLI_Connector (<device>, <info>, <rw_flag>, <oflag>, and <udata>)
  // so that either mechanism can be used transparently for ATM.

  ~ACE_ATM_Params ();

  int get_protocol_family (void) const;
  void set_protocol_family (int);
  // Get/set protocol family.

  int get_protocol (void) const;
  void set_protocol (int);
  // Get/set protocol.

  int get_type (void) const;
  void set_type (int);
  // Get/set type.

  ACE_Protocol_Info *get_protocol_info( void );
  void set_protocol_info( ACE_Protocol_Info *);
  // Get/set protocol info.

  ACE_SOCK_GROUP get_sock_group( void );
  void set_sock_group( ACE_SOCK_GROUP );
  // Get/set socket group.

  u_long get_flags( void );
  void set_flags( u_long );
  // Get/set socket flags.

  int get_reuse_addr (void) const;
  void set_reuse_addr (int);
  // Get/set reuse_addr flag.

  const char* get_device (void) const;
  // Get device.

  Param_Info* get_info (void) const;
  void set_info (Param_Info *);
  // Get/set info.

  int get_rw_flag (void) const;
  void set_rw_flag (int);
  // Get/set r/w flag.

  Param_Udata* get_user_data (void) const;
  void set_user_data (Param_Udata*);
  // Get/set user data.

  int get_oflag (void) const;
  void set_oflag (int);
  // /set open flag.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int protocol_family_;
  // Protocol family for sockets connections.

  int protocol_;
  // Protocol for sockets connections.

  int type_;
  // Type for opening sockets.

  ACE_Protocol_Info *protocol_info_;
  // Information about the protocol.

  ACE_SOCK_GROUP group_;
  // Socket group used (for sockets only).

  u_long flags_;
  // Flags for sockets (for sockets only).

  int reuse_addr_;
  // Flag for reusing address for opening sockets.

  const char *device_;
  // Device name for XTI/ATM connections.

  Param_Info *info_;
  // Info for XTI/ATM connections.

  int rw_flag_;
  // R/W flag for XTI/ATM connections.

  Param_Udata *udata_;
  // User data for XTI/ATM connections.

  int oflag_;
  // Open flag for XTI/ATM connections.
};

#if defined (__ACE_INLINE__)
#include "ace/ATM_Params.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_ATM */
#include "ace/post.h"
#endif /* ACE_ATM_PARAMS_H */

