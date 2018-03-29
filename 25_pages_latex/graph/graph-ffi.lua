local ffi = require "ffi"

ffi.cdef [[
void *mgi_new(int size);
int mgi_getEdge(void *self, int i, int j);
int mgi_setEdge(void *self, int i, int j, int e);
int mgi_setBiedge(void *self, int i, int j, int e);
void mgi_destroy(void *self);

struct matrixGraphInt {
  void *g;
};
]]

local lib = ffi.load("./libgraph.so")

local mgi = ffi.metatype("struct matrixGraphInt", {
    __new = function(size)
      return ffi.new("struct matrixGraphInt", lib.mgi_new(size))
    end,
    __index = {
      getEdge = function(self, i, j)
        return lib.mgi_getEdge(self.g, i, j)
      end,
      setEdge = function(self, i, j, e)
        lib.mgi_setEdge(self.g, i, j, e)
      end,
      setBiedge = function(self, i, j, e)
        lib.mgi_setBiedge(self, i, j, e)
      end
    },
    __gc = function(self)
      lib.mgi_destroy(self.g)
    end
  }
)

return mgi