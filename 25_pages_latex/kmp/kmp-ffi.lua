local ffi = require "ffi"

ffi.cdef [[
void * kmp_create(const char* str);
void * kmp_find(void *kmp, const char* str);
int kmp_res_get(void* res, int i);
int kmp_res_len(void* res);
void kmp_res_free(void* res);
void kmp_free(void* kmp);

struct kmp {
  void *k;
};
struct kmp_res {
  void *r;
};
]]

local lib = ffi.load('./libkmp.so')

local kmp = ffi.metatype("struct kmp", {
  __new = function(ct, str)
    return ffi.new("struct kmp", lib.kmp_create(str))
  end,
  __call = function(self, str)
    local res = lib.kmp_find(self.k, str)
    local len = lib.kmp_res_len(res)
    local out = {}
    for i = 0, len - 1 do
      out[i+1] = lib.kmp_res_get(res, i)
    end
    lib.kmp_res_free(res)
    return out
  end,
  __gc = function(self)
    lib.kmp_free(self.k)
  end
})

return kmp