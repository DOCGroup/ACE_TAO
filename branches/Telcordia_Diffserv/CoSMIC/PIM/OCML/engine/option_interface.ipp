// $Id$

namespace OCML
{

  /*
   * Constructor and destructor.
   */

  inline
  Option_Interface::Option_Interface(Constraint_Engine* constraint_engine,
                                     Option_Definition* definition,
                                     Option* option, Option_Value* value)
    : constraint_engine_(constraint_engine),
      definition_(definition),
      option_(option),
      flagged_(false),
      value_(value->duplicate())
  {
    constraint_engine_->register_listener(definition_, this);
  }

  inline
  Option_Interface::~Option_Interface()
  {
    constraint_engine_->unregister_listener(definition_);
    
    for (listener_index iter = listeners_.begin(); iter != listeners_.end();
         ++iter)
    {
      (*iter)->interface_destroyed();
    }
  }

  /*
   * Set methods.
   */

  inline bool
  Option_Interface::value(Option_Value* value)
  {
    bool result = option_->value(value);

    return result;
  }
  
  inline bool
  Option_Interface::clear()
  {
    bool result = option_->clear();
    return result;
  }

  inline bool
  Option_Interface::str_value(const char* str)
  {
    bool result = option_->str_value(str);
    return result;
  }

  /*
   * Get methods.
   */

  inline const Option_Value*
  Option_Interface::value() const
  {
    Option_Value* result = value_.get();
    return result;
  }

  inline bool
  Option_Interface::assigned() const
  {
    bool result = (value_.get() && !(value_->is_null()));
    return result;
  }

  inline bool
  Option_Interface::flagged() const
  {
    return flagged_;
  }

  inline std::string
  Option_Interface::str_value() const
  {
    std::string result = value_->str_value();
    return result;
  }

  /**
   * The register method specifies which option to be wathced by this
   * specific instance of the listener.
   */
  inline void
  Option_Interface::value_changed(bool flagged, Option_Value* value)
  {
    flagged_ = flagged;
    if (value->is_null())
      value_.reset();
    else
      value_.reset(value->duplicate());

    for (listener_index iter = listeners_.begin(); iter != listeners_.end();
         ++iter)
    {
      (*iter)->value_changed(flagged_, value_.get());
    }
  }
  
  inline Option_Interface::listener_index
  Option_Interface::register_listener(Option_Interface_Value_Change_Listener* l)
  {
    listeners_.push_front(l);
    return listeners_.begin();
  }

  inline void
  Option_Interface::unregister_listener(listener_index index)
  {
    listeners_.erase(index);
  }

  /*
   * Option_Interface_Factory inline methods.
   */
  
  inline Option_Interface*
  Option_Interface_Factory::create_interface(Constraint_Engine* engine,
                                             Option_Definition* definition)
  {
    Option_Interface* result = 0;

    if (dynamic_cast<Boolean_Option_Definition*>(definition))
    {
      result = new Option_Interface(engine, definition,
                                    new Boolean_Option(definition, engine),
                                    (Option_Value*) new Boolean_Option_Value);
    }
    else if (dynamic_cast<String_Option_Definition*>(definition))
    {
      result = new Option_Interface(engine, definition,
                                    new String_Option(definition, engine),
                                    (Option_Value*) new String_Option_Value);
    }

    else if (dynamic_cast<Integer_Option_Definition*>(definition))
    {
      result = new Option_Interface(engine, definition,
                                    new Integer_Option(definition, engine),
                                    (Option_Value*) new Integer_Option_Value);
    }
    else {
      std::cerr << "Option_Interface_Factory::create_interface: "
                << "Unable to cast Option_Definition."
                << std::endl;
    }

    return result;
 }
  
} // namespace OCML
