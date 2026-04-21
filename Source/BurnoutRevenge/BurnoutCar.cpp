#include "BurnoutCar.h"
#include "BurnoutGameMode.h"
#include "CrashCameraComponent.h"
#include "NearMissComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

ABurnoutCar::ABurnoutCar()
{
	PrimaryActorTick.bCanEverTick = true;

	CrashCamera = CreateDefaultSubobject<UCrashCameraComponent>(TEXT("CrashCamera"));
	NearMiss = CreateDefaultSubobject<UNearMissComponent>(TEXT("NearMiss"));
	NearMiss->SetupAttachment(GetRootComponent());
}

void ABurnoutCar::BeginPlay()
{
	Super::BeginPlay();
	BoostCharge = BoostMaxCharge * 0.3f;
}

void ABurnoutCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsBoosting && BoostCharge > 0.f)
	{
		GetMesh()->AddForce(GetActorForwardVector() * BoostForce, NAME_None, true);
		BoostCharge = FMath::Max(0.f, BoostCharge - BoostDrainRate * DeltaTime);
		if (BoostCharge <= 0.f)
			bIsBoosting = false;
	}
}

void ABurnoutCar::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	CrashCamera->TriggerCrash(NormalImpulse);
}

void ABurnoutCar::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Boost", IE_Pressed, this, &ABurnoutCar::OnBoostPressed);
	InputComponent->BindAction("Boost", IE_Released, this, &ABurnoutCar::OnBoostReleased);
	InputComponent->BindAction("RestartGame", IE_Pressed, this, &ABurnoutCar::OnRestartPressed);
}

void ABurnoutCar::AddBoostCharge(float Amount)
{
	BoostCharge = FMath::Min(BoostMaxCharge, BoostCharge + Amount);
}

void ABurnoutCar::OnBoostPressed()
{
	if (BoostCharge > 0.f)
		bIsBoosting = true;
}

void ABurnoutCar::OnBoostReleased()
{
	bIsBoosting = false;
}

void ABurnoutCar::OnRestartPressed()
{
	if (ABurnoutGameMode* GM = Cast<ABurnoutGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		GM->RestartGame();
}
