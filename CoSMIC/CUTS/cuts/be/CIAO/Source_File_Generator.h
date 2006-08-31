// $Id$

//=============================================================================
/**
 * @file      Source_File_Generator.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_BE_CIAO_SOURCE_FILE_GENERATOR_H_
#define _CUTS_BE_CIAO_SOURCE_FILE_GENERATOR_H_

#include "UDM_CIAO_Export.h"
#include "File_Generator_Base.h"
#include "cuts/be/String_Set.h"

//=============================================================================
/**
 * @class CUTS_CIAO_Source_File_Generator
 *
 * CIAO source file generator for the CUTS-PICML backend.
 */
//=============================================================================

class CUTS_UDM_CIAO_Export CUTS_CIAO_Source_File_Generator :
  public CUTS_CIAO_File_Generator_Base
{
public:
  /// Default constructor.
  CUTS_CIAO_Source_File_Generator (void);

  /// Destructor.
  virtual ~CUTS_CIAO_Source_File_Generator (void);

  virtual void open (const std::string & pathname);

  /**
   * Write the preamble for the file.
   *
   * @param[in]     container       Top-most container of implementation.
   */
  virtual void write_preamble (
    const PICML::ComponentImplementationContainer & container);

  virtual void write_component_begin (
    const PICML::Component & component);

  virtual void write_component_end (
    const PICML::Component & component);

  virtual void write_method_begin (
    const PICML::InEventPort & sink);

  virtual void write_method_begin (
    const PICML::ProvidedRequestPort & facet);

  virtual void write_method_begin (
    const PICML::PeriodicAction & periodic);

  virtual void write_method_end (void);

  virtual void write_variable (
    const PICML::WorkerType & worker_type);

  virtual void write_variable (
    const PICML::Variable & variable);

  virtual void write_component_factory_begin (
    const PICML::ComponentFactory &,
    const PICML::Component &);

protected:
  void write_variable (const PICML::InEventPort & sink);
  void write_register_obv_factory (const std::string & event);

  virtual void write_set_session_context (const PICML::Component & component);

  virtual void write_ciao_preactivate (const PICML::Component & component);

  virtual void write_ccm_activate (const PICML::Component & component);

  virtual void write_ciao_postactivate (const PICML::Component & component);

  virtual void write_ccm_passivate (const PICML::Component & component);

  virtual void write_ccm_remove (const PICML::Component & component);

private:
  /// Collection of events for this component.
  CUTS_String_Set events_;
  CUTS_String_Set event_sinks_;
};

#if defined (__CUTS_INLINE__)
#include "Source_File_Generator.inl"
#endif

#endif  // !defined _CUTS_BE_CIAO_SOURCE_FILE_GENERATOR_H_
