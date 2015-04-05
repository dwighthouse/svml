#!/usr/bin/perl -w

require "util.pl";

# Swizzle Negate, Scalar Multiplication, Scalar Division

sub Negate
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Negate [-]\n";
	print "template <typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator-(const SWIZZLE& toNegate)\n";
	print "{\n";
	print "\treturn -(typename SWIZZLE::PARENT(toNegate));\n";
	print "}\n\n";
}

sub ScalarMultiplication
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Multiplication [*]: Scalar\n";
	print "template <typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const SWIZZLE& lhs, const SCALAR_TYPE& rhs) { return typename SWIZZLE::PARENT(lhs) * rhs; }\n";
	print "template <typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const SCALAR_TYPE& lhs, const SWIZZLE& rhs) { return lhs * typename SWIZZLE::PARENT(rhs); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> operator*(const VECTOR" . $dimension . "<TYPE>& lhs, const SCALAR_TYPE& rhs)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "lhs." . NumberToSwizzle($d) . " * rhs";
		}
	
	print ");\n";
	print "}\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> operator*(const SCALAR_TYPE& lhs, const VECTOR" . $dimension . "<TYPE>& rhs)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "lhs * rhs." . NumberToSwizzle($d);
		}
	
	print ");\n";
	print "}\n\n";
}

sub ScalarDivision
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Division [/]: Scalar\n";
	print "template <typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const SWIZZLE& lhs, const SCALAR_TYPE& rhs) { return typename SWIZZLE::PARENT(lhs) / rhs; }\n";
	print "template <typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const SCALAR_TYPE& lhs, const SWIZZLE& rhs) { return lhs / typename SWIZZLE::PARENT(rhs); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> operator/(const VECTOR" . $dimension . "<TYPE>& lhs, const SCALAR_TYPE& rhs)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "lhs." . NumberToSwizzle($d) . " / rhs";
		}
	
	print ");\n";
	print "}\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> operator/(const SCALAR_TYPE& lhs, const VECTOR" . $dimension . "<TYPE>& rhs)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "lhs / rhs." . NumberToSwizzle($d);
		}
	
	print ");\n";
	print "}\n\n";
}

return 1;