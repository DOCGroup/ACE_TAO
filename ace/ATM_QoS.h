/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    ATM_QoS.h
//
// = AUTHOR
//    Joe Hoffert
//
// ============================================================================

#ifndef ACE_ATM_QoS_H
#include "ace/pre.h"
#define ACE_ATM_QoS_H

#include "ace/ACE.h"

#if !defined(ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_ATM)

#if defined (ACE_HAS_FORE_ATM_WS2)
// just map to WS2 GQOS struct
typedef ACE_QoS ATM_QoS;
#elif defined (ACE_HAS_FORE_ATM_XTI)
typedef struct netbuf ATM_QoS;
#else
typedef int ATM_QoS;
#endif /* ACE_HAS_FORE_ATM_WS2 */

class ACE_Export ACE_ATM_QoS
{
  // = TITLE
  //	Define the QoS parameters for ATM
  //
  // = DESCRIPTION
  //     This class wraps up QoS parameters for both ATM/XTI and
  //     ATM/WinSock2 to make the mechanism for the ATM protocol
  //     transparent.
public:
  // Constants used for ATM options
  static const long LINE_RATE;
  static const int OPT_FLAGS_CPID;
  static const int OPT_FLAGS_PMP;
  static const int DEFAULT_SELECTOR;

  // = Initializattion and termination methods.
  ACE_ATM_QoS ();
  // Default constructor.

  ACE_ATM_QoS( int );
  // Constructor with a CBR rate.

  ~ACE_ATM_QoS ();

  void set_rate (ACE_HANDLE,
                 int,
                 int);
  // Set the rate.

  void set_cbr_rate (int);
  // Set CBR rate in cells per second.

  ATM_QoS get_qos (void);
  // Get ATM_QoS struct.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  char* construct_options(ACE_HANDLE,
                          int,
                          int,
                          long*);
  // Construct QoS options.

private:
  ATM_QoS qos_;
};

#if defined (__ACE_INLINE__)
#include "ace/ATM_QoS.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_ATM */
#include "ace/post.h"
#endif /* ACE_ATM_QoS_H */

