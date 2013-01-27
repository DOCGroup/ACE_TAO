// $Id$

#include "orbsvcs/PortableGroup/PG_Object_Group_Storable.h"

#include "tao/Storable_File_Guard.h"
#include "tao/Storable_Factory.h"
#include "tao/CDR.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace
{
  template <typename T>
  void read_cdr (TAO::Storable_Base & stream, T & corba_data)
  {
  int size;
  stream >> size;

  char *tmp = 0;
  ACE_NEW_THROW_EX (tmp, char [size], CORBA::NO_MEMORY ());
  ACE_Auto_Basic_Array_Ptr<char> buf (tmp);
  stream.read (size, buf.get ());

  TAO_InputCDR cdr (buf.get (), size);
  cdr >> corba_data;
  if (!cdr.good_bit ())
    {
      stream.clear ();
      throw CORBA::INTERNAL ();
    }
  }
}

namespace TAO
{

  class Object_Group_File_Guard : public TAO::Storable_File_Guard
  {
  public:

    Object_Group_File_Guard ( TAO::PG_Object_Group_Storable & object_group,
                              Method_Type method_type);

    ~Object_Group_File_Guard ();

    virtual void set_object_last_changed (const time_t & time);

    virtual time_t get_object_last_changed ();

    /// Check if the guarded object is current with the last
    /// update which could have been performed independently of
    /// the owner of this object.
    virtual bool object_obsolete ();

    /// Mark the object as current with respect to the
    /// file to which it was persisted.
    virtual void mark_object_current ();

    virtual void load_from_stream ();

    virtual bool is_loaded_from_stream ();

    virtual TAO::Storable_Base * create_stream (const char * mode);

  private:

    TAO::PG_Object_Group_Storable & object_group_;
  };

}

TAO::Object_Group_File_Guard::Object_Group_File_Guard (
  TAO::PG_Object_Group_Storable & object_group,
  Method_Type method_type)
  : TAO::Storable_File_Guard(true)
  , object_group_(object_group)
{
  try
    {
      this->init (method_type);
    }
  catch (const TAO::Storable_Read_Exception &)
    {
      throw CORBA::INTERNAL ();
    }
}

TAO::Object_Group_File_Guard::~Object_Group_File_Guard ()
{
  this->release ();

  // Notify if persistent store was updated.
  if (object_group_.write_occurred_)
    object_group_.state_written ();
}

void
TAO::Object_Group_File_Guard::set_object_last_changed (const time_t & time)
{
  object_group_.last_changed_ = time;
}

time_t
TAO::Object_Group_File_Guard::get_object_last_changed ()
{
  return object_group_.last_changed_;
}

bool
TAO::Object_Group_File_Guard::object_obsolete ()
{
  return object_group_.is_obsolete (fl_->last_changed ());
}

void
TAO::Object_Group_File_Guard::mark_object_current ()
{
  object_group_.stale (false);
  TAO::Storable_File_Guard::mark_object_current ();
}

void
TAO::Object_Group_File_Guard::load_from_stream ()
{
  object_group_.read (this->peer ());
  object_group_.loaded_from_stream_ = true;
}

bool
TAO::Object_Group_File_Guard::is_loaded_from_stream ()
{
  return object_group_.loaded_from_stream_;
}

TAO::Storable_Base *
TAO::Object_Group_File_Guard::create_stream (const char * mode)
{
  return object_group_.create_stream (mode);
}

// Make shortcut to get to Method_Type enums
typedef TAO::Storable_File_Guard SFG;

