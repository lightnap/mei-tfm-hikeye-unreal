#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LoadingStep.generated.h"

/**
* This class does not need to be modified and only exists for reflection purposes.
*/
UINTERFACE(MinimalAPI)
class ULoadingStep : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface which implements functionality about a step of the loading process.
 * Add interface functions to this class. This is the class that will be inherited to implement this interface.
 */
class EYEHIKE_API ILoadingStep
{
	GENERATED_BODY()

public:
	/**
	* @brief Loads from files data corresponding to current step generated in a previous loading procedure.
	*/
	virtual void LoadDataFromFiles(); 

	/**
	* @brief Launches Thread that will perform the current generation step. 
	*/
	virtual void LaunchLoadThread();

	/**
	*  @brief Kills current load process. 
	*/
	virtual void StopLoadThread();

	/**
	 * @brief Tells whether this particular loading step has finished. 
	 * @return True if loading has finished, false if not. 
	*/
	virtual bool LoadingFinished() const;

protected:
	bool m_LoadingFinished {true}; //!< Indicates if the loading step has finished. 
};