// $Id$

ACE_INLINE
TAO_Active_Object_Map_Entry::TAO_Active_Object_Map_Entry (void)
  : id_ (),
    generation_ (0),
    servant_ (0),
    is_free_ (1)
{
}

ACE_INLINE
TAO_Active_Object_Map_Iterator_Impl::~TAO_Active_Object_Map_Iterator_Impl (void)
{
}

ACE_INLINE
TAO_Active_Object_Map_Impl::~TAO_Active_Object_Map_Impl (void)
{
}

ACE_INLINE int
TAO_Active_Object_Map_Impl::find (const PortableServer::ObjectId &id)
{
  PortableServer::Servant servant;
  return this->find (id, servant);
}

ACE_INLINE int
TAO_Active_Object_Map_Impl::find (const PortableServer::Servant servant)
{
  PortableServer::ObjectId id;
  return this->find (servant, id);
}

ACE_INLINE
TAO_Dynamic_Hash_Active_Object_Map::TAO_Dynamic_Hash_Active_Object_Map (CORBA::ULong size)
  : hash_map_ (size),
    counter_ (0)
{
}

ACE_INLINE int
TAO_Dynamic_Hash_Active_Object_Map::find (const PortableServer::ObjectId &id,
                                          PortableServer::Servant &servant)
{
  return this->hash_map_.find (id, servant);
}

ACE_INLINE int
TAO_Dynamic_Hash_Active_Object_Map::bind (const PortableServer::ObjectId &id,
                                          PortableServer::Servant servant)
{
  return this->hash_map_.bind (id, servant);
}

ACE_INLINE int
TAO_Dynamic_Hash_Active_Object_Map::unbind (const PortableServer::ObjectId &id,
                                            PortableServer::Servant &servant)
{
  return this->hash_map_.unbind (id, servant);
}

ACE_INLINE int
TAO_Dynamic_Hash_Active_Object_Map::find (const PortableServer::Servant servant)
{
  return this->TAO_Active_Object_Map_Impl::find (servant);
}

ACE_INLINE int
TAO_Dynamic_Hash_Active_Object_Map::find (const PortableServer::ObjectId &id)
{
  return this->TAO_Active_Object_Map_Impl::find (id);
}

ACE_INLINE int
TAO_Dynamic_Hash_Active_Object_Map::find (const PortableServer::Servant servant,
                                          PortableServer::ObjectId &id)
{
  return this->TAO_Active_Object_Map_Impl::find (servant, id);
}

ACE_INLINE CORBA::ULong
TAO_Dynamic_Hash_Active_Object_Map::system_id_size (void) const
{
  return sizeof (CORBA::ULong);
}

ACE_INLINE int
TAO_Dynamic_Hash_Active_Object_Map::is_free (const TAO_Active_Object_Map_Entry &item) const
{
  ACE_UNUSED_ARG (item);
  return 0;
}

ACE_INLINE
TAO_Dynamic_Hash_Active_Object_Map_Iterator::TAO_Dynamic_Hash_Active_Object_Map_Iterator (const Impl &impl)
  : impl_ (impl)
{
}

ACE_INLINE TAO_Active_Object_Map_Iterator_Impl *
TAO_Dynamic_Hash_Active_Object_Map::begin (void) const
{
  TAO_Dynamic_Hash_Active_Object_Map *non_const =
    ACE_const_cast (TAO_Dynamic_Hash_Active_Object_Map *,
                    this);

  TAO_Active_Object_Map_Iterator_Impl *tmp;
  ACE_NEW_RETURN (tmp,
                  TAO_Dynamic_Hash_Active_Object_Map_Iterator (iterator (non_const->hash_map_)),
                  0);
  return tmp;
}

ACE_INLINE TAO_Active_Object_Map_Iterator_Impl *
TAO_Dynamic_Hash_Active_Object_Map::end (void) const
{
  return 0;
}

ACE_INLINE TAO_Active_Object_Map_Iterator_Impl*
TAO_Dynamic_Hash_Active_Object_Map_Iterator::clone (void) const
{
  TAO_Active_Object_Map_Iterator_Impl *tmp;
  ACE_NEW_RETURN (tmp,
                  TAO_Dynamic_Hash_Active_Object_Map_Iterator (*this),
                  0);
  return tmp;
}

ACE_INLINE const TAO_Active_Object_Map_Entry &
TAO_Dynamic_Hash_Active_Object_Map_Iterator::item (void) const
{
  TAO_Active_Object_Map_Entry &entry =
    ACE_const_cast (TAO_Active_Object_Map_Entry &,
                    this->entry_);
  ACE_Hash_Map_Entry<PortableServer::ObjectId, PortableServer::Servant> *tmp;

  if (ACE_const_cast (TAO_Dynamic_Hash_Active_Object_Map_Iterator*,
                      this)->impl_.next (tmp) == 1)
    {
      entry.servant_ = tmp->int_id_;
      entry.id_ = tmp->ext_id_;
    }

  return entry;
}

ACE_INLINE void
TAO_Dynamic_Hash_Active_Object_Map_Iterator::advance (void)
{
  this->impl_.advance ();
}

ACE_INLINE int
TAO_Dynamic_Hash_Active_Object_Map_Iterator::done (const TAO_Active_Object_Map_Iterator_Impl *) const
{
  return this->impl_.done ();
}

ACE_INLINE
TAO_Array_Active_Object_Map_Iterator::TAO_Array_Active_Object_Map_Iterator (TAO_Active_Object_Map_Entry *pos)
  : pos_ (pos)
{
}

ACE_INLINE TAO_Active_Object_Map_Iterator_Impl *
TAO_Array_Active_Object_Map_Iterator::clone (void) const
{
  TAO_Active_Object_Map_Iterator_Impl *tmp;
  ACE_NEW_RETURN (tmp,
                  TAO_Array_Active_Object_Map_Iterator (*this),
                  0);
  return tmp;
}

ACE_INLINE const TAO_Active_Object_Map_Entry &
TAO_Array_Active_Object_Map_Iterator::item (void) const
{
  return *this->pos_;
}

ACE_INLINE void
TAO_Array_Active_Object_Map_Iterator::advance (void)
{
  ++this->pos_;
}

ACE_INLINE int
TAO_Array_Active_Object_Map_Iterator::done (const TAO_Active_Object_Map_Iterator_Impl *end) const
{
  const TAO_Array_Active_Object_Map_Iterator *tmp =
    ACE_dynamic_cast (const TAO_Array_Active_Object_Map_Iterator*, end);
  return this->pos_ == tmp->pos_;
}

ACE_INLINE
TAO_Linear_Active_Object_Map::TAO_Linear_Active_Object_Map (CORBA::ULong size)
  : next_ (0),
    mapsize_ (size),
    counter_ (0)
{
  ACE_NEW (map_,
           TAO_Active_Object_Map_Entry[this->mapsize_]);
}

ACE_INLINE
TAO_Linear_Active_Object_Map::~TAO_Linear_Active_Object_Map (void)
{
  delete [] this->map_;
}

ACE_INLINE int
TAO_Linear_Active_Object_Map::find (const PortableServer::Servant servant)
{
  return this->TAO_Active_Object_Map_Impl::find (servant);
}

ACE_INLINE int
TAO_Linear_Active_Object_Map::find (const PortableServer::ObjectId &id)
{
  return this->TAO_Active_Object_Map_Impl::find (id);
}

ACE_INLINE int
TAO_Linear_Active_Object_Map::find (const PortableServer::Servant servant,
                                    PortableServer::ObjectId &id)
{
  return this->TAO_Active_Object_Map_Impl::find (servant, id);
}

ACE_INLINE CORBA::ULong
TAO_Linear_Active_Object_Map::system_id_size (void) const
{
  return sizeof (CORBA::ULong);
}

ACE_INLINE int
TAO_Linear_Active_Object_Map::is_free (const TAO_Active_Object_Map_Entry &item) const
{
  return item.is_free_;
}

ACE_INLINE TAO_Active_Object_Map_Iterator_Impl *
TAO_Linear_Active_Object_Map::begin (void) const
{
  TAO_Active_Object_Map_Iterator_Impl *tmp;
  ACE_NEW_RETURN (tmp,
                  TAO_Array_Active_Object_Map_Iterator (this->map_),
                  0);
  return tmp;
}

ACE_INLINE TAO_Active_Object_Map_Iterator_Impl *
TAO_Linear_Active_Object_Map::end (void) const
{
  TAO_Active_Object_Map_Iterator_Impl *tmp;
  ACE_NEW_RETURN (tmp,
                  TAO_Array_Active_Object_Map_Iterator (this->map_ + this->next_),
                  0);
  return tmp;
}

ACE_INLINE
TAO_Active_Demux_Active_Object_Map::TAO_Active_Demux_Active_Object_Map (CORBA::ULong size)
  : TAO_Linear_Active_Object_Map (size)
{
}

ACE_INLINE
TAO_Active_Demux_Active_Object_Map::~TAO_Active_Demux_Active_Object_Map (void)
{
}

ACE_INLINE int
TAO_Active_Demux_Active_Object_Map::find (const PortableServer::Servant servant)
{
  return this->TAO_Active_Object_Map_Impl::find (servant);
}

