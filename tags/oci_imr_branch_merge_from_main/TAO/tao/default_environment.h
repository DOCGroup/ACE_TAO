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
#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CORBA
{
  class Environment;
}

/// Provide a simple function to access the TSS default
/// environment. This will be used in TAO wherever we have a default
/// environement.
/**
 * @note We tried with CORBA::default_environment() and others.
 */
TAO_Export CORBA::Environment& TAO_default_environment (void);

#include /**/ "ace/post.h"
#endif /*TAO_DEFAULT_ENVIRONMENT_H*/
