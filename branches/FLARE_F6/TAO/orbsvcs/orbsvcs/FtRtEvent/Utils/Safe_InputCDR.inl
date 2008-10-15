// -*- C++ -*-
//
// $Id$

#include "ace/OS_Memory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
Safe_InputCDR::Safe_InputCDR(const char* buf, int size)
: TAO_InputCDR((ACE_Message_Block*)0)
{
  if (ACE_ptr_align_binary(buf, ACE_CDR::MAX_ALIGNMENT) != buf) {
    ACE_CDR::grow(&start_,size);
    this->start_.copy(buf, size);
  }
  else {
    this->start_.init(buf,size);
    this->start_.wr_ptr(size);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
