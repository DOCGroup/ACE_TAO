// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL Backend
//
// = FILENAME
//    be_visitor_factory.h
//
// = DESCRIPTION
//    Define an abstract visitor Factory and a bunch of concrete visitor
//    factories (whcih are singletons).
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef TAO_BE_VISITOR_FACTORY_H
#define TAO_BE_VISITOR_FACTORY_H

class TAO_Visitor_Factory
{
  // =TITLE
  //   TAO_Visitor_Factory
  //
  // =DESCRIPTION
  //   An abstract visitor factory class

public:

  TAO_Visitor_Factory (void);
  // constructor

  virtual ~TAO_Visitor_Factory (void);
  // destructor

  virtual be_visitor *make_visitor (be_visitor_context *) = 0;
  // make a visitor
};

class TAO_Common_Visitor_Factory : public TAO_Visitor_Factory
{
  // =TITLE
  //   TAO_Common_Visitor_Factory
  //
  // =DESCRIPTION
  //   A concrete visitor factory for generating the visitors irrsepective of
  //   whether we use compiled or interpretive marshaling

public:

  TAO_Common_Visitor_Factory (void);
  // constructor

  virtual ~TAO_Common_Visitor_Factory (void);
  // destructor

  virtual be_visitor *make_visitor (be_visitor_context *);
  // make the right visitor based on the context and code generation state
};

class TAO_Compiled_Visitor_Factory : public TAO_Visitor_Factory
{
  // =TITLE
  //   TAO_Compiled_Visitor_Factory
  //
  // =DESCRIPTION
  //   A concrete visitor factory for generating the visitors for stubs and
  //   skeletons using compiled marshaling.

public:

  TAO_Compiled_Visitor_Factory (void);
  // constructor

  virtual ~TAO_Compiled_Visitor_Factory (void);
  // destructor

  virtual be_visitor *make_visitor (be_visitor_context *);
  // make the right visitor based on the context and code generation state
};

// Singleton instance of the BE Visitor Factory
typedef ACE_Singleton<TAO_Common_Visitor_Factory, ACE_SYNCH_RECURSIVE_MUTEX> TAO_COMMON_VISITOR_FACTORY;
typedef ACE_Singleton<TAO_Compiled_Visitor_Factory, ACE_SYNCH_RECURSIVE_MUTEX> TAO_COMPILED_VISITOR_FACTORY;

#endif /* if !defined _TAO_BE_VISITOR_FACTORY_H_ */
