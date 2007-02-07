// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Object_Ref_Table::TAO_Object_Ref_Table (void)
  : table_ (TAO_DEFAULT_OBJECT_REF_TABLE_SIZE)
  , lock_ ()
{
}

ACE_INLINE int
TAO_Object_Ref_Table::register_initial_reference (
  const char *id,
  CORBA::Object_ptr obj,
  bool rebind)
{
  if (rebind)
    {
      if (this->unbind (id) == -1)
        return -1;
      else
        return this->bind (id, obj);
    }
  else
    return this->bind (id, obj);
}

ACE_INLINE CORBA::Object_ptr
TAO_Object_Ref_Table::resolve_initial_reference (const char * id)
{
  return this->find (id);  // Returns a duplicate.
}

ACE_INLINE void
TAO_Object_Ref_Table::destroy (void)
{
  Table tmp;

  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);

  this->table_.swap (tmp);  // Force release of memory held by our table.
}

ACE_INLINE TAO_Object_Ref_Table::iterator
TAO_Object_Ref_Table::begin (void)
{
  return this->table_.begin ();
}

ACE_INLINE TAO_Object_Ref_Table::iterator
TAO_Object_Ref_Table::end (void)
{
  return this->table_.end ();
}

ACE_INLINE size_t
TAO_Object_Ref_Table::current_size (void) const
{
  return this->table_.size ();
}

ACE_INLINE int
TAO_Object_Ref_Table::unbind (const char *id)
{
  return
    (this->table_.erase (CORBA::String_var (id)) == 0 ? -1 : 0);
}

TAO_END_VERSIONED_NAMESPACE_DECL
