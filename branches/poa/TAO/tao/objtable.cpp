//
// $Id$
//

#include "ace/Auto_Ptr.h"

#include "tao/corba.h"
#include "tao/objtable.h"

TAO_Object_Table_Iterator_Impl::~TAO_Object_Table_Iterator_Impl (void)
{
}

TAO_Object_Table_Impl::~TAO_Object_Table_Impl (void)
{
}

int TAO_Object_Table_Impl::find (const PortableServer::Servant servant)
{
  PortableServer::ObjectId* id;
  int ret = this->find (servant, id);
  if (ret == -1)
    return -1;
  
  // It was found and returned in <id>, we must release it.
  delete id;
  return 0;
}

int TAO_Object_Table_Impl::find (const PortableServer::ObjectId &id)
{
  PortableServer::Servant servant;
  return this->find (id, servant);
}

int TAO_Object_Table_Impl::find (const PortableServer::Servant servant,
				 PortableServer::ObjectId_out id)
{
  auto_ptr<PortableServer::ObjectId> found;
  auto_ptr<TAO_Object_Table_Iterator_Impl> end = this->end ();
  for (TAO_Object_Table_Iterator_Impl *i = this->begin ();
       !i->done (end.get ());
       i->advance ())
    {
      const TAO_Object_Table_Entry& item = i->item ();
      if (item.int_id_ == servant)
	{
	  if (found.get () != 0)
	    {
	      // More than one match return -1.
	      found = 0;
	      return -1;
	    }
	  // Store the match....
	  found = new PortableServer::ObjectId (item.ext_id_);
	}
    }
  id = found.release ();
  return (id==0)?-1:0;
}



TAO_Object_Table_Iterator::
TAO_Object_Table_Iterator (TAO_Object_Table_Iterator_Impl *impl)
  :  impl_ (impl)
{
}

TAO_Object_Table_Iterator::
TAO_Object_Table_Iterator (const TAO_Object_Table_Iterator &x)
  :  impl_ (0)
{
  if (x.impl_ != 0)
    {
      this->impl_ = x.impl_->clone ();
    }
}

TAO_Object_Table_Iterator&
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

TAO_Object_Table_Iterator::~TAO_Object_Table_Iterator (void)
{
  if (this->impl_ != 0)
    {
      delete this->impl_;
      this->impl_ = 0;
    }
}

const TAO_Object_Table_Entry&
TAO_Object_Table_Iterator::operator* (void) const
{
  return this->impl_->item ();
}

TAO_Object_Table_Iterator
TAO_Object_Table_Iterator::operator++ (void)
{
  TAO_Object_Table_Iterator tmp = *this;
  this->impl_->advance ();
  return tmp;
}

TAO_Object_Table_Iterator
TAO_Object_Table_Iterator::operator++ (int)
{
  this->impl_->advance ();
  return *this;
}

int operator== (const TAO_Object_Table_Iterator &l,
		const TAO_Object_Table_Iterator &r)
{
  return l.impl_->done (r.impl_);
}

int operator!= (const TAO_Object_Table_Iterator &l,
		const TAO_Object_Table_Iterator &r)
{
  return !(l == r);
}



TAO_Object_Table::TAO_Object_Table (void)
{
  this->impl_ = TAO_ORB_Core_instance ()->server_factory ()->create_object_table ();
}
  
TAO_Object_Table::~TAO_Object_Table (void)
{
  delete this->impl_;
}

int
TAO_Object_Table::find (const PortableServer::ObjectId &id, 
			PortableServer::Servant &servant)
{
  return this->impl_->find (id, servant);
}

int
TAO_Object_Table::bind (const PortableServer::ObjectId &id, 
			PortableServer::Servant servant)
{
  return this->impl_->bind (id, servant);
}

int
TAO_Object_Table::unbind (const PortableServer::ObjectId &id,
			  PortableServer::Servant &servant)
{
  return this->impl_->unbind (id, servant);
}

int
TAO_Object_Table::find (const PortableServer::Servant servant)
{
  return this->impl_->find (servant);
}

int
TAO_Object_Table::find (const PortableServer::ObjectId &id)
{
  return this->impl_->find (id);
}

int
TAO_Object_Table::find (const PortableServer::Servant servant,
			PortableServer::ObjectId_out id)
{
  return this->impl_->find (servant, id);
}

TAO_Object_Table::iterator
TAO_Object_Table::begin (void) const
{
  return TAO_Object_Table::iterator (this->impl_->begin ());
}

TAO_Object_Table::iterator
TAO_Object_Table::end (void) const
{
  return TAO_Object_Table::iterator (this->impl_->end ());
}



int
operator== (const PortableServer::ObjectId &l,
	    const PortableServer::ObjectId &r)
{
  if (l.length () != r.length ())
    return 0;

  for (CORBA::ULong i = 0;
       i < l.length ();
       ++i)
    {
      if (l[i] != r[i])
	return 0;
    }
  return 1;
}