ACE_INLINE int
TAO_Active_Demux_Active_Object_Map::find (const PortableServer::ObjectId &id)
{
  return this->TAO_Active_Object_Map_Impl::find (id);
}

ACE_INLINE int
TAO_Active_Demux_Active_Object_Map::find (const PortableServer::Servant servant,
                                          PortableServer::ObjectId &id)
{
  return this->TAO_Active_Object_Map_Impl::find (servant, id);
}

ACE_INLINE int
TAO_Active_Demux_Active_Object_Map::parse_object_id (const PortableServer::ObjectId &id,
                                                     CORBA::ULong &index,
                                                     CORBA::ULong &generation)
{
  CORBA::ULong id_data[2];

  ACE_OS::memcpy (&id_data,
                  id.get_buffer (),
                  sizeof id_data);

  index =
    id_data[TAO_Active_Demux_Active_Object_Map::INDEX_FIELD];
  generation =
    id_data[TAO_Active_Demux_Active_Object_Map::GENERATION_FIELD];

  return 0;
}

ACE_INLINE CORBA::ULong
TAO_Active_Demux_Active_Object_Map::system_id_size (void) const
{
  return 2 * sizeof (CORBA::ULong);
}

ACE_INLINE
TAO_Dynamic_Hash_Active_Object_Map::~TAO_Dynamic_Hash_Active_Object_Map (void)
{
}

ACE_INLINE
TAO_Dynamic_Hash_Active_Object_Map_Iterator::~TAO_Dynamic_Hash_Active_Object_Map_Iterator (void)
{
}

ACE_INLINE
TAO_Array_Active_Object_Map_Iterator::~TAO_Array_Active_Object_Map_Iterator (void)
{
}

ACE_INLINE
TAO_Reverse_Active_Object_Map_Impl::TAO_Reverse_Active_Object_Map_Impl (void)
{
}

ACE_INLINE
TAO_Reverse_Active_Object_Map_Impl::~TAO_Reverse_Active_Object_Map_Impl (void)
{
}

ACE_INLINE
TAO_Reverse_Active_Object_Map_For_Unique_Id_Policy::TAO_Reverse_Active_Object_Map_For_Unique_Id_Policy (size_t size)
  : map_ (size)
{
}

ACE_INLINE
TAO_Reverse_Active_Object_Map_For_Unique_Id_Policy::~TAO_Reverse_Active_Object_Map_For_Unique_Id_Policy (void)
{
}

ACE_INLINE int
TAO_Reverse_Active_Object_Map_For_Unique_Id_Policy::bind (PortableServer::Servant servant,
                                                          const PortableServer::ObjectId &id)
{
  return this->map_.bind (servant, id);
}

ACE_INLINE int
TAO_Reverse_Active_Object_Map_For_Unique_Id_Policy::unbind (PortableServer::Servant servant)
{
  return this->map_.unbind (servant);
}

ACE_INLINE int
TAO_Reverse_Active_Object_Map_For_Unique_Id_Policy::find (const PortableServer::Servant servant,
                                                          PortableServer::ObjectId &id)
{
  return this->map_.find (servant, id);
}

ACE_INLINE int
TAO_Reverse_Active_Object_Map_For_Unique_Id_Policy::find (PortableServer::Servant servant)
{
  return this->map_.find (servant);
}

ACE_INLINE
TAO_Reverse_Active_Object_Map_For_Multiple_Id_Policy::TAO_Reverse_Active_Object_Map_For_Multiple_Id_Policy (void)
{
}

ACE_INLINE
TAO_Reverse_Active_Object_Map_For_Multiple_Id_Policy::~TAO_Reverse_Active_Object_Map_For_Multiple_Id_Policy (void)
{
}

ACE_INLINE int
TAO_Reverse_Active_Object_Map_For_Multiple_Id_Policy::bind (PortableServer::Servant servant,
                                                            const PortableServer::ObjectId &id)
{
  ACE_UNUSED_ARG (servant);
  ACE_UNUSED_ARG (id);

  // Successful no-op
  return 0;
}

ACE_INLINE int
TAO_Reverse_Active_Object_Map_For_Multiple_Id_Policy::unbind (PortableServer::Servant servant)
{
  ACE_UNUSED_ARG (servant);

  // Successful no-op
  return 0;
}

ACE_INLINE int
TAO_Reverse_Active_Object_Map_For_Multiple_Id_Policy::find (const PortableServer::Servant servant,
                                                            PortableServer::ObjectId &id)
{
  ACE_UNUSED_ARG (servant);
  ACE_UNUSED_ARG (id);

  // Unsuccessful no-op
  return -1;
}

ACE_INLINE int
TAO_Reverse_Active_Object_Map_For_Multiple_Id_Policy::find (PortableServer::Servant servant)
{
  ACE_UNUSED_ARG (servant);

  // Unsuccessful no-op
  return -1;
}

