//
// $Id$
//

#ifndef _BE_VISITOR_INTERFACE_H
#define _BE_VISITOR_INTERFACE_H

#include "be_visitor.h"

// = DESCRIPTION
//   Interface code generation is complex, the client stubs require
//   two different class and the server also require other two.
//   Only onw of them includes whatever nested typedefs, exceptions
//   and or structures are defined in the interface.
//   To break this complexity a different visitor is used for each
//   case.
//
// = NOTE
//   Visitors were added recently into the compiler, so some
//   code is still generated using the old "state oriented" approach;
//   notably only the _collocated and the _stub classes are actually
//   generated using the visitor, in the other cases (the interface
//   class and the skeleton class) we still call back the old code
//   generation methods.
//

class be_visitor_interface_ch : public be_visitor
{
  // = TITLE
  //   Generate code for the client header.

public:
  be_visitor_interface_ch (void);
  virtual ~be_visitor_interface_ch (void);

  virtual int visit_interface (be_interface *node);
};

class be_visitor_interface_cs : public be_visitor
{
  // = TITLE
  //   Generate code for the client stubs (.cpp).

public:
  be_visitor_interface_cs (void);
  virtual ~be_visitor_interface_cs (void);

  virtual int visit_interface (be_interface *node);
};

class be_visitor_interface_ci : public be_visitor
{
  // = TITLE
  //   Generate code for the client inline (.cpp).

public:
  be_visitor_interface_ci (void);
  virtual ~be_visitor_interface_ci (void);

  virtual int visit_interface (be_interface *node);
};

class be_visitor_collocated_sh : public be_visitor
{
  // = TITLE
  //   Generate the "collocated" class declaration.

public:
  be_visitor_collocated_sh (void);
  virtual ~be_visitor_collocated_sh (void);

  virtual int visit_interface (be_interface *node);
  virtual int visit_operation (be_operation *node);
  // virtual int visit_argument (be_argument *node);
  // virtual int visit_attribute (be_attribute *node);
};

class be_visitor_collocated_ss : public be_visitor
{
  // = TITLE
  //   Generate the "collocated" class declaration.

public:
  be_visitor_collocated_ss (void);
  virtual ~be_visitor_collocated_ss (void);

  virtual int visit_interface (be_interface *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_argument (be_argument *node);
  // virtual int visit_attribute (be_attribute *node);

private:
  be_interface* current_interface_;
};

#endif // _BE_VISITOR_INTERFACE_H
