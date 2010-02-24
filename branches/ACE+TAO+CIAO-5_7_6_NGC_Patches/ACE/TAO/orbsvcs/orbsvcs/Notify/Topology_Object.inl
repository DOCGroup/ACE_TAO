// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  ACE_INLINE
  bool
  Topology_Object::is_changed () const
  {
    return this->self_changed_ | this->children_changed_;
  }

  ACE_INLINE
  bool
  Topology_Parent::child_change (void)
  {
    this->children_changed_ = true;
    return send_change ();
  }

} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL
