// $Id$

namespace OCML
{

  inline Option_Category*
  Configurator::root_category()
  {
    return root_.get();
  }

  inline Option_Interface*
  Configurator::interface(Option_Definition* definition)
  {
    return interfaces_[definition];
  }

  inline Constraint_Engine*
  Configurator::constraint_engine()
  {
    return constraint_engine_.get();
  }

} // namespace OCML

