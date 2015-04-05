#!/usr/bin/perl -w

require "util.pl";

# Rotation

sub Rotation2D
{
	print "// 2D Rotate()\n";
	print "template <typename SWIZZLE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Rotate(const SWIZZLE& vector, const SCALAR_TYPE& angle) { return Rotate(typename SWIZZLE::PARENT(vector), angle); }\n";
	print "template <typename TYPE> VECTOR2<TYPE> Rotate(const VECTOR2<TYPE>& vector, const SCALAR_TYPE& angle)\n";
	print "{\n";
	print "\tSCALAR_TYPE sa = sin(angle);\n";
	print "\tSCALAR_TYPE ca = cos(angle);\n";
	print "\treturn VECTOR2<TYPE>(vector.x * ca - vector.y * sa, vector.x * sa + vector.y * ca);\n";
	print "}\n\n";
}

sub Perpendicular
{
	print "// 2D Perpendicular()\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is2D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Perpendicular(const SWIZZLE& toPerpendicular) { return Perpendicular(typename SWIZZLE::PARENT(toPerpendicular)); }\n";
	print "template <typename TYPE> VECTOR2<TYPE> Perpendicular(const VECTOR2<TYPE>& toPerpendicular)\n";
	print "{\n";
	print "\treturn vec2(-toPerpendicular.y, toPerpendicular.x);\n";
	print "}\n\n";
}

return 1;