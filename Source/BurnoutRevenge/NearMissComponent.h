#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "NearMissComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BURNOUTREVENGE_API UNearMissComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UNearMissComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NearMiss")
	float BoostReward = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NearMiss")
	float NearMissCooldown = 0.5f;

protected:
	virtual void BeginPlay() override;

private:
	float LastNearMissTime = -999.f;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
