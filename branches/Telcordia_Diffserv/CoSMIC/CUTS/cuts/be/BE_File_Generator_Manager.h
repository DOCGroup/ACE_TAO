// -*- C++ -*-

//=============================================================================
/**
 * @class     BE_File_Generator_Manager.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_BE_FILE_GENERATOR_MANAGER_H_
#define _CUTS_BE_FILE_GENERATOR_MANAGER_H_

#include "Preprocess_Data.h"
#include <stack>

// Forward decl.
class CUTS_BE_File_Generator;

//=============================================================================
/**
 * @class CUTS_BE_File_Generator_Manager
 *
 * Manager class for generating files from PICML models.
 */
//=============================================================================

class CUTS_BE_Export CUTS_BE_File_Generator_Manager :
  public PICML::Visitor
{
public:
  /// Default constructor.
  CUTS_BE_File_Generator_Manager (void);

  /**
   * Initialize the file generator manager.
   *
   * @param[in]       outdir      Output directory.
   */
  void init (const std::string & outdir);

  /// Destructor.
  virtual ~CUTS_BE_File_Generator_Manager (void);

  /**
   * Insert a new generator into the manager. Once the generator
   * has been installed into the manager, it manages all its
   * memory.
   *
   * @param[in]     generator     Pointer to the new generator.
   */
  void insert (CUTS_BE_File_Generator * generator);

  /// Remove all the generators from the manager.
  void remove_all (void);

  const CUTS_BE_Preprocess_Data & preprocess_data (void);

protected:
  virtual void Visit_ComponentImplementationContainer (
    const PICML::ComponentImplementationContainer & container);

  virtual void Visit_MonolithicImplementation (
    const PICML::MonolithicImplementation & mono);

  virtual void Visit_Component (
    const PICML::Component & component);

  virtual void Visit_InEventPort (
    const PICML::InEventPort & sink);

  virtual void Visit_ProvidedRequestPort (
    const PICML::ProvidedRequestPort & facet);

  virtual void Visit_Input (
    const PICML::Input &);

  virtual void Visit_InputAction (
    const PICML::InputAction & action);

  virtual void Visit_PeriodicAction (
    const PICML::PeriodicAction &);

  virtual void Visit_Action (
    const PICML::Action & action);

  virtual void Visit_OutputAction (
    const PICML::OutputAction & action);

  virtual void Visit_CompositeAction (
    const PICML::CompositeAction & action);

  virtual void Visit_Effect (
    const PICML::Effect & effect);

  virtual void Visit_State (
    const PICML::State & state);

  virtual void Visit_Property (
    const PICML::Property & property);

  virtual void Visit_Transition (
    const PICML::Transition & transition);

  virtual void Visit_Variable (
    const PICML::Variable & variable);

  virtual void Visit_WorkerType (
    const PICML::WorkerType & worker_type);

private:
  /// Clear all the component dependent information.
  void clear (void);

  /**
   * Get the factory for the component.
   */
  PICML::ComponentFactory
    get_component_factory (const PICML::Component & component);

  /// Output directory for the manager.
  std::string outdir_;

  /// Preprocess data for the components.
  CUTS_BE_Preprocess_Data ppd_;

  /// Flag that determines if effects should be ignored.
  bool ignore_effects_;

  /// Current depth of the flow control.
  int depth_;

  /// Type definition for a ordered collection of return states
  typedef std::stack <PICML::State> Holding_State;

  /// Collection of ordered return states.
  Holding_State holding_state_;

  typedef std::stack <PICML::InputAction> Call_Stack;

  Call_Stack call_stack_;

  /// Type definition for the collection of file generators.
  typedef std::set <CUTS_BE_File_Generator *> File_Generator_Set;

  /// Collection of file generators.
  File_Generator_Set generators_;

  // prevent the following operations
  CUTS_BE_File_Generator_Manager (const CUTS_BE_File_Generator_Manager &);
  const CUTS_BE_File_Generator_Manager & operator = (const CUTS_BE_File_Generator_Manager &);
};

#if defined (__CUTS_INLINE__)
#include "BE_File_Generator_Manager.inl"
#endif

#endif  // !defined _CUTS_BE_FILE_GENERATOR_MANAGER_H_
