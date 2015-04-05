#!/usr/bin/perl -w

require "util.pl";

# Swizzle Equal To, Not Equal To, AlmostEqual, Less Than, Greater Than, Less Than or Equal To, Greater Than or Equal To


sub EqualTo
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Equal To [==]\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, bool >::type >::type operator==(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) == typename SWIZZLE1::PARENT(rhs); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, bool >::type operator==(const SWIZZLE& lhs, const VECTOR" . $dimension . "<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) == rhs; }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, bool >::type operator==(const VECTOR" . $dimension . "<TYPE>& lhs, const SWIZZLE& rhs) { return lhs == typename SWIZZLE::PARENT(rhs); }\n";
	print "template <typename TYPE> bool operator==(const VECTOR" . $dimension . "<TYPE>& lhs, const VECTOR" . $dimension . "<TYPE>& rhs)\n";
	print "{\n";
	print "\treturn !(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print " || ";
			}
			print "lhs." . NumberToSwizzle($d) . " != rhs." . NumberToSwizzle($d);
		}
	
	print ");\n";
	print "}\n\n";
}

sub NotEqualTo
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Not Equal To [!=]\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, bool >::type >::type operator!=(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) != typename SWIZZLE1::PARENT(rhs); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, bool >::type operator!=(const SWIZZLE& lhs, const VECTOR" . $dimension . "<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) != rhs; }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, bool >::type operator!=(const VECTOR" . $dimension . "<TYPE>& lhs, const SWIZZLE& rhs) { return lhs != typename SWIZZLE::PARENT(rhs); }\n";
	print "template <typename TYPE> bool operator!=(const VECTOR" . $dimension . "<TYPE>& lhs, const VECTOR" . $dimension . "<TYPE>& rhs)\n";
	print "{\n";
	print "\treturn ";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print " || ";
			}
			print "lhs." . NumberToSwizzle($d) . " != rhs." . NumberToSwizzle($d);
		}
	
	print ";\n";
	print "}\n\n";
}

sub AlmostEqual
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D AlmostEqual()\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, bool >::type >::type AlmostEqual(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return AlmostEqual((typename SWIZZLE0::PARENT(lhs)), (typename SWIZZLE1::PARENT(rhs))); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, bool >::type AlmostEqual(const SWIZZLE& lhs, const VECTOR" . $dimension . "<TYPE>& rhs) { return AlmostEqual((typename SWIZZLE::PARENT(lhs)), rhs); }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, bool >::type AlmostEqual(const VECTOR" . $dimension . "<TYPE>& lhs, const SWIZZLE& rhs) { return AlmostEqual(lhs, (typename SWIZZLE::PARENT(rhs))); }\n";
	print "template <typename TYPE> bool AlmostEqual(const VECTOR" . $dimension . "<TYPE>& lhs, const VECTOR" . $dimension . "<TYPE>& rhs)\n";
	print "{\n";
	print "\treturn ";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print " && ";
			}
			print "fabs(lhs." . NumberToSwizzle($d) . " - rhs." . NumberToSwizzle($d) . ") < COMPARISON_EPSILON";
		}
	
	print ";\n";
	print "}\n\n";
}

sub LessThan
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Less Than [<]\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, bool >::type >::type operator<(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) < typename SWIZZLE1::PARENT(rhs); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, bool >::type operator<(const SWIZZLE& lhs, const VECTOR" . $dimension . "<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) < rhs; }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, bool >::type operator<(const VECTOR" . $dimension . "<TYPE>& lhs, const SWIZZLE& rhs) { return lhs < typename SWIZZLE::PARENT(rhs); }\n";
	print "template <typename TYPE> bool operator<(const VECTOR" . $dimension . "<TYPE>& lhs, const VECTOR" . $dimension . "<TYPE>& rhs)\n";
	print "{\n";
	print "\treturn lhs.x < rhs.x || (lhs.x == rhs.x && lhs." . SwizzleRemainder($dimension) . " < rhs." . SwizzleRemainder($dimension) . ");\n";
	print "}\n\n";
}

sub GreaterThan
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Greater Than [>]\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, bool >::type >::type operator>(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) > typename SWIZZLE1::PARENT(rhs); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, bool >::type operator>(const SWIZZLE& lhs, const VECTOR" . $dimension . "<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) > rhs; }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, bool >::type operator>(const VECTOR" . $dimension . "<TYPE>& lhs, const SWIZZLE& rhs) { return lhs > typename SWIZZLE::PARENT(rhs); }\n";
	print "template <typename TYPE> bool operator>(const VECTOR" . $dimension . "<TYPE>& lhs, const VECTOR" . $dimension . "<TYPE>& rhs)\n";
	print "{\n";
	print "\treturn lhs.x > rhs.x || (lhs.x == rhs.x && lhs." . SwizzleRemainder($dimension) . " > rhs." . SwizzleRemainder($dimension) . ");\n";
	print "}\n\n";
}

sub LessThanOrEqual
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Less Than Or Equal To [<=]\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, bool >::type >::type operator<=(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) <= typename SWIZZLE1::PARENT(rhs); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, bool >::type operator<=(const SWIZZLE& lhs, const VECTOR" . $dimension . "<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) <= rhs; }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, bool >::type operator<=(const VECTOR" . $dimension . "<TYPE>& lhs, const SWIZZLE& rhs) { return lhs <= typename SWIZZLE::PARENT(rhs); }\n";
	print "template <typename TYPE> bool operator<=(const VECTOR" . $dimension . "<TYPE>& lhs, const VECTOR" . $dimension . "<TYPE>& rhs)\n";
	print "{\n";
	print "\treturn lhs.x < rhs.x || (lhs.x == rhs.x && lhs." . SwizzleRemainder($dimension) . " <= rhs." . SwizzleRemainder($dimension) . ");\n";
	print "}\n\n";
}

sub GreaterThanOrEqual
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Greater Than Or Equal To [<=]\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, bool >::type >::type operator>=(const SWIZZLE0& lhs, const SWIZZLE1& rhs) { return typename SWIZZLE0::PARENT(lhs) >= typename SWIZZLE1::PARENT(rhs); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, bool >::type operator>=(const SWIZZLE& lhs, const VECTOR" . $dimension . "<TYPE>& rhs) { return typename SWIZZLE::PARENT(lhs) >= rhs; }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, bool >::type operator>=(const VECTOR" . $dimension . "<TYPE>& lhs, const SWIZZLE& rhs) { return lhs >= typename SWIZZLE::PARENT(rhs); }\n";
	print "template <typename TYPE> bool operator>=(const VECTOR" . $dimension . "<TYPE>& lhs, const VECTOR" . $dimension . "<TYPE>& rhs)\n";
	print "{\n";
	print "\treturn lhs.x > rhs.x || (lhs.x == rhs.x && lhs." . SwizzleRemainder($dimension) . " >= rhs." . SwizzleRemainder($dimension) . ");\n";
	print "}\n\n";
}

return 1;