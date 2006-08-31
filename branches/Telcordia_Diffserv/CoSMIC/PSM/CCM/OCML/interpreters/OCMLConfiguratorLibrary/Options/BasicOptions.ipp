// $Id$

namespace OCML
{

  /*
   *  Inline methods for the BooleanOption.
   */

  inline
  BooleanOption::BooleanOption(const char* name, const char* description,
                               value_type default_value)
    : Basic_Option<bool>(ovk_boolean, name, description, default_value)
  {
  }

  /*
   *  Inline methods for the StringOption.
   */

  inline
  StringOption::StringOption(const char* name, const char* description,
                             value_type default_value)
    : Basic_Option<std::string>(ovk_string, name, description, default_value)
  {
  }

  /*
   *  Inline methods for the IntegerOption.
   */

  inline
  IntegerOption::IntegerOption(const char* name, const char* description,
                               value_type default_value)
    : Basic_Option<int>(ovk_integer, name, description, default_value)
  {
  }

  /*
   *  Inline methods for the EnumOption.
   */

  inline
  EnumOption::EnumOption(const char* name, const char* description)
    : Base_Option<std::string>(ovk_enum, name, description),
      has_default_(false)
  {
  }

  inline void
  EnumOption::add_item(const char* item_name, bool default_value)
  {
    items_.push_front(std::string(item_name));
    if (default_value)
      {
        has_default_ = true;
        default_ = items_.begin();
      }
  }

  inline void
  EnumOption::set_default(item_iterator item)
  {
    has_default_ = true;
    default_ = items_.begin();
  }

  inline bool
  EnumOption::is_default(item_iterator item)
  {
    return has_default_ && (item == default_);
  }

  inline EnumOption::item_iterator
  EnumOption::get_default()
  {
    return default_;
  }

  inline bool
  EnumOption::has_default()
  {
    return has_default_;
  }

  /*
   *  Exception class EnumOption::InvalidItem implementation.
   */

  inline
  EnumOption::InvalidItem::InvalidItem(const std::string& msg)
    : msg_(std::string("EnumOption:InvalidItem ") +
           std::string("\"") +
           msg +
           std::string("\""))
  {
  }

  inline
  EnumOption::InvalidItem::~InvalidItem() throw()
  {
  }

  inline const char*
  EnumOption::InvalidItem::what() const throw()
  {
    return msg_.c_str();
  }

} // namespace OCML
