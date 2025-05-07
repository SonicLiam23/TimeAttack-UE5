// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"
#include "Components/TextBlock.h"
#include "PlayerInfo.h"
#include "UserEnum.h"
#include "PlayerInfoGameInstance.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Fonts/SlateFontInfo.h"
#include "Assignment/Public/PlayGameButton.h"

void UPlayerUI::UpdateInfo()
{
	if (infoText && PlayerInfo::GetInstance()->isInitialized)
	{
		if (PlayerInfo::GetInstance()->GetGameState() != EUserPlayStateEnum::PLAY)
		{
			infoText->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			infoText->SetVisibility(ESlateVisibility::Visible);
		}

		PlayerInfo* info = PlayerInfo::GetInstance();

		

		FString healthText = FString::Printf(TEXT("%d / 100"), info->health);
		FString collectibleText = FString::Printf(TEXT("%d / %d"), info->collectibleValue, info->collectibleValueNeeded);
		FString blockText = FString::Printf(TEXT("%d / %d"), info->releaseBlocksHit, info->releaseBlocksNeeded);
		
		

		infoText->SetText(FText::FromString(FString::Printf(TEXT(
			"Health: %s\nCollectible Value: %s\nBlocks Hit: %s "), 
			*healthText,
			*collectibleText,
			*blockText)));
	}

	if (menuText && PlayerInfo::GetInstance()->isInitialized)
	{
		switch (PlayerInfo::GetInstance()->GetGameState())
		{
		case EUserPlayStateEnum::MENU:
			menuText->SetText(FText::FromString(TEXT("MAIN MENU")));
			break;
		case EUserPlayStateEnum::WIN:
			menuText->SetText(FText::FromString(TEXT("YOU WIN")));
			break;
		case EUserPlayStateEnum::LOSE:
			menuText->SetText(FText::FromString(TEXT("YOU LOSE")));
			break;
		case EUserPlayStateEnum::PLAY:
			menuText->SetText(FText::FromString(TEXT("")));
			break;
		}

		if (playButton && PlayerInfo::GetInstance()->GetGameState() != EUserPlayStateEnum::PLAY)
		{
			playButton->SetVisibility(ESlateVisibility::Visible);
			playButton->SetIsEnabled(true);
		}
		else if (playButton)
		{
			playButton->SetVisibility(ESlateVisibility::Hidden);
			playButton->SetIsEnabled(false);

		}
	}
}

void UPlayerUI::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	UpdateInfo();
}

void UPlayerUI::NativeConstruct()
{
    Super::NativeConstruct();


    // Check if TextBlock is successfully created
    if (infoText)
    {

		// Set the font size (optional)
		FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 24);
		infoText->SetFont(FontInfo);
    }

	if (menuText)
	{
		FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 42);
		menuText->SetFont(FontInfo);
	}
}
