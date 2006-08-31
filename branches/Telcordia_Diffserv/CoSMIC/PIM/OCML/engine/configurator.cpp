// $Id$

#include "spirit_helpers.hpp"  // include this first

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

#include "configurator.hpp"

#include "tree_parser.hpp"
#include "value_parser.hpp"

#include "rules_grammar.hpp"

#ifdef WIN32
#define BIN_PATH "\\bin\\"
#else
#define BIN_PATH "/bin/"
#endif

namespace OCML
{

  Configurator* Configurator::instance_ = 0;

  /// Returns static instance, by creating if not yet created.
  /// @todo: The parameters are not used when called with an already existing
  ///        instance and there is no error when that happens.
  Configurator*
    Configurator::instance(const char* tree_file,
                           const char* rules, size_t rules_size,
                           const char* values, size_t values_size)
  {
    if (instance_ == 0)
    {
        new Configurator(tree_file, rules, rules_size, values, values_size);
    }
    return instance_;
  }

  Configurator* Configurator::instance()
  {
    return instance_;
  }

  Configurator::Configurator()
  {
     // THIS IS NOT USED AND PRIVATE.
  }

  Configurator::Configurator(const char* tree_file,
                             const char* rules, size_t rules_size,
                             const char* values, size_t values_size)
  {
    instance_ = this;
    load_options_tree(tree_file);
    load_values(values, values_size);
    load_rules(rules, rules_size);
  }

  void Configurator::load_options_tree(const char* tree_file)
  {
    // Read the options file from $COSMIC_ROOT/bin/tree_file.
    XercesAutoPtr<xercesc::DOMBuilder> parser(system_.new_parser());
    xercesc::DOMDocument *tree_doc = 0;

    char *buffer = getenv("COSMIC_ROOT");

    std::string tree_file_path;
    if (buffer)
    {
      // delete double quotes (if there are any)
      std::remove_copy(buffer, buffer + strlen(buffer),
                       std::back_inserter(tree_file_path), '"');
      tree_file_path += BIN_PATH;
    }
    tree_file_path += tree_file;

    // parse the options tree
    tree_doc = parser->parseURI(tree_file_path.c_str());

    if (tree_doc == 0)
    {
      throw "Config file not found.";
    }

    root_.reset(Option_Tree_Parser::parse_xml(tree_doc));

    constraint_engine_.reset(new Constraint_Engine(root_.get()));
    create_interfaces(root_.get());
  }

  void Configurator::load_rules(const char* rules, size_t size)
  {
    if (rules != 0)
    {
      std::string buffer(rules, rules + size);
      OCML::Rule_Parser::Syntax parser;
      boost::spirit::parse_info<> info =
        boost::spirit::parse(buffer.c_str(), parser, boost::spirit::space_p);

      if (!info.full)
        throw Rules_Parse_Error();

      if (constraint_engine_->validate() == 0)
      {
        throw Initialization_Constraint_Violation();
      }
    }
  }

  void Configurator::load_values(const char* values, size_t size)
  {
    XercesAutoPtr<xercesc::DOMBuilder> parser(system_.new_parser());
    xercesc::DOMDocument *value_doc = 0;
    value_doc =
      system_.parse_string(parser.get(), "OCMLValues", values, size);
  
    Option_Value_Parser::parse_xml(root_.get(), interfaces_, value_doc);
  }

  Configurator::~Configurator()
  {
    // Delete interfaces.
    for (std::map<const Option_Definition*, Option_Interface*>::iterator iter =
         interfaces_.begin(); iter != interfaces_.end(); ++iter)
    {
      delete iter->second;
    }
  }

  Option_Definition* Configurator::definition(const Option_Path& path, size_t i,
                                              Option_Category* root)
  {
    if (root == 0) root = root_.get();
    if (i == path.size() - 1)
    {
      for (Option_Category::option_iterator iter = root->begin_options();
           iter != root->end_options(); ++iter)
      {
        if (path[i] == (*iter)->name())
          return *iter;
      }
      return 0;
    }
    else
    {
      for (Option_Category::category_iterator iter = root->begin_categories();
           iter != root->end_categories(); ++iter)
      {
        if (path[i] == (*iter)->name())
          return definition(path, i+1, *iter);
      }
      return 0;
    }
  }

  /**
   *  Recursively traverse the categories and create corresponding interfaces.
   */
  void Configurator::create_interfaces(Option_Category* category)
  {
    for (Option_Category::option_iterator iter = category->begin_options();
         iter != category->end_options(); ++iter)
    {
      interfaces_[*iter] =
        Option_Interface_Factory::create_interface(constraint_engine_.get(), *iter); 
    }

    for (Option_Category::category_iterator iter = category->begin_categories();
         iter != category->end_categories(); ++iter)
    {
      create_interfaces(*iter);
    }
  }

  xercesc::DOMElement*
  Configurator::write_option(const Option_Definition* definition,
                             const Option_Interface* interface,
                             xercesc::DOMDocument* doc) const
  {
    xercesc::DOMElement* result = 0;

    if (interface->assigned())
    {
      XMLUnicodeString type_string = definition->type_string();
      XMLUnicodeString name = definition->name().c_str();
      XMLUnicodeString value = interface->str_value().c_str();

      result = doc->createElement(type_string);

      result->setAttribute(XMLUnicodeString("name"), name);
      xercesc::DOMText* text_node = doc->createTextNode(value);
      result->appendChild(text_node);
    }

    return result;
  }
 
  xercesc::DOMElement* 
  Configurator::write_category(const Option_Category* category,
                               xercesc::DOMDocument* doc) const
  {
    xercesc::DOMElement* result = 0;

    for (Option_Category::const_category_iterator iter =
           category->begin_categories();
         iter != category->end_categories(); ++iter)
    {
      xercesc::DOMElement* node = write_category(*iter, doc);

      if (node)
      {
        if (result == 0)
        {
          result = doc->createElement(XMLUnicodeString("OptionCategory"));
          result->setAttribute(XMLUnicodeString("name"),
                               XMLUnicodeString(category->name().c_str()));
        }

        result->appendChild(node);
      }
    }

    for (Option_Category::const_option_iterator iter =
           category->begin_options();
         iter != category->end_options(); ++iter)
    {
      std::map<const Option_Definition*, Option_Interface*>::const_iterator
        interface_iter = interfaces_.find(*iter);

      xercesc::DOMElement* node =
        write_option(*iter, interface_iter->second, doc);

      if (node)
      {
        if (result == 0)
        {
          result = doc->createElement(XMLUnicodeString("OptionCategory"));
          result->setAttribute(XMLUnicodeString("name"),
                               XMLUnicodeString(category->name().c_str()));
        }
        result->appendChild(node);
      }
    }

    return result;
  }

  std::string Configurator::write() const
  {
    XercesAutoPtr<xercesc::DOMDocument>
      new_doc(system_.impl()->createDocument(0, XMLUnicodeString("test"), 0));
    XercesAutoPtr<xercesc::DOMWriter> writer(system_.impl()->createDOMWriter());

    /// @todo: auto_ptr for node. 
    xercesc::DOMNode* node = write_category(root_.get(), new_doc.get()); 
    XMLUnicodeString result;
    if (node)
      result = writer->writeToString(*node);
  
    return result.str();
  }

} // namespace OCML

