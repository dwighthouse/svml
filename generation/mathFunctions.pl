#!/usr/bin/perl -w

require "util.pl";

# Max, Min, ScalarMax, ScalarMin, Ceil, Floor

sub Max
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Max(): Component-wise\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Max(const SWIZZLE0& a, const SWIZZLE1& b) { return Max(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const SWIZZLE& a, const VECTOR" . $dimension . "<TYPE>& b) { return Max(typename SWIZZLE::PARENT(a), b); }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const VECTOR" . $dimension . "<TYPE>& a, const SWIZZLE& b) { return Max(a, typename SWIZZLE::PARENT(b)); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> Max(const VECTOR" . $dimension . "<TYPE>& a, const VECTOR" . $dimension . "<TYPE>& b)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "max(a." . NumberToSwizzle($d) . ", b." . NumberToSwizzle($d) . ")";
		}
	
	print ");\n";
	print "}\n\n";
}

sub Min
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Min(): Component-wise\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Min(const SWIZZLE0& a, const SWIZZLE1& b) { return Min(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const SWIZZLE& a, const VECTOR" . $dimension . "<TYPE>& b) { return Min(typename SWIZZLE::PARENT(a), b); }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const VECTOR" . $dimension . "<TYPE>& a, const SWIZZLE& b) { return Min(a, typename SWIZZLE::PARENT(b)); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> Min(const VECTOR" . $dimension . "<TYPE>& a, const VECTOR" . $dimension . "<TYPE>& b)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "min(a." . NumberToSwizzle($d) . ", b." . NumberToSwizzle($d) . ")";
		}
	
	print ");\n";
	print "}\n\n";
}

sub ScalarMax
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Max(): Scalar\n";
	print "template <typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const SWIZZLE& a, const SCALAR_TYPE& maximum) { return Max(typename SWIZZLE::PARENT(a), maximum); }\n";
	print "template <typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Max(const SCALAR_TYPE& maximum, const SWIZZLE& b) { return Max(maximum, typename SWIZZLE::PARENT(b)); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> Max(const VECTOR" . $dimension . "<TYPE>& a, const SCALAR_TYPE& maximum)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "max((SCALAR_TYPE)a." . NumberToSwizzle($d) . ", maximum)";
		}
	
	print ");\n";
	print "}\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> Max(const SCALAR_TYPE& maximum, const VECTOR" . $dimension . "<TYPE>& b)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "max(maximum, (SCALAR_TYPE)b." . NumberToSwizzle($d) . ")";
		}
	
	print ");\n";
	print "}\n\n";
}

sub ScalarMin
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Min(): Scalar\n";
	print "template <typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const SWIZZLE& a, const SCALAR_TYPE& minimum) { return Min(typename SWIZZLE::PARENT(a), minimum); }\n";
	print "template <typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Min(const SCALAR_TYPE& minimum, const SWIZZLE& b) { return Min(minimum, typename SWIZZLE::PARENT(b)); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> Min(const VECTOR" . $dimension . "<TYPE>& a, const SCALAR_TYPE& minimum)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "min((SCALAR_TYPE)a." . NumberToSwizzle($d) . ", minimum)";
		}
	
	print ");\n";
	print "}\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> Min(const SCALAR_TYPE& minimum, const VECTOR" . $dimension . "<TYPE>& b)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "min(minimum, (SCALAR_TYPE)b." . NumberToSwizzle($d) . ")";
		}
	
	print ");\n";
	print "}\n\n";
}

sub Ceil
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Ceil()\n";
	print "template <typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Ceil(const SWIZZLE& toCeil) { return Ceil(typename SWIZZLE::PARENT(toCeil)); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> Ceil(const VECTOR" . $dimension . "<TYPE>& toCeil)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "ceil(toCeil." . NumberToSwizzle($d) . ")";
		}
	
	print ");\n";
	print "}\n\n";
}

sub Floor
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Floor()\n";
	print "template <typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Floor(const SWIZZLE& toFloor) { return Floor(typename SWIZZLE::PARENT(toFloor)); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> Floor(const VECTOR" . $dimension . "<TYPE>& toFloor)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print "floor(toFloor." . NumberToSwizzle($d) . ")";
		}
	
	print ");\n";
	print "}\n\n";
}

return 1;