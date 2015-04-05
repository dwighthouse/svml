#ifndef SVML_H
#define SVML_H

/*

SVML - Simple Vector Math Library
Author: Dwight House ( http://dwighthouse.com )
Assistance by: Dr. Dmitri Volper, Mike Fried, Jeff Hubbard, Chris Peters

Instructions and Information: https://github.com/dwighthouse/svml

Colossians 3:23-24

Copyright (c) 2010-2015 Dwight House

*/

#include <iostream> // cout, endl
#include <sstream> // ostream, ostringstream, string
#include <math.h> // sqrt, fabs, min, max, ceil, floor, sin, cos

namespace SVML
{

//----------------------------------------------------------------------
// 
// Sec. 01 - Startup, globals, predefinitions, types
// 
//----------------------------------------------------------------------

using std::cout;
using std::endl;
using std::string;
using std::ostream;
using std::ostringstream;
using std::max;
using std::min;

// Typedefs
typedef float SCALAR_TYPE;

// Global constants
const SCALAR_TYPE COMPARISON_EPSILON = (SCALAR_TYPE)0.000001; // Used by AlmostEqual()

// Predefines
template <typename TYPE> union VECTOR2;
template <typename TYPE> union VECTOR3;
template <typename TYPE> union VECTOR4;

// Default types (BUILT-IN TYPE CUSTOMIZATION HERE!)
typedef VECTOR2<float> vec2;
typedef VECTOR3<float> vec3;
typedef VECTOR4<float> vec4;
// etc.



//----------------------------------------------------------------------
// 
// Sec. 02 - Math utilities not specific to vectors
// 
//----------------------------------------------------------------------

inline SCALAR_TYPE DegToRad(const SCALAR_TYPE& degrees)
{
	return degrees * (SCALAR_TYPE)0.017453292519943; // (pi / 180)
}

inline SCALAR_TYPE RadToDeg(const SCALAR_TYPE& radians)
{
	return radians * (SCALAR_TYPE)57.295779513082325; // (180 / pi)
}

inline SCALAR_TYPE Lerp(const SCALAR_TYPE& start, const SCALAR_TYPE& end, const SCALAR_TYPE& delta)
{
	return start + delta * (end - start);
}



//----------------------------------------------------------------------
// 
// Sec. 03 - SFINAE utilities
// 
//----------------------------------------------------------------------

// EnableIf
template <typename TYPE, typename R, bool b = TYPE::value> struct EnableIf {};
template <typename TYPE, typename R> struct EnableIf<TYPE, R, true> { typedef R type; };

// Generic types
template <typename TYPE> struct Is2D { enum { value = false }; };
template <typename TYPE> struct Is3D { enum { value = false }; };
template <typename TYPE> struct Is4D { enum { value = false }; };

// Specific type
template <typename TYPE> struct Is2D< VECTOR2<TYPE> > { enum { value = true }; };
template <typename TYPE> struct Is3D< VECTOR3<TYPE> > { enum { value = true }; };
template <typename TYPE> struct Is4D< VECTOR4<TYPE> > { enum { value = true }; };



//----------------------------------------------------------------------
// 
// Sec. 04 - 2D vector type and associated functions
// 
//----------------------------------------------------------------------

template <typename TYPE>
union VECTOR2
{
private:
	struct { TYPE x, y; } v;

	struct s1 { protected: TYPE x, y; };
	struct s2 { protected: TYPE x, y; public: typedef VECTOR2<TYPE> PARENT; };
	struct s3 { protected: TYPE x, y; public: typedef VECTOR3<TYPE> PARENT; };
	struct s4 { protected: TYPE x, y; public: typedef VECTOR4<TYPE> PARENT; };

	struct X : s1
	{
		operator TYPE() const { return s1::x; }
		const X& operator=(const TYPE& rhs) { s1::x = rhs; return *this; }
		const X& operator+=(const TYPE& rhs) { s1::x += rhs; return *this; }
		const X& operator-=(const TYPE& rhs) { s1::x -= rhs; return *this; }
		const X& operator*=(const TYPE& rhs) { s1::x *= rhs; return *this; }
		const X& operator/=(const TYPE& rhs) { s1::x /= rhs; return *this; }
	};
	struct Y : s1
	{
		operator TYPE() const { return s1::y; }
		const Y& operator=(const TYPE& rhs) { s1::y = rhs; return *this; }
		const Y& operator+=(const TYPE& rhs) { s1::y += rhs; return *this; }
		const Y& operator-=(const TYPE& rhs) { s1::y -= rhs; return *this; }
		const Y& operator*=(const TYPE& rhs) { s1::y *= rhs; return *this; }
		const Y& operator/=(const TYPE& rhs) { s1::y /= rhs; return *this; }
	};
	struct XX : s2 { operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::x, s2::x); } };
	struct XY : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::x, s2::y); }
		const XY& operator=(const VECTOR2<TYPE>& rhs) { s2::x = rhs.v.x; s2::y = rhs.v.y; return *this; }
		const XY& operator+=(const VECTOR2<TYPE>& rhs) { s2::x += rhs.v.x; s2::y += rhs.v.y; return *this; }
		const XY& operator-=(const VECTOR2<TYPE>& rhs) { s2::x -= rhs.v.x; s2::y -= rhs.v.y; return *this; }
		const XY& operator*=(const VECTOR2<TYPE>& rhs) { s2::x *= rhs.v.x; s2::y *= rhs.v.y; return *this; }
		const XY& operator/=(const VECTOR2<TYPE>& rhs) { s2::x /= rhs.v.x; s2::y /= rhs.v.y; return *this; }
		const XY& operator*=(const TYPE& rhs) { s2::x *= rhs; s2::y *= rhs; return *this; }
		const XY& operator/=(const TYPE& rhs) { s2::x /= rhs; s2::y /= rhs; return *this; }
	};
	struct YX : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::y, s2::x); }
		const YX& operator=(const VECTOR2<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y = rhs.v.x; s2::x = t0; return *this; }
		const YX& operator+=(const VECTOR2<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y += rhs.v.x; s2::x += t0; return *this; }
		const YX& operator-=(const VECTOR2<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y -= rhs.v.x; s2::x -= t0; return *this; }
		const YX& operator*=(const VECTOR2<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y *= rhs.v.x; s2::x *= t0; return *this; }
		const YX& operator/=(const VECTOR2<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y /= rhs.v.x; s2::x /= t0; return *this; }
		const YX& operator*=(const TYPE& rhs) { s2::y *= rhs; s2::x *= rhs; return *this; }
		const YX& operator/=(const TYPE& rhs) { s2::y /= rhs; s2::x /= rhs; return *this; }
	};
	struct YY : s2 { operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::y, s2::y); } };
	struct XXX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::x, s3::x); } };
	struct XXY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::x, s3::y); } };
	struct XYX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::y, s3::x); } };
	struct XYY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::y, s3::y); } };
	struct YXX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::x, s3::x); } };
	struct YXY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::x, s3::y); } };
	struct YYX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::y, s3::x); } };
	struct YYY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::y, s3::y); } };
	struct XXXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::x, s4::x); } };
	struct XXXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::x, s4::y); } };
	struct XXYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::y, s4::x); } };
	struct XXYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::y, s4::y); } };
	struct XYXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::x, s4::x); } };
	struct XYXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::x, s4::y); } };
	struct XYYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::y, s4::x); } };
	struct XYYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::y, s4::y); } };
	struct YXXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::x, s4::x); } };
	struct YXXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::x, s4::y); } };
	struct YXYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::y, s4::x); } };
	struct YXYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::y, s4::y); } };
	struct YYXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::x, s4::x); } };
	struct YYXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::x, s4::y); } };
	struct YYYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::y, s4::x); } };
	struct YYYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::y, s4::y); } };

	// Length property
	struct LENGTH
	{
	private:
		TYPE x, y;
		TYPE ApplyLength(const TYPE& curMag, const TYPE& newMag)
		{
			TYPE inverseScaledMagnitude = newMag / curMag;
			x *= inverseScaledMagnitude;
			y *= inverseScaledMagnitude;
			return newMag;
		}
	public:
		operator TYPE() const { return sqrt(x * x + y * y); }
		TYPE operator=(const TYPE& rhs) { return ApplyLength((TYPE)(*this), rhs); }
		TYPE operator+=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag + rhs); }
		TYPE operator-=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag - rhs); }
		TYPE operator*=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag * rhs); }
		TYPE operator/=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag / rhs); }
	};

public:
	VECTOR2() {}
	VECTOR2(const TYPE& x, const TYPE& y) { v.x = x; v.y = y; }

	// Swizzle variables
	X x, r, s;
	Y y, g, t;
	XX xx, rr, ss;
	XY xy, rg, st;
	YX yx, gr, ts;
	YY yy, gg, tt;
	XXX xxx, rrr, sss;
	XXY xxy, rrg, sst;
	XYX xyx, rgr, sts;
	XYY xyy, rgg, stt;
	YXX yxx, grr, tss;
	YXY yxy, grg, tst;
	YYX yyx, ggr, tts;
	YYY yyy, ggg, ttt;
	XXXX xxxx, rrrr, ssss;
	XXXY xxxy, rrrg, ssst;
	XXYX xxyx, rrgr, ssts;
	XXYY xxyy, rrgg, sstt;
	XYXX xyxx, rgrr, stss;
	XYXY xyxy, rgrg, stst;
	XYYX xyyx, rggr, stts;
	XYYY xyyy, rggg, sttt;
	YXXX yxxx, grrr, tsss;
	YXXY yxxy, grrg, tsst;
	YXYX yxyx, grgr, tsts;
	YXYY yxyy, grgg, tstt;
	YYXX yyxx, ggrr, ttss;
	YYXY yyxy, ggrg, ttst;
	YYYX yyyx, gggr, ttts;
	YYYY yyyy, gggg, tttt;

	// Length property
	LENGTH Length;

	// Assignment operators (default for = is fine)
	const VECTOR2& operator+=(const VECTOR2& rhs) { v.x += rhs.v.x; v.y += rhs.v.y; return *this; }
	const VECTOR2& operator-=(const VECTOR2& rhs) { v.x -= rhs.v.x; v.y -= rhs.v.y; return *this; }
	const VECTOR2& operator*=(const VECTOR2& rhs) { v.x *= rhs.v.x; v.y *= rhs.v.y; return *this; }
	const VECTOR2& operator/=(const VECTOR2& rhs) { v.x /= rhs.v.x; v.y /= rhs.v.y; return *this; }
	const VECTOR2& operator*=(const TYPE& rhs) { v.x *= rhs; v.y *= rhs; return *this; }
	const VECTOR2& operator/=(const TYPE& rhs) { v.x /= rhs; v.y /= rhs; return *this; }

	// Array notation access
	const TYPE& operator[](const unsigned& index)
	{
		if (index == 0) { return v.x; }
		else if (index == 1) { return v.y; }
		else { cout << "Fatal Error: Attempted out of bounds bracket access of 2D vector." << endl << " - Vector: " << *this << endl << " - Index:  " << index << endl; exit(-1); }
	}

	// Overload for cout
	friend ostream& operator<<(ostream& os, const VECTOR2<TYPE>& printVector)
	{
		os << "(" << printVector.v.x << ", " << printVector.v.y << ")";
		return os;
	}

	// Negation
	VECTOR2 operator-() const { return VECTOR2(-v.x, -v.y); }

	void Normalize() { *this /= this->Length; }
};

// 2D ToString()
template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, string >::type ToString(const SWIZZLE& printSwizzle) { return ToString(typename SWIZZLE::PARENT(printSwizzle)); }
template <typename TYPE> string ToString(const VECTOR2<TYPE>& printVector)
{
	std::ostringstream buffer;
	buffer << "(" << printVector.x << ", " << printVector.y << ")";
	return buffer.str();
}

// 2D Negate [-]
template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator-(const SWIZZLE& toNegate)
{
	return -(typename SWIZZLE::PARENT(toNegate));
}

// 2D Addition [+]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator+(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) + typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator+(const SWIZZLE& lhs, const VECTOR2<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) + rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator+(const VECTOR2<TYPE>& lhs, const SWIZZLE& rhs) { return lhs + typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR2<TYPE> operator+(const VECTOR2<TYPE>& lhs, const VECTOR2<TYPE>& rhs)
{
	return VECTOR2<TYPE>(lhs.x + rhs.x, lhs.y + rhs.y);
}

// 2D Subtraction [-]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator-(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) - typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator-(const SWIZZLE& lhs, const VECTOR2<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) - rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator-(const VECTOR2<TYPE>& lhs, const SWIZZLE& rhs) { return lhs - typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR2<TYPE> operator-(const VECTOR2<TYPE>& lhs, const VECTOR2<TYPE>& rhs)
{
	return VECTOR2<TYPE>(lhs.x - rhs.x, lhs.y - rhs.y);
}

// 2D Multiplication [*]: Component-wise
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator*(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) * typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const SWIZZLE& lhs, const VECTOR2<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) * rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const VECTOR2<TYPE>& lhs, const SWIZZLE& rhs) { return lhs * typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR2<TYPE> operator*(const VECTOR2<TYPE>& lhs, const VECTOR2<TYPE>& rhs)
{
	return VECTOR2<TYPE>(lhs.x * rhs.x, lhs.y * rhs.y);
}

// 2D Division [/]: Component-wise
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator/(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) / typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const SWIZZLE& lhs, const VECTOR2<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) / rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const VECTOR2<TYPE>& lhs, const SWIZZLE& rhs) { return lhs / typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR2<TYPE> operator/(const VECTOR2<TYPE>& lhs, const VECTOR2<TYPE>& rhs)
{
	return VECTOR2<TYPE>(lhs.x / rhs.x, lhs.y / rhs.y);
}

// 2D Multiplication [*]: Scalar
template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const SWIZZLE& lhs, const SCALAR_TYPE& rhs) { return typename SWIZZLE::PARENT(lhs) * rhs; }
template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const SCALAR_TYPE& lhs, const SWIZZLE& rhs) { return lhs * typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR2<TYPE> operator*(const VECTOR2<TYPE>& lhs, const SCALAR_TYPE& rhs)
{
	return VECTOR2<TYPE>(lhs.x * rhs, lhs.y * rhs);
}
template <typename TYPE> VECTOR2<TYPE> operator*(const SCALAR_TYPE& lhs, const VECTOR2<TYPE>& rhs)
{
	return VECTOR2<TYPE>(lhs * rhs.x, lhs * rhs.y);
}

// 2D Division [/]: Scalar
template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const SWIZZLE& lhs, const SCALAR_TYPE& rhs) { return typename SWIZZLE::PARENT(lhs) / rhs; }
template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const SCALAR_TYPE& lhs, const SWIZZLE& rhs) { return lhs / typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR2<TYPE> operator/(const VECTOR2<TYPE>& lhs, const SCALAR_TYPE& rhs)
{
	return VECTOR2<TYPE>(lhs.x / rhs, lhs.y / rhs);
}
template <typename TYPE> VECTOR2<TYPE> operator/(const SCALAR_TYPE& lhs, const VECTOR2<TYPE>& rhs)
{
	return VECTOR2<TYPE>(lhs / rhs.x, lhs / rhs.y);
}

// 2D Equal To [==]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, bool >::type >::type operator==(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) == typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, bool >::type operator==(const SWIZZLE& lhs, const VECTOR2<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) == rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, bool >::type operator==(const VECTOR2<TYPE>& lhs, const SWIZZLE& rhs) { return lhs == typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator==(const VECTOR2<TYPE>& lhs, const VECTOR2<TYPE>& rhs)
{
	return !(lhs.x != rhs.x || lhs.y != rhs.y);
}

// 2D Not Equal To [!=]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, bool >::type >::type operator!=(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) != typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, bool >::type operator!=(const SWIZZLE& lhs, const VECTOR2<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) != rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, bool >::type operator!=(const VECTOR2<TYPE>& lhs, const SWIZZLE& rhs) { return lhs != typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator!=(const VECTOR2<TYPE>& lhs, const VECTOR2<TYPE>& rhs)
{
	return lhs.x != rhs.x || lhs.y != rhs.y;
}

// 2D AlmostEqual()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, bool >::type >::type AlmostEqual(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return AlmostEqual((typename SWIZZLE0::PARENT(lhs)), (typename SWIZZLE1::PARENT(rhs))); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, bool >::type AlmostEqual(const SWIZZLE& lhs, const VECTOR2<TYPE>& rhs) { return AlmostEqual((typename SWIZZLE::PARENT(lhs)), rhs); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, bool >::type AlmostEqual(const VECTOR2<TYPE>& lhs, const SWIZZLE& rhs) { return AlmostEqual(lhs, (typename SWIZZLE::PARENT(rhs))); }
template <typename TYPE> bool AlmostEqual(const VECTOR2<TYPE>& lhs, const VECTOR2<TYPE>& rhs)
{
	return fabs(lhs.x - rhs.x) < COMPARISON_EPSILON && fabs(lhs.y - rhs.y) < COMPARISON_EPSILON;
}

// 2D Less Than [<]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, bool >::type >::type operator<(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) < typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, bool >::type operator<(const SWIZZLE& lhs, const VECTOR2<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) < rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, bool >::type operator<(const VECTOR2<TYPE>& lhs, const SWIZZLE& rhs) { return lhs < typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator<(const VECTOR2<TYPE>& lhs, const VECTOR2<TYPE>& rhs)
{
	return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}

// 2D Greater Than [>]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, bool >::type >::type operator>(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) > typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, bool >::type operator>(const SWIZZLE& lhs, const VECTOR2<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) > rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, bool >::type operator>(const VECTOR2<TYPE>& lhs, const SWIZZLE& rhs) { return lhs > typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator>(const VECTOR2<TYPE>& lhs, const VECTOR2<TYPE>& rhs)
{
	return lhs.x > rhs.x || (lhs.x == rhs.x && lhs.y > rhs.y);
}

// 2D Less Than Or Equal To [<=]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, bool >::type >::type operator<=(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) <= typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, bool >::type operator<=(const SWIZZLE& lhs, const VECTOR2<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) <= rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, bool >::type operator<=(const VECTOR2<TYPE>& lhs, const SWIZZLE& rhs) { return lhs <= typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator<=(const VECTOR2<TYPE>& lhs, const VECTOR2<TYPE>& rhs)
{
	return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y <= rhs.y);
}

// 2D Greater Than Or Equal To [<=]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, bool >::type >::type operator>=(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) >= typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, bool >::type operator>=(const SWIZZLE& lhs, const VECTOR2<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) >= rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, bool >::type operator>=(const VECTOR2<TYPE>& lhs, const SWIZZLE& rhs) { return lhs >= typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator>=(const VECTOR2<TYPE>& lhs, const VECTOR2<TYPE>& rhs)
{
	return lhs.x > rhs.x || (lhs.x == rhs.x && lhs.y >= rhs.y);
}

// 2D Normalize()
template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Normalize(const SWIZZLE& toNormalize) { return Normalize(typename SWIZZLE::PARENT(toNormalize)); }
template <typename TYPE> VECTOR2<TYPE> Normalize(const VECTOR2<TYPE>& toNormalize)
{
	return toNormalize / toNormalize.Length;
}

// 2D Dot()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, SCALAR_TYPE >::type >::type Dot(const SWIZZLE0& a, const SWIZZLE1& b) { return Dot(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Dot(const SWIZZLE& a, const VECTOR2<TYPE>& b) { return Dot(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Dot(const VECTOR2<TYPE>& a, const SWIZZLE& b) { return Dot(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> SCALAR_TYPE Dot(const VECTOR2<TYPE>& a, const VECTOR2<TYPE>& b)
{
	return a.x * b.x + a.y * b.y;
}

// 2D Perpendicular()
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Perpendicular(const SWIZZLE& toPerpendicular) { return Perpendicular(typename SWIZZLE::PARENT(toPerpendicular)); }
template <typename TYPE> VECTOR2<TYPE> Perpendicular(const VECTOR2<TYPE>& toPerpendicular)
{
	return vec2(-toPerpendicular.y, toPerpendicular.x);
}

// 2D Project()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Project(const SWIZZLE0& projectThis, const SWIZZLE1& ontoThis) { return Project(typename SWIZZLE0::PARENT(projectThis), typename SWIZZLE1::PARENT(ontoThis)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Project(const SWIZZLE& projectThis, const VECTOR2<TYPE>& ontoThis) { return Project(typename SWIZZLE::PARENT(projectThis), ontoThis); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Project(const VECTOR2<TYPE>& projectThis, const SWIZZLE& ontoThis) { return Project(projectThis, typename SWIZZLE::PARENT(ontoThis)); }
template <typename TYPE> VECTOR2<TYPE> Project(const VECTOR2<TYPE>& projectThis, const VECTOR2<TYPE>& ontoThis)
{
	return (Dot(projectThis, ontoThis) / Dot(ontoThis, ontoThis)) * ontoThis;
}

// 2D Rotate()
template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Rotate(const SWIZZLE& vector, const SCALAR_TYPE& angle) { return Rotate(typename SWIZZLE::PARENT(vector), angle); }
template <typename TYPE> VECTOR2<TYPE> Rotate(const VECTOR2<TYPE>& vector, const SCALAR_TYPE& angle)
{
	SCALAR_TYPE sa = sin(angle);
	SCALAR_TYPE ca = cos(angle);
	return VECTOR2<TYPE>(vector.x * ca - vector.y * sa, vector.x * sa + vector.y * ca);
}

// 2D Lerp()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Lerp(const SWIZZLE0& start, const SWIZZLE1& end, const SCALAR_TYPE& delta) { return Lerp(typename SWIZZLE0::PARENT(start), typename SWIZZLE1::PARENT(end), delta); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Lerp(const SWIZZLE& start, const VECTOR2<TYPE>& end, const SCALAR_TYPE& delta) { return Lerp(typename SWIZZLE::PARENT(start), end, delta); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Lerp(const VECTOR2<TYPE>& start, const SWIZZLE& end, const SCALAR_TYPE& delta) { return Lerp(start, typename SWIZZLE::PARENT(end), delta); }
template <typename TYPE> VECTOR2<TYPE> Lerp(const VECTOR2<TYPE>& start, const VECTOR2<TYPE>& end, const SCALAR_TYPE& delta)
{
	return VECTOR2<TYPE>(start.x + delta * (end.x - start.x),
	                     start.y + delta * (end.y - start.y));
}

