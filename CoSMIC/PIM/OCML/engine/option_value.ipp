namespace OCML
{

  inline void Option_Value::str_value(const char* value)
  {
    std::istringstream s(value);
    this->read(s);
  }

  inline std::string Option_Value::str_value() const
  {
    std::ostringstream s;
    this->write(s);
    return s.str();
  }

  inline bool Option_Value::operator==(const Option_Value& option_value) const
  {
    return this->compare(&option_value);
  }

  inline bool Option_Value::operator!=(const Option_Value& option_value) const
  {
    return !(this->compare(&option_value));
  }

} // namespace OCML
