// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Traits_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_TRAITS_T_H
#define TAO_TRAITS_T_H

#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   *
   * @brief Template class specialized for IDL arg/return types.
   *
   */
  template<typename S>
  class Arg_Traits
  {
  };
};

#include "ace/post.h"

#endif /* TAO_TRAITS_T_H */
