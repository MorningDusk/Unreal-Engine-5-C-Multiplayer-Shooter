// Fill out your copyright notice in the Description page of Project Settings.


#include "Casing.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"



ACasing::ACasing()
{
	PrimaryActorTick.bCanEverTick = false;
	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(CasingMesh);
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true);
	ShellEjectionImpulse = 10.f;
	DestroyCasingDelay = 0.5f;
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();

	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);

	CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
	const FRotator RandomRotation = FRotator(FMath::RandRange(0.f, 360.f), FMath::RandRange(0.f, 360.f), FMath::RandRange(0.f, 360.f));
	CasingMesh->AddRelativeRotation(RandomRotation);
}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ShellSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShellSound, GetActorLocation());

	}
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACasing::DestroyCasing, DestroyCasingDelay, false);
}

void ACasing::DestroyCasing()
{
	Destroy();
}

