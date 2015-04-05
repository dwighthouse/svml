# SVML
Simple Vector Math Library

SVML (Simple Vector Math Library) is designed to provide powerful vector/point operations for 2, 3, and 4D vector types with an absurdly simple interface and installation.

**This project is abandoned, please do not use! It was moved from Google Code to GitHub for historical reasons and as an example of SFINAE in C++. Since I wrote this, I have become aware of better methods of performing the same actions using techniques found in the Boost libraries. C++11 or later versions also likely have better methods of achieving these tasks. I will not be continuing or updating this project because I have largely moved on to JavaScript for day-to-day and hobby projects.**

The "simple" in SVML comes from two features:
 1. All definitions and functionality exist in a single header file that can be downloaded and included in the project. No multiple files, no dependencies, no registering libraries in your IDE.
 2. Vector swizzling with the standard syntax (including both read and write) is available for all operations, making it very easy to reorder vector's components and change dimensions without the need for manually created temporary objects or tedious reordering. All this without hacks, dependences, or namespace pollution.

Swizzling, for the uninformed, is a syntax that allows the following lines to be legal:
```
my3dVector.zyx = my2dVector.xyy;
my3dVector = my4dVector.xyz * 5;
my3dVector.zyx += my3dVector.xyz;
```

## Documentation
 * [Usage Doc](https://github.com/dwighthouse/svml/tree/master/docs/Usage.md)
 * [Technology Overview Doc](https://github.com/dwighthouse/svml/tree/master/docs/TechnologyOverview.md)

## Status
 * Abandoned
 * Alpha
 * Works on GCC
 * DOES NOT WORK on VC++
 * Need to fix VC++ issues and add final unit tests for existing functionality to move to Beta