// $Id$

#include "ace/Auto_Ptr.h"
#include "tao/corba.h"
#include "tao/Object_Table.h"

ACE_RCSID(tao, Object_Table, "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/Object_Table.i"
#endif /* ! __ACE_INLINE__ */

TAO_Object_Table::TAO_Object_Table (TAO_Object_Table_Impl *impl,
                                    int delete_impl)
  : impl_ (impl),
    delete_impl_ (delete_impl)
{
  if (this->impl_ == 0)
    {
      this->impl_ =
        TAO_ORB_Core_instance ()->server_factory ()->create_object_table ();
      this->delete_impl_ = 1;
    }
}

int
TAO_Object_Table_Impl::find (const PortableServer::Servant servant,
                             PortableServer::ObjectId_out id)
{
  id.ptr () = 0;
  auto_ptr<TAO_Object_Table_Iterator_Impl> end (this->end ());

  for (auto_ptr<TAO_Object_Table_Iterator_Impl> i (this->begin ());
       !i->done (end.get ());
       i->advance ())
    {
      const TAO_Object_Table_Entry &item = i->item ();

      if (item.servant_ == servant)
        {
          if (id.ptr () != 0)
            {
              // More than one match return -1.
              delete id.ptr ();
              return -1;
            }
          // Store the match....
          ACE_NEW_RETURN (id.ptr (),
                          PortableServer::ObjectId (item.id_),
                          -1);
        }
    }

  return id.ptr () == 0 ? -1 : 0;
}

int
TAO_Object_Table_Impl::find (const PortableServer::Servant servant)
{
  PortableServer::ObjectId *id;
  PortableServer::ObjectId_out id_out (id);

  if (this->find (servant, id_out) == -1)
    return -1;

  // It was found and returned in <id>, we must release it.
  delete id;
  return 0;
}

// Template specialization....
u_long
ACE_Hash_Map_Manager<PortableServer::ObjectId, PortableServer::Servant, ACE_SYNCH_NULL_MUTEX>::hash (const PortableServer::ObjectId &id)
{
  // Based on hash_pjw function on the ACE library.
  u_long hash = 0;

  for (CORBA::ULong i = 0;
       i < id.length ();
       ++i)
    {
      hash = (hash << 4) + (id[i] * 13);

      u_long g = hash & 0xf0000000;

      if (g)
        {
          hash ^= (g >> 24);
          hash ^= g;
        }
    }

  return hash;
}

int
TAO_Dynamic_Hash_ObjTable::find (const PortableServer::ObjectId &id,
                                 PortableServer::Servant &servant)
{
  return this->hash_map_.find (id, servant);
}

int
TAO_Dynamic_Hash_ObjTable::bind (const PortableServer::ObjectId &id,
                                 PortableServer::Servant servant)
{
  return this->hash_map_.bind (id, servant);
}

int
TAO_Dynamic_Hash_ObjTable::unbind (const PortableServer::ObjectId &id,
                                   PortableServer::Servant &servant)
{
  return this->hash_map_.unbind (id, servant);
}

TAO_Dynamic_Hash_ObjTable::TAO_Dynamic_Hash_ObjTable (CORBA::ULong size)
  : hash_map_ (size == 0 ?
      ACE_static_cast (size_t, TAO_Object_Table_Impl::DEFAULT_TABLE_SIZE) :
      size)
{
}

TAO_Linear_ObjTable::TAO_Linear_ObjTable (CORBA::ULong size)
  : next_ (0),
    tablesize_ (size == 0 ?
      ACE_static_cast (size_t, TAO_Object_Table_Impl::DEFAULT_TABLE_SIZE) :
      size)
{
  ACE_NEW (table_, TAO_Object_Table_Entry[this->tablesize_]);
}

// Active Demux search strategy
// constructor

TAO_Active_Demux_ObjTable::TAO_Active_Demux_ObjTable (CORBA::ULong size)
  : TAO_Linear_ObjTable (size)
{
}

int
TAO_Linear_ObjTable::bind (const PortableServer::ObjectId &id,
                           PortableServer::Servant servant)
{
  // Check existing entries
  for (TAO_Object_Table_Entry *i = this->table_;
       i != this->table_ + this->next_;
       ++i)
    {
      if (i->is_free_)
        {
          i->id_ = id;
          i->servant_ = servant;
          i->is_free_ = 0;
          return 0;
        }
    }

  // Resize
  if (this->next_ == this->tablesize_)
    {
      int result = this->resize ();
      if (result != 0)
        return result;
    }

  // Put the entry at the end of the new section
  this->table_[this->next_].id_ = id;
  this->table_[this->next_].servant_ = servant;
  this->table_[this->next_].is_free_ = 0;

  // Increment next
  this->next_++;

  return 0;
}

int
TAO_Linear_ObjTable::find (const PortableServer::ObjectId &id,
                           PortableServer::Servant &servant)
{
  for (TAO_Object_Table_Entry *i = this->table_;
       i != this->table_ + this->next_;
       ++i)
    {
      if (i->is_free_)
        continue;
      else if (i->id_ == id)
        {
          servant = i->servant_;
          return 0;
        }
    }
  return -1;
}

int
TAO_Linear_ObjTable::unbind (const PortableServer::ObjectId &id,
                             PortableServer::Servant &servant)
{
  for (TAO_Object_Table_Entry *i = this->table_;
       i != this->table_ + this->next_;
       ++i)
    {
      if (i->is_free_)
        continue;
      else if (i->id_ == id)
        {
          servant = i->servant_;
          i->is_free_ = 1;
          return 0;
        }
    }
  return -1;
}

int
TAO_Linear_ObjTable::resize (void)
{
  if (this->tablesize_ < TAO_Linear_ObjTable::MAX_EXPONENTIAL)
    this->tablesize_ *= 2;
  else
    this->tablesize_ += TAO_Linear_ObjTable::LINEAR_INCREASE;

  TAO_Object_Table_Entry *tmp;
  ACE_NEW_RETURN (tmp,
                  TAO_Object_Table_Entry[this->tablesize_],
                  -1);

  // Copy old stuff
  for (TAO_Object_Table_Entry *i = this->table_, *j = tmp;
       i != this->table_ + this->next_;
       ++i, ++j)
    *j = *i;

  delete [] this->table_;

  this->table_ = tmp;
  return 0;
}

int
TAO_Active_Demux_ObjTable::bind (const PortableServer::ObjectId &id,
                                 PortableServer::Servant servant)
{
  CORBA::ULong index = 0;
  CORBA::ULong generation = 0;
  int result = this->parse_object_id (id, index, generation);

  if (result != 0 ||
      index > this->tablesize_ ||
      this->table_[index].generation_ != generation)
    return -1;

  ACE_ASSERT (this->table_[index].is_free_ == 0);
  this->table_[index].servant_ = servant;

  return 0;
}

int
TAO_Active_Demux_ObjTable::find (const PortableServer::ObjectId &id,
                                 PortableServer::Servant &servant)
{
  CORBA::ULong index = 0;
  CORBA::ULong generation = 0;
  int result = this->parse_object_id (id, index, generation);

  if (result != 0 ||
      index > this->tablesize_ ||
      this->table_[index].generation_ != generation ||
      this->table_[index].is_free_ != 0)
    return -1;

  servant = this->table_[index].servant_;

  return 0;
}

int
TAO_Active_Demux_ObjTable::unbind (const PortableServer::ObjectId &id,
                                   PortableServer::Servant &servant)
{
  CORBA::ULong index = 0;
  CORBA::ULong generation = 0;
  int result = this->parse_object_id (id, index, generation);

  if (result != 0
      || index > this->tablesize_
      || this->table_[index].generation_ != generation)
    return -1;

  servant = this->table_[index].servant_;
  this->table_[index].is_free_ = 1;

  return 0;
}

PortableServer::ObjectId *
TAO_Active_Demux_ObjTable::create_object_id (PortableServer::Servant servant,
                                             CORBA::Environment &env)
{
  // This method assumes that locks are held when it is called
  CORBA::ULong id_data[2];
  CORBA::ULong index = this->next_free ();
  id_data[TAO_Active_Demux_ObjTable::INDEX_FIELD] = index;

  // Increment generation count.
  id_data[TAO_Active_Demux_ObjTable::GENERATION_FIELD] =
    ++this->table_[index].generation_;

  // Move next along if index is not reused
  if (index == this->next_)
    this->next_++;

  PortableServer::ObjectId *id;
  CORBA::ULong size = 2 * sizeof (CORBA::ULong);
  ACE_NEW_RETURN (id,
                  PortableServer::ObjectId (size),
                  0);

  id->length (size);

  ACE_OS::memcpy (id->get_buffer (),
                  &id_data,
                  size);

  // Set the new values
  this->table_[index].id_ = *id;
  this->table_[index].servant_ = servant;
  this->table_[index].is_free_ = 0;

  return id;
}

CORBA::ULong
TAO_Active_Demux_ObjTable::next_free (void)
{
  for (;;)
    {
      for (TAO_Object_Table_Entry *i = this->table_;
           i != this->table_ + this->tablesize_;
           ++i)
        if (i->is_free_)
          return i - this->table_;

      this->resize ();
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Iterator_Base<PortableServer::ObjectId, PortableServer::Servant, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator<PortableServer::ObjectId, PortableServer::Servant, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<PortableServer::ObjectId, PortableServer::Servant, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Manager<PortableServer::ObjectId, PortableServer::Servant, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Hash_Map_Entry<PortableServer::ObjectId, PortableServer::Servant>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Iterator_Base<PortableServer::ObjectId, PortableServer::Servant, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<PortableServer::ObjectId, PortableServer::Servant, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<PortableServer::ObjectId, PortableServer::Servant, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager<PortableServer::ObjectId, PortableServer::Servant, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Entry<PortableServer::ObjectId, PortableServer::Servant>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
