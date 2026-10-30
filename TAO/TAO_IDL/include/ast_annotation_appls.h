#ifndef AST_ANNOTATION_APPL_VECTOR_HEADER
#define AST_ANNOTATION_APPL_VECTOR_HEADER

#include "ace/Bound_Ptr.h"
#include "ace/Synch_Traits.h"
#include "ace/Vector_T.h"

#include "TAO_IDL_FE_Export.h"

#include <string>

class AST_Annotation_Appl;
class AST_Annotation_Decl;

/**
 * Container for AST_Annotation_Appl
 *
 * Uses ACE_Strong_Bound_Ptr because they can be shared between AST_Decl.
 */
class TAO_IDL_FE_Export AST_Annotation_Appls {
public:
//FUZZ: disable check_for_ACE_SYNCH_MUTEX
  using AST_Annotation_Appl_Ptr = ACE_Strong_Bound_Ptr<AST_Annotation_Appl, ACE_SYNCH_MUTEX>;
//FUZZ: enable check_for_ACE_SYNCH_MUTEX
  using AST_Annotation_Appl_Ptrs = ACE_Vector<AST_Annotation_Appl_Ptr>;
  using iterator = AST_Annotation_Appl_Ptrs::iterator;
  using const_iterator = AST_Annotation_Appl_Ptrs::const_iterator;

  AST_Annotation_Appls ();
  AST_Annotation_Appls (AST_Annotation_Appls const &other);
  ~AST_Annotation_Appls ();

  AST_Annotation_Appls &operator= (AST_Annotation_Appls const &other);

  /**
   * Add an AST_Annotation_Appl to the vector. This class assumes ownership of
   * the pointer which is managed by ACE_Strong_Bound_Ptr.
   */
  void add (AST_Annotation_Appl *appl);

  /**
   * Add all the AST_Annotation_Appls from another AST_Annotation_Appls to the
   * this one.
   */
  void add (AST_Annotation_Appls const &other);

  bool empty () const;
  size_t size () const;

  /// Iterate through vector_
  ///{
  iterator begin ();
  iterator end ();
  const_iterator begin () const;
  const_iterator end () const;
  ///}

  /**
   * Direct access to the AST_Annotation_Appl pointers by index.
   */
  AST_Annotation_Appl *operator[] (size_t index);

  /**
   * Return last AST_Annotation_Appl that applies the AST_Annotation_Decl
   * or return nullptr if there are none that do.
   */
  AST_Annotation_Appl *find (AST_Annotation_Decl *annotation);

  /**
   * Same as find (AST_Annotation_Decl*) except do a look up of the
   * AST_Annotation_Decl first.
   */
  AST_Annotation_Appl *find (std::string const& annotation);

private:
  AST_Annotation_Appl_Ptrs vector_;
};

#endif
