#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Handle.generated.h"

UCLASS()
class MATHASSIGNMENT_API AHandle : public AActor
{
	GENERATED_BODY()

public:
	AHandle();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
