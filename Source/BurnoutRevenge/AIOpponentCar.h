#pragma once

#include "CoreMinimal.h"
#include "BurnoutCar.h"
#include "AIOpponentCar.generated.h"

UCLASS()
class BURNOUTREVENGE_API AAIOpponentCar : public ABurnoutCar
{
	GENERATED_BODY()

public:
	AAIOpponentCar();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float TargetSpeed = 1800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float SteerStrength = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<AActor*> Waypoints;

protected:
	virtual void BeginPlay() override;

private:
	int32 CurrentWaypoint = 0;

	void SteerTowardTarget(FVector TargetLocation, float DeltaTime);
	AActor* GetNextWaypoint() const;
};
