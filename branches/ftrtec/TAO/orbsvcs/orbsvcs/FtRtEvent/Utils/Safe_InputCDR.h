// -*- C++ -*-
//=============================================================================
/**
 *  @file   Safe_InputCDR.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef SAFE_INPUTCDR_H
#define SAFE_INPUTCDR_H
#include /**/ "ace/pre.h"

#include "tao/CDR.h"

class Safe_InputCDR : public TAO_InputCDR
{
public:
  /**
   * Create an input stream from an arbitrary buffer.
   *
   * This contructor will work no matter the buffer is aligned properly
   * or not. If the buffer is not properly aligned, it will make a copy
   * of the buffer.
   */
  Safe_InputCDR(const char* buf, int size);
};


#include "Safe_InputCDR.inl"

#include /**/ "ace/post.h"
#endif
