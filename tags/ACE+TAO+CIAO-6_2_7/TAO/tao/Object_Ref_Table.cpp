// $Id$

#include "tao/Object_Ref_Table.h"
#include "tao/ORB.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"
#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"

#ifndef __ACE_INLINE__
# include "tao/Object_Ref_Table.inl"
#endif  /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_Object_Ref_Table::register_initial_reference (
  const char *id,
  CORBA::Object_ptr obj,
  bool rebind)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    -1);

  if (rebind)
    {
      if (this->unbind_i (id) == -1)
        return -1;
      else
        return this->bind_i (id, obj);
    }
  else
    return this->bind_i (id, obj);
}

CORBA::Object_ptr
TAO_Object_Ref_Table::unregister_initial_reference (
  const char *id)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    CORBA::Object::_nil());

  CORBA::Object_ptr obj = this->find_i (id);
  if (this->unbind_i (id) == -1)
    {
      if (TAO_debug_level > 1)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) Object_Ref_Table::bind_i: ")
                      ACE_TEXT ("Could not unregister object <%C> ")
                      ACE_TEXT ("from the ORB\n"),
                      id));
        }
    }

  return obj;
}

int
TAO_Object_Ref_Table::bind_i (const char *id, CORBA::Object_ptr obj)
{
  // Make sure that the supplied Object reference is valid,
  // i.e. not nil.
  if (id == 0
      || ACE_OS::strlen (id) == 0
      || ::CORBA::is_nil (obj))
    {
      errno = EINVAL;
      return -1;
    };

  Table::value_type const value =
    std::make_pair (CORBA::String_var (id),
                    CORBA::Object_var (CORBA::Object::_duplicate (obj)));

  std::pair<iterator, bool> const result = this->table_.insert (value);

  if (!result.second)
    {
      if (TAO_debug_level > 1)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) Object_Ref_Table::bind_i: ")
                      ACE_TEXT ("Could not register duplicate object <%C> ")
                      ACE_TEXT ("with the ORB\n"),
                      id));
        }

      return -1;
    }

  return 0;
}

CORBA::Object_ptr
TAO_Object_Ref_Table::resolve_initial_reference (const char * id)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    CORBA::Object::_nil ());

  return this->find_i (id);  // Returns a duplicate.
}

TAO_END_VERSIONED_NAMESPACE_DECL
