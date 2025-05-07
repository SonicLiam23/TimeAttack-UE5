// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include <cmath>

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// no external forces so equilibrium position is the centre
	equilibrium = (locationOne + locationTwo) / 2.0f;

	// half the distance between the 2 locations
	amplitude = (locationTwo - locationOne).Size() / 2.0f;

	moveDirection = (locationTwo - locationOne).GetSafeNormal();

	aFrequency = (2 * PI) / timePeriod; // Angular frequency or omega
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float time = GetWorld()->GetTimeSeconds();
	float displacement = amplitude * FMath::Sin(aFrequency * time);
	SetActorLocation(equilibrium + moveDirection * displacement);
}

