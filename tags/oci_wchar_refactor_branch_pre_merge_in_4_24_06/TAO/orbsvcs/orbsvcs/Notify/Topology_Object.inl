// $Id$
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
  Topology_Parent::child_change (ACE_ENV_SINGLE_ARG_DECL)
  {
    this->children_changed_ = true;
    return send_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

} // namespace TAO_Notify

