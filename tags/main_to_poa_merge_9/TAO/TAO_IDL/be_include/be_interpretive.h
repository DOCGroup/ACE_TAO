// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL Backend
//
// = FILENAME
//    be_interpretive_visitor.h
//
// = DESCRIPTION
//    Defines a factory that returns a specialized visitor object based on the
//    code generation state. The visitors returned by this factory generate
//    stubs and skeletons that use interpretive form of marshaling
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

class TAO_Interpretive_Visitor_Factory: public TAO_Visitor_Factory
{
  // =TITLE
  //   TAO_Visitor_Factory
  //
  // =DESCRIPTION
  //   Factory that creates visitors that generate stubs/skeletons for
  //   interpretive form of marshaling
public:
  TAO_Interpretive_Visitor_Factory (void);
  // constructor

  virtual ~TAO_Interpretive_Visitor_Factory (void);
  // destructor

  virtual be_visitor *make_visitor (TAO_CodeGen::CG_STATE);
  // create the right visitor

};
