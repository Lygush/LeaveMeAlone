// LeaveMeAlone Game by Netologiya. All RightsReserved

#include "Components/LMAStaminaComponent.h"
#include "Player/LMADefaultCharacter.h"

ULMAStaminaComponent::ULMAStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	DecreaseStaminaTimerDelegate = FTimerDelegate::CreateUObject(this, &ULMAStaminaComponent::DecreaseStamina);
	IncreaseStaminaTimerDelegate = FTimerDelegate::CreateUObject(this, &ULMAStaminaComponent::IncreaseStamina);
}

void ULMAStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
	Stamina = MaxStamina;
	OnStaminaChange.Broadcast(Stamina);
}

void ULMAStaminaComponent::SetStamina(float _Stamina)
{
	Stamina = _Stamina;
	OnStaminaChange.Broadcast(Stamina);
}

void ULMAStaminaComponent::SetStaminaStep(float _StaminaTimerDelay)
{
	StaminaTimerDelay = _StaminaTimerDelay;
}

void ULMAStaminaComponent::DecreaseStamina()
{
	if (SprintOn && Stamina > 0)
	{
		Stamina = Stamina - StaminaChangeStrength;
	}
	else
	{
		StopSprint();
	}
	OnStaminaChange.Broadcast(Stamina);
}

void ULMAStaminaComponent::IncreaseStamina()
{
	if (Stamina < MaxStamina && !SprintOn)
	{
		Stamina = Stamina + StaminaChangeStrength;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(IncreaseStaminaTimerHandle);
	}
	OnStaminaChange.Broadcast(Stamina);
}

void ULMAStaminaComponent::StartSprint()
{
	const auto Character = Cast<ALMADefaultCharacter>(GetOwner());
	if (Character->GetCharacterForwardDirection())
	{
		if (!Character->GetWeaponComponent()->AnimReloading && !SprintOn)
		{
			SprintOn = true;
			Character->SprintOnAnim = true;
			Character->SetCharacterMaxWalkSpeed(SprintSpeed);
			GetWorld()->GetTimerManager().SetTimer(DecreaseStaminaTimerHandle, DecreaseStaminaTimerDelegate, StaminaTimerDelay, true, 0.0f);
		}
	}
}

void ULMAStaminaComponent::StopSprint()
{
	if (SprintOn)
	{
		const auto Character = Cast<ALMADefaultCharacter>(GetOwner());
		Character->SprintOnAnim = false;
		Character->SetCharacterMaxWalkSpeed(WalkSpeed);
		GetWorld()->GetTimerManager().ClearTimer(DecreaseStaminaTimerHandle);
		if (Stamina / MaxStamina < 0.15f)
		{
			IncreaseStaminaLag = 3.0f;
		}
		GetWorld()->GetTimerManager().SetTimer(IncreaseStaminaTimerHandle, IncreaseStaminaTimerDelegate, StaminaTimerDelay, true, IncreaseStaminaLag);
		IncreaseStaminaLag = 0.0f;
		SprintOn = false;
	}
}
