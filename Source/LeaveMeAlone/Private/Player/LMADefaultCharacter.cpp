// LeaveMeAlone Game by Netologiya. All RightsReserved

#include "Player/LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h "
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/LMAStaminaComponent.h"
#include "Components/LMAHealthComponent.h"

ALMADefaultCharacter::ALMADefaultCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	StaminaComponent = CreateDefaultSubobject<ULMAStaminaComponent>("StaminaComponent");
	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");

	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("Weapon");
}

void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
	HealthComponent->OnDeath.AddDynamic(this, &ALMADefaultCharacter::OnDeath);
	DeathCameraDelegate = FTimerDelegate::CreateUObject(this, &ALMADefaultCharacter::DeathCamera);
}

void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!(HealthComponent->IsDead()))
	{
		RotationPlayerOnCursor();
	}
}

void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAction("ZoomUp", IE_Pressed, this, &ALMADefaultCharacter::ZoomUp);
	PlayerInputComponent->BindAction("ZoomDown", IE_Pressed, this, &ALMADefaultCharacter::ZoomDown);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, StaminaComponent, &ULMAStaminaComponent::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, StaminaComponent, &ULMAStaminaComponent::StopSprint);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &ULMAWeaponComponent::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Reload);
}

void ALMADefaultCharacter::SetCharacterMaxWalkSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

bool ALMADefaultCharacter::GetCharacterForwardDirection()
{
	return IsMoveForward;
}

void ALMADefaultCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
	if (Value == 1)
	{
		IsMoveForward = true;
		if (StaminaComponent->SprintOn)
		{
			StaminaComponent->StartSprint();
		}
	}
	else
	{
		IsMoveForward = false;
		if (StaminaComponent->SprintOn)
		{
			StaminaComponent->StopSprint();
		}
	}
}
void ALMADefaultCharacter::MoveRight(float Value)
{
	if (!StaminaComponent->SprintOn)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ALMADefaultCharacter::ZoomUp()
{
	if (!(SpringArmComponent->TargetArmLength <= MinZoom))
	{
		SpringArmComponent->TargetArmLength = SpringArmComponent->TargetArmLength - ZoomStrength;
	}
}

void ALMADefaultCharacter::ZoomDown()
{
	if (!(SpringArmComponent->TargetArmLength >= MaxZoom))
	{
		SpringArmComponent->TargetArmLength = SpringArmComponent->TargetArmLength + ZoomStrength;
	}
}

void ALMADefaultCharacter::OnDeath()
{
	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	CurrentCursor->DestroyRenderState_Concurrent();
	DestroyPlayerInputComponent();
	GetWorld()->GetTimerManager().SetTimer(DeathCameraHandle, DeathCameraDelegate, 0.01, true, 0.0f);
	DeathCameraArmLenght = SpringArmComponent->TargetArmLength;
	WeaponComponent->Death = true;
	WeaponComponent->StartFire();
	DeathLocation = SpringArmComponent->GetRelativeLocation();
}

void ALMADefaultCharacter::RotationPlayerOnCursor()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
}

void ALMADefaultCharacter::DeathCamera()
{
	{
		if (DeathCameraArmLenght > 800.0f && !DeathMinZoom)
		{
			DeathCameraArmLenght -= 3.0f;
			if (DeathCameraArmLenght <= 1200.0f)
			{
				DeathLocation.X -= 1.0f;
			}
		}
		else
		{
			DeathMinZoom = true;
			DeathCameraYaw += 0.2f;
			DeathCameraArmLenght += 1.0f;
		}
		SpringArmComponent->TargetArmLength = DeathCameraArmLenght;
		SpringArmComponent->SetRelativeRotation(FRotator(-75.0, DeathCameraYaw, 0.0f));
		SpringArmComponent->SetRelativeLocation(DeathLocation);
		if (DeathCameraArmLenght >= 1700.0f && DeathMinZoom)
		{
			GetWorld()->GetTimerManager().ClearTimer(DeathCameraHandle);
			SetLifeSpan(0.0f);
			GameOver.Broadcast();
		}
	}
}
