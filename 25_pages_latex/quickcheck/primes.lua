local sieve = require "primes.primes-ffi".sieve

local function isprime(n)
  if n == 0 or n == 1 then return false end
  if n > 2 then
    for i = 2, math.sqrt(n)+1 do
      if n % i == 0 then
        return false
      end
    end
  end
  return true
end

local s = sieve(101)

property 'Sieve generates accurate primes' {
  generators = { int(100) },
  check = function(n)
    return isprime(n) == s[n]
  end
}
