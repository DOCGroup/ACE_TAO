// $Id$

#include "Savable.h"
#include "tao/Storable_Base.h"
#include "tao/Storable_Factory.h"
#include "tao/Storable_File_Guard.h"

class Savable_File_Guard: public TAO::Storable_File_Guard
{
public:

  Savable_File_Guard (Savable & savable, const char *mode);

  ~Savable_File_Guard ();

  virtual void set_parent_last_changed (const time_t & time);

  virtual time_t get_parent_last_changed ();

  virtual void create_child ();

  virtual bool is_child_created ();

  virtual TAO::Storable_Base * create_stream (const char * mode);

  TAO::Storable_Base & peer ();

private:
  Savable & savable_;
  bool child_created_;
};

Savable_File_Guard::Savable_File_Guard (Savable & savable, const char * mode)
  : TAO::Storable_File_Guard(false)
  , savable_(savable)
  , child_created_(false)
{
  this->init(mode);
}

Savable_File_Guard::~Savable_File_Guard ()
{
  this->release ();
}

TAO::Storable_Base &
Savable_File_Guard::peer ()
{
  return *fl_;
}

void
Savable_File_Guard::set_parent_last_changed (const time_t & time)
{
  savable_.last_changed_ = time;
}

time_t
Savable_File_Guard::get_parent_last_changed ()
{
  return savable_.last_changed_;
}

void
Savable_File_Guard::create_child ()
{
  savable_.load (this->peer ());
  child_created_ = true;
}

bool
Savable_File_Guard::is_child_created ()
{
  return child_created_;
}

TAO::Storable_Base *
Savable_File_Guard::create_stream (const char * mode)
{
  return savable_.storable_factory_.create_stream ("test.dat", mode);
}

Savable::Savable (TAO::Storable_Factory & storable_factory)
  : storable_factory_(storable_factory)
{
  ACE_Auto_Ptr<TAO::Storable_Base> stream (storable_factory_.create_stream("test.dat", "r"));
  if (stream->exists ())
    {
      Savable_File_Guard fg(*this, "r");
    }
  else
    {
      Savable_File_Guard fg(*this, "wc");
    }
}

Savable::~Savable ()
{
}

void
Savable::load (TAO::Storable_Base & stream)
{
  for (int i = 0; i < 2; ++i)
    {
      stream >> string_[i];
    }
}

void
Savable::write (TAO::Storable_Base & stream)
{
  for (int i = 0; i < 2; ++i)
    {
      stream << string_[i];
    }
}

void
Savable::string_set (int index, const ACE_CString &s)
{
  Savable_File_Guard fg(*this, "w");
  this->string_[index] = s;
  this->write (fg.peer ());
}

const ACE_CString &
Savable::string_get (int index)
{
  Savable_File_Guard fg(*this, "r");
  return this->string_[index];
}
