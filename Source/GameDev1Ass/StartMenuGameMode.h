// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StartMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEV1ASS_API AStartMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	//Spawns Menu UI and plays background music.
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> MainMenuClass;						//Blueprint to create and attach Main Menu UI to viewport.
	UPROPERTY()														
		UUserWidget* MainMenuCount;									//Monitors UI from Inception and is used as an interface to the UI.
	UPROPERTY(EditAnywhere) float BackgroundMusicVolume = 1.0f;		//Sound file to play Background Music.
	UPROPERTY(EditAnywhere) USoundBase* GameBackgroundMusic;		//Volume to play Background Music.
};
