// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	//
	//Used with ServerSideRewind
	// 
		
	bool bUseServerSideRewind;

	FVector_NetQuantize TraceStart;
	FVector_NetQuantize100 InitialVelocity;

	UPROPERTY(EditAnywhere)
	float InitialSpeed = 15000.f;
		
	// Only set this for Greandes and Rockets
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;


	//Doesn't matter for greandes and rockets
	UPROPERTY(EditAnywhere)
	float HeadShotDamage = 40.f;

protected:
	virtual void BeginPlay() override;
	
	virtual void SpawnTrailSystem();

	virtual void StartDestoryTimer();

	virtual void DestroyTimerFinished();

	virtual void ExplodeDamage();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* PlayerImpactParticles;

	UPROPERTY()
	class UParticleSystem* ParticlesToPlay;

	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere)
	class USoundCue* PlayerImpactSound;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Damage")
	class UNiagaraSystem* TrailSystem;

	UPROPERTY()
	class UNiagaraComponent* TrailSystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "Damage")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float MinimumDamage = 10.f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float InnerDamageRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float OuterDamageRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageFalloff = 1.f;

private:

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Tracer;

	UPROPERTY()
	class UParticleSystemComponent* TracerComponent;

	FTimerHandle DestroyTimer;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float DestroyTime = 3.f;

public:	

};
