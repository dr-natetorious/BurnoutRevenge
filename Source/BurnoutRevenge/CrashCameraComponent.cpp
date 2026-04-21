#include "CrashCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

UCrashCameraComponent::UCrashCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCrashCameraComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCrashCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bInCrash)
		return;

	CrashTimer -= DeltaTime;
	if (CrashTimer <= 0.f)
		EndCrash();
}

void UCrashCameraComponent::TriggerCrash(FVector ImpactVelocity)
{
	if (bInCrash || ImpactVelocity.Size() < ImpactThreshold)
		return;

	bInCrash = true;
	CrashTimer = SlowMoDuration;

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowMoTimeDilation);

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		PC->ClientStartCameraShake(nullptr);
}

void UCrashCameraComponent::EndCrash()
{
	bInCrash = false;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}
