# Panic Engine
WHAT

## What's probably inside
- Engine
  - chat only with engine and not with other APIs
- Core: debug log
- Graphics
- (opt) AI

## Every library has
- **Inc**
  - **<projectname>.h** - anything created specifically in said project
  - **Common.h** - anything common or external
- **Src**
  - **Precompiled.h** - to compile headers and so you don't have to include header files everywhere.  Avoid cyclical dependancy (basically A includes B and vise versa bc they run into infinite loop)
  - **Precompiled.cpp** - _see above_
