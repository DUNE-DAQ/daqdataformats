# Introduction

This document describes the format of the GeoID struct, version 0 (unversioned).

# Description of GeoID

GeoID version 0 (unversioned) consists of four 16-bit words (for a total of 64 bits):

0. Component Type
1. Region Number
2. Element Number (upper 16 bits)
3. Element Number (lower 16 bits)

# C++ Code for GeoID

```CPP
enum class GeoIDComponentType : uint16_t
{
  kTPC = 1,
  kPDS = 2,
  kDataSelection = 3,
...
  kInvalid = 0
};
struct GeoID
{
  GeoIDComponentType component_type{ GeoIDComponentType::kInvalid };
  uint16_t region_number{ std::numeric_limits<uint16_t>::max() };
  uint32_t element_number{ std::numeric_limits<uint32_t>::max() };
};
```

# Notes

GeoID does not currently have a version field. If its format changes, it would have to either be rolled into a version update of the products that use it, or have a version number added.
