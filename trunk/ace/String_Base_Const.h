/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    String_Base_Const.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#if !defined ACE_STRING_BASE_CONST_H
#define ACE_STRING_BASE_CONST_H

#include "ace/pre.h"
#include "ace/config-all.h"

/**
 * @class ACE_String_Base_Const
 *
 * @brief This class defines a constant for ACE_String_Base to
 * circumvent a bug in SunCC 6.0.
 */
class ACE_Export ACE_String_Base_Const
{
public:
  static int npos;
};

#include "ace/post.h"
#endif /* ACE_STRING_BASE_CONST_H */
