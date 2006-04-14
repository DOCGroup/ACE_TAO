// -*- C++ -*-
//
// $Id$

namespace TAO
{
  ACE_INLINE
  NamedValue_Argument::NamedValue_Argument (CORBA::NamedValue_ptr x)
    : x_ (x)
      , byte_order_ (0)
  {}

  ACE_INLINE int
  NamedValue_Argument::byte_order (void) const
  {
    return this->byte_order_;
  }

  // ===================================================================

  ACE_INLINE
  NVList_Argument::NVList_Argument (CORBA::NVList_ptr x,
                                    bool lazy_eval)
    : x_ (x)
      , lazy_evaluation_ (lazy_eval)
  {}
}
