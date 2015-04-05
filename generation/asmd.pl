#!/usr/bin/perl -w

require "util.pl";

# Swizzle Addition, Subtraction, Multiplication, Division Component-based Functions

sub Addition
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Addition [+]\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator+(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) + typename SWIZZLE1::PARENT(rhs); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator+(const SWIZZLE& lhs, const VECTOR" . $dimension . "<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) + rhs; }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator+(const VECTOR" . $dimension . "<TYPE>& lhs, const SWIZZLE& rhs) { return lhs + typename SWIZZLE::PARENT(rhs); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> operator+(const VECTOR" . $dimension . "<TYPE>& lhs, const VECTOR" . $dimension . "<TYPE>& rhs)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "lhs." . NumberToSwizzle($d) . " + rhs." . NumberToSwizzle($d);
		}
	
	print ");\n";
	print "}\n\n";
}

sub Subtraction
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Subtraction [-]\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator-(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) - typename SWIZZLE1::PARENT(rhs); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator-(const SWIZZLE& lhs, const VECTOR" . $dimension . "<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) - rhs; }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator-(const VECTOR" . $dimension . "<TYPE>& lhs, const SWIZZLE& rhs) { return lhs - typename SWIZZLE::PARENT(rhs); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> operator-(const VECTOR" . $dimension . "<TYPE>& lhs, const VECTOR" . $dimension . "<TYPE>& rhs)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "lhs." . NumberToSwizzle($d) . " - rhs." . NumberToSwizzle($d);
		}
	
	print ");\n";
	print "}\n\n";
}

sub Multiplication
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Multiplication [*]: Component-wise\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator*(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) * typename SWIZZLE1::PARENT(rhs); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const SWIZZLE& lhs, const VECTOR" . $dimension . "<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) * rhs; }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator*(const VECTOR" . $dimension . "<TYPE>& lhs, const SWIZZLE& rhs) { return lhs * typename SWIZZLE::PARENT(rhs); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> operator*(const VECTOR" . $dimension . "<TYPE>& lhs, const VECTOR" . $dimension . "<TYPE>& rhs)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "lhs." . NumberToSwizzle($d) . " * rhs." . NumberToSwizzle($d);
		}
	
	print ");\n";
	print "}\n\n";
}

sub Division
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Division [/]: Component-wise\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type operator/(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) / typename SWIZZLE1::PARENT(rhs); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const SWIZZLE& lhs, const VECTOR" . $dimension . "<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) / rhs; }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type operator/(const VECTOR" . $dimension . "<TYPE>& lhs, const SWIZZLE& rhs) { return lhs / typename SWIZZLE::PARENT(rhs); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> operator/(const VECTOR" . $dimension . "<TYPE>& lhs, const VECTOR" . $dimension . "<TYPE>& rhs)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "lhs." . NumberToSwizzle($d) . " / rhs." . NumberToSwizzle($d);
		}
	
	print ");\n";
	print "}\n\n";
}

return 1;