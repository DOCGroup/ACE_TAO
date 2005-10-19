/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    Codeset.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#ifndef TAO_CODESET_H
#define TAO_CODESET_H

#include /**/ "ace/pre.h"

#include "codeset_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_Codeset_Export TAO_Codeset_Initializer
{
public:
  /// Used to force the initialization of the ORB code.
  static int init (void);
};

static int
TAO_Requires_Codeset_Initializer = TAO_Codeset_Initializer::init ();

#include /**/ "ace/post.h"

#endif /* TAO_CODESET_H */
