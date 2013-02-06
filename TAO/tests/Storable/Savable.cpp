// $Id$

#include "Savable.h"
#include "tao/Storable_Base.h"
#include "tao/Storable_Factory.h"
#include "tao/Storable_File_Guard.h"

const int Savable::bytes_size_max = 128;

class Savable_File_Guard: public TAO::Storable_File_Guard
{
public:

  Savable_File_Guard (Savable & savable, Method_Type method_type);

  ~Savable_File_Guard ();

  virtual void set_object_last_changed (const time_t & time);

  virtual time_t get_object_last_changed ();

  virtual void load_from_stream ();

  virtual bool is_loaded_from_stream ();

  virtual TAO::Storable_Base * create_stream (const ACE_TCHAR * mode);

private:
  Savable & savable_;
};

Savable_File_Guard::Savable_File_Guard (Savable & savable,
                                        Method_Type method_type)
  : TAO::Storable_File_Guard(false)
  , savable_(savable)
{
  try
    {
      this->init(method_type);
    }
  catch (TAO::Storable_Read_Exception &)
    {
      throw Savable_Exception();
    }
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
  savable_.read (this->peer ());
  savable_.loaded_from_stream_ = true;
  this->peer ().rewind ();
}

bool
Savable_File_Guard::is_loaded_from_stream ()
{
  return savable_.loaded_from_stream_;
}

TAO::Storable_Base *
Savable_File_Guard::create_stream (const ACE_TCHAR * mode)
{
  return savable_.storable_factory_.create_stream ("test.dat", mode);
}

typedef TAO::Storable_File_Guard SFG;

Savable::Savable (TAO::Storable_Factory & storable_factory)
  : storable_factory_(storable_factory)
  , loaded_from_stream_ (false)
{

  for (int index = 0; index < 2; ++index)
    {
      this->i_[index] = 0;
      this->ui_[index] = 0;
      this->bytes_size_[index] = 0;
      this->bytes_[index] = new char [this->bytes_size_max];
      for (int i = 0; i < this->bytes_size_max; ++i)
        {
          this->bytes_[index][i] = ACE_CHAR_MAX;
        }
    }

  ACE_Auto_Ptr<TAO::Storable_Base>
    stream (storable_factory_.create_stream("test.dat", ACE_TEXT ("r")));
  if (stream->exists ())
    {
      Savable_File_Guard fg(*this, SFG::CREATE_WITH_FILE);
    }
  else
    {
      Savable_File_Guard fg(*this, SFG::CREATE_WITHOUT_FILE);
      this->write (fg.peer ());
    }
}

Savable::~Savable ()
{
  for (int index = 0; index < 2; ++index)
    {
      delete []this->bytes_[index];
    }
}

bool
Savable::is_loaded_from_stream ()
{
  return this->loaded_from_stream_;
}

void
Savable::read (TAO::Storable_Base & stream)
{
  stream.rewind ();

  for (int index = 0; index < 2; ++index)
    {
      stream >> this->string_[index];
      stream >> this->i_[index];
      stream >> this->ui_[index];
      stream >> this->bytes_size_[index];
      stream.read (this->bytes_size_[index], this->bytes_[index]);
    }
}

void
Savable::write (TAO::Storable_Base & stream)
{
  stream.rewind ();

  for (int index = 0; index < 2; ++index)
    {
      stream << this->string_[index];
      stream << this->i_[index];
      stream << this->ui_[index];
      stream << this->bytes_size_[index];
      stream.write (this->bytes_size_[index], this->bytes_[index]);
    }

  stream.flush ();
}

void
Savable::string_set (int index, const ACE_CString &s)
{
  Savable_File_Guard fg(*this, SFG::MUTATOR);
  this->string_[index] = s;
  this->write (fg.peer ());
}

const ACE_CString &
Savable::string_get (int index)
{
  Savable_File_Guard fg(*this, SFG::ACCESSOR);
  return this->string_[index];
}

void
Savable::int_set (int index, int i)
{
  Savable_File_Guard fg(*this, SFG::MUTATOR);
  this->i_[index] = i;
  this->write (fg.peer ());
}

int
Savable::int_get (int index)
{
  Savable_File_Guard fg(*this, SFG::ACCESSOR);
  return this->i_[index];
}

void
Savable::unsigned_int_set (int index, unsigned int ui)
{
  Savable_File_Guard fg(*this, SFG::MUTATOR);
  this->ui_[index] = ui;
  this->write (fg.peer ());
}

unsigned int
Savable::unsigned_int_get (int index)
{
  Savable_File_Guard fg(*this, SFG::ACCESSOR);
  return this->ui_[index];
}

void
Savable::bytes_set (int index, int size, char * bytes)
{
  Savable_File_Guard fg(*this, SFG::MUTATOR);
  this->bytes_size_[index] = size;
  for (int i = 0; i < this->bytes_size_[index]; ++i)
    {
      this->bytes_[index][i] = bytes[i];
    }
  this->write (fg.peer ());
}

int
Savable::bytes_get (int index, char * bytes)
{
  Savable_File_Guard fg(*this, SFG::ACCESSOR);
  for (int i = 0; i < this->bytes_size_[index]; ++i)
    {
      bytes[i] = this->bytes_[index][i];
    }
  return this->bytes_size_[index];
}
