#!/usr/bin/perl -w

require "util.pl";

# ToString, Normalize, Dot, Project, Distance, DistanceSquared, Lerp

sub ToString
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D ToString()\n";
	print "template <typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, string >::type ToString(const SWIZZLE& printSwizzle) { return ToString(typename SWIZZLE::PARENT(printSwizzle)); }\n";
	print "template <typename TYPE> string ToString(const VECTOR" . $dimension . "<TYPE>& printVector)\n";
	print "{\n";
	print "\tstd::ostringstream buffer;\n";
	print "\tbuffer << \"(\" << ";
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print " << \", \" << ";
			}
			print "printVector." . NumberToSwizzle($d);
		}
	
	print " << \")\";\n";
	print "\treturn buffer.str();\n";
	print "}\n\n";
}

sub Normalize
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Normalize()\n";
	print "template <typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Normalize(const SWIZZLE& toNormalize) { return Normalize(typename SWIZZLE::PARENT(toNormalize)); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> Normalize(const VECTOR" . $dimension . "<TYPE>& toNormalize)\n";
	print "{\n";
	if ($dimension == 4)
	{
		print "\treturn VECTOR4<TYPE>(Normalize(toNormalize.xyz), toNormalize.w);\n";
	}
	else
	{
		print "\treturn toNormalize / toNormalize.Length;\n";
	}
	print "}\n\n";
}

sub Dot
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Dot()\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, SCALAR_TYPE >::type >::type Dot(const SWIZZLE0& a, const SWIZZLE1& b) { return Dot(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Dot(const SWIZZLE& a, const VECTOR" . $dimension . "<TYPE>& b) { return Dot(typename SWIZZLE::PARENT(a), b); }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Dot(const VECTOR" . $dimension . "<TYPE>& a, const SWIZZLE& b) { return Dot(a, typename SWIZZLE::PARENT(b)); }\n";
	print "template <typename TYPE> SCALAR_TYPE Dot(const VECTOR" . $dimension . "<TYPE>& a, const VECTOR" . $dimension . "<TYPE>& b)\n";
	print "{\n";
	print "\treturn ";
	
	if ($dimension == 4)
	{
		print "Dot(a.xyz, b.xyz)"
	}
	else
	{
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print " + ";
			}
			print "a." . NumberToSwizzle($d) . " * b." . NumberToSwizzle($d);
		}
	
	}
	print ";\n";
	print "}\n\n";
}