ACE_INLINE
TAO_Active_Object_Map_Iterator::TAO_Active_Object_Map_Iterator (TAO_Active_Object_Map_Iterator_Impl *impl)
  : impl_ (impl)
{
}

ACE_INLINE
TAO_Active_Object_Map_Iterator::TAO_Active_Object_Map_Iterator (const TAO_Active_Object_Map_Iterator &x)
  : impl_ (0)
{
  if (x.impl_ != 0)
    this->impl_ = x.impl_->clone ();
}

ACE_INLINE TAO_Active_Object_Map_Iterator &
TAO_Active_Object_Map_Iterator::operator= (const TAO_Active_Object_Map_Iterator &x)
{
  if (this != &x)
    {
      delete this->impl_;
      if (x.impl_ == 0)
        this->impl_ = 0;
      else
        this->impl_ = x.impl_->clone ();
    }
  return *this;
}

ACE_INLINE
TAO_Active_Object_Map_Iterator::~TAO_Active_Object_Map_Iterator (void)
{
  delete this->impl_;
}

ACE_INLINE const TAO_Active_Object_Map_Entry &
TAO_Active_Object_Map_Iterator::operator *(void) const
{
  return this->impl_->item ();
}

ACE_INLINE TAO_Active_Object_Map_Iterator
TAO_Active_Object_Map_Iterator::operator++ (void)
{
  TAO_Active_Object_Map_Iterator tmp = *this;
  this->impl_->advance ();
  return tmp;
}

ACE_INLINE TAO_Active_Object_Map_Iterator
TAO_Active_Object_Map_Iterator::operator++ (int)
{
  this->impl_->advance ();
  return *this;
}

ACE_INLINE int
operator== (const TAO_Active_Object_Map_Iterator &l,
            const TAO_Active_Object_Map_Iterator &r)
{
  return l.impl_->done (r.impl_);
}

ACE_INLINE int
operator!= (const TAO_Active_Object_Map_Iterator &l,
            const TAO_Active_Object_Map_Iterator &r)
{
  return !(l == r);
}

ACE_INLINE
TAO_Active_Object_Map::~TAO_Active_Object_Map (void)
{
  delete this->impl_;
  delete this->reverse_impl_;
}

ACE_INLINE int
TAO_Active_Object_Map::bind (const PortableServer::ObjectId &id,
                             PortableServer::Servant servant)
{
  int result = this->impl_->bind (id, servant);
  if (result != 0)
    {
      return result;
    }

  result = this->reverse_impl_->bind (servant, id);
  if (result != 0)
    {
      this->impl_->unbind (id, servant);
    }

  return result;
}

ACE_INLINE int
TAO_Active_Object_Map::unbind (const PortableServer::ObjectId &id,
                               PortableServer::Servant &servant)
{
  int result = this->impl_->unbind (id, servant);
  if (result != 0)
    return result;

  return this->reverse_impl_->unbind (servant);
}

ACE_INLINE int
TAO_Active_Object_Map::find (const PortableServer::ObjectId &id,
                             PortableServer::Servant &servant)
{
  return this->impl_->find (id, servant);
}

ACE_INLINE int
TAO_Active_Object_Map::find (const PortableServer::ObjectId &id)
{
  return this->impl_->find (id);
}

ACE_INLINE int
TAO_Active_Object_Map::find (const PortableServer::Servant servant)
{
  if (this->unique_id_policy_)
    {
      return this->reverse_impl_->find (servant);
    }
  else
    {
      return this->impl_->find (servant);
    }
}

ACE_INLINE int
TAO_Active_Object_Map::find (const PortableServer::Servant servant,
                             PortableServer::ObjectId &id)
{
  if (this->unique_id_policy_)
    {
      return this->reverse_impl_->find (servant, id);
    }
  else
    {
      return this->impl_->find (servant, id);
    }
}

ACE_INLINE TAO_Active_Object_Map::iterator
TAO_Active_Object_Map::begin (void) const
{
  return TAO_Active_Object_Map::iterator (this->impl_->begin ());
}

ACE_INLINE TAO_Active_Object_Map::iterator
TAO_Active_Object_Map::end (void) const
{
  return TAO_Active_Object_Map::iterator (this->impl_->end ());
}

ACE_INLINE PortableServer::ObjectId *
TAO_Active_Object_Map::create_object_id (PortableServer::Servant servant,
                                         CORBA::Environment &TAO_IN_ENV)
{
  return this->impl_->create_object_id (servant, TAO_IN_ENV);
}

ACE_INLINE CORBA::ULong
TAO_Active_Object_Map::system_id_size (void) const
{
  return this->impl_->system_id_size ();
}
