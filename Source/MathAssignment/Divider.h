#pragma once

#include "CoreMinimal.h"
#include "IntersectionHelper.h"
#include "RelativeContext.h"
#include "GameFramework/Actor.h"
#include "Divider.generated.h"

UCLASS()
class MATHASSIGNMENT_API ADivider : public AActor
{
	GENERATED_BODY()

public:
	ADivider();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> CornerActorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Line1Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Line1End;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Line2Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Line2End;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector2D> Corners;
	UPROPERTY(BlueprintReadOnly)
	FVector2D IntersectionPoint;	
	FTransform2D Transform;

protected:
	virtual void BeginPlay() override;
	void UpdateCorners();

	void UpdateTransform();
	void UpdatePositions();
	UPROPERTY(BlueprintReadOnly)
	TMap<int, AActor*> CornerActors;

public:
	virtual void Tick(float DeltaTime) override;

	ERelativeContext GetRelativeContextOfActor(AActor* Actor);
};
