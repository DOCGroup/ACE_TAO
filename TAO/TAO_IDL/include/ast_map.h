/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

*/

#ifndef _AST_MAP_AST_MAP_HH
#define _AST_MAP_AST_MAP_HH

#include "ast_concrete_type.h"


class AST_Expression;
class AST_Type;

class TAO_IDL_FE_Export AST_Map : public virtual AST_ConcreteType
{
public:
  AST_Map (AST_Expression *max_size,
           AST_Type *key_bt,
           AST_Type *val_bt,
           UTL_ScopedName *n,
           bool local,
           bool abstract);

  virtual ~AST_Map () = default;

  virtual bool in_recursion (ACE_Unbounded_Queue<AST_Type *> &list);
  // Are we or the node represented by node involved in recursion.

  // Data Accessors.
  AST_Expression *max_size () const;

  AST_Type *key_type () const;
  AST_Type *value_type () const;

  /**
   * Returns the fully dealiased base type if it's a typedef. If it's not a
   * typedef, the it returns the same value as as key_type().
   */
  AST_Type *primitive_key_type () const;

  /**
   * Returns the fully dealiased base type if it's a typedef. If it's not a
   * typedef, the it returns the same value as as value_type().
   */
  AST_Type *primitive_value_type () const;

  virtual bool unbounded () const;
  // Is this map bounded or not.

  // Recursively called on valuetype to check for legal use as
  // a primary key. Overridden for valuetype, struct, map,
  // union, array, typedef, and interface.
  virtual bool legal_for_primary_key () const;

  // Is the element type a forward declared struct or union
  // that hasn't yet been fully defined?
  virtual bool is_defined ();

  // Cleanup method.
  virtual void destroy ();

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  static AST_Decl::NodeType const NT;

  /**
   * Get and Set Annotations on the key type
   */
  ///{
  AST_Annotation_Appls &key_type_annotations ();
  void key_type_annotations (const AST_Annotation_Appls &annotations);
  ///}

  /**
   * Get and Set Annotations on the value type
   */
  ///{
  AST_Annotation_Appls &value_type_annotations ();
  void value_type_annotations (const AST_Annotation_Appls &annotations);
  ///}

private:
  // Data.
  AST_Expression *pd_max_size;
  // Maximum map size.

  /**
  * Key type for map
  **/
  AST_Type *key_pd_type;

  /**
  * Value type for map
  **/
  AST_Type *value_pd_type;

  bool unbounded_;
  // Whether we are bounded or unbounded.

  bool owns_key_type_;
  // If our key type is anonymous array or map, we're
  // responsible for destroying it.
  bool owns_value_type_;
  // If our value type is anonymous array or map, we're
  // responsible for destroying it.

  /**
   * Annotations on the key type
   */
  AST_Annotation_Appls key_type_annotations_;

  /**
   * Annotations on the value type
   */
  AST_Annotation_Appls value_type_annotations_;
};

#endif           // _AST_MAP_AST_MAP_HH
