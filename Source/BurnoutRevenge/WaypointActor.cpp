#include "WaypointActor.h"

#if WITH_EDITORONLY_DATA
#include "Components/BillboardComponent.h"
#endif

AWaypointActor::AWaypointActor()
{
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
	Sprite = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	if (Sprite)
		RootComponent = Sprite;
#endif
}
