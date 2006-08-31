template <typename OPTION_TRAIT>
void write_info(OCML::Basic_Option_Value<OPTION_TRAIT>* value)
{
  std::cout << OPTION_TRAIT::type_string() << ": ";
  if (value->is_null())
    std::cout << "NULL";
  else
    value->write(std::cout);
  std::cout << std::endl;
}

template <typename OPTION_TRAIT>
bool try_write_info(OCML::Option_Value* value)
{
  typedef OCML::Basic_Option_Value<OPTION_TRAIT> basic_option_value;

  basic_option_value* basic_value = dynamic_cast<basic_option_value*>(value);

  if (basic_value != 0)
    write_info(basic_value);

  return basic_value != 0;
}

template <typename VALUE_TYPE>
VALUE_TYPE create_some_value<VALUE_TYPE>::operator()()
{
  return VALUE_TYPE();
}

