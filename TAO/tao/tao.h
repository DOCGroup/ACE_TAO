/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    tao
// 
// = FILENAME
//   tao.h
//
// = AUTHOR
//    Chris Cleeland
//    
// 
// ============================================================================

#if !defined (TAO_H)
#define	TAO_H

#include "tao/tao_util.h"
#include "tao/objkeyC.h"

class TAO
{
  ACE_CLASS_IS_NAMESPACE (TAO);
  
  // From objkeyC
  typedef ObjectKey TAO_ObjectKey;
  typedef ObjectKey_var TAO_ObjectKey_var;
  typedef ObjectKey_out TAO_ObjectKey_out;

  // From tao_util
  typedef TAO_ORB_Manager ORB_Manager;
};

  
#endif /* TAO_H */
