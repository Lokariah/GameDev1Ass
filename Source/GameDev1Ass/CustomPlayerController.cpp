// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "PlayerCharacter.h"
#include "GameDev1AssGameModeBase.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"

void ACustomPlayerController::BeginPlay() {
	Super::BeginPlay();
	playerCharacter = Cast<APlayerCharacter>(GetPawn());
	GameModeRef = Cast<AGameDev1AssGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	playerHealth = playerHealthMax;
	gunClipAmmo = gunClipSize;
	PlayerHUDCount = CreateWidget(this, PlayerHUDClass);
	if (PlayerHUDCount) PlayerHUDCount->AddToViewport();
	MiniMapCount = CreateWidget(GetWorld(), MiniMapClass);
	if (MiniMapCount) MiniMapCount->AddToViewport();

}

void ACustomPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	check(InputComponent);
	InputComponent->BindAxis(TEXT("Forward"), this, &ACustomPlayerController::MoveForwards);
	InputComponent->BindAxis(TEXT("Strafe"), this, &ACustomPlayerController::Strafe);
	InputComponent->BindAxis(TEXT("Turn"), this, &ACustomPlayerController::Turn);
	InputComponent->BindAxis(TEXT("Pitch"), this, &ACustomPlayerController::Pitch);
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACustomPlayerController::JumpCharacter);
	InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ACustomPlayerController::Fire);
	InputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ACustomPlayerController::Reload);
	InputComponent->BindAction(TEXT("Catch"), IE_Pressed, this, &ACustomPlayerController::Catch);
}

void ACustomPlayerController::JumpCharacter()
{
	if (playerCharacter && GameModeRef->GetPlayAllowed()) playerCharacter->Jump();
}

void ACustomPlayerController::Fire() {
	UE_LOG(LogTemp, Warning, TEXT("Fire Pressed"));
	if (GameModeRef->GetPlayAllowed()) {
		if (bBallHeld) {
			UE_LOG(LogTemp, Warning, TEXT("Ball Held"));
			if (playerCharacter) playerCharacter->Fire();
			bBallHeld = false;
		}
		else {
			if (gunClipAmmo > 0) {
				gunClipAmmo--;
				FVector cameraLocation;
				FRotator cameraRotation;
				GetPlayerViewPoint(cameraLocation, cameraRotation);

				FVector end = cameraLocation + cameraRotation.Vector() * castRange;

				FHitResult Hit;
				FCollisionQueryParams lineTraceParams;

				lineTraceParams.AddIgnoredActor(playerCharacter);
				bool bDidHit = GetWorld()->LineTraceSingleByChannel(Hit, cameraLocation, end, ECC_Pawn, lineTraceParams);
				if (bDidHit && Hit.GetActor() != nullptr) {
					UE_LOG(LogTemp, Warning, TEXT("Hit something: %s"), *Hit.GetActor()->GetName());
					if (Hit.GetActor()->FindComponentByClass(UCharacterMovementComponent::StaticClass())) {

					}
					else {
						UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Hit.GetActor()->GetRootComponent());
						RootComp->AddImpulse(cameraRotation.Vector() * impulseForce * RootComp->GetMass());
					}
					UGameplayStatics::ApplyDamage(Hit.GetActor(), gunBaseDamage, this, playerCharacter, UDamageType::StaticClass());
					ADamagePointVisualiser* TempDamageVisualiser = GetWorld()->SpawnActor<ADamagePointVisualiser>(DamagePointClass, Hit.ImpactPoint, cameraRotation);
					TempDamageVisualiser->SetOwner(this);
				}
			}
			else {
				OutOfAmmoCount = CreateWidget(this, OutOfAmmoClass);
				if (OutOfAmmoCount) OutOfAmmoCount->AddToViewport();
			}
		}
	}
}

void ACustomPlayerController::Reload() {
	gunClipAmmo = gunClipSize;
}

void ACustomPlayerController::Catch() {
	UE_LOG(LogTemp, Warning, TEXT("Catch Pressed"));
	if (playerCharacter && GameModeRef->GetPlayAllowed()) {
		FVector cameraLocation;
		FRotator cameraRotation;
		GetPlayerViewPoint(cameraLocation, cameraRotation);

		FVector end = cameraLocation + cameraRotation.Vector() * castRange;

		FHitResult Hit;
		FCollisionQueryParams lineTraceParams;

		lineTraceParams.AddIgnoredActor(playerCharacter);
		bool bDidHit = GetWorld()->LineTraceSingleByChannel(Hit, cameraLocation, end, ECC_Pawn, lineTraceParams);

		if (bDidHit && Hit.GetActor() == GameModeRef->inPlayBall && Hit.Distance < ballPickUpDistance) {
			bBallHeld = true;
			UE_LOG(LogTemp, Warning, TEXT("Ball Held value %s"), bBallHeld? TEXT("true") : TEXT("false"));
			GameModeRef->DeleteBall();
		}
	}
}


void ACustomPlayerController::MoveForwards(float axisAmount)
{
	if (playerCharacter && GameModeRef->GetPlayAllowed()) playerCharacter->AddMovementInput(playerCharacter->GetActorForwardVector() * axisAmount * moveSpeed * GetWorld()->DeltaTimeSeconds);
}

void ACustomPlayerController::Strafe(float axisAmount)
{
	if (playerCharacter && GameModeRef->GetPlayAllowed()) playerCharacter->AddMovementInput(playerCharacter->GetActorRightVector() * axisAmount * moveSpeed * GetWorld()->DeltaTimeSeconds);
}

void ACustomPlayerController::Turn(float axisAmount)
{
	if (playerCharacter && GameModeRef->GetPlayAllowed()) playerCharacter->AddControllerPitchInput(axisAmount * rotationSpeed * GetWorld()->DeltaTimeSeconds);
}

void ACustomPlayerController::Pitch(float axisAmount)
{
	if (playerCharacter && GameModeRef->GetPlayAllowed()) playerCharacter->AddControllerYawInput(axisAmount * rotationSpeed * GetWorld()->DeltaTimeSeconds);
}

void ACustomPlayerController::GravLift(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	if (playerCharacter && GameModeRef->GetPlayAllowed()) playerCharacter->LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
}

float ACustomPlayerController::GetHealth()
{
	return playerHealth;
}

float ACustomPlayerController::GetHealthTotal()
{
	return playerHealthMax;
}

float ACustomPlayerController::GetAmmo()
{
	return gunClipAmmo;
}

float ACustomPlayerController::GetAmmoTotal()
{
	return gunClipSize;
}

void ACustomPlayerController::RecastPlayerCharacter()
{
	playerCharacter = Cast<APlayerCharacter>(GetPawn());
}
