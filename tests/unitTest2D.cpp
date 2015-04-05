#include <iostream>

#include "svml.h"

using std::cout;
using std::endl;
using std::string;

void PerformTest(string operation, string dimension, string kindOfTest, bool test)
{
	if (test)
	{
		cout << operation << ", " << dimension << ", " << kindOfTest << " - check" << endl;
	}
	else
	{
		cout << "ERROR: " << operation << ", " << dimension << ", " << kindOfTest << endl;
		exit(-1);
	}
}

int main (int argc, char * const argv[])
{
	using SVML::DegToRad;
	using SVML::vec2;
	using SVML::vec3;
	using SVML::vec4;
	
	//////////////////////////////////
	//
	// Unit Tests (2D)
	//
	//////////////////////////////////
	
	vec2 normalized = vec2(4, 5);
	normalized.Normalize();
	
	vec2 lengthTest = vec2(4, 5);
	lengthTest.Length = 1;
	
	PerformTest("ToString()", "2D", "function variations", (ToString(vec2(2, 1)) == "(2, 1)") &&
	                                                       (ToString(vec2(2, 1).xy) == "(2, 1)"));
	
	PerformTest("- (negate)", "2D", "function variations", (-vec2(1, 2) == vec2(-1, -2)) &&
	                                                       (-vec2(1, 2).xy == vec2(-1, -2)));
	
	PerformTest("+", "2D", "function variations", (vec2(1, 2) + vec2(2, 3) == vec2(3, 5)) &&
	                                              (vec2(1, 2) + vec2(2, 3).xy == vec2(3, 5)) &&
	                                              (vec2(1, 2).xy + vec2(2, 3) == vec2(3, 5)) &&
	                                              (vec2(1, 2).xy + vec2(2, 3).xy == vec2(3, 5)));
	
	PerformTest("-", "2D", "function variations", (vec2(2, 2) - vec2(1, 2) == vec2(1, 0)) &&
	                                              (vec2(2, 2) - vec2(1, 2).xy == vec2(1, 0)) &&
	                                              (vec2(2, 2).xy - vec2(1, 2) == vec2(1, 0)) &&
	                                              (vec2(2, 2).xy - vec2(1, 2).xy == vec2(1, 0)));
	
	PerformTest("* (vector)", "2D", "function variations", (vec2(3, 2) * vec2(1, 2) == vec2(3, 4)) &&
	                                                       (vec2(3, 2) * vec2(1, 2).xy == vec2(3, 4)) &&
	                                                       (vec2(3, 2).xy * vec2(1, 2) == vec2(3, 4)) &&
	                                                       (vec2(3, 2).xy * vec2(1, 2).xy == vec2(3, 4)));
	
	PerformTest("/ (vector)", "2D", "function variations", (vec2(6, 2) / vec2(2, 2) == vec2(3, 1)) &&
	                                                       (vec2(6, 2) / vec2(2, 2).xy == vec2(3, 1)) &&
	                                                       (vec2(6, 2).xy / vec2(2, 2) == vec2(3, 1)) &&
	                                                       (vec2(6, 2).xy / vec2(2, 2).xy == vec2(3, 1)));
	
	PerformTest("* (scalar)", "2D", "function variations", (vec2(3, 2) * 2.0f == vec2(6, 4)) &&
	                                                       (vec2(3, 2).xy * 2.0f == vec2(6, 4)) &&
	                                                       (2.0f * vec2(3, 2) == vec2(6, 4)) &&
	                                                       (2.0f * vec2(3, 2).xy == vec2(6, 4)));
	
	PerformTest("/ (scalar)", "2D", "function variations", (vec2(6, 2) / 2.0f == vec2(3, 1)) &&
	                                                       (vec2(6, 2) / 2.0f == vec2(3, 1)) &&
	                                                       (2.0f / vec2(2, 2) == vec2(1, 1)) &&
	                                                       (2.0f / vec2(2, 2).xy == vec2(1, 1)));
	
	PerformTest("AlmostEqual()", "2D", "function functionality", (AlmostEqual(vec2(1, 0), vec2(1, 0.00000001)) == true) &&
	                                                             (AlmostEqual(vec2(1, 0).xy, vec2(1, 0.00000001)) == true) &&
	                                                             (AlmostEqual(vec2(1, 0).xy, vec2(1, 0.0000011)) == false) &&
	                                                             (AlmostEqual(vec2(1, 0), vec2(1, 0.00001)) == false));
	
	PerformTest("==", "2D", "function variations", vec2(1, 2) == vec2(1, 2) &&
	                                               vec2(1, 2) == vec2(1, 2).xy &&
	                                               vec2(1, 2).xy == vec2(1, 2) &&
	                                               vec2(1, 2).xy == vec2(1, 2).xy);
	
	PerformTest("!=", "2D", "function variations", vec2(1, 2) != vec2(2, 1) &&
	                                               vec2(1, 2) != vec2(2, 1).xy &&
	                                               vec2(1, 2).xy != vec2(2, 1) &&
	                                               vec2(1, 2).xy != vec2(2, 1).xy);
	
	PerformTest("<", "2D", "functionality", (vec2(1, 1) < vec2(1, 2)) == true &&
	                                        (vec2(0, 5) < vec2(1, 2)) == true &&
	                                        (vec2(1, 2) < vec2(1, 2)) == false &&
	                                        (vec2(2, 0) < vec2(1, 2)) == false &&
	                                        (vec2(1, 3) < vec2(1, 2)) == false);
	
	PerformTest("<", "2D", "function variations", (vec2(1, 1) < vec2(1, 2)) == true &&
	                                              (vec2(1, 1).xy < vec2(1, 2)) == true &&
	                                              (vec2(1, 1) < vec2(1, 2).xy) == true &&
	                                              (vec2(1, 1).xy < vec2(1, 2).xy) == true);
	
	PerformTest(">", "2D", "functionality", (vec2(1, 2) > vec2(1, 1)) == true &&
	                                        (vec2(1, 2) > vec2(0, 5)) == true &&
	                                        (vec2(1, 2) > vec2(1, 2)) == false &&
	                                        (vec2(1, 2) > vec2(2, 0)) == false &&
	                                        (vec2(1, 2) > vec2(1, 3)) == false);
	
	PerformTest(">", "2D", "function variations", (vec2(1, 2) > vec2(1, 1)) == true &&
	                                              (vec2(1, 2).xy > vec2(1, 1)) == true &&
	                                              (vec2(1, 2) > vec2(1, 1).xy) == true &&
	                                              (vec2(1, 2).xy > vec2(1, 1).xy) == true);
	
	PerformTest("<=", "2D", "functionality", (vec2(1, 1) <= vec2(1, 2)) == true &&
	                                         (vec2(0, 5) <= vec2(1, 2)) == true &&
	                                         (vec2(1, 2) <= vec2(1, 2)) == true &&
	                                         (vec2(2, 0) <= vec2(1, 2)) == false &&
	                                         (vec2(1, 3) <= vec2(1, 2)) == false);
	
	PerformTest("<=", "2D", "function variations", (vec2(1, 1) <= vec2(1, 2)) == true &&
	                                               (vec2(1, 1).xy <= vec2(1, 2)) == true &&
	                                               (vec2(1, 1) <= vec2(1, 2).xy) == true &&
	                                               (vec2(1, 1).xy <= vec2(1, 2).xy) == true);
	
	PerformTest(">=", "2D", "functionality", (vec2(1, 2) >= vec2(1, 1)) == true &&
	                                         (vec2(1, 2) >= vec2(0, 5)) == true &&
	                                         (vec2(1, 2) >= vec2(1, 2)) == true &&
	                                         (vec2(1, 2) >= vec2(2, 0)) == false &&
	                                         (vec2(1, 2) >= vec2(1, 3)) == false);
	
	PerformTest(">=", "2D", "function variations", (vec2(1, 2) >= vec2(1, 1)) == true &&
	                                               (vec2(1, 2).xy >= vec2(1, 1)) == true &&
	                                               (vec2(1, 2) >= vec2(1, 1).xy) == true &&
	                                               (vec2(1, 2).xy >= vec2(1, 1).xy) == true);
	
	PerformTest("Normalize()", "2D", "function variations", (Normalize(vec2(4, 5)) == normalized) &&
	                                                        (Normalize(vec2(4, 5).xy) == normalized));
	
	PerformTest("Dot()", "2D", "function variations", (Dot(vec2(1,0), vec2(0,1)) == 0) &&
	                                                  (Dot(vec2(1,0), vec2(0,1).xy) == 0) &&
	                                                  (Dot(vec2(1,0).xy, vec2(0,1)) == 0) &&
	                                                  (Dot(vec2(1,0).xy, vec2(0,1).xy) == 0));
	
	PerformTest("Perpendicular()", "2D", "function variations", (AlmostEqual(Perpendicular(vec2(1, 0)), vec2(0, 1))));
	
	PerformTest("Project()", "2D", "function variations", (Project(vec2(1, 1), vec2(2, 0))) == vec2(1, 0) &&
	                                                      (Project(vec2(1, 1), vec2(2, 0).xy)) == vec2(1, 0) &&
	                                                      (Project(vec2(1, 1).xy, vec2(2, 0))) == vec2(1, 0) &&
	                                                      (Project(vec2(1, 1).xy, vec2(2, 0).xy)) == vec2(1, 0));
	
	PerformTest("Rotate()", "2D", "function variations", (AlmostEqual(Rotate(vec2(1, 0), DegToRad(90.0f)), vec2(0, 1))) &&
	                                                     (AlmostEqual(Rotate(vec2(1, 0).xy, DegToRad(90.0f)), vec2(0, 1))));
	
	PerformTest("Lerp()", "2D", "function variations", Lerp(vec2(1,0), vec2(0, 1), 0.5f) == vec2(0.5, 0.5) &&
	                                                   Lerp(vec2(1,0), vec2(0, 1).xy, 0.5f) == vec2(0.5, 0.5) &&
	                                                   Lerp(vec2(1,0).xy, vec2(0, 1), 0.5f) == vec2(0.5, 0.5) &&
	                                                   Lerp(vec2(1,0).xy, vec2(0, 1).xy, 0.5f) == vec2(0.5, 0.5));
	
	PerformTest("Max()", "2D", "function variations", (Max(vec2(2,0), vec2(0,1)) == vec2(2, 1)) &&
	                                                  (Max(vec2(2,0), vec2(0,1).xy) == vec2(2, 1)) &&
	                                                  (Max(vec2(2,0).xy, vec2(0,1)) == vec2(2, 1)) &&
	                                                  (Max(vec2(2,0).xy, vec2(0,1).xy) == vec2(2, 1)));
	
	PerformTest("Min()", "2D", "function variations", (Min(vec2(2,0), vec2(0,1)) == vec2(0, 0)) &&
	                                                  (Min(vec2(2,0), vec2(0,1).xy) == vec2(0, 0)) &&
	                                                  (Min(vec2(2,0).xy, vec2(0,1)) == vec2(0, 0)) &&
	                                                  (Min(vec2(2,0).xy, vec2(0,1).xy) == vec2(0, 0)));
	
	PerformTest("Max() (scalar)", "2D", "function variations", (Max(vec2(2,0), 1) == vec2(2, 1)) &&
	                                                           (Max(vec2(2,0).xy, 1) == vec2(2, 1)));
	
	PerformTest("Min() (scalar)", "2D", "function variations", (Min(vec2(2,0), 1) == vec2(1, 0)) &&
	                                                           (Min(vec2(2,0).xy, 1) == vec2(1, 0)));
	
	PerformTest("Ceil()", "2D", "function variations", (Ceil(vec2(2.3, 2)) == vec2(3, 2)) &&
	                                                   (Ceil(vec2(2.3, 2).xy) == vec2(3, 2)));
	
	PerformTest("Floor()", "2D", "function variations", (Floor(vec2(2.6, 2)) == vec2(2, 2)) &&
	                                                    (Floor(vec2(2.6, 2).xy) == vec2(2, 2)));
	
	PerformTest("Distance()", "2D", "function variations", (Distance(vec2(1, 5), vec2(1, 7.5)) == 2.5f) &&
	                                                       (Distance(vec2(1, 5), vec2(1, 7.5).xy) == 2.5f) &&
	                                                       (Distance(vec2(1, 5).xy, vec2(1, 7.5)) == 2.5f) &&
	                                                       (Distance(vec2(1, 5).xy, vec2(1, 7.5).xy) == 2.5f) );
	
	PerformTest("DistanceSquared()", "2D", "function variations", (DistanceSquared(vec2(1, 5), vec2(1, 7.5)) == 6.25f) &&
	                                                              (DistanceSquared(vec2(1, 5), vec2(1, 7.5).xy) == 6.25f) &&
	                                                              (DistanceSquared(vec2(1, 5).xy, vec2(1, 7.5)) == 6.25f) &&
	                                                              (DistanceSquared(vec2(1, 5).xy, vec2(1, 7.5).xy) == 6.25f) );
	
	PerformTest("=", "2D", "function variations", (vec2(1, 2) = vec2(1, 1)) == vec2(1, 1) &&
	                                              (vec2(1, 2).xy = vec2(1, 1)) == vec2(1, 1) &&
	                                              (vec2(1, 2) = vec2(1, 1).xy) == vec2(1, 1) &&
	                                              (vec2(1, 2).xy = vec2(1, 1).xy) == vec2(1, 1));
	
	PerformTest("+=", "2D", "function variations", (vec2(1, 2) += vec2(1, 1)) == vec2(2, 3) &&
	                                               (vec2(1, 2).xy += vec2(1, 1)) == vec2(2, 3) &&
	                                               (vec2(1, 2) += vec2(1, 1).xy) == vec2(2, 3) &&
	                                               (vec2(1, 2).xy += vec2(1, 1).xy) == vec2(2, 3));
	
	PerformTest("-=", "2D", "function variations", (vec2(1, 2) -= vec2(1, 1)) == vec2(0, 1) &&
	                                               (vec2(1, 2).xy -= vec2(1, 1)) == vec2(0, 1) &&
	                                               (vec2(1, 2) -= vec2(1, 1).xy) == vec2(0, 1) &&
	                                               (vec2(1, 2).xy -= vec2(1, 1).xy) == vec2(0, 1));
	
	PerformTest("*=", "2D", "function variations", (vec2(1, 2) *= vec2(2, 2)) == vec2(2, 4) &&
	                                               (vec2(1, 2).xy *= vec2(2, 2)) == vec2(2, 4) &&
	                                               (vec2(1, 2) *= vec2(2, 2).xy) == vec2(2, 4) &&
	                                               (vec2(1, 2).xy *= vec2(2, 2).xy) == vec2(2, 4));
	
	PerformTest("/=", "2D", "function variations", (vec2(4, 2) /= vec2(2, 2)) == vec2(2, 1) &&
	                                               (vec2(4, 2).xy /= vec2(2, 2)) == vec2(2, 1) &&
	                                               (vec2(4, 2) /= vec2(2, 2).xy) == vec2(2, 1) &&
	                                               (vec2(4, 2).xy /= vec2(2, 2).xy) == vec2(2, 1));
	
	PerformTest("*= (float)", "2D", "function variations", (vec2(1, 2) *= 2.0f) == vec2(2, 4) &&
	                                                       (vec2(1, 2).xy *= 2.0f) == vec2(2, 4));
	
	PerformTest("/= (float)", "2D", "function variations", (vec2(4, 2) /= 2.0f) == vec2(2, 1) &&
	                                                       (vec2(4, 2).xy /= 2.0f) == vec2(2, 1));
	
	PerformTest(".Normalize()", "2D", "function functionality", (normalized.Length == 1.0f));
	
	PerformTest(".Length =", "2D", "function functionality", (lengthTest.Length == 1.0f));
	
	lengthTest.Length += 1;
	PerformTest(".Length +=", "2D", "function functionality", (lengthTest.Length == 2.0f));
	
	lengthTest.Length -= 3;
	PerformTest(".Length -=", "2D", "function functionality", (lengthTest.Length == 1.0f));
	
	lengthTest.Length *= -2;
	PerformTest(".Length *=", "2D", "function functionality", (lengthTest.Length == 2.0f));
	
	lengthTest.Length /= 2;
	PerformTest(".Length *=", "2D", "function functionality", (lengthTest.Length == 1.0f));
	
	vec2 two(1, 2);
	vec3 three(3, 4, 5);
	vec4 four(6, 7, 8, 9);
	
	cout << "Vector printing tests:" << endl;
	cout << two << endl;
	cout << three << endl;
	cout << four << endl;
	
	cout << "Swizzle printing tests:" << endl;
	cout << two.x << endl;
	cout << two.xy << endl;
	cout << two.xyy << endl;
	cout << two.xyyy << endl;
	cout << three.x << endl;
	cout << three.xy << endl;
	cout << three.xyz << endl;
	cout << three.xyzz << endl;
	cout << four.x << endl;
	cout << four.xy << endl;
	cout << four.xyz << endl;
	cout << four.xyzw << endl;
	
	return 0;
}
