inline void
write_info(OCML::Option_Value* value)
{
  bool result = false;

  if (!result)
    result = try_write_info<OCML::Boolean_Option_Trait>(value);
  if (!result)
    result = try_write_info<OCML::Integer_Option_Trait>(value);
  if (!result)
    result = try_write_info<OCML::String_Option_Trait>(value);
  if (!result)
    std::cerr << "Unable to downcast Option_Value" << std::endl;
}

inline bool
create_some_value<bool>::operator()()
{
  return true;
}

inline int
create_some_value<int>::operator()()
{
  return 3;
}

inline std::string
create_some_value<std::string>::operator()()
{
  return "test";
}

