# Lua port

This is a port of the [Lua](http://www.lua.org/) programming language for profanOS.

## building

```
python build.py
```

## running

Copy the `lua.bin` file to the commands directory of your profanOS installation,
```
cp lua.bin ../profanOS/out/zapps/commands/
```

and the core program to the tools directory.
```
cp lua-core.bin ../profanOS/out/zapps/tools/
```

Then, run the following command in the profanOS shell:
```
profanOS [/] -> lua
```
