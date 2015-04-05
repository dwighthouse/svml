#!/usr/bin/perl -w

require "support.pl";
require "vectorType.pl";
require "swizzles.pl";
require "asmd.pl";
require "comparisons.pl";
require "otherArithmetic.pl";
require "standardVectorFunctions.pl";
require "mathFunctions.pl";
require "2DSpecificFunctions.pl";
require "3DSpecificFunctions.pl";
require "4DSpecificFunctions.pl";


TopData();

for ($z = 2; $z <= 4; $z++)
{
	VectorSectionContent($z, 2);
	
	MakeVectorType($z);
	
	ToString($z);
	Negate($z);
	
	Addition($z);
	Subtraction($z);
	Multiplication($z);
	Division($z);
	
	ScalarMultiplication($z);
	ScalarDivision($z);
	
	EqualTo($z);
	NotEqualTo($z);
	AlmostEqual($z);
	LessThan($z);
	GreaterThan($z);
	LessThanOrEqual($z);
	GreaterThanOrEqual($z);
	
	Normalize($z);
	Dot($z);
	if ($z == 2)
	{
		Perpendicular();
	}
	elsif ($z == 3)
	{
		Cross();
	}
	elsif ($z == 4)
	{
		Cross4D();
	}
	Projection($z);
	if ($z == 2)
	{
		Rotation2D();
	}
	elsif ($z == 3)
	{
		Rotation3D();
	}
	elsif ($z == 4)
	{
		Rotation4D();
	}
	Lerp($z);
	
	Max($z);
	Min($z);
	ScalarMax($z);
	ScalarMin($z);
	Ceil($z);
	Floor($z);
	
	Distance($z);
	DistanceSquared($z);
	
	print "\n";
	print "\n";
}

BottomData();
