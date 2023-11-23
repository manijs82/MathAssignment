#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "IntersectionHelper.generated.h"

UCLASS()
class MATHASSIGNMENT_API UIntersectionHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Intersection")
	static FVector2D LineLineSegmentIntersection(FVector2D l1Start, FVector2D l1End, FVector2D l2Start, FVector2D l2End)
	{
		FVector2D l1Dir = (l1End - l1Start).GetSafeNormal();
		FVector2D l2Dir = (l2End - l2Start).GetSafeNormal();

		float d = DeterminantLine(l1Dir, l2Dir);
		if (FMath::Abs(d) < 0.0001f)
			return FVector2D::Zero();

		FVector2D aToB = l2Start - l1Start;
		return l1Start + l1Dir * (DeterminantLine(aToB, l2Dir) / d);
	}

	UFUNCTION(BlueprintCallable, Category = "Intersection")
	static FVector2D LineLineIntersection(FVector2D l1Start, FVector2D l1Dir, FVector2D l2Start, FVector2D l2Dir)
	{
		float d = DeterminantLine(l1Dir, l2Dir);
		if (FMath::Abs(d) < 0.0001f)
			return FVector2D::Zero();

		FVector2D aToB = l2Start - l1Start;
		return l1Start + l1Dir * (DeterminantLine(aToB, l2Dir) / d);
	}

	UFUNCTION(BlueprintCallable, Category = "Intersection")
	static bool LineCircleIntersection(FVector2D LineStart, FVector2D LineEnd, FVector2D CircleCenter, float CircleRadius)
	{
		bool inside1 = PointCircleIntersection(LineStart, CircleCenter, CircleRadius);
		bool inside2 = PointCircleIntersection(LineEnd, CircleCenter, CircleRadius);
		if (inside1 || inside2) return true;

		float lineLen = FVector2D::Distance(LineStart, LineEnd);

		// get dot product of the line and circle
		float dot = ((CircleCenter.X - LineStart.X) * (LineEnd.X - LineStart.X) + (CircleCenter.Y - LineStart.Y) * (LineEnd.Y - LineStart.Y)) / FMath::Pow(lineLen,2);

		// find the closest point on the line
		float closestX = LineStart.X + dot * (LineEnd.X - LineStart.X);
		float closestY = LineStart.Y + dot * (LineEnd.Y - LineStart.Y);

		// is this point actually on the line segment?
		// if so keep going, but if not, return false
		bool onSegment = PointLineIntersection(LineStart, LineEnd, FVector2D(closestX, closestY));
		if (!onSegment)
			return false;
		
		float distance = FVector2D::Distance(FVector2D(closestX, closestY), CircleCenter);

		if (distance <= CircleRadius)
			return true;
		return false;
	}

	UFUNCTION(BlueprintCallable, Category = "Intersection")
	static bool PointCircleIntersection(FVector2D Point, FVector2D CircleCenter, float CircleRadius)
	{
		if (FVector2D::Distance(CircleCenter, Point) <= CircleRadius)
			return true;
		return false;
	}

	UFUNCTION(BlueprintCallable, Category = "Intersection")
	static bool PointLineIntersection(FVector2D Point, FVector2D LineStart, FVector2D LineEnd)
	{
		float d1 = FVector2D::Distance(Point, LineStart);
		float d2 = FVector2D::Distance(Point, LineEnd);
		float lineLen = FVector2D::Distance(LineStart, LineEnd);

		float buffer = 2;    // higher # = less accurate

		// if the two distances are equal to the line's
		// length, the point is on the line!
		if (d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer)
		{
			return false;
		}
		return true;
	}

	UFUNCTION(BlueprintCallable, Category = "Intersection")
	static float DeterminantLineSegment(FVector2D Start, FVector2D End)
	{
		return Start.X * End.Y - Start.Y * End.X;
	}

	UFUNCTION(BlueprintCallable, Category = "Intersection")
	static float DeterminantLine(FVector2D aDirection, FVector2D bDirection)
	{
		return aDirection.X * bDirection.Y - aDirection.Y * bDirection.X;
	}
};
