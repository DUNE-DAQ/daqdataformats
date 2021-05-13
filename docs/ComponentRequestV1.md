# Introduction

This document describes the format of the ComponentRequest struct, version 1.

# Description of ComponentRequest

ComponentRequest version 1 consists of 8 32-bit words:

0. Version Number
1. [GeoID version 1](GeoIDV1.md) Version Number
2. [GeoID version 1](GeoIDV1.md) Component Type (upper 16 bits), Region ID (lower 16 bits)
3. [GeoID version 1](GeoIDV1.md) Element ID
4. Window Begin (upper 32 bits)
5. Window Begin (lower 32 bits)
6. Window End (upper 32 bits)
7. Window End (lower 32 bits)

# C++ Code for ComponentRequest

```CPP
struct ComponentRequest
{
  uint32_t version{ 1 };
  GeoID component;
  timestamp_t window_begin{ TypeDefaults::s_invalid_timestamp };
  timestamp_t window_end{ TypeDefaults::s_invalid_timestamp };

  ComponentRequest();
  ComponentRequest(GeoID comp, timestamp_t wbegin, timestamp_t wend);
};
```

# Notes

ComponentRequest can no longer be brace-initialized, as it provides a user constructor.