sub Projection
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Project()\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Project(const SWIZZLE0& projectThis, const SWIZZLE1& ontoThis) { return Project(typename SWIZZLE0::PARENT(projectThis), typename SWIZZLE1::PARENT(ontoThis)); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Project(const SWIZZLE& projectThis, const VECTOR" . $dimension . "<TYPE>& ontoThis) { return Project(typename SWIZZLE::PARENT(projectThis), ontoThis); }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Project(const VECTOR" . $dimension . "<TYPE>& projectThis, const SWIZZLE& ontoThis) { return Project(projectThis, typename SWIZZLE::PARENT(ontoThis)); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> Project(const VECTOR" . $dimension . "<TYPE>& projectThis, const VECTOR" . $dimension . "<TYPE>& ontoThis)\n";
	print "{\n";
	if ($dimension == 4)
	{
		print "\treturn VECTOR4<TYPE>(Project(projectThis.xyz, ontoThis.xyz), ontoThis.w);\n";
	}
	else
	{
		print "\treturn (Dot(projectThis, ontoThis) / Dot(ontoThis, ontoThis)) * ontoThis;\n";
	}
	print "}\n\n";
}

sub Distance
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Distance()\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, SCALAR_TYPE >::type >::type Distance(const SWIZZLE0& a, const SWIZZLE1& b) { return Distance(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Distance(const SWIZZLE& a, const VECTOR" . $dimension . "<TYPE>& b) { return Distance(typename SWIZZLE::PARENT(a), b); }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type Distance(const VECTOR" . $dimension . "<TYPE>& a, const SWIZZLE& b) { return Distance(a, typename SWIZZLE::PARENT(b)); }\n";
	print "template <typename TYPE> SCALAR_TYPE Distance(const VECTOR" . $dimension . "<TYPE>& a, const VECTOR" . $dimension . "<TYPE>& b)\n";
	print "{\n";
	if ($dimension == 4)
	{
		print "\treturn sqrt(DistanceSquared(a.xyz, b.xyz));\n";
	}
	else
	{
		print "\treturn sqrt(DistanceSquared(a, b));\n";
	}
	print "}\n\n";
}

sub DistanceSquared
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D DistanceSquared()\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, SCALAR_TYPE >::type >::type DistanceSquared(const SWIZZLE0& a, const SWIZZLE1& b) { return DistanceSquared(typename SWIZZLE0::PARENT(a), typename SWIZZLE1::PARENT(b)); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type DistanceSquared(const SWIZZLE& a, const VECTOR" . $dimension . "<TYPE>& b) { return DistanceSquared(typename SWIZZLE::PARENT(a), b); }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, SCALAR_TYPE >::type DistanceSquared(const VECTOR" . $dimension . "<TYPE>& a, const SWIZZLE& b) { return DistanceSquared(a, typename SWIZZLE::PARENT(b)); }\n";
	print "template <typename TYPE> SCALAR_TYPE DistanceSquared(const VECTOR" . $dimension . "<TYPE>& a, const VECTOR" . $dimension . "<TYPE>& b)\n";
	print "{\n";
	print "\treturn ";
	
	if ($dimension == 4)
	{
		print "DistanceSquared(a.xyz, b.xyz)";
	}
	else
	{
	
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				print " + ";
			}
			print "(a." . NumberToSwizzle($d) . " - b." . NumberToSwizzle($d) . ") * (a." . NumberToSwizzle($d) . " - b." . NumberToSwizzle($d) . ")";
		}
	
	}
	print ";\n";
	print "}\n\n";
}

sub Lerp
{
	my($dimension) = @_;
	
	print "// " . $dimension . "D Lerp()\n";
	print "template <typename SWIZZLE0, typename SWIZZLE1> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE0::PARENT >, typename EnableIf< Is" . $dimension . "D< typename SWIZZLE1::PARENT >, typename SWIZZLE0::PARENT >::type >::type Lerp(const SWIZZLE0& start, const SWIZZLE1& end, const SCALAR_TYPE& delta) { return Lerp(typename SWIZZLE0::PARENT(start), typename SWIZZLE1::PARENT(end), delta); }\n";
	print "template <typename SWIZZLE, typename TYPE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Lerp(const SWIZZLE& start, const VECTOR" . $dimension . "<TYPE>& end, const SCALAR_TYPE& delta) { return Lerp(typename SWIZZLE::PARENT(start), end, delta); }\n";
	print "template <typename TYPE, typename SWIZZLE> inline typename EnableIf< Is" . $dimension . "D< typename SWIZZLE::PARENT >, typename SWIZZLE::PARENT >::type Lerp(const VECTOR" . $dimension . "<TYPE>& start, const SWIZZLE& end, const SCALAR_TYPE& delta) { return Lerp(start, typename SWIZZLE::PARENT(end), delta); }\n";
	print "template <typename TYPE> VECTOR" . $dimension . "<TYPE> Lerp(const VECTOR" . $dimension . "<TYPE>& start, const VECTOR" . $dimension . "<TYPE>& end, const SCALAR_TYPE& delta)\n";
	print "{\n";
	print "\treturn VECTOR" . $dimension . "<TYPE>(start.x + delta * (end.x - start.x),\n";
	print "\t                     start.y + delta * (end.y - start.y)";
	if ($dimension > 2)
	{
		print ",\n\t                     start.z + delta * (end.z - start.z)";
	}
	if ($dimension > 3)
	{
		print ",\n\t                     start.w + delta * (end.w - start.w)";
	}
	print ");\n";
	print "}\n\n";
}

return 1;