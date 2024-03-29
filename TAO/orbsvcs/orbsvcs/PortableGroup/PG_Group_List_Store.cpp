// -*- C++ -*-

//=============================================================================
/**
 * @file  PG_Group_List_Store.cpp
 *
 * @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================


#include "orbsvcs/PortableGroup/PG_Group_List_Store.h"

#include "tao/Storable_Base.h"
#include "tao/Storable_Factory.h"
#include "tao/Storable_File_Guard.h"
#include <algorithm>
#include <memory>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class PG_Group_List_Store_File_Guard : public TAO::Storable_File_Guard
  {
  public:
    PG_Group_List_Store_File_Guard (PG_Group_List_Store & list_store,
                                    Method_Type method_type);

    ~PG_Group_List_Store_File_Guard () noexcept(false);

    virtual void set_object_last_changed (const time_t & time);

    virtual time_t get_object_last_changed ();

    virtual bool object_obsolete ();

    virtual void mark_object_current ();

    virtual int load_from_stream ();

    virtual bool is_loaded_from_stream ();

    virtual TAO::Storable_Base * create_stream (const char * mode);

private:
    PG_Group_List_Store & list_store_;
  };
}

TAO::PG_Group_List_Store_File_Guard::PG_Group_List_Store_File_Guard (
  PG_Group_List_Store & list_store, Method_Type method_type)
  : TAO::Storable_File_Guard(true),
  list_store_(list_store)
{
  if (list_store_.lock_.acquire() == -1)
    {
      throw CORBA::INTERNAL ();
    }
  try
    {
      this->init (method_type);
    }
  catch (const TAO::Storable_Exception &)
    {
      throw CORBA::INTERNAL ();
    }
}

TAO::PG_Group_List_Store_File_Guard::~PG_Group_List_Store_File_Guard () noexcept(false)
{
  this->release ();
  if (list_store_.lock_.release() == -1)
    {
      throw CORBA::INTERNAL ();
    }
}

void
TAO::PG_Group_List_Store_File_Guard::set_object_last_changed (
  const time_t & time)
{
  list_store_.last_changed_ = time;
}

time_t
TAO::PG_Group_List_Store_File_Guard::get_object_last_changed ()
{
  return list_store_.last_changed_;
}

bool
TAO::PG_Group_List_Store_File_Guard::object_obsolete ()
{
  return list_store_.is_obsolete (fl_->last_changed ());
}

void
TAO::PG_Group_List_Store_File_Guard::mark_object_current ()
{
  // Reset the stale flag
  list_store_.stale(false);
  // Set the last update time to the file last update time
  this->set_object_last_changed (fl_->last_changed ());
}

int
TAO::PG_Group_List_Store_File_Guard::load_from_stream ()
{
  list_store_.read (this->peer ());
  list_store_.loaded_from_stream_ = true;
  this->peer ().rewind ();
  return this->peer ().good ();
}

bool
TAO::PG_Group_List_Store_File_Guard::is_loaded_from_stream ()
{
  return list_store_.loaded_from_stream_;
}

TAO::Storable_Base *
TAO::PG_Group_List_Store_File_Guard::create_stream (const char * mode)
{
  return list_store_.create_stream (mode);
}

typedef TAO::PG_Group_List_Store_File_Guard File_Guard;

// Make shortcut to get to Method_Type enums
typedef TAO::Storable_File_Guard SFG;

TAO::PG_Group_List_Store::PG_Group_List_Store (
  Storable_Factory & storable_factory)
  : next_group_id_ (0)
  , storable_factory_ (storable_factory)
  , loaded_from_stream_ (false)
  , last_changed_ (0)
  , stale_ (false)
{
  // Create a temporary stream simply to check if a readable
  // version already exists.
  bool stream_exists = false;
  {
    std::unique_ptr<TAO::Storable_Base> stream (this->create_stream ("r"));

    if (stream->exists ())
      stream_exists = true;
  }

  if (stream_exists)
    {
      File_Guard fg(*this, SFG::CREATE_WITH_FILE);
    }
  else
    {
      File_Guard fg(*this, SFG::CREATE_WITHOUT_FILE);
      this->write (fg.peer ());
    }
}

TAO::PG_Group_List_Store::~PG_Group_List_Store ()
{
}

PortableGroup::ObjectGroupId
TAO::PG_Group_List_Store::get_next_group_id ()
{
  File_Guard fg(*this, SFG::MUTATOR);
  PortableGroup::ObjectGroupId next_id = this->next_group_id_;
  ++this->next_group_id_;
  this->write (fg.peer ());
  return next_id;
}

int
TAO::PG_Group_List_Store::add (PortableGroup::ObjectGroupId id)
{
  File_Guard fg(*this, SFG::MUTATOR);
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
  File_Guard fg(*this, SFG::MUTATOR);
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
  File_Guard fg(*this, SFG::ACCESSOR);
  return group_ids_;
}

void
TAO::PG_Group_List_Store::read (TAO::Storable_Base & stream)
{
  group_ids_.clear ();

  stream.rewind ();

  stream >> this->next_group_id_;

  ACE_UINT64 size;
  stream >> size;

  PortableGroup::ObjectGroupId group_id;
  for (ACE_UINT64 i = 0; i < size; ++i)
    {
      stream >> group_id;
      group_ids_.insert (group_id);
    }
}

void
TAO::PG_Group_List_Store::write (TAO::Storable_Base & stream)
{
  stream.rewind ();

  stream << this->next_group_id_;
  stream << static_cast<ACE_UINT64> (group_ids_.size ());
  for (Group_Id_Const_Iterator it = group_ids_.begin ();
                               it != group_ids_.end (); ++it)
    {
      stream << *it;
    }

  stream.flush ();
}

TAO::Storable_Base *
TAO::PG_Group_List_Store::create_stream (const char * mode)
{
  ACE_CString file_name("ObjectGroup_global");

  return this->storable_factory_.create_stream (
    file_name.c_str (), mode);
}

bool
TAO::PG_Group_List_Store::list_obsolete ()
{
  // TODO: Update if obsolete flag is set based on CORBA call.
  std::unique_ptr<TAO::Storable_Base> stream (this->create_stream ("r"));
  if (!stream->exists ())
    throw CORBA::INTERNAL ();
  if (stream->open () != 0)
    throw CORBA::INTERNAL ();
  return this->is_obsolete (stream->last_changed ());
}

bool
TAO::PG_Group_List_Store::is_obsolete (time_t stored_time)
{
  return
    (!this->loaded_from_stream_) ||
    this->stale () ||
    (stored_time > this->last_changed_);
}

void
TAO::PG_Group_List_Store::stale (bool is_stale)
{
  this->stale_ = is_stale;
}

bool
TAO::PG_Group_List_Store::stale ()
{
  return this->stale_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
