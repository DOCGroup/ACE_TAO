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
                                         const ACE_TCHAR * mode) = 0;
  };

}

#endif /* TAO_STORABLE_FACTORY_H */
