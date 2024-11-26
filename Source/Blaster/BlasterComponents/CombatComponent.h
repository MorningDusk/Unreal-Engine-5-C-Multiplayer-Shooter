// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "CombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCombatComponent();

	friend class ABlasterCharacter;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EquipWeapon(class AWeapon* WeaponToEquip);

	void SwapWeapons();

	void Reload();

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	UFUNCTION(BlueprintCallable)
	void FinishSwap();

	UFUNCTION(BlueprintCallable)
	void FinishSwapAttachWeapons();

	void FireButtonPressed(bool bPressed);

	UFUNCTION(BlueprintCallable)
	void ShotgunShellReload();

	void JumpToShotgunEnd();
	
	UFUNCTION(BlueprintCallable)
	void ThrowGrenadeFinished();

	UFUNCTION(BlueprintCallable)
	void LaunchGrenade();

	UFUNCTION(Server, Reliable)
	void ServerLaunchGrenade(const FVector_NetQuantize& Target);

	void PickUpAmmo(EWeaponType WeaponType, int32 AmmoAmount);

	bool bLocallyReloading = false;

protected:

	virtual void BeginPlay() override;

	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION()
	void OnRep_SecondaryWeapon();

	void Fire();
	void FireProjectileWeapon();
	void FireHitScanWeapon();
	void FireShotgun();

	void LocalFire(const FVector_NetQuantize& TraceHitTarget);

	void ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTarget);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget, float FireDelay);

	UFUNCTION(Server, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void ServerShotgunFire(const TArray<FVector_NetQuantize>& HitTargets, float FireDelay);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastShotgunFire(const TArray<FVector_NetQuantize>& HitTargets);


	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void ServerReload();

	void HandleReload();

	int32 AmountToReload();

	void ThrowGrenade();

	UFUNCTION(Server, Reliable)
	void ServerThrowGrenade();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> GrenadeClass;

	void DropEquippedWeapon();

	void AttachActorToRightHand(AActor* ActorToAttach);
	void AttachActorToLeftHand(AActor* ActorToAttach);
	void AttachFlagToLeftHand(AWeapon* Flag);
	void AttachActorToBackpack(AActor* ActorToAttach);
	void UpdateCarriedAmmo();

	void PlayEquipWeaponSound(AWeapon* WeaponToEquip);

	void ReloadEmptyWeapon();

	void ShowAttachedGreande(bool bShowGrenade);

	void EquipPrimaryWeapon(AWeapon* WeaponToEquip);
	void EquipSecondaryWeapon(AWeapon* WeaponToEquip);

private:

	UPROPERTY()
	class ABlasterCharacter* Character;
	UPROPERTY()
	class ABlasterPlayerController* Controller;
	UPROPERTY()
	class ABlasterHUD* HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	class AWeapon* EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
	AWeapon* SecondaryWeapon;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Aiming)
	bool bAiming = false;

	bool bAimButtonPressed = false;

	UFUNCTION()
	void OnRep_Aiming();

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;
	
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	/**
	* HUD and Crosshairs
	**/

	float CrosshairVelocityFactor;

	float CrosshairInAirFactor;
	 
	float CrosshairAimFactor;

	float CrosshairShootingFactor;

	float CrosshairAimAtPlayerFactor;

	FVector HitTarget;

	FHUDPackage HUDPackage;

	//UPROPERTY(EditAnywhere)
	//float CrosshairAimSize = 0.58f;
	/**
	* Aiming and FOV
	**/

	//Field of View when not aiming; set to the camera's FOV in BeginPlay
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomInterpSpeed = 20.f;

	float CurrentFOV;

	void InterpFOV(float DeltaTime);

	/**
	* Autoamatic Fire
	**/

	FTimerHandle FireTimer;
	
	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire();

	// Carried Ammo for the currently Equipped Weapon
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 StartingARAmmo = 60;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 StartingRocketAmmo = 8;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 StartingPistolAmmo = 30;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 StartingSMGAmmo = 40;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 StartingShotgunAmmo = 8;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 StartingSniperAmmo = 8;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 StartingGrenadeLauncherAmmo = 12;

	void InitialzieCarriedAmmo();

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
	void OnRep_CombatState();

	void UpdateAmmoValues();

	void UpdateShotgunAmmoValues();

	UPROPERTY(ReplicatedUsing = OnRep_Grenades)
	int32 Grenades = 4;

	UFUNCTION()
	void OnRep_Grenades();

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 MaxGrenades = 4;

	void UpdateHUDGrenades();

	UPROPERTY(ReplicatedUsing = OnRep_HoldingTheFlag)
	bool bHoldingTheFlag = false;

	UFUNCTION()
	void OnRep_HoldingTheFlag();

	UPROPERTY()
	AWeapon* TheFlag;

public:	

	FORCEINLINE int32 GetGreandes() const { return Grenades; }

	FORCEINLINE bool ShouldSwapWeapons() { return EquippedWeapon != nullptr && SecondaryWeapon != nullptr; };
};
