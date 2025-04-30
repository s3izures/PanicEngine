# Panic Engine
A custom game engine.

## Every library has
- **Inc** - where all headers go
  - **\<projectname\>.h** - anything created specifically in said project
  - **Common.h** - anything common or external
- **Src** - where all cpp files go
  - **Precompiled.h** - to compile headers and so you don't have to include header files everywhere.  Avoid cyclical dependancy (basically A includes B and vise versa bc they run into infinite loop)
  - **Precompiled.cpp** - _see above_
