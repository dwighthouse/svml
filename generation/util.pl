#!/usr/bin/perl -w

# Swizzle Utilities

@swizzle2DSet = ( "x", "y", "xx", "xy", "yx", "yy", "xxx", "xxy", "xyx", "xyy", "yxx", "yxy", "yyx", "yyy", "xxxx", "xxxy", "xxyx", "xxyy", "xyxx", "xyxy", "xyyx", "xyyy", "yxxx", "yxxy", "yxyx", "yxyy", "yyxx", "yyxy", "yyyx", "yyyy" );
@swizzle3DSet = ( "x", "y", "z", "xx", "xy", "xz", "yx", "yy", "yz", "zx", "zy", "zz", "xxx", "xxy", "xxz", "xyx", "xyy", "xyz", "xzx", "xzy", "xzz", "yxx", "yxy", "yxz", "yyx", "yyy", "yyz", "yzx", "yzy", "yzz", "zxx", "zxy", "zxz", "zyx", "zyy", "zyz", "zzx", "zzy", "zzz", "xxxx", "xxxy", "xxxz", "xxyx", "xxyy", "xxyz", "xxzx", "xxzy", "xxzz", "xyxx", "xyxy", "xyxz", "xyyx", "xyyy", "xyyz", "xyzx", "xyzy", "xyzz", "xzxx", "xzxy", "xzxz", "xzyx", "xzyy", "xzyz", "xzzx", "xzzy", "xzzz", "yxxx", "yxxy", "yxxz", "yxyx", "yxyy", "yxyz", "yxzx", "yxzy", "yxzz", "yyxx", "yyxy", "yyxz", "yyyx", "yyyy", "yyyz", "yyzx", "yyzy", "yyzz", "yzxx", "yzxy", "yzxz", "yzyx", "yzyy", "yzyz", "yzzx", "yzzy", "yzzz", "zxxx", "zxxy", "zxxz", "zxyx", "zxyy", "zxyz", "zxzx", "zxzy", "zxzz", "zyxx", "zyxy", "zyxz", "zyyx", "zyyy", "zyyz", "zyzx", "zyzy", "zyzz", "zzxx", "zzxy", "zzxz", "zzyx", "zzyy", "zzyz", "zzzx", "zzzy", "zzzz" );
@swizzle4DSet = ( "x", "y", "z", "w", "xx", "xy", "xz", "xw", "yx", "yy", "yz", "yw", "zx", "zy", "zz", "zw", "wx", "wy", "wz", "ww", "xxx", "xxy", "xxz", "xxw", "xyx", "xyy", "xyz", "xyw", "xzx", "xzy", "xzz", "xzw", "xwx", "xwy", "xwz", "xww", "yxx", "yxy", "yxz", "yxw", "yyx", "yyy", "yyz", "yyw", "yzx", "yzy", "yzz", "yzw", "ywx", "ywy", "ywz", "yww", "zxx", "zxy", "zxz", "zxw", "zyx", "zyy", "zyz", "zyw", "zzx", "zzy", "zzz", "zzw", "zwx", "zwy", "zwz", "zww", "wxx", "wxy", "wxz", "wxw", "wyx", "wyy", "wyz", "wyw", "wzx", "wzy", "wzz", "wzw", "wwx", "wwy", "wwz", "www", "xxxx", "xxxy", "xxxz", "xxxw", "xxyx", "xxyy", "xxyz", "xxyw", "xxzx", "xxzy", "xxzz", "xxzw", "xxwx", "xxwy", "xxwz", "xxww", "xyxx", "xyxy", "xyxz", "xyxw", "xyyx", "xyyy", "xyyz", "xyyw", "xyzx", "xyzy", "xyzz", "xyzw", "xywx", "xywy", "xywz", "xyww", "xzxx", "xzxy", "xzxz", "xzxw", "xzyx", "xzyy", "xzyz", "xzyw", "xzzx", "xzzy", "xzzz", "xzzw", "xzwx", "xzwy", "xzwz", "xzww", "xwxx", "xwxy", "xwxz", "xwxw", "xwyx", "xwyy", "xwyz", "xwyw", "xwzx", "xwzy", "xwzz", "xwzw", "xwwx", "xwwy", "xwwz", "xwww", "yxxx", "yxxy", "yxxz", "yxxw", "yxyx", "yxyy", "yxyz", "yxyw", "yxzx", "yxzy", "yxzz", "yxzw", "yxwx", "yxwy", "yxwz", "yxww", "yyxx", "yyxy", "yyxz", "yyxw", "yyyx", "yyyy", "yyyz", "yyyw", "yyzx", "yyzy", "yyzz", "yyzw", "yywx", "yywy", "yywz", "yyww", "yzxx", "yzxy", "yzxz", "yzxw", "yzyx", "yzyy", "yzyz", "yzyw", "yzzx", "yzzy", "yzzz", "yzzw", "yzwx", "yzwy", "yzwz", "yzww", "ywxx", "ywxy", "ywxz", "ywxw", "ywyx", "ywyy", "ywyz", "ywyw", "ywzx", "ywzy", "ywzz", "ywzw", "ywwx", "ywwy", "ywwz", "ywww", "zxxx", "zxxy", "zxxz", "zxxw", "zxyx", "zxyy", "zxyz", "zxyw", "zxzx", "zxzy", "zxzz", "zxzw", "zxwx", "zxwy", "zxwz", "zxww", "zyxx", "zyxy", "zyxz", "zyxw", "zyyx", "zyyy", "zyyz", "zyyw", "zyzx", "zyzy", "zyzz", "zyzw", "zywx", "zywy", "zywz", "zyww", "zzxx", "zzxy", "zzxz", "zzxw", "zzyx", "zzyy", "zzyz", "zzyw", "zzzx", "zzzy", "zzzz", "zzzw", "zzwx", "zzwy", "zzwz", "zzww", "zwxx", "zwxy", "zwxz", "zwxw", "zwyx", "zwyy", "zwyz", "zwyw", "zwzx", "zwzy", "zwzz", "zwzw", "zwwx", "zwwy", "zwwz", "zwww", "wxxx", "wxxy", "wxxz", "wxxw", "wxyx", "wxyy", "wxyz", "wxyw", "wxzx", "wxzy", "wxzz", "wxzw", "wxwx", "wxwy", "wxwz", "wxww", "wyxx", "wyxy", "wyxz", "wyxw", "wyyx", "wyyy", "wyyz", "wyyw", "wyzx", "wyzy", "wyzz", "wyzw", "wywx", "wywy", "wywz", "wyww", "wzxx", "wzxy", "wzxz", "wzxw", "wzyx", "wzyy", "wzyz", "wzyw", "wzzx", "wzzy", "wzzz", "wzzw", "wzwx", "wzwy", "wzwz", "wzww", "wwxx", "wwxy", "wwxz", "wwxw", "wwyx", "wwyy", "wwyz", "wwyw", "wwzx", "wwzy", "wwzz", "wwzw", "wwwx", "wwwy", "wwwz", "wwww" );
@returnTypeSet = ( "TYPE", "VECTOR2<TYPE>", "VECTOR3<TYPE>", "VECTOR4<TYPE" );

