local ffi = require "ffi"

ffi.cdef[[
void *malloc(size_t);
void free(void *);

typedef long long int64_t;
typedef unsigned long long uint64_t;
int64_t hungarian_match(int64_t**, uint64_t, uint64_t);
]]

local lib = ffi.load "./libhungarian-interface.so"

local M = {}

local function make_array(type, size, data)
    ptr = ffi.cast(type.."*", ffi.C.malloc(size*ffi.sizeof(type)))
    if data then ffi.fill(ptr, size, data) end
    return ptr
end

M.match = function(data)
    local err = "HungarianMatch requires rectangular cost matrix"
    --Confirm input is a table
    assert(type(data) == "table", err)
    assert(#data > 0, err)

    --Size of dimension 2
    local d2 = -1

    --Check that all of first dimension is tables
    --of the same size
    for i,v in ipairs(data) do
        assert(type(v) == "table", err)
        assert(#v > 0, err)
        if d2 < 0 then d2 = #v else assert(#v == d2, err) end

        --Confirm that second dimension is all numbers
        for j,w in ipairs(v) do assert(type(w) == "number", err) end
    end

    local height = #data
    local width = d2
    assert(height <= width, "HungarianMatch requires at least as many workers as there are jobs")

    local table = make_array("int64_t*", height)
    for i,v in ipairs(data) do
        table[i-1] = make_array("int64_t", width, v)
    end

    local out = lib.hungarian_match(table, height, width)

    for i=1, height do ffi.C.free(table[i-1]) end
    ffi.C.free(table)

    return tonumber(out)
end

return M