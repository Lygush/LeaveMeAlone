// LeaveMeAlone Game by Netologiya. All RightsReserved

#include "Player/LMAPlayerController.h"
#include "Player/LMADefaultCharacter.h"

ALMAPlayerController::ALMAPlayerController() {}

void ALMAPlayerController::BeginPlay()
{
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void ALMAPlayerController::BeginSpectatingState()
{
	Super::BeginSpectatingState();
}