// -*- C++ -*-

//=============================================================================
/**
 *  @file Argument_T.h
 *
 *  $Id$
 *
 *  @author Ossama Othman
 */
//=============================================================================


#ifndef TAO_ARGUMENT_T_H
#define TAO_ARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace TAO
{
  /**
   * @class Const_Argument_T
   *
   * @brief 
   *
   * 
   */
  template <typename T>
  class Const_Argument_T : public Argument
  {
  public:

    virtual T arg (void) const = 0;
    
  };

  /**
   * @class Mutable_Argument_T
   *
   * @brief 
   *
   * 
   */
  template <typename T>
  class Mutable_Argument_T : public Argument
  {
  public:

    virtual T arg (void) = 0;
    
  };

}

#include /**/ "ace/post.h"

#endif /* TAO_ARGUMENT_T_H */
