# Introduction

This document describes the format of the GeoID struct, version 0 (unversioned).

# Description of GeoID

GeoID version 0 (unversioned) consists of two 32-bit words:

0. APA Number
1. Link Number

# C++ Code for GeoID

```CPP
struct GeoID
{
  uint32_t apa_number{ std::numeric_limits<uint32_t>::max() };
  uint32_t link_number{ std::numeric_limits<uint32_t>::max() };
};
```

# Notes

GeoID does not currently have a version field. If its format changes, it would have to either be rolled into a version update of the products that use it, or have a version number added.
