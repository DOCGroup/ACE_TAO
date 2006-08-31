/**
 * @file option_interface.hpp
 *
 * $Id$
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef _OPTION_INTERFACE_HPP_
#define _OPTION_INTERFACE_HPP_

#include "option.hpp"
#include "constraint_engine.hpp"

namespace OCML
{

  class Option_Interface_Value_Change_Listener
  {
  public:
    virtual void value_changed(bool flagged, Option_Value* value) = 0;
    virtual void interface_destroyed() = 0;
  };
  
  /**
   * Provides the get and set methods for the options.
   *
   * It is a thin wrapper for constraint engine value change listener and the
   * option. This class uses option to change the values and gets the changed
   * value via callback method of the value change listener. Keeps a copy of
   * the current value of the option.
   */
  class Option_Interface
    : public Constraint_Value_Change_Listener
  {
  private:
    typedef std::list<Option_Interface_Value_Change_Listener*> listeners_type;

  public:
    typedef listeners_type::iterator listener_index;

  public:
    OCML_Engine_Export Option_Interface(Constraint_Engine* constraint_engine,
                                        Option_Definition* definition,
                                        Option* option, Option_Value* value);

    OCML_Engine_Export virtual ~Option_Interface();

    OCML_Engine_Export bool value(Option_Value* value);
    OCML_Engine_Export bool clear();
    OCML_Engine_Export bool str_value(const char* str);

    OCML_Engine_Export const Option_Value* value() const;
    OCML_Engine_Export bool assigned() const;
    OCML_Engine_Export bool flagged() const;

    OCML_Engine_Export Option_Definition* definition() { return definition_; }

    OCML_Engine_Export std::string str_value() const;

    OCML_Engine_Export listener_index register_listener(Option_Interface_Value_Change_Listener* l);
    OCML_Engine_Export void unregister_listener(listener_index index);
    
    /**
     * The callback method which is invoked by the constraint manager whenever
     * the value of the option change.
     */
    OCML_Engine_Export virtual void value_changed(bool flagged, Option_Value* value);

  private:
    Constraint_Engine* constraint_engine_;
    Option_Definition* definition_;
    std::auto_ptr<Option> option_;
    bool flagged_;
    std::auto_ptr<Option_Value> value_;
    listeners_type listeners_;
  };

  class Option_Interface_Factory
  {
  public:
    static Option_Interface*
    create_interface(Constraint_Engine* engine,
                     Option_Definition* definition);
  };
  
} //namespace OCML

#include "option_interface.ipp"

#endif // _OPTION_INTERFACE_HPP_
