// LeaveMeAlone Game by Netologiya. All RightsReserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAStaminaComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEAVEMEALONE_API ULMAStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULMAStaminaComponent();

	UFUNCTION(BlueprintCallable)
	float GetStamina() const { return Stamina; }
	UFUNCTION(BlueprintCallable)
	void SetStamina(float _Stamina);

	UFUNCTION(BlueprintCallable)
	float GetStaminaStep() const { return StaminaStep; }
	UFUNCTION(BlueprintCallable)
	void SetStaminaStep(float _StaminaStep);

	UFUNCTION(BlueprintCallable)
	float GetMaxStamina() const { return MaxStamina; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	float StaminaStep = 0.4f;

public:
private:
	float Stamina = 0.0f;
};
