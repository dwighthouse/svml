#!/usr/bin/perl -w

require "util.pl";

# Swizzle sub-structs
sub CreateSwizzleSingularAssignmentOperator_1D
{
	my($swizzle, $operator) = @_;
	
	$outString = "const " . uc($swizzle) . "& operator" . $operator . "(const TYPE& rhs) { s1::" . $swizzle . " " . $operator . " rhs; return *this; }";
	
	return $outString;
}

sub CreateSwizzleSingularAssignmentOperator
{
	my($swizzle, $parentDimension, $operator) = @_;
	
	$swizzleLength = length($swizzle);
	
	$outString = "const " . uc($swizzle) . "& operator" . $operator . "(const TYPE& rhs) { ";
	
	for ($d = 0; $d < $swizzleLength; $d++)
	{
		$outString .= "s" . $swizzleLength . "::" . substr($swizzle, $d, 1) . " " . $operator . " rhs; ";
	}
	
	$outString .= "return *this; }";
	
	return $outString;
}

sub CreateSwizzleComponentAssignmentOperator
{
	my($swizzle, $parentDimension, $operator) = @_;
	
	$swizzleLength = length($swizzle);
	
	$outString = "const " . uc($swizzle) . "& operator" . $operator . "(const VECTOR" . $parentDimension . "<TYPE>& rhs) {";
	
	@right = ();
	
	$unorderedCount = 0;
	for ($s = 0; $s < $swizzleLength; $s++)
	{
		if (NumberToSwizzle($s) ne substr($swizzle, $s, 1))
		{
			if ($unorderedCount == 0)
			{
				$rightSide = "rhs.v." . NumberToSwizzle($s);
				push(@right, $rightSide);
			}
			else
			{
				$outString .= " TYPE t" . ($unorderedCount - 1) . " = rhs.v." . NumberToSwizzle($s) . ";";
				
				$rightSide = "t" . ($unorderedCount - 1);
				push(@right, $rightSide);
			}
			
			$unorderedCount++;
		}
		else
		{
			$rightSide = "rhs.v." . NumberToSwizzle($s);
			push(@right, $rightSide);
		}
	}
	
	for ($d = 0; $d < $swizzleLength; $d++)
	{
		$outString .= " s" . $swizzleLength . "::" . substr($swizzle, $d, 1) . " " . $operator . " " . $right[$d] . ";";
	}
	
	$outString .= " return *this; }";
	
	return $outString;
}

sub HasNoDuplicates
{
	my($swizzle) = @_;
	
	if (length($swizzle) == 1)
	{
		return 1;
	}
	
	@swizzleArray = split(//, $swizzle);
	
	@sortedArray = sort(@swizzleArray);
	
	$prev = $sortedArray[0];
	
	for ($s = 1; $s < @sortedArray; $s++)
	{
		if ($prev eq $sortedArray[$s])
		{
			return 0;
		}
		
		$prev = $sortedArray[$s];
	}
	
	return 1;
}

sub PrintSwizzle
{
	my($swizzle, $parentDimension) = @_;
	
	$dimension = length($swizzle);
	
	# One dimensional swizzle
	if ($dimension == 1)
	{
		print "\tstruct " . uc($swizzle) . " : s" . $dimension . "\n";
		print "\t{\n";
		
		print "\t\toperator TYPE() const { return s1::" . $swizzle . "; }\n";
		print "\t\t" . CreateSwizzleSingularAssignmentOperator_1D($swizzle, "=") . "\n";
		print "\t\t" . CreateSwizzleSingularAssignmentOperator_1D($swizzle, "+=") . "\n";
		print "\t\t" . CreateSwizzleSingularAssignmentOperator_1D($swizzle, "-=") . "\n";
		print "\t\t" . CreateSwizzleSingularAssignmentOperator_1D($swizzle, "*=") . "\n";
		print "\t\t" . CreateSwizzleSingularAssignmentOperator_1D($swizzle, "/=") . "\n";
		
		print "\t};\n";
	}
	elsif (HasNoDuplicates($swizzle))
	{
		print "\tstruct " . uc($swizzle) . " : s" . $dimension . "\n";
		print "\t{\n";
		
		print "\t\toperator VECTOR" . $dimension . "<TYPE>() const { return VECTOR" . $dimension . "<TYPE>(";
			for ($s = 0; $s < $dimension; $s++)
			{
				if ($s > 0)
				{
					print ", ";
				}
				
				print "s" . $dimension . "::" . substr($swizzle, $s, 1);
			}
		print "); }\n";
		
		print "\t\t" . CreateSwizzleComponentAssignmentOperator($swizzle, $parentDimension, "=") . "\n";
		print "\t\t" . CreateSwizzleComponentAssignmentOperator($swizzle, $parentDimension, "+=") . "\n";
		print "\t\t" . CreateSwizzleComponentAssignmentOperator($swizzle, $parentDimension, "-=") . "\n";
		print "\t\t" . CreateSwizzleComponentAssignmentOperator($swizzle, $parentDimension, "*=") . "\n";
		print "\t\t" . CreateSwizzleComponentAssignmentOperator($swizzle, $parentDimension, "/=") . "\n";
		print "\t\t" . CreateSwizzleSingularAssignmentOperator($swizzle, $parentDimension, "*=") . "\n";
		print "\t\t" . CreateSwizzleSingularAssignmentOperator($swizzle, $parentDimension, "/=") . "\n";
		
		print "\t};\n";
	}
	else
	{
		print "\tstruct " . uc($swizzle) . " : s" . $dimension . " { operator VECTOR" . $dimension . "<TYPE>() const { return VECTOR" . $dimension . "<TYPE>(";
			for ($s = 0; $s < $dimension; $s++)
			{
				if ($s > 0)
				{
					print ", ";
				}
				
				print "s" . $dimension . "::" . substr($swizzle, $s, 1);
			}
		print "); } };\n";
	}
}

return 1;