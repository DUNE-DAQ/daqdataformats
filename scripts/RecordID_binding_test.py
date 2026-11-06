#!/usr/bin/env python3

import sys
from daqdataformats import RecordID


def main() -> int:

    rid = RecordID(5, 1)
    if (rid.number, rid.sequence) != (5, 1) or RecordID(5).sequence != 0:
        print("RecordID constructor/field test failed")
        return 1
    else:
        print("RecordID constructor/field test passed")

    # equal RecordIDs hash equally, so they work as dict keys and set members
    same = RecordID(5, 1)
    if hash(same) != hash(rid) or len({rid, same}) != 1 or {rid: "x"}.get(same) != "x":
        print("RecordID hash/equality test failed")
        return 1
    else:
        print("RecordID hash/equality test passed")

    if not (RecordID(5, 0) < RecordID(5, 1) < RecordID(6, 0)):
        print("RecordID ordering test failed")
        return 1
    else:
        print("RecordID ordering test passed")

    # the fields are read-only, so a stored key can't change its hash
    for field in ("number", "sequence"):
        try:
            setattr(rid, field, 9)
        except AttributeError:
            continue
        print(f"RecordID read-only field test failed: {field} could be assigned")
        return 1
    print("RecordID read-only field test passed")

    print("\nAll RecordID Python binding tests passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
