// $Id$

namespace CIAO
{
  // CIAO-specific.

  ACE_INLINE CIAO::Session_Container *
  Context_Impl_Base::_ciao_the_Container (void) const
  {
    return this->container_;
  }

  ACE_INLINE const char *
  Context_Impl_Base::_ciao_instance_id (void) const
  {
    return this->ciao_instance_id_.in ();
  }

  ACE_INLINE void
  Context_Impl_Base::_ciao_instance_id (const char *instance_id)
  {
    this->ciao_instance_id_ = instance_id;
  }
}
