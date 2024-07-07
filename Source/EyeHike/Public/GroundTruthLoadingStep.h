#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "LoadingStep.h"

#include "GroundTruthLoadingStep.generated.h"

/**
 * @brief Loading step that obtains the ground truth.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EYEHIKE_API UGroundTruthLoadingStep : public USceneComponent, public ILoadingStep
{
	GENERATED_BODY()

public:	
	/**
	* @brief Constructor. Sets default values for this component's properties.
	*/
	UGroundTruthLoadingStep();

protected:
	/**
	 * @brief Called when the game starts.
	*/
	virtual void BeginPlay() override;

public:	
	/**
	 * @brief Struct which represents an OSM Point.
	*/
	struct sOSMPoint
	{
		double Longitude {0.0};	//!< Longitude of the point. 
		double Latitude {0.0};	//!< Latitude of the point. 
		uint64 Id {0LU};		//!< OSMref (Id) of the point. 

		sOSMPoint(double aLongitude, double aLatitude, uint64_t aId) :
			Longitude(aLongitude),
			Latitude(aLatitude),
			Id(aId)
		{}
	};

	using tOSMTrack = std::vector<sOSMPoint>; //!< Type to save the information relating to a OSM way.
	using tGroundTruth = std::vector<tOSMTrack>; //!< Type to save all the ground truth information, as parsed from the xml.

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
	 * @brief Returns the ground truth info.
	 * @return The ground truth info. 
	*/
	tGroundTruth& GetGroundTruth();

private: 
	tGroundTruth m_GroundTruth {}; //!< The ground truth.
};
