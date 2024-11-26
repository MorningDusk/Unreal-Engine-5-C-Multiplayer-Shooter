// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileRocket.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()
	
public:

	AProjectileRocket();

	virtual void Destroyed() override;

protected:

	virtual void BeginPlay() override;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(EditAnywhere, Category = "Damage")
	class USoundCue* ProjectileLoop;

	UPROPERTY()
	class UAudioComponent* ProjecileLoopComponent;

	UPROPERTY(EditAnywhere, Category = "Damage")
	USoundAttenuation* LoopingSoundAttenuation;

	UPROPERTY(VisibleAnywhere, Category = "Damage")
	class URocketMovementComponent* RocketMovementComponent;

private:

};
