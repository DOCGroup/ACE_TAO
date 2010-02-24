// -*- C++ -*-

//=============================================================================
/**
 *  @file   default_environment.h
 *
 *  $Id$
 *
 * @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_DEFAULT_ENVIRONMENT_H
#define TAO_DEFAULT_ENVIRONMENT_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Environment;
}

/// Provide a simple function to access the TSS default
/// environment. This will be used in TAO wherever we have a default
/// environment.
/**
 * @note We tried with CORBA::default_environment() and others.
 */
TAO_Export CORBA::Environment& TAO_default_environment (void);

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_DEFAULT_ENVIRONMENT_H */
