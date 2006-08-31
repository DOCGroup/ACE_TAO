// $Id$

namespace OCML
{

  /*
   *  Inline method implementations for Option_Reference.
   */
  
  inline
  Option_Reference::Option_Reference()
    : assigned_(false), allowed_(true), required_(false)
  {
  }

  inline bool
  Option_Reference::assigned() const
  {
    return assigned_;
  }

  inline bool
  Option_Reference::required() const
  {
    return required_;
  }

  inline bool
  Option_Reference::allowed() const
  {
    return allowed_;
  }

  inline void
  Option_Reference::assigned(bool val)
  {
    assigned_ = val;
  }

  inline void
  Option_Reference::required(bool val)
  {
    required_ = val;
  }

  inline void
  Option_Reference::allowed(bool val)
  {
    allowed_ = val;
  }

  inline void
  Option_Reference::clear()
  {
    assigned_ = false;
    required_ = false;
    allowed_ = true;
  }

  inline bool
  Option_Reference::flagged() const
  {
    return required_ && !assigned_;
  }

  /*
   *  Inline method implementations for Option_ReferenceFactory.
   */
  
  inline Boolean_Option_Reference*
  Option_Reference_Factory::create_boolean_option_reference()
  {
    return create_basic_option_reference<Boolean_Option_Trait>();
  }

  inline Integer_Option_Reference*
  Option_Reference_Factory::create_integer_option_reference()
  {
    return create_basic_option_reference<Integer_Option_Trait>();
  }

  inline String_Option_Reference*
  Option_Reference_Factory::create_string_option_reference()
  {
    return create_basic_option_reference<String_Option_Trait>();
  }
 
  inline Option_Reference*
  Option_Reference_Factory::
  create_option_reference(Option_Definition* definition)
  {
    Option_Reference* reference = 0;

    if (!reference)
      reference = try_create<Boolean_Option_Trait>(definition);
    if (!reference)
      reference = try_create<Integer_Option_Trait>(definition);
    if (!reference)
      reference = try_create<String_Option_Trait>(definition);
    if (!reference)
      throw Unknown_Option_Kind();

    return reference;
  }

} // namespace OCML
