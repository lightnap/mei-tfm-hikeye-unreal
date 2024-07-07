#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LoadingStep.h"
#include "GroundTruthLoadingStep.h"
#include "QueriesLoadingStep.generated.h"

/**
 * @brief Loading step that queries matches to enrichen ground truth info.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EYEHIKE_API UQueriesLoadingStep : public USceneComponent, public ILoadingStep
{
	GENERATED_BODY()

public:	
	/**
	* @brief Constructor. Sets default values for this component's properties.
	*/
	UQueriesLoadingStep();

protected:
	/**
	 * @brief Called when the game starts.
	*/
	virtual void BeginPlay() override;

public:	

	/**
	 * @brief Struct whith the info to paint a line between to points in the bitmap.
	*/
	struct sSegmentVisualInfo
	{
		FColor Color {}; //!< Color of the line. 
		float Width {1.0f}; //!< Width of the line, in some unit. 

		sSegmentVisualInfo(FColor aColor, uint64_t aWidth) :
			Color(aColor),
			Width(aWidth)
		{}

		sSegmentVisualInfo() :
			Color(255U,0U,0U,255U), // TODO: Move default color into variable. 
			Width(1.0f)
		{}
	};

	using tTrackVisualInfo = std::vector<sSegmentVisualInfo>; //!< Type to save the information to draw a OSM way. 
	using tGroundTruthVisualInfo = std::vector<tTrackVisualInfo>; //!< Type to save the information to draw the ground truth. 

	/**
	 * @brief Struct to represent a UTMT Point. 
	*/
	struct sUTMPoint 
	{
		double Easting {0.0};	//!< Meters from central meridian of zone. 
		double Northing {0.0};	//!< Meters from ecuator.
		uint64 Id {0UL};		//!< OSMref (Id) of the point. 
		// FName GridZone //!< Grid zone. Ommited for now, assumed to all have the same one. 

		/**
		 * @brief Constructor.
		*/
		sUTMPoint(const double aEasting, const double aNorthing, const double aId)
			: Easting(aEasting)
			, Northing(aNorthing)
			, Id(aId)
		{};

		/**
		 * @brief Move contructor.
		*/
		sUTMPoint(sUTMPoint&& aOther)
			: Easting(aOther.Easting)
			, Northing(aOther.Northing)
			, Id(aOther.Id)
		{};

		/**
		 * @brief Move assignment operator. 
		*/
		sUTMPoint& operator=(sUTMPoint& aOther)
		{
			Easting = aOther.Easting;
			Northing = aOther.Northing;
			Id = aOther.Id;
		};
	};

	using tUTMTrack = std::vector<sUTMPoint>;		//!< Type to save the information relating to a linear tree of points in UTM coordinates. 
	using tUTMGroundTruth = std::vector<tUTMTrack>;	//!< Type to save all the ground truth information, in UTM coordinates. 
	tUTMGroundTruth m_ProcessedGroundTruth {};		//!< Contains ground truth information, in UTM coordinates. 

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

	/**
	 * @brief Returns the queries info to paint the bit map. 
	 * @return Thethe queries info.
	*/
	tGroundTruthVisualInfo& GetQueriesInfo();

private:

	/**
	 * @brief Makes all preparations needed to query the data we obtained from the matches.
	*/
	void PreProcessQueries();

	/**
	 * @brief Creates universal constants needed for UMT coordinate conversion. 
	*/
	void CreateUTMConstants();

	/**
	 * @brief Converts ground truth data from Lon Lat to UMT coordinates. 
	*/
	void ConvertGroundTruthToUTM();

	/**
	 * @brief Transforms a point from Lon/Lat coordinates to UTM coordinates. 
	 * @param aOSMPoint Point in Lon/Lat cordinates. 
	 * @return Reference (so that we may move it) to the point transformed to UTM coordinates. 
	*/
	sUTMPoint ConvertPointToUTM(const UGroundTruthLoadingStep::sOSMPoint& aOSMPoint);

	/**
	 * @brief Generates queries based on configuration. 
	*/
	void GenerateQueries();

private:

	tGroundTruthVisualInfo m_GroundTruthVisualInfo {}; //!< Information on how to paint the segments relating to the ground truth. Index of ways and points is same as in ground truth.
};
