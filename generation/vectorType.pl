#!/usr/bin/perl -w

require "util.pl";

@componentSets = ( "x, y", "x, y, z", "x, y, z, w" );

# Vector types

sub MakeComponentAssignment
{
	my($dimension, $operator) = @_;
	
	$outString = "const VECTOR" . $dimension . "& operator" . $operator . "(const VECTOR" . $dimension . "& rhs) { ";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				$outString .= "; ";
			}
			
			$outString .= "v." . NumberToSwizzle($d) . " " . $operator . " rhs.v." . NumberToSwizzle($d);
		}
	
	$outString .= "; return *this; }";
	
	return $outString;
}

sub MakeScalarAssignment
{
	my($dimension, $operator) = @_;
	
	$outString = "const VECTOR" . $dimension . "& operator" . $operator . "(const TYPE& rhs) { ";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				$outString .= "; ";
			}
			
			$outString .= "v." . NumberToSwizzle($d) . " " . $operator . " rhs";
		}
	
	$outString .= "; return *this; }";
	
	return $outString;
}

sub PrintConstructors
{
	my($dimension) = @_;
	
	if ($dimension == 2)
	{
		print "\tVECTOR2() {}\n";
		print "\tVECTOR2(const TYPE& x, const TYPE& y) { v.x = x; v.y = y; }\n";
	}
	elsif ($dimension == 3)
	{
		print "\tVECTOR3() {}\n";
		print "\tVECTOR3(const TYPE& x, const TYPE& y, const TYPE& z) { v.x = x; v.y = y; v.z = z; }\n";
		print "\tVECTOR3(const VECTOR2<TYPE>& xy, const TYPE& z) { v.x = xy.x; v.y = xy.y; v.z = z; }\n";
		print "\tVECTOR3(const TYPE& x, const VECTOR2<TYPE>& yz) { v.x = x; v.y = yz.x; v.z = yz.y; }\n";
	}
	else
	{
		print "\tVECTOR4() {}\n";
		print "\tVECTOR4(const TYPE& x, const TYPE& y, const TYPE& z, const TYPE& w) { v.x = x; v.y = y; v.z = z; v.w = w; }\n";
		print "\tVECTOR4(const TYPE& x, const TYPE& y, const VECTOR2<TYPE>& zw) { v.x = x; v.y = y; v.z = zw.x; v.w = zw.y; }\n";
		print "\tVECTOR4(const TYPE& x, const VECTOR2<TYPE>& yz, const TYPE& w) { v.x = x; v.y = yz.x; v.z = yz.y; v.w = w; }\n";
		print "\tVECTOR4(const VECTOR2<TYPE>& xy, const TYPE& z, const TYPE& w) { v.x = xy.x; v.y = xy.y; v.z = z; v.w = w; }\n";
		print "\tVECTOR4(const VECTOR2<TYPE>& xy, const VECTOR2<TYPE>& zw) { v.x = xy.x; v.y = xy.y; v.z = zw.x; v.w = zw.y; }\n";
		print "\tVECTOR4(const TYPE& x, const VECTOR3<TYPE>& yzw) { v.x = x; v.y = yzw.x; v.z = yzw.y; v.w = yzw.z; }\n";
		print "\tVECTOR4(const VECTOR3<TYPE>& xyz, const TYPE& w) { v.x = xyz.x; v.y = xyz.y; v.z = xyz.z; v.w = w; }\n";
	}
}

