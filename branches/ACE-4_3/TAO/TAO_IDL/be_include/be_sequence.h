#if !defined(BE_SEQUENCE_H)
#define BE_SEQUENCE_H

/*
 * BE_Sequence
 */
class	be_sequence : public virtual AST_Sequence {
public:
  // Operations
  be_sequence();
  be_sequence(AST_Expression *v, AST_Type *bt);

  // Narrowing
  DEF_NARROW_METHODS1(be_sequence, AST_Sequence);
  DEF_NARROW_FROM_DECL(be_sequence);
};

#endif
