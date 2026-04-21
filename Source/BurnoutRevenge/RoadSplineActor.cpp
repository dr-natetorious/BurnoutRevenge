#include "RoadSplineActor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

ARoadSplineActor::ARoadSplineActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	RootComponent = Spline;
	Spline->SetClosedLoop(true);
}

void ARoadSplineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	BuildRoad();
}

void ARoadSplineActor::ClearRoad()
{
	for (USplineMeshComponent* Mesh : SplineMeshes)
	{
		if (Mesh)
			Mesh->DestroyComponent();
	}
	SplineMeshes.Empty();
}

void ARoadSplineActor::BuildRoad()
{
	ClearRoad();

	if (!RoadMesh)
		return;

	int32 NumPoints = Spline->GetNumberOfSplinePoints();
	if (NumPoints < 2)
		return;

	float SplineLength = Spline->GetSplineLength();
	int32 NumSegments = FMath::CeilToInt(SplineLength / MeshSegmentLength);

	for (int32 i = 0; i < NumSegments; i++)
	{
		float StartDist = (float)i / NumSegments * SplineLength;
		float EndDist = (float)(i + 1) / NumSegments * SplineLength;

		FVector StartPos, StartTangent, EndPos, EndTangent;
		StartPos = Spline->GetLocationAtDistanceAlongSpline(StartDist, ESplineCoordinateSpace::Local);
		StartTangent = Spline->GetTangentAtDistanceAlongSpline(StartDist, ESplineCoordinateSpace::Local).GetClampedToMaxSize(MeshSegmentLength);
		EndPos = Spline->GetLocationAtDistanceAlongSpline(EndDist, ESplineCoordinateSpace::Local);
		EndTangent = Spline->GetTangentAtDistanceAlongSpline(EndDist, ESplineCoordinateSpace::Local).GetClampedToMaxSize(MeshSegmentLength);

		// Road surface
		USplineMeshComponent* RoadSeg = NewObject<USplineMeshComponent>(this);
		RoadSeg->SetStaticMesh(RoadMesh);
		RoadSeg->SetMobility(EComponentMobility::Static);
		RoadSeg->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
		RoadSeg->SetStartScale(FVector2D(RoadWidth / 100.f, 1.f));
		RoadSeg->SetEndScale(FVector2D(RoadWidth / 100.f, 1.f));
		RoadSeg->RegisterComponent();
		RoadSeg->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMeshes.Add(RoadSeg);

		// Left barrier
		if (BarrierMesh)
		{
			for (int32 Side = -1; Side <= 1; Side += 2)
			{
				FVector Offset = FVector(0.f, Side * BarrierOffset, 0.f);
				USplineMeshComponent* Barrier = NewObject<USplineMeshComponent>(this);
				Barrier->SetStaticMesh(BarrierMesh);
				Barrier->SetMobility(EComponentMobility::Static);
				Barrier->SetStartAndEnd(StartPos + Offset, StartTangent, EndPos + Offset, EndTangent);
				Barrier->RegisterComponent();
				Barrier->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
				SplineMeshes.Add(Barrier);
			}
		}
	}
}
