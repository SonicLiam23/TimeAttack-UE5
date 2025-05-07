// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UserEnum.h"

class UPlayerInfoGameInstance; class UWorld;

/**
 * 
 */
class ASSIGNMENT_API PlayerInfo
{
public:

	

	~PlayerInfo();
	static PlayerInfo* GetInstance();
	static void Reset(); 
	int health = 100;
	int collectibleValue = 0;

	// set in AAssignmentGameMode::PostBeginPlay()
	int collectibleValueNeeded;

	int releaseBlocksNeeded = 3;
	int releaseBlocksHit = 0;
	bool targetGet = false;
	
	void Init(UWorld* WorldContext);
	bool isInitialized = false;
	void SetGameState(EUserPlayStateEnum state);
	EUserPlayStateEnum GetGameState();

	

private:
	PlayerInfo();
	static PlayerInfo* instance;

	UPlayerInfoGameInstance* gameInstance;
};
