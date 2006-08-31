inline OCML::Option_Value*
create_option_value(OCML::Option_Definition* option)
{
  OCML::Option_Value* result = 0;
 
  if (result == 0)
    result = try_create_option_value<OCML::Integer_Option_Trait>(option);
  if (result == 0)
    result = try_create_option_value<OCML::Boolean_Option_Trait>(option);
  if (result == 0)
    result = try_create_option_value<OCML::String_Option_Trait>(option);
  if (result == 0)
    std::cerr << "Cannot downcast option definition\n";

  return result;
}

inline option_value_map* create_values(OCML::Option_Category* category)
{
  option_value_map* result = new option_value_map;
  for (OCML::Option_Category::option_iterator iter = category->begin_options();
       iter != category->end_options(); ++iter)
  {
    (*result)[*iter] = create_option_value(*iter);
  }
  return result;
}

inline void delete_values(option_value_map* values)
{
  for (option_value_map::iterator iter = values->begin();
       iter != values->end(); ++iter)
  {
    delete iter->second;
  }
}

inline void clear_values(option_value_map* values)
{
  for (option_value_map::iterator iter = values->begin();
       iter != values->end(); ++iter)
  {
    iter->second->clear();
  }
}

inline void set_values(option_value_map* values)
{
  for (option_value_map::iterator iter = values->begin();
       iter != values->end(); ++iter)
  {
    iter->second->set(create_option_value(iter->first));
  }
}

inline void write_info(option_value_map* values)
{
  for (option_value_map::iterator iter = values->begin();
       iter != values->end(); ++iter)
  {
    std::cout << "Option:" << std::endl;
    write_info(iter->first);
    std::cout << "Value:" << std::endl;
    write_info(iter->second);
  }
}
