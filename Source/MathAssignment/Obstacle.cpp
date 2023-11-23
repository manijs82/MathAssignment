#include "Obstacle.h"

#include "CollisionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AObstacle::AObstacle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AObstacle::BeginPlay()
{
	Super::BeginPlay();

	UStaticMeshComponent* mesh = GetComponentByClass<UStaticMeshComponent>();
	UMaterialInterface* mat = mesh->GetMaterial(0);
	Material = UMaterialInstanceDynamic::Create(mat, NULL);
	mesh->SetMaterial(0, Material);

	Divider = Cast<ADivider>(UGameplayStatics::GetActorOfClass(GetWorld(), ADivider::StaticClass()));

	Direction = FMath::RandPointInCircle(1).GetSafeNormal();
	Speed = InitialSpeed;
	Acceleration = InitialAcceleration;
	ActionCooldownTimer = ActionCooldown;

	GetWorld()->GetSubsystem<UCollisionSubsystem>()->RegisterDemonstrator(this);
}

void AObstacle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetSubsystem<UCollisionSubsystem>()->UnregisterDemonstrator(this);
	Super::EndPlay(EndPlayReason);
}

void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Context = Divider->GetRelativeContextOfActor(this);

	ActionCooldownTimer -= DeltaTime;
	
	SetState();
	UpdatePosition(DeltaTime);

	if(State == EState::Speeding)
	{
		Acceleration = -InitialAcceleration * 10;
	}
	if(State == EState::Scaling)
	{
		float noise = FMath::PerlinNoise2D(FVector2D(GetActorLocation().X, GetActorLocation().Y));
		noise = FMath::Lerp(0.5f, 3, noise);
		SetActorScale3D(UKismetMathLibrary::VInterpTo(GetActorScale(), FVector::One() * noise, DeltaTime, 1));
	}
	if(State == EState::Spawning && ActionCooldownTimer <= 0)
	{
		AActor* actor = GetWorld()->SpawnActor(GetClass());
		actor->SetActorLocation(GetActorLocation());
	}
	if(State == EState::Destroying && ActionCooldownTimer <= 0)
	{
		Destroy();
	}

	if(ActionCooldownTimer <= 0)
		ActionCooldownTimer = ActionCooldown;
}

void AObstacle::SetState()
{
	switch (Context)
	{
	case ERelativeContext::TopRight:
		State = EState::Spawning;
		MoveColorTowards(FLinearColor(1, 1, 0));
		break;
	case ERelativeContext::TopLeft:
		State = EState::Speeding;
		MoveColorTowards(FLinearColor(1, 0, 0));
		break;
	case ERelativeContext::BottomRight:
		State = EState::Scaling;
		MoveColorTowards(FLinearColor(0, 1, 0));
		break;
	case ERelativeContext::BottomLeft:
		State = EState::Destroying;
		MoveColorTowards(FLinearColor(0, 0, 0));
		break;
	case ERelativeContext::OutOfBounds:
		Destroy();
	default: ;
	}
}

void AObstacle::MoveColorTowards(FLinearColor target)
{
	FLinearColor current;
	Material->GetVectorParameterValue(FName(TEXT("Color")), current);

	FLinearColor next = UKismetMathLibrary::CInterpTo(current, target, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), ColorChangeSpeed);	
	
	Material->SetVectorParameterValue(FName(TEXT("Color")), next);
}

void AObstacle::UpdatePosition(float DeltaTime)
{
	FVector movementVector = FVector(Direction.X * Speed, Direction.Y * Speed, 0) * DeltaTime;
	SetActorLocation(GetActorLocation() + movementVector);

	Speed += Acceleration * DeltaTime;
	Speed = FMath::Max(0, Speed);

	Acceleration -= Deceleration * DeltaTime;
	Acceleration = FMath::Max(InitialAcceleration, Acceleration);
}

