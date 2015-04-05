#!/usr/bin/perl -w

require "util.pl";

# Cross4d, Rotation4D

sub Cross4D
{
	print "// 4D Cross()\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Cross(const SWIZZLE0& a, const SWIZZLE1& b) { return Cross(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Cross(const SWIZZLE& a, const VECTOR4<TYPE>& b) { return Cross(typename SWIZZLE::PARENT(a), b); }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Cross(const VECTOR4<TYPE>& a, const SWIZZLE& b) { return Cross(a, typename SWIZZLE::PARENT(b)); }\n";
	print "template <typename TYPE> VECTOR4<TYPE> Cross(const VECTOR4<TYPE>& a, const VECTOR4<TYPE>& b)\n";
	print "{\n";
	print "\treturn VECTOR4<TYPE>(Cross(a.xyz, b.xyz), a.w);\n";
	print "}\n\n";
}

sub Rotation4D
{
	print "// 4D Rotate()\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is4D< typename SWIZZLE0::PARENT >, typename EnableIf< Is4D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Rotate(const SWIZZLE0& toRotate, const SWIZZLE1& rotationAxis, const SCALAR_TYPE& radians) { return Rotate(typename SWIZZLE0::PARENT(toRotate), typename SWIZZLE1::PARENT(rotationAxis), radians); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Rotate(const SWIZZLE& toRotate, const VECTOR4<TYPE>& rotationAxis, const SCALAR_TYPE& radians) { return Rotate(typename SWIZZLE::PARENT(toRotate), rotationAxis, radians); }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is4D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Rotate(const VECTOR4<TYPE>& toRotate, const SWIZZLE& rotationAxis, const SCALAR_TYPE& radians) { return Rotate(toRotate, typename SWIZZLE::PARENT(rotationAxis), radians); }\n";
	print "template <typename TYPE> VECTOR4<TYPE> Rotate(const VECTOR4<TYPE>& toRotate, const VECTOR4<TYPE>& rotationAxis, const SCALAR_TYPE& radians)\n";
	print "{\n";
	print "\treturn VECTOR4<TYPE>(Rotate(toRotate.xyz, rotationAxis.xyz, radians), toRotate.w);\n";
	print "}\n\n";
}

return 1;