// LeaveMeAlone Game by Netologiya. All RightsReserved

#include "Components/LMAWeaponComponent.h"
#include "Player/LMADefaultCharacter.h"
#include "Weapon/LMABaseWeapon.h"
#include <Animations/LMAReloadFinishedAnimNotify.h>

ULMAWeaponComponent::ULMAWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ShootTimerDelegate = FTimerDelegate::CreateUObject(this, &ULMAWeaponComponent::Fire);
}

void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();
	InitAnimNotify();
}

void ULMAWeaponComponent::SpawnWeapon()
{
	if (IsValid(GetWorld()))
	{
		Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
		Weapon->EmptyClip.AddDynamic(this, &ULMAWeaponComponent::OnEmtyClipHandler);
		if (Weapon)
		{
			const auto Character = Cast<ACharacter>(GetOwner());
			if (Character)
			{
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
				Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponSocket);
			}
		}
	}
}

void ULMAWeaponComponent::Fire()
{
	const auto Character = Cast<ALMADefaultCharacter>(GetOwner());
	if (Weapon && !AnimReloading && !Character->SprintOn)
	{
		Weapon->Fire();
	}
}

void ULMAWeaponComponent::StartFire()
{
	GetWorld()->GetTimerManager().SetTimer(ShootTimerHandle, ShootTimerDelegate, ShootDelay, true, 0);
}

void ULMAWeaponComponent::OnEmtyClipHandler()
{
	StopFire();
	AutoReload();
}

void ULMAWeaponComponent::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
}

void ULMAWeaponComponent::InitAnimNotify()
{
	if (!ReloadMontage)
		return;

	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
	}
}

bool ULMAWeaponComponent::CanReload() const
{
	if (Weapon->FullClip())
	{
		return false;
	}
	const auto Character = Cast<ALMADefaultCharacter>(GetOwner());
	if (Character->SprintOn)
	{
		return false;
	}
	return !AnimReloading;
}

void ULMAWeaponComponent::Reload()
{
	AutoReload();
}

void ULMAWeaponComponent::AutoReload()
{
	if (!CanReload())
		return;
	Weapon->ChangeClip();
	AnimReloading = true;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	Character->PlayAnimMontage(ReloadMontage);
}
