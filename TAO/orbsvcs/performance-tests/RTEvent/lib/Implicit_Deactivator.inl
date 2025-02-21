/**
 * @file Implicit_Deactivator.inl
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE void
Implicit_Deactivator::release ()
{
  this->id_ = 0;
}
