/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/IFR_Service
//
// = FILENAME
//    Contained_i.h
//
// = DESCRIPTION
//    Contained servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_CONTAINED_I_H
#define TAO_CONTAINED_I_H

#include "IRObject_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Contained_i : public virtual TAO_IRObject_i
{
  // = TITLE
  //    TAO_Contained_i
  //
  // = DESCRIPTION
  //    Abstract base class for all IR objects that are contained
  //    by other IR objects.
  //
public:
  TAO_Contained_i (TAO_Repository_i *repo,
                   ACE_Configuration_Section_Key section_key);
  // Constructor.

  virtual ~TAO_Contained_i (void);
  // Destructor.

  virtual void destroy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Remove the repository entry.

  virtual void destroy_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char *id (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  char *id_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void id (
      const char *id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void id_i (
      const char *id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char *name (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  char *name_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void name (
      const char *name
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void name_i (
      const char *name
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char *version (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  char *version_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void version (
      const char *version
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void version_i (
      const char *version
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_Container_ptr defined_in (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_Container_ptr defined_in_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char *absolute_name (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  char *absolute_name_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_Repository_ptr containing_repository (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_Repository_ptr containing_repository_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_Contained::Description *describe (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Pure virtual.

  virtual CORBA_Contained::Description *describe_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Pure virtual.

  virtual void move (
      CORBA_Container_ptr new_container,
      const char *new_name,
      const char *new_version
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  void move_i (
      CORBA_Container_ptr new_container,
      const char *new_name,
      const char *new_version,
      CORBA::Boolean cleanup
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Engine for move() with an extra 'cleanup'
  // parameter. Since a section removal can be
  // recursive, this need be done only at the top
  // level.

private:
  CORBA::Boolean name_exists (
      const char *name
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Check if <name> already exists in our container

  void contents_name_update (
      ACE_TString stem,
      ACE_Configuration_Section_Key key
    );
  // Recursively update the scoped name of our contents.

  void move_pre_process (
      CORBA_Container_ptr container,
      const char *contained_path,
      const char *name
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Mangle the names of references that are also
  // defined in the scope, so a name clash will
  // not occur when create_* is called. Create_*
  // unmangles the names.

  void move_contents (
      CORBA_Container_ptr new_container
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Recursively calls move_i for definitions,
  // as well as operations and attributes if
  // applicable.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_CONTAINED_I_H */
