# Lua port

This is a port of the [Lua](http://www.lua.org/) programming language for profanOS.

## building

```
python build.py
```

## running

Copy the `lua.bin` file to the fatpath directory of your profanOS installation,
```
cp lua.bin ../profanOS/out/zapps/fatpath/
```

Then, run the following command in the profanOS shell:
```
profanOS [/] > lua
```
