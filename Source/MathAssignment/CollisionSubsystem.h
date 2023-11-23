#pragma once

#include "CoreMinimal.h"
#include "Obstacle.h"
#include "Subsystems/WorldSubsystem.h"
#include "CollisionSubsystem.generated.h"

UCLASS()
class MATHASSIGNMENT_API UCollisionSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AObstacle*> CollisionDemonstrators;

public:
	virtual void Tick(float DeltaTime) override;

	void RegisterDemonstrator(AObstacle* Demonstrator);
	void UnregisterDemonstrator(AObstacle* Demonstrator);

	UPROPERTY()
	ADivider* Divider;

	virtual TStatId GetStatId() const override;
};
