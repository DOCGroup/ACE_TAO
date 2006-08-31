#ifndef CONFIGURATOR_UI_HPP
#define CONFIGURATOR_UI_HPP

#include "CONFIGURATOR_UI_Export.h"
#include <string>

class CONFIGURATOR_UI_Export Configurator_Dialog {
public:
  Configurator_Dialog() { }

  std::string show(const std::string& tree_file,
                   const std::string& values,
                   const std::string& rule_file);
};

#endif // CONFIGURATOR_UI_HPP
