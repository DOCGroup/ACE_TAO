/* -*- C++ -*- */

//=============================================================================
/**
 * @file      RawComponent.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef RAWCOMPONENT_H
#define RAWCOMPONENT_H

#ifdef BUILDER_OBJECT_NETWORK
#error   This file should only be included in the RAW COM configurations
#endif

#include <string>
#include <set>

//=============================================================================
/**
 * @class RawComponent
 */
//=============================================================================

class RawComponent
{
public:
  /// Constructor.
  RawComponent (void);

  /// Destructor.
  virtual ~RawComponent (void);

  /* GME specific members */
  CComPtr <IMgaAddOn> addon;
  bool interactive;

  /* RawComponent specific methods */
  STDMETHODIMP Initialize (struct IMgaProject *);
  STDMETHODIMP Invoke (IMgaProject*, IMgaFCOs *, long );
  STDMETHODIMP InvokeEx (IMgaProject *, IMgaFCO *, IMgaFCOs *, long);
  STDMETHODIMP ObjectsInvokeEx (IMgaProject *, IMgaObject *, IMgaObjects *, long);
  STDMETHODIMP get_ComponentParameter (BSTR, VARIANT *);
  STDMETHODIMP put_ComponentParameter (BSTR, VARIANT);
  STDMETHODIMP GlobalEvent (globalevent_enum);
  STDMETHODIMP ObjectEvent (IMgaObject *, unsigned long, VARIANT);

private:
  /**
   * Create a UUID for the FCO.
   *
   * @param[in]       fco         Target FCO.
   */
  void create_uuid (IMgaFCO * fco);

  /**
   * Verify the UUID of an FCO.
   *
   * @param[in]       fco         Target FCO.
   */
  void verify_uuid (IMgaFCO * fco);

  /// This verifies all UUID's in the project.
  void verify_all_uuids (void);

  void handle_pending (void);

  /**
   * Get the UUID from a FCO object.
   *
   * @param[in]     fco       Source FCO
   * @param[out]    attr      Pointer to the UUID attribute.
   * @param[out]    status    The status of the attribute.
   * @retval        true      The attribute was found.
   * @retval        false     The attribute was not found.
   */
  bool get_uuid_i (IMgaFCO * fco, IMgaAttribute ** attr, long & status);

  /// The project is in import mode.
  int importing_;

  /// Collection of pending FCO's to validate.
  CInterfaceList <IMgaFCO> pending_;

  /// Interface pointer to the hosting project.
  CComPtr <IMgaProject> project_;

  /// PICML types with a UUID attribute.
  static std::set <std::wstring> picml_types_;
};


#endif /* RAWCOMPONENT_H */
