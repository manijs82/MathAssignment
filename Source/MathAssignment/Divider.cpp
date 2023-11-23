#include "Divider.h"

#include "CollisionSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ADivider::ADivider()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADivider::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetSubsystem<UCollisionSubsystem>()->Divider = this;
	
	for (int i = 0; i < 4; ++i)
	{
		AActor* actor = GetWorld()->SpawnActor(CornerActorClass);
		CornerActors.Add(i, actor);
		Corners.Add(FVector2D::Zero());
	}
	
}

void ADivider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePositions();
	UpdateTransform();	
	UpdateCorners();
}

void ADivider::UpdatePositions()
{
	Line1Start.X = FMath::Min(-100, Line1Start.X);
	Line1Start.Y = FMath::Clamp(Line1Start.Y, Line2Start.Y, Line2End.Y);
	
	Line1End.X = FMath::Max(100, Line1End.X);
	Line1End.Y = FMath::Clamp(Line1End.Y, Line2Start.Y, Line2End.Y);

	Line2Start.X = FMath::Clamp(Line2Start.X, Line1Start.X, Line1End.X);
	Line2Start.Y = FMath::Min(-100, Line2Start.Y);

	Line2End.X = FMath::Clamp(Line2End.X, Line1Start.X, Line1End.X);
	Line2End.Y = FMath::Max(100, Line2End.Y);
	
	IntersectionPoint = UIntersectionHelper::LineLineSegmentIntersection(Line1Start, Line1End, Line2Start, Line2End);
}

void ADivider::UpdateTransform()
{
	FVector2D Forward = (Line1End - IntersectionPoint);
	FVector2D Right = (Line2End - IntersectionPoint);

	FMatrix2x2 Matrix = FMatrix2x2(Forward.X, Forward.Y, Right.X, Right.Y);
	Transform = FTransform2D(Matrix, IntersectionPoint);
}

void ADivider::UpdateCorners()
{
	float rightOne = (Line2End - IntersectionPoint).Length();
	float upOne = (Line1End - IntersectionPoint).Length();
	float left = -((Line2Start - IntersectionPoint).Length() / rightOne);
	float down = -((Line1Start - IntersectionPoint).Length() / upOne);
	
	Corners[0] = Transform.TransformPoint(FVector2D(down, left));
	Corners[1] = Transform.TransformPoint(FVector2D(down, 1));
	Corners[2] = Transform.TransformPoint(FVector2D(1, 1));
	Corners[3] = Transform.TransformPoint(FVector2D(1, left));
	
	CornerActors[0]->SetActorLocation(FVector(Line1Start, 0));
	CornerActors[1]->SetActorLocation(FVector(Line1End, 0));
	CornerActors[2]->SetActorLocation(FVector(Line2Start, 0));
	CornerActors[3]->SetActorLocation(FVector(Line2End, 0));

	// draw debug lines
	for (int j = 0; j < 4; j++)
	{
		FVector2D lineStart = Corners[j];
		FVector2D lineEnd;
		if(j == 3)
			lineEnd = Corners[0];
		else
			lineEnd = Corners[j + 1];

		FVector lineStart3D(lineStart, 0);
		FVector lineEnd3D(lineEnd, 0);

		UKismetSystemLibrary::DrawDebugLine(GetWorld(), lineStart3D, lineEnd3D, FColor::Black);

		FVector2D lineDir = (lineEnd - lineStart).GetSafeNormal();
		FVector2D inter1 = UIntersectionHelper::LineLineIntersection(lineStart, lineDir, IntersectionPoint, Transform.TransformPoint(FVector2D(1, 0)) - IntersectionPoint);
		FVector2D inter2 = UIntersectionHelper::LineLineIntersection(lineStart, lineDir, IntersectionPoint, Transform.TransformPoint(FVector2D(0, 1)) - IntersectionPoint);
		
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), FVector(IntersectionPoint, 0), FVector(inter1.IsNearlyZero() ? inter2 : inter1, 0), FColor::Black);
	}
}

ERelativeContext ADivider::GetRelativeContextOfActor(AActor* Actor)
{
	FVector ActorLocation = Actor->GetActorLocation();
	FVector2D ActorLocation2D(ActorLocation.X, ActorLocation.Y);
	FVector2D LocalPoint = Transform.Inverse().TransformPoint(ActorLocation2D);

	for (int i = 0; i < 4; i++)
	{
		FVector2D lineStart = Corners[i];
		FVector2D lineEnd;
		if(i == 3)
			lineEnd = Corners[0];
		else
			lineEnd = Corners[i + 1];
		FVector2D lineDir = (lineEnd - lineStart).GetSafeNormal();
		FVector2D dirToActor = (ActorLocation2D - lineStart).GetSafeNormal();

		
		if(lineDir.X * dirToActor.Y - lineDir.Y * dirToActor.X > 0)
		{
			return ERelativeContext::OutOfBounds; 
		}
	}

	if(LocalPoint.X >= 0 && LocalPoint.Y >= 0)
		return ERelativeContext::TopRight;
	if(LocalPoint.X >= 0 && LocalPoint.Y < 0)
		return ERelativeContext::TopLeft;
	if(LocalPoint.X < 0 && LocalPoint.Y >= 0)
		return ERelativeContext::BottomRight;
	if(LocalPoint.X < 0 && LocalPoint.Y < 0)
		return ERelativeContext::BottomLeft;

	return ERelativeContext::OutOfBounds;
}

