<p align="center">
  <img src="Github assets/gamemaker_anywhere_logo.png" alt="GameMaker Anywhere logo" width="300" />
</p>

## Info
<p align="left">
A work in progress open source port of gamemaker to several consoles using devkitpro and C++.
https://discord.gg/cc2YA2pChU
</p>

<br>

<p align="center">
  <img src="Github assets/example.gif" alt="The example gif" width="600" />
</p>

## How does this work?
Rather then our old approach (interpreting gml directly) it converts it to C syntax and the C project has the functions and variables to make it run! (A little simplified)

## Currently supported
Check out the [wiki](https://github.com/Ralcactus/GameMaker-Anywhere/wiki/Currently-added-features) to see what you can use!
<br>(also current consoles are wii, gamecube, and 3ds!) <br>

## contributing
Quick tutorial on editing the project: <br>
Open the compiler (GML2C.yyp) in gamemaker, this compiles the sprites, code, object, etc to the C++ project. That C++ project is stored in the datafiles\Runner folder.
For adding new functions, add it to the gm_funcs files in the appropriate type. So a timer function would go in misc, rumble in input, etc
When you actually wanna test your changes, compile the GML2C project and compile it. Do not compile the c++ project from the datafiles directly!
<br><br>
If you wanna add a new export platform: Don't!<br>Later on more platforms will be allowed, but as of currently we don't wanna deal with editing like 10 different platforms at once

## Credits
Meet the team!
- Ralcactus (coder and creator)
- FoxFore (coder)
- Loaf (Logo designer)
- Finlay (coder)
- Samuel Venable (GameMaker extension developer)
- Feel free to contribute! (we really need the help...)
