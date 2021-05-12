# Introduction

This document describes the format of the GeoID struct, version 1.

# Description of GeoID

GeoID version 1 consists of three 32-bit words:

0. System Type (upper 16 bits), Region Number (lower 16 bits)
1. Element Number
2. Version Number

# C++ Code for GeoID

```CPP
struct GeoID
{
  enum class SystemType : uint16_t
  {
    kTPC = 1,
    kPDS = 2,
    kDataSelection = 3,
  ...
    kInvalid = 0
  };
  SystemType system_type{ SystemType::kInvalid };
  uint16_t region_number{ std::numeric_limits<uint16_t>::max() };
  uint32_t element_number{ std::numeric_limits<uint32_t>::max() };
  uint32_t version{ 1 };
};
```

# Notes

The version field is last so that brace-initialization works without specifying version.