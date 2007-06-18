// -*- C++ -*-

//==========================================================================
/**
 *  @file    SOCK_SCTP_SEQPACK.h
 *
 *  $Id$
 *
 *  @author Andrew J. Stafford <ajstafford@gmail.com>
 */
//==========================================================================

#ifndef ACE_SOCK_SCTP_SEQPACK_H
#define ACE_SOCK_SCTP_SEQPACK_H

#include /**/ "ace/pre.h"

#include "ace/SOCK.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ACE.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_SOCK_SCTP_SEQPACK
 *
 * @brief Defines the methods for the ACE one-to-many style SCTP socket. 
 *
 */
class ACE_Export ACE_SOCK_SCTP_SEQPACK : public ACE_SOCK
{
  
public:

  /// Constructor.
  ACE_SOCK_SCTP_SEQPACK (void);

  /// Destructor.
  ~ACE_SOCK_SCTP_SEQPACK (void);
};

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* ACE_SOCK_SCTP_SEQPACK_H */
