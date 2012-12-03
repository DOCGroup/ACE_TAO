// $Id$

#include "orbsvcs/PortableGroup/PG_Object_Group_Storable.h"

#include "tao/Storable_File_Guard.h"
#include "tao/Storable_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

class Group_Object_Synch : public TAO::Storable_File_Guard
{
public:

  Group_Object_Synch ( TAO::PG_Object_Group_Storable & object_group,
                       const char *mode);

  ~Group_Object_Synch ();

  virtual void set_object_last_changed (const time_t & time);

  virtual time_t get_object_last_changed ();

  virtual void load_from_stream ();

  virtual bool is_loaded_from_stream ();

  virtual TAO::Storable_Base * create_stream (const char * mode);

  TAO::Storable_Base & peer ();

private:

  TAO::PG_Object_Group_Storable & object_group_;
  bool loaded_from_stream_;
};

Group_Object_Synch::Group_Object_Synch ( TAO::PG_Object_Group_Storable & object_group,
                                         const char *mode)
  : TAO::Storable_File_Guard(false)
  , object_group_(object_group)
  , loaded_from_stream_(false)
{
  this->init (mode);
}

Group_Object_Synch::~Group_Object_Synch ()
{
  this->release ();
}

void
Group_Object_Synch::set_object_last_changed (const time_t & time)
{
  object_group_.last_changed_ = time;
}

time_t
Group_Object_Synch::get_object_last_changed ()
{
  return object_group_.last_changed_;
}

void
Group_Object_Synch::load_from_stream ()
{
  loaded_from_stream_ = true;
}

bool
Group_Object_Synch::is_loaded_from_stream ()
{
  return loaded_from_stream_;
}

TAO::Storable_Base *
Group_Object_Synch::create_stream (const char * mode)
{
  return object_group_.create_stream (mode);
}

}

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
  , storable_factory_ (storable_factory)
  , last_changed_ (0)
{
  ACE_Auto_Ptr<TAO::Storable_Base> stream (this->create_stream ("r"));
  if (stream->exists ())
    {
      Group_Object_Synch gos (*this, "r");
    }
  else
    {
      Group_Object_Synch gos (*this, "wc");
    }
}

TAO::PG_Object_Group_Storable::~PG_Object_Group_Storable (void)
{
}

const PortableGroup::Location &
TAO::PG_Object_Group_Storable::get_primary_location (void) const
{
  return TAO::PG_Object_Group::get_primary_location ();
}


void
TAO::PG_Object_Group_Storable::add_member (const PortableGroup::Location & the_location,
                                  CORBA::Object_ptr member)
{
  PG_Object_Group::add_member (the_location, member);
}

int
TAO::PG_Object_Group_Storable::set_primary_member (
    TAO_IOP::TAO_IOR_Property * prop,
    const PortableGroup::Location & the_location)
{
  return PG_Object_Group::set_primary_member (prop, the_location);
}

void
TAO::PG_Object_Group_Storable::remove_member (
    const PortableGroup::Location & the_location)
{
  PG_Object_Group::remove_member (the_location);
}


PortableGroup::Locations *
TAO::PG_Object_Group_Storable::locations_of_members (void)
{
  return PG_Object_Group::locations_of_members ();
}

void
TAO::PG_Object_Group_Storable::create_member (
    const PortableGroup::Location & the_location,
    const char * type_id,
    const PortableGroup::Criteria & the_criteria)
{
  PG_Object_Group::create_member (the_location,
                                  type_id,
                                  the_criteria);
}

void
TAO::PG_Object_Group_Storable::set_name (const char* group_name)
{
  PG_Object_Group::set_name (group_name);
}

const char*
TAO::PG_Object_Group_Storable::get_name (void)
{
  return PG_Object_Group::get_name ();
}

void
TAO::PG_Object_Group_Storable::initial_populate (void)
{
  PG_Object_Group::initial_populate ();
}

void
TAO::PG_Object_Group_Storable::minimum_populate (void)
{
  PG_Object_Group::minimum_populate ();
}

int
TAO::PG_Object_Group_Storable::has_member_at (const PortableGroup::Location & location)
{
  return PG_Object_Group::has_member_at (location);
}

void
TAO::PG_Object_Group_Storable::distribute (int value)
{
  PG_Object_Group::distribute (value);
}

CORBA::Object_ptr
TAO::PG_Object_Group_Storable::get_member_reference (
  const PortableGroup::Location & the_location)
{
  return PG_Object_Group::get_member_reference (the_location);
}

TAO::Storable_Base *
TAO::PG_Object_Group_Storable::create_stream (const char * mode)
{
  const char * file_name = this->get_type_id ();
  return this->storable_factory_.create_stream (file_name, mode);
}

void
TAO::PG_Object_Group_Storable::load (TAO::Storable_Base & stream)
{
}

void
TAO::PG_Object_Group_Storable::write (TAO::Storable_Base & stream)
{
  stream << this->get_name ();
  stream << this->distribute_;
  stream << this->role_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

