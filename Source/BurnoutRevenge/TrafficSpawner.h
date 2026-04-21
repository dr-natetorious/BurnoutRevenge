#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficSpawner.generated.h"

UCLASS()
class BURNOUTREVENGE_API ATrafficSpawner : public AActor
{
	GENERATED_BODY()

public:
	ATrafficSpawner();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic")
	TSubclassOf<AActor> TrafficClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic")
	float SpawnInterval = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic")
	int32 MaxTrafficCount = 12;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic")
	float SpawnAheadDistance = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic")
	float LaneWidth = 400.f;

protected:
	virtual void BeginPlay() override;

private:
	float SpawnTimer = 0.f;

	void SpawnTrafficVehicle(bool bOncoming);
};