// Template specialization....
u_long
ACE_Hash_Map_Manager<PortableServer::ObjectId, PortableServer::Servant, ACE_SYNCH_RW_MUTEX>::
hash (const PortableServer::ObjectId &ext_id)
{
  // Based on hash_pjw function on the ACE library.
  u_long hash = 0;

  for (CORBA::ULong i = 0;
       i < ext_id.length ();
       ++i)
    {
      hash = (hash << 4) + (ext_id[i] * 13);

      u_long g = hash & 0xf0000000;

      if (g)
        {
          hash ^= (g >> 24);
          hash ^= g;
        }
    }

  return hash;
}

TAO_Dynamic_Hash_ObjTable::TAO_Dynamic_Hash_ObjTable (CORBA::ULong size)
  :  hash_map_ (size)
{
}

int TAO_Dynamic_Hash_ObjTable::find (const PortableServer::ObjectId &id,
				     PortableServer::Servant &servant)
{
  return this->hash_map_.find (id, servant);
}

int TAO_Dynamic_Hash_ObjTable::bind (const PortableServer::ObjectId &id,
				     PortableServer::Servant servant)
{
  return this->hash_map_.bind (id, servant);
}

int TAO_Dynamic_Hash_ObjTable::unbind (const PortableServer::ObjectId &id,
				       PortableServer::Servant &servant)
{
  return this->hash_map_.unbind (id, servant);
}

TAO_Object_Table_Iterator_Impl*
TAO_Dynamic_Hash_ObjTable::begin (void) const
{
  TAO_Dynamic_Hash_ObjTable *non_const = 
    ACE_const_cast(TAO_Dynamic_Hash_ObjTable*, this);
  return new TAO_Dynamic_Hash_ObjTable_Iterator (Iterator (non_const->hash_map_));
}

TAO_Object_Table_Iterator_Impl*
TAO_Dynamic_Hash_ObjTable::end (void) const
{
  return 0;
}

TAO_Dynamic_Hash_ObjTable_Iterator::
TAO_Dynamic_Hash_ObjTable_Iterator (const Impl& impl)
  :  impl_ (impl)
{
}

TAO_Object_Table_Iterator_Impl*
TAO_Dynamic_Hash_ObjTable_Iterator::clone (void) const
{
  return new TAO_Dynamic_Hash_ObjTable_Iterator (*this);
}

const TAO_Object_Table_Entry&
TAO_Dynamic_Hash_ObjTable_Iterator::item (void) const
{
  static TAO_Object_Table_Entry entry;

  ACE_Hash_Map_Entry<PortableServer::ObjectId,PortableServer::Servant>* tmp;
  if (ACE_const_cast(TAO_Dynamic_Hash_ObjTable_Iterator*,this)->impl_.next (tmp) == 1)
    {
      entry.int_id_ = tmp->int_id_;
      entry.ext_id_ = tmp->ext_id_;
    }
  return entry;
}

void
TAO_Dynamic_Hash_ObjTable_Iterator::advance (void)
{
  this->impl_.advance ();
}

int
TAO_Dynamic_Hash_ObjTable_Iterator::done (const TAO_Object_Table_Iterator_Impl *) const
{
  return this->impl_.done ();
}



TAO_Array_ObjTable_Iterator::
TAO_Array_ObjTable_Iterator (TAO_Object_Table_Entry *pos)
  :  pos_ (pos)
{
}

TAO_Object_Table_Iterator_Impl *
TAO_Array_ObjTable_Iterator::clone (void) const
{
  return new TAO_Array_ObjTable_Iterator (*this);
}

const TAO_Object_Table_Entry&
TAO_Array_ObjTable_Iterator::item (void) const
{
  return *this->pos_;
}

void
TAO_Array_ObjTable_Iterator::advance (void)
{
  this->pos_++;
}

int
TAO_Array_ObjTable_Iterator::done (const TAO_Object_Table_Iterator_Impl *end) const
{
  TAO_Array_ObjTable_Iterator *tmp =
    ACE_dynamic_cast(TAO_Array_ObjTable_Iterator*, end);
  return (this->pos_ == tmp->pos_);
}



TAO_Linear_ObjTable::
TAO_Linear_ObjTable (CORBA::ULong size)
  :  next_ (0),
     tablesize_ (0),
     table_ (0)
{
}

TAO_Linear_ObjTable::~TAO_Linear_ObjTable (void)
{
  if (this->table_ != 0)
    {
      delete[] this->table_;
      this->table_ = 0;
    }
}

int
TAO_Linear_ObjTable::find (const PortableServer::ObjectId &id, 
			   PortableServer::Servant &servant)
{
  for (TAO_Object_Table_Entry *i = this->table_;
       i != this->table_ + this->next_;
       ++i)
    {
      if ((*i).ext_id_ == id)
	{
	  servant = (*i).int_id_;
	  return 0;
	}
    }
  return -1;
}

const int start_tblsiz = 128;
const int max_exp = 65536; // Grow table exponentially up to 64K
const int lin_chunk = 32768; // afterwards grow in chunks of 32K

