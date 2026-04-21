#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "BurnoutCameraShake.generated.h"

UCLASS()
class BURNOUTREVENGE_API UBurnoutCameraShake : public UCameraShakeBase
{
	GENERATED_BODY()

public:
	UBurnoutCameraShake(const FObjectInitializer& ObjectInitializer);
};
