local kmp = require 'kmp.kmp-ffi'
local strgen = require 'lqc.generators.string'

property "kmp finds string matches" {
  generators = {strgen(), strgen(), int()},
  check = function(src, pat, offset)
    local match = kmp(pat)
    offset = math.abs(offset) % #src
    local searchspace = src:sub(1, offset) .. pat .. src:sub(offset+1, -1)
    local results = match(searchspace)
    --print(("searching for %q in %q, should be at %d found at %s"):format(pat, searchspace, offset, results[1]))
    for i, v in ipairs(results) do
      if searchspace:sub(v+1, v+#pat) ~= pat then
        return false
      end
    end
    return true
  end
}