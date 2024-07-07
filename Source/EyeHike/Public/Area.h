#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <map>


#include "Area.generated.h"

class ILoadingStep;

/**
 * Class that represents a given Geographical area. 
*/
UCLASS()
class EYEHIKE_API AArea : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * @brief Constructor.
	*/
	AArea();

	/**
	* @brief Called every frame.
	* @param DeltaTime.
	*/
	virtual void Tick(float DeltaTime) override;

	/**
	* Enum defining all loading steps we need to get trhough before being able to load.
	*/
	enum class eLoadingStep
	{
		Unloaded = 0,	//!< Area is unloaded and unactive. 
		Download,		//!< Dowload tracks, ground truth, height map.
		Match,			//!< Match tracks to groundtruth.
		Query,			//!< Query matched to enrichen ground truth.
		Visualization,	//!< Create visualization
		Loaded,			//!< Loading has finished.
		Total			//!< Total number of items in this enum. 
	};

	/**
	* Enum defining all loading components which perform some loading.
	*/
	enum class eLoadingComponents
	{
		Tracks = 0,		//!< Downloads the tracks from the internet.
		GroundTruth,	//!< Downloads ground truth from the internet.
		HeightMap,		//!< Downloads height map from the internet.
		Match,			//!< Runs the matching algorithm.
		Query,			//!< Populates ground truth with extra information. 
		BitMap,			//!< Turns queries tracks into texture.
		Terrain,		//!< Creates 3d terrain and applies texture.
		Total			//!< Total number of items in this enum. 
	};

	/**
	* @brief Loads area from the given LoadingStep. 
	* @param aInitialLoadingStep Idicates from which state one must start loading. 
	*/
	void LoadFrom(eLoadingStep aInitialLoadingStep);

	/**
	 * @brief Stops the loading process leaving the area at unload.
	*/
	void KillLoadProcesses();

	/**
	 * @brief Returns the path of where the area files are saved.
	 * @return The path of where the area files are saved.
	*/
	FString GetAreaFilesPath();

	UPROPERTY(EditAnywhere, Category="AreaConfig") 
		FString u_AreaName {}; //!< Indicates the name of the area. 

protected:
	/**
	 * @brief Called when the game starts or when spawned.
	*/
	virtual void BeginPlay() override;

private:
	/**
	 * @brief Launches the loading process of the components related to the current loading step. 
	 * @param aLoadingStep the loading step to launch.
	*/
	void LaunchLoadingStep(eLoadingStep aLoadingStep);

	/**
	 * @brief Launches the reading data from file process of the components related to the current loading step.
	 * @param aLoadingStep the loading step to launch.
	*/
	void LaunchLoadFromFile(eLoadingStep aLoadingStep);

	/**
	 * @brief Turns the eLoadingStep enum type into a string. 
	 * @param aLoadingStep The eLoadingStep enim.
	 * @return The string.
	*/
	FString LoadStepToString(eLoadingStep aLoadingStep);

private:
	using tComponentsMap		= std::map<eLoadingComponents, ILoadingStep*>;	//!< Vector of loading step components. 
	using tComponentEnumVector	= std::vector<eLoadingComponents>;				//!< Vector of loading step enums. 
	using tStepsToComponents	= std::map<eLoadingStep, tComponentEnumVector>;	//!< Type relating loading steps to loading components. 

	FString m_DataDirectoyPath; //!< Path of all the data associated with this area.  

	eLoadingStep		m_CurrentLoadingStep;	//!< Indicates the current loading step.
	tComponentsMap		m_ComponentsVector;		//!< Vector holding pointers to all Loading steps components of the current area. 
	tStepsToComponents	m_StepsToComponentsMap;	//!< Map which tells us which loading components are uses for which loading steps. 
};
