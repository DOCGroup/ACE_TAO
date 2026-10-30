#include "tao/Cleanup_Func_Registry.h"

#if !defined (__ACE_INLINE__)
# include "tao/Cleanup_Func_Registry.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Log_Msg.h"

#include <new>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_Cleanup_Func_Registry::register_cleanup_function (ACE_CLEANUP_FUNC func,
                                                      size_t &slot_id)
{
  size_t const slot = this->cleanup_funcs_.size ();

  try
    {
      this->cleanup_funcs_.push_back (func);
    }
  catch (std::bad_alloc const &)
    {
      return -1;
    }

  slot_id = slot;

  return 0;
}

void
TAO_Cleanup_Func_Registry::cleanup (ACE_Array_Base <void *> &ts_objects)
{
  size_t const len = ts_objects.size ();

  // The allocated slot may never have been used.  It is therefore
  // possible that the TSS array size may be less than the cleanup
  // function size.  However, there is still a one-to-one
  // correspondence between cleanup_func[foo] and ts_object[foo].

  ACE_ASSERT (len <= this->cleanup_funcs_.size ());

  /// Cleanup each TSS object.
  for (size_t i = 0; i < len; ++i)
    {
      ACE_CLEANUP_FUNC destructor = this->cleanup_funcs_[i];
      if (destructor != nullptr)
        destructor (ts_objects[i], nullptr);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
