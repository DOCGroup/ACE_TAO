// -*- C++ -*-
//
// $Id$


#include "Object_Ref_Table.h"
#include "Object.h"
#include "Exception.h"
#include "Environment.h"
#include "CORBA_String.h"
#include "debug.h"

ACE_RCSID (tao,
           Object_Ref_Table,
           "$Id$")


// ****************************************************************

TAO_Object_Ref_Table::TAO_Object_Ref_Table (void)
  : table_ ()
{
}

TAO_Object_Ref_Table::~TAO_Object_Ref_Table (void)
{
  // Must explicitly call destroy() in the destructor since not all
  // applications will invoke ORB::shutdown() or ORB::destroy().
  this->destroy ();
}

void
TAO_Object_Ref_Table::register_initial_reference (
  const char *id,
  CORBA::Object_ptr obj
  TAO_ENV_ARG_DECL)
{
  if (id == 0)
    ACE_THROW (CORBA::BAD_PARAM (TAO_OMG_VMCID | 27,
                                 CORBA::COMPLETED_NO));
  else if (ACE_OS_String::strlen (id) == 0)
    ACE_THROW (CORBA::BAD_PARAM (TAO_OMG_VMCID | 27,
                                 CORBA::COMPLETED_NO));

  if (CORBA::is_nil (obj))
    ACE_THROW (CORBA::BAD_PARAM (TAO_OMG_VMCID | 27,
                                 CORBA::COMPLETED_NO));

  int result = this->bind (id, obj);

  if (result == 1)
    {
      if (TAO_debug_level > 1)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) Object_Ref_Table::register_initial_reference:\n"
                    "  Could not register duplicate object <%s> with "
                    "the ORB\n",
                    id));

      ACE_THROW (CORBA::INV_OBJREF ());
    }

  if (result == -1)
    {
      if (TAO_debug_level > 1)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) Object_Ref_Table::register_initial_reference:\n"
                    "  Could not register object <%s> with "
                    "the ORB\n",
                    id));

      ACE_THROW (CORBA::INTERNAL ());
    }
}

CORBA::Object_ptr
TAO_Object_Ref_Table::resolve_initial_references (
  const char *id
  TAO_ENV_ARG_DECL_NOT_USED)
{
  return this->find (id);  // Returns a duplicate.
}

void
TAO_Object_Ref_Table::destroy (void)
{
  for (Iterator i = this->begin ();
       i != this->end ();
       ++i)
    {
      // Deallocate the id.
      CORBA::string_free (ACE_const_cast (char *, (*i).ext_id_));

      // Release the Object.
      CORBA::release ((*i).int_id_);
    }

  this->table_.unbind_all ();
}

TAO_Object_Ref_Table::Iterator
TAO_Object_Ref_Table::begin (void)
{
  return this->table_.begin ();
}

TAO_Object_Ref_Table::Iterator
TAO_Object_Ref_Table::end (void)
{
  return this->table_.end ();
}

size_t
TAO_Object_Ref_Table::current_size (void) const
{
  return this->table_.current_size ();
}

int
TAO_Object_Ref_Table::bind (const char *id,
                            CORBA::Object_ptr obj)
{
  // Make sure that the supplied Object reference is valid,
  // i.e. not nil.
  if (id == 0 || CORBA::is_nil (obj))
    {
      errno = EINVAL;
      return -1;
    };

  CORBA::String_var name = CORBA::string_dup (id);
  CORBA::Object_var object = CORBA::Object::_duplicate (obj);

  int result = this->table_.bind (name.in (),
                                  object.in ());

  if (result == 0)
    {
      // Transfer ownership to the Object Table.
      (void) name._retn ();
      (void) object._retn ();
    }

  return result;
}

CORBA::Object_ptr
TAO_Object_Ref_Table::find (const char *id)
{
  CORBA::Object_ptr found = CORBA::Object::_nil ();

  this->table_.find (id, found);

  return CORBA::Object::_duplicate (found);
}

int
TAO_Object_Ref_Table::unbind (const char *id)
{
  Table::ENTRY *entry = 0;

  int result = this->table_.find (id, entry);

  if (result == 0)
    {
      // Deallocate the external ID and obtain the ORB core pointer
      // before unbinding the entry since the entry is deallocated
      // during the call to unbind().
      CORBA::string_free (ACE_const_cast (char *, entry->ext_id_));
      CORBA::Object_ptr obj = entry->int_id_;

      result = this->table_.unbind (entry);

      if (result != 0)
        return result;

      CORBA::release (obj);
    }

  return result;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<const char *, CORBA::Object_ptr>;
template class ACE_Hash_Map_Manager_Ex<const char *, CORBA::Object_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<const char *, CORBA::Object_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<const char *, CORBA::Object_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<const char *, CORBA::Object_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<const char *, CORBA::Object_ptr>
#pragma instantiate ACE_Hash_Map_Manager_Ex<const char *, CORBA::Object_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<const char *, CORBA::Object_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<const char *, CORBA::Object_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<const char *, CORBA::Object_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
