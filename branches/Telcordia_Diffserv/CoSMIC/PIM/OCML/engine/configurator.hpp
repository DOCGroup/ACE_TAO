/**
 * @file Configurator.hpp
 *
 * Definition of the boolean edit control.
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef CONFIGURATOR_HPP
#define CONFIGURATOR_HPP

#include <string>
#include <memory>

#include <common/xerces_helpers.hpp>

#include "definitions.hpp"
#include "option_interface.hpp"

namespace OCML
{

  typedef std::vector<std::string> Option_Path;
  
  /**
   * The main class of the OCML configurator library.
   * 
   * <B> Contains </B>
   * <UL>
   *   <LI> The options tree </LI>
   *   <LI> Constraint engine </LI>
   *   <LI> Option interfaces </LI>
   * </UL>
   *
   * Also handles loading the XML files (the options tree file and
   * option values) and storing the option values.
   */
  class Configurator
  {
  public:
    /// Thrown on errors in constructor on XML errors.
    class InitializationError { };

    /// Thrown on errors occured when parsing the rules.
    class Rules_Parse_Error { };

    /// Thrown on constraint violation while loading the options.
    class Initialization_Constraint_Violation { };

  private:
    /// Default constructor, empty option tree.
    Configurator();

    /// Parses the Options tree.
    Configurator(const char* tree_file,
                 const char* rules, size_t rules_size,
                 const char* values, size_t values_size);

  public:
    OCML_Engine_Export ~Configurator();

    /// Implements the singleton pattern.
    OCML_Engine_Export static Configurator* instance(const char* tree_file,
                                                     const char* rules, size_t rules_size,
                                                     const char* values, size_t values_size);

    OCML_Engine_Export static Configurator* instance();

    /// Exports the option tree into the return value as XML tree.
    OCML_Engine_Export std::string write() const;
  
    /// Returns the root category.
    OCML_Engine_Export Option_Category* root_category();

    OCML_Engine_Export Option_Definition* definition(const Option_Path& path, size_t i = 0,
                                                     Option_Category* root = 0);

    OCML_Engine_Export Option_Interface* interface(Option_Definition* definition);

    OCML_Engine_Export Constraint_Engine* constraint_engine();

  private:
    /*
     *  The following tree are called once by the constructor.
     */
    void load_options_tree(const char* tree_file);

    void load_rules(const char* rules_file, size_t size);

    void load_values(const char* values, size_t size);
    
    /// Creates the interfaces.
    void create_interfaces(Option_Category* category);

    xercesc::DOMElement* write_option(const Option_Definition* definition,
                                      const Option_Interface* interface,
                                      xercesc::DOMDocument* doc) const;

    xercesc::DOMElement* write_category(const Option_Category* category,
                                        xercesc::DOMDocument* doc) const;
 
  private:
    /// Xerces environment. @see XercesSystem for more information.
    XercesSystem system_;
    /// The root of the option tree.
    std::auto_ptr<Option_Category> root_;
    /// The constraint manager.
    std::auto_ptr<Constraint_Engine> constraint_engine_;
    /// The option interfaces.
    std::map<const Option_Definition*, Option_Interface*> interfaces_;
    /// The singleton.
    static Configurator* instance_;
  };

} // namespace OCML

#include "configurator.ipp"

#endif // CONFIGURATOR_HPP

