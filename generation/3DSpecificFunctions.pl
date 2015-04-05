#!/usr/bin/perl -w

require "util.pl";

# Cross, Rotation3D

sub Cross
{
	print "// 3D Cross()\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Cross(const SWIZZLE0& a, const SWIZZLE1& b) { return Cross(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Cross(const SWIZZLE& a, const VECTOR3<TYPE>& b) { return Cross(typename SWIZZLE::PARENT(a), b); }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Cross(const VECTOR3<TYPE>& a, const SWIZZLE& b) { return Cross(a, typename SWIZZLE::PARENT(b)); }\n";
	print "template <typename TYPE> VECTOR3<TYPE> Cross(const VECTOR3<TYPE>& a, const VECTOR3<TYPE>& b)\n";
	print "{\n";
	print "\treturn VECTOR3<TYPE>(a.y * b.z - a.z * b.y,\n";
	print "\t                     a.z * b.x - a.x * b.z,\n";
	print "\t                     a.x * b.y - a.y * b.x);\n";
	print "}\n\n";
}

sub Rotation3D
{
	print "// 3D Rotate()\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is3D< typename SWIZZLE0::PARENT >, typename EnableIf< Is3D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Rotate(const SWIZZLE0& toRotate, const SWIZZLE1& rotationAxis, const SCALAR_TYPE& radians) { return Rotate(typename SWIZZLE0::PARENT(toRotate), typename SWIZZLE1::PARENT(rotationAxis), radians); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Rotate(const SWIZZLE& toRotate, const VECTOR3<TYPE>& rotationAxis, const SCALAR_TYPE& radians) { return Rotate(typename SWIZZLE::PARENT(toRotate), rotationAxis, radians); }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is3D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Rotate(const VECTOR3<TYPE>& toRotate, const SWIZZLE& rotationAxis, const SCALAR_TYPE& radians) { return Rotate(toRotate, typename SWIZZLE::PARENT(rotationAxis), radians); }\n";
	print "template <typename TYPE> VECTOR3<TYPE> Rotate(const VECTOR3<TYPE>& toRotate, const VECTOR3<TYPE>& rotationAxis, const SCALAR_TYPE& radians)\n";
	print "{\n";
	print "\tSCALAR_TYPE cr = cos(radians);\n";
	print "\treturn (toRotate * cr) + (rotationAxis * Dot(toRotate, rotationAxis) * (1 - cr)) + (Cross(rotationAxis, toRotate) * sin(radians));\n";
	print "}\n\n";
}

return 1;