/**
 *  @file    Dump_T.h
 *
 *  @author Doug Schmidt
 */

#ifndef ACE_DUMP_T_H
#define ACE_DUMP_T_H
#include /**/ "ace/pre.h"

#include "ace/Dump.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Dumpable_Adapter
 *
 * @brief
 * This class inherits the interface of the abstract ACE_Dumpable
 * class and is instantiated with the implementation of the
 * concrete component class @c Concrete.
 *
 * This design is similar to the Adapter and Decorator patterns
 * from the ``Gang of Four'' book.  Note that @c Concrete
 * need not inherit from a common class since ACE_Dumpable
 * provides the uniform virtual interface!
 */
template <class Concrete>
class ACE_Dumpable_Adapter : public ACE_Dumpable
{
public:
  ACE_Dumpable_Adapter (Concrete const *t);
  ~ACE_Dumpable_Adapter () override;

  /// Concrete dump method (simply delegates to the dump() method of
  /// @c Concrete).
  void dump () const override;

  /// Delegate to methods in the Concrete class.
  Concrete *operator->() const;

private:
  /// Pointer to @c this of @c Concrete.
  Concrete const *this_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

// Some useful macros for conditionally compiling this feature...
#if defined (ACE_NDEBUG)
#define ACE_REGISTER_OBJECT(CLASS)
#define ACE_REMOVE_OBJECT
#else
#define ACE_REGISTER_OBJECT(CLASS) \
        ACE_ODB::instance ()->register_object \
          (new ACE_Dumpable_Adapter<CLASS> (this));
#define ACE_REMOVE_OBJECT \
        ACE_ODB::instance ()->remove_object    \
          ((void *) this);
#endif /* ACE_NDEBUG */

#include "ace/Dump_T.cpp"

#include /**/ "ace/post.h"
#endif /* ACE_DUMP_T_H */
