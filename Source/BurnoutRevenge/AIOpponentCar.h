#pragma once

#include "CoreMinimal.h"
#include "BurnoutCar.h"
#include "WaypointActor.h"
#include "AIOpponentCar.generated.h"

UCLASS()
class BURNOUTREVENGE_API AAIOpponentCar : public ABurnoutCar
{
	GENERATED_BODY()

public:
	AAIOpponentCar();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float SteerStrength = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float WaypointAcceptRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	AWaypointActor* StartWaypoint = nullptr;

protected:
	virtual void BeginPlay() override;

private:
	AWaypointActor* CurrentWaypoint = nullptr;

	void SteerTowardTarget(FVector TargetLocation);
};
