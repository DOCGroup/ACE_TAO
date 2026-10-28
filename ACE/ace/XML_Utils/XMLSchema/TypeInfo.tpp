// file      : XMLSchema/TypeInfo.tpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>

namespace XMLSchema
{
  template <typename T, typename Base = XSCRT::Type>
  void add_info (XSCRT::ExtendedTypeInfoMap &map, T *)
  {
    XSCRT::TypeId const id (typeid (T));
    XSCRT::ExtendedTypeInfo info (id);

    info.add_base (XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (Base));

    map.insert (std::make_pair (id, info));
  }

  template <typename C>
  inline TypeInfoInitializer<C>::TypeInfoInitializer (XSCRT::ExtendedTypeInfoMap &map)
  {
    // XSCRT::Type
    {
      XSCRT::TypeId const id (typeid (XSCRT::Type));
      map.insert (std::make_pair (id, XSCRT::ExtendedTypeInfo (id)));
    }

    add_info<byte> (map, nullptr);
    add_info<unsignedByte> (map, nullptr);

    add_info<short_> (map, nullptr);
    add_info<unsignedShort> (map, nullptr);

    add_info<int_> (map, nullptr);
    add_info<unsignedInt> (map, nullptr);

    add_info<long_> (map, nullptr);
    add_info<unsignedLong> (map, nullptr);

    add_info<boolean> (map, nullptr);

    add_info<float_> (map, nullptr);
    add_info<double_> (map, nullptr);

    add_info<string<C>> (map, nullptr);

    add_info<normalizedString<C>, string<C>> (map, nullptr);
    add_info<token<C>, normalizedString<C>> (map, nullptr);
    add_info<NMTOKEN<C>, token<C>> (map, nullptr);
    add_info<Name<C>, token<C>> (map, nullptr);
    add_info<NCName<C>, Name<C>> (map, nullptr);
    add_info<QName<C>, Name<C>> (map, nullptr);

    add_info<ID<C>, NCName<C>> (map, nullptr);

    // IDREF_Base
    {
      XSCRT::TypeId const id (typeid (IDREF_Base));
      XSCRT::ExtendedTypeInfo info (id);

      info.add_base (XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (XSCRT::Type));

      map.insert (std::make_pair (id, info));
    }

    // IDREF
    {
      XSCRT::TypeId const id (typeid (IDREF<C>));
      XSCRT::ExtendedTypeInfo info (id);

      info.add_base (XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (IDREF_Base));

      map.insert (std::make_pair (id, info));
    }
  }
}
