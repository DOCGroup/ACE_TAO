// $Id$

//
// write_single_line_comment
//
CUTS_INLINE
void CUTS_BE_File_Generator::
write_single_line_comment (const std::string &)
{

}

//
// write_component_begin
//
CUTS_INLINE
void CUTS_BE_File_Generator::
write_component_begin (const PICML::Component & component)
{

}

//
// write_component_end
//
CUTS_INLINE
void CUTS_BE_File_Generator::
write_component_end (const PICML::Component & component)
{

}

//
// write_component_factory_begin
//
CUTS_INLINE
void CUTS_BE_File_Generator::
write_component_factory_begin (const PICML::ComponentFactory &,
                               const PICML::Component &)
{

}

//
// write_component_factory_end
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_component_factory_end (void)
{

}

//
// write_preamble
//
CUTS_INLINE
void CUTS_BE_File_Generator::
write_preamble (const PICML::ComponentImplementationContainer &)
{

}

//
// write_postamble
//
CUTS_INLINE
void CUTS_BE_File_Generator::
write_postamble (const PICML::ComponentImplementationContainer &)
{

}

//
// open_file
//
CUTS_INLINE
void CUTS_BE_File_Generator::open (const std::string & path)
{

}

//
// close_file
//
CUTS_INLINE
void CUTS_BE_File_Generator::close (void)
{

}

//
// write_monolithic_begin
//
CUTS_INLINE
void CUTS_BE_File_Generator::
write_monolithic_begin (const PICML::MonolithicImplementation &)
{

}

//
// write_monolithic_end
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_monolithic_end (void)
{

}

//
// init
//
CUTS_INLINE
void CUTS_BE_File_Generator::init (const CUTS_BE_Preprocess_Data * ppd)
{
  this->ppd_ = ppd;
}

//
// preprocess_data
//
CUTS_INLINE
const CUTS_BE_Preprocess_Data &
CUTS_BE_File_Generator::preprocess_data (void) const
{
  // This should never be NULL!!!!!
  return *this->ppd_;
}

//
// write_include_file
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_include_file (const std::string & str)
{

}

//
// write_method_begin
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_method_begin (
  const PICML::InEventPort & sink)
{

}

//
// write_method_begin
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_method_begin (
  const PICML::ProvidedRequestPort & facet)
{

}

//
// write_method_begin
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_method_begin (
  const PICML::PeriodicAction & periodic)
{

}

//
// write_method_begin
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_method_begin (
  const PICML::ReadonlyAttribute & attr)
{

}

//
// write_method_begin
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_method_begin (
  const PICML::Attribute & attr)
{

}

//
// write_method_end
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_method_end (void)
{

}

//
// write_action_begin
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_action_begin (
  const PICML::Worker & parent,
  const PICML::Action & action)
{

}

//
// write_action_begin
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_action_begin (
  const PICML::OutputAction &)
{

}

//
// write_action_end
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_action_end (void)
{

}

//
// write_action_property
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_action_property (
  const PICML::Property &)
{

}

//
// write_postcondition
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_postcondition (
  const std::string & postcondition)
{

}

//
// write_precondition
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_precondition (
  const std::string & precondition)
{

}

//
// write_variables_begin
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_variables_begin (void)
{

}

//
// write_variable
//
CUTS_INLINE
void CUTS_BE_File_Generator::
write_variable (const PICML::Variable & variable)
{

}

//
// write_variable
//
CUTS_INLINE
void CUTS_BE_File_Generator::
write_variable (const PICML::WorkerType & worker_type)
{

}

//
// write_variable
//
CUTS_INLINE
void CUTS_BE_File_Generator::
write_variable (const PICML::ReadonlyAttribute & attribute)
{

}

//
// write_variable
//
CUTS_INLINE
void CUTS_BE_File_Generator::
write_variable (const PICML::Attribute & attribute)
{

}

//
// write_variables_end
//
CUTS_INLINE
void CUTS_BE_File_Generator::write_variables_end (void)
{

}
