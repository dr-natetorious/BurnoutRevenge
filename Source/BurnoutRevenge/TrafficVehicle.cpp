#include "TrafficVehicle.h"
#include "BurnoutCar.h"
#include "BurnoutGameMode.h"
#include "BurnoutHUD.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

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

	ABurnoutCar* Car = Cast<ABurnoutCar>(OtherActor);
	if (!Car)
		return;

	// Scale takedown reward by relative speed
	float CarSpeed = Car->GetVelocity().Size();
	float SpeedBonus = FMath::Clamp(CarSpeed / 1500.f, 0.5f, 2.f);

	Wreck(Car, SpeedBonus);

	Car->AddBoostCharge(Car->BoostChargePerNearMiss * SpeedBonus);

	if (ABurnoutGameMode* GM = Cast<ABurnoutGameMode>(UGameplayStatics::GetGameMode(this)))
		GM->RegisterTakedown();

	// Flash takedown on HUD
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		if (ABurnoutHUD* HUD = Cast<ABurnoutHUD>(PC->GetHUD()))
			HUD->FlashTakedown();
}

void ATrafficVehicle::Wreck(AActor* Attacker, float ForceMultiplier)
{
	bWrecked = true;
	MeshComponent->SetSimulatePhysics(true);

	FVector LaunchDir = Attacker->GetActorForwardVector() + FVector(0.f, 0.f, 0.3f);
	MeshComponent->AddImpulse(LaunchDir.GetSafeNormal() * 800000.f * ForceMultiplier);
	MeshComponent->AddAngularImpulseInDegrees(FVector(
		FMath::RandRange(-200.f, 200.f),
		FMath::RandRange(-200.f, 200.f),
		FMath::RandRange(-400.f, 400.f)) * ForceMultiplier);

	DestroyAfterSeconds = 4.f;
	LifeTimer = 0.f;
}
