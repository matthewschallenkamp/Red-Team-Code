local ffi = require "ffi"

ffi.cdef[[
  void four1_interface(double *data, unsigned long nn);
  void fft_interface(complex *data, unsigned long n);
]]

local lib = ffi.load "./libfourier.so"

return {
  fourier = function(data)
    local native = ffi.new("complex[?]", #data, data)
    print "entering native fourier"
    lib.fft_interface(native, #data)
    print "finished native fourier"
    local res = {}
    print "allocated res"
    for i=0, #data-1 do
      res[i+1] = native[i].re
    end
    return res
  end
  --[[function(data)
    local native = ffi.new("double[?]", #data* 2, data)
    print "calling native fourier"
    lib.four1_interface(native, #data)
    print "copying data back"
    local res = {}
    print "allocated res"
    for i=0, #data-1 do
      res[i+1] = native[i]
    end
    print("exiting fourier interface")
    return res
  end]]
}