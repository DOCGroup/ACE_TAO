#ifndef UTL_ANNOTATIONS_HEADER
#define UTL_ANNOTATIONS_HEADER

#include "TAO_IDL_FE_Export.h"

#include "ast_annotation_appl.h"

/**
 * Given an AST_Annotation_Appls, find the last annotation that matches the
 * given annotation declaration. Returns 0 if not found.
 *
 * "declaration" must be an absolutely scoped name or a pointer to the
 * AST_Annotation_Decl.
 */
///{
TAO_IDL_FE_Export AST_Annotation_Appl *UTL_find_annotation (
  AST_Annotation_Appls &annotations, AST_Annotation_Decl *annotation);
TAO_IDL_FE_Export AST_Annotation_Appl *UTL_find_annotation (
  AST_Annotation_Appls *annotations, AST_Annotation_Decl *annotation);
TAO_IDL_FE_Export AST_Annotation_Appl *UTL_find_annotation (
  AST_Annotation_Appls &annotations, const char *annotation);
TAO_IDL_FE_Export AST_Annotation_Appl *UTL_find_annotation (
  AST_Annotation_Appls *annotations, const char *annotation);
///}

#endif
