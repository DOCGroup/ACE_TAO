// -*- C++ -*-

//=============================================================================
/**
 *  @file    PSDL_OctetSeq.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================


#ifndef TAO_PSDL_OCTETSEQ_H
#define TAO_PSDL_OCTETSEQ_H
#include "ace/pre.h"

#include "psdl_export.h"
#include "tao/OctetSeqC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PSDL_Datastore.h"

/**
 * @class TAO_PSDL_OctetSeq
 *
 * @brief This class facilitates implementation of Persistent
 * Service. Helps perform the bind and find to the hash_map and to
 * make the data persistent.
 **/

class ACE_Allocator;

class TAO_PSDL_Export TAO_PSDL_OctetSeq
{
public:

  /// Constructor.
  TAO_PSDL_OctetSeq (void);

  /// Constructor.
  TAO_PSDL_OctetSeq (ACE_Allocator *persistent_allocator);

  /// Copy constructor.
  TAO_PSDL_OctetSeq (const TAO_PSDL_OctetSeq & rhs);

  /// Destructor.
  ~TAO_PSDL_OctetSeq (void);

  /// Assignment operator.
  void operator= (const TAO_PSDL_OctetSeq & rhs);

  // Conversion operator (assignment)
  void operator= (const CORBA::OctetSeq & rhs);

  // Conversion operators (cast)
  // (caller owns storage of return values)
  operator CORBA::OctetSeq *() const;

  operator CORBA::OctetSeq *();

  // Persistent allocator.
  ACE_Allocator * allocator_;

  // Persistent buffer.
  CORBA::Octet * buffer_;

  // Length of the persistent buffer.
  CORBA::ULong length_;
};


#include "ace/post.h"
#endif /* TAO_PSDL_OCTETSEQ_H */
