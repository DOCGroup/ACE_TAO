#include "BasicOptions.hpp"

#include <sstream>
#include <algorithm>

using namespace OCML;

/*
 *  class BooleanOption implementation.
 */

/// @see DescribedItem::type_string
const char*
BooleanOption::type_string() const
{
  return "BooleanOption";
}

/// @see BasicOption::get_str_value.
std::string
BooleanOption::get_str_value() const
{
  if (get())
    return "true";
  else
    return "false";
}

/*
 *  class StringOption implementation.
 */

/// @see DescribedItem::type_string
const char*
StringOption::type_string() const
{
  return "StringOption";
}

/// @see BasicOption::get_str_value
std::string
StringOption::get_str_value() const
{
  return get();
}

/*
 *  class IntegerOption implementation.
 */

/// @see DescribedItem::type_string
const char*
IntegerOption::type_string() const
{
  return "IntegerOption";
}

/// @see BasicOption::get_str_value
std::string
IntegerOption::get_str_value() const
{
  std::ostringstream os;
  os << get();
  return os.str();
}

/*
 *  class EnumOption implementation.
 */

/// @see DescribedItem::type_string
const char*
EnumOption::type_string() const
{
  return "EnumOption";
}

/// @see BasicOption::get_str_value
std::string
EnumOption::get_str_value() const
{
  return get();
}

/**
 * Checks if the given value is in the value list. If not throws the
 * InvalidItem exception, otherwise calls the BasicOption::set to set the
 * given value.
 */
void
EnumOption::set(EnumOption::value_type value)
{
  if (std::find(items_.begin(), items_.end(), value) == items_.end())
    throw InvalidItem(value);

  Base_Option<value_type>::set(value);
}

EnumOption::item_iterator
EnumOption::begin_items()
{
  return items_.begin();
}

EnumOption::item_iterator
EnumOption::end_items()
{
  return items_.end();
}