sub MakeVectorType
{
	my($dimension) = @_;
	
	print "template <typename TYPE>\n";
	print "union VECTOR" . $dimension . "\n";
	print "{\n";
	print "private:\n";
	print "\t" . CreateInternalData($dimension) . "\n\n";
	
	print "\tstruct s1 { protected: TYPE " . $componentSets[$dimension - 2] . "; };\n";
	print "\tstruct s2 { protected: TYPE " . $componentSets[$dimension - 2] . "; public: typedef VECTOR2<TYPE> PARENT; };\n";
	print "\tstruct s3 { protected: TYPE " . $componentSets[$dimension - 2] . "; public: typedef VECTOR3<TYPE> PARENT; };\n";
	print "\tstruct s4 { protected: TYPE " . $componentSets[$dimension - 2] . "; public: typedef VECTOR4<TYPE> PARENT; };\n\n";
	
	# Swizzles
	if ($dimension == 2)
	{
		@swizzleSet = @swizzle2DSet;
	}
	elsif ($dimension == 3)
	{
		@swizzleSet = @swizzle3DSet;
	}
	else
	{
		@swizzleSet = @swizzle4DSet;
	}
	
	for ($u = 0; $u < @swizzleSet; $u++)
	{
		PrintSwizzle($swizzleSet[$u], $dimension);
	}
	print "\n";
	
	# Length Property
	print "\t// Length property\n";
	print "\tstruct LENGTH\n";
	print "\t{\n";
	print "\tprivate:\n";
	print "\t\tTYPE ";
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print ", ";
			}
			print NumberToSwizzle($d);
		}
	print ";\n";
	print "\t\tTYPE ApplyLength(const TYPE& curMag, const TYPE& newMag)\n";
	print "\t\t{\n";
	print "\t\t\tTYPE inverseScaledMagnitude = newMag / curMag;\n";
	print "\t\t\tx *= inverseScaledMagnitude;\n";
	print "\t\t\ty *= inverseScaledMagnitude;\n";
	if ($dimension > 2)
	{
		print "\t\t\tz *= inverseScaledMagnitude;\n";
	}
	print "\t\t\treturn newMag;\n";
	print "\t\t}\n";
	
	print "\tpublic:\n";
	print "\t\toperator TYPE() const { return sqrt(";
	
	$useDimension = $dimension;
	if ($dimension == 4)
	{
		$useDimension = 3;
	}
	
		for ($d = 0; $d < $useDimension; $d++)
		{
			if ($d > 0)
			{
				print " + ";
			}
			
			print NumberToSwizzle($d) . " * " . NumberToSwizzle($d);
		}
	
	print "); }\n";
	
	print"\t\tTYPE operator=(const TYPE& rhs) { return ApplyLength((TYPE)(*this), rhs); }\n";
	print"\t\tTYPE operator+=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag + rhs); }\n";
	print"\t\tTYPE operator-=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag - rhs); }\n";
	print"\t\tTYPE operator*=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag * rhs); }\n";
	print"\t\tTYPE operator/=(const TYPE& rhs) { TYPE curMag = (TYPE)(*this); return ApplyLength(curMag, curMag / rhs); }\n";
	
	print "\t};\n\n";
	
	print "public:\n";
	
	# Constructors
	PrintConstructors($dimension);
	print "\n";
	
	# Swizzle variables
	print "\t// Swizzle variables\n";
	for ($u = 0; $u < @swizzleSet; $u++)
	{
		print "\t" . uc($swizzleSet[$u]) . " " . $swizzleSet[$u] . ", " . GetSwizzleVariations($swizzleSet[$u]) . ";\n";
	}
	print "\n";
	
	# Length property variable
	print "\t// Length property\n";
	print "\tLENGTH Length;\n\n";
	
	# Assignment operators
	print "\t// Assignment operators (default for = is fine)\n";
	print "\t" . MakeComponentAssignment($dimension, "+=") . "\n";
	print "\t" . MakeComponentAssignment($dimension, "-=") . "\n";
	print "\t" . MakeComponentAssignment($dimension, "*=") . "\n";
	print "\t" . MakeComponentAssignment($dimension, "/=") . "\n";
	print "\t" . MakeScalarAssignment($dimension, "*=") . "\n";
	print "\t" . MakeScalarAssignment($dimension, "/=") . "\n\n";
	
	# Array access
	print "\t// Array notation access\n";
	print "\tconst TYPE& operator[](const unsigned& index)\n";
	print "\t{\n";
	print "\t\tif (index == 0) { return v.x; }\n";
	print "\t\telse if (index == 1) { return v.y; }\n";
	if ($dimension > 2)
	{
		print "\t\telse if (index == 2) { return v.z; }\n";
	}
	if ($dimension > 3)
	{
		print "\t\telse if (index == 3) { return v.w; }\n";
	}
	print "\t\telse { cout << \"Fatal Error: Attempted out of bounds bracket access of " . $dimension . "D vector.\" << endl << \" - Vector: \" << *this << endl << \" - Index:  \" << index << endl; exit(-1); }\n";
	print "\t}\n\n";
	
	# Cout
	print "\t// Overload for cout\n";
	print "\tfriend ostream& operator<<(ostream& os, const VECTOR" . $dimension;
	print "<TYPE>& printVector)\n";
	print "\t{\n";
	print "\t\tos << ";
	print "\"(\" << ";
	
		for ($i = 0; $i < $dimension; $i++)
		{
			if ($i > 0)
			{
				print " << \", \" << ";
			}
			
			print "printVector.v." . NumberToSwizzle($i);
		}
		
	print " << \")\";\n";
	print "\t\treturn os;\n";
	print "\t}\n\n";
	
	# Negation/Normalization
	print "\t// Negation\n";
	print "\tVECTOR" . $dimension . " operator-() const { return VECTOR" . $dimension . "(-v.x, -v.y";
	if ($dimension > 2)
	{
		print ", -v.z";
	}
	if ($dimension > 3)
	{
		print ", v.w";
	}
	print "); }\n\n";
	
	if ($dimension == 4)
	{
		print "\tvoid Normalize() { (*this).xyz /= this->Length; }\n";
	}
	else
	{
		print "\tvoid Normalize() { *this /= this->Length; }\n";
	}
	
	print "};\n\n";
}

return 1;