#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrashCameraComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BURNOUTREVENGE_API UCrashCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCrashCameraComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crash")
	float SlowMoTimeDilation = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crash")
	float SlowMoDuration = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crash")
	float ImpactThreshold = 800.f;

	UFUNCTION(BlueprintCallable, Category = "Crash")
	void TriggerCrash(FVector ImpactVelocity);

	UPROPERTY(BlueprintReadOnly, Category = "Crash")
	bool bInCrash = false;

protected:
	virtual void BeginPlay() override;

private:
	float CrashTimer = 0.f;

	void EndCrash();
};
