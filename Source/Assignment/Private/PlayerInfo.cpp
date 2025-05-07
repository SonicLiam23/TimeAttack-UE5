// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfo.h"
#include "PlayerInfoGameInstance.h"
#include "Kismet/GameplayStatics.h"

PlayerInfo* PlayerInfo::instance = nullptr;

void PlayerInfo::Reset()
{
	delete instance;
	instance = nullptr;
}

PlayerInfo::PlayerInfo()
{
}

PlayerInfo::~PlayerInfo()
{
}

PlayerInfo* PlayerInfo::GetInstance()
{
	if (!instance)
	{
		instance = new PlayerInfo();
	}
	return instance;
}

void PlayerInfo::Init(UWorld* WorldContext)
{
	gameInstance = Cast<UPlayerInfoGameInstance>(UGameplayStatics::GetGameInstance(WorldContext));
	isInitialized = true;
}

void PlayerInfo::SetGameState(EUserPlayStateEnum state)
{
	gameInstance->currentEUserPlayStateEnum = state;
}

EUserPlayStateEnum PlayerInfo::GetGameState()
{
	return gameInstance->currentEUserPlayStateEnum;
}
