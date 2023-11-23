#pragma once

#include "CoreMinimal.h"
#include "Divider.h"
#include "State.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class MATHASSIGNMENT_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	AObstacle();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ColorChangeSpeed = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InitialSpeed = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InitialAcceleration = -20;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Deceleration = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ActionCooldown = 2;

	UPROPERTY(BlueprintReadOnly)
	ERelativeContext Context;
	UPROPERTY(BlueprintReadOnly)
	EState State;
	UPROPERTY(BlueprintReadOnly)
	FVector2D Direction;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void UpdatePosition(float DeltaTime);

	UPROPERTY(BlueprintReadOnly)
	ADivider* Divider;
	UPROPERTY()
	UMaterialInstanceDynamic* Material;
	
	float Speed;
	float Acceleration;
	float ActionCooldownTimer;
	
public:	
	virtual void Tick(float DeltaTime) override;

	void SetState();
	void MoveColorTowards(FLinearColor target);
};
