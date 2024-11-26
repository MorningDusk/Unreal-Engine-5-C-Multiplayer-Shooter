#pragma once

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_ThrowingGreande UMETA(DisplayName = "Throwing Greande"),
	ECS_SwitchingWeapons UMETA(DisplayName = "Switching Weapons"),
	
	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};