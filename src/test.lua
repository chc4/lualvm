local core = require 'lualvm.core'

-- teste do contexto
ctx = core.contextCreate ()
print ("Metatable do Contexto ", getmetatable (ctx))

-- teste do módulo
mod = core.moduleCreate ('oie')
print ("Metatable do Module ", getmetatable (mod), '\n')
print ("global module:dump ()")
mod:dump ()

mod = ctx:moduleCreate ('inCtx')
print ("\nctx print (module)")
print (mod)

print ("\nctx module clone + dump")
mod:clone ():dump ()

