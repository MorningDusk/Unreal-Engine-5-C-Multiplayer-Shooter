// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "BlasterPlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighPingDelegate, bool, bPingTooHigh);

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	void SetHUDHealth(float Health, float MaxHealth);

	void SetHUDShield(float Shield, float MaxShield);

	void SetHUDScore(float Score);

	void SetHUDDefeats(int32 Defeats);

	void SetHUDWeaponAmmo(int32 Ammo);

	void SetHUDCarriedAmmo(int32 Ammo);

	void SetHUDWeaponType(EWeaponType WeaponType);

	void SetHUDMatchCountdown(float CountdownTime);

	void SetHUDAnnoucementCountDown(float CountdownTime);

	void SetHUDGrenades(int32 Grenades);

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void HideTeamScores();
	void InitTeamScores();
	void SetHUDRedTeamScore(int32 RedScore);
	void SetHUDBlueTeamScore(int32 BlueScore);

	virtual float GetServerTime();//Synced with server world clock

	virtual void ReceivedPlayer() override;//Synced with server clock as soon as possible

	void OnMatchStateSet(FName State, bool bTeamsMatch = false);

	void HandleMatchHasStarted(bool bTeamsMatch = false);

	void HandleCooldown();

	float SingleTripTime = 0.f;

	FHighPingDelegate HighPingDelegate;

	void BroadcastElim(ABlasterPlayerState* Attacker, ABlasterPlayerState* Victim);
protected:

	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	void SetHUDTime();

	void PollInit();

	/**
	*Sync Time between client and server
	**/

	//Requests the current server time passing in the client's time when the request was sent
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	//Reports the current server time to the client in response to ServerRequestServerTime
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeClientRequest, float TimeServerReceivedClientRequest);

	//Differance between client and server time
	float ClientServerDelta = 0.f;

	UPROPERTY(EditAnywhere, Category = "Time")
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;

	void CheckTimeSync(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();

	UFUNCTION(Client, Reliable)
	void ClientJoinMidGame(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime);

	void HighPingWarning();

	void StopHighPingWarning();

	void CheckPing(float DeltaTime);

	void ShowReturnToMainMenu();

	UFUNCTION(Client,Reliable)
	void ClientElimAnnouncement(ABlasterPlayerState* Attacker, ABlasterPlayerState* Victim);

	UPROPERTY(ReplicatedUsing = OnRep_ShowTeamsScores)
	bool bShowTeamsScores = false;

	UFUNCTION()
	void OnRep_ShowTeamsScores();

	FString GetInfoText(const TArray<class ABlasterPlayerState*>& Players);
	FString GetTeamsInfoText(class ABlasterGameState* BlasterGameState);

private:

	UPROPERTY()
	class ABlasterHUD* BlasterHUD;
	
	/**
	Return to main menu;
	**/

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> ReturnToMainMenuWidget;

	UPROPERTY()
	class UReturnToMainMenu* ReturnToMainMenu;

	bool bReturnToMainMenuOpen = false;

	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;
	
	float MatchTime = 0.f;

	float WarmupTime = 0.f;

	float CooldownTime = 0.f;

	float LevelStartingTime = 0.f;

	uint32 CountdownInt = 0;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	/*bool bInitializeCharacterOverlay = false;

	bool bInitializeCharacterOverlay = false;*/

	float HUDHealth;
	bool bInitializeHealth = false;

	float HUDMaxShield;
	float HUDScode;
	bool bInitializeScore = false;

	int32 HUDDefeats;
	bool bInitializeDefeats = false;

	int32 HUDGrenades;
	bool bInitializeGrenades = false;

	float HUDShield;
	bool bInitializeShield = false;

	float HUDMaxHealth;

	float HUDCarriedAmmo;
	bool bInitializeCarriedAmmo = false;

	float HUDWeaponAmmo;
	bool bInitializeWeaponAmmo = false;

	EWeaponType HUDWeaponType;
	bool bInitializeWeaponType = false;

	float HighPingRunningTime = 0.f;

	UPROPERTY(EditAnywhere, Category = "Ping")
	float HighPingDuration = 5.f;

	float PingAnimationRunningTime = 0.f;

	UPROPERTY(EditAnywhere, Category = "Ping")
	float CheckPingFrequency = 20.f;

	UFUNCTION(Server, Reliable)
	void ServerReportPingStatus(bool bHighPing);

	UPROPERTY(EditAnywhere, Category = "Ping")
	float HighPingThreshold = 50.f;
};
