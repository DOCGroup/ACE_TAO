// $Id$

#include "orbsvcs/PortableGroup/PG_Object_Group_Storable.h"

#include "tao/Storable_File_Guard.h"
#include "tao/Storable_Factory.h"
#include "tao/CDR.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  class Object_Group_File_Guard : public TAO::Storable_File_Guard
  {
  public:

    Object_Group_File_Guard ( TAO::PG_Object_Group_Storable & object_group,
                         const char *mode);

    ~Object_Group_File_Guard ();

    virtual void set_object_last_changed (const time_t & time);

    virtual time_t get_object_last_changed ();

    virtual void load_from_stream ();

    virtual bool is_loaded_from_stream ();

    virtual TAO::Storable_Base * create_stream (const char * mode);

  private:

    TAO::PG_Object_Group_Storable & object_group_;
  };

}

TAO::Object_Group_File_Guard::Object_Group_File_Guard ( TAO::PG_Object_Group_Storable & object_group,
                                              const char *mode)
  : TAO::Storable_File_Guard(false)
  , object_group_(object_group)
{
  this->init (mode);
}

TAO::Object_Group_File_Guard::~Object_Group_File_Guard ()
{
  this->release ();
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
      Object_Group_File_Guard fg (*this, "r");
    }
  else
    {
      Object_Group_File_Guard fg (*this, "wc");
      this->write (fg.peer ());
    }
}

TAO::PG_Object_Group_Storable::~PG_Object_Group_Storable (void)
{
}

const PortableGroup::Location &
TAO::PG_Object_Group_Storable::get_primary_location (void)
{
  Object_Group_File_Guard fg (*this, "r");
  return TAO::PG_Object_Group::get_primary_location ();
}


void
TAO::PG_Object_Group_Storable::add_member (const PortableGroup::Location & the_location,
                                           CORBA::Object_ptr member)
{
  Object_Group_File_Guard fg (*this, "rw");
  PG_Object_Group::add_member (the_location, member);
  this->write (fg.peer ());
}

int
TAO::PG_Object_Group_Storable::set_primary_member (
                                                   TAO_IOP::TAO_IOR_Property * prop,
                                                   const PortableGroup::Location & the_location)
{
  Object_Group_File_Guard fg (*this, "rw");
  int primary_member = PG_Object_Group::set_primary_member (prop, the_location);
  this->write (fg.peer ());
  return primary_member;
}

void
TAO::PG_Object_Group_Storable::remove_member (
                                              const PortableGroup::Location & the_location)
{
  Object_Group_File_Guard fg (*this, "rw");
  PG_Object_Group::remove_member (the_location);
  this->write (fg.peer ());
}


PortableGroup::Locations *
TAO::PG_Object_Group_Storable::locations_of_members (void)
{
  Object_Group_File_Guard fg (*this, "r");
  return PG_Object_Group::locations_of_members ();
}

void
TAO::PG_Object_Group_Storable::create_member (
                                              const PortableGroup::Location & the_location,
                                              const char * type_id,
                                              const PortableGroup::Criteria & the_criteria)
{
  Object_Group_File_Guard fg (*this, "rw");
  PG_Object_Group::create_member (the_location,
                                  type_id,
                                  the_criteria);
  this->write (fg.peer ());
}

void
TAO::PG_Object_Group_Storable::set_name (const char* group_name)
{
  Object_Group_File_Guard fg (*this, "rw");
  PG_Object_Group::set_name (group_name);
  this->write (fg.peer ());
}

const char*
TAO::PG_Object_Group_Storable::get_name (void)
{
  Object_Group_File_Guard fg (*this, "r");
  return PG_Object_Group::get_name ();
}

void
TAO::PG_Object_Group_Storable::initial_populate (void)
{
  Object_Group_File_Guard fg (*this, "rw");
  PG_Object_Group::initial_populate ();
  this->write (fg.peer ());
}

void
TAO::PG_Object_Group_Storable::minimum_populate (void)
{
  Object_Group_File_Guard fg (*this, "rw");
  PG_Object_Group::minimum_populate ();
  this->write (fg.peer ());
}

int
TAO::PG_Object_Group_Storable::has_member_at (const PortableGroup::Location & location)
{
  Object_Group_File_Guard fg (*this, "r");
  return PG_Object_Group::has_member_at (location);
}

void
TAO::PG_Object_Group_Storable::distribute (int value)
{
  Object_Group_File_Guard fg (*this, "rw");
  PG_Object_Group::distribute (value);
  this->write (fg.peer ());
}

CORBA::Object_ptr
TAO::PG_Object_Group_Storable::get_member_reference (
                                                     const PortableGroup::Location & the_location)
{
  Object_Group_File_Guard fg (*this, "r");
  return PG_Object_Group::get_member_reference (the_location);
}

TAO::Storable_Base *
TAO::PG_Object_Group_Storable::create_stream (const char * mode)
{
  char file_name[BUFSIZ];
  // Although PortableGroup::ObjectGroupId is a typedef
  // to long long int, make ID type explicit to avoid
  // GNU C++ warning on sprintf statement.
  long long int group_id = this->get_object_group_id ();
  ACE_OS::sprintf (file_name, "%lld", group_id);

  return this->storable_factory_.create_stream (file_name, mode);
}

