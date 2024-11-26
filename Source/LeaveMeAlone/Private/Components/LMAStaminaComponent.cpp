// LeaveMeAlone Game by Netologiya. All RightsReserved

#include "Components/LMAStaminaComponent.h"

ULMAStaminaComponent::ULMAStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULMAStaminaComponent::SetStamina(float _Stamina)
{
	Stamina = _Stamina;
}

void ULMAStaminaComponent::SetStaminaStep(float _StaminaStep)
{

	StaminaStep = _StaminaStep;
}

void ULMAStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	Stamina = MaxStamina;
}
