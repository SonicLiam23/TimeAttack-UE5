// Fill out your copyright notice in the Description page of Project Settings.


#include "Gate.h"
#include "PlayerInfo.h"

// Sets default values
AGate::AGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerInfo::GetInstance()->releaseBlocksHit >= PlayerInfo::GetInstance()->releaseBlocksNeeded && PlayerInfo::GetInstance()->targetGet)
	{
		FVector pos = GetActorLocation();
		pos.Z -= 2.0f;
		SetActorLocation(pos);

		if (pos.Z < -500)
		{
			Destroy();
		}
	}

}

