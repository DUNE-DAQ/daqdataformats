#!/usr/bin/env python3

import sys
from daqdataformats import SourceID, SourceIDGeoIDMap
from detdataformats import DetID, GeoID


def main() -> int:

    tpc_0 = SourceID(SourceID.Subsystem.kDetectorReadout, 0)
    tpc_1 = SourceID(SourceID.Subsystem.kDetectorReadout, 1)
    geo_a = GeoID(DetID.Subdetector.kHD_TPC, 1, 0, 0)
    geo_b = GeoID(DetID.Subdetector.kHD_TPC, 1, 0, 1)

    the_map = SourceIDGeoIDMap()
    the_map.add(tpc_0, geo_a)
    the_map.add(tpc_1, geo_b)
    if the_map.get_source_id(geo_a) != tpc_0 or the_map.get_source_ids(geo_a) != [tpc_0]:
        print("SourceIDGeoIDMap lookup test failed")
        return 1
    else:
        print("SourceIDGeoIDMap lookup test passed")

    # building a map: a GeoID can't get a second SourceID
    try:
        the_map.add(tpc_1, geo_a)
        print("SourceIDGeoIDMap strict add test failed: a second SourceID was accepted")
        return 1
    except ValueError:
        print("SourceIDGeoIDMap strict add test passed")

    # reading a stored map: a shared GeoID is kept, reported, and has no single SourceID
    encoded = the_map.to_encoded()
    encoded[tpc_1].append(geo_a.encode())
    read_map = SourceIDGeoIDMap.from_encoded(encoded)
    if (read_map.get_shared_geo_ids() != {geo_a} or read_map.get_source_ids(geo_a) != [tpc_0, tpc_1]
            or read_map.get_source_id(geo_a) is not None or read_map.get_source_id(geo_b) != tpc_1):
        print("SourceIDGeoIDMap lenient read test failed")
        return 1
    else:
        print("SourceIDGeoIDMap lenient read test passed")

    print("\nAll SourceIDGeoIDMap Python binding tests passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