// 2D Max(): Component-wise
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Max(const SWIZZLE0& a, const SWIZZLE1& b) { return Max(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const SWIZZLE& a, const VECTOR2<TYPE>& b) { return Max(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const VECTOR2<TYPE>& a, const SWIZZLE& b) { return Max(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> VECTOR2<TYPE> Max(const VECTOR2<TYPE>& a, const VECTOR2<TYPE>& b)
{
	return VECTOR2<TYPE>(max(a.x, b.x), max(a.y, b.y));
}

// 2D Min(): Component-wise
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Min(const SWIZZLE0& a, const SWIZZLE1& b) { return Min(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const SWIZZLE& a, const VECTOR2<TYPE>& b) { return Min(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const VECTOR2<TYPE>& a, const SWIZZLE& b) { return Min(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> VECTOR2<TYPE> Min(const VECTOR2<TYPE>& a, const VECTOR2<TYPE>& b)
{
	return VECTOR2<TYPE>(min(a.x, b.x), min(a.y, b.y));
}

// 2D Max(): Scalar
template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const SWIZZLE& a, const SCALAR_TYPE& maximum) { return Max(typename SWIZZLE::PARENT(a), maximum); }
template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const SCALAR_TYPE& maximum, const SWIZZLE& b) { return Max(maximum, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> VECTOR2<TYPE> Max(const VECTOR2<TYPE>& a, const SCALAR_TYPE& maximum)
{
	return VECTOR2<TYPE>(max((SCALAR_TYPE)a.x, maximum), max((SCALAR_TYPE)a.y, maximum));
}
template <typename TYPE> VECTOR2<TYPE> Max(const SCALAR_TYPE& maximum, const VECTOR2<TYPE>& b)
{
	return VECTOR2<TYPE>(max(maximum, (SCALAR_TYPE)b.x), max(maximum, (SCALAR_TYPE)b.y));
}

// 2D Min(): Scalar
template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const SWIZZLE& a, const SCALAR_TYPE& minimum) { return Min(typename SWIZZLE::PARENT(a), minimum); }
template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const SCALAR_TYPE& minimum, const SWIZZLE& b) { return Min(minimum, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> VECTOR2<TYPE> Min(const VECTOR2<TYPE>& a, const SCALAR_TYPE& minimum)
{
	return VECTOR2<TYPE>(min((SCALAR_TYPE)a.x, minimum), min((SCALAR_TYPE)a.y, minimum));
}
template <typename TYPE> VECTOR2<TYPE> Min(const SCALAR_TYPE& minimum, const VECTOR2<TYPE>& b)
{
	return VECTOR2<TYPE>(min(minimum, (SCALAR_TYPE)b.x), min(minimum, (SCALAR_TYPE)b.y));
}

// 2D Ceil()
template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Ceil(const SWIZZLE& toCeil) { return Ceil(typename SWIZZLE::PARENT(toCeil)); }
template <typename TYPE> VECTOR2<TYPE> Ceil(const VECTOR2<TYPE>& toCeil)
{
	return VECTOR2<TYPE>(ceil(toCeil.x), ceil(toCeil.y));
}

// 2D Floor()
template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Floor(const SWIZZLE& toFloor) { return Floor(typename SWIZZLE::PARENT(toFloor)); }
template <typename TYPE> VECTOR2<TYPE> Floor(const VECTOR2<TYPE>& toFloor)
{
	return VECTOR2<TYPE>(floor(toFloor.x), floor(toFloor.y));
}

// 2D Distance()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, SCALAR_TYPE >::type >::type Distance(const SWIZZLE0& a, const SWIZZLE1& b) { return Distance(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Distance(const SWIZZLE& a, const VECTOR2<TYPE>& b) { return Distance(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Distance(const VECTOR2<TYPE>& a, const SWIZZLE& b) { return Distance(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> SCALAR_TYPE Distance(const VECTOR2<TYPE>& a, const VECTOR2<TYPE>& b)
{
	return sqrt(DistanceSquared(a, b));
}

// 2D DistanceSquared()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is2D< typename SWIZZLE0::PARENT >, typename EnableIf< Is2D< typename SWIZZLE1::PARENT >, SCALAR_TYPE >::type >::type DistanceSquared(const SWIZZLE0& a, const SWIZZLE1& b) { return DistanceSquared(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type DistanceSquared(const SWIZZLE& a, const VECTOR2<TYPE>& b) { return DistanceSquared(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type DistanceSquared(const VECTOR2<TYPE>& a, const SWIZZLE& b) { return DistanceSquared(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> SCALAR_TYPE DistanceSquared(const VECTOR2<TYPE>& a, const VECTOR2<TYPE>& b)
{
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}



//----------------------------------------------------------------------
// 
// Sec. 05 - 3D vector type and associated functions
// 
//----------------------------------------------------------------------

template <typename TYPE>
union VECTOR3
{
private:
	struct { TYPE x, y, z; } v;

	struct s1 { protected: TYPE x, y, z; };
	struct s2 { protected: TYPE x, y, z; public: typedef VECTOR2<TYPE> PARENT; };
	struct s3 { protected: TYPE x, y, z; public: typedef VECTOR3<TYPE> PARENT; };
	struct s4 { protected: TYPE x, y, z; public: typedef VECTOR4<TYPE> PARENT; };

	struct X : s1
	{
		operator TYPE() const { return s1::x; }
		const X& operator=(const TYPE& rhs) { s1::x = rhs; return *this; }
		const X& operator+=(const TYPE& rhs) { s1::x += rhs; return *this; }
		const X& operator-=(const TYPE& rhs) { s1::x -= rhs; return *this; }
		const X& operator*=(const TYPE& rhs) { s1::x *= rhs; return *this; }
		const X& operator/=(const TYPE& rhs) { s1::x /= rhs; return *this; }
	};
	struct Y : s1
	{
		operator TYPE() const { return s1::y; }
		const Y& operator=(const TYPE& rhs) { s1::y = rhs; return *this; }
		const Y& operator+=(const TYPE& rhs) { s1::y += rhs; return *this; }
		const Y& operator-=(const TYPE& rhs) { s1::y -= rhs; return *this; }
		const Y& operator*=(const TYPE& rhs) { s1::y *= rhs; return *this; }
		const Y& operator/=(const TYPE& rhs) { s1::y /= rhs; return *this; }
	};
	struct Z : s1
	{
		operator TYPE() const { return s1::z; }
		const Z& operator=(const TYPE& rhs) { s1::z = rhs; return *this; }
		const Z& operator+=(const TYPE& rhs) { s1::z += rhs; return *this; }
		const Z& operator-=(const TYPE& rhs) { s1::z -= rhs; return *this; }
		const Z& operator*=(const TYPE& rhs) { s1::z *= rhs; return *this; }
		const Z& operator/=(const TYPE& rhs) { s1::z /= rhs; return *this; }
	};
	struct XX : s2 { operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::x, s2::x); } };
	struct XY : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::x, s2::y); }
		const XY& operator=(const VECTOR3<TYPE>& rhs) { s2::x = rhs.v.x; s2::y = rhs.v.y; return *this; }
		const XY& operator+=(const VECTOR3<TYPE>& rhs) { s2::x += rhs.v.x; s2::y += rhs.v.y; return *this; }
		const XY& operator-=(const VECTOR3<TYPE>& rhs) { s2::x -= rhs.v.x; s2::y -= rhs.v.y; return *this; }
		const XY& operator*=(const VECTOR3<TYPE>& rhs) { s2::x *= rhs.v.x; s2::y *= rhs.v.y; return *this; }
		const XY& operator/=(const VECTOR3<TYPE>& rhs) { s2::x /= rhs.v.x; s2::y /= rhs.v.y; return *this; }
		const XY& operator*=(const TYPE& rhs) { s2::x *= rhs; s2::y *= rhs; return *this; }
		const XY& operator/=(const TYPE& rhs) { s2::x /= rhs; s2::y /= rhs; return *this; }
	};
	struct XZ : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::x, s2::z); }
		const XZ& operator=(const VECTOR3<TYPE>& rhs) { s2::x = rhs.v.x; s2::z = rhs.v.y; return *this; }
		const XZ& operator+=(const VECTOR3<TYPE>& rhs) { s2::x += rhs.v.x; s2::z += rhs.v.y; return *this; }
		const XZ& operator-=(const VECTOR3<TYPE>& rhs) { s2::x -= rhs.v.x; s2::z -= rhs.v.y; return *this; }
		const XZ& operator*=(const VECTOR3<TYPE>& rhs) { s2::x *= rhs.v.x; s2::z *= rhs.v.y; return *this; }
		const XZ& operator/=(const VECTOR3<TYPE>& rhs) { s2::x /= rhs.v.x; s2::z /= rhs.v.y; return *this; }
		const XZ& operator*=(const TYPE& rhs) { s2::x *= rhs; s2::z *= rhs; return *this; }
		const XZ& operator/=(const TYPE& rhs) { s2::x /= rhs; s2::z /= rhs; return *this; }
	};
	struct YX : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::y, s2::x); }
		const YX& operator=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y = rhs.v.x; s2::x = t0; return *this; }
		const YX& operator+=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y += rhs.v.x; s2::x += t0; return *this; }
		const YX& operator-=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y -= rhs.v.x; s2::x -= t0; return *this; }
		const YX& operator*=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y *= rhs.v.x; s2::x *= t0; return *this; }
		const YX& operator/=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y /= rhs.v.x; s2::x /= t0; return *this; }
		const YX& operator*=(const TYPE& rhs) { s2::y *= rhs; s2::x *= rhs; return *this; }
		const YX& operator/=(const TYPE& rhs) { s2::y /= rhs; s2::x /= rhs; return *this; }
	};
	struct YY : s2 { operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::y, s2::y); } };
	struct YZ : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::y, s2::z); }
		const YZ& operator=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y = rhs.v.x; s2::z = t0; return *this; }
		const YZ& operator+=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y += rhs.v.x; s2::z += t0; return *this; }
		const YZ& operator-=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y -= rhs.v.x; s2::z -= t0; return *this; }
		const YZ& operator*=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y *= rhs.v.x; s2::z *= t0; return *this; }
		const YZ& operator/=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y /= rhs.v.x; s2::z /= t0; return *this; }
		const YZ& operator*=(const TYPE& rhs) { s2::y *= rhs; s2::z *= rhs; return *this; }
		const YZ& operator/=(const TYPE& rhs) { s2::y /= rhs; s2::z /= rhs; return *this; }
	};
	struct ZX : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::z, s2::x); }
		const ZX& operator=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z = rhs.v.x; s2::x = t0; return *this; }
		const ZX& operator+=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z += rhs.v.x; s2::x += t0; return *this; }
		const ZX& operator-=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z -= rhs.v.x; s2::x -= t0; return *this; }
		const ZX& operator*=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z *= rhs.v.x; s2::x *= t0; return *this; }
		const ZX& operator/=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z /= rhs.v.x; s2::x /= t0; return *this; }
		const ZX& operator*=(const TYPE& rhs) { s2::z *= rhs; s2::x *= rhs; return *this; }
		const ZX& operator/=(const TYPE& rhs) { s2::z /= rhs; s2::x /= rhs; return *this; }
	};
	struct ZY : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::z, s2::y); }
		const ZY& operator=(const VECTOR3<TYPE>& rhs) { s2::z = rhs.v.x; s2::y = rhs.v.y; return *this; }
		const ZY& operator+=(const VECTOR3<TYPE>& rhs) { s2::z += rhs.v.x; s2::y += rhs.v.y; return *this; }
		const ZY& operator-=(const VECTOR3<TYPE>& rhs) { s2::z -= rhs.v.x; s2::y -= rhs.v.y; return *this; }
		const ZY& operator*=(const VECTOR3<TYPE>& rhs) { s2::z *= rhs.v.x; s2::y *= rhs.v.y; return *this; }
		const ZY& operator/=(const VECTOR3<TYPE>& rhs) { s2::z /= rhs.v.x; s2::y /= rhs.v.y; return *this; }
		const ZY& operator*=(const TYPE& rhs) { s2::z *= rhs; s2::y *= rhs; return *this; }
		const ZY& operator/=(const TYPE& rhs) { s2::z /= rhs; s2::y /= rhs; return *this; }
	};
	struct ZZ : s2 { operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::z, s2::z); } };
	struct XXX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::x, s3::x); } };
	struct XXY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::x, s3::y); } };
	struct XXZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::x, s3::z); } };
	struct XYX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::y, s3::x); } };
	struct XYY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::y, s3::y); } };
	struct XYZ : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::y, s3::z); }
		const XYZ& operator=(const VECTOR3<TYPE>& rhs) { s3::x = rhs.v.x; s3::y = rhs.v.y; s3::z = rhs.v.z; return *this; }
		const XYZ& operator+=(const VECTOR3<TYPE>& rhs) { s3::x += rhs.v.x; s3::y += rhs.v.y; s3::z += rhs.v.z; return *this; }
		const XYZ& operator-=(const VECTOR3<TYPE>& rhs) { s3::x -= rhs.v.x; s3::y -= rhs.v.y; s3::z -= rhs.v.z; return *this; }
		const XYZ& operator*=(const VECTOR3<TYPE>& rhs) { s3::x *= rhs.v.x; s3::y *= rhs.v.y; s3::z *= rhs.v.z; return *this; }
		const XYZ& operator/=(const VECTOR3<TYPE>& rhs) { s3::x /= rhs.v.x; s3::y /= rhs.v.y; s3::z /= rhs.v.z; return *this; }
		const XYZ& operator*=(const TYPE& rhs) { s3::x *= rhs; s3::y *= rhs; s3::z *= rhs; return *this; }
		const XYZ& operator/=(const TYPE& rhs) { s3::x /= rhs; s3::y /= rhs; s3::z /= rhs; return *this; }
	};
	struct XZX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::z, s3::x); } };
	struct XZY : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::z, s3::y); }
		const XZY& operator=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x = rhs.v.x; s3::z = rhs.v.y; s3::y = t0; return *this; }
		const XZY& operator+=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x += rhs.v.x; s3::z += rhs.v.y; s3::y += t0; return *this; }
		const XZY& operator-=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x -= rhs.v.x; s3::z -= rhs.v.y; s3::y -= t0; return *this; }
		const XZY& operator*=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x *= rhs.v.x; s3::z *= rhs.v.y; s3::y *= t0; return *this; }
		const XZY& operator/=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x /= rhs.v.x; s3::z /= rhs.v.y; s3::y /= t0; return *this; }
		const XZY& operator*=(const TYPE& rhs) { s3::x *= rhs; s3::z *= rhs; s3::y *= rhs; return *this; }
		const XZY& operator/=(const TYPE& rhs) { s3::x /= rhs; s3::z /= rhs; s3::y /= rhs; return *this; }
	};
	struct XZZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::z, s3::z); } };
	struct YXX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::x, s3::x); } };
	struct YXY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::x, s3::y); } };
	struct YXZ : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::x, s3::z); }
		const YXZ& operator=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y = rhs.v.x; s3::x = t0; s3::z = rhs.v.z; return *this; }
		const YXZ& operator+=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y += rhs.v.x; s3::x += t0; s3::z += rhs.v.z; return *this; }
		const YXZ& operator-=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y -= rhs.v.x; s3::x -= t0; s3::z -= rhs.v.z; return *this; }
		const YXZ& operator*=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y *= rhs.v.x; s3::x *= t0; s3::z *= rhs.v.z; return *this; }
		const YXZ& operator/=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y /= rhs.v.x; s3::x /= t0; s3::z /= rhs.v.z; return *this; }
		const YXZ& operator*=(const TYPE& rhs) { s3::y *= rhs; s3::x *= rhs; s3::z *= rhs; return *this; }
		const YXZ& operator/=(const TYPE& rhs) { s3::y /= rhs; s3::x /= rhs; s3::z /= rhs; return *this; }
	};
	struct YYX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::y, s3::x); } };
	struct YYY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::y, s3::y); } };
	struct YYZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::y, s3::z); } };
	struct YZX : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::z, s3::x); }
		const YZX& operator=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y = rhs.v.x; s3::z = t0; s3::x = t1; return *this; }
		const YZX& operator+=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y += rhs.v.x; s3::z += t0; s3::x += t1; return *this; }
		const YZX& operator-=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y -= rhs.v.x; s3::z -= t0; s3::x -= t1; return *this; }
		const YZX& operator*=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y *= rhs.v.x; s3::z *= t0; s3::x *= t1; return *this; }
		const YZX& operator/=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y /= rhs.v.x; s3::z /= t0; s3::x /= t1; return *this; }
		const YZX& operator*=(const TYPE& rhs) { s3::y *= rhs; s3::z *= rhs; s3::x *= rhs; return *this; }
		const YZX& operator/=(const TYPE& rhs) { s3::y /= rhs; s3::z /= rhs; s3::x /= rhs; return *this; }
	};
	struct YZY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::z, s3::y); } };
	struct YZZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::z, s3::z); } };
	struct ZXX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::x, s3::x); } };
	struct ZXY : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::x, s3::y); }
		const ZXY& operator=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z = rhs.v.x; s3::x = t0; s3::y = t1; return *this; }
		const ZXY& operator+=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z += rhs.v.x; s3::x += t0; s3::y += t1; return *this; }
		const ZXY& operator-=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z -= rhs.v.x; s3::x -= t0; s3::y -= t1; return *this; }
		const ZXY& operator*=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z *= rhs.v.x; s3::x *= t0; s3::y *= t1; return *this; }
		const ZXY& operator/=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z /= rhs.v.x; s3::x /= t0; s3::y /= t1; return *this; }
		const ZXY& operator*=(const TYPE& rhs) { s3::z *= rhs; s3::x *= rhs; s3::y *= rhs; return *this; }
		const ZXY& operator/=(const TYPE& rhs) { s3::z /= rhs; s3::x /= rhs; s3::y /= rhs; return *this; }
	};
	struct ZXZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::x, s3::z); } };
	struct ZYX : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::y, s3::x); }
		const ZYX& operator=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z = rhs.v.x; s3::y = rhs.v.y; s3::x = t0; return *this; }
		const ZYX& operator+=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z += rhs.v.x; s3::y += rhs.v.y; s3::x += t0; return *this; }
		const ZYX& operator-=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z -= rhs.v.x; s3::y -= rhs.v.y; s3::x -= t0; return *this; }
		const ZYX& operator*=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z *= rhs.v.x; s3::y *= rhs.v.y; s3::x *= t0; return *this; }
		const ZYX& operator/=(const VECTOR3<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z /= rhs.v.x; s3::y /= rhs.v.y; s3::x /= t0; return *this; }
		const ZYX& operator*=(const TYPE& rhs) { s3::z *= rhs; s3::y *= rhs; s3::x *= rhs; return *this; }
		const ZYX& operator/=(const TYPE& rhs) { s3::z /= rhs; s3::y /= rhs; s3::x /= rhs; return *this; }
	};
	struct ZYY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::y, s3::y); } };
	struct ZYZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::y, s3::z); } };
	struct ZZX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::z, s3::x); } };
	struct ZZY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::z, s3::y); } };
	struct ZZZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::z, s3::z); } };
	struct XXXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::x, s4::x); } };
	struct XXXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::x, s4::y); } };
	struct XXXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::x, s4::z); } };
	struct XXYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::y, s4::x); } };
	struct XXYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::y, s4::y); } };
	struct XXYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::y, s4::z); } };
	struct XXZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::z, s4::x); } };
	struct XXZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::z, s4::y); } };
	struct XXZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::z, s4::z); } };
	struct XYXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::x, s4::x); } };
	struct XYXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::x, s4::y); } };
	struct XYXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::x, s4::z); } };
	struct XYYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::y, s4::x); } };
	struct XYYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::y, s4::y); } };
	struct XYYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::y, s4::z); } };
	struct XYZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::z, s4::x); } };
	struct XYZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::z, s4::y); } };
	struct XYZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::z, s4::z); } };
	struct XZXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::x, s4::x); } };
	struct XZXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::x, s4::y); } };
	struct XZXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::x, s4::z); } };
	struct XZYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::y, s4::x); } };
	struct XZYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::y, s4::y); } };
	struct XZYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::y, s4::z); } };
	struct XZZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::z, s4::x); } };
	struct XZZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::z, s4::y); } };
	struct XZZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::z, s4::z); } };
	struct YXXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::x, s4::x); } };
	struct YXXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::x, s4::y); } };
	struct YXXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::x, s4::z); } };
	struct YXYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::y, s4::x); } };
	struct YXYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::y, s4::y); } };
	struct YXYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::y, s4::z); } };
	struct YXZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::z, s4::x); } };
	struct YXZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::z, s4::y); } };
	struct YXZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::z, s4::z); } };
	struct YYXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::x, s4::x); } };
	struct YYXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::x, s4::y); } };
	struct YYXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::x, s4::z); } };
	struct YYYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::y, s4::x); } };
	struct YYYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::y, s4::y); } };
	struct YYYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::y, s4::z); } };
	struct YYZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::z, s4::x); } };
	struct YYZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::z, s4::y); } };
	struct YYZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::z, s4::z); } };
	struct YZXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::x, s4::x); } };
	struct YZXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::x, s4::y); } };
	struct YZXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::x, s4::z); } };
	struct YZYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::y, s4::x); } };
	struct YZYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::y, s4::y); } };
	struct YZYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::y, s4::z); } };
	struct YZZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::z, s4::x); } };
	struct YZZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::z, s4::y); } };
	struct YZZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::z, s4::z); } };
	struct ZXXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::x, s4::x); } };
	struct ZXXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::x, s4::y); } };
	struct ZXXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::x, s4::z); } };
	struct ZXYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::y, s4::x); } };
	struct ZXYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::y, s4::y); } };
	struct ZXYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::y, s4::z); } };
	struct ZXZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::z, s4::x); } };
	struct ZXZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::z, s4::y); } };
	struct ZXZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::z, s4::z); } };
	struct ZYXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::x, s4::x); } };
	struct ZYXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::x, s4::y); } };
	struct ZYXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::x, s4::z); } };
	struct ZYYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::y, s4::x); } };
	struct ZYYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::y, s4::y); } };
	struct ZYYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::y, s4::z); } };
	struct ZYZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::z, s4::x); } };
	struct ZYZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::z, s4::y); } };
	struct ZYZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::z, s4::z); } };
	struct ZZXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::x, s4::x); } };
	struct ZZXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::x, s4::y); } };
	struct ZZXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::x, s4::z); } };
	struct ZZYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::y, s4::x); } };
	struct ZZYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::y, s4::y); } };
	struct ZZYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::y, s4::z); } };
	struct ZZZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::z, s4::x); } };
	struct ZZZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::z, s4::y); } };
	struct ZZZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::z, s4::z); } };

	// Length property
	struct LENGTH
	{
	private:
		TYPE x, y, z;
		TYPE ApplyLength(const TYPE& curMag, const TYPE& newMag)
		{
			TYPE inverseScaledMagnitude = newMag / curMag;
			x *= inverseScaledMagnitude;
			y *= inverseScaledMagnitude;
			z *= inverseScaledMagnitude;
			return newMag;
		}
	public:
		operator TYPE() const { return sqrt(x * x + y * y + z * z); }
		TYPE operator=(const TYPE& rhs) { return ApplyLength((TYPE)(*this), rhs); }
		TYPE operator+=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag + rhs); }
		TYPE operator-=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag - rhs); }
		TYPE operator*=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag * rhs); }
		TYPE operator/=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag / rhs); }
	};

