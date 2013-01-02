// -*- C++ -*-

//=============================================================================
/**
 * @file  PG_Group_List_Store.cpp
 *
 * $Id$
 *
 * @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================


#include "orbsvcs/PortableGroup/PG_Group_List_Store.h"

#include "tao/Storable_Base.h"
#include "tao/Storable_Factory.h"
#include "tao/Storable_File_Guard.h"

#include <algorithm>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class PG_Group_List_Store_File_Guard : public TAO::Storable_File_Guard
  {
  public:
    PG_Group_List_Store_File_Guard ( PG_Group_List_Store & list_store, const char * mode);

    ~PG_Group_List_Store_File_Guard ();

  virtual void set_object_last_changed (const time_t & time);

  virtual time_t get_object_last_changed ();

  virtual void load_from_stream ();

  virtual bool is_loaded_from_stream ();

  virtual TAO::Storable_Base * create_stream (const char * mode);

private:

    PG_Group_List_Store & list_store_;
  };
}

TAO::PG_Group_List_Store_File_Guard::PG_Group_List_Store_File_Guard (PG_Group_List_Store & list_store, const char * mode)
  : TAO::Storable_File_Guard(true)
  , list_store_(list_store)
{
  this->init(mode);
}

TAO::PG_Group_List_Store_File_Guard::~PG_Group_List_Store_File_Guard ()
{
  this->release ();
}

void
TAO::PG_Group_List_Store_File_Guard::set_object_last_changed (const time_t & time)
{
  list_store_.last_changed_ = time;
}

time_t
TAO::PG_Group_List_Store_File_Guard::get_object_last_changed ()
{
  return list_store_.last_changed_;
}

void
TAO::PG_Group_List_Store_File_Guard::load_from_stream ()
{
  list_store_.read (this->peer ());
  list_store_.loaded_from_stream_ = true;
  this->peer ().rewind ();
}

bool
TAO::PG_Group_List_Store_File_Guard::is_loaded_from_stream ()
{
  return list_store_.loaded_from_stream_;
}

TAO::Storable_Base *
TAO::PG_Group_List_Store_File_Guard::create_stream (const char * mode)
{
  return list_store_.storable_factory_.create_stream (list_store_.file_name_, mode);
}

typedef TAO::PG_Group_List_Store_File_Guard File_Guard;

/* static */
ACE_CString TAO::PG_Group_List_Store::file_name_ = "ObjectGroup_global";

TAO::PG_Group_List_Store::PG_Group_List_Store (Storable_Factory & storable_factory)
  : next_group_id_ (0)
  , storable_factory_ (storable_factory)
  , loaded_from_stream_ (false)
  , last_changed_ (0)
{
  // Create a temporary stream simply to check if a readable
  // version already exists.
  bool stream_exists = false;
  {
    ACE_Auto_Ptr<TAO::Storable_Base> stream (this->create_stream ("r"));
    if (stream->exists ())
      stream_exists = true;
  }

  if (stream_exists)
    {
      File_Guard fg(*this, "r");
    }
  else
    {
      File_Guard fg(*this, "wc");
      this->write (fg.peer ());
    }
}

TAO::PG_Group_List_Store::~PG_Group_List_Store ()
{
}

PortableGroup::ObjectGroupId
TAO::PG_Group_List_Store::get_next_group_id ()
{
  File_Guard fg(*this, "rw");
  PortableGroup::ObjectGroupId next_id = this->next_group_id_;
  ++this->next_group_id_;
  this->write (fg.peer ());
  return next_id;
}

int
TAO::PG_Group_List_Store::add (PortableGroup::ObjectGroupId id)
{
  File_Guard fg(*this, "rw");
  Group_Id_Const_Iterator it = std::find (this->group_ids_.begin (),
                                          this->group_ids_.end (),
                                          id);
  if (it != this->group_ids_.end())
    return -1;
  this->group_ids_.insert (id);
  this->write (fg.peer ());
  return 0;
}

int
TAO::PG_Group_List_Store::remove (PortableGroup::ObjectGroupId id)
{
  File_Guard fg(*this, "rw");
  Group_Id_Iterator it = std::find (this->group_ids_.begin (),
                                    this->group_ids_.end (),
                                    id);
  if (it == this->group_ids_.end ())
    return -1;
  this->group_ids_.erase (it);
  this->write (fg.peer ());
  return 0;
}

TAO::PG_Group_List_Store::Group_Ids &
TAO::PG_Group_List_Store::get_group_ids ()
{
  File_Guard fg(*this, "r");
  return group_ids_;
}

void
TAO::PG_Group_List_Store::read (TAO::Storable_Base & stream)
{
  group_ids_.clear ();

  stream.rewind ();
  if (!stream.good ())
    throw CORBA::INTERNAL ();

  unsigned int next_group_id;
  stream >> next_group_id;
  this->next_group_id_ = next_group_id;
  if (!stream.good ())
    throw CORBA::INTERNAL ();

  int size;
  stream >> size;
  if (!stream.good ())
    throw CORBA::INTERNAL ();

  // TODO: Look at adding streaming of unsigned long long
  // PortableGroup::ObjectGroupId group_id;
  int group_id;
  for (int i = 0; i < size; ++i)
    {
      stream >> group_id;
      if (!stream.good ())
        throw CORBA::INTERNAL ();
      group_ids_.insert (group_id);
    }
}

void
TAO::PG_Group_List_Store::write (TAO::Storable_Base & stream)
{
  stream.rewind ();

  unsigned int next_group_id = this->next_group_id_;
  stream << next_group_id;

  int size = group_ids_.size ();
  stream << size;
  for (Group_Id_Const_Iterator it = group_ids_.begin (); it != group_ids_.end (); ++it)
    {
      int group_id = static_cast<int> (*it);
      stream << group_id;
    }

  stream.flush ();
}

TAO::Storable_Base *
TAO::PG_Group_List_Store::create_stream (const char * mode)
{
  return this->storable_factory_.create_stream (this->file_name_, mode);
}

bool
TAO::PG_Group_List_Store::list_obsolete ()
{
  // TODO: Upate if obsolete flag is set based on CORBA call.
  ACE_Auto_Ptr<TAO::Storable_Base> stream (this->create_stream ("r"));
  if (!stream->exists ())
    throw CORBA::INTERNAL ();
  stream->open ();
  return stream->last_changed () > this->last_changed_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
