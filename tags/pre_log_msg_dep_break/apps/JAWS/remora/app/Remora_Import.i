// $Id

inline
Remora_Import::operator CORBA::Long()
{
  return this->grab_value();
}

inline CORBA::Long
Remora_Import::get(void)
{
  return this->grab_value();
}

inline int
operator==( Remora_Import& me,  int new_value)
{
  return me.grab_value() == new_value;
}

inline int
operator==( Remora_Import& me,  Remora_Import& import)
{
  return me.grab_value() == import.grab_value();
}

inline int
operator!=( Remora_Import& me,  int new_value)
{
  return me.grab_value() != new_value;
}

inline int
operator!=( Remora_Import& me,  Remora_Import& import)
{
  return me.grab_value() != import.grab_value();
}

inline int
operator<=( Remora_Import& me,  int new_value)
{
  return me.grab_value() <= new_value;
}

inline int
operator<=( Remora_Import& me,  Remora_Import& import)
{
  return me.grab_value() <= import.grab_value();
}

inline int
operator>=( Remora_Import& me,  int new_value)
{
  return me.grab_value() >= new_value;
}

inline int
operator>=( Remora_Import& me,  Remora_Import& import)
{
  return me.grab_value() >= import.grab_value();
}

inline int
operator<( Remora_Import& me,  int new_value)
{
  return me.grab_value() < new_value;
}

inline int
operator<( Remora_Import& me,  Remora_Import& import)
{
  return me.grab_value() < import.grab_value();
}

inline int
operator>( Remora_Import& me,  int new_value)
{
  return me.grab_value() > new_value;
}

inline int
operator>( Remora_Import& me,  Remora_Import& import)
{
  return me.grab_value() > import.grab_value();
}
