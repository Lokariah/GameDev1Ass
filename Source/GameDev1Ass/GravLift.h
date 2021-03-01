// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "CustomPlayerController.h"
#include "GameFramework/Actor.h"
#include "GravLift.generated.h"

UCLASS()
class GAMEDEV1ASS_API AGravLift : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGravLift();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
		ACustomPlayerController* playerController;	   //Used to call Player lift function to create a similar effect to impulse.
private:

	//Applies force to objects that enters and calls function on player to provide similar effect.
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* GravLiftMesh;			  
	UPROPERTY(EditAnywhere)
		UBoxComponent* CollisionBox;				   
	UPROPERTY(EditAnywhere)
		USceneComponent* ForceOriginPoint;			   //Position to enact force from.
	UPROPERTY(EditAnywhere)
		float forceRadius = 100.0f;					   //Adjustable area that force should apply
	UPROPERTY(EditAnywhere)
		float forceStrength = 100.0f;				   //Adjustable strength of force applied
};