public:
	VECTOR3() {}
	VECTOR3(const TYPE& x, const TYPE& y, const TYPE& z) { v.x = x; v.y = y; v.z = z; }
	VECTOR3(const VECTOR2<TYPE>& xy, const TYPE& z) { v.x = xy.x; v.y = xy.y; v.z = z; }
	VECTOR3(const TYPE& x, const VECTOR2<TYPE>& yz) { v.x = x; v.y = yz.x; v.z = yz.y; }

	// Swizzle variables
	X x, r, s;
	Y y, g, t;
	Z z, b, p;
	XX xx, rr, ss;
	XY xy, rg, st;
	XZ xz, rb, sp;
	YX yx, gr, ts;
	YY yy, gg, tt;
	YZ yz, gb, tp;
	ZX zx, br, ps;
	ZY zy, bg, pt;
	ZZ zz, bb, pp;
	XXX xxx, rrr, sss;
	XXY xxy, rrg, sst;
	XXZ xxz, rrb, ssp;
	XYX xyx, rgr, sts;
	XYY xyy, rgg, stt;
	XYZ xyz, rgb, stp;
	XZX xzx, rbr, sps;
	XZY xzy, rbg, spt;
	XZZ xzz, rbb, spp;
	YXX yxx, grr, tss;
	YXY yxy, grg, tst;
	YXZ yxz, grb, tsp;
	YYX yyx, ggr, tts;
	YYY yyy, ggg, ttt;
	YYZ yyz, ggb, ttp;
	YZX yzx, gbr, tps;
	YZY yzy, gbg, tpt;
	YZZ yzz, gbb, tpp;
	ZXX zxx, brr, pss;
	ZXY zxy, brg, pst;
	ZXZ zxz, brb, psp;
	ZYX zyx, bgr, pts;
	ZYY zyy, bgg, ptt;
	ZYZ zyz, bgb, ptp;
	ZZX zzx, bbr, pps;
	ZZY zzy, bbg, ppt;
	ZZZ zzz, bbb, ppp;
	XXXX xxxx, rrrr, ssss;
	XXXY xxxy, rrrg, ssst;
	XXXZ xxxz, rrrb, sssp;
	XXYX xxyx, rrgr, ssts;
	XXYY xxyy, rrgg, sstt;
	XXYZ xxyz, rrgb, sstp;
	XXZX xxzx, rrbr, ssps;
	XXZY xxzy, rrbg, sspt;
	XXZZ xxzz, rrbb, sspp;
	XYXX xyxx, rgrr, stss;
	XYXY xyxy, rgrg, stst;
	XYXZ xyxz, rgrb, stsp;
	XYYX xyyx, rggr, stts;
	XYYY xyyy, rggg, sttt;
	XYYZ xyyz, rggb, sttp;
	XYZX xyzx, rgbr, stps;
	XYZY xyzy, rgbg, stpt;
	XYZZ xyzz, rgbb, stpp;
	XZXX xzxx, rbrr, spss;
	XZXY xzxy, rbrg, spst;
	XZXZ xzxz, rbrb, spsp;
	XZYX xzyx, rbgr, spts;
	XZYY xzyy, rbgg, sptt;
	XZYZ xzyz, rbgb, sptp;
	XZZX xzzx, rbbr, spps;
	XZZY xzzy, rbbg, sppt;
	XZZZ xzzz, rbbb, sppp;
	YXXX yxxx, grrr, tsss;
	YXXY yxxy, grrg, tsst;
	YXXZ yxxz, grrb, tssp;
	YXYX yxyx, grgr, tsts;
	YXYY yxyy, grgg, tstt;
	YXYZ yxyz, grgb, tstp;
	YXZX yxzx, grbr, tsps;
	YXZY yxzy, grbg, tspt;
	YXZZ yxzz, grbb, tspp;
	YYXX yyxx, ggrr, ttss;
	YYXY yyxy, ggrg, ttst;
	YYXZ yyxz, ggrb, ttsp;
	YYYX yyyx, gggr, ttts;
	YYYY yyyy, gggg, tttt;
	YYYZ yyyz, gggb, tttp;
	YYZX yyzx, ggbr, ttps;
	YYZY yyzy, ggbg, ttpt;
	YYZZ yyzz, ggbb, ttpp;
	YZXX yzxx, gbrr, tpss;
	YZXY yzxy, gbrg, tpst;
	YZXZ yzxz, gbrb, tpsp;
	YZYX yzyx, gbgr, tpts;
	YZYY yzyy, gbgg, tptt;
	YZYZ yzyz, gbgb, tptp;
	YZZX yzzx, gbbr, tpps;
	YZZY yzzy, gbbg, tppt;
	YZZZ yzzz, gbbb, tppp;
	ZXXX zxxx, brrr, psss;
	ZXXY zxxy, brrg, psst;
	ZXXZ zxxz, brrb, pssp;
	ZXYX zxyx, brgr, psts;
	ZXYY zxyy, brgg, pstt;
	ZXYZ zxyz, brgb, pstp;
	ZXZX zxzx, brbr, psps;
	ZXZY zxzy, brbg, pspt;
	ZXZZ zxzz, brbb, pspp;
	ZYXX zyxx, bgrr, ptss;
	ZYXY zyxy, bgrg, ptst;
	ZYXZ zyxz, bgrb, ptsp;
	ZYYX zyyx, bggr, ptts;
	ZYYY zyyy, bggg, pttt;
	ZYYZ zyyz, bggb, pttp;
	ZYZX zyzx, bgbr, ptps;
	ZYZY zyzy, bgbg, ptpt;
	ZYZZ zyzz, bgbb, ptpp;
	ZZXX zzxx, bbrr, ppss;
	ZZXY zzxy, bbrg, ppst;
	ZZXZ zzxz, bbrb, ppsp;
	ZZYX zzyx, bbgr, ppts;
	ZZYY zzyy, bbgg, pptt;
	ZZYZ zzyz, bbgb, pptp;
	ZZZX zzzx, bbbr, ppps;
	ZZZY zzzy, bbbg, pppt;
	ZZZZ zzzz, bbbb, pppp;

	// Length property
	LENGTH Length;

	// Assignment operators (default for = is fine)
	const VECTOR3& operator+=(const VECTOR3& rhs) { v.x += rhs.v.x; v.y += rhs.v.y; v.z += rhs.v.z; return *this; }
	const VECTOR3& operator-=(const VECTOR3& rhs) { v.x -= rhs.v.x; v.y -= rhs.v.y; v.z -= rhs.v.z; return *this; }
	const VECTOR3& operator*=(const VECTOR3& rhs) { v.x *= rhs.v.x; v.y *= rhs.v.y; v.z *= rhs.v.z; return *this; }
	const VECTOR3& operator/=(const VECTOR3& rhs) { v.x /= rhs.v.x; v.y /= rhs.v.y; v.z /= rhs.v.z; return *this; }
	const VECTOR3& operator*=(const TYPE& rhs) { v.x *= rhs; v.y *= rhs; v.z *= rhs; return *this; }
	const VECTOR3& operator/=(const TYPE& rhs) { v.x /= rhs; v.y /= rhs; v.z /= rhs; return *this; }

	// Array notation access
	const TYPE& operator[](const unsigned& index)
	{
		if (index == 0) { return v.x; }
		else if (index == 1) { return v.y; }
		else if (index == 2) { return v.z; }
		else { cout << "Fatal Error: Attempted out of bounds bracket access of 3D vector." << endl << " - Vector: " << *this << endl << " - Index:  " << index << endl; exit(-1); }
	}

	// Overload for cout
	friend ostream& operator<<(ostream& os, const VECTOR3<TYPE>& printVector)
	{
		os << "(" << printVector.v.x << ", " << printVector.v.y << ", " << printVector.v.z << ")";
		return os;
	}

	// Negation
	VECTOR3 operator-() const { return VECTOR3(-v.x, -v.y, -v.z); }

	void Normalize() { *this /= this->Length; }
};

// 3D ToString()
template <typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, string >::type ToString(const SWIZZLE& printSwizzle) { return ToString(typename SWIZZLE::PARENT(printSwizzle)); }
template <typename TYPE> string ToString(const VECTOR3<TYPE>& printVector)
{
	std::ostringstream buffer;
	buffer << "(" << printVector.x << ", " << printVector.y << ", " << printVector.z << ")";
	return buffer.str();
}

// 3D Negate [-]
template <typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator-(const SWIZZLE& toNegate)
{
	return -(typename SWIZZLE::PARENT(toNegate));
}

// 3D Addition [+]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator+(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) + typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator+(const SWIZZLE& lhs, const VECTOR3<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) + rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator+(const VECTOR3<TYPE>& lhs, const SWIZZLE& rhs) { return lhs + typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR3<TYPE> operator+(const VECTOR3<TYPE>& lhs, const VECTOR3<TYPE>& rhs)
{
	return VECTOR3<TYPE>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

// 3D Subtraction [-]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator-(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) - typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator-(const SWIZZLE& lhs, const VECTOR3<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) - rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator-(const VECTOR3<TYPE>& lhs, const SWIZZLE& rhs) { return lhs - typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR3<TYPE> operator-(const VECTOR3<TYPE>& lhs, const VECTOR3<TYPE>& rhs)
{
	return VECTOR3<TYPE>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

// 3D Multiplication [*]: Component-wise
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator*(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) * typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const SWIZZLE& lhs, const VECTOR3<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) * rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const VECTOR3<TYPE>& lhs, const SWIZZLE& rhs) { return lhs * typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR3<TYPE> operator*(const VECTOR3<TYPE>& lhs, const VECTOR3<TYPE>& rhs)
{
	return VECTOR3<TYPE>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

// 3D Division [/]: Component-wise
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator/(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) / typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const SWIZZLE& lhs, const VECTOR3<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) / rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const VECTOR3<TYPE>& lhs, const SWIZZLE& rhs) { return lhs / typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR3<TYPE> operator/(const VECTOR3<TYPE>& lhs, const VECTOR3<TYPE>& rhs)
{
	return VECTOR3<TYPE>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}

// 3D Multiplication [*]: Scalar
template <typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const SWIZZLE& lhs, const SCALAR_TYPE& rhs) { return typename SWIZZLE::PARENT(lhs) * rhs; }
template <typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const SCALAR_TYPE& lhs, const SWIZZLE& rhs) { return lhs * typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR3<TYPE> operator*(const VECTOR3<TYPE>& lhs, const SCALAR_TYPE& rhs)
{
	return VECTOR3<TYPE>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}
template <typename TYPE> VECTOR3<TYPE> operator*(const SCALAR_TYPE& lhs, const VECTOR3<TYPE>& rhs)
{
	return VECTOR3<TYPE>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

// 3D Division [/]: Scalar
template <typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const SWIZZLE& lhs, const SCALAR_TYPE& rhs) { return typename SWIZZLE::PARENT(lhs) / rhs; }
template <typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const SCALAR_TYPE& lhs, const SWIZZLE& rhs) { return lhs / typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR3<TYPE> operator/(const VECTOR3<TYPE>& lhs, const SCALAR_TYPE& rhs)
{
	return VECTOR3<TYPE>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}
template <typename TYPE> VECTOR3<TYPE> operator/(const SCALAR_TYPE& lhs, const VECTOR3<TYPE>& rhs)
{
	return VECTOR3<TYPE>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
}

// 3D Equal To [==]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, bool >::type >::type operator==(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) == typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, bool >::type operator==(const SWIZZLE& lhs, const VECTOR3<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) == rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, bool >::type operator==(const VECTOR3<TYPE>& lhs, const SWIZZLE& rhs) { return lhs == typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator==(const VECTOR3<TYPE>& lhs, const VECTOR3<TYPE>& rhs)
{
	return !(lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z);
}

// 3D Not Equal To [!=]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, bool >::type >::type operator!=(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) != typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, bool >::type operator!=(const SWIZZLE& lhs, const VECTOR3<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) != rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, bool >::type operator!=(const VECTOR3<TYPE>& lhs, const SWIZZLE& rhs) { return lhs != typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator!=(const VECTOR3<TYPE>& lhs, const VECTOR3<TYPE>& rhs)
{
	return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}

// 3D AlmostEqual()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, bool >::type >::type AlmostEqual(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return AlmostEqual((typename SWIZZLE0::PARENT(lhs)), (typename SWIZZLE1::PARENT(rhs))); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, bool >::type AlmostEqual(const SWIZZLE& lhs, const VECTOR3<TYPE>& rhs) { return AlmostEqual((typename SWIZZLE::PARENT(lhs)), rhs); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, bool >::type AlmostEqual(const VECTOR3<TYPE>& lhs, const SWIZZLE& rhs) { return AlmostEqual(lhs, (typename SWIZZLE::PARENT(rhs))); }
template <typename TYPE> bool AlmostEqual(const VECTOR3<TYPE>& lhs, const VECTOR3<TYPE>& rhs)
{
	return fabs(lhs.x - rhs.x) < COMPARISON_EPSILON && fabs(lhs.y - rhs.y) < COMPARISON_EPSILON && fabs(lhs.z - rhs.z) < COMPARISON_EPSILON;
}

// 3D Less Than [<]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, bool >::type >::type operator<(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) < typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, bool >::type operator<(const SWIZZLE& lhs, const VECTOR3<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) < rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, bool >::type operator<(const VECTOR3<TYPE>& lhs, const SWIZZLE& rhs) { return lhs < typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator<(const VECTOR3<TYPE>& lhs, const VECTOR3<TYPE>& rhs)
{
	return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.yz < rhs.yz);
}

// 3D Greater Than [>]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, bool >::type >::type operator>(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) > typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, bool >::type operator>(const SWIZZLE& lhs, const VECTOR3<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) > rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, bool >::type operator>(const VECTOR3<TYPE>& lhs, const SWIZZLE& rhs) { return lhs > typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator>(const VECTOR3<TYPE>& lhs, const VECTOR3<TYPE>& rhs)
{
	return lhs.x > rhs.x || (lhs.x == rhs.x && lhs.yz > rhs.yz);
}

// 3D Less Than Or Equal To [<=]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, bool >::type >::type operator<=(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) <= typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, bool >::type operator<=(const SWIZZLE& lhs, const VECTOR3<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) <= rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, bool >::type operator<=(const VECTOR3<TYPE>& lhs, const SWIZZLE& rhs) { return lhs <= typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator<=(const VECTOR3<TYPE>& lhs, const VECTOR3<TYPE>& rhs)
{
	return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.yz <= rhs.yz);
}

// 3D Greater Than Or Equal To [<=]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, bool >::type >::type operator>=(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) >= typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, bool >::type operator>=(const SWIZZLE& lhs, const VECTOR3<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) >= rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, bool >::type operator>=(const VECTOR3<TYPE>& lhs, const SWIZZLE& rhs) { return lhs >= typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator>=(const VECTOR3<TYPE>& lhs, const VECTOR3<TYPE>& rhs)
{
	return lhs.x > rhs.x || (lhs.x == rhs.x && lhs.yz >= rhs.yz);
}

// 3D Normalize()
template <typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Normalize(const SWIZZLE& toNormalize) { return Normalize(typename SWIZZLE::PARENT(toNormalize)); }
template <typename TYPE> VECTOR3<TYPE> Normalize(const VECTOR3<TYPE>& toNormalize)
{
	return toNormalize / toNormalize.Length;
}

// 3D Dot()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, SCALAR_TYPE >::type >::type Dot(const SWIZZLE0& a, const SWIZZLE1& b) { return Dot(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Dot(const SWIZZLE& a, const VECTOR3<TYPE>& b) { return Dot(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Dot(const VECTOR3<TYPE>& a, const SWIZZLE& b) { return Dot(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> SCALAR_TYPE Dot(const VECTOR3<TYPE>& a, const VECTOR3<TYPE>& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

// 3D Cross()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Cross(const SWIZZLE0& a, const SWIZZLE1& b) { return Cross(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Cross(const SWIZZLE& a, const VECTOR3<TYPE>& b) { return Cross(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Cross(const VECTOR3<TYPE>& a, const SWIZZLE& b) { return Cross(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> VECTOR3<TYPE> Cross(const VECTOR3<TYPE>& a, const VECTOR3<TYPE>& b)
{
	return VECTOR3<TYPE>(a.y * b.z - a.z * b.y,
	                     a.z * b.x - a.x * b.z,
	                     a.x * b.y - a.y * b.x);
}

// 3D Project()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Project(const SWIZZLE0& projectThis, const SWIZZLE1& ontoThis) { return Project(typename SWIZZLE0::PARENT(projectThis), typename SWIZZLE1::PARENT(ontoThis)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Project(const SWIZZLE& projectThis, const VECTOR3<TYPE>& ontoThis) { return Project(typename SWIZZLE::PARENT(projectThis), ontoThis); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Project(const VECTOR3<TYPE>& projectThis, const SWIZZLE& ontoThis) { return Project(projectThis, typename SWIZZLE::PARENT(ontoThis)); }
template <typename TYPE> VECTOR3<TYPE> Project(const VECTOR3<TYPE>& projectThis, const VECTOR3<TYPE>& ontoThis)
{
	return (Dot(projectThis, ontoThis) / Dot(ontoThis, ontoThis)) * ontoThis;
}

// 3D Rotate()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Rotate(const SWIZZLE0& toRotate, const SWIZZLE1& rotationAxis, const SCALAR_TYPE& radians) { return Rotate(typename SWIZZLE0::PARENT(toRotate), typename SWIZZLE1::PARENT(rotationAxis), radians); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Rotate(const SWIZZLE& toRotate, const VECTOR3<TYPE>& rotationAxis, const SCALAR_TYPE& radians) { return Rotate(typename SWIZZLE::PARENT(toRotate), rotationAxis, radians); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Rotate(const VECTOR3<TYPE>& toRotate, const SWIZZLE& rotationAxis, const SCALAR_TYPE& radians) { return Rotate(toRotate, typename SWIZZLE::PARENT(rotationAxis), radians); }
template <typename TYPE> VECTOR3<TYPE> Rotate(const VECTOR3<TYPE>& toRotate, const VECTOR3<TYPE>& rotationAxis, const SCALAR_TYPE& radians)
{
	SCALAR_TYPE cr = cos(radians);
	return (toRotate * cr) + (rotationAxis * Dot(toRotate, rotationAxis) * (1 - cr)) + (Cross(rotationAxis, toRotate) * sin(radians));
}

// 3D Lerp()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Lerp(const SWIZZLE0& start, const SWIZZLE1& end, const SCALAR_TYPE& delta) { return Lerp(typename SWIZZLE0::PARENT(start), typename SWIZZLE1::PARENT(end), delta); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Lerp(const SWIZZLE& start, const VECTOR3<TYPE>& end, const SCALAR_TYPE& delta) { return Lerp(typename SWIZZLE::PARENT(start), end, delta); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Lerp(const VECTOR3<TYPE>& start, const SWIZZLE& end, const SCALAR_TYPE& delta) { return Lerp(start, typename SWIZZLE::PARENT(end), delta); }
template <typename TYPE> VECTOR3<TYPE> Lerp(const VECTOR3<TYPE>& start, const VECTOR3<TYPE>& end, const SCALAR_TYPE& delta)
{
	return VECTOR3<TYPE>(start.x + delta * (end.x - start.x),
	                     start.y + delta * (end.y - start.y),
	                     start.z + delta * (end.z - start.z));
}

