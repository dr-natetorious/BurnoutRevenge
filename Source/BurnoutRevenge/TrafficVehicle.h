#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficVehicle.generated.h"

UCLASS()
class BURNOUTREVENGE_API ATrafficVehicle : public AActor
{
	GENERATED_BODY()

public:
	ATrafficVehicle();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic")
	float Speed = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic")
	float DestroyAfterSeconds = 10.f;

	UPROPERTY(BlueprintReadOnly, Category = "Traffic")
	bool bWrecked = false;

	UFUNCTION(BlueprintCallable, Category = "Traffic")
	void Wreck(AActor* Attacker);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

private:
	float LifeTimer = 0.f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
