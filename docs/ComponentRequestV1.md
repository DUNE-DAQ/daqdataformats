# Introduction

This document describes the format of the ComponentRequest struct, version 1.

# Description of ComponentRequest

ComponentRequest version 1 consists of 8 32-bit words:

0. [GeoID version 1](GeoIDV1.md) Component Type (upper 16 bits), Region ID (lower 16 bits)
1. [GeoID version 1](GeoIDV1.md) Element ID
2. [GeoID version 1](GeoIDV1.md) Version Number
2. Window Begin (upper 32 bits)
3. Window Begin (lower 32 bits)
4. Window End (upper 32 bits)
5. Window End (lower 32 bits)
6. Version Number

# C++ Code for ComponentRequest

```CPP
struct ComponentRequest
{
  GeoID component;
  timestamp_t window_begin{ TypeDefaults::s_invalid_timestamp };
  timestamp_t window_end{ TypeDefaults::s_invalid_timestamp };
  uint32_t version{ 1 };
};
```

# Notes

The version field is last so that brace-initialization works without specifying version.