#include "TrafficVehicle.h"
#include "BurnoutCar.h"
#include "BurnoutGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ATrafficVehicle::ATrafficVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->OnComponentHit.AddDynamic(this, &ATrafficVehicle::OnHit);
}

void ATrafficVehicle::BeginPlay()
{
	Super::BeginPlay();
}

void ATrafficVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bWrecked)
		AddActorWorldOffset(GetActorForwardVector() * Speed * DeltaTime, true);

	LifeTimer += DeltaTime;
	if (LifeTimer >= DestroyAfterSeconds)
		Destroy();
}

void ATrafficVehicle::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bWrecked)
		return;

	if (ABurnoutCar* Car = Cast<ABurnoutCar>(OtherActor))
	{
		Wreck(Cast<AActor>(Car));

		// Reward boost and register takedown
		Car->AddBoostCharge(Car->BoostChargePerNearMiss);

		if (ABurnoutGameMode* GM = Cast<ABurnoutGameMode>(UGameplayStatics::GetGameMode(this)))
			GM->RegisterTakedown();
	}
}

void ATrafficVehicle::Wreck(AActor* Attacker)
{
	bWrecked = true;
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->AddImpulse(Attacker->GetActorForwardVector() * 500000.f);
	DestroyAfterSeconds = 4.f;
	LifeTimer = 0.f;
}
