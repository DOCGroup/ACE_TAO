// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Typecode_Constants.h
 *
 *  $Id$
 *
 *  Header file for the class that creates and destroy's the ORB's typecodes.
 *
 *  @author  Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_TYPECODE_CONSTANTS_H
#define TAO_TYPECODE_CONSTANTS_H

#include /**/ "ace/pre.h"

namespace TAO
{
  /**
   * @class TypeCode_Constants
   *
   * @brief This class is a namespace for TypeCode-related static data that
   * is owned by the ORB.
   */
  class TypeCode_Constants
  {
  public:
    /// Runtime initialization of all standard typecodes.
    /// Called from <CORBA::ORB_init>.
    static void init (void);

    /// Runtime finalization of all standard typecodes.
    static void fini (void);

  private:
    /// Flag that denotes that the TAO TypeCode constants have been
    /// initialized.
    static int initialized_;
  };
}

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_CONSTANTS_H */
