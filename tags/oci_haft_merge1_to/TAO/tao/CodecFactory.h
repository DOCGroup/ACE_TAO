// -*- C++ -*-

//=============================================================================
/**
 * @file CodecFactory.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_CODEC_FACTORY_H
#define TAO_CODEC_FACTORY_H

#include "ace/pre.h"

#include "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "IOPC.h"
#include "LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


/**
 * @class TAO_CodecFactory
 *
 * @brief Implementation of the IOP::CodecFactory interface.
 *
 * This class can be used to create Codec (coder/decoder) of a given
 * type, such as a CDR encapsulation Codec.
 */
class TAO_Export TAO_CodecFactory
  : public virtual IOP::CodecFactory,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor
  TAO_CodecFactory (TAO_ORB_Core * orb_core);

  /// Create a Coder/Decoder for the given type of encoding.
  virtual IOP::Codec_ptr create_codec (const IOP::Encoding & enc
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     IOP::CodecFactory::UnknownEncoding));

private:

  /// Prevent copying through the copy constructor and the assignment
  /// operator.
  ACE_UNIMPLEMENTED_FUNC (
    TAO_CodecFactory (const TAO_CodecFactory &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_CodecFactory &))

private:

  /// Pointer to the ORB Core.
  /**
   * Some Codec implementations may need access to the ORB Core with
   * which they are associated.
   */
  TAO_ORB_Core * orb_core_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif  /* TAO_CODEC_FACTORY_H */
