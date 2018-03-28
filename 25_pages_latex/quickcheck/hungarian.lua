--[[local hungarian = require "hungarian.hungarian-ffi"
local inttbl = require "int-table"

property 'fenwick tree sums the numbers within the bounds' {
  generators = { inttbl(100), int(100), int(100) },
  check = function(tbl, l, r)
    --print("{"..table.concat(tbl, ", ").."}", l, r)
    l = l % #tbl
    r = r % #tbl
    l, r = math.max(math.min(l, r), 1), math.max(l, r)
    local sum = 0
    for i = l, r do sum = sum + tbl[i] end
    local fsum = fenwick(tbl):sum(l, r)
    --print(sum, fsum)
    return sum == fsum
  end
}
--]]