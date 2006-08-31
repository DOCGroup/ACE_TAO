// $Id$

namespace OCML
{

  /// Initializes the default value with it's default constructor.
  template <typename OPTION_TRAIT>
  Basic_Option_Definition<OPTION_TRAIT>::
  Basic_Option_Definition(const char* name, const char* description)
    : Option_Definition(name, description)
  {
  }

  /// Initializes the default value with the given parameter.
  template <typename OPTION_TRAIT>
  Basic_Option_Definition<OPTION_TRAIT>::
  Basic_Option_Definition(const char* name, const char* description,
                          value_type option_default_value)
    : Option_Definition(name, description),
      default_value_(option_default_value)
  {
  }

  template <typename OPTION_TRAIT>
  const typename Basic_Option_Definition<OPTION_TRAIT>::value_type&
  Basic_Option_Definition<OPTION_TRAIT>::
  default_value() const
  {
    return default_value_;
  }

  template <typename OPTION_TRAIT>
  const char*
  Basic_Option_Definition<OPTION_TRAIT>::
  type_string() const
  {
    return option_trait::type_string();
  }

} // namespace OCML
