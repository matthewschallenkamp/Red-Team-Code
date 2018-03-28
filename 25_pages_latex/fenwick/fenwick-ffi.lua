local ffi = require "ffi"

ffi.cdef[[
void *malloc(size_t);
void free(void *);
typedef long long ll;
void fenwick_update(ll *a, int n, int i, int delta);
ll fenwick_sum(ll *a,int i);
ll fenwick_getsum(ll *a, int l, int r);
struct fenwick {
  ll *a;
  int len;
};
]]

local lib = ffi.load "./libfenwick.so"

local function make_array(size)
  return ffi.cast("ll *", ffi.C.malloc(size*ffi.sizeof"ll"))
end

local fenwick = ffi.metatype("struct fenwick", {
    __new = function(class, data)
      local cdata, len, f
      if type(data) == "number" then
        cdata = make_array(data)
        ffi.fill(cdata, data*ffi.sizeof("ll"))
        len = data
        f = ffi.new("struct fenwick", cdata, len)
      elseif type(data) == "table" then
        cdata = make_array(#data)
        ffi.fill(cdata, #data*ffi.sizeof("ll"))
        len = #data
        f = ffi.new("struct fenwick", cdata, len)
        for i, v in ipairs(data) do
          f:update(i, v)
        end
      else
        error("Invalid initializer type", 2)
      end
      ffi.gc(cdata, nil)
      return f
    end,
    __index = {
      update = function(self, idx, delta)
        lib.fenwick_update(self.a, self.len, idx, delta)
      end,
      sum = function(self, l, r)
        if r == nil then
          return lib.fenwick_sum(self.a, l)
        else
          return lib.fenwick_getsum(self.a, l, r)
        end
      end,
      at = function(self, idx)
        if idx < 0 or idx > self.len then
          error("index out of bounds", 2)
        end
        return self.a[idx]
      end,
    },
    __len = function(self)
      return self.len
    end,
    __gc = function(self)
      ffi.C.free(self.a)
    end
  }
)

return fenwick