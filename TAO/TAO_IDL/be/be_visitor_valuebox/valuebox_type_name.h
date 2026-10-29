/* -*- c++ -*- */

#ifndef TAO_IDL_VALUEBOX_TYPE_NAME_H
#define TAO_IDL_VALUEBOX_TYPE_NAME_H

#include <memory>
#include <string>
#include <string_view>

namespace
{
  std::string
  valuebox_type_name (std::string_view prefix,
                     std::string_view name,
                     std::string_view suffix = {})
  {
    bool const is_const = prefix.substr (0, 6) == "const ";
    std::string type_name (is_const ? prefix.substr (6) : prefix);
    type_name.append (name);

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

    type_name.append (suffix);
    return type_name;
  }

  inline void
  emit_valuebox_type_name (TAO_OutStream *os,
                           std::string_view prefix,
                           UTL_IdList *name,
                           std::string_view suffix)
  {
    std::string const name_string = name->get_string_copy ();
    *os << valuebox_type_name (prefix, name_string, suffix);
  }
}

#endif /* TAO_IDL_VALUEBOX_TYPE_NAME_H */
