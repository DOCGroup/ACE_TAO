#if !defined(BE_UNION_LABEL_H)
#define BE_UNION_LABEL_H

/*
 * BE_UnionLabel
 */
class	be_union_label : public virtual AST_UnionLabel {
public:
  // Operations
  be_union_label();
  be_union_label(AST_UnionLabel::UnionLabel ul, AST_Expression *v);
};

#endif
