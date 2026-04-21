#include "BurnoutCar.h"
#include "BurnoutGameMode.h"
#include "CrashCameraComponent.h"
#include "NearMissComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

ABurnoutCar::ABurnoutCar()
{
	PrimaryActorTick.bCanEverTick = true;

	CrashCamera = CreateDefaultSubobject<UCrashCameraComponent>(TEXT("CrashCamera"));
	NearMiss = CreateDefaultSubobject<UNearMissComponent>(TEXT("NearMiss"));
	NearMiss->SetupAttachment(GetRootComponent());
	EngineAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineAudio"));
	EngineAudio->SetupAttachment(GetRootComponent());
	EngineAudio->bAutoActivate = false;
	BoostAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("BoostAudio"));
	BoostAudio->SetupAttachment(GetRootComponent());
	BoostAudio->bAutoActivate = false;
}

void ABurnoutCar::BeginPlay()
{
	Super::BeginPlay();
	BoostCharge = BoostMaxCharge * 0.3f;
	if (EngineAudio->Sound)
		EngineAudio->Play();
}

void ABurnoutCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsBoosting && BoostCharge > 0.f)
	{
		GetMesh()->AddForce(GetActorForwardVector() * BoostForce, NAME_None, true);
		BoostCharge = FMath::Max(0.f, BoostCharge - BoostDrainRate * DeltaTime);
		if (BoostCharge <= 0.f)
		{
			bIsBoosting = false;
			if (BoostAudio->IsPlaying()) BoostAudio->Stop();
		}
	}

	// Scale engine audio pitch with speed
	if (EngineAudio->IsPlaying())
	{
		float Speed = 0.f;
		if (UChaosVehicleMovementComponent* VM = GetVehicleMovementComponent())
			Speed = FMath::Abs(VM->GetForwardSpeed());
		float MaxSpeed = 3000.f;
		float Pitch = FMath::Lerp(EnginePitchMin, EnginePitchMax, FMath::Clamp(Speed / MaxSpeed, 0.f, 1.f));
		EngineAudio->SetPitchMultiplier(Pitch);
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
	{
		bIsBoosting = true;
		if (BoostAudio->Sound && !BoostAudio->IsPlaying())
			BoostAudio->Play();
	}
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
