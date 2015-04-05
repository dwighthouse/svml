How to use SVML.

## Installation
 1. Download the latest version of svml.h. (Found [http://code.google.com/p/svml/source/browse/trunk/svml.h here])
 2. Save file to your project folder with the rest of your code.
 3. Use `#include "svml.h"` in whatever header files need the functionality.
 4. Use the information in Namespace below to gain access to the functionality.

## Namespace
SVML uses the namespace `SVML`. To obtain the use of each type (including associated functions), simply use `using SVML::vec2;` or whatever primative-based type and dimension you need. If you plan to use your own type for the vector's components, use instead the vector templated type: `using SVML::VECTOR2;` or other dimension. From there, you can define the templated type normally and typedef it to whatever name desired.

Utility functions, such as `DegToRad`(), `RadToDeg`(), and the scalar Lerp() must be used specifically by name. For example: `using SVML::DegToRad;`

## Functionality
For vectors and swizzles (mix and match, "vec" refers to vector or swizzle, so those functions using more than one have the same dimension):
 * `ToString(vec)` - Returns string of vector
 * `<<` - Printing overload for cout

 * `-` - Negation
 * `+` - Addition
 * `-` - Subtraction
 * `*` - Multiplication (component-wise)
 * `/` - Division (component-wise)
 * `*` - Multiplication (scalar) - both directions valid
 * `/` - Division (scalar) - both directions valid

 * `AboutEqual`(vec, vec) - Uses an epsilon to do a component-wise equality check
 * `==` - Equality
 * `!=` - Inequality
 * `<` - Less Than
 * `>` - Greater Than
 * `<=` - Less Than or Equal To
 * `>=` - Greater Than or Equal To

 * `Normalize(vec)` - Returns normalized vector
 * `Dot(vec, vec)` - Returns dot product of two vectors
 * `Perpendicular(vec)` - Returns the perpendicular vector, equivalent to rotating 90 degrees (2D only)
 * `Cross(vec, vec)` - Returns cross product vector of two input vectors (3D only)
 * `Project(vec, vec)` - Projects first vector onto second
 * `Rotate(vec, scalar)` - Returns vector rotated by scalar angle (2D version)
 * `Rotate(vec, vec, scalar)` - Returns first vector rotated about second vector by scalar angle (3D version)
 * `Lerp(vec, vec, scalar)` - Linear interpolation of the vectors' components

 * `Max(vec, vec)` - Returns vector with maximum components two vectors
 * `Min(vec, vec)` - Returns vector with minimum components two vectors
 * `Max(vec, scalar)` - Returns vector with maximum components of vector and scalar value
 * `Min(vec, scalar)` - Returns vector with minimum components of vector and scalar value
 * `Ceil(vec)` - Rounds up to nearest integer for all components
 * `Floor(vec)` - Rounds down to nearest integer for all components

 * `Distance(vec, vec)` - Return distance between two points
 * `DistanceSquared(vec, vec)` - Return squared distance between two points

For swizzles with no duplicates (such as xyz, but not xyy) and for plain vectors:
 * `=` - Assignment
 * `+=` - Assignment by addition
 * `-=` - Assignment by subtraction
 * `*=` - Assignment by multiplication (component-wise)
 * `/=` - Assignment by division (component-wise)
 * `*=` - Assignment by multiplication (scalar)
 * `/=` - Assignment by division (scalar)

For vectors only:
 * `[]` - Array subscripting (`[0]` = .x, `[1]` = .y, `[2]` = .z, `[3]` = .w)
 * `.Normalize()` - Normalizes the object it is a member of, returns nothing

 * `.Length` - Returns or assigns the vector's magnitude
   * Defined assignment forms: `=`, `+=`, `-=`, `*=`, `/=`

## Additional Swizzle Notations
In addition to the "x, y, z, w" geometric swizzles, there is also the "r, g, b, a" swizzles for storing colors and "s, t, p, q" swizzles for texture coordinates.

## 4D Homogeneous Vectors
4D vectors are not true four-dimensional vectors, but rather three-dimensional vectors with a homogeneous w component. Thus, the functions and operations applied to and used with them have different behaviors than the 2D and 3D vectors.
 * The following properties and functions only apply to the first three components, ignoring the w component
   * `-` - Negate
   * `.Length` - Both returning and assigning
   * `.Normalize()`
   * `Normalize()`
   * `Dot()`
   * `Cross()` - Result vector takes the w component of the "a" parameter
   * `Project()` - Result vector takes the w component of the "ontoThis" parameter
   * `Rotate()` - Result vector takes the w component of the "toRotate" parameter
   * `Distance()`
   * `DistanceSquared()`
 * All remaining functions and operators treat 4D vectors just like 3D and 2D vectors, operating on or applying to all components with no special consideration given to the w component

## Properties
All swizzles and Length are C++ properties implemented using unions. Properties are actually functions, objects, or sets of functions that act like a single variable. In Length's case, you can assign a length to it, and the vector will be scaled to reflect that length. Using the length property simply returns the length. Examples:
```
vector2.Length = 1; // The same as vector2.Normalize();
float magnitude = vector2.Length; // magnitude now has the length of vector2
vector3.Length = vector2.Length; // Both vector2 and vector3 now have the same magnitude, even though they
                                 // may not have the same components or even the same dimension
```

## Watch Out
 * Default constructors do not initialize the vector's components
 * Division and division assignment do not check for divide-by-zero
 * Only swizzles with no duplicates have assignment operators (capable of write swizzling)
 * The Dot() function assumes both vectors are already normalized
 * Out-of-bounds errors for the array subscript will result in the program termination along with an error message, it does not throw
 * Vectors are only available in 2, 3, and 4 dimensions. By default, pre-defined vectors with float components are defined as vec2, vec3, and vec4
 * The Rotate functions use radians instead of degrees and the rotation axis is assumed to be already normalized
 * The Rotate, Perpendicular, and Cross functions produce right-handed rotation results.