TAO::PG_Object_Group_Storable::PG_Object_Group_Storable (
  CORBA::ORB_ptr orb,
  PortableGroup::FactoryRegistry_ptr factory_registry,
  TAO::PG_Object_Group_Manipulator & manipulator,
  CORBA::Object_ptr empty_group,
  const PortableGroup::TagGroupTaggedComponent & tagged_component,
  const char * type_id,
  const PortableGroup::Criteria & the_criteria,
  TAO::PG_Property_Set * type_properties,
  TAO::Storable_Factory & storable_factory)
  : PG_Object_Group(orb,
                    factory_registry,
                    manipulator,
                    empty_group,
                    tagged_component,
                    type_id,
                    the_criteria,
                    type_properties)
  , group_previously_stored_(false)
  , group_id_previously_stored_(0)
  , storable_factory_ (storable_factory)
  , last_changed_ (0)
  , loaded_from_stream_ (false)
  , destroyed_ (false)
  , write_occurred_ (false)
{
  // Create a temporary stream simply to check if a readable
  // version already exists.
  bool stream_exists = false;
  {
    ACE_Auto_Ptr<TAO::Storable_Base> stream (
      this->create_stream (ACE_TEXT_ALWAYS_CHAR ("r")));

    if (stream->exists ())
      stream_exists = true;
  }

  if (stream_exists)
    {
      Object_Group_File_Guard fg (*this, SFG::CREATE_WITH_FILE);
    }
  else
    {
      Object_Group_File_Guard fg (*this, SFG::CREATE_WITHOUT_FILE);
      this->write (fg.peer ());
    }
}

TAO::PG_Object_Group_Storable::PG_Object_Group_Storable (
  PortableGroup::ObjectGroupId group_id,
  CORBA::ORB_ptr orb,
  PortableGroup::FactoryRegistry_ptr factory_registry,
  TAO::PG_Object_Group_Manipulator & manipulator,
  TAO::Storable_Factory & storable_factory)
  : PG_Object_Group(orb,
                    factory_registry,
                    manipulator)
  , group_previously_stored_(true)
  , group_id_previously_stored_(group_id)
  , storable_factory_ (storable_factory)
  , last_changed_ (0)
  , loaded_from_stream_ (false)
  , destroyed_ (false)
  , write_occurred_ (false)
{
  // Create a temporary stream to simply to check if a readable
  // version already exists.
  bool stream_exists = false;
  {
    ACE_Auto_Ptr<TAO::Storable_Base> stream (
      this->create_stream (ACE_TEXT_ALWAYS_CHAR ("r")));

    if (stream->exists ())
      stream_exists = true;
  }

  if (stream_exists)
    {
      Object_Group_File_Guard fg (*this, SFG::ACCESSOR);
    }
  else
    {
      throw CORBA::INTERNAL ();
    }
}

TAO::PG_Object_Group_Storable::~PG_Object_Group_Storable (void)
{
  if (destroyed_)
    {
      ACE_Auto_Ptr<TAO::Storable_Base> stream (
        this->create_stream (ACE_TEXT_ALWAYS_CHAR ("r")));

      if (stream->exists ())
        {
          stream->remove ();
        }

    }

}

void
TAO::PG_Object_Group_Storable::set_destroyed (bool destroyed)
{
  this->destroyed_ = destroyed;
}

const PortableGroup::Location &
TAO::PG_Object_Group_Storable::get_primary_location (void)
{
  Object_Group_File_Guard fg (*this, SFG::ACCESSOR);
  return TAO::PG_Object_Group::get_primary_location ();
}


void
TAO::PG_Object_Group_Storable::add_member (
  const PortableGroup::Location & the_location,
  CORBA::Object_ptr member)
{
  Object_Group_File_Guard fg (*this, SFG::MUTATOR);
  PG_Object_Group::add_member (the_location, member);
  this->write (fg.peer ());
}

int
TAO::PG_Object_Group_Storable::set_primary_member (
  TAO_IOP::TAO_IOR_Property * prop,
  const PortableGroup::Location & the_location)
{
  Object_Group_File_Guard fg (*this, SFG::MUTATOR);
  int primary_member = PG_Object_Group::set_primary_member (prop, the_location);
  this->write (fg.peer ());
  return primary_member;
}

void
TAO::PG_Object_Group_Storable::remove_member (
  const PortableGroup::Location & the_location)
{
  Object_Group_File_Guard fg (*this, SFG::MUTATOR);
  PG_Object_Group::remove_member (the_location);
  this->write (fg.peer ());
}


PortableGroup::Locations *
TAO::PG_Object_Group_Storable::locations_of_members (void)
{
  Object_Group_File_Guard fg (*this, SFG::ACCESSOR);
  return PG_Object_Group::locations_of_members ();
}

void
TAO::PG_Object_Group_Storable::create_member (
  const PortableGroup::Location & the_location,
  const char * type_id,
  const PortableGroup::Criteria & the_criteria)
{
  Object_Group_File_Guard fg (*this, SFG::MUTATOR);
  PG_Object_Group::create_member (the_location,
                                  type_id,
                                  the_criteria);
  this->write (fg.peer ());
}

void
TAO::PG_Object_Group_Storable::set_name (const char* group_name)
{
  Object_Group_File_Guard fg (*this, SFG::MUTATOR);
  PG_Object_Group::set_name (group_name);
  this->write (fg.peer ());
}

const char*
TAO::PG_Object_Group_Storable::get_name (void)
{
  Object_Group_File_Guard fg (*this, SFG::ACCESSOR);
  return PG_Object_Group::get_name ();
}

void
TAO::PG_Object_Group_Storable::initial_populate (void)
{
  Object_Group_File_Guard fg (*this, SFG::MUTATOR);
  PG_Object_Group::initial_populate ();
  this->write (fg.peer ());
}

void
TAO::PG_Object_Group_Storable::minimum_populate (void)
{
  Object_Group_File_Guard fg (*this, SFG::MUTATOR);
  PG_Object_Group::minimum_populate ();
  this->write (fg.peer ());
}

int
TAO::PG_Object_Group_Storable::has_member_at (
  const PortableGroup::Location & location)
{
  Object_Group_File_Guard fg (*this, SFG::ACCESSOR);
  return PG_Object_Group::has_member_at (location);
}

void
TAO::PG_Object_Group_Storable::distribute (int value)
{
  Object_Group_File_Guard fg (*this, SFG::MUTATOR);
  PG_Object_Group::distribute (value);
  this->write (fg.peer ());
}

CORBA::Object_ptr
TAO::PG_Object_Group_Storable::get_member_reference (
  const PortableGroup::Location & the_location)
{
  Object_Group_File_Guard fg (*this, SFG::ACCESSOR);
  return PG_Object_Group::get_member_reference (the_location);
}

PortableGroup::ObjectGroupId
TAO::PG_Object_Group_Storable::get_object_group_id () const
{
  if (this->group_previously_stored_)
    return this->group_id_previously_stored_;

  return PG_Object_Group::get_object_group_id ();
}

TAO::Storable_Base *
TAO::PG_Object_Group_Storable::create_stream (const char * mode)
{
  char file_name[BUFSIZ];
  // Although PortableGroup::ObjectGroupId is a typedef
  // to long long int, make ID type explicit to avoid
  // GNU C++ warning on sprintf statement.
  long long int id =  this->get_object_group_id ();
  ACE_OS::sprintf (file_name, ACE_TEXT_ALWAYS_CHAR ("ObjectGroup_%lld"), id);
  return this->storable_factory_.create_stream (file_name, mode);
}

