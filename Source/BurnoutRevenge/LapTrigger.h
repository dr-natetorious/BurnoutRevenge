#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LapTrigger.generated.h"

UCLASS()
class BURNOUTREVENGE_API ALapTrigger : public AActor
{
	GENERATED_BODY()

public:
	ALapTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lap")
	int32 LapNumber = 1;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerBox;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