int
TAO_Linear_ObjTable::bind (const PortableServer::ObjectId &id, 
			   PortableServer::Servant servant)
{
  for (TAO_Object_Table_Entry *i = this->table_;
       i != this->table_ + this->next_;
       ++i)
    {
      if ((*i).ext_id_ == id || (*i).int_id_ == 0)
	{
	  (*i).ext_id_ = id;
	  (*i).int_id_ = servant;
	  return 0;
	}
    }
  if (this->next_ == this->tablesize_)
    {
      if (this->next_ == 0)
	{
	  this->tablesize_ = start_tblsiz;
	  ACE_NEW_RETURN (this->table_,
			  TAO_Object_Table_Entry[this->tablesize_],
			  -1);
	}
      else
	{
	  if (this->tablesize_ < max_exp)
	    {
	      this->tablesize_ *= 2;
	    }
	  else
	    {
	      this->tablesize_ += lin_chunk;
	    }
	  TAO_Object_Table_Entry *tmp;
	  ACE_NEW_RETURN (tmp,
			  TAO_Object_Table_Entry[this->tablesize_],
			  -1);
	  for (TAO_Object_Table_Entry *i = this->table_, *j = tmp;
	       i != this->table_ + this->next_;
	       ++i, ++j)
	    {
	      *j = *i;
	    }
	  delete[] this->table_;
	  this->table_ = tmp;
	}
    }
  this->table_[this->next_].ext_id_ = id;
  this->table_[this->next_].int_id_ = servant;
  this->next_++;
  return 0;
}

int
TAO_Linear_ObjTable::unbind (const PortableServer::ObjectId &id,
			     PortableServer::Servant &servant)
{
  for (TAO_Object_Table_Entry *i = this->table_;
       i != this->table_ + this->next_;
       ++i)
    {
      if ((*i).ext_id_ == id)
	{
	  servant = (*i).int_id_;
	  (*i).int_id_ = 0;
	  return 0;
	}
    }
  return -1;
}

TAO_Object_Table_Iterator_Impl*
TAO_Linear_ObjTable::begin () const
{
  return new TAO_Array_ObjTable_Iterator (this->table_);
}

TAO_Object_Table_Iterator_Impl*
TAO_Linear_ObjTable::end () const
{
  return new TAO_Array_ObjTable_Iterator (this->table_ + this->next_);
}



// Active Demux search strategy
// constructor
TAO_Active_Demux_ObjTable::TAO_Active_Demux_ObjTable (CORBA::ULong size)
  : tablesize_ (size)
{
  ACE_NEW (this->table_, TAO_Object_Table_Entry[size]);
  // @@ Maybe a proper constructor for TAO_Object_Table_Entry will
  // solve this more cleanly.
  for (TAO_Object_Table_Entry *i = this->table_;
       i != this->table_ + this->tablesize_;
       ++i)
    {
      (*i).int_id_ = 0;
    }
}

// destructor
TAO_Active_Demux_ObjTable::~TAO_Active_Demux_ObjTable ()
{
  delete [] this->table_;
}

int
TAO_Active_Demux_ObjTable::index_from_id (const PortableServer::ObjectId &id) const
{
  // @@ TODO parse id an obtain the index, maybe write a "index" to id
  // function or some method to obtain the next "free" id.
  return 0;
}


int
TAO_Active_Demux_ObjTable::next_free (void) const
{
  for (TAO_Object_Table_Entry *i = this->table_;
       i != this->table_ + this->tablesize_;
       ++i)
    {
      if ((*i).int_id_ == 0)
	{
	  return (i - this->table_);
	}
    }
  return -1;
}

int
TAO_Active_Demux_ObjTable::find (const PortableServer::ObjectId &id, 
				 PortableServer::Servant &servant)
{
  int index = this->index_from_id (id);
  if (index < 0 || index > this->tablesize_)
    {
      return -1;
    }
  servant = this->table_[index].int_id_;
  return 0;
}

int
TAO_Active_Demux_ObjTable::bind (const PortableServer::ObjectId &id, 
				 PortableServer::Servant servant)
{
  int index = this->index_from_id (id);
  if (index < 0 || index > this->tablesize_)
    {
      return -1;
    }
  this->table_[index].ext_id_ = id;
  this->table_[index].int_id_ = servant;
  return 0;
}

int
TAO_Active_Demux_ObjTable::unbind (const PortableServer::ObjectId &id,
				   PortableServer::Servant &servant)
{
  int index = this->index_from_id (id);
  if (index < 0 || index > this->tablesize_)
    {
      return -1;
    }
  servant = this->table_[index].int_id_;
  this->table_[index].int_id_ = 0;
  return 0;
}

TAO_Object_Table_Iterator_Impl*
TAO_Active_Demux_ObjTable::begin () const
{
  return new TAO_Array_ObjTable_Iterator (this->table_);
}

TAO_Object_Table_Iterator_Impl*
TAO_Active_Demux_ObjTable::end () const
{
  return new TAO_Array_ObjTable_Iterator (this->table_ + this->tablesize_);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Iterator<char const*, PortableServer::Servant, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Manager<char const*, PortableServer::Servant, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Entry<char const*, PortableServer::Servant>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Iterator<char const*, PortableServer::Servant, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager<char const*, PortableServer::Servant, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Entry<char const*, PortableServer::Servant>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