void
TAO::PG_Object_Group_Storable::read (TAO::Storable_Base & stream)
{
  stream.rewind ();

  ACE_CString group_name;
  stream >> group_name;

  PG_Object_Group::set_name(group_name.c_str());

  stream >> this->distribute_;

  stream >> this->role_;

  ///// primary_location_ /////
  read_cdr (stream, this->primary_location_);

  ///// reference_ /////
  ACE_CString reference_ior;
  stream >> reference_ior;
  this->reference_ = this->orb_->string_to_object (reference_ior.c_str ());

  ///// tagged_component_ /////
  read_cdr (stream, this->tagged_component_);

  ///// type_id_ /////
  read_cdr(stream, this->type_id_);

  ///// properties_ /////
  PortableGroup::Criteria properties;
  read_cdr (stream, properties);
  PG_Object_Group::set_properties_dynamically (properties);

  ///// members_ /////
  int num_members;
  stream >> num_members;

  this->clear_members_map ();

  for (int i = 0; i < num_members; ++i)
    {
      ///// location used as members_ key /////
      PortableGroup::Location the_location;
      read_cdr (stream, the_location);

      ///// member /////
      ACE_CString member_ior;
      stream >> member_ior;
      CORBA::Object_var member =
        this->orb_->string_to_object (member_ior.c_str ());
      if (CORBA::is_nil (member))
        throw CORBA::INV_OBJREF ();

      ///// location /////
      PortableGroup::Location location;
      read_cdr (stream, location);

      ///// factory /////
      ACE_CString factory_ior;
      stream >> factory_ior;
      CORBA::Object_var obj =
        this->orb_->string_to_object (factory_ior.c_str ());
      PortableGroup::GenericFactory_var factory =
        PortableGroup::GenericFactory::_narrow (obj.in());

      ///// factory_id (typedef of CORBA::Any) /////
      PortableGroup::GenericFactory::FactoryCreationId factory_id;
      read_cdr (stream, factory_id);

      ///// is_primary /////
      int is_primary;
      stream >> is_primary;

      MemberInfo * info = 0;
      ACE_NEW_THROW_EX (info, MemberInfo(member.in (),
                                         the_location,
                                         factory,
                                         factory_id),
                        CORBA::NO_MEMORY());

      info->is_primary_ = is_primary;

      if (this->members_.bind (the_location, info) != 0)
        {
          throw CORBA::NO_MEMORY();
        }
    }
}

void
TAO::PG_Object_Group_Storable::write (TAO::Storable_Base & stream)
{
  stream.rewind ();

  ACE_CString group_name = PG_Object_Group::get_name ();
  stream << group_name;
  stream << this->distribute_;
  stream << this->role_;

  TAO_OutputCDR primary_location_cdr;
  primary_location_cdr << PG_Object_Group::get_primary_location ();
  stream << primary_location_cdr;

  ACE_CString reference_ior =
    this->orb_->object_to_string (this->reference_.in ());
  stream << reference_ior;

  TAO_OutputCDR tagged_component_cdr;
  tagged_component_cdr << this->tagged_component_;
  stream << tagged_component_cdr;

  TAO_OutputCDR type_id_cdr;
  PortableGroup::TypeId_var type_id = PG_Object_Group::get_type_id ();
  type_id_cdr << type_id;
  stream << type_id_cdr;

  TAO_OutputCDR properties_cdr;
  PortableGroup::Criteria properties;
  this->properties_.export_properties (properties);
  properties_cdr << properties;
  stream << properties_cdr;

  ///// members_ /////
  int num_members = this->members_.current_size  ();
  stream << num_members;
  for (MemberMap_Iterator it = this->members_.begin ();
       it != this->members_.end ();
       ++it)
    {
      PortableGroup::Location the_location = it->key ();
      TAO_OutputCDR the_location_cdr;
      the_location_cdr << the_location;
      stream << the_location_cdr;

      MemberInfo * member = it->item ();
      ACE_CString member_ior =
        this->orb_->object_to_string (member->member_.in ());
      stream << member_ior;

      TAO_OutputCDR location_cdr;
      location_cdr << member->location_;
      stream << location_cdr;

      ACE_CString factory_ior =
        this->orb_->object_to_string (member->factory_.in ());
      stream << factory_ior;

      TAO_OutputCDR factory_id_cdr;
      factory_id_cdr << member->factory_id_;
      stream << factory_id_cdr;

      stream << member->is_primary_;
    }
  stream.flush ();
  this->write_occurred_ = true;
}

void
TAO::PG_Object_Group_Storable::stale (bool is_stale)
{
  ACE_UNUSED_ARG (is_stale);
  // Default implementation is no-op
}

bool
TAO::PG_Object_Group_Storable::stale ()
{
  // Default is to return false
  return false;
}

void
TAO::PG_Object_Group_Storable::state_written (void)
{
  // No-op. Overridden by derived class.
}

bool
TAO::PG_Object_Group_Storable::is_obsolete (time_t stored_time)
{
  return (!this->loaded_from_stream_) ||
    stored_time > this->last_changed_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
