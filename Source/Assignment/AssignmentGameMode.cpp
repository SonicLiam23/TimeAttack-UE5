// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssignmentGameMode.h"
#include "AssignmentCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerInfo.h"
#include "PlayerUI.h"
#include "Collectible.h"

AAssignmentGameMode::AAssignmentGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AAssignmentGameMode::BeginPlay()
{
	FMath::RandInit(FDateTime::Now().GetMillisecond());
	PlayerInfo::Reset();

	if (PlayerUIClass)
	{
		UPlayerUI* PlayerUI = CreateWidget<UPlayerUI>(GetWorld(), PlayerUIClass);
		if (PlayerUI)
		{
			PlayerUI->AddToViewport(9999);
			PlayerUI->SetVisibility(ESlateVisibility::Visible);
			UE_LOG(LogTemp, Warning, TEXT("Player UI Added to viewport"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create widget from PlayerUIClass"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerUIClass is null!"));
	}

	// something that runs after begin play on all objects, but still in the first few frames
	FTimerHandle tempTimer;
	GetWorld()->GetTimerManager().SetTimer(tempTimer, this, &AAssignmentGameMode::PostBeginPlay, 0.01f, false);
	// this is a bit of a hack, but it works
}

void AAssignmentGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Quit)
	{
		// clean up the memory (as i used new)
		PlayerInfo::Reset();
	}
}

void AAssignmentGameMode::PostBeginPlay()
{
	PlayerInfo::GetInstance()->Init(GetWorld());
	PlayerInfo::GetInstance()->collectibleValueNeeded = FMath::RandRange(5, ACollectible::totalValue);

	if (PlayerInfo::GetInstance()->GetGameState() != EUserPlayStateEnum::PLAY)
	{
		GetWorld()->GetFirstPlayerController()->SetPause(true);
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		GetWorld()->GetFirstPlayerController()->bEnableClickEvents = true;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	}
	else
	{
		GetWorld()->GetFirstPlayerController()->SetPause(false);
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
		GetWorld()->GetFirstPlayerController()->bEnableClickEvents = false;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	}
}
