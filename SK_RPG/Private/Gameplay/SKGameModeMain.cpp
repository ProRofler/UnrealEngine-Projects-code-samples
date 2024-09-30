// Copyright (c) 2024. Sir Knight title is a property of Quantinum ltd. All rights reserved.


#include "Gameplay/SKGameModeMain.h"
#include "Characters/SKPlayerCharacter.h"
#include "Controllers/SKPlayerController.h"


ASKGameModeMain::ASKGameModeMain()
{

    DefaultPawnClass = ASKPlayerCharacter::StaticClass();
    PlayerControllerClass =  ASKPlayerController::StaticClass();

}


