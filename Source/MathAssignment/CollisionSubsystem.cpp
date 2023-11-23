#include "CollisionSubsystem.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

TStatId UCollisionSubsystem::GetStatId() const
{
	return GetStatID();
}

void UCollisionSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (auto Demonstrator : CollisionDemonstrators)
	{
		for (int i = 0; i < 4; ++i)
		{
			FVector2D center = FVector2D(Demonstrator->GetActorLocation().X, Demonstrator->GetActorLocation().Y);
			float radius = Demonstrator->GetActorScale().X * 50;

			FVector2D lineStart = Divider->Corners[i];
			FVector2D lineEnd;
			if(i == 3)
				lineEnd = Divider->Corners[0];
			else
				lineEnd = Divider->Corners[i + 1];

			FVector2D lineDir = (lineEnd - lineStart).GetSafeNormal();
			FVector lineNormal = FVector(-lineDir.Y, lineDir.X, 0);
			
			if(UIntersectionHelper::LineCircleIntersection(lineStart, lineEnd, center, radius))
			{
				FVector reflectedDir = UKismetMathLibrary::GetReflectionVector(FVector(Demonstrator->Direction, 0), lineNormal);

				float d = UIntersectionHelper::DeterminantLine(Demonstrator->Direction, lineDir);
				if (FMath::Abs(d) < 0.01f)
					Demonstrator->Direction = FVector2D(lineNormal.X, lineNormal.Y);
				else
					Demonstrator->Direction = FVector2D(reflectedDir.X, reflectedDir.Y);
			}
		}
	}
}

void UCollisionSubsystem::RegisterDemonstrator(AObstacle* Demonstrator)
{
	if(!CollisionDemonstrators.Contains(Demonstrator))
		CollisionDemonstrators.Add(Demonstrator);
}

void UCollisionSubsystem::UnregisterDemonstrator(AObstacle* Demonstrator)
{
	if(CollisionDemonstrators.Contains(Demonstrator))
		CollisionDemonstrators.Remove(Demonstrator);
}
