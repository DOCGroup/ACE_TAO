
//=============================================================================
/**
 *  @file    array_cs.h
 *
 *  $Id$
 *
 *  Visitors for generation of code for Arrays in client stubs.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ARRAY_CS_H_
#define _BE_VISITOR_ARRAY_CS_H_

// ************************************************************
// class be_visitor_array_cs
// ************************************************************

/**
 * @class be_visitor_array_cs
 *
 * @brief be_visitor_array_cs
 *
 * Code to be generated for arrays in the client stub file
 */
class be_visitor_array_cs : public be_visitor_array
{
public:
  /// constructor
  be_visitor_array_cs (be_visitor_context *ctx);

  /// destructor
  virtual ~be_visitor_array_cs (void);

  /// visit the array node
  virtual int visit_array (be_array *node);
};

#endif /* _BE_VISITOR_ARRAY_CS_H_ */
