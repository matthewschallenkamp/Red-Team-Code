--[[describe("graph", function()
    describe("core", function()
        it("should initialize safely", function()
            local graph = require "graph.graph-ffi"
            local g = graph(20)
            assert.is_truthy(g)
          end
        )
        it("should store edge weights", function()
            local graph = require "graph.graph-ffi"
            local g = graph(3)
            g:setBiedge(1, 2, 1)
            assert.is.equal(g:getEdge(2, 1), 1)
          end
        )
        it("should not affect other edge weights", function()
            local graph = require "graph.graph-ffi"
            local g = graph(3)
            g:setBiedge(1, 2, 1)
            assert.isnt.equal(g:getEdge(2, 3), 1)
          end
        )
      end
    )
  end
)
]]