// LeaveMeAlone Game by Netologiya. All RightsReserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAHealthComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEAVEMEALONE_API ULMAHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULMAHealthComponent();
	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 100.0f;

public:

	DECLARE_MULTICAST_DELEGATE(FOnDeath);
	FOnDeath OnDeath;

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);
	FOnHealthChanged OnHealthChanged;

	bool AddHealth(float NewHealth);

private:
	float Health = 0.0f;
	
	bool IsHealthFull() const;
};
