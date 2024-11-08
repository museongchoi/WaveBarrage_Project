
#include "WBMonsterSpawnerComponent.h"


UWBMonsterSpawnerComponent::UWBMonsterSpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}


void UWBMonsterSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void UWBMonsterSpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

