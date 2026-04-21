#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaypointActor.generated.h"

UCLASS()
class BURNOUTREVENGE_API AWaypointActor : public AActor
{
	GENERATED_BODY()

public:
	AWaypointActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
	AWaypointActor* NextWaypoint = nullptr;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere)
	UBillboardComponent* Sprite;
#endif
};