// 3D Max(): Component-wise
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Max(const SWIZZLE0& a, const SWIZZLE1& b) { return Max(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const SWIZZLE& a, const VECTOR3<TYPE>& b) { return Max(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const VECTOR3<TYPE>& a, const SWIZZLE& b) { return Max(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> VECTOR3<TYPE> Max(const VECTOR3<TYPE>& a, const VECTOR3<TYPE>& b)
{
	return VECTOR3<TYPE>(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}

// 3D Min(): Component-wise
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Min(const SWIZZLE0& a, const SWIZZLE1& b) { return Min(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const SWIZZLE& a, const VECTOR3<TYPE>& b) { return Min(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const VECTOR3<TYPE>& a, const SWIZZLE& b) { return Min(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> VECTOR3<TYPE> Min(const VECTOR3<TYPE>& a, const VECTOR3<TYPE>& b)
{
	return VECTOR3<TYPE>(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}

// 3D Max(): Scalar
template <typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const SWIZZLE& a, const SCALAR_TYPE& maximum) { return Max(typename SWIZZLE::PARENT(a), maximum); }
template <typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const SCALAR_TYPE& maximum, const SWIZZLE& b) { return Max(maximum, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> VECTOR3<TYPE> Max(const VECTOR3<TYPE>& a, const SCALAR_TYPE& maximum)
{
	return VECTOR3<TYPE>(max((SCALAR_TYPE)a.x, maximum), max((SCALAR_TYPE)a.y, maximum), max((SCALAR_TYPE)a.z, maximum));
}
template <typename TYPE> VECTOR3<TYPE> Max(const SCALAR_TYPE& maximum, const VECTOR3<TYPE>& b)
{
	return VECTOR3<TYPE>(max(maximum, (SCALAR_TYPE)b.x), max(maximum, (SCALAR_TYPE)b.y), max(maximum, (SCALAR_TYPE)b.z));
}

// 3D Min(): Scalar
template <typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const SWIZZLE& a, const SCALAR_TYPE& minimum) { return Min(typename SWIZZLE::PARENT(a), minimum); }
template <typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const SCALAR_TYPE& minimum, const SWIZZLE& b) { return Min(minimum, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> VECTOR3<TYPE> Min(const VECTOR3<TYPE>& a, const SCALAR_TYPE& minimum)
{
	return VECTOR3<TYPE>(min((SCALAR_TYPE)a.x, minimum), min((SCALAR_TYPE)a.y, minimum), min((SCALAR_TYPE)a.z, minimum));
}
template <typename TYPE> VECTOR3<TYPE> Min(const SCALAR_TYPE& minimum, const VECTOR3<TYPE>& b)
{
	return VECTOR3<TYPE>(min(minimum, (SCALAR_TYPE)b.x), min(minimum, (SCALAR_TYPE)b.y), min(minimum, (SCALAR_TYPE)b.z));
}

// 3D Ceil()
template <typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Ceil(const SWIZZLE& toCeil) { return Ceil(typename SWIZZLE::PARENT(toCeil)); }
template <typename TYPE> VECTOR3<TYPE> Ceil(const VECTOR3<TYPE>& toCeil)
{
	return VECTOR3<TYPE>(ceil(toCeil.x), ceil(toCeil.y), ceil(toCeil.z));
}

// 3D Floor()
template <typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Floor(const SWIZZLE& toFloor) { return Floor(typename SWIZZLE::PARENT(toFloor)); }
template <typename TYPE> VECTOR3<TYPE> Floor(const VECTOR3<TYPE>& toFloor)
{
	return VECTOR3<TYPE>(floor(toFloor.x), floor(toFloor.y), floor(toFloor.z));
}

// 3D Distance()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, SCALAR_TYPE >::type >::type Distance(const SWIZZLE0& a, const SWIZZLE1& b) { return Distance(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Distance(const SWIZZLE& a, const VECTOR3<TYPE>& b) { return Distance(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Distance(const VECTOR3<TYPE>& a, const SWIZZLE& b) { return Distance(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> SCALAR_TYPE Distance(const VECTOR3<TYPE>& a, const VECTOR3<TYPE>& b)
{
	return sqrt(DistanceSquared(a, b));
}

// 3D DistanceSquared()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, SCALAR_TYPE >::type >::type DistanceSquared(const SWIZZLE0& a, const SWIZZLE1& b) { return DistanceSquared(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type DistanceSquared(const SWIZZLE& a, const VECTOR3<TYPE>& b) { return DistanceSquared(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type DistanceSquared(const VECTOR3<TYPE>& a, const SWIZZLE& b) { return DistanceSquared(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> SCALAR_TYPE DistanceSquared(const VECTOR3<TYPE>& a, const VECTOR3<TYPE>& b)
{
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}



//----------------------------------------------------------------------
// 
// Sec. 06 - 4D vector type and associated functions
// 
//----------------------------------------------------------------------

template <typename TYPE>
union VECTOR4
{
private:
	struct { TYPE x, y, z, w; } v;

	struct s1 { protected: TYPE x, y, z, w; };
	struct s2 { protected: TYPE x, y, z, w; public: typedef VECTOR2<TYPE> PARENT; };
	struct s3 { protected: TYPE x, y, z, w; public: typedef VECTOR3<TYPE> PARENT; };
	struct s4 { protected: TYPE x, y, z, w; public: typedef VECTOR4<TYPE> PARENT; };

	struct X : s1
	{
		operator TYPE() const { return s1::x; }
		const X& operator=(const TYPE& rhs) { s1::x = rhs; return *this; }
		const X& operator+=(const TYPE& rhs) { s1::x += rhs; return *this; }
		const X& operator-=(const TYPE& rhs) { s1::x -= rhs; return *this; }
		const X& operator*=(const TYPE& rhs) { s1::x *= rhs; return *this; }
		const X& operator/=(const TYPE& rhs) { s1::x /= rhs; return *this; }
	};
	struct Y : s1
	{
		operator TYPE() const { return s1::y; }
		const Y& operator=(const TYPE& rhs) { s1::y = rhs; return *this; }
		const Y& operator+=(const TYPE& rhs) { s1::y += rhs; return *this; }
		const Y& operator-=(const TYPE& rhs) { s1::y -= rhs; return *this; }
		const Y& operator*=(const TYPE& rhs) { s1::y *= rhs; return *this; }
		const Y& operator/=(const TYPE& rhs) { s1::y /= rhs; return *this; }
	};
	struct Z : s1
	{
		operator TYPE() const { return s1::z; }
		const Z& operator=(const TYPE& rhs) { s1::z = rhs; return *this; }
		const Z& operator+=(const TYPE& rhs) { s1::z += rhs; return *this; }
		const Z& operator-=(const TYPE& rhs) { s1::z -= rhs; return *this; }
		const Z& operator*=(const TYPE& rhs) { s1::z *= rhs; return *this; }
		const Z& operator/=(const TYPE& rhs) { s1::z /= rhs; return *this; }
	};
	struct W : s1
	{
		operator TYPE() const { return s1::w; }
		const W& operator=(const TYPE& rhs) { s1::w = rhs; return *this; }
		const W& operator+=(const TYPE& rhs) { s1::w += rhs; return *this; }
		const W& operator-=(const TYPE& rhs) { s1::w -= rhs; return *this; }
		const W& operator*=(const TYPE& rhs) { s1::w *= rhs; return *this; }
		const W& operator/=(const TYPE& rhs) { s1::w /= rhs; return *this; }
	};
	struct XX : s2 { operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::x, s2::x); } };
	struct XY : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::x, s2::y); }
		const XY& operator=(const VECTOR4<TYPE>& rhs) { s2::x = rhs.v.x; s2::y = rhs.v.y; return *this; }
		const XY& operator+=(const VECTOR4<TYPE>& rhs) { s2::x += rhs.v.x; s2::y += rhs.v.y; return *this; }
		const XY& operator-=(const VECTOR4<TYPE>& rhs) { s2::x -= rhs.v.x; s2::y -= rhs.v.y; return *this; }
		const XY& operator*=(const VECTOR4<TYPE>& rhs) { s2::x *= rhs.v.x; s2::y *= rhs.v.y; return *this; }
		const XY& operator/=(const VECTOR4<TYPE>& rhs) { s2::x /= rhs.v.x; s2::y /= rhs.v.y; return *this; }
		const XY& operator*=(const TYPE& rhs) { s2::x *= rhs; s2::y *= rhs; return *this; }
		const XY& operator/=(const TYPE& rhs) { s2::x /= rhs; s2::y /= rhs; return *this; }
	};
	struct XZ : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::x, s2::z); }
		const XZ& operator=(const VECTOR4<TYPE>& rhs) { s2::x = rhs.v.x; s2::z = rhs.v.y; return *this; }
		const XZ& operator+=(const VECTOR4<TYPE>& rhs) { s2::x += rhs.v.x; s2::z += rhs.v.y; return *this; }
		const XZ& operator-=(const VECTOR4<TYPE>& rhs) { s2::x -= rhs.v.x; s2::z -= rhs.v.y; return *this; }
		const XZ& operator*=(const VECTOR4<TYPE>& rhs) { s2::x *= rhs.v.x; s2::z *= rhs.v.y; return *this; }
		const XZ& operator/=(const VECTOR4<TYPE>& rhs) { s2::x /= rhs.v.x; s2::z /= rhs.v.y; return *this; }
		const XZ& operator*=(const TYPE& rhs) { s2::x *= rhs; s2::z *= rhs; return *this; }
		const XZ& operator/=(const TYPE& rhs) { s2::x /= rhs; s2::z /= rhs; return *this; }
	};
	struct XW : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::x, s2::w); }
		const XW& operator=(const VECTOR4<TYPE>& rhs) { s2::x = rhs.v.x; s2::w = rhs.v.y; return *this; }
		const XW& operator+=(const VECTOR4<TYPE>& rhs) { s2::x += rhs.v.x; s2::w += rhs.v.y; return *this; }
		const XW& operator-=(const VECTOR4<TYPE>& rhs) { s2::x -= rhs.v.x; s2::w -= rhs.v.y; return *this; }
		const XW& operator*=(const VECTOR4<TYPE>& rhs) { s2::x *= rhs.v.x; s2::w *= rhs.v.y; return *this; }
		const XW& operator/=(const VECTOR4<TYPE>& rhs) { s2::x /= rhs.v.x; s2::w /= rhs.v.y; return *this; }
		const XW& operator*=(const TYPE& rhs) { s2::x *= rhs; s2::w *= rhs; return *this; }
		const XW& operator/=(const TYPE& rhs) { s2::x /= rhs; s2::w /= rhs; return *this; }
	};
	struct YX : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::y, s2::x); }
		const YX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y = rhs.v.x; s2::x = t0; return *this; }
		const YX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y += rhs.v.x; s2::x += t0; return *this; }
		const YX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y -= rhs.v.x; s2::x -= t0; return *this; }
		const YX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y *= rhs.v.x; s2::x *= t0; return *this; }
		const YX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y /= rhs.v.x; s2::x /= t0; return *this; }
		const YX& operator*=(const TYPE& rhs) { s2::y *= rhs; s2::x *= rhs; return *this; }
		const YX& operator/=(const TYPE& rhs) { s2::y /= rhs; s2::x /= rhs; return *this; }
	};
	struct YY : s2 { operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::y, s2::y); } };
	struct YZ : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::y, s2::z); }
		const YZ& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y = rhs.v.x; s2::z = t0; return *this; }
		const YZ& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y += rhs.v.x; s2::z += t0; return *this; }
		const YZ& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y -= rhs.v.x; s2::z -= t0; return *this; }
		const YZ& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y *= rhs.v.x; s2::z *= t0; return *this; }
		const YZ& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y /= rhs.v.x; s2::z /= t0; return *this; }
		const YZ& operator*=(const TYPE& rhs) { s2::y *= rhs; s2::z *= rhs; return *this; }
		const YZ& operator/=(const TYPE& rhs) { s2::y /= rhs; s2::z /= rhs; return *this; }
	};
	struct YW : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::y, s2::w); }
		const YW& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y = rhs.v.x; s2::w = t0; return *this; }
		const YW& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y += rhs.v.x; s2::w += t0; return *this; }
		const YW& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y -= rhs.v.x; s2::w -= t0; return *this; }
		const YW& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y *= rhs.v.x; s2::w *= t0; return *this; }
		const YW& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::y /= rhs.v.x; s2::w /= t0; return *this; }
		const YW& operator*=(const TYPE& rhs) { s2::y *= rhs; s2::w *= rhs; return *this; }
		const YW& operator/=(const TYPE& rhs) { s2::y /= rhs; s2::w /= rhs; return *this; }
	};
	struct ZX : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::z, s2::x); }
		const ZX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z = rhs.v.x; s2::x = t0; return *this; }
		const ZX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z += rhs.v.x; s2::x += t0; return *this; }
		const ZX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z -= rhs.v.x; s2::x -= t0; return *this; }
		const ZX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z *= rhs.v.x; s2::x *= t0; return *this; }
		const ZX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z /= rhs.v.x; s2::x /= t0; return *this; }
		const ZX& operator*=(const TYPE& rhs) { s2::z *= rhs; s2::x *= rhs; return *this; }
		const ZX& operator/=(const TYPE& rhs) { s2::z /= rhs; s2::x /= rhs; return *this; }
	};
	struct ZY : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::z, s2::y); }
		const ZY& operator=(const VECTOR4<TYPE>& rhs) { s2::z = rhs.v.x; s2::y = rhs.v.y; return *this; }
		const ZY& operator+=(const VECTOR4<TYPE>& rhs) { s2::z += rhs.v.x; s2::y += rhs.v.y; return *this; }
		const ZY& operator-=(const VECTOR4<TYPE>& rhs) { s2::z -= rhs.v.x; s2::y -= rhs.v.y; return *this; }
		const ZY& operator*=(const VECTOR4<TYPE>& rhs) { s2::z *= rhs.v.x; s2::y *= rhs.v.y; return *this; }
		const ZY& operator/=(const VECTOR4<TYPE>& rhs) { s2::z /= rhs.v.x; s2::y /= rhs.v.y; return *this; }
		const ZY& operator*=(const TYPE& rhs) { s2::z *= rhs; s2::y *= rhs; return *this; }
		const ZY& operator/=(const TYPE& rhs) { s2::z /= rhs; s2::y /= rhs; return *this; }
	};
	struct ZZ : s2 { operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::z, s2::z); } };
	struct ZW : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::z, s2::w); }
		const ZW& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z = rhs.v.x; s2::w = t0; return *this; }
		const ZW& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z += rhs.v.x; s2::w += t0; return *this; }
		const ZW& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z -= rhs.v.x; s2::w -= t0; return *this; }
		const ZW& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z *= rhs.v.x; s2::w *= t0; return *this; }
		const ZW& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::z /= rhs.v.x; s2::w /= t0; return *this; }
		const ZW& operator*=(const TYPE& rhs) { s2::z *= rhs; s2::w *= rhs; return *this; }
		const ZW& operator/=(const TYPE& rhs) { s2::z /= rhs; s2::w /= rhs; return *this; }
	};
	struct WX : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::w, s2::x); }
		const WX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::w = rhs.v.x; s2::x = t0; return *this; }
		const WX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::w += rhs.v.x; s2::x += t0; return *this; }
		const WX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::w -= rhs.v.x; s2::x -= t0; return *this; }
		const WX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::w *= rhs.v.x; s2::x *= t0; return *this; }
		const WX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::w /= rhs.v.x; s2::x /= t0; return *this; }
		const WX& operator*=(const TYPE& rhs) { s2::w *= rhs; s2::x *= rhs; return *this; }
		const WX& operator/=(const TYPE& rhs) { s2::w /= rhs; s2::x /= rhs; return *this; }
	};
	struct WY : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::w, s2::y); }
		const WY& operator=(const VECTOR4<TYPE>& rhs) { s2::w = rhs.v.x; s2::y = rhs.v.y; return *this; }
		const WY& operator+=(const VECTOR4<TYPE>& rhs) { s2::w += rhs.v.x; s2::y += rhs.v.y; return *this; }
		const WY& operator-=(const VECTOR4<TYPE>& rhs) { s2::w -= rhs.v.x; s2::y -= rhs.v.y; return *this; }
		const WY& operator*=(const VECTOR4<TYPE>& rhs) { s2::w *= rhs.v.x; s2::y *= rhs.v.y; return *this; }
		const WY& operator/=(const VECTOR4<TYPE>& rhs) { s2::w /= rhs.v.x; s2::y /= rhs.v.y; return *this; }
		const WY& operator*=(const TYPE& rhs) { s2::w *= rhs; s2::y *= rhs; return *this; }
		const WY& operator/=(const TYPE& rhs) { s2::w /= rhs; s2::y /= rhs; return *this; }
	};
	struct WZ : s2
	{
		operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::w, s2::z); }
		const WZ& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::w = rhs.v.x; s2::z = t0; return *this; }
		const WZ& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::w += rhs.v.x; s2::z += t0; return *this; }
		const WZ& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::w -= rhs.v.x; s2::z -= t0; return *this; }
		const WZ& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::w *= rhs.v.x; s2::z *= t0; return *this; }
		const WZ& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s2::w /= rhs.v.x; s2::z /= t0; return *this; }
		const WZ& operator*=(const TYPE& rhs) { s2::w *= rhs; s2::z *= rhs; return *this; }
		const WZ& operator/=(const TYPE& rhs) { s2::w /= rhs; s2::z /= rhs; return *this; }
	};
	struct WW : s2 { operator VECTOR2<TYPE>() const { return VECTOR2<TYPE>(s2::w, s2::w); } };
	struct XXX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::x, s3::x); } };
	struct XXY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::x, s3::y); } };
	struct XXZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::x, s3::z); } };
	struct XXW : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::x, s3::w); } };
	struct XYX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::y, s3::x); } };
	struct XYY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::y, s3::y); } };
	struct XYZ : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::y, s3::z); }
		const XYZ& operator=(const VECTOR4<TYPE>& rhs) { s3::x = rhs.v.x; s3::y = rhs.v.y; s3::z = rhs.v.z; return *this; }
		const XYZ& operator+=(const VECTOR4<TYPE>& rhs) { s3::x += rhs.v.x; s3::y += rhs.v.y; s3::z += rhs.v.z; return *this; }
		const XYZ& operator-=(const VECTOR4<TYPE>& rhs) { s3::x -= rhs.v.x; s3::y -= rhs.v.y; s3::z -= rhs.v.z; return *this; }
		const XYZ& operator*=(const VECTOR4<TYPE>& rhs) { s3::x *= rhs.v.x; s3::y *= rhs.v.y; s3::z *= rhs.v.z; return *this; }
		const XYZ& operator/=(const VECTOR4<TYPE>& rhs) { s3::x /= rhs.v.x; s3::y /= rhs.v.y; s3::z /= rhs.v.z; return *this; }
		const XYZ& operator*=(const TYPE& rhs) { s3::x *= rhs; s3::y *= rhs; s3::z *= rhs; return *this; }
		const XYZ& operator/=(const TYPE& rhs) { s3::x /= rhs; s3::y /= rhs; s3::z /= rhs; return *this; }
	};
	struct XYW : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::y, s3::w); }
		const XYW& operator=(const VECTOR4<TYPE>& rhs) { s3::x = rhs.v.x; s3::y = rhs.v.y; s3::w = rhs.v.z; return *this; }
		const XYW& operator+=(const VECTOR4<TYPE>& rhs) { s3::x += rhs.v.x; s3::y += rhs.v.y; s3::w += rhs.v.z; return *this; }
		const XYW& operator-=(const VECTOR4<TYPE>& rhs) { s3::x -= rhs.v.x; s3::y -= rhs.v.y; s3::w -= rhs.v.z; return *this; }
		const XYW& operator*=(const VECTOR4<TYPE>& rhs) { s3::x *= rhs.v.x; s3::y *= rhs.v.y; s3::w *= rhs.v.z; return *this; }
		const XYW& operator/=(const VECTOR4<TYPE>& rhs) { s3::x /= rhs.v.x; s3::y /= rhs.v.y; s3::w /= rhs.v.z; return *this; }
		const XYW& operator*=(const TYPE& rhs) { s3::x *= rhs; s3::y *= rhs; s3::w *= rhs; return *this; }
		const XYW& operator/=(const TYPE& rhs) { s3::x /= rhs; s3::y /= rhs; s3::w /= rhs; return *this; }
	};
	struct XZX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::z, s3::x); } };
	struct XZY : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::z, s3::y); }
		const XZY& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x = rhs.v.x; s3::z = rhs.v.y; s3::y = t0; return *this; }
		const XZY& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x += rhs.v.x; s3::z += rhs.v.y; s3::y += t0; return *this; }
		const XZY& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x -= rhs.v.x; s3::z -= rhs.v.y; s3::y -= t0; return *this; }
		const XZY& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x *= rhs.v.x; s3::z *= rhs.v.y; s3::y *= t0; return *this; }
		const XZY& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x /= rhs.v.x; s3::z /= rhs.v.y; s3::y /= t0; return *this; }
		const XZY& operator*=(const TYPE& rhs) { s3::x *= rhs; s3::z *= rhs; s3::y *= rhs; return *this; }
		const XZY& operator/=(const TYPE& rhs) { s3::x /= rhs; s3::z /= rhs; s3::y /= rhs; return *this; }
	};
	struct XZZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::z, s3::z); } };
	struct XZW : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::z, s3::w); }
		const XZW& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x = rhs.v.x; s3::z = rhs.v.y; s3::w = t0; return *this; }
		const XZW& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x += rhs.v.x; s3::z += rhs.v.y; s3::w += t0; return *this; }
		const XZW& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x -= rhs.v.x; s3::z -= rhs.v.y; s3::w -= t0; return *this; }
		const XZW& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x *= rhs.v.x; s3::z *= rhs.v.y; s3::w *= t0; return *this; }
		const XZW& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x /= rhs.v.x; s3::z /= rhs.v.y; s3::w /= t0; return *this; }
		const XZW& operator*=(const TYPE& rhs) { s3::x *= rhs; s3::z *= rhs; s3::w *= rhs; return *this; }
		const XZW& operator/=(const TYPE& rhs) { s3::x /= rhs; s3::z /= rhs; s3::w /= rhs; return *this; }
	};
	struct XWX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::w, s3::x); } };
	struct XWY : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::w, s3::y); }
		const XWY& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x = rhs.v.x; s3::w = rhs.v.y; s3::y = t0; return *this; }
		const XWY& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x += rhs.v.x; s3::w += rhs.v.y; s3::y += t0; return *this; }
		const XWY& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x -= rhs.v.x; s3::w -= rhs.v.y; s3::y -= t0; return *this; }
		const XWY& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x *= rhs.v.x; s3::w *= rhs.v.y; s3::y *= t0; return *this; }
		const XWY& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::x /= rhs.v.x; s3::w /= rhs.v.y; s3::y /= t0; return *this; }
		const XWY& operator*=(const TYPE& rhs) { s3::x *= rhs; s3::w *= rhs; s3::y *= rhs; return *this; }
		const XWY& operator/=(const TYPE& rhs) { s3::x /= rhs; s3::w /= rhs; s3::y /= rhs; return *this; }
	};
	struct XWZ : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::w, s3::z); }
		const XWZ& operator=(const VECTOR4<TYPE>& rhs) { s3::x = rhs.v.x; s3::w = rhs.v.y; s3::z = rhs.v.z; return *this; }
		const XWZ& operator+=(const VECTOR4<TYPE>& rhs) { s3::x += rhs.v.x; s3::w += rhs.v.y; s3::z += rhs.v.z; return *this; }
		const XWZ& operator-=(const VECTOR4<TYPE>& rhs) { s3::x -= rhs.v.x; s3::w -= rhs.v.y; s3::z -= rhs.v.z; return *this; }
		const XWZ& operator*=(const VECTOR4<TYPE>& rhs) { s3::x *= rhs.v.x; s3::w *= rhs.v.y; s3::z *= rhs.v.z; return *this; }
		const XWZ& operator/=(const VECTOR4<TYPE>& rhs) { s3::x /= rhs.v.x; s3::w /= rhs.v.y; s3::z /= rhs.v.z; return *this; }
		const XWZ& operator*=(const TYPE& rhs) { s3::x *= rhs; s3::w *= rhs; s3::z *= rhs; return *this; }
		const XWZ& operator/=(const TYPE& rhs) { s3::x /= rhs; s3::w /= rhs; s3::z /= rhs; return *this; }
	};
	struct XWW : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::x, s3::w, s3::w); } };
	struct YXX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::x, s3::x); } };
	struct YXY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::x, s3::y); } };
	struct YXZ : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::x, s3::z); }
		const YXZ& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y = rhs.v.x; s3::x = t0; s3::z = rhs.v.z; return *this; }
		const YXZ& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y += rhs.v.x; s3::x += t0; s3::z += rhs.v.z; return *this; }
		const YXZ& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y -= rhs.v.x; s3::x -= t0; s3::z -= rhs.v.z; return *this; }
		const YXZ& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y *= rhs.v.x; s3::x *= t0; s3::z *= rhs.v.z; return *this; }
		const YXZ& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y /= rhs.v.x; s3::x /= t0; s3::z /= rhs.v.z; return *this; }
		const YXZ& operator*=(const TYPE& rhs) { s3::y *= rhs; s3::x *= rhs; s3::z *= rhs; return *this; }
		const YXZ& operator/=(const TYPE& rhs) { s3::y /= rhs; s3::x /= rhs; s3::z /= rhs; return *this; }
	};
	struct YXW : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::x, s3::w); }
		const YXW& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y = rhs.v.x; s3::x = t0; s3::w = t1; return *this; }
		const YXW& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y += rhs.v.x; s3::x += t0; s3::w += t1; return *this; }
		const YXW& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y -= rhs.v.x; s3::x -= t0; s3::w -= t1; return *this; }
		const YXW& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y *= rhs.v.x; s3::x *= t0; s3::w *= t1; return *this; }
		const YXW& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y /= rhs.v.x; s3::x /= t0; s3::w /= t1; return *this; }
		const YXW& operator*=(const TYPE& rhs) { s3::y *= rhs; s3::x *= rhs; s3::w *= rhs; return *this; }
		const YXW& operator/=(const TYPE& rhs) { s3::y /= rhs; s3::x /= rhs; s3::w /= rhs; return *this; }
	};
	struct YYX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::y, s3::x); } };
	struct YYY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::y, s3::y); } };
	struct YYZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::y, s3::z); } };
	struct YYW : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::y, s3::w); } };
	struct YZX : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::z, s3::x); }
		const YZX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y = rhs.v.x; s3::z = t0; s3::x = t1; return *this; }
		const YZX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y += rhs.v.x; s3::z += t0; s3::x += t1; return *this; }
		const YZX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y -= rhs.v.x; s3::z -= t0; s3::x -= t1; return *this; }
		const YZX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y *= rhs.v.x; s3::z *= t0; s3::x *= t1; return *this; }
		const YZX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y /= rhs.v.x; s3::z /= t0; s3::x /= t1; return *this; }
		const YZX& operator*=(const TYPE& rhs) { s3::y *= rhs; s3::z *= rhs; s3::x *= rhs; return *this; }
		const YZX& operator/=(const TYPE& rhs) { s3::y /= rhs; s3::z /= rhs; s3::x /= rhs; return *this; }
	};
	struct YZY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::z, s3::y); } };
	struct YZZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::z, s3::z); } };
	struct YZW : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::z, s3::w); }
		const YZW& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y = rhs.v.x; s3::z = t0; s3::w = t1; return *this; }
		const YZW& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y += rhs.v.x; s3::z += t0; s3::w += t1; return *this; }
		const YZW& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y -= rhs.v.x; s3::z -= t0; s3::w -= t1; return *this; }
		const YZW& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y *= rhs.v.x; s3::z *= t0; s3::w *= t1; return *this; }
		const YZW& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y /= rhs.v.x; s3::z /= t0; s3::w /= t1; return *this; }
		const YZW& operator*=(const TYPE& rhs) { s3::y *= rhs; s3::z *= rhs; s3::w *= rhs; return *this; }
		const YZW& operator/=(const TYPE& rhs) { s3::y /= rhs; s3::z /= rhs; s3::w /= rhs; return *this; }
	};
	struct YWX : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::w, s3::x); }
		const YWX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y = rhs.v.x; s3::w = t0; s3::x = t1; return *this; }
		const YWX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y += rhs.v.x; s3::w += t0; s3::x += t1; return *this; }
		const YWX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y -= rhs.v.x; s3::w -= t0; s3::x -= t1; return *this; }
		const YWX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y *= rhs.v.x; s3::w *= t0; s3::x *= t1; return *this; }
		const YWX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::y /= rhs.v.x; s3::w /= t0; s3::x /= t1; return *this; }
		const YWX& operator*=(const TYPE& rhs) { s3::y *= rhs; s3::w *= rhs; s3::x *= rhs; return *this; }
		const YWX& operator/=(const TYPE& rhs) { s3::y /= rhs; s3::w /= rhs; s3::x /= rhs; return *this; }
	};
	struct YWY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::w, s3::y); } };
	struct YWZ : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::w, s3::z); }
		const YWZ& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y = rhs.v.x; s3::w = t0; s3::z = rhs.v.z; return *this; }
		const YWZ& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y += rhs.v.x; s3::w += t0; s3::z += rhs.v.z; return *this; }
		const YWZ& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y -= rhs.v.x; s3::w -= t0; s3::z -= rhs.v.z; return *this; }
		const YWZ& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y *= rhs.v.x; s3::w *= t0; s3::z *= rhs.v.z; return *this; }
		const YWZ& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::y /= rhs.v.x; s3::w /= t0; s3::z /= rhs.v.z; return *this; }
		const YWZ& operator*=(const TYPE& rhs) { s3::y *= rhs; s3::w *= rhs; s3::z *= rhs; return *this; }
		const YWZ& operator/=(const TYPE& rhs) { s3::y /= rhs; s3::w /= rhs; s3::z /= rhs; return *this; }
	};
	struct YWW : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::y, s3::w, s3::w); } };
	struct ZXX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::x, s3::x); } };
	struct ZXY : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::x, s3::y); }
		const ZXY& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z = rhs.v.x; s3::x = t0; s3::y = t1; return *this; }
		const ZXY& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z += rhs.v.x; s3::x += t0; s3::y += t1; return *this; }
		const ZXY& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z -= rhs.v.x; s3::x -= t0; s3::y -= t1; return *this; }
		const ZXY& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z *= rhs.v.x; s3::x *= t0; s3::y *= t1; return *this; }
		const ZXY& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z /= rhs.v.x; s3::x /= t0; s3::y /= t1; return *this; }
		const ZXY& operator*=(const TYPE& rhs) { s3::z *= rhs; s3::x *= rhs; s3::y *= rhs; return *this; }
		const ZXY& operator/=(const TYPE& rhs) { s3::z /= rhs; s3::x /= rhs; s3::y /= rhs; return *this; }
	};
	struct ZXZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::x, s3::z); } };
	struct ZXW : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::x, s3::w); }
		const ZXW& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z = rhs.v.x; s3::x = t0; s3::w = t1; return *this; }
		const ZXW& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z += rhs.v.x; s3::x += t0; s3::w += t1; return *this; }
		const ZXW& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z -= rhs.v.x; s3::x -= t0; s3::w -= t1; return *this; }
		const ZXW& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z *= rhs.v.x; s3::x *= t0; s3::w *= t1; return *this; }
		const ZXW& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z /= rhs.v.x; s3::x /= t0; s3::w /= t1; return *this; }
		const ZXW& operator*=(const TYPE& rhs) { s3::z *= rhs; s3::x *= rhs; s3::w *= rhs; return *this; }
		const ZXW& operator/=(const TYPE& rhs) { s3::z /= rhs; s3::x /= rhs; s3::w /= rhs; return *this; }
	};
	struct ZYX : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::y, s3::x); }
		const ZYX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z = rhs.v.x; s3::y = rhs.v.y; s3::x = t0; return *this; }
		const ZYX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z += rhs.v.x; s3::y += rhs.v.y; s3::x += t0; return *this; }
		const ZYX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z -= rhs.v.x; s3::y -= rhs.v.y; s3::x -= t0; return *this; }
		const ZYX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z *= rhs.v.x; s3::y *= rhs.v.y; s3::x *= t0; return *this; }
		const ZYX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z /= rhs.v.x; s3::y /= rhs.v.y; s3::x /= t0; return *this; }
		const ZYX& operator*=(const TYPE& rhs) { s3::z *= rhs; s3::y *= rhs; s3::x *= rhs; return *this; }
		const ZYX& operator/=(const TYPE& rhs) { s3::z /= rhs; s3::y /= rhs; s3::x /= rhs; return *this; }
	};
	struct ZYY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::y, s3::y); } };
	struct ZYZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::y, s3::z); } };
	struct ZYW : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::y, s3::w); }
		const ZYW& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z = rhs.v.x; s3::y = rhs.v.y; s3::w = t0; return *this; }
		const ZYW& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z += rhs.v.x; s3::y += rhs.v.y; s3::w += t0; return *this; }
		const ZYW& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z -= rhs.v.x; s3::y -= rhs.v.y; s3::w -= t0; return *this; }
		const ZYW& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z *= rhs.v.x; s3::y *= rhs.v.y; s3::w *= t0; return *this; }
		const ZYW& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::z /= rhs.v.x; s3::y /= rhs.v.y; s3::w /= t0; return *this; }
		const ZYW& operator*=(const TYPE& rhs) { s3::z *= rhs; s3::y *= rhs; s3::w *= rhs; return *this; }
		const ZYW& operator/=(const TYPE& rhs) { s3::z /= rhs; s3::y /= rhs; s3::w /= rhs; return *this; }
	};
	struct ZZX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::z, s3::x); } };
	struct ZZY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::z, s3::y); } };
	struct ZZZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::z, s3::z); } };
	struct ZZW : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::z, s3::w); } };
	struct ZWX : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::w, s3::x); }
		const ZWX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z = rhs.v.x; s3::w = t0; s3::x = t1; return *this; }
		const ZWX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z += rhs.v.x; s3::w += t0; s3::x += t1; return *this; }
		const ZWX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z -= rhs.v.x; s3::w -= t0; s3::x -= t1; return *this; }
		const ZWX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z *= rhs.v.x; s3::w *= t0; s3::x *= t1; return *this; }
		const ZWX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z /= rhs.v.x; s3::w /= t0; s3::x /= t1; return *this; }
		const ZWX& operator*=(const TYPE& rhs) { s3::z *= rhs; s3::w *= rhs; s3::x *= rhs; return *this; }
		const ZWX& operator/=(const TYPE& rhs) { s3::z /= rhs; s3::w /= rhs; s3::x /= rhs; return *this; }
	};
	struct ZWY : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::w, s3::y); }
		const ZWY& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z = rhs.v.x; s3::w = t0; s3::y = t1; return *this; }
		const ZWY& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z += rhs.v.x; s3::w += t0; s3::y += t1; return *this; }
		const ZWY& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z -= rhs.v.x; s3::w -= t0; s3::y -= t1; return *this; }
		const ZWY& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z *= rhs.v.x; s3::w *= t0; s3::y *= t1; return *this; }
		const ZWY& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::z /= rhs.v.x; s3::w /= t0; s3::y /= t1; return *this; }
		const ZWY& operator*=(const TYPE& rhs) { s3::z *= rhs; s3::w *= rhs; s3::y *= rhs; return *this; }
		const ZWY& operator/=(const TYPE& rhs) { s3::z /= rhs; s3::w /= rhs; s3::y /= rhs; return *this; }
	};
	struct ZWZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::w, s3::z); } };
	struct ZWW : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::z, s3::w, s3::w); } };
	struct WXX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::x, s3::x); } };
	struct WXY : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::x, s3::y); }
		const WXY& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w = rhs.v.x; s3::x = t0; s3::y = t1; return *this; }
		const WXY& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w += rhs.v.x; s3::x += t0; s3::y += t1; return *this; }
		const WXY& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w -= rhs.v.x; s3::x -= t0; s3::y -= t1; return *this; }
		const WXY& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w *= rhs.v.x; s3::x *= t0; s3::y *= t1; return *this; }
		const WXY& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w /= rhs.v.x; s3::x /= t0; s3::y /= t1; return *this; }
		const WXY& operator*=(const TYPE& rhs) { s3::w *= rhs; s3::x *= rhs; s3::y *= rhs; return *this; }
		const WXY& operator/=(const TYPE& rhs) { s3::w /= rhs; s3::x /= rhs; s3::y /= rhs; return *this; }
	};
	struct WXZ : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::x, s3::z); }
		const WXZ& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::w = rhs.v.x; s3::x = t0; s3::z = rhs.v.z; return *this; }
		const WXZ& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::w += rhs.v.x; s3::x += t0; s3::z += rhs.v.z; return *this; }
		const WXZ& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::w -= rhs.v.x; s3::x -= t0; s3::z -= rhs.v.z; return *this; }
		const WXZ& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::w *= rhs.v.x; s3::x *= t0; s3::z *= rhs.v.z; return *this; }
		const WXZ& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s3::w /= rhs.v.x; s3::x /= t0; s3::z /= rhs.v.z; return *this; }
		const WXZ& operator*=(const TYPE& rhs) { s3::w *= rhs; s3::x *= rhs; s3::z *= rhs; return *this; }
		const WXZ& operator/=(const TYPE& rhs) { s3::w /= rhs; s3::x /= rhs; s3::z /= rhs; return *this; }
	};
	struct WXW : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::x, s3::w); } };
	struct WYX : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::y, s3::x); }
		const WYX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::w = rhs.v.x; s3::y = rhs.v.y; s3::x = t0; return *this; }
		const WYX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::w += rhs.v.x; s3::y += rhs.v.y; s3::x += t0; return *this; }
		const WYX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::w -= rhs.v.x; s3::y -= rhs.v.y; s3::x -= t0; return *this; }
		const WYX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::w *= rhs.v.x; s3::y *= rhs.v.y; s3::x *= t0; return *this; }
		const WYX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s3::w /= rhs.v.x; s3::y /= rhs.v.y; s3::x /= t0; return *this; }
		const WYX& operator*=(const TYPE& rhs) { s3::w *= rhs; s3::y *= rhs; s3::x *= rhs; return *this; }
		const WYX& operator/=(const TYPE& rhs) { s3::w /= rhs; s3::y /= rhs; s3::x /= rhs; return *this; }
	};
	struct WYY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::y, s3::y); } };
	struct WYZ : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::y, s3::z); }
		const WYZ& operator=(const VECTOR4<TYPE>& rhs) { s3::w = rhs.v.x; s3::y = rhs.v.y; s3::z = rhs.v.z; return *this; }
		const WYZ& operator+=(const VECTOR4<TYPE>& rhs) { s3::w += rhs.v.x; s3::y += rhs.v.y; s3::z += rhs.v.z; return *this; }
		const WYZ& operator-=(const VECTOR4<TYPE>& rhs) { s3::w -= rhs.v.x; s3::y -= rhs.v.y; s3::z -= rhs.v.z; return *this; }
		const WYZ& operator*=(const VECTOR4<TYPE>& rhs) { s3::w *= rhs.v.x; s3::y *= rhs.v.y; s3::z *= rhs.v.z; return *this; }
		const WYZ& operator/=(const VECTOR4<TYPE>& rhs) { s3::w /= rhs.v.x; s3::y /= rhs.v.y; s3::z /= rhs.v.z; return *this; }
		const WYZ& operator*=(const TYPE& rhs) { s3::w *= rhs; s3::y *= rhs; s3::z *= rhs; return *this; }
		const WYZ& operator/=(const TYPE& rhs) { s3::w /= rhs; s3::y /= rhs; s3::z /= rhs; return *this; }
	};
	struct WYW : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::y, s3::w); } };
	struct WZX : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::z, s3::x); }
		const WZX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w = rhs.v.x; s3::z = t0; s3::x = t1; return *this; }
		const WZX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w += rhs.v.x; s3::z += t0; s3::x += t1; return *this; }
		const WZX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w -= rhs.v.x; s3::z -= t0; s3::x -= t1; return *this; }
		const WZX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w *= rhs.v.x; s3::z *= t0; s3::x *= t1; return *this; }
		const WZX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w /= rhs.v.x; s3::z /= t0; s3::x /= t1; return *this; }
		const WZX& operator*=(const TYPE& rhs) { s3::w *= rhs; s3::z *= rhs; s3::x *= rhs; return *this; }
		const WZX& operator/=(const TYPE& rhs) { s3::w /= rhs; s3::z /= rhs; s3::x /= rhs; return *this; }
	};
	struct WZY : s3
	{
		operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::z, s3::y); }
		const WZY& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w = rhs.v.x; s3::z = t0; s3::y = t1; return *this; }
		const WZY& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w += rhs.v.x; s3::z += t0; s3::y += t1; return *this; }
		const WZY& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w -= rhs.v.x; s3::z -= t0; s3::y -= t1; return *this; }
		const WZY& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w *= rhs.v.x; s3::z *= t0; s3::y *= t1; return *this; }
		const WZY& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s3::w /= rhs.v.x; s3::z /= t0; s3::y /= t1; return *this; }
		const WZY& operator*=(const TYPE& rhs) { s3::w *= rhs; s3::z *= rhs; s3::y *= rhs; return *this; }
		const WZY& operator/=(const TYPE& rhs) { s3::w /= rhs; s3::z /= rhs; s3::y /= rhs; return *this; }
	};
	struct WZZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::z, s3::z); } };
	struct WZW : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::z, s3::w); } };
	struct WWX : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::w, s3::x); } };
	struct WWY : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::w, s3::y); } };
	struct WWZ : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::w, s3::z); } };
	struct WWW : s3 { operator VECTOR3<TYPE>() const { return VECTOR3<TYPE>(s3::w, s3::w, s3::w); } };
	struct XXXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::x, s4::x); } };
	struct XXXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::x, s4::y); } };
	struct XXXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::x, s4::z); } };
	struct XXXW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::x, s4::w); } };
	struct XXYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::y, s4::x); } };
	struct XXYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::y, s4::y); } };
	struct XXYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::y, s4::z); } };
	struct XXYW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::y, s4::w); } };
	struct XXZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::z, s4::x); } };
	struct XXZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::z, s4::y); } };
	struct XXZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::z, s4::z); } };
	struct XXZW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::z, s4::w); } };
	struct XXWX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::w, s4::x); } };
	struct XXWY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::w, s4::y); } };
	struct XXWZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::w, s4::z); } };
	struct XXWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::x, s4::w, s4::w); } };
	struct XYXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::x, s4::x); } };
	struct XYXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::x, s4::y); } };
	struct XYXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::x, s4::z); } };
	struct XYXW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::x, s4::w); } };
	struct XYYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::y, s4::x); } };
	struct XYYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::y, s4::y); } };
	struct XYYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::y, s4::z); } };
	struct XYYW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::y, s4::w); } };
	struct XYZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::z, s4::x); } };
	struct XYZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::z, s4::y); } };
	struct XYZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::z, s4::z); } };
	struct XYZW : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::z, s4::w); }
		const XYZW& operator=(const VECTOR4<TYPE>& rhs) { s4::x = rhs.v.x; s4::y = rhs.v.y; s4::z = rhs.v.z; s4::w = rhs.v.w; return *this; }
		const XYZW& operator+=(const VECTOR4<TYPE>& rhs) { s4::x += rhs.v.x; s4::y += rhs.v.y; s4::z += rhs.v.z; s4::w += rhs.v.w; return *this; }
		const XYZW& operator-=(const VECTOR4<TYPE>& rhs) { s4::x -= rhs.v.x; s4::y -= rhs.v.y; s4::z -= rhs.v.z; s4::w -= rhs.v.w; return *this; }
		const XYZW& operator*=(const VECTOR4<TYPE>& rhs) { s4::x *= rhs.v.x; s4::y *= rhs.v.y; s4::z *= rhs.v.z; s4::w *= rhs.v.w; return *this; }
		const XYZW& operator/=(const VECTOR4<TYPE>& rhs) { s4::x /= rhs.v.x; s4::y /= rhs.v.y; s4::z /= rhs.v.z; s4::w /= rhs.v.w; return *this; }
		const XYZW& operator*=(const TYPE& rhs) { s4::x *= rhs; s4::y *= rhs; s4::z *= rhs; s4::w *= rhs; return *this; }
		const XYZW& operator/=(const TYPE& rhs) { s4::x /= rhs; s4::y /= rhs; s4::z /= rhs; s4::w /= rhs; return *this; }
	};
	struct XYWX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::w, s4::x); } };
	struct XYWY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::w, s4::y); } };
	struct XYWZ : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::w, s4::z); }
		const XYWZ& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::x = rhs.v.x; s4::y = rhs.v.y; s4::w = rhs.v.z; s4::z = t0; return *this; }
		const XYWZ& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::x += rhs.v.x; s4::y += rhs.v.y; s4::w += rhs.v.z; s4::z += t0; return *this; }
		const XYWZ& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::x -= rhs.v.x; s4::y -= rhs.v.y; s4::w -= rhs.v.z; s4::z -= t0; return *this; }
		const XYWZ& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::x *= rhs.v.x; s4::y *= rhs.v.y; s4::w *= rhs.v.z; s4::z *= t0; return *this; }
		const XYWZ& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::x /= rhs.v.x; s4::y /= rhs.v.y; s4::w /= rhs.v.z; s4::z /= t0; return *this; }
		const XYWZ& operator*=(const TYPE& rhs) { s4::x *= rhs; s4::y *= rhs; s4::w *= rhs; s4::z *= rhs; return *this; }
		const XYWZ& operator/=(const TYPE& rhs) { s4::x /= rhs; s4::y /= rhs; s4::w /= rhs; s4::z /= rhs; return *this; }
	};
	struct XYWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::y, s4::w, s4::w); } };
	struct XZXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::x, s4::x); } };
	struct XZXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::x, s4::y); } };
	struct XZXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::x, s4::z); } };
	struct XZXW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::x, s4::w); } };
	struct XZYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::y, s4::x); } };
	struct XZYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::y, s4::y); } };
	struct XZYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::y, s4::z); } };
	struct XZYW : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::y, s4::w); }
		const XZYW& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s4::x = rhs.v.x; s4::z = rhs.v.y; s4::y = t0; s4::w = rhs.v.w; return *this; }
		const XZYW& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s4::x += rhs.v.x; s4::z += rhs.v.y; s4::y += t0; s4::w += rhs.v.w; return *this; }
		const XZYW& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s4::x -= rhs.v.x; s4::z -= rhs.v.y; s4::y -= t0; s4::w -= rhs.v.w; return *this; }
		const XZYW& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s4::x *= rhs.v.x; s4::z *= rhs.v.y; s4::y *= t0; s4::w *= rhs.v.w; return *this; }
		const XZYW& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s4::x /= rhs.v.x; s4::z /= rhs.v.y; s4::y /= t0; s4::w /= rhs.v.w; return *this; }
		const XZYW& operator*=(const TYPE& rhs) { s4::x *= rhs; s4::z *= rhs; s4::y *= rhs; s4::w *= rhs; return *this; }
		const XZYW& operator/=(const TYPE& rhs) { s4::x /= rhs; s4::z /= rhs; s4::y /= rhs; s4::w /= rhs; return *this; }
	};
	struct XZZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::z, s4::x); } };
	struct XZZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::z, s4::y); } };
	struct XZZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::z, s4::z); } };
	struct XZZW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::z, s4::w); } };
	struct XZWX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::w, s4::x); } };
	struct XZWY : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::w, s4::y); }
		const XZWY& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::x = rhs.v.x; s4::z = rhs.v.y; s4::w = t0; s4::y = t1; return *this; }
		const XZWY& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::x += rhs.v.x; s4::z += rhs.v.y; s4::w += t0; s4::y += t1; return *this; }
		const XZWY& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::x -= rhs.v.x; s4::z -= rhs.v.y; s4::w -= t0; s4::y -= t1; return *this; }
		const XZWY& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::x *= rhs.v.x; s4::z *= rhs.v.y; s4::w *= t0; s4::y *= t1; return *this; }
		const XZWY& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::x /= rhs.v.x; s4::z /= rhs.v.y; s4::w /= t0; s4::y /= t1; return *this; }
		const XZWY& operator*=(const TYPE& rhs) { s4::x *= rhs; s4::z *= rhs; s4::w *= rhs; s4::y *= rhs; return *this; }
		const XZWY& operator/=(const TYPE& rhs) { s4::x /= rhs; s4::z /= rhs; s4::w /= rhs; s4::y /= rhs; return *this; }
	};
	struct XZWZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::w, s4::z); } };
	struct XZWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::z, s4::w, s4::w); } };
	struct XWXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::x, s4::x); } };
	struct XWXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::x, s4::y); } };
	struct XWXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::x, s4::z); } };
	struct XWXW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::x, s4::w); } };
	struct XWYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::y, s4::x); } };
	struct XWYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::y, s4::y); } };
	struct XWYZ : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::y, s4::z); }
		const XWYZ& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::x = rhs.v.x; s4::w = rhs.v.y; s4::y = t0; s4::z = t1; return *this; }
		const XWYZ& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::x += rhs.v.x; s4::w += rhs.v.y; s4::y += t0; s4::z += t1; return *this; }
		const XWYZ& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::x -= rhs.v.x; s4::w -= rhs.v.y; s4::y -= t0; s4::z -= t1; return *this; }
		const XWYZ& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::x *= rhs.v.x; s4::w *= rhs.v.y; s4::y *= t0; s4::z *= t1; return *this; }
		const XWYZ& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::x /= rhs.v.x; s4::w /= rhs.v.y; s4::y /= t0; s4::z /= t1; return *this; }
		const XWYZ& operator*=(const TYPE& rhs) { s4::x *= rhs; s4::w *= rhs; s4::y *= rhs; s4::z *= rhs; return *this; }
		const XWYZ& operator/=(const TYPE& rhs) { s4::x /= rhs; s4::w /= rhs; s4::y /= rhs; s4::z /= rhs; return *this; }
	};
	struct XWYW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::y, s4::w); } };
	struct XWZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::z, s4::x); } };
	struct XWZY : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::z, s4::y); }
		const XWZY& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::x = rhs.v.x; s4::w = rhs.v.y; s4::z = rhs.v.z; s4::y = t0; return *this; }
		const XWZY& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::x += rhs.v.x; s4::w += rhs.v.y; s4::z += rhs.v.z; s4::y += t0; return *this; }
		const XWZY& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::x -= rhs.v.x; s4::w -= rhs.v.y; s4::z -= rhs.v.z; s4::y -= t0; return *this; }
		const XWZY& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::x *= rhs.v.x; s4::w *= rhs.v.y; s4::z *= rhs.v.z; s4::y *= t0; return *this; }
		const XWZY& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::x /= rhs.v.x; s4::w /= rhs.v.y; s4::z /= rhs.v.z; s4::y /= t0; return *this; }
		const XWZY& operator*=(const TYPE& rhs) { s4::x *= rhs; s4::w *= rhs; s4::z *= rhs; s4::y *= rhs; return *this; }
		const XWZY& operator/=(const TYPE& rhs) { s4::x /= rhs; s4::w /= rhs; s4::z /= rhs; s4::y /= rhs; return *this; }
	};
	struct XWZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::z, s4::z); } };
	struct XWZW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::z, s4::w); } };
	struct XWWX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::w, s4::x); } };
	struct XWWY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::w, s4::y); } };
	struct XWWZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::w, s4::z); } };
	struct XWWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::x, s4::w, s4::w, s4::w); } };
	struct YXXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::x, s4::x); } };
	struct YXXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::x, s4::y); } };
	struct YXXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::x, s4::z); } };
	struct YXXW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::x, s4::w); } };
	struct YXYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::y, s4::x); } };
	struct YXYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::y, s4::y); } };
	struct YXYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::y, s4::z); } };
	struct YXYW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::y, s4::w); } };
	struct YXZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::z, s4::x); } };
	struct YXZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::z, s4::y); } };
	struct YXZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::z, s4::z); } };
	struct YXZW : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::z, s4::w); }
		const YXZW& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s4::y = rhs.v.x; s4::x = t0; s4::z = rhs.v.z; s4::w = rhs.v.w; return *this; }
		const YXZW& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s4::y += rhs.v.x; s4::x += t0; s4::z += rhs.v.z; s4::w += rhs.v.w; return *this; }
		const YXZW& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s4::y -= rhs.v.x; s4::x -= t0; s4::z -= rhs.v.z; s4::w -= rhs.v.w; return *this; }
		const YXZW& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s4::y *= rhs.v.x; s4::x *= t0; s4::z *= rhs.v.z; s4::w *= rhs.v.w; return *this; }
		const YXZW& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; s4::y /= rhs.v.x; s4::x /= t0; s4::z /= rhs.v.z; s4::w /= rhs.v.w; return *this; }
		const YXZW& operator*=(const TYPE& rhs) { s4::y *= rhs; s4::x *= rhs; s4::z *= rhs; s4::w *= rhs; return *this; }
		const YXZW& operator/=(const TYPE& rhs) { s4::y /= rhs; s4::x /= rhs; s4::z /= rhs; s4::w /= rhs; return *this; }
	};
	struct YXWX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::w, s4::x); } };
	struct YXWY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::w, s4::y); } };
	struct YXWZ : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::w, s4::z); }
		const YXWZ& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y = rhs.v.x; s4::x = t0; s4::w = t1; s4::z = t2; return *this; }
		const YXWZ& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y += rhs.v.x; s4::x += t0; s4::w += t1; s4::z += t2; return *this; }
		const YXWZ& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y -= rhs.v.x; s4::x -= t0; s4::w -= t1; s4::z -= t2; return *this; }
		const YXWZ& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y *= rhs.v.x; s4::x *= t0; s4::w *= t1; s4::z *= t2; return *this; }
		const YXWZ& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y /= rhs.v.x; s4::x /= t0; s4::w /= t1; s4::z /= t2; return *this; }
		const YXWZ& operator*=(const TYPE& rhs) { s4::y *= rhs; s4::x *= rhs; s4::w *= rhs; s4::z *= rhs; return *this; }
		const YXWZ& operator/=(const TYPE& rhs) { s4::y /= rhs; s4::x /= rhs; s4::w /= rhs; s4::z /= rhs; return *this; }
	};
	struct YXWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::x, s4::w, s4::w); } };
	struct YYXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::x, s4::x); } };
	struct YYXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::x, s4::y); } };
	struct YYXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::x, s4::z); } };
	struct YYXW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::x, s4::w); } };
	struct YYYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::y, s4::x); } };
	struct YYYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::y, s4::y); } };
	struct YYYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::y, s4::z); } };
	struct YYYW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::y, s4::w); } };
	struct YYZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::z, s4::x); } };
	struct YYZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::z, s4::y); } };
	struct YYZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::z, s4::z); } };
	struct YYZW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::z, s4::w); } };
	struct YYWX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::w, s4::x); } };
	struct YYWY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::w, s4::y); } };
	struct YYWZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::w, s4::z); } };
	struct YYWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::y, s4::w, s4::w); } };
	struct YZXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::x, s4::x); } };
	struct YZXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::x, s4::y); } };
	struct YZXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::x, s4::z); } };
	struct YZXW : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::x, s4::w); }
		const YZXW& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s4::y = rhs.v.x; s4::z = t0; s4::x = t1; s4::w = rhs.v.w; return *this; }
		const YZXW& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s4::y += rhs.v.x; s4::z += t0; s4::x += t1; s4::w += rhs.v.w; return *this; }
		const YZXW& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s4::y -= rhs.v.x; s4::z -= t0; s4::x -= t1; s4::w -= rhs.v.w; return *this; }
		const YZXW& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s4::y *= rhs.v.x; s4::z *= t0; s4::x *= t1; s4::w *= rhs.v.w; return *this; }
		const YZXW& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s4::y /= rhs.v.x; s4::z /= t0; s4::x /= t1; s4::w /= rhs.v.w; return *this; }
		const YZXW& operator*=(const TYPE& rhs) { s4::y *= rhs; s4::z *= rhs; s4::x *= rhs; s4::w *= rhs; return *this; }
		const YZXW& operator/=(const TYPE& rhs) { s4::y /= rhs; s4::z /= rhs; s4::x /= rhs; s4::w /= rhs; return *this; }
	};
	struct YZYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::y, s4::x); } };
	struct YZYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::y, s4::y); } };
	struct YZYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::y, s4::z); } };
	struct YZYW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::y, s4::w); } };
	struct YZZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::z, s4::x); } };
	struct YZZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::z, s4::y); } };
	struct YZZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::z, s4::z); } };
	struct YZZW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::z, s4::w); } };
	struct YZWX : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::w, s4::x); }
		const YZWX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y = rhs.v.x; s4::z = t0; s4::w = t1; s4::x = t2; return *this; }
		const YZWX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y += rhs.v.x; s4::z += t0; s4::w += t1; s4::x += t2; return *this; }
		const YZWX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y -= rhs.v.x; s4::z -= t0; s4::w -= t1; s4::x -= t2; return *this; }
		const YZWX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y *= rhs.v.x; s4::z *= t0; s4::w *= t1; s4::x *= t2; return *this; }
		const YZWX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y /= rhs.v.x; s4::z /= t0; s4::w /= t1; s4::x /= t2; return *this; }
		const YZWX& operator*=(const TYPE& rhs) { s4::y *= rhs; s4::z *= rhs; s4::w *= rhs; s4::x *= rhs; return *this; }
		const YZWX& operator/=(const TYPE& rhs) { s4::y /= rhs; s4::z /= rhs; s4::w /= rhs; s4::x /= rhs; return *this; }
	};
	struct YZWY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::w, s4::y); } };
	struct YZWZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::w, s4::z); } };
	struct YZWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::z, s4::w, s4::w); } };
	struct YWXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::x, s4::x); } };
	struct YWXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::x, s4::y); } };
	struct YWXZ : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::x, s4::z); }
		const YWXZ& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y = rhs.v.x; s4::w = t0; s4::x = t1; s4::z = t2; return *this; }
		const YWXZ& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y += rhs.v.x; s4::w += t0; s4::x += t1; s4::z += t2; return *this; }
		const YWXZ& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y -= rhs.v.x; s4::w -= t0; s4::x -= t1; s4::z -= t2; return *this; }
		const YWXZ& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y *= rhs.v.x; s4::w *= t0; s4::x *= t1; s4::z *= t2; return *this; }
		const YWXZ& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::y /= rhs.v.x; s4::w /= t0; s4::x /= t1; s4::z /= t2; return *this; }
		const YWXZ& operator*=(const TYPE& rhs) { s4::y *= rhs; s4::w *= rhs; s4::x *= rhs; s4::z *= rhs; return *this; }
		const YWXZ& operator/=(const TYPE& rhs) { s4::y /= rhs; s4::w /= rhs; s4::x /= rhs; s4::z /= rhs; return *this; }
	};
	struct YWXW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::x, s4::w); } };
	struct YWYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::y, s4::x); } };
	struct YWYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::y, s4::y); } };
	struct YWYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::y, s4::z); } };
	struct YWYW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::y, s4::w); } };
	struct YWZX : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::z, s4::x); }
		const YWZX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.w; s4::y = rhs.v.x; s4::w = t0; s4::z = rhs.v.z; s4::x = t1; return *this; }
		const YWZX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.w; s4::y += rhs.v.x; s4::w += t0; s4::z += rhs.v.z; s4::x += t1; return *this; }
		const YWZX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.w; s4::y -= rhs.v.x; s4::w -= t0; s4::z -= rhs.v.z; s4::x -= t1; return *this; }
		const YWZX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.w; s4::y *= rhs.v.x; s4::w *= t0; s4::z *= rhs.v.z; s4::x *= t1; return *this; }
		const YWZX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.w; s4::y /= rhs.v.x; s4::w /= t0; s4::z /= rhs.v.z; s4::x /= t1; return *this; }
		const YWZX& operator*=(const TYPE& rhs) { s4::y *= rhs; s4::w *= rhs; s4::z *= rhs; s4::x *= rhs; return *this; }
		const YWZX& operator/=(const TYPE& rhs) { s4::y /= rhs; s4::w /= rhs; s4::z /= rhs; s4::x /= rhs; return *this; }
	};
	struct YWZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::z, s4::y); } };
	struct YWZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::z, s4::z); } };
	struct YWZW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::z, s4::w); } };
	struct YWWX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::w, s4::x); } };
	struct YWWY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::w, s4::y); } };
	struct YWWZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::w, s4::z); } };
	struct YWWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::y, s4::w, s4::w, s4::w); } };
	struct ZXXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::x, s4::x); } };
	struct ZXXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::x, s4::y); } };
	struct ZXXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::x, s4::z); } };
	struct ZXXW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::x, s4::w); } };
	struct ZXYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::y, s4::x); } };
	struct ZXYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::y, s4::y); } };
	struct ZXYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::y, s4::z); } };
	struct ZXYW : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::y, s4::w); }
		const ZXYW& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s4::z = rhs.v.x; s4::x = t0; s4::y = t1; s4::w = rhs.v.w; return *this; }
		const ZXYW& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s4::z += rhs.v.x; s4::x += t0; s4::y += t1; s4::w += rhs.v.w; return *this; }
		const ZXYW& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s4::z -= rhs.v.x; s4::x -= t0; s4::y -= t1; s4::w -= rhs.v.w; return *this; }
		const ZXYW& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s4::z *= rhs.v.x; s4::x *= t0; s4::y *= t1; s4::w *= rhs.v.w; return *this; }
		const ZXYW& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; s4::z /= rhs.v.x; s4::x /= t0; s4::y /= t1; s4::w /= rhs.v.w; return *this; }
		const ZXYW& operator*=(const TYPE& rhs) { s4::z *= rhs; s4::x *= rhs; s4::y *= rhs; s4::w *= rhs; return *this; }
		const ZXYW& operator/=(const TYPE& rhs) { s4::z /= rhs; s4::x /= rhs; s4::y /= rhs; s4::w /= rhs; return *this; }
	};
	struct ZXZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::z, s4::x); } };
	struct ZXZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::z, s4::y); } };
	struct ZXZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::z, s4::z); } };
	struct ZXZW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::z, s4::w); } };
	struct ZXWX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::w, s4::x); } };
	struct ZXWY : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::w, s4::y); }
		const ZXWY& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z = rhs.v.x; s4::x = t0; s4::w = t1; s4::y = t2; return *this; }
		const ZXWY& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z += rhs.v.x; s4::x += t0; s4::w += t1; s4::y += t2; return *this; }
		const ZXWY& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z -= rhs.v.x; s4::x -= t0; s4::w -= t1; s4::y -= t2; return *this; }
		const ZXWY& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z *= rhs.v.x; s4::x *= t0; s4::w *= t1; s4::y *= t2; return *this; }
		const ZXWY& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z /= rhs.v.x; s4::x /= t0; s4::w /= t1; s4::y /= t2; return *this; }
		const ZXWY& operator*=(const TYPE& rhs) { s4::z *= rhs; s4::x *= rhs; s4::w *= rhs; s4::y *= rhs; return *this; }
		const ZXWY& operator/=(const TYPE& rhs) { s4::z /= rhs; s4::x /= rhs; s4::w /= rhs; s4::y /= rhs; return *this; }
	};
	struct ZXWZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::w, s4::z); } };
	struct ZXWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::x, s4::w, s4::w); } };
	struct ZYXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::x, s4::x); } };
	struct ZYXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::x, s4::y); } };
	struct ZYXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::x, s4::z); } };
	struct ZYXW : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::x, s4::w); }
		const ZYXW& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s4::z = rhs.v.x; s4::y = rhs.v.y; s4::x = t0; s4::w = rhs.v.w; return *this; }
		const ZYXW& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s4::z += rhs.v.x; s4::y += rhs.v.y; s4::x += t0; s4::w += rhs.v.w; return *this; }
		const ZYXW& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s4::z -= rhs.v.x; s4::y -= rhs.v.y; s4::x -= t0; s4::w -= rhs.v.w; return *this; }
		const ZYXW& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s4::z *= rhs.v.x; s4::y *= rhs.v.y; s4::x *= t0; s4::w *= rhs.v.w; return *this; }
		const ZYXW& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; s4::z /= rhs.v.x; s4::y /= rhs.v.y; s4::x /= t0; s4::w /= rhs.v.w; return *this; }
		const ZYXW& operator*=(const TYPE& rhs) { s4::z *= rhs; s4::y *= rhs; s4::x *= rhs; s4::w *= rhs; return *this; }
		const ZYXW& operator/=(const TYPE& rhs) { s4::z /= rhs; s4::y /= rhs; s4::x /= rhs; s4::w /= rhs; return *this; }
	};
	struct ZYYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::y, s4::x); } };
	struct ZYYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::y, s4::y); } };
	struct ZYYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::y, s4::z); } };
	struct ZYYW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::y, s4::w); } };
	struct ZYZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::z, s4::x); } };
	struct ZYZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::z, s4::y); } };
	struct ZYZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::z, s4::z); } };
	struct ZYZW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::z, s4::w); } };
	struct ZYWX : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::w, s4::x); }
		const ZYWX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::z = rhs.v.x; s4::y = rhs.v.y; s4::w = t0; s4::x = t1; return *this; }
		const ZYWX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::z += rhs.v.x; s4::y += rhs.v.y; s4::w += t0; s4::x += t1; return *this; }
		const ZYWX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::z -= rhs.v.x; s4::y -= rhs.v.y; s4::w -= t0; s4::x -= t1; return *this; }
		const ZYWX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::z *= rhs.v.x; s4::y *= rhs.v.y; s4::w *= t0; s4::x *= t1; return *this; }
		const ZYWX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::z /= rhs.v.x; s4::y /= rhs.v.y; s4::w /= t0; s4::x /= t1; return *this; }
		const ZYWX& operator*=(const TYPE& rhs) { s4::z *= rhs; s4::y *= rhs; s4::w *= rhs; s4::x *= rhs; return *this; }
		const ZYWX& operator/=(const TYPE& rhs) { s4::z /= rhs; s4::y /= rhs; s4::w /= rhs; s4::x /= rhs; return *this; }
	};
	struct ZYWY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::w, s4::y); } };
	struct ZYWZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::w, s4::z); } };
	struct ZYWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::y, s4::w, s4::w); } };
	struct ZZXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::x, s4::x); } };
	struct ZZXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::x, s4::y); } };
	struct ZZXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::x, s4::z); } };
	struct ZZXW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::x, s4::w); } };
	struct ZZYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::y, s4::x); } };
	struct ZZYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::y, s4::y); } };
	struct ZZYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::y, s4::z); } };
	struct ZZYW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::y, s4::w); } };
	struct ZZZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::z, s4::x); } };
	struct ZZZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::z, s4::y); } };
	struct ZZZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::z, s4::z); } };
	struct ZZZW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::z, s4::w); } };
	struct ZZWX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::w, s4::x); } };
	struct ZZWY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::w, s4::y); } };
	struct ZZWZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::w, s4::z); } };
	struct ZZWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::z, s4::w, s4::w); } };
	struct ZWXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::x, s4::x); } };
	struct ZWXY : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::x, s4::y); }
		const ZWXY& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z = rhs.v.x; s4::w = t0; s4::x = t1; s4::y = t2; return *this; }
		const ZWXY& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z += rhs.v.x; s4::w += t0; s4::x += t1; s4::y += t2; return *this; }
		const ZWXY& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z -= rhs.v.x; s4::w -= t0; s4::x -= t1; s4::y -= t2; return *this; }
		const ZWXY& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z *= rhs.v.x; s4::w *= t0; s4::x *= t1; s4::y *= t2; return *this; }
		const ZWXY& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z /= rhs.v.x; s4::w /= t0; s4::x /= t1; s4::y /= t2; return *this; }
		const ZWXY& operator*=(const TYPE& rhs) { s4::z *= rhs; s4::w *= rhs; s4::x *= rhs; s4::y *= rhs; return *this; }
		const ZWXY& operator/=(const TYPE& rhs) { s4::z /= rhs; s4::w /= rhs; s4::x /= rhs; s4::y /= rhs; return *this; }
	};
	struct ZWXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::x, s4::z); } };
	struct ZWXW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::x, s4::w); } };
	struct ZWYX : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::y, s4::x); }
		const ZWYX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z = rhs.v.x; s4::w = t0; s4::y = t1; s4::x = t2; return *this; }
		const ZWYX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z += rhs.v.x; s4::w += t0; s4::y += t1; s4::x += t2; return *this; }
		const ZWYX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z -= rhs.v.x; s4::w -= t0; s4::y -= t1; s4::x -= t2; return *this; }
		const ZWYX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z *= rhs.v.x; s4::w *= t0; s4::y *= t1; s4::x *= t2; return *this; }
		const ZWYX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::z /= rhs.v.x; s4::w /= t0; s4::y /= t1; s4::x /= t2; return *this; }
		const ZWYX& operator*=(const TYPE& rhs) { s4::z *= rhs; s4::w *= rhs; s4::y *= rhs; s4::x *= rhs; return *this; }
		const ZWYX& operator/=(const TYPE& rhs) { s4::z /= rhs; s4::w /= rhs; s4::y /= rhs; s4::x /= rhs; return *this; }
	};
	struct ZWYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::y, s4::y); } };
	struct ZWYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::y, s4::z); } };
	struct ZWYW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::y, s4::w); } };
	struct ZWZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::z, s4::x); } };
	struct ZWZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::z, s4::y); } };
	struct ZWZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::z, s4::z); } };
	struct ZWZW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::z, s4::w); } };
	struct ZWWX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::w, s4::x); } };
	struct ZWWY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::w, s4::y); } };
	struct ZWWZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::w, s4::z); } };
	struct ZWWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::z, s4::w, s4::w, s4::w); } };
	struct WXXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::x, s4::x); } };
	struct WXXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::x, s4::y); } };
	struct WXXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::x, s4::z); } };
	struct WXXW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::x, s4::w); } };
	struct WXYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::y, s4::x); } };
	struct WXYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::y, s4::y); } };
	struct WXYZ : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::y, s4::z); }
		const WXYZ& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w = rhs.v.x; s4::x = t0; s4::y = t1; s4::z = t2; return *this; }
		const WXYZ& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w += rhs.v.x; s4::x += t0; s4::y += t1; s4::z += t2; return *this; }
		const WXYZ& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w -= rhs.v.x; s4::x -= t0; s4::y -= t1; s4::z -= t2; return *this; }
		const WXYZ& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w *= rhs.v.x; s4::x *= t0; s4::y *= t1; s4::z *= t2; return *this; }
		const WXYZ& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w /= rhs.v.x; s4::x /= t0; s4::y /= t1; s4::z /= t2; return *this; }
		const WXYZ& operator*=(const TYPE& rhs) { s4::w *= rhs; s4::x *= rhs; s4::y *= rhs; s4::z *= rhs; return *this; }
		const WXYZ& operator/=(const TYPE& rhs) { s4::w /= rhs; s4::x /= rhs; s4::y /= rhs; s4::z /= rhs; return *this; }
	};
	struct WXYW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::y, s4::w); } };
	struct WXZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::z, s4::x); } };
	struct WXZY : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::z, s4::y); }
		const WXZY& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.w; s4::w = rhs.v.x; s4::x = t0; s4::z = rhs.v.z; s4::y = t1; return *this; }
		const WXZY& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.w; s4::w += rhs.v.x; s4::x += t0; s4::z += rhs.v.z; s4::y += t1; return *this; }
		const WXZY& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.w; s4::w -= rhs.v.x; s4::x -= t0; s4::z -= rhs.v.z; s4::y -= t1; return *this; }
		const WXZY& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.w; s4::w *= rhs.v.x; s4::x *= t0; s4::z *= rhs.v.z; s4::y *= t1; return *this; }
		const WXZY& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.w; s4::w /= rhs.v.x; s4::x /= t0; s4::z /= rhs.v.z; s4::y /= t1; return *this; }
		const WXZY& operator*=(const TYPE& rhs) { s4::w *= rhs; s4::x *= rhs; s4::z *= rhs; s4::y *= rhs; return *this; }
		const WXZY& operator/=(const TYPE& rhs) { s4::w /= rhs; s4::x /= rhs; s4::z /= rhs; s4::y /= rhs; return *this; }
	};
	struct WXZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::z, s4::z); } };
	struct WXZW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::z, s4::w); } };
	struct WXWX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::w, s4::x); } };
	struct WXWY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::w, s4::y); } };
	struct WXWZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::w, s4::z); } };
	struct WXWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::x, s4::w, s4::w); } };
	struct WYXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::x, s4::x); } };
	struct WYXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::x, s4::y); } };
	struct WYXZ : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::x, s4::z); }
		const WYXZ& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::w = rhs.v.x; s4::y = rhs.v.y; s4::x = t0; s4::z = t1; return *this; }
		const WYXZ& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::w += rhs.v.x; s4::y += rhs.v.y; s4::x += t0; s4::z += t1; return *this; }
		const WYXZ& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::w -= rhs.v.x; s4::y -= rhs.v.y; s4::x -= t0; s4::z -= t1; return *this; }
		const WYXZ& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::w *= rhs.v.x; s4::y *= rhs.v.y; s4::x *= t0; s4::z *= t1; return *this; }
		const WYXZ& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.z; TYPE t1 = rhs.v.w; s4::w /= rhs.v.x; s4::y /= rhs.v.y; s4::x /= t0; s4::z /= t1; return *this; }
		const WYXZ& operator*=(const TYPE& rhs) { s4::w *= rhs; s4::y *= rhs; s4::x *= rhs; s4::z *= rhs; return *this; }
		const WYXZ& operator/=(const TYPE& rhs) { s4::w /= rhs; s4::y /= rhs; s4::x /= rhs; s4::z /= rhs; return *this; }
	};
	struct WYXW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::x, s4::w); } };
	struct WYYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::y, s4::x); } };
	struct WYYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::y, s4::y); } };
	struct WYYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::y, s4::z); } };
	struct WYYW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::y, s4::w); } };
	struct WYZX : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::z, s4::x); }
		const WYZX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::w = rhs.v.x; s4::y = rhs.v.y; s4::z = rhs.v.z; s4::x = t0; return *this; }
		const WYZX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::w += rhs.v.x; s4::y += rhs.v.y; s4::z += rhs.v.z; s4::x += t0; return *this; }
		const WYZX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::w -= rhs.v.x; s4::y -= rhs.v.y; s4::z -= rhs.v.z; s4::x -= t0; return *this; }
		const WYZX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::w *= rhs.v.x; s4::y *= rhs.v.y; s4::z *= rhs.v.z; s4::x *= t0; return *this; }
		const WYZX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.w; s4::w /= rhs.v.x; s4::y /= rhs.v.y; s4::z /= rhs.v.z; s4::x /= t0; return *this; }
		const WYZX& operator*=(const TYPE& rhs) { s4::w *= rhs; s4::y *= rhs; s4::z *= rhs; s4::x *= rhs; return *this; }
		const WYZX& operator/=(const TYPE& rhs) { s4::w /= rhs; s4::y /= rhs; s4::z /= rhs; s4::x /= rhs; return *this; }
	};
	struct WYZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::z, s4::y); } };
	struct WYZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::z, s4::z); } };
	struct WYZW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::z, s4::w); } };
	struct WYWX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::w, s4::x); } };
	struct WYWY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::w, s4::y); } };
	struct WYWZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::w, s4::z); } };
	struct WYWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::y, s4::w, s4::w); } };
	struct WZXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::x, s4::x); } };
	struct WZXY : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::x, s4::y); }
		const WZXY& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w = rhs.v.x; s4::z = t0; s4::x = t1; s4::y = t2; return *this; }
		const WZXY& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w += rhs.v.x; s4::z += t0; s4::x += t1; s4::y += t2; return *this; }
		const WZXY& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w -= rhs.v.x; s4::z -= t0; s4::x -= t1; s4::y -= t2; return *this; }
		const WZXY& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w *= rhs.v.x; s4::z *= t0; s4::x *= t1; s4::y *= t2; return *this; }
		const WZXY& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w /= rhs.v.x; s4::z /= t0; s4::x /= t1; s4::y /= t2; return *this; }
		const WZXY& operator*=(const TYPE& rhs) { s4::w *= rhs; s4::z *= rhs; s4::x *= rhs; s4::y *= rhs; return *this; }
		const WZXY& operator/=(const TYPE& rhs) { s4::w /= rhs; s4::z /= rhs; s4::x /= rhs; s4::y /= rhs; return *this; }
	};
	struct WZXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::x, s4::z); } };
	struct WZXW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::x, s4::w); } };
	struct WZYX : s4
	{
		operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::y, s4::x); }
		const WZYX& operator=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w = rhs.v.x; s4::z = t0; s4::y = t1; s4::x = t2; return *this; }
		const WZYX& operator+=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w += rhs.v.x; s4::z += t0; s4::y += t1; s4::x += t2; return *this; }
		const WZYX& operator-=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w -= rhs.v.x; s4::z -= t0; s4::y -= t1; s4::x -= t2; return *this; }
		const WZYX& operator*=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w *= rhs.v.x; s4::z *= t0; s4::y *= t1; s4::x *= t2; return *this; }
		const WZYX& operator/=(const VECTOR4<TYPE>& rhs) { TYPE t0 = rhs.v.y; TYPE t1 = rhs.v.z; TYPE t2 = rhs.v.w; s4::w /= rhs.v.x; s4::z /= t0; s4::y /= t1; s4::x /= t2; return *this; }
		const WZYX& operator*=(const TYPE& rhs) { s4::w *= rhs; s4::z *= rhs; s4::y *= rhs; s4::x *= rhs; return *this; }
		const WZYX& operator/=(const TYPE& rhs) { s4::w /= rhs; s4::z /= rhs; s4::y /= rhs; s4::x /= rhs; return *this; }
	};
	struct WZYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::y, s4::y); } };
	struct WZYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::y, s4::z); } };
	struct WZYW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::y, s4::w); } };
	struct WZZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::z, s4::x); } };
	struct WZZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::z, s4::y); } };
	struct WZZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::z, s4::z); } };
	struct WZZW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::z, s4::w); } };
	struct WZWX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::w, s4::x); } };
	struct WZWY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::w, s4::y); } };
	struct WZWZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::w, s4::z); } };
	struct WZWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::z, s4::w, s4::w); } };
	struct WWXX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::x, s4::x); } };
	struct WWXY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::x, s4::y); } };
	struct WWXZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::x, s4::z); } };
	struct WWXW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::x, s4::w); } };
	struct WWYX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::y, s4::x); } };
	struct WWYY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::y, s4::y); } };
	struct WWYZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::y, s4::z); } };
	struct WWYW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::y, s4::w); } };
	struct WWZX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::z, s4::x); } };
	struct WWZY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::z, s4::y); } };
	struct WWZZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::z, s4::z); } };
	struct WWZW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::z, s4::w); } };
	struct WWWX : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::w, s4::x); } };
	struct WWWY : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::w, s4::y); } };
	struct WWWZ : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::w, s4::z); } };
	struct WWWW : s4 { operator VECTOR4<TYPE>() const { return VECTOR4<TYPE>(s4::w, s4::w, s4::w, s4::w); } };

	// Length property
	struct LENGTH
	{
	private:
		TYPE x, y, z, w;
		TYPE ApplyLength(const TYPE& curMag, const TYPE& newMag)
		{
			TYPE inverseScaledMagnitude = newMag / curMag;
			x *= inverseScaledMagnitude;
			y *= inverseScaledMagnitude;
			z *= inverseScaledMagnitude;
			return newMag;
		}
	public:
		operator TYPE() const { return sqrt(x * x + y * y + z * z); }
		TYPE operator=(const TYPE& rhs) { return ApplyLength((TYPE)(*this), rhs); }
		TYPE operator+=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag + rhs); }
		TYPE operator-=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag - rhs); }
		TYPE operator*=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag * rhs); }
		TYPE operator/=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag / rhs); }
	};

