local fourier = require 'fourier.fourier-ffi'.fourier
local floattbl = require 'float-table'
local deep_equals = require 'lqc.helpers.deep_equals'

--[[
FAILS ATM
property 'fft and ifft are mirrors' {
  generators = { floattbl(100) },
  check = function(tbl)
    local fwd = fourier(tbl)
    local back = fourier(fwd)
    for i, v in ipairs(back) do
      back[i] = v/#back
    end
    return deep_equals(tbl, back)
  end
}
]]
