// LeaveMeAlone Game by Netologiya. All RightsReserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULMAStaminaComponent;
class ULMAHealthComponent;
class UAnimMontage;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALMADefaultCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Stamina")
	ULMAStaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	virtual void BeginPlay() override;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	bool SprintOn = false;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }

private:
	float YRotation = -75.0f;
	UPROPERTY(EditAnywhere)
	float ArmLength = 1400.0f;
	float FOV = 55.0f;

	void MoveForward(float Value);
	void MoveRight(float Value);

	UPROPERTY(EditAnywhere, Category = "Sprint");
	float SprintSpeed = 600.0f;
	UPROPERTY(EditAnywhere, Category = "Sprint");
	float WalkSpeed = 300.0f;
	bool Shift = false;

	void ShiftOn();
	void ShiftOff();

	UPROPERTY(EditAnywhere, Category = "Zoom")
	float MinZoom = 700.0f;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float MaxZoom = 3400.0f;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float ZoomStrength = 80.0f;

	void ZoomUp();
	void ZoomDown();

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	void OnDeath();
	void RotationPlayerOnCursor();
	void OnHealthChanged(float NewHealth);
};
