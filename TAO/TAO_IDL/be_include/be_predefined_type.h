#ifndef _BE_PREDEFINED_TYPE_H
#define _BE_PREDEFINED_TYPE_H

// be_classes.hh
//
// Defines all classes for dummy BE

/*
 * BE_PredefinedType
 */
class	be_predefined_type : public virtual AST_PredefinedType {
public:
  // Operations
  be_predefined_type();
  be_predefined_type(AST_PredefinedType::PredefinedType t, UTL_ScopedName *n,
		     UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_predefined_type, AST_PredefinedType);
  DEF_NARROW_FROM_DECL(be_predefined_type);
};

#endif
