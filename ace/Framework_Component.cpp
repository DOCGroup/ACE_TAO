// Framework_Component.cpp
// $Id$

#include "ace/Framework_Component.h"
#include "ace/Object_Manager.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "ace/Framework_Component.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Framework_Component, "$Id$")

ACE_Framework_Component:: ~ACE_Framework_Component (void)
{
  ACE_TRACE ("ACE_Framework_Component::dtor");
}

/***************************************************************/

ACE_ALLOC_HOOK_DEFINE(ACE_Framework_Repository)

// Pointer to the Singleton instance.
ACE_Framework_Repository *ACE_Framework_Repository::repository_ = 0;

ACE_Framework_Repository::~ACE_Framework_Repository (void)
{
  ACE_TRACE ("ACE_Framework_Repository::~ACE_Framework_Repository");
  this->close ();
}

int
ACE_Framework_Repository::open (int size)
{
  ACE_TRACE ("ACE_Framework_Repository::open");

  ACE_Framework_Component **temp;

  ACE_NEW_RETURN (temp,
                  ACE_Framework_Component *[size],
                  -1);

  this->component_vector_ = ACE_const_cast (const ACE_Framework_Component **,
                                            temp);
  this->total_size_ = size;
  return 0;
}

int
ACE_Framework_Repository::close (void)
{
  ACE_TRACE ("ACE_Framework_Repository::close");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));

  if (this->component_vector_ != 0)
    {
      // Delete components in reverse order.
      for (int i = this->current_size_ - 1; i >= 0; i--)
        {
          ACE_Framework_Component *s = ACE_const_cast (ACE_Framework_Component *,
                                                       this->component_vector_[i]);
          --this->current_size_;
          delete s;
        }

      delete [] this->component_vector_;
      this->component_vector_ = 0;
      this->current_size_ = 0;
    }

  return 0;
}

ACE_Framework_Repository *
ACE_Framework_Repository::instance (int size)
{
  ACE_TRACE ("ACE_Framework_Repository::instance");

  if (ACE_Framework_Repository::repository_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), 0));
      if (ACE_Framework_Repository::repository_ == 0)
        {
          if (ACE_Object_Manager::starting_up () ||
              !ACE_Object_Manager::shutting_down ())
            {
              ACE_NEW_RETURN (ACE_Framework_Repository::repository_,
                              ACE_Framework_Repository (size),
                              0);
            }
        }
    }

  return ACE_Framework_Repository::repository_;
}

void
ACE_Framework_Repository::close_singleton (void)
{
  ACE_TRACE ("ACE_Framework_Repository::close_singleton");

  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon,
                     *ACE_Static_Object_Lock::instance ()));

  delete ACE_Framework_Repository::repository_;
  ACE_Framework_Repository::repository_ = 0;
}

int
ACE_Framework_Repository::register_component (const ACE_Framework_Component *fc)
{
  ACE_TRACE ("ACE_Framework_Repository::register_component");
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1));
  int i;

  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT("register_component\n")));

  // Check to see if it's already registered
  for (i = 0; i < this->current_size_; i++)
    if (fc->this_ == this->component_vector_[i]->this_)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("Component already registered.\n")));
        // Delete it since it's already here and component adapter was newed.
        delete fc;
        return 0;
      }

  if (i < this->total_size_)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("Component registered.\n")));
      this->component_vector_[i] = fc;
      this->current_size_++;
      return 0;
    }

  return -1;
}

void
ACE_Framework_Repository::dump (void) const
{
  ACE_TRACE ("ACE_Framework_Repository::dump");
}

ACE_Framework_Repository::ACE_Framework_Repository (int size)
  : current_size_ (0)
{
  ACE_TRACE ("ACE_Framework_Repository::ctor");

  if (this->open (size) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Framework_Repository")));
}
