// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameButton.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerInfo.h"

void UPlayGameButton::NativeConstruct()
{
	Super::NativeConstruct();
	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UPlayGameButton::OnPlayClicked);
	}
}

void UPlayGameButton::OnPlayClicked()
{
	// Set the game state to PLAY
	PlayerInfo::GetInstance()->SetGameState(EUserPlayStateEnum::PLAY);
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
	// Hide it visually

	UGameplayStatics::OpenLevel(this, FName("MainGame"));
	PlayButton->SetVisibility(ESlateVisibility::Hidden);

	// Disable interaction
	PlayButton->SetIsEnabled(false);
}
