template <typename OPTION_TYPE>
OPTION_TYPE* create_definition(const char* name, const char* descr,
                               typename OPTION_TYPE::value_type value)
{
  return new OPTION_TYPE(name, descr, value);
}