public:
	VECTOR4() {}
	VECTOR4(const TYPE& x, const TYPE& y, const TYPE& z, const TYPE& w) { v.x = x; v.y = y; v.z = z; v.w = w; }
	VECTOR4(const TYPE& x, const TYPE& y, const VECTOR2<TYPE>& zw) { v.x = x; v.y = y; v.z = zw.x; v.w = zw.y; }
	VECTOR4(const TYPE& x, const VECTOR2<TYPE>& yz, const TYPE& w) { v.x = x; v.y = yz.x; v.z = yz.y; v.w = w; }
	VECTOR4(const VECTOR2<TYPE>& xy, const TYPE& z, const TYPE& w) { v.x = xy.x; v.y = xy.y; v.z = z; v.w = w; }
	VECTOR4(const VECTOR2<TYPE>& xy, const VECTOR2<TYPE>& zw) { v.x = xy.x; v.y = xy.y; v.z = zw.x; v.w = zw.y; }
	VECTOR4(const TYPE& x, const VECTOR3<TYPE>& yzw) { v.x = x; v.y = yzw.x; v.z = yzw.y; v.w = yzw.z; }
	VECTOR4(const VECTOR3<TYPE>& xyz, const TYPE& w) { v.x = xyz.x; v.y = xyz.y; v.z = xyz.z; v.w = w; }

	// Swizzle variables
	X x, r, s;
	Y y, g, t;
	Z z, b, p;
	W w, a, q;
	XX xx, rr, ss;
	XY xy, rg, st;
	XZ xz, rb, sp;
	XW xw, ra, sq;
	YX yx, gr, ts;
	YY yy, gg, tt;
	YZ yz, gb, tp;
	YW yw, ga, tq;
	ZX zx, br, ps;
	ZY zy, bg, pt;
	ZZ zz, bb, pp;
	ZW zw, ba, pq;
	WX wx, ar, qs;
	WY wy, ag, qt;
	WZ wz, ab, qp;
	WW ww, aa, qq;
	XXX xxx, rrr, sss;
	XXY xxy, rrg, sst;
	XXZ xxz, rrb, ssp;
	XXW xxw, rra, ssq;
	XYX xyx, rgr, sts;
	XYY xyy, rgg, stt;
	XYZ xyz, rgb, stp;
	XYW xyw, rga, stq;
	XZX xzx, rbr, sps;
	XZY xzy, rbg, spt;
	XZZ xzz, rbb, spp;
	XZW xzw, rba, spq;
	XWX xwx, rar, sqs;
	XWY xwy, rag, sqt;
	XWZ xwz, rab, sqp;
	XWW xww, raa, sqq;
	YXX yxx, grr, tss;
	YXY yxy, grg, tst;
	YXZ yxz, grb, tsp;
	YXW yxw, gra, tsq;
	YYX yyx, ggr, tts;
	YYY yyy, ggg, ttt;
	YYZ yyz, ggb, ttp;
	YYW yyw, gga, ttq;
	YZX yzx, gbr, tps;
	YZY yzy, gbg, tpt;
	YZZ yzz, gbb, tpp;
	YZW yzw, gba, tpq;
	YWX ywx, gar, tqs;
	YWY ywy, gag, tqt;
	YWZ ywz, gab, tqp;
	YWW yww, gaa, tqq;
	ZXX zxx, brr, pss;
	ZXY zxy, brg, pst;
	ZXZ zxz, brb, psp;
	ZXW zxw, bra, psq;
	ZYX zyx, bgr, pts;
	ZYY zyy, bgg, ptt;
	ZYZ zyz, bgb, ptp;
	ZYW zyw, bga, ptq;
	ZZX zzx, bbr, pps;
	ZZY zzy, bbg, ppt;
	ZZZ zzz, bbb, ppp;
	ZZW zzw, bba, ppq;
	ZWX zwx, bar, pqs;
	ZWY zwy, bag, pqt;
	ZWZ zwz, bab, pqp;
	ZWW zww, baa, pqq;
	WXX wxx, arr, qss;
	WXY wxy, arg, qst;
	WXZ wxz, arb, qsp;
	WXW wxw, ara, qsq;
	WYX wyx, agr, qts;
	WYY wyy, agg, qtt;
	WYZ wyz, agb, qtp;
	WYW wyw, aga, qtq;
	WZX wzx, abr, qps;
	WZY wzy, abg, qpt;
	WZZ wzz, abb, qpp;
	WZW wzw, aba, qpq;
	WWX wwx, aar, qqs;
	WWY wwy, aag, qqt;
	WWZ wwz, aab, qqp;
	WWW www, aaa, qqq;
	XXXX xxxx, rrrr, ssss;
	XXXY xxxy, rrrg, ssst;
	XXXZ xxxz, rrrb, sssp;
	XXXW xxxw, rrra, sssq;
	XXYX xxyx, rrgr, ssts;
	XXYY xxyy, rrgg, sstt;
	XXYZ xxyz, rrgb, sstp;
	XXYW xxyw, rrga, sstq;
	XXZX xxzx, rrbr, ssps;
	XXZY xxzy, rrbg, sspt;
	XXZZ xxzz, rrbb, sspp;
	XXZW xxzw, rrba, sspq;
	XXWX xxwx, rrar, ssqs;
	XXWY xxwy, rrag, ssqt;
	XXWZ xxwz, rrab, ssqp;
	XXWW xxww, rraa, ssqq;
	XYXX xyxx, rgrr, stss;
	XYXY xyxy, rgrg, stst;
	XYXZ xyxz, rgrb, stsp;
	XYXW xyxw, rgra, stsq;
	XYYX xyyx, rggr, stts;
	XYYY xyyy, rggg, sttt;
	XYYZ xyyz, rggb, sttp;
	XYYW xyyw, rgga, sttq;
	XYZX xyzx, rgbr, stps;
	XYZY xyzy, rgbg, stpt;
	XYZZ xyzz, rgbb, stpp;
	XYZW xyzw, rgba, stpq;
	XYWX xywx, rgar, stqs;
	XYWY xywy, rgag, stqt;
	XYWZ xywz, rgab, stqp;
	XYWW xyww, rgaa, stqq;
	XZXX xzxx, rbrr, spss;
	XZXY xzxy, rbrg, spst;
	XZXZ xzxz, rbrb, spsp;
	XZXW xzxw, rbra, spsq;
	XZYX xzyx, rbgr, spts;
	XZYY xzyy, rbgg, sptt;
	XZYZ xzyz, rbgb, sptp;
	XZYW xzyw, rbga, sptq;
	XZZX xzzx, rbbr, spps;
	XZZY xzzy, rbbg, sppt;
	XZZZ xzzz, rbbb, sppp;
	XZZW xzzw, rbba, sppq;
	XZWX xzwx, rbar, spqs;
	XZWY xzwy, rbag, spqt;
	XZWZ xzwz, rbab, spqp;
	XZWW xzww, rbaa, spqq;
	XWXX xwxx, rarr, sqss;
	XWXY xwxy, rarg, sqst;
	XWXZ xwxz, rarb, sqsp;
	XWXW xwxw, rara, sqsq;
	XWYX xwyx, ragr, sqts;
	XWYY xwyy, ragg, sqtt;
	XWYZ xwyz, ragb, sqtp;
	XWYW xwyw, raga, sqtq;
	XWZX xwzx, rabr, sqps;
	XWZY xwzy, rabg, sqpt;
	XWZZ xwzz, rabb, sqpp;
	XWZW xwzw, raba, sqpq;
	XWWX xwwx, raar, sqqs;
	XWWY xwwy, raag, sqqt;
	XWWZ xwwz, raab, sqqp;
	XWWW xwww, raaa, sqqq;
	YXXX yxxx, grrr, tsss;
	YXXY yxxy, grrg, tsst;
	YXXZ yxxz, grrb, tssp;
	YXXW yxxw, grra, tssq;
	YXYX yxyx, grgr, tsts;
	YXYY yxyy, grgg, tstt;
	YXYZ yxyz, grgb, tstp;
	YXYW yxyw, grga, tstq;
	YXZX yxzx, grbr, tsps;
	YXZY yxzy, grbg, tspt;
	YXZZ yxzz, grbb, tspp;
	YXZW yxzw, grba, tspq;
	YXWX yxwx, grar, tsqs;
	YXWY yxwy, grag, tsqt;
	YXWZ yxwz, grab, tsqp;
	YXWW yxww, graa, tsqq;
	YYXX yyxx, ggrr, ttss;
	YYXY yyxy, ggrg, ttst;
	YYXZ yyxz, ggrb, ttsp;
	YYXW yyxw, ggra, ttsq;
	YYYX yyyx, gggr, ttts;
	YYYY yyyy, gggg, tttt;
	YYYZ yyyz, gggb, tttp;
	YYYW yyyw, ggga, tttq;
	YYZX yyzx, ggbr, ttps;
	YYZY yyzy, ggbg, ttpt;
	YYZZ yyzz, ggbb, ttpp;
	YYZW yyzw, ggba, ttpq;
	YYWX yywx, ggar, ttqs;
	YYWY yywy, ggag, ttqt;
	YYWZ yywz, ggab, ttqp;
	YYWW yyww, ggaa, ttqq;
	YZXX yzxx, gbrr, tpss;
	YZXY yzxy, gbrg, tpst;
	YZXZ yzxz, gbrb, tpsp;
	YZXW yzxw, gbra, tpsq;
	YZYX yzyx, gbgr, tpts;
	YZYY yzyy, gbgg, tptt;
	YZYZ yzyz, gbgb, tptp;
	YZYW yzyw, gbga, tptq;
	YZZX yzzx, gbbr, tpps;
	YZZY yzzy, gbbg, tppt;
	YZZZ yzzz, gbbb, tppp;
	YZZW yzzw, gbba, tppq;
	YZWX yzwx, gbar, tpqs;
	YZWY yzwy, gbag, tpqt;
	YZWZ yzwz, gbab, tpqp;
	YZWW yzww, gbaa, tpqq;
	YWXX ywxx, garr, tqss;
	YWXY ywxy, garg, tqst;
	YWXZ ywxz, garb, tqsp;
	YWXW ywxw, gara, tqsq;
	YWYX ywyx, gagr, tqts;
	YWYY ywyy, gagg, tqtt;
	YWYZ ywyz, gagb, tqtp;
	YWYW ywyw, gaga, tqtq;
	YWZX ywzx, gabr, tqps;
	YWZY ywzy, gabg, tqpt;
	YWZZ ywzz, gabb, tqpp;
	YWZW ywzw, gaba, tqpq;
	YWWX ywwx, gaar, tqqs;
	YWWY ywwy, gaag, tqqt;
	YWWZ ywwz, gaab, tqqp;
	YWWW ywww, gaaa, tqqq;
	ZXXX zxxx, brrr, psss;
	ZXXY zxxy, brrg, psst;
	ZXXZ zxxz, brrb, pssp;
	ZXXW zxxw, brra, pssq;
	ZXYX zxyx, brgr, psts;
	ZXYY zxyy, brgg, pstt;
	ZXYZ zxyz, brgb, pstp;
	ZXYW zxyw, brga, pstq;
	ZXZX zxzx, brbr, psps;
	ZXZY zxzy, brbg, pspt;
	ZXZZ zxzz, brbb, pspp;
	ZXZW zxzw, brba, pspq;
	ZXWX zxwx, brar, psqs;
	ZXWY zxwy, brag, psqt;
	ZXWZ zxwz, brab, psqp;
	ZXWW zxww, braa, psqq;
	ZYXX zyxx, bgrr, ptss;
	ZYXY zyxy, bgrg, ptst;
	ZYXZ zyxz, bgrb, ptsp;
	ZYXW zyxw, bgra, ptsq;
	ZYYX zyyx, bggr, ptts;
	ZYYY zyyy, bggg, pttt;
	ZYYZ zyyz, bggb, pttp;
	ZYYW zyyw, bgga, pttq;
	ZYZX zyzx, bgbr, ptps;
	ZYZY zyzy, bgbg, ptpt;
	ZYZZ zyzz, bgbb, ptpp;
	ZYZW zyzw, bgba, ptpq;
	ZYWX zywx, bgar, ptqs;
	ZYWY zywy, bgag, ptqt;
	ZYWZ zywz, bgab, ptqp;
	ZYWW zyww, bgaa, ptqq;
	ZZXX zzxx, bbrr, ppss;
	ZZXY zzxy, bbrg, ppst;
	ZZXZ zzxz, bbrb, ppsp;
	ZZXW zzxw, bbra, ppsq;
	ZZYX zzyx, bbgr, ppts;
	ZZYY zzyy, bbgg, pptt;
	ZZYZ zzyz, bbgb, pptp;
	ZZYW zzyw, bbga, pptq;
	ZZZX zzzx, bbbr, ppps;
	ZZZY zzzy, bbbg, pppt;
	ZZZZ zzzz, bbbb, pppp;
	ZZZW zzzw, bbba, pppq;
	ZZWX zzwx, bbar, ppqs;
	ZZWY zzwy, bbag, ppqt;
	ZZWZ zzwz, bbab, ppqp;
	ZZWW zzww, bbaa, ppqq;
	ZWXX zwxx, barr, pqss;
	ZWXY zwxy, barg, pqst;
	ZWXZ zwxz, barb, pqsp;
	ZWXW zwxw, bara, pqsq;
	ZWYX zwyx, bagr, pqts;
	ZWYY zwyy, bagg, pqtt;
	ZWYZ zwyz, bagb, pqtp;
	ZWYW zwyw, baga, pqtq;
	ZWZX zwzx, babr, pqps;
	ZWZY zwzy, babg, pqpt;
	ZWZZ zwzz, babb, pqpp;
	ZWZW zwzw, baba, pqpq;
	ZWWX zwwx, baar, pqqs;
	ZWWY zwwy, baag, pqqt;
	ZWWZ zwwz, baab, pqqp;
	ZWWW zwww, baaa, pqqq;
	WXXX wxxx, arrr, qsss;
	WXXY wxxy, arrg, qsst;
	WXXZ wxxz, arrb, qssp;
	WXXW wxxw, arra, qssq;
	WXYX wxyx, argr, qsts;
	WXYY wxyy, argg, qstt;
	WXYZ wxyz, argb, qstp;
	WXYW wxyw, arga, qstq;
	WXZX wxzx, arbr, qsps;
	WXZY wxzy, arbg, qspt;
	WXZZ wxzz, arbb, qspp;
	WXZW wxzw, arba, qspq;
	WXWX wxwx, arar, qsqs;
	WXWY wxwy, arag, qsqt;
	WXWZ wxwz, arab, qsqp;
	WXWW wxww, araa, qsqq;
	WYXX wyxx, agrr, qtss;
	WYXY wyxy, agrg, qtst;
	WYXZ wyxz, agrb, qtsp;
	WYXW wyxw, agra, qtsq;
	WYYX wyyx, aggr, qtts;
	WYYY wyyy, aggg, qttt;
	WYYZ wyyz, aggb, qttp;
	WYYW wyyw, agga, qttq;
	WYZX wyzx, agbr, qtps;
	WYZY wyzy, agbg, qtpt;
	WYZZ wyzz, agbb, qtpp;
	WYZW wyzw, agba, qtpq;
	WYWX wywx, agar, qtqs;
	WYWY wywy, agag, qtqt;
	WYWZ wywz, agab, qtqp;
	WYWW wyww, agaa, qtqq;
	WZXX wzxx, abrr, qpss;
	WZXY wzxy, abrg, qpst;
	WZXZ wzxz, abrb, qpsp;
	WZXW wzxw, abra, qpsq;
	WZYX wzyx, abgr, qpts;
	WZYY wzyy, abgg, qptt;
	WZYZ wzyz, abgb, qptp;
	WZYW wzyw, abga, qptq;
	WZZX wzzx, abbr, qpps;
	WZZY wzzy, abbg, qppt;
	WZZZ wzzz, abbb, qppp;
	WZZW wzzw, abba, qppq;
	WZWX wzwx, abar, qpqs;
	WZWY wzwy, abag, qpqt;
	WZWZ wzwz, abab, qpqp;
	WZWW wzww, abaa, qpqq;
	WWXX wwxx, aarr, qqss;
	WWXY wwxy, aarg, qqst;
	WWXZ wwxz, aarb, qqsp;
	WWXW wwxw, aara, qqsq;
	WWYX wwyx, aagr, qqts;
	WWYY wwyy, aagg, qqtt;
	WWYZ wwyz, aagb, qqtp;
	WWYW wwyw, aaga, qqtq;
	WWZX wwzx, aabr, qqps;
	WWZY wwzy, aabg, qqpt;
	WWZZ wwzz, aabb, qqpp;
	WWZW wwzw, aaba, qqpq;
	WWWX wwwx, aaar, qqqs;
	WWWY wwwy, aaag, qqqt;
	WWWZ wwwz, aaab, qqqp;
	WWWW wwww, aaaa, qqqq;

	// Length property
	LENGTH Length;

	// Assignment operators (default for = is fine)
	const VECTOR4& operator+=(const VECTOR4& rhs) { v.x += rhs.v.x; v.y += rhs.v.y; v.z += rhs.v.z; v.w += rhs.v.w; return *this; }
	const VECTOR4& operator-=(const VECTOR4& rhs) { v.x -= rhs.v.x; v.y -= rhs.v.y; v.z -= rhs.v.z; v.w -= rhs.v.w; return *this; }
	const VECTOR4& operator*=(const VECTOR4& rhs) { v.x *= rhs.v.x; v.y *= rhs.v.y; v.z *= rhs.v.z; v.w *= rhs.v.w; return *this; }
	const VECTOR4& operator/=(const VECTOR4& rhs) { v.x /= rhs.v.x; v.y /= rhs.v.y; v.z /= rhs.v.z; v.w /= rhs.v.w; return *this; }
	const VECTOR4& operator*=(const TYPE& rhs) { v.x *= rhs; v.y *= rhs; v.z *= rhs; v.w *= rhs; return *this; }
	const VECTOR4& operator/=(const TYPE& rhs) { v.x /= rhs; v.y /= rhs; v.z /= rhs; v.w /= rhs; return *this; }

	// Array notation access
	const TYPE& operator[](const unsigned& index)
	{
		if (index == 0) { return v.x; }
		else if (index == 1) { return v.y; }
		else if (index == 2) { return v.z; }
		else if (index == 3) { return v.w; }
		else { cout << "Fatal Error: Attempted out of bounds bracket access of 4D vector." << endl << " - Vector: " << *this << endl << " - Index:  " << index << endl; exit(-1); }
	}

	// Overload for cout
	friend ostream& operator<<(ostream& os, const VECTOR4<TYPE>& printVector)
	{
		os << "(" << printVector.v.x << ", " << printVector.v.y << ", " << printVector.v.z << ", " << printVector.v.w << ")";
		return os;
	}

	// Negation
	VECTOR4 operator-() const { return VECTOR4(-v.x, -v.y, -v.z, v.w); }

	void Normalize() { (*this).xyz /= this->Length; }
};

