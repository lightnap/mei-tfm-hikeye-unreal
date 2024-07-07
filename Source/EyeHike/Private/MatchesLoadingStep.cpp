#include "MatchesLoadingStep.h"

UMatchesLoadingStep::UMatchesLoadingStep()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame. 
	// Turn these features off to improve performance if not needed.
	PrimaryComponentTick.bCanEverTick = true;
}

void UMatchesLoadingStep::BeginPlay()
{
	Super::BeginPlay();
}

void UMatchesLoadingStep::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMatchesLoadingStep::LoadDataFromFiles() 
{
	UE_LOG(LogTemp, Warning, TEXT("Loading matches from file."));
	m_LoadingFinished = true;
	// Load matches from files: parse several xml. 
}

void UMatchesLoadingStep::LaunchLoadThread()
{
	// TODO: Make the matching correctly and not just loading from file. 
	LoadDataFromFiles(); 
}