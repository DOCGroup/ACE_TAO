template <typename OPTION_VALUE_TYPE>
OCML::Basic_Option_Value<OPTION_VALUE_TYPE>*
try_create_option_value(OCML::Option_Definition* option)
{
  if (dynamic_cast<OCML::Basic_Option_Definition<OPTION_VALUE_TYPE>*>(option))
    return new OCML::Basic_Option_Value<OPTION_VALUE_TYPE>(
          create_some_value<typename OPTION_VALUE_TYPE::value_type>()());
  else
    return 0;
}

