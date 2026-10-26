/* -*- c++ -*- */

#ifndef TAO_IDL_VALUEBOX_TYPE_NAME_H
#define TAO_IDL_VALUEBOX_TYPE_NAME_H

#include <memory>
#include <string>

namespace
{
  std::string
  valuebox_type_name (std::string const &prefix,
                     std::string const &name,
                     std::string const &suffix = "")
  {
    bool is_const = prefix.compare (0, 6, "const ") == 0;
    std::string type_name = is_const ? prefix.substr (6) : prefix;
    type_name += name;

    if (is_const)
      {
        std::string::size_type star = type_name.find ('*');

        if (star == std::string::npos)
          {
            type_name += " const";
          }
        else
          {
            std::string::size_type position = star;
            while (position > 0 && type_name[position - 1] == ' ')
              {
                --position;
              }

            type_name.insert (position, " const");
            if (position == star)
              {
                type_name.insert (position + 6, " ");
              }
          }
      }

    type_name += suffix;
    return type_name;
  }

  void
  emit_valuebox_type_name (TAO_OutStream *os,
                           std::string const &prefix,
                           UTL_IdList *name,
                           std::string const &suffix)
  {
    std::unique_ptr<char[]> name_string (name->get_string_copy ());
    *os << valuebox_type_name (prefix, name_string.get (), suffix).c_str ();
  }
}

#endif /* TAO_IDL_VALUEBOX_TYPE_NAME_H */
