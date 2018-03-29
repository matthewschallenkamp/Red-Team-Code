local ffi = require "ffi"

ffi.cdef [[
void *sieve_gen(int n);
bool sieve_get(void *self, int i);
void sieve_delete(void *self);

struct sieve {
  void * s;
};
]]

local lib = ffi.load "./libprimes.so"

local sieve = ffi.metatype("struct sieve",
  {
    __new = function(self, n)
      return ffi.new("struct sieve", lib.sieve_gen(n))
    end,
    __index = function(self, idx)
      return lib.sieve_get(self.s, idx)
    end,
    __gc = function(self)
      lib.sieve_delete(self.s)
    end
  }
)

return {
  sieve = sieve
}