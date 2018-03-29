// Fill out your copyright notice in the Description page of Project Settings.

#include "MinesweeperPlayerController.h"

AMinesweeperPlayerController::AMinesweeperPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = false;
	ClickEventKeys.Add(EKeys::RightMouseButton);
}
