#ifndef DEPENDENCYVISITOR_H
#define DEPENDENCYVISITOR_H

#include "IDMLBonExtension.h"

namespace IDML
{
  class DependencyVisitor
  {
  public :
		void visitOrderableImpl( const Orderable& object );

  private:
	  void update_depends_lists( const Orderable& object );
    void set_include_components_idl( const BON::Model& mod );
    void set_include_orb_idl( const BON::Reference& ref );
    bool match_scopes( BON::FCO target_obj,
                       BON::Model target_scope,
                       BON::FCO& try_obj,
                       BON::Model try_scope );
    File get_containing_file( const BON::FCO& fco );
    void discover_include( const Orderable& object,
                           const BON::FCO& ref );
    void discover_ref_manage( const BON::Reference& ref ); 
  };
}

#endif // DEPENDENCYVISITOR_H
