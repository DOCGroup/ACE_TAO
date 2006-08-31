
inline std::string bool_as_string(bool value)
{
  if (value)
      return "true";
  else
      return "false";
}

inline void write_bool_str(bool value)
{
  if (value)
    std::cout << "true";
  else
    std::cout << "false";
}

