// The schema for SourceID in JSON
//

local moo = import "moo.jsonnet";

// A schema builder in the given path (namespace)
local ns = "dunedaq.daqdataformats.sourceid";
local s = moo.oschema.schema(ns);

// Object structure used by the test/fake producer module
local sourceid = {
      
    source_id : s.number("id_t", "u4"),
    subsystem : s.string("subsystem_t"),

    sourceid : s.record("SourceID", [s.field("id", self.source_id, doc="" ),
        s.field("subsystem", self.subsystem, doc="" )
    ], doc="SourceID JSONnet Configuration"),

};

moo.oschema.sort_select(sourceid, ns)
