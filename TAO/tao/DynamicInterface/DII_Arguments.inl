// -*- C++ -*-
//
// $Id$

namespace TAO
{
  ACE_INLINE
  NamedValue_Argument::NamedValue_Argument (CORBA::NamedValue_ptr x)
    : x_ (x)
  {}

  // ===================================================================

  ACE_INLINE
  NVList_Argument::NVList_Argument (CORBA::NVList_ptr x)
    : x_ (x)
  {}
}

