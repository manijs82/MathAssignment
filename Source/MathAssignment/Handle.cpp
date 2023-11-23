// Fill out your copyright notice in the Description page of Project Settings.


#include "Handle.h"


// Sets default values
AHandle::AHandle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHandle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

