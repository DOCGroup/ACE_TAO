// $Id$

#include "tao/Storable_Base.h"

#include "ace/SString.h"

namespace TAO
{
  class Storable_Factory;
}

class Storable_Exception
{
 public:

  Storable_Exception(TAO::Storable_Base::Storable_State state)
    : state_(state)
  {
  }

  TAO::Storable_Base::Storable_State get_state ()
    {
      return state_;
    }

 private:
  TAO::Storable_Base::Storable_State state_;
};

/// A class whose contents are to be saved
class Savable
{
 public:
  Savable (TAO::Storable_Factory & storable_factory);

  ~Savable ();

  void string_set (int index, const ACE_CString &s);
  const ACE_CString & string_get (int index);

  void int_set (int i);
  int int_get ();

  void bytes_set (int size, char * bytes);
  int bytes_get (char *& bytes);

  bool is_loaded_from_stream ();

 private:
  TAO::Storable_Factory & storable_factory_;
  bool loaded_from_stream_;
  time_t last_changed_;
  void load (TAO::Storable_Base & stream);
  void write (TAO::Storable_Base & stream);

  ACE_CString string_[2];
  int i_;

  static const int bytes_size_max;
  int bytes_size_;
  char * bytes_;

  friend class Savable_File_Guard;
};

