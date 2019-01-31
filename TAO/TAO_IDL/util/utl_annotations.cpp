#include "utl_annotations.h"

AST_Annotation_Appl *UTL_find_annotation (
  AST_Annotation_Appls &annotations, AST_Annotation_Decl *annotation)
{
  AST_Annotation_Appl *result = 0;
  if (annotation)
    {
      for (AST_Annotation_Appls::iterator i = annotations.begin ();
        i != annotations.end (); ++i)
        {
          AST_Annotation_Appl *appl = i->get ();
          if (appl && appl->annotation_decl () == annotation)
            {
              result = appl;
            }
        }
    }
  return result;
}

AST_Annotation_Appl *UTL_find_annotation (
  AST_Annotation_Appls &annotations, const char *annotation)
{
  AST_Annotation_Appl *result = 0;
  if (annotation)
    {
      result = UTL_find_annotation (
        annotations,
        AST_Annotation_Decl::narrow_from_decl (
          idl_global->scopes ().bottom ()->lookup_by_name (annotation)));
    }
  return result;
}
