// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL Backend
//
// = FILENAME
//    be_factory.h
//
// = DESCRIPTION
//    Defines a factory that returns a specialized output stream object that
//    understands a specific Front End
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef TAO_BE_FACTORY_H
#define TAO_BE_FACTORY_H

#include "ace/Singleton.h"
#include "ace/Synch.h"

class TAO_OutStream;

class TAO_OutStream_Factory
{
  // =TITLE
  //   TAO_OutStream_Factory
  // =DESCRIPTION
  //   factory to produce specialized instances of the output stream objects
public:
  enum TAO_OutStream_Type
  {
    TAO_SUNSOFT,
    TAO_FLICK
  };

  TAO_OutStream_Factory (void);
  // constructor

  ~TAO_OutStream_Factory (void);
  // destructor

  TAO_OutStream *make_outstream (void);
  // make the specialized out stream class

  int set_stream_type (TAO_OutStream_Type t);
  // set the stream type

private:
  TAO_OutStream_Type  strm_type_;
};

typedef ACE_Singleton<TAO_OutStream_Factory, ACE_SYNCH_RECURSIVE_MUTEX> TAO_OUTSTREAM_FACTORY;
// Singleton instance of the OutStream factory

#endif /* if !defined _TAO_BE_FACTORY_H_ */
