// -*- C++ -*-
//=============================================================================
/**
 *  @file   Safe_InputCDR.h
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef SAFE_INPUTCDR_H
#define SAFE_INPUTCDR_H
#include /**/ "ace/pre.h"

#include "orbsvcs/FtRtEvent/Utils/ftrtevent_export.h"
#include "tao/CDR.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FtRtEvent_Export Safe_InputCDR : public TAO_InputCDR
{
public:
  /**
   * Create an input stream from an arbitrary buffer.
   *
   * This constructor will work no matter the buffer is aligned properly
   * or not. If the buffer is not properly aligned, it will make a copy
   * of the buffer.
   */
  Safe_InputCDR(const char* buf, int size);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "orbsvcs/FtRtEvent/Utils/Safe_InputCDR.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif
