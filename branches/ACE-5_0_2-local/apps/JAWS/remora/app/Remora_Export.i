// $Id

inline void
Remora_Export::operator=(const int new_value)
{
  this->update_value(new_value);
}

inline void
Remora_Export::operator=(const Remora_Export& export)
{
  this->update_value(export.stat_.value_);
}

inline void
Remora_Export::operator+=(const int new_value)
{
  this->update_value(this->stat_.value_ + new_value);
}

inline void
Remora_Export::operator+=(const Remora_Export& export)
{
  this->update_value(this->stat_.value_ + export.stat_.value_);
}

inline void
Remora_Export::operator-=(const int new_value)
{
  this->update_value(this->stat_.value_ - new_value);
}

inline void
Remora_Export::operator-=(const Remora_Export& export)
{
  this->update_value(this->stat_.value_ - export.stat_.value_);
}

inline void
Remora_Export::operator*=(const int new_value)
{
  this->update_value(this->stat_.value_ * new_value);
}

inline void
Remora_Export::operator*=(const Remora_Export& export)
{
  this->update_value(this->stat_.value_ * export.stat_.value_);
}

inline void
Remora_Export::operator/=(const int new_value)
{
  this->update_value(this->stat_.value_ / new_value);
}

inline void
Remora_Export::operator/=(const Remora_Export& export)
{
  this->update_value(this->stat_.value_ / export.stat_.value_);
}

inline
Remora_Export::operator CORBA::Long() const
{
  return this->stat_.value_;
}

inline
Remora_Export::operator CORBA::Long&()
{
  return this->stat_.value_;
}

inline int
operator==(const Remora_Export& me, const int new_value)
{
  return me.stat_.value_ == new_value;
}

inline int
operator==(const Remora_Export& me, const Remora_Export& export)
{
  return me.stat_.value_ == export.stat_.value_;
}

inline int
operator!=(const Remora_Export& me, const int new_value)
{
  return me.stat_.value_ != new_value;
}

inline int
operator!=(const Remora_Export& me, const Remora_Export& export)
{
  return me.stat_.value_ != export.stat_.value_;
}

inline int
operator<=(const Remora_Export& me, const int new_value)
{
  return me.stat_.value_ <= new_value;
}

inline int
operator<=(const Remora_Export& me, const Remora_Export& export)
{
  return me.stat_.value_ <= export.stat_.value_;
}

inline int
operator>=(const Remora_Export& me, const int new_value)
{
  return me.stat_.value_ >= new_value;
}

inline int
operator>=(const Remora_Export& me, const Remora_Export& export)
{
  return me.stat_.value_ >= export.stat_.value_;
}

inline int
operator<(const Remora_Export& me, const int new_value)
{
  return me.stat_.value_ < new_value;
}

inline int
operator<(const Remora_Export& me, const Remora_Export& export)
{
  return me.stat_.value_ < export.stat_.value_;
}

inline int
operator>(const Remora_Export& me, const int new_value)
{
  return me.stat_.value_ > new_value;
}

inline int
operator>(const Remora_Export& me, const Remora_Export& export)
{
  return me.stat_.value_ > export.stat_.value_;
}

