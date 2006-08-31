// -*- C++ -*-

#ifndef OCML_INTERPRETER_H
#define OCML_INTERPRETER_H

#include <string>
#include "OCMLBonX.h"
#include <xercesc/dom/DOM.hpp>

class OCMLInterpreter
{
public:
  OCMLInterpreter();

  ~OCMLInterpreter();

  void write(const char* file_name, xercesc::DOMDocument* doc);

//  xercesc::DOMDocument* create_document(std::set<BON::Model>& root_models);
  xercesc::DOMDocument*
    create_option_tree(std::set<OCML_BON::Option_Category>& categories);

  bool save_file(const char* title, std::string& file_name);

private:
  xercesc::DOMImplementation* impl_;
};

#endif // OCML_INTERPRETER_H
