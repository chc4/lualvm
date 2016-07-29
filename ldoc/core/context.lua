--- @module lualvm.core.context
-- [LLVM-C Context](http://www.llvm.org/docs/doxygen/html/group__LLVMCCoreContext.html)
-- functions, usually accessed in Lua through the `LLVMContext` methods

--- Create a new LLVMModule associated with Context
--
-- @Context ctx Context
-- @string name Module's name
--
-- @treturn LLVMModule
function core.context.Module (ctx, name) end

--- Get Context string representation, which is it's address (like lua functions)
-- 
-- @Context ctx Context
--
-- @treturn string "LLVMContext: 0xADDRESS"
function core.context.__tostring (ctx) end