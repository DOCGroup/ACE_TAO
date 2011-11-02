// -*- C++ -*-

//=============================================================================
/**
 *  @file   Codeset_Descriptor_Base.h
 *
 *  $Id$
 *
 *  @author Phil Mesnier
 */
//=============================================================================

#ifndef TAO_CODESET_DESCRIPTOR_BASE_H
#define TAO_CODESET_DESCRIPTOR_BASE_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * TAO_Codeset_Descriptor_Base describes the necessary interface for
 * Objects within libTAO to be able to supply information to the codeset
 * subsystem, as required.
 *
 * This class also serves as a default implementation when codeset support
 * is not linked in.
 */
class TAO_Export TAO_Codeset_Descriptor_Base
{
public:
  virtual ~TAO_Codeset_Descriptor_Base (void);

  virtual void ncs (const ACE_TCHAR *name) = 0;
  virtual void add_translator (const ACE_TCHAR *name) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_CODESET_DESCRIPTOR_BASE_H */
