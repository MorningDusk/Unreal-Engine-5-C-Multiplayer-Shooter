// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "Components/TimelineComponent.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "Blaster/BlasterTypes/Team.h"
#include "BlasterCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlasterCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostInitializeComponents() override;

	void PlayFireMontage(bool bAiming);

	void PlayReloadMontage();

	void PlayElimMontage();

	void PlayThrowGrenadeMontage();

	void PlaySwapMontage();

	virtual void OnRep_ReplicatedMovement() override;

	void Elim(bool bPlayerLeftGame);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim(bool bPlayerLeftGame);

	virtual void Destroyed() override;

	UPROPERTY(Replicated)
	bool bDisableGameplay = false;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

	void UpdateHUDHealth();

	void UpdateHUDShield();

	void UpdateHUDAmmo();

	void UpdateHUDWeaponType();

	void SpawnDefaultWeapon();

	UPROPERTY()
	TMap<FName, class UBoxComponent*> HitCollisionBoxes;

	bool bFinishedSwapping = false;

	UFUNCTION(Server, Reliable)
	void ServerLeaveGame();

	FOnLeftGame OnLeftGame;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastGainedTheLead();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLostTheLead();

	void SetTeamColor(ETeam Team);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void SwitchWeaponButtonPressed();
	void ReloadButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased ();
	void FireButtonPressed();
	void FireButtonReleased();
	void GrenadeButtonPressed();

	void DropOrDestoryWeapon(AWeapon* Weapon);
	void DropOrDestoryWeapons();

	void SetSpawnPoint();

	void OnPlayerStateInitialized();

	void AimOffset(float DeltaTime);

	void CaclulateAO_Pitch();

	void SimProxiesTurn();

	virtual void Jump() override;

	void PlayHitReactMontage();

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);


	//Poll for any relevent classes and intiate the HUD
	void PollInit();

	void RotateInPlace(float DeltaTime);

	//**
	// Hit Boxes used for Server Side Rewind
	//

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	class UBoxComponent* head;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* pelvis;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* spine_02;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* spine_03;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* upperarm_l;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* upperarm_r;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* lowerarm_l;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* lowerarm_r;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* hand_l;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* hand_r;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* backpack;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* blanket;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* thigh_l;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* thigh_r;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* calf_l;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* calf_r;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* foot_l;

	UPROPERTY(EditAnywhere, Category = "HitBoxes")
	UBoxComponent* foot_r;

private:

	UPROPERTY(VisibleAnywhere, Category =  Camera )
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	//Blaster Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBuffComponent* Buff;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ULagCompensationComponent* LagCompensation;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	UFUNCTION(Server, Reliable)
	void ServerSwitchButtonButtonPressed();

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;

	void TurnInPlace(float DeltaTime);

	/**
	* Animation Montages
	*/

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireWeaponMontage;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ThrowGrenadeMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* SwapMontage;

	void HideCameraIfCharacterClose();

	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;

	bool bRotateRootBone;

	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;

	float ProxyYaw;

	float TimeSinceLastMovementReplication;

	float CaclulateSpeed();

	//*
	//Player Health
	// *//
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health(float LastHealth);

	//*
	//Player Shield
	// *//

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxShield = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Shield, EditAnywhere, Category = "Player Stats")
	float Shield = 0.f;

	UFUNCTION()
	void OnRep_Shield(float LastShield);

	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

	bool bElimmed = false;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

	void ElimTimerFinished();

	bool bLeftGame = false;
	/**
	* Dissolve Effect
	**/

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;
	
	FOnTimelineFloat DissolveTrack;

	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);

	void StartDissolve();

	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;

	// Dynamic Instance that we can change in runtime
	UPROPERTY(VisibleAnywhere, Category = "Elim")
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

//Material Instance set on the blueprint, used with the dynamic material instance
	UPROPERTY(VisibleAnywhere, Category = "Elim")
	UMaterialInstance* DissolveMaterialInstance;

	/*
	* Team Colors
	*/
	UPROPERTY(EditAnywhere, Category = "Elim")
	UMaterialInstance* RedDissolveMatInst;

	UPROPERTY(EditAnywhere, Category = "Elim")
	UMaterialInstance* RedMaterial;

	UPROPERTY(EditAnywhere, Category = "Elim")
	UMaterialInstance* BlueDissolveMatInst;

	UPROPERTY(EditAnywhere, Category = "Elim")
	UMaterialInstance* BlueMaterial;

	UPROPERTY(EditAnywhere, Category = "Elim")
	UMaterialInstance* OriginalMaterial;

	/**
	* Elim Effects
	**/
	UPROPERTY(EditAnywhere, Category = "Elim")
	UParticleSystem* ElimBotEffect;

	UPROPERTY(VisibleAnywhere, Category = "Elim")
	UParticleSystemComponent* ElimBotComponent;

	UPROPERTY(EditAnywhere, Category = "Elim")
	class USoundCue* ElimBotSound;

	UPROPERTY()
	class ABlasterPlayerState* BlasterPlayerState;

	UPROPERTY(EditAnywhere, Category = "Elim")
	class UNiagaraSystem* CrownSystem;

	UPROPERTY()
	class UNiagaraComponent* CrownComponent;

	//**
	// Grenade*//

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AttachedGrenade;

	//**
	// Default Weapon*//

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<AWeapon> DefaultWeaponClass;

	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;

public:	
	
	void SetOverlappingWeapon(AWeapon* Weapon);

	bool IsWeaponEquipped();

	bool IsAiming();

	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	AWeapon* GetEquippedWeapon();

	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }

	FVector GetHitTarget() const;

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }

	FORCEINLINE bool IsElimmed() const { return bElimmed; }

	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	
	FORCEINLINE float GetShield() const { return Shield; }
	FORCEINLINE void SetShield(float Amount) { Shield = Amount; }
	FORCEINLINE float GetMaxShield() const { return MaxShield; }

	ECombatState GetCombatState() const;

	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE UBuffComponent* GetBuff() const { return Buff; }
	FORCEINLINE ULagCompensationComponent* GetLagCompensation() const { return LagCompensation; }

	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }

	bool IsHoldingTheFlag() const;

	bool IsLocallyReloading();

	ETeam GetTeam();

	void SetHoldingTheFlag(bool bHolding);
};

