// $Id$

ACE_INLINE 
TAO_Object_Table_Iterator_Impl::~TAO_Object_Table_Iterator_Impl (void)
{
}

ACE_INLINE 
TAO_Object_Table_Impl::~TAO_Object_Table_Impl (void)
{
}

ACE_INLINE int
TAO_Object_Table_Impl::find (const PortableServer::ObjectId &id)
{
  PortableServer::Servant servant;
  return this->find (id, servant);
}

ACE_INLINE PortableServer::ObjectId *
TAO_Object_Table_Impl::create_object_id (PortableServer::Servant servant, 
                                         CORBA::Environment &env)
{
  CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
  env.exception (exception);
  return 0;
}

ACE_INLINE 
TAO_Object_Table_Iterator::TAO_Object_Table_Iterator (TAO_Object_Table_Iterator_Impl *impl)
  : impl_ (impl)
{
}

ACE_INLINE 
TAO_Object_Table_Iterator::TAO_Object_Table_Iterator (const TAO_Object_Table_Iterator &x)
  : impl_ (0)
{
  if (x.impl_ != 0)
    {
      this->impl_ = x.impl_->clone ();
    }
}

ACE_INLINE TAO_Object_Table_Iterator &
TAO_Object_Table_Iterator::operator= (const TAO_Object_Table_Iterator &x)
{
  if (this != &x)
    {
      delete this->impl_;
      if (x.impl_ == 0)
	{
	  this->impl_ = 0;
	}
      else
	{
	  this->impl_ = x.impl_->clone ();
	}
    }
  return *this;
}

ACE_INLINE 
TAO_Object_Table_Iterator::~TAO_Object_Table_Iterator (void)
{
  delete this->impl_;
}

ACE_INLINE const TAO_Object_Table_Entry &
TAO_Object_Table_Iterator::operator *(void) const
{
  return this->impl_->item ();
}

ACE_INLINE TAO_Object_Table_Iterator
TAO_Object_Table_Iterator::operator++ (void)
{
  TAO_Object_Table_Iterator tmp = *this;
  this->impl_->advance ();
  return tmp;
}

ACE_INLINE TAO_Object_Table_Iterator
TAO_Object_Table_Iterator::operator++ (int)
{
  this->impl_->advance ();
  return *this;
}

ACE_INLINE int
operator== (const TAO_Object_Table_Iterator &l,
	    const TAO_Object_Table_Iterator &r)
{
  return l.impl_->done (r.impl_);
}

ACE_INLINE int
operator!= (const TAO_Object_Table_Iterator &l,
	    const TAO_Object_Table_Iterator &r)
{
  return !(l == r);
}

ACE_INLINE 
TAO_Object_Table::~TAO_Object_Table (void)
{
  if (this->delete_impl_)
    delete this->impl_;
}

ACE_INLINE int
TAO_Object_Table::find (const PortableServer::ObjectId &id, 
			PortableServer::Servant &servant)
{
  return this->impl_->find (id, servant);
}

ACE_INLINE int
TAO_Object_Table::bind (const PortableServer::ObjectId &id, 
			PortableServer::Servant servant)
{
  return this->impl_->bind (id, servant);
}

ACE_INLINE int
TAO_Object_Table::unbind (const PortableServer::ObjectId &id,
			  PortableServer::Servant &servant)
{
  return this->impl_->unbind (id, servant);
}

ACE_INLINE int
TAO_Object_Table::find (const PortableServer::Servant servant)
{
  return this->impl_->find (servant);
}

ACE_INLINE int
TAO_Object_Table::find (const PortableServer::ObjectId &id)
{
  return this->impl_->find (id);
}

ACE_INLINE int
TAO_Object_Table::find (const PortableServer::Servant servant,
			PortableServer::ObjectId_out id)
{
  return this->impl_->find (servant, id);
}

ACE_INLINE TAO_Object_Table::iterator
TAO_Object_Table::begin (void) const
{
  return TAO_Object_Table::iterator (this->impl_->begin ());
}

ACE_INLINE TAO_Object_Table::iterator
TAO_Object_Table::end (void) const
{
  return TAO_Object_Table::iterator (this->impl_->end ());
}

ACE_INLINE PortableServer::ObjectId *
TAO_Object_Table::create_object_id (PortableServer::Servant servant, 
                                    CORBA::Environment &env)
{
  return this->impl_->create_object_id (servant, env);
}

ACE_INLINE int
TAO_Dynamic_Hash_ObjTable::find (const PortableServer::Servant servant)
{
  return this->TAO_Object_Table_Impl::find (servant);
}

ACE_INLINE int
TAO_Dynamic_Hash_ObjTable::find (const PortableServer::ObjectId &id)
{
  return this->TAO_Object_Table_Impl::find (id);
}

ACE_INLINE int
TAO_Dynamic_Hash_ObjTable::find (const PortableServer::Servant servant,
				 PortableServer::ObjectId_out id)
{
  return this->TAO_Object_Table_Impl::find (servant, id);
}

ACE_INLINE int
TAO_Dynamic_Hash_ObjTable::find (const PortableServer::ObjectId &id,
				 PortableServer::Servant &servant)
{
  return this->hash_map_.find (id, servant);
}

