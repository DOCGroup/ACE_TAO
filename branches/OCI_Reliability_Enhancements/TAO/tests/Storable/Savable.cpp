// $Id$

#include "Savable.h"
#include "tao/Storable_Base.h"
#include "tao/Storable_Factory.h"
#include "tao/Storable_File_Guard.h"

const int Savable::bytes_size_max = 128;

class Savable_File_Guard: public TAO::Storable_File_Guard
{
public:

  Savable_File_Guard (Savable & savable, const char *mode);

  ~Savable_File_Guard ();

  virtual void set_object_last_changed (const time_t & time);

  virtual time_t get_object_last_changed ();

  virtual void load_from_stream ();

  virtual bool is_loaded_from_stream ();

  virtual TAO::Storable_Base * create_stream (const char * mode);

private:
  Savable & savable_;
};

Savable_File_Guard::Savable_File_Guard (Savable & savable, const char * mode)
  : TAO::Storable_File_Guard(false)
  , savable_(savable)
{
  this->init(mode);
}

Savable_File_Guard::~Savable_File_Guard ()
{
  this->release ();
}

void
Savable_File_Guard::set_object_last_changed (const time_t & time)
{
  savable_.last_changed_ = time;
}

time_t
Savable_File_Guard::get_object_last_changed ()
{
  return savable_.last_changed_;
}

void
Savable_File_Guard::load_from_stream ()
{
  savable_.load (this->peer ());
  savable_.loaded_from_stream_ = true;
  this->peer ().rewind ();
}

bool
Savable_File_Guard::is_loaded_from_stream ()
{
  return savable_.loaded_from_stream_;
}

TAO::Storable_Base *
Savable_File_Guard::create_stream (const char * mode)
{
  return savable_.storable_factory_.create_stream ("test.dat", mode);
}

Savable::Savable (TAO::Storable_Factory & storable_factory)
  : storable_factory_(storable_factory)
  , loaded_from_stream_ (false)
  , i_(42)
  , bytes_size_(0)
{

  this->bytes_ = new char [this->bytes_size_max];
  for (int i = 0; i < this->bytes_size_max; ++i)
    {
      this->bytes_[i] = 255;
    }

  ACE_Auto_Ptr<TAO::Storable_Base> stream (storable_factory_.create_stream("test.dat", "r"));
  if (stream->exists ())
    {
      Savable_File_Guard fg(*this, "r");
    }
  else
    {
      Savable_File_Guard fg(*this, "wc");
      this->write (fg.peer ());
    }
}

Savable::~Savable ()
{
}

bool
Savable::is_loaded_from_stream ()
{
  return this->loaded_from_stream_;
}

void
Savable::load (TAO::Storable_Base & stream)
{
  stream.rewind ();
  if (!stream.good ())
    throw Storable_Exception (stream.rdstate ());

  for (int i = 0; i < 2; ++i)
    {
      stream >> string_[i];
      if (!stream.good ())
        throw Storable_Exception (stream.rdstate ());
    }
  stream >> i_;
  if (!stream.good ())
    throw Storable_Exception (stream.rdstate ());

  stream >> this->bytes_size_;
  stream.read (this->bytes_size_, this->bytes_);
  if (!stream.good ())
    throw Storable_Exception (stream.rdstate ());
}

void
Savable::write (TAO::Storable_Base & stream)
{
  stream.rewind ();
  if (!stream.good ())
    throw Storable_Exception (stream.rdstate ());

  for (int i = 0; i < 2; ++i)
    {
      stream << string_[i];
      if (!stream.good ())
        throw Storable_Exception (stream.rdstate ());
    }
  stream << i_;
  if (!stream.good ())
    throw Storable_Exception (stream.rdstate ());

  stream << this->bytes_size_;
  if (!stream.good ())
    throw Storable_Exception (stream.rdstate ());
  stream.write (this->bytes_size_, this->bytes_);
  if (!stream.good ())
    throw Storable_Exception (stream.rdstate ());

  stream.flush ();
}

void
Savable::string_set (int index, const ACE_CString &s)
{
  Savable_File_Guard fg(*this, "rw");
  this->string_[index] = s;
  this->write (fg.peer ());
}

const ACE_CString &
Savable::string_get (int index)
{
  Savable_File_Guard fg(*this, "r");
  return this->string_[index];
}

void
Savable::int_set (int i)
{
  Savable_File_Guard fg(*this, "rw");
  this->i_ = i;
  this->write (fg.peer ());
}

int
Savable::int_get ()
{
  Savable_File_Guard fg(*this, "r");
  return this->i_;
}

void
Savable::bytes_set (int size, char * bytes)
{
  Savable_File_Guard fg(*this, "rw");
  bytes_size_ = size;
  for (int i = 0; i < this->bytes_size_; ++i)
    {
      this->bytes_[i] = bytes[i];
    }
  this->write (fg.peer ());
}

int
Savable::bytes_get (char *& bytes)
{
  Savable_File_Guard fg(*this, "r");
  for (int i = 0; i < this->bytes_size_; ++i)
    {
      bytes[i] = this->bytes_[i];
    }
  return this->bytes_size_;
}
