/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    sequence_cs.h
 *
 *  Concrete visitor for the Sequence class
 *  This one provides code generation for the Sequence node in the client
 *  stubs.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_MAP_MAP_CS_H_
#define _BE_VISITOR_MAP_MAP_CS_H_

/**
 * @class be_visitor_map_cs
 *
 * @brief be_visitor_map_cs
 *
 * This is a concrete visitor to generate the client stubs for
 * maps
 */
class TAO_OutStream;
class be_visitor_map_cs : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_map_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_map_cs ();

  /// visit map node
  virtual int visit_map (be_map *node);
};

#endif /* _BE_VISITOR_MAP_MAP_CS_H_ */