sub NumberToSwizzle
{
	my($number) = @_;
	
	@swizzleArray = ("x", "y", "z", "w");
	
	return $swizzleArray[$number];
}

sub SwizzleRemainder
{
	my($dimension) = @_;
	
	return substr("xyzw", 1, $dimension - 1);
}

sub GetSwizzleVariations
{
	my($swizzle) = @_;
	
	$outString = "";
	
	# Colors
	for ($s = 0; $s < length($swizzle); $s++)
	{
		$subSwizzle = substr($swizzle, $s, 1);
		
		if ($subSwizzle eq "x")
		{
			$outString .= "r";
		}
		elsif ($subSwizzle eq "y")
		{
			$outString .= "g";
		}
		elsif ($subSwizzle eq "z")
		{
			$outString .= "b";
		}
		else
		{
			$outString .= "a";
		}
	}
	
	$outString .= ", ";
	
	# Textures
	for ($s = 0; $s < length($swizzle); $s++)
	{
		$subSwizzle = substr($swizzle, $s, 1);
		
		if ($subSwizzle eq "x")
		{
			$outString .= "s";
		}
		elsif ($subSwizzle eq "y")
		{
			$outString .= "t";
		}
		elsif ($subSwizzle eq "z")
		{
			$outString .= "p";
		}
		else
		{
			$outString .= "q";
		}
	}
	
	return $outString;
}

sub CreateInternalData
{
	my($dimension) = @_;
	
	$outString = "struct { TYPE ";
		for ($d = 0; $d < $dimension; $d++)
		{
			if ($d > 0)
			{
				$outString .= ", ";
			}
			$outString .= NumberToSwizzle($d);
		}
	$outString .= "; } v;";
	
	return $outString;
}

return 1;