// 4D ToString()
template <typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, string >::type ToString(const SWIZZLE& printSwizzle) { return ToString(typename SWIZZLE::PARENT(printSwizzle)); }
template <typename TYPE> string ToString(const VECTOR4<TYPE>& printVector)
{
	std::ostringstream buffer;
	buffer << "(" << printVector.x << ", " << printVector.y << ", " << printVector.z << ", " << printVector.w << ")";
	return buffer.str();
}

// 4D Negate [-]
template <typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator-(const SWIZZLE& toNegate)
{
	return -(typename SWIZZLE::PARENT(toNegate));
}

// 4D Addition [+]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator+(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) + typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator+(const SWIZZLE& lhs, const VECTOR4<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) + rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator+(const VECTOR4<TYPE>& lhs, const SWIZZLE& rhs) { return lhs + typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR4<TYPE> operator+(const VECTOR4<TYPE>& lhs, const VECTOR4<TYPE>& rhs)
{
	return VECTOR4<TYPE>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

// 4D Subtraction [-]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator-(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) - typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator-(const SWIZZLE& lhs, const VECTOR4<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) - rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator-(const VECTOR4<TYPE>& lhs, const SWIZZLE& rhs) { return lhs - typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR4<TYPE> operator-(const VECTOR4<TYPE>& lhs, const VECTOR4<TYPE>& rhs)
{
	return VECTOR4<TYPE>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

// 4D Multiplication [*]: Component-wise
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator*(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) * typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const SWIZZLE& lhs, const VECTOR4<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) * rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const VECTOR4<TYPE>& lhs, const SWIZZLE& rhs) { return lhs * typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR4<TYPE> operator*(const VECTOR4<TYPE>& lhs, const VECTOR4<TYPE>& rhs)
{
	return VECTOR4<TYPE>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}

// 4D Division [/]: Component-wise
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator/(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) / typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const SWIZZLE& lhs, const VECTOR4<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) / rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const VECTOR4<TYPE>& lhs, const SWIZZLE& rhs) { return lhs / typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR4<TYPE> operator/(const VECTOR4<TYPE>& lhs, const VECTOR4<TYPE>& rhs)
{
	return VECTOR4<TYPE>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}

// 4D Multiplication [*]: Scalar
template <typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const SWIZZLE& lhs, const SCALAR_TYPE& rhs) { return typename SWIZZLE::PARENT(lhs) * rhs; }
template <typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const SCALAR_TYPE& lhs, const SWIZZLE& rhs) { return lhs * typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR4<TYPE> operator*(const VECTOR4<TYPE>& lhs, const SCALAR_TYPE& rhs)
{
	return VECTOR4<TYPE>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}
template <typename TYPE> VECTOR4<TYPE> operator*(const SCALAR_TYPE& lhs, const VECTOR4<TYPE>& rhs)
{
	return VECTOR4<TYPE>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
}

// 4D Division [/]: Scalar
template <typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const SWIZZLE& lhs, const SCALAR_TYPE& rhs) { return typename SWIZZLE::PARENT(lhs) / rhs; }
template <typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const SCALAR_TYPE& lhs, const SWIZZLE& rhs) { return lhs / typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> VECTOR4<TYPE> operator/(const VECTOR4<TYPE>& lhs, const SCALAR_TYPE& rhs)
{
	return VECTOR4<TYPE>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
}
template <typename TYPE> VECTOR4<TYPE> operator/(const SCALAR_TYPE& lhs, const VECTOR4<TYPE>& rhs)
{
	return VECTOR4<TYPE>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w);
}

// 4D Equal To [==]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, bool >::type >::type operator==(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) == typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, bool >::type operator==(const SWIZZLE& lhs, const VECTOR4<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) == rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, bool >::type operator==(const VECTOR4<TYPE>& lhs, const SWIZZLE& rhs) { return lhs == typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator==(const VECTOR4<TYPE>& lhs, const VECTOR4<TYPE>& rhs)
{
	return !(lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w);
}

// 4D Not Equal To [!=]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, bool >::type >::type operator!=(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) != typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, bool >::type operator!=(const SWIZZLE& lhs, const VECTOR4<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) != rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, bool >::type operator!=(const VECTOR4<TYPE>& lhs, const SWIZZLE& rhs) { return lhs != typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator!=(const VECTOR4<TYPE>& lhs, const VECTOR4<TYPE>& rhs)
{
	return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w;
}

// 4D AlmostEqual()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, bool >::type >::type AlmostEqual(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return AlmostEqual((typename SWIZZLE0::PARENT(lhs)), (typename SWIZZLE1::PARENT(rhs))); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, bool >::type AlmostEqual(const SWIZZLE& lhs, const VECTOR4<TYPE>& rhs) { return AlmostEqual((typename SWIZZLE::PARENT(lhs)), rhs); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, bool >::type AlmostEqual(const VECTOR4<TYPE>& lhs, const SWIZZLE& rhs) { return AlmostEqual(lhs, (typename SWIZZLE::PARENT(rhs))); }
template <typename TYPE> bool AlmostEqual(const VECTOR4<TYPE>& lhs, const VECTOR4<TYPE>& rhs)
{
	return fabs(lhs.x - rhs.x) < COMPARISON_EPSILON && fabs(lhs.y - rhs.y) < COMPARISON_EPSILON && fabs(lhs.z - rhs.z) < COMPARISON_EPSILON && fabs(lhs.w - rhs.w) < COMPARISON_EPSILON;
}

// 4D Less Than [<]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, bool >::type >::type operator<(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) < typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, bool >::type operator<(const SWIZZLE& lhs, const VECTOR4<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) < rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, bool >::type operator<(const VECTOR4<TYPE>& lhs, const SWIZZLE& rhs) { return lhs < typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator<(const VECTOR4<TYPE>& lhs, const VECTOR4<TYPE>& rhs)
{
	return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.yzw < rhs.yzw);
}

// 4D Greater Than [>]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, bool >::type >::type operator>(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) > typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, bool >::type operator>(const SWIZZLE& lhs, const VECTOR4<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) > rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, bool >::type operator>(const VECTOR4<TYPE>& lhs, const SWIZZLE& rhs) { return lhs > typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator>(const VECTOR4<TYPE>& lhs, const VECTOR4<TYPE>& rhs)
{
	return lhs.x > rhs.x || (lhs.x == rhs.x && lhs.yzw > rhs.yzw);
}

// 4D Less Than Or Equal To [<=]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, bool >::type >::type operator<=(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) <= typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, bool >::type operator<=(const SWIZZLE& lhs, const VECTOR4<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) <= rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, bool >::type operator<=(const VECTOR4<TYPE>& lhs, const SWIZZLE& rhs) { return lhs <= typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator<=(const VECTOR4<TYPE>& lhs, const VECTOR4<TYPE>& rhs)
{
	return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.yzw <= rhs.yzw);
}

// 4D Greater Than Or Equal To [<=]
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, bool >::type >::type operator>=(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) >= typename SWIZZLE1::PARENT(rhs); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, bool >::type operator>=(const SWIZZLE& lhs, const VECTOR4<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) >= rhs; }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, bool >::type operator>=(const VECTOR4<TYPE>& lhs, const SWIZZLE& rhs) { return lhs >= typename SWIZZLE::PARENT(rhs); }
template <typename TYPE> bool operator>=(const VECTOR4<TYPE>& lhs, const VECTOR4<TYPE>& rhs)
{
	return lhs.x > rhs.x || (lhs.x == rhs.x && lhs.yzw >= rhs.yzw);
}

// 4D Normalize()
template <typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Normalize(const SWIZZLE& toNormalize) { return Normalize(typename SWIZZLE::PARENT(toNormalize)); }
template <typename TYPE> VECTOR4<TYPE> Normalize(const VECTOR4<TYPE>& toNormalize)
{
	return VECTOR4<TYPE>(Normalize(toNormalize.xyz), toNormalize.w);
}

// 4D Dot()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, SCALAR_TYPE >::type >::type Dot(const SWIZZLE0& a, const SWIZZLE1& b) { return Dot(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Dot(const SWIZZLE& a, const VECTOR4<TYPE>& b) { return Dot(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Dot(const VECTOR4<TYPE>& a, const SWIZZLE& b) { return Dot(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> SCALAR_TYPE Dot(const VECTOR4<TYPE>& a, const VECTOR4<TYPE>& b)
{
	return Dot(a.xyz, b.xyz);
}

// 4D Cross()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Cross(const SWIZZLE0& a, const SWIZZLE1& b) { return Cross(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Cross(const SWIZZLE& a, const VECTOR4<TYPE>& b) { return Cross(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Cross(const VECTOR4<TYPE>& a, const SWIZZLE& b) { return Cross(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> VECTOR4<TYPE> Cross(const VECTOR4<TYPE>& a, const VECTOR4<TYPE>& b)
{
	return VECTOR4<TYPE>(Cross(a.xyz, b.xyz), a.w);
}

// 4D Project()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Project(const SWIZZLE0& projectThis, const SWIZZLE1& ontoThis) { return Project(typename SWIZZLE0::PARENT(projectThis), typename SWIZZLE1::PARENT(ontoThis)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Project(const SWIZZLE& projectThis, const VECTOR4<TYPE>& ontoThis) { return Project(typename SWIZZLE::PARENT(projectThis), ontoThis); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Project(const VECTOR4<TYPE>& projectThis, const SWIZZLE& ontoThis) { return Project(projectThis, typename SWIZZLE::PARENT(ontoThis)); }
template <typename TYPE> VECTOR4<TYPE> Project(const VECTOR4<TYPE>& projectThis, const VECTOR4<TYPE>& ontoThis)
{
	return VECTOR4<TYPE>(Project(projectThis.xyz, ontoThis.xyz), ontoThis.w);
}

// 4D Rotate()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Rotate(const SWIZZLE0& toRotate, const SWIZZLE1& rotationAxis, const SCALAR_TYPE& radians) { return Rotate(typename SWIZZLE0::PARENT(toRotate), typename SWIZZLE1::PARENT(rotationAxis), radians); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Rotate(const SWIZZLE& toRotate, const VECTOR4<TYPE>& rotationAxis, const SCALAR_TYPE& radians) { return Rotate(typename SWIZZLE::PARENT(toRotate), rotationAxis, radians); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Rotate(const VECTOR4<TYPE>& toRotate, const SWIZZLE& rotationAxis, const SCALAR_TYPE& radians) { return Rotate(toRotate, typename SWIZZLE::PARENT(rotationAxis), radians); }
template <typename TYPE> VECTOR4<TYPE> Rotate(const VECTOR4<TYPE>& toRotate, const VECTOR4<TYPE>& rotationAxis, const SCALAR_TYPE& radians)
{
	return VECTOR4<TYPE>(Rotate(toRotate.xyz, rotationAxis.xyz, radians), toRotate.w);
}

// 4D Lerp()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Lerp(const SWIZZLE0& start, const SWIZZLE1& end, const SCALAR_TYPE& delta) { return Lerp(typename SWIZZLE0::PARENT(start), typename SWIZZLE1::PARENT(end), delta); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Lerp(const SWIZZLE& start, const VECTOR4<TYPE>& end, const SCALAR_TYPE& delta) { return Lerp(typename SWIZZLE::PARENT(start), end, delta); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Lerp(const VECTOR4<TYPE>& start, const SWIZZLE& end, const SCALAR_TYPE& delta) { return Lerp(start, typename SWIZZLE::PARENT(end), delta); }
template <typename TYPE> VECTOR4<TYPE> Lerp(const VECTOR4<TYPE>& start, const VECTOR4<TYPE>& end, const SCALAR_TYPE& delta)
{
	return VECTOR4<TYPE>(start.x + delta * (end.x - start.x),
	                     start.y + delta * (end.y - start.y),
	                     start.z + delta * (end.z - start.z),
	                     start.w + delta * (end.w - start.w));
}

// 4D Max(): Component-wise
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Max(const SWIZZLE0& a, const SWIZZLE1& b) { return Max(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const SWIZZLE& a, const VECTOR4<TYPE>& b) { return Max(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const VECTOR4<TYPE>& a, const SWIZZLE& b) { return Max(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> VECTOR4<TYPE> Max(const VECTOR4<TYPE>& a, const VECTOR4<TYPE>& b)
{
	return VECTOR4<TYPE>(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w));
}

// 4D Min(): Component-wise
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Min(const SWIZZLE0& a, const SWIZZLE1& b) { return Min(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const SWIZZLE& a, const VECTOR4<TYPE>& b) { return Min(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const VECTOR4<TYPE>& a, const SWIZZLE& b) { return Min(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> VECTOR4<TYPE> Min(const VECTOR4<TYPE>& a, const VECTOR4<TYPE>& b)
{
	return VECTOR4<TYPE>(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w));
}

// 4D Max(): Scalar
template <typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const SWIZZLE& a, const SCALAR_TYPE& maximum) { return Max(typename SWIZZLE::PARENT(a), maximum); }
template <typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const SCALAR_TYPE& maximum, const SWIZZLE& b) { return Max(maximum, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> VECTOR4<TYPE> Max(const VECTOR4<TYPE>& a, const SCALAR_TYPE& maximum)
{
	return VECTOR4<TYPE>(max((SCALAR_TYPE)a.x, maximum), max((SCALAR_TYPE)a.y, maximum), max((SCALAR_TYPE)a.z, maximum), max((SCALAR_TYPE)a.w, maximum));
}
template <typename TYPE> VECTOR4<TYPE> Max(const SCALAR_TYPE& maximum, const VECTOR4<TYPE>& b)
{
	return VECTOR4<TYPE>(max(maximum, (SCALAR_TYPE)b.x), max(maximum, (SCALAR_TYPE)b.y), max(maximum, (SCALAR_TYPE)b.z), max(maximum, (SCALAR_TYPE)b.w));
}

// 4D Min(): Scalar
template <typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const SWIZZLE& a, const SCALAR_TYPE& minimum) { return Min(typename SWIZZLE::PARENT(a), minimum); }
template <typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const SCALAR_TYPE& minimum, const SWIZZLE& b) { return Min(minimum, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> VECTOR4<TYPE> Min(const VECTOR4<TYPE>& a, const SCALAR_TYPE& minimum)
{
	return VECTOR4<TYPE>(min((SCALAR_TYPE)a.x, minimum), min((SCALAR_TYPE)a.y, minimum), min((SCALAR_TYPE)a.z, minimum), min((SCALAR_TYPE)a.w, minimum));
}
template <typename TYPE> VECTOR4<TYPE> Min(const SCALAR_TYPE& minimum, const VECTOR4<TYPE>& b)
{
	return VECTOR4<TYPE>(min(minimum, (SCALAR_TYPE)b.x), min(minimum, (SCALAR_TYPE)b.y), min(minimum, (SCALAR_TYPE)b.z), min(minimum, (SCALAR_TYPE)b.w));
}

// 4D Ceil()
template <typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Ceil(const SWIZZLE& toCeil) { return Ceil(typename SWIZZLE::PARENT(toCeil)); }
template <typename TYPE> VECTOR4<TYPE> Ceil(const VECTOR4<TYPE>& toCeil)
{
	return VECTOR4<TYPE>(ceil(toCeil.x), ceil(toCeil.y), ceil(toCeil.z), ceil(toCeil.w));
}

// 4D Floor()
template <typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Floor(const SWIZZLE& toFloor) { return Floor(typename SWIZZLE::PARENT(toFloor)); }
template <typename TYPE> VECTOR4<TYPE> Floor(const VECTOR4<TYPE>& toFloor)
{
	return VECTOR4<TYPE>(floor(toFloor.x), floor(toFloor.y), floor(toFloor.z), floor(toFloor.w));
}

// 4D Distance()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, SCALAR_TYPE >::type >::type Distance(const SWIZZLE0& a, const SWIZZLE1& b) { return Distance(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Distance(const SWIZZLE& a, const VECTOR4<TYPE>& b) { return Distance(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Distance(const VECTOR4<TYPE>& a, const SWIZZLE& b) { return Distance(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> SCALAR_TYPE Distance(const VECTOR4<TYPE>& a, const VECTOR4<TYPE>& b)
{
	return sqrt(DistanceSquared(a.xyz, b.xyz));
}

// 4D DistanceSquared()
template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, SCALAR_TYPE >::type >::type DistanceSquared(const SWIZZLE0& a, const SWIZZLE1& b) { return DistanceSquared(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }
template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type DistanceSquared(const SWIZZLE& a, const VECTOR4<TYPE>& b) { return DistanceSquared(typename SWIZZLE::PARENT(a), b); }
template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type DistanceSquared(const VECTOR4<TYPE>& a, const SWIZZLE& b) { return DistanceSquared(a, typename SWIZZLE::PARENT(b)); }
template <typename TYPE> SCALAR_TYPE DistanceSquared(const VECTOR4<TYPE>& a, const VECTOR4<TYPE>& b)
{
	return DistanceSquared(a.xyz, b.xyz);
}



//----------------------------------------------------------------------
// 
// Sec. 07 - Swizzle operator<< overloads for cout printing
// 
//----------------------------------------------------------------------

template <typename SWIZZLE> inline
typename EnableIf< Is2D< typename SWIZZLE::PARENT >, ostream >::type&
operator<<(ostream& os, const SWIZZLE& printVector)
{
	os << (typename SWIZZLE::PARENT(printVector));
	return os;
}

template <typename SWIZZLE> inline
typename EnableIf< Is3D< typename SWIZZLE::PARENT >, ostream >::type&
operator<<(ostream& os, const SWIZZLE& printVector)
{
	os << (typename SWIZZLE::PARENT(printVector));
	return os;
}

template <typename SWIZZLE> inline
typename EnableIf< Is4D< typename SWIZZLE::PARENT >, ostream >::type&
operator<<(ostream& os, const SWIZZLE& printVector)
{
	os << (typename SWIZZLE::PARENT(printVector));
	return os;
}



} // SVML namespace

#endif // SVML_H
