// $Id$

#include "ace/Auto_Ptr.h"
#include "tao/Active_Object_Map.h"
#include "tao/ORB_Core.h"
#include "tao/POA.h"
#include "tao/Server_Strategy_Factory.h"

ACE_RCSID(tao, Active_Object_Map, "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/Active_Object_Map.i"
#endif /* ! __ACE_INLINE__ */

u_long 
TAO_ObjectId_Hash::operator () (const PortableServer::ObjectId &id) const
{
  return ACE::hash_pjw ((const char *) id.get_buffer (),
                        id.length ());
}

TAO_Active_Object_Map::TAO_Active_Object_Map (int user_id_policy,
                                              int unique_id_policy)
  : unique_id_policy_ (unique_id_policy)
{
  this->impl_ = 
    TAO_ORB_Core_instance ()->server_factory ()->create_active_object_map (user_id_policy);
  this->reverse_impl_ = 
    TAO_ORB_Core_instance ()->server_factory ()->create_reverse_active_object_map (unique_id_policy);
}

int
TAO_Active_Object_Map_Impl::find (const PortableServer::Servant servant,
                                  PortableServer::ObjectId &id)
{
  auto_ptr<TAO_Active_Object_Map_Iterator_Impl> end (this->end ());

  for (auto_ptr<TAO_Active_Object_Map_Iterator_Impl> i (this->begin ());
       !i->done (end.get ());
       i->advance ())
    {
      const TAO_Active_Object_Map_Entry &item = i->item ();

      if (!this->is_free (item))
        {
          if (item.servant_ == servant)
            {
              // Store the match....
              id = item.id_;
              return 0;
            }
        }
    }
  
  return -1;
}

PortableServer::ObjectId *
TAO_Dynamic_Hash_Active_Object_Map::create_object_id (PortableServer::Servant,
                                                      CORBA::Environment &)
{
  // This method assumes that locks are held when it is called
  PortableServer::ObjectId *id;
  CORBA::ULong size = sizeof (CORBA::ULong);
  ACE_NEW_RETURN (id,
                  PortableServer::ObjectId (size),
                  0);

  id->length (size);

  ACE_OS::memcpy (id->get_buffer (),
                  &this->counter_,
                  size);

  this->counter_++;

  return id;  
}

int
TAO_Linear_Active_Object_Map::bind (const PortableServer::ObjectId &id,
                                    PortableServer::Servant servant)
{
  // Check existing entries
  for (TAO_Active_Object_Map_Entry *i = this->map_;
       i != this->map_ + this->next_;
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
  if (this->next_ == this->mapsize_)
    {
      int result = this->resize ();
      if (result != 0)
        return result;
    }

  // Put the entry at the end of the new section
  this->map_[this->next_].id_ = id;
  this->map_[this->next_].servant_ = servant;
  this->map_[this->next_].is_free_ = 0;

  // Increment next
  this->next_++;

  return 0;
}

int
TAO_Linear_Active_Object_Map::find (const PortableServer::ObjectId &id,
                                    PortableServer::Servant &servant)
{
  for (TAO_Active_Object_Map_Entry *i = this->map_;
       i != this->map_ + this->next_;
       ++i)
    if (i->is_free_)
      continue;
    else if (i->id_ == id)
      {
        servant = i->servant_;
        return 0;
      }

  return -1;
}

int
TAO_Linear_Active_Object_Map::unbind (const PortableServer::ObjectId &id,
                                      PortableServer::Servant &servant)
{
  for (TAO_Active_Object_Map_Entry *i = this->map_;
       i != this->map_ + this->next_;
       ++i)
    if (i->is_free_)
      continue;
    else if (i->id_ == id)
      {
        servant = i->servant_;
        i->is_free_ = 1;
        return 0;
      }

  return -1;
}

int
TAO_Linear_Active_Object_Map::resize (void)
{
  if (this->mapsize_ < TAO_Linear_Active_Object_Map::MAX_EXPONENTIAL)
    this->mapsize_ *= 2;
  else
    this->mapsize_ += TAO_Linear_Active_Object_Map::LINEAR_INCREASE;

  TAO_Active_Object_Map_Entry *tmp;
  ACE_NEW_RETURN (tmp,
                  TAO_Active_Object_Map_Entry[this->mapsize_],
                  -1);

  // Copy old stuff
  for (TAO_Active_Object_Map_Entry *i = this->map_, *j = tmp;
       i != this->map_ + this->next_;
       ++i, ++j)
    *j = *i;

  delete [] this->map_;

  this->map_ = tmp;
  return 0;
}

PortableServer::ObjectId *
TAO_Linear_Active_Object_Map::create_object_id (PortableServer::Servant,
                                                CORBA::Environment &)
{
  PortableServer::ObjectId *id;
  CORBA::ULong size = sizeof (CORBA::ULong);
  ACE_NEW_RETURN (id,
                  PortableServer::ObjectId (size),
                  0);

  id->length (size);

  ACE_OS::memcpy (id->get_buffer (),
                  &this->counter_,
                  size);

  this->counter_++;
  
  return id;  
}

int
TAO_Active_Demux_Active_Object_Map::bind (const PortableServer::ObjectId &id,
                                          PortableServer::Servant servant)
{
  CORBA::ULong index = 0;
  CORBA::ULong generation = 0;
  int result = this->parse_object_id (id, index, generation);

  if (result != 0 ||
      index > this->mapsize_ ||
      this->map_[index].generation_ != generation ||
      this->map_[index].is_free_ != 0)
    return -1;

  this->map_[index].servant_ = servant;

  return 0;
}

int
TAO_Active_Demux_Active_Object_Map::find (const PortableServer::ObjectId &id,
                                          PortableServer::Servant &servant)
{
  CORBA::ULong index = 0;
  CORBA::ULong generation = 0;
  int result = this->parse_object_id (id, index, generation);

  if (result != 0 
      || index > this->mapsize_ 
      || this->map_[index].generation_ != generation 
      || this->map_[index].is_free_ != 0)
    return -1;

  servant = this->map_[index].servant_;

  return 0;
}

int
TAO_Active_Demux_Active_Object_Map::unbind (const PortableServer::ObjectId &id,
                                            PortableServer::Servant &servant)
{
  CORBA::ULong index = 0;
  CORBA::ULong generation = 0;
  int result = this->parse_object_id (id, index, generation);

  if (result != 0 || 
      index > this->mapsize_ || 
      this->map_[index].generation_ != generation ||
      this->map_[index].is_free_ != 0)
    return -1;

  servant = this->map_[index].servant_;
  this->map_[index].is_free_ = 1;

  return 0;
}

PortableServer::ObjectId *
TAO_Active_Demux_Active_Object_Map::create_object_id (PortableServer::Servant servant,
                                                      CORBA::Environment &)
{
  // This method assumes that locks are held when it is called
  CORBA::ULong id_data[2];
  CORBA::ULong index = this->next_free ();
  id_data[TAO_Active_Demux_Active_Object_Map::INDEX_FIELD] = index;

  // Increment generation count.
  id_data[TAO_Active_Demux_Active_Object_Map::GENERATION_FIELD] =
    ++this->map_[index].generation_;

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
  this->map_[index].id_ = *id;
  this->map_[index].servant_ = servant;
  this->map_[index].is_free_ = 0;

  return id;
}

CORBA::ULong
TAO_Active_Demux_Active_Object_Map::next_free (void)
{
  for (;;)
    {
      for (TAO_Active_Object_Map_Entry *i = this->map_;
           i != this->map_ + this->mapsize_;
           ++i)
        if (i->is_free_)
          return i - this->map_;

      this->resize ();
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Iterator_Base_Ex<PortableServer::ObjectId, PortableServer::Servant, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<PortableServer::ObjectId, PortableServer::Servant, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<PortableServer::ObjectId, PortableServer::Servant, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId, PortableServer::Servant, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<PortableServer::ObjectId, PortableServer::Servant>;
template class ACE_Equal_To<PortableServer::ObjectId>;

template class ACE_Hash_Map_Iterator_Base_Ex<PortableServer::Servant, PortableServer::ObjectId, TAO_Servant_Hash, ACE_Equal_To<PortableServer::Servant>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<PortableServer::Servant, PortableServer::ObjectId, TAO_Servant_Hash, ACE_Equal_To<PortableServer::Servant>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<PortableServer::Servant, PortableServer::ObjectId, TAO_Servant_Hash, ACE_Equal_To<PortableServer::Servant>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<PortableServer::Servant, PortableServer::ObjectId, TAO_Servant_Hash, ACE_Equal_To<PortableServer::Servant>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<PortableServer::Servant, PortableServer::ObjectId>;
template class ACE_Equal_To<PortableServer::Servant>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<PortableServer::ObjectId, PortableServer::Servant, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<PortableServer::ObjectId, PortableServer::Servant, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<PortableServer::ObjectId, PortableServer::Servant, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId, PortableServer::Servant, TAO_ObjectId_Hash, ACE_Equal_To<PortableServer::ObjectId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<PortableServer::ObjectId, PortableServer::Servant>
#pragma instantiate ACE_Equal_To<PortableServer::ObjectId>

#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<PortableServer::Servant, PortableServer::ObjectId, TAO_Servant_Hash, ACE_Equal_To<PortableServer::Servant>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<PortableServer::Servant, PortableServer::ObjectId, TAO_Servant_Hash, ACE_Equal_To<PortableServer::Servant>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<PortableServer::Servant, PortableServer::ObjectId, TAO_Servant_Hash, ACE_Equal_To<PortableServer::Servant>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<PortableServer::Servant, PortableServer::ObjectId, TAO_Servant_Hash, ACE_Equal_To<PortableServer::Servant>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<PortableServer::Servant, PortableServer::ObjectId>
#pragma instantiate ACE_Equal_To<PortableServer::Servant>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
