// $Id$

#include "tao/ORB_Table.h"
#include "tao/ORB_Core.h"
#include "tao/TAO_Singleton.h"

#if !defined (__ACE_INLINE__)
# include "tao/ORB_Table.inl"
#endif /* ! __ACE_INLINE__ */

#include "ace/SString.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::ORB_Table::ORB_Table (void)
  : lock_ (),
    first_orb_not_default_ (false),
    table_ (TAO_DEFAULT_ORB_TABLE_SIZE),
    first_orb_ (0)
{
}

int
TAO::ORB_Table::bind (char const * orb_id,
                      TAO_ORB_Core * orb_core)
{
  // Make sure that the supplied ORB core pointer is valid,
  // i.e. non-zero.
  if (orb_id == 0 || orb_core == 0)
    {
      errno = EINVAL;
      return -1;
    };

  value_type const value =
    std::make_pair (key_type (orb_id), data_type (orb_core));

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    -1);

  std::pair<iterator, bool> result = this->table_.insert (value);

  if (result.second)
    {
      // This is not the first ORB, but if the current default ORB
      // decided not to be the default and there is more than one ORB
      // then set this ORB to be the default.
      if (this->first_orb_ != 0
          && this->first_orb_not_default_)
        {
          this->first_orb_ = orb_core;
          this->first_orb_not_default_ = false;
        }

      // Set the "first_orb_" member for the first given ORB Core
      // that was successfully added to the ORB table.
      if (this->first_orb_ == 0)
        {
          this->first_orb_ = orb_core;
        }
    }

  return (result.second ? 0 : 1);
}

TAO_ORB_Core *
TAO::ORB_Table::find (char const * orb_id)
{
  TAO_ORB_Core * orb_core = 0;

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    0);

  iterator const i = this->table_.find (Table::key_type (orb_id));

  // Maintain ownership of the ORB_Core.
  if (i != this->end ())
    {
      orb_core = (*i).second.core ();
      (void) orb_core->_incr_refcnt ();
    }

  return orb_core;
}

int
TAO::ORB_Table::unbind (const char *orb_id)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    -1);

  iterator const result = this->table_.find (key_type (orb_id));

  if (result != this->end ())
    {
      TAO::ORB_Core_Ref_Counter oc ((*result).second);

      this->table_.erase (result);

      if (oc.core () == this->first_orb_)
        {
          if (!this->table_.empty ())
            {
              this->first_orb_ = (*this->begin ()).second.core ();
            }
          else
            {
              this->first_orb_ = 0;
            }
        }
    }

  return 0;
}

void
TAO::ORB_Table::set_default (char const * orb_id)
{
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);

  iterator const i = this->table_.find (key_type (orb_id));

  if (i != this->end ())
    this->first_orb_ = (*i).second.core ();
}

void
TAO::ORB_Table::not_default (char const * orb_id)
{
  // @@  This method now works for restricted cases. Should work on
  //     generalizing it. It works if the first ORB that is registered
  //     decides to not want be the default ORB. Should generalize it
  //     to handle all cases.
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);

  // Check if there is a default ORB already and if it is *not* the
  // same as the orb_id thats passed in.  We don't have to do
  // anything.
  if (this->first_orb_ != 0)
    {
      if (ACE_OS::strcmp (this->first_orb_->orbid (), orb_id) != 0)
        {
          // There is another default ORB. No need to change anything
          return;
        }
      else
        {
          // The ORB with orbid 'orb_id' is the default now. We need
          // to change it.
          this->first_orb_not_default_ = true;
        }
    }
}

TAO::ORB_Table *
TAO::ORB_Table::instance (void)
{
  return TAO_Singleton<TAO::ORB_Table, TAO_SYNCH_MUTEX>::instance ();
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template TAO_Singleton<TAO::ORB_Table,TAO_SYNCH_MUTEX> * TAO_Singleton<TAO::ORB_Table,TAO_SYNCH_MUTEX>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */

TAO_END_VERSIONED_NAMESPACE_DECL
