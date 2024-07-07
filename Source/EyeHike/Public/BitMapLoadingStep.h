#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LoadingStep.h"
#include "Engine/TextureRenderTarget2D.h"
#include "LandscapeProxy.h"
#include "BitMapLoadingStep.generated.h"

/**
 * @brief Loading step that generates bit map textrue from queries.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EYEHIKE_API UBitMapLoadingStep : public USceneComponent, public ILoadingStep
{
	GENERATED_BODY()

public:	
	/**
	* @brief Constructor. Sets default values for this component's properties.
	*/
	UBitMapLoadingStep();

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
	* Loads from files data corresponding to current step generated in a previous loading procedure.
	*/
	void LoadDataFromFiles() override;

	/**
	* Launches Thread that will perform the current generation step.
	*/
	void LaunchLoadThread() override;

	/**
	* Kills current load process.
	*/
	void StopLoadThread() override;

	UPROPERTY(EditAnywhere, Category = "Materials")
		UTextureRenderTarget2D* up_RenderTarget {nullptr};

	UPROPERTY(EditAnywhere, Category = "Meshes")
		ALandscapeProxy* up_TerrainActor {nullptr};
};