ACE_INLINE int
TAO_Dynamic_Hash_ObjTable::bind (const PortableServer::ObjectId &id,
				 PortableServer::Servant servant)
{
  return this->hash_map_.bind (id, servant);
}

ACE_INLINE int
TAO_Dynamic_Hash_ObjTable::unbind (const PortableServer::ObjectId &id,
				   PortableServer::Servant &servant)
{
  return this->hash_map_.unbind (id, servant);
}

ACE_INLINE TAO_Object_Table_Iterator_Impl *
TAO_Dynamic_Hash_ObjTable::begin (void) const
{
  TAO_Dynamic_Hash_ObjTable *non_const = 
    ACE_const_cast(TAO_Dynamic_Hash_ObjTable*, this);
  return new TAO_Dynamic_Hash_ObjTable_Iterator (Iterator (non_const->hash_map_));
}

ACE_INLINE TAO_Object_Table_Iterator_Impl *
TAO_Dynamic_Hash_ObjTable::end (void) const
{
  return 0;
}

ACE_INLINE 
TAO_Dynamic_Hash_ObjTable_Iterator::TAO_Dynamic_Hash_ObjTable_Iterator (const Impl &impl)
  : impl_ (impl)
{
}

ACE_INLINE TAO_Object_Table_Iterator_Impl*
TAO_Dynamic_Hash_ObjTable_Iterator::clone (void) const
{
  return new TAO_Dynamic_Hash_ObjTable_Iterator (*this);
}

ACE_INLINE const TAO_Object_Table_Entry &
TAO_Dynamic_Hash_ObjTable_Iterator::item (void) const
{
  TAO_Object_Table_Entry &entry = 
    ACE_const_cast(TAO_Object_Table_Entry&, this->entry_);
  ACE_Hash_Map_Entry<PortableServer::ObjectId,PortableServer::Servant> *tmp;
  if (ACE_const_cast(TAO_Dynamic_Hash_ObjTable_Iterator*,this)->impl_.next (tmp) == 1)
    {
      entry.int_id_ = tmp->int_id_;
      entry.ext_id_ = tmp->ext_id_;
    }
  return entry;
}

ACE_INLINE void
TAO_Dynamic_Hash_ObjTable_Iterator::advance (void)
{
  this->impl_.advance ();
}

ACE_INLINE int
TAO_Dynamic_Hash_ObjTable_Iterator::done (const TAO_Object_Table_Iterator_Impl *) const
{
  return this->impl_.done ();
}

ACE_INLINE 
TAO_Array_ObjTable_Iterator::TAO_Array_ObjTable_Iterator (TAO_Object_Table_Entry *pos)
  : pos_ (pos)
{
}

ACE_INLINE TAO_Object_Table_Iterator_Impl *
TAO_Array_ObjTable_Iterator::clone (void) const
{
  return new TAO_Array_ObjTable_Iterator (*this);
}

ACE_INLINE const TAO_Object_Table_Entry &
TAO_Array_ObjTable_Iterator::item (void) const
{
  return *this->pos_;
}

ACE_INLINE void
TAO_Array_ObjTable_Iterator::advance (void)
{
  this->pos_++;
}

ACE_INLINE int
TAO_Array_ObjTable_Iterator::done (const TAO_Object_Table_Iterator_Impl *end) const
{
  const TAO_Array_ObjTable_Iterator *tmp =
    ACE_dynamic_cast(const TAO_Array_ObjTable_Iterator*, end);
  return (this->pos_ == tmp->pos_);
}

ACE_INLINE 
TAO_Linear_ObjTable::~TAO_Linear_ObjTable (void)
{
  delete[] this->table_;
}

ACE_INLINE int
TAO_Linear_ObjTable::find (const PortableServer::Servant servant)
{
  return this->TAO_Object_Table_Impl::find (servant);
}

ACE_INLINE int
TAO_Linear_ObjTable::find (const PortableServer::ObjectId &id)
{
  return this->TAO_Object_Table_Impl::find (id);
}

ACE_INLINE int
TAO_Linear_ObjTable::find (const PortableServer::Servant servant,
			   PortableServer::ObjectId_out id)
{
  return this->TAO_Object_Table_Impl::find (servant, id);
}

ACE_INLINE TAO_Object_Table_Iterator_Impl *
TAO_Linear_ObjTable::begin (void) const
{
  return new TAO_Array_ObjTable_Iterator (this->table_);
}

ACE_INLINE TAO_Object_Table_Iterator_Impl*
TAO_Linear_ObjTable::end (void) const
{
  return new TAO_Array_ObjTable_Iterator (this->table_ + this->next_);
}

ACE_INLINE 
TAO_Active_Demux_ObjTable::~TAO_Active_Demux_ObjTable (void)
{
}

ACE_INLINE int
TAO_Active_Demux_ObjTable::find (const PortableServer::Servant servant)
{
  return this->TAO_Object_Table_Impl::find (servant);
}

ACE_INLINE int
TAO_Active_Demux_ObjTable::find (const PortableServer::ObjectId &id)
{
  return this->TAO_Object_Table_Impl::find (id);
}

ACE_INLINE int
TAO_Active_Demux_ObjTable::find (const PortableServer::Servant servant,
				 PortableServer::ObjectId_out id)
{
  return this->TAO_Object_Table_Impl::find (servant, id);
}


