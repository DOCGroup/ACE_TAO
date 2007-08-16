// -*- C++ -*-


//=============================================================================
/**
 *  @file    Contained_i.h
 *
 *  $Id$
 *
 *  Contained servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_CONTAINED_I_H
#define TAO_CONTAINED_I_H

#include "orbsvcs/IFRService/IRObject_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Contained_i
 *
 * @brief TAO_Contained_i
 *
 * Abstract base class for all IR objects that are contained
 * by other IR objects.
 */
class TAO_IFRService_Export TAO_Contained_i : public virtual TAO_IRObject_i
{
public:
  /// Constructor.
  TAO_Contained_i (TAO_Repository_i *repo);

  /// Destructor.
  virtual ~TAO_Contained_i (void);

  /// Remove the repository entry.
  virtual void destroy (
    );

  virtual void destroy_i (
    );

  virtual char *id (
    );

  char *id_i (
    );

  virtual void id (
      const char *id
    );

  void id_i (
      const char *id
    );

  virtual char *name (
    );

  char *name_i (
    );

  virtual void name (
      const char *name
    );

  void name_i (
      const char *name
    );

  virtual char *version (
    );

  char *version_i (
    );

  virtual void version (
      const char *version
    );

  void version_i (
      const char *version
    );

  virtual CORBA::Container_ptr defined_in (
    );

  CORBA::Container_ptr defined_in_i (
    );

  virtual char *absolute_name (
    );

  char *absolute_name_i (
    );

  virtual CORBA::Repository_ptr containing_repository (
    );

  virtual CORBA::Contained::Description *describe (
    );

  virtual CORBA::Contained::Description *describe_i ();

  virtual void move (
      CORBA::Container_ptr new_container,
      const char *new_name,
      const char *new_version
    );

  /// Called from TAO_IFR_Service_Utils::name_exists.
  static int same_as_tmp_name (const char *name);

protected:
  /**
   * Engine for move() with an extra 'cleanup'
   * parameter. Since a section removal can be
   * recursive, this need be done only at the top
   * level.
   */
  void move_i (
      CORBA::Container_ptr new_container,
      const char *new_name,
      const char *new_version,
      CORBA::Boolean cleanup
    );

private:
  /// Check if <name> already exists in our container
  CORBA::Boolean name_exists (
      const char *name
    );

  /// Recursively update the scoped name of our contents.
  void contents_name_update (
      ACE_TString stem,
      ACE_Configuration_Section_Key key
    );

  /**
   * Mangle the names of references that are also
   * defined in the scope, so a name clash will
   * not occur when create_* is called. Create_*
   * unmangles the names.
   */
  void move_pre_process (
      CORBA::Container_ptr container,
      const char *contained_path,
      const char *name
    );

  /**
   * Recursively calls move_i for definitions,
   * as well as operations and attributes if
   * applicable.
   */
  void move_contents (
      CORBA::Container_ptr new_container
    );

private:
  static const char *tmp_name_holder_;
};

TAO_END_VERSIONED_NAMESPACE_DECL


#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_CONTAINED_I_H */
