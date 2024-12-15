// LeaveMeAlone Game by Netologiya. All RightsReserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAStaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChange, float, Stamina);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEAVEMEALONE_API ULMAStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULMAStaminaComponent();

	UFUNCTION(BlueprintCallable)
	void SetStamina(float _Stamina);

	UFUNCTION(BlueprintCallable)
	void SetStaminaStep(float _StaminaStep);

	UFUNCTION()
	void DecreaseStamina();
	UFUNCTION()
	void IncreaseStamina();

	UFUNCTION()
	void StartSprint();

	UFUNCTION()
	void StopSprint();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float StaminaTimerDelay = 0.01f;

	FTimerHandle DecreaseStaminaTimerHandle;
	FTimerDelegate DecreaseStaminaTimerDelegate;

	FTimerHandle IncreaseStaminaTimerHandle;
	FTimerDelegate IncreaseStaminaTimerDelegate;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float IncreaseStaminaLag = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement");
	float SprintSpeed = 600.0f;
	UPROPERTY(EditAnywhere, Category = "Movement");
	float WalkSpeed = 300.0f;
	

private:
	float Stamina = 0.0f;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool SprintOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StaminaChangeStrength = 0.15f;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaChange OnStaminaChange;
};
