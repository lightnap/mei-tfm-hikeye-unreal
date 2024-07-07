#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LoadingStep.h"
#include "MatchesLoadingStep.generated.h"

/**
 * @brief Loading step that matches the tracks.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EYEHIKE_API UMatchesLoadingStep : public USceneComponent, public ILoadingStep
{
	GENERATED_BODY()

public:	
	/**
	* @brief Constructor. Sets default values for this component's properties.
	*/
	UMatchesLoadingStep();

protected:
	/**
	 * @brief Called when the game starts.
	*/
	virtual void BeginPlay() override;

public:	
	/**
	 * @brief Called every frame.
	 * @param DeltaTime
	 * @param TickType
	 * @param ThisTickFunction
	*/
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* @brief Loads from files data corresponding to current step generated in a previous loading procedure.
	*/
	void LoadDataFromFiles() override; 

	/**
	* @brief Launches Thread that will perform the current generation step.
	*/
	void LaunchLoadThread() override;
};
