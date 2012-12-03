// $Id$

#include "ace/SString.h"

namespace TAO
{
  class Storable_Base;
  class Storable_Factory;
}

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

 private:
  TAO::Storable_Factory & storable_factory_;
  time_t last_changed_;
  void load (TAO::Storable_Base & stream);
  void write (TAO::Storable_Base & stream);

  ACE_CString string_[2];
  int i_;

  friend class Savable_File_Guard;
};

