#include "LapTrigger.h"
#include "BurnoutCar.h"
#include "BurnoutGameMode.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ALapTrigger::ALapTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(100.f, 1000.f, 200.f));
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = TriggerBox;
}

void ALapTrigger::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ALapTrigger::OnOverlap);
}

void ALapTrigger::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<ABurnoutCar>(OtherActor))
		return;

	if (ABurnoutGameMode* GM = Cast<ABurnoutGameMode>(UGameplayStatics::GetGameMode(this)))
		GM->RegisterLapComplete(LapNumber);
}