void
TAO::PG_Object_Group_Storable::read (TAO::Storable_Base & stream)
{
  ACE_DEBUG ((LM_DEBUG, "%N:%l (%P|%t|%T) @@ TAO::PG_Object_Group_Storable::read (TAO::Storable_Base & stream)\n"));
  stream.rewind ();

  ACE_CString group_name;
  stream >> group_name;
  if (!stream.good ())
    {
      stream.clear ();
      throw CORBA::INTERNAL ();
    }

  PG_Object_Group::set_name(group_name.c_str());

  stream >> this->distribute_;
  if (!stream.good ())
    {
      stream.clear ();
      throw CORBA::INTERNAL ();
    }

  stream >> this->role_;
  if (!stream.good ())
    {
      stream.clear ();
      throw CORBA::INTERNAL ();
    }

  int size;
  char * buf;

  ///// primary_location_ /////
  stream >> size;
  if (!stream.good ())
    {
      stream.clear ();
      throw CORBA::INTERNAL ();
    }
  buf = new char [size];
  stream.read (size, buf);
  if (!stream.good ())
    {
      stream.clear ();
      throw CORBA::INTERNAL ();
    }
  TAO_InputCDR primary_location_cdr (buf, size);
  primary_location_cdr >> this->primary_location_;
  delete [] buf;


  ///// members_ /////
  int num_members;
  stream >> num_members;
  if (!stream.good ())
    {
      stream.clear ();
      throw CORBA::INTERNAL ();
    }

  this->clear_members_map ();

  for (int i = 0; i < num_members; ++i)
    {
      ///// location used as members_ key /////
      stream >> size;
      if (!stream.good ())
        {
          stream.clear ();
          throw CORBA::INTERNAL ();
        }
      buf = new char [size];
      stream.read (size, buf);
      if (!stream.good ())
        {
          stream.clear ();
          throw CORBA::INTERNAL ();
        }
      TAO_InputCDR the_location_cdr (buf, size);
      PortableGroup::Location the_location;
      the_location_cdr >> the_location;
      if (!the_location_cdr.good_bit ())
        throw CORBA::INV_OBJREF ();

      ///// member /////
      ACE_CString member_ior;
      stream >> member_ior;
      if (!stream.good ())
        {
          stream.clear ();
          throw CORBA::INTERNAL ();
        }
      CORBA::Object_var member = this->orb_->string_to_object (member_ior.c_str ());
      if (CORBA::is_nil (member))
        throw CORBA::INV_OBJREF ();

      ///// location /////
      stream >> size;
      if (!stream.good ())
        {
          stream.clear ();
          throw CORBA::INTERNAL ();
        }
      buf = new char [size];
      stream.read (size, buf);
      if (!stream.good ())
        {
          stream.clear ();
          throw CORBA::INTERNAL ();
        }
      TAO_InputCDR location_cdr (buf, size);
      PortableGroup::Location location;
      location_cdr >> location;
      if (!location_cdr.good_bit ())
        {
          stream.clear ();
          throw CORBA::INTERNAL ();
        }

      ///// factory /////
      ACE_CString factory_ior;
      CORBA::Object_var obj = this->orb_->string_to_object (factory_ior.c_str ());
      PortableGroup::GenericFactory_var factory =
        PortableGroup::GenericFactory::_narrow (obj.in());

      ///// factory_id (typedef of CORBA::Any) /////
      stream >> size;
      if (!stream.good ())
        {
          stream.clear ();
          throw CORBA::INTERNAL ();
        }
      buf = new char [size];
      stream.read (size, buf);
      if (!stream.good ())
        {
          stream.clear ();
          throw CORBA::INTERNAL ();
        }
      TAO_InputCDR factory_id_cdr (buf, size);
      PortableGroup::GenericFactory::FactoryCreationId factory_id;
      factory_id_cdr >> factory_id;
      if (!factory_id_cdr.good_bit ())
        {
          stream.clear ();
          throw CORBA::INTERNAL ();
        }

      ///// is_primary /////
      int is_primary;
      stream >> is_primary;
      if (!stream.good ())
        {
          stream.clear ();
          throw CORBA::INTERNAL ();
        }

      MemberInfo * info = 0;
      ACE_NEW_THROW_EX (info, MemberInfo(member.in(),
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
  primary_location_cdr << PG_Object_Group::get_primary_location();
  stream << primary_location_cdr;

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
      ACE_CString member_ior = this->orb_->object_to_string (member->member_.in ());
      stream << member_ior;

      TAO_OutputCDR location_cdr;
      location_cdr << member->location_;
      stream << location_cdr;

      ACE_CString factory_ior = this->orb_->object_to_string (member->factory_.in ());
      stream << factory_ior;

      TAO_OutputCDR factory_id_cdr;
      factory_id_cdr << member->factory_id_;
      stream << factory_id_cdr;

      stream << member->is_primary_;
    }
  stream.flush ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

