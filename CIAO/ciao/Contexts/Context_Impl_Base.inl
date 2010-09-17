// $Id$

namespace CIAO
{
  // CIAO-specific.

  ACE_INLINE CIAO::Container_ptr
  Context_Impl_Base::_ciao_the_Container (void) const
  {
    return this->container_;
  }
}
