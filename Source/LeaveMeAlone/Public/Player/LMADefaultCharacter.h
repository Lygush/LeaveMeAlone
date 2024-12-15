// LeaveMeAlone Game by Netologiya. All RightsReserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Components/LMAWeaponComponent.h>
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
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Stamina")
	ULMAStaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Weapon")
	ULMAWeaponComponent* WeaponComponent;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }
	UFUNCTION()
	ULMAStaminaComponent* GetStaminaComponent() const { return StaminaComponent; }
	UFUNCTION()
	ULMAWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }

	UFUNCTION()
	void SetCharacterMaxWalkSpeed(float Speed);

	UFUNCTION()
	bool GetCharacterForwardDirection();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool SprintOnAnim = false;

private:
	float YRotation = -75.0f;
	UPROPERTY(EditAnywhere)
	float ArmLength = 1400.0f;
	float FOV = 55.0f;

	void MoveForward(float Value);
	void MoveRight(float Value);

	bool IsMoveForward = false;

	UPROPERTY(EditAnywhere, Category = "Zoom")
	float MinZoom = 800.0f;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float MaxZoom = 3400.0f;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float ZoomStrength = 80.0f;

	void ZoomUp();
	void ZoomDown();

	UPROPERTY(EditDefaultsOnly, Category = "Death|Animation")
	UAnimMontage* DeathMontage;

	UFUNCTION()
	void OnDeath();
	void RotationPlayerOnCursor();

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float DeathCameraYaw = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Death")
	float DeathCameraArmLenght = 1000.0f;
	bool DeathMinZoom = false;
	FVector DeathLocation;

	FTimerDelegate DeathCameraDelegate;
	FTimerHandle DeathCameraHandle;

	void DeathCamera();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverDelegate);
	UPROPERTY(BlueprintAssignable)
	FGameOverDelegate GameOver;
	
};
