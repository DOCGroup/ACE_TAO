// file      : CCF/IDL2/SemanticGraph/Translation.ipp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      // ContainsPrincipal
      //
      //
      inline void ContainsPrincipal::
      set_left_node (TranslationUnit& n)
      {
        Contains::set_left_node (n);
      }

      inline void ContainsPrincipal::
      set_right_node (TranslationRegion& n)
      {
        Contains::set_right_node (n);
      }

      // ContainsImplied
      //
      //
      inline void ContainsImplied::
      set_left_node (TranslationUnit& n)
      {
        Contains::set_left_node (n);
      }

      inline void ContainsImplied::
      set_right_node (TranslationRegion& n)
      {
        Contains::set_right_node (n);
      }

      // ContainsRoot
      //
      //
      inline void ContainsRoot::
      set_left_node (TranslationRegion& n)
      {
        Contains::set_left_node (n);
      }

      inline void ContainsRoot::
      set_right_node (Root& n)
      {
        Contains::set_right_node (n);
      }


      // Includes
      //
      //
      inline void Includes::
      set_left_node (TranslationRegion& n)
      {
        Contains::set_left_node (n);
      }

      inline void Includes::
      set_right_node (TranslationRegion& n)
      {
        Contains::set_right_node (n);
      }
    }
  }
}
