In this document, the basic idea and underlying concepts necessary to understand SVML's structure are explained.

## Introduction

SVML uses several obscure features of C++ to achieve swizzling. Bending your brain around these will be much easier with the below basic explanations.

## Unions and Swizzling
In order to achieve the swizzling syntax, several obscure features of C++ were used in tandem.

The first were unions. Unions in C++ act exactly like structs, except that all data members start at the same address in memory. Unions are rarely used, and when they are, it is typically for the purpose of viewing the same data as different data types. In the case of SVML, however, they are used as the overarching storage container for each vector type (VECTOR2, VECTOR3, and VECTOR4). By placing structs inside the union, two things are achieved. First, more than a single variable can be used, and second, a new "space" is created where the vector data can be accessed with a different name. The basic code idea is below:

```
// ...

union VECTOR2
{
    struct
    {
        float x, y;
    } v;
    
    VECTOR2(const float& x, const float& y) { v.x = x; v.y = y; }
    
    struct X
    {
        struct { TYPE x, y; } v;
        
        operator float() const { return v.x; }
    } x;
    
    struct Y
    {
        struct { TYPE x, y; } v;
        
        operator float() const { return v.y; }
    } y;
    
    struct XY
    {
        struct { TYPE x, y; } v;
        
        operator VECTOR2() const { return VECTOR2(v.x, v.y); }
    } xy;
    
    struct YX
    {
        struct { TYPE x, y; } v;
        
        operator VECTOR2() const { return VECTOR2(v.y, v.x); }
    } yx;

} vec2;

// ...

vec2 testVector(1, 2);

// testVector.x is another name for 1
// testVector.xy is another name for the vector (1, 2)
// testVector.yx is another name for the vector (2, 1)
```

Notice how every "swizzle sub-struct" contains a typecast operator overload? This allows them to convert themselves appropriately whenever needed. This accounts for all read swizzling. Write swizzling is little more than adding assignment operators to those swizzle sub-structs that contain no duplicates (.yxz has no duplicates, and would have assignment operators; .yyy has duplicates and would not). Because these child structs are actually variables inside an object that have access to the parent's data (being a union), they have the exact syntax one would expect from a language with swizzling capabilities (such as GLSL).

## Functions
To prevent a massive amount of operator and function duplication (one per swizzle), the non-member functions were used whenever possible. They look like this, in the case of the addition operator:

```
VECTOR2 operator+(const VECTOR2& lhs, const VECTOR2& rhs)
{
    return VECTOR2(lhs.x + rhs.x, lhs.y + rhs.y);
}
```

If these non-member functions were not used, and all calculations were placed in the local space of all swizzles, the library would become ten to twenty thousand lines long, even when greater code compression is used. Assignment operators and type conversions, however, necessarily require local access. Thankfully, they are relatively few when compared to the number of functions the library supports.

If using a swizzle variation in a calculation, the vector's swizzle is implicitly converted down to a regular vector, doing the rearrangement in the typecast operator. This works very well, until templates are used.

## Templates and SFINAE
The single most requested feature of the SVML was that it support templated types. Unfortunately, templated types cannot directly be used when implicit conversions are necessary. To solve this, every swizzle contains a typedef that refers to the parent type. Using this name and the C++ idiom Substitution Failure Is Not An Error (SFINAE), the correct function can be determined and the implicit conversions can be turned into explicit ones. The final look of swizzle sub-structs look like this:

```
struct YZZ
{
    struct { TYPE x, y, z; } v;
    typedef VECTOR3<TYPE> PARENT;
    operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(v.y, v.z, v.z); }
} yzz;
```

Where TYPE is the templated type for the parent union vector type, in this case, VECTOR3. The conversion and assignment operators work nearly identically, but to make the addition function work, three additional variations are required: `<Vector, Swizzle>`, `<Swizzle, Vector>`, and `<Swizzle, Swizzle>`. Each of these simply explicitly converts the swizzle types before calling the <Vector, Vector> variation previously mentioned. In total, they look like this:

```
template <typename SWIZZLE0, typename SWIZZLE1> inline
typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type
operator+(const SWIZZLE0& lhs, const SWIZZLE1& rhs)
{
    return typename SWIZZLE0::PARENT(lhs) + typename SWIZZLE1::PARENT(rhs);
}

template <typename SWIZZLE, typename TYPE> inline
typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type
operator+(const SWIZZLE& lhs, const VECTOR3<TYPE>& rhs)
{
    return typename SWIZZLE::PARENT(lhs) + rhs;
}

template <typename TYPE, typename SWIZZLE> inline
typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type
operator+(const VECTOR3<TYPE>& lhs, const SWIZZLE& rhs)
{
    return lhs + typename SWIZZLE::PARENT(rhs);
}

template <typename TYPE>
VECTOR3<TYPE> operator+(const VECTOR3<TYPE>& lhs, const VECTOR3<TYPE>& rhs)
{
    return VECTOR3<TYPE>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
```

The massive template header at the top of the first three functions use SFINAE functions included in the file to determine if the swizzle is really the 3D type or not. All three of the top functions simply explicitly convert their attributes from swizzles to vectors where appropriate and call the bottom function.

This generally means four duplicate functions per operation per dimension. Not as good as the single function for each operation that implicit conversion allows, but an order of magnitude better than one duplicate per operation per swizzle.

## Other Considerations
The first three functions of each set are inlined since they only do explicit conversions. Other functions may be inlined in the future, but at this stage, it is unknown if it would lead to increased or decreased performance.

The actual library is much more careful with it's access rights. Swizzle variables are declared public, but not the types. The overarching memory struct for the whole union is also private. This allows all access to the data to go through appropriate channels, preventing syntax such as `vector.xyz.v` or `vector.v.x`.

The same qualities that allow swizzles to work allow more general "property" syntax options. This is currently only used for the vectors' magnitudes: Length. One can use `vector.Length` both to retrieve and assign (in a defined manner) a vector's magnitude.

Whenever a scalar value is used, its type is defined as SCALAR_TYPE, which by default is float. Some functions may not work properly if a non-floating-point type is used.