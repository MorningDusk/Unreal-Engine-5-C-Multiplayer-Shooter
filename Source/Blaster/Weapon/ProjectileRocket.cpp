// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileRocket.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystemInstance.h"
#include "Components/AudioComponent.h"
#include "RocketMovementComponent.h"



AProjectileRocket::AProjectileRocket()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RocketMovementComponent = CreateDefaultSubobject<URocketMovementComponent>(TEXT("RocketMovementComponent"));
	RocketMovementComponent->bRotationFollowsVelocity = true;
	RocketMovementComponent->SetIsReplicated(true);

}


void AProjectileRocket::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) {
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectileRocket::OnHit);
	}

	SpawnTrailSystem();

	if (ProjectileLoop && LoopingSoundAttenuation) {
		ProjecileLoopComponent = UGameplayStatics::SpawnSoundAttached(
			ProjectileLoop, 
			GetRootComponent(), 
			FName(), 
			GetActorLocation(), 
			EAttachLocation::KeepWorldPosition,
			false,
			1.f,
			1.f,
			0.f,
			LoopingSoundAttenuation,
			(USoundConcurrency*)nullptr,
			false
		);
	}

}

void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner()) {
		return;
	}

	ExplodeDamage();
	StartDestoryTimer();

	if (PlayerImpactParticles) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PlayerImpactParticles, GetActorTransform());
	}
	if (ImpactSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
	}
	if (ProjectileMesh) {
		ProjectileMesh->SetVisibility(false);
	}
	if (CollisionBox) {
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (TrailSystemComponent && TrailSystemComponent->GetSystemInstance()) {
		TrailSystemComponent->GetSystemInstance()->Deactivate();
	}
	if (ProjecileLoopComponent && ProjecileLoopComponent->IsPlaying()) {
		ProjecileLoopComponent->Stop();
	}
	//Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AProjectileRocket::Destroyed()
{

}
