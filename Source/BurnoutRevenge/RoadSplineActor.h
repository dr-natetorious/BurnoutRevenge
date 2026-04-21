#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoadSplineActor.generated.h"

class USplineComponent;
class USplineMeshComponent;

UCLASS()
class BURNOUTREVENGE_API ARoadSplineActor : public AActor
{
	GENERATED_BODY()

public:
	ARoadSplineActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Road")
	USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road")
	UStaticMesh* RoadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road")
	UStaticMesh* BarrierMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road")
	float RoadWidth = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road")
	float BarrierOffset = 520.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road")
	float MeshSegmentLength = 1000.f;

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Road")
	void BuildRoad();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Road")
	void ClearRoad();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	TArray<USplineMeshComponent*> SplineMeshes;
};
