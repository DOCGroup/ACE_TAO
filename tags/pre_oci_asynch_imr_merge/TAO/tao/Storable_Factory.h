// -*- C++ -*-

//=============================================================================
/**
 * @file  Storable_Factory.h
 *
 * $Id$
 *
 * @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#ifndef TAO_STORABLE_FACTORY_H
#define TAO_STORABLE_FACTORY_H

#include /**/ "ace/pre.h"
#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Storable_Base.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  class TAO_Export Storable_Factory
  {
  public:
    Storable_Factory();

    virtual ~Storable_Factory();

    // Factory Methods

    virtual Storable_Base *create_stream(const ACE_CString & file,
                                         const char * mode,
                                         bool use_backup =
                                         Storable_Base::use_backup_default) = 0;
  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_STORABLE_FACTORY_H */
