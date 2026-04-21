#include "NearMissComponent.h"
#include "BurnoutCar.h"
#include "BurnoutGameMode.h"
#include "TrafficVehicle.h"
#include "Kismet/GameplayStatics.h"

UNearMissComponent::UNearMissComponent()
{
	InitBoxExtent(FVector(300.f, 200.f, 100.f));
	SetCollisionProfileName(TEXT("OverlapAll"));
	SetGenerateOverlapEvents(true);
}

void UNearMissComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UNearMissComponent::OnOverlapBegin);
}

void UNearMissComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<ATrafficVehicle>(OtherActor))
		return;

	ABurnoutCar* Car = Cast<ABurnoutCar>(GetOwner());
	if (!Car)
		return;

	float Now = GetWorld()->GetTimeSeconds();
	if (Now - LastNearMissTime < NearMissCooldown)
		return;

	LastNearMissTime = Now;
	Car->AddBoostCharge(BoostReward);

	if (ABurnoutGameMode* GM = Cast<ABurnoutGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		GM->RegisterNearMiss();
}
