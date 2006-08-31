namespace OCML
{

  template <typename OPTION_TRAIT>
  Basic_Option<OPTION_TRAIT>::
  Basic_Option(Option_Definition* option_definition,
               Constraint_Engine* constraint_engine)
    : Option(option_definition, constraint_engine)
  {
  }
    
  template <typename OPTION_TRAIT>
  bool Basic_Option<OPTION_TRAIT>::str_value(const char* str)
  {
    // Parse the string by using type info.
    value_type val = option_trait::parse_string(str);
    // Create the appropriate option value.
    Option_Value* option_value =
      (Option_Value*) new Basic_Option_Value<option_trait>(val);
    // Try to set the value, and return the assignment success result.
    bool result = value(option_value);

    return result;
  }

} // namespace OCML
