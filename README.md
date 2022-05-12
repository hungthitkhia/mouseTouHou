# mouseTouhou
A program to make Touhou available for mouse controlling.

This program is bascially for Touhou 18 and above (probably), for older Touhou danmaku games, use [ThMouse](https://github.com/hwei/ThMouse).

# A more optimized version
This version is a faster, and less laggier version than the original version of the program.

By adding a bool value to indicate whether the mouse speed have been changed, this remove dramatically the constant mouse speed writing which causes the intensive lag.

# Purpose
Since Touhou 18, [ThMouse by HWei](https://github.com/hwei/ThMouse) can't be used because DirectInput is removed. ThMouse uses function hooking (redirect calling the actual function to our own custom function) of DirectInput to enable mouse control, while mouseTouhou directly modifies the memory of the game itself, which applies to any game you want (as long as you know where the position address is).

# Compilation
1. Install Codeblocks, and any compiler (GCC, MSVC,...).
2. Build it.
3. Enjoy. Yup, that's actually it.

# Other games (advanced)
Just go into ```gm.cpp``` and add the required data in. Only do it when you know what you're doing.

There's also that small define macro USE_NWM (using NtWriteProcessMemory instead of WriteProcessMemory from kernel32) in pch.h that you can turn it on or off. I don't know if that affects performance or not, I just put it in for fun. :)

# Performance
Surprisingly, using this with any game before Touhou 18 will receive a notable FPS decrease, while 18 won't.
