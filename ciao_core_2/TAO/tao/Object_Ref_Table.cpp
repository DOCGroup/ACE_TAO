// $Id$

#include "tao/Object_Ref_Table.h"
#include "tao/ORB.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"
#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"


ACE_RCSID (tao,
           Object_Ref_Table,
           "$Id$")

#ifndef __ACE_INLINE__
# include "tao/Object_Ref_Table.inl"
#endif  /* __ACE_INLINE__ */


// ****************************************************************

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_Object_Ref_Table::bind (const char *id, CORBA::Object_ptr obj)
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

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    -1);

  std::pair<iterator, bool> const result = this->table_.insert (value);

  if (!result.second)
    {
      if (TAO_debug_level > 1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) Object_Ref_Table::")
                      ACE_TEXT ("bind:")
                      ACE_TEXT ("  Could not register duplicate object <%s> ")
                      ACE_TEXT ("with the ORB\n"),
                      ACE_TEXT_CHAR_TO_TCHAR (id)));
        }

      return -1;
    }

  return 0;
}

CORBA::Object_ptr
TAO_Object_Ref_Table::find (const char *id)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    CORBA::Object::_nil ());

  iterator const found =
    this->table_.find (CORBA::String_var (id));

  if (found == this->table_.end ())
    return CORBA::Object::_nil ();

  return CORBA::Object::_duplicate ((*found).second.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
