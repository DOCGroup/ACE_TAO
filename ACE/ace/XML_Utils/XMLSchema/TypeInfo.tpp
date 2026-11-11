// file      : XMLSchema/TypeInfo.tpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>

namespace XMLSchema
{
  template <typename T, typename Base = XSCRT::Type>
  void add_info (XSCRT::ExtendedTypeInfoMap &map)
  {
    XSCRT::TypeId const id (typeid (T));
    XSCRT::ExtendedTypeInfo info (id);

    info.add_base (XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (Base));

    map.try_emplace (id, std::move (info));
  }

  template <typename C>
  inline TypeInfoInitializer<C>::TypeInfoInitializer (XSCRT::ExtendedTypeInfoMap &map)
  {
    // XSCRT::Type
    {
      XSCRT::TypeId const id (typeid (XSCRT::Type));
      map.try_emplace (id, id);
    }

    add_info<byte> (map);
    add_info<unsignedByte> (map);

    add_info<short_> (map);
    add_info<unsignedShort> (map);

    add_info<int_> (map);
    add_info<unsignedInt> (map);

    add_info<long_> (map);
    add_info<unsignedLong> (map);

    add_info<boolean> (map);

    add_info<float_> (map);
    add_info<double_> (map);

    add_info<string<C>> (map);

    add_info<normalizedString<C>, string<C>> (map);
    add_info<token<C>, normalizedString<C>> (map);
    add_info<NMTOKEN<C>, token<C>> (map);
    add_info<Name<C>, token<C>> (map);
    add_info<NCName<C>, Name<C>> (map);
    add_info<QName<C>, Name<C>> (map);

    add_info<ID<C>, NCName<C>> (map);

    // IDREF_Base
    {
      XSCRT::TypeId const id (typeid (IDREF_Base));
      XSCRT::ExtendedTypeInfo info (id);

      info.add_base (XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (XSCRT::Type));

      map.try_emplace (id, std::move (info));
    }

    // IDREF
    {
      XSCRT::TypeId const id (typeid (IDREF<C>));
      XSCRT::ExtendedTypeInfo info (id);

      info.add_base (XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (IDREF_Base));

      map.try_emplace (id, std::move (info));
    }
  }
}
