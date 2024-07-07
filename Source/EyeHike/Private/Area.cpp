#include "Area.h"

#include "BitMapLoadingStep.h"
#include "GroundTruthLoadingStep.h"
#include "LoadingStep.h"
#include "MatchesLoadingStep.h"
#include "QueriesLoadingStep.h"

#include <algorithm>

AArea::AArea()
{
	PrimaryActorTick.bCanEverTick = true;
	m_StepsToComponentsMap[eLoadingStep::Download]		= {eLoadingComponents::GroundTruth, eLoadingComponents::Tracks, eLoadingComponents::HeightMap};
	m_StepsToComponentsMap[eLoadingStep::Match]			= {eLoadingComponents::Match};
	m_StepsToComponentsMap[eLoadingStep::Query]			= {eLoadingComponents::Query};
	m_StepsToComponentsMap[eLoadingStep::Visualization]	= {eLoadingComponents::Terrain, eLoadingComponents::BitMap};
	m_CurrentLoadingStep = eLoadingStep::Unloaded;

	m_DataDirectoyPath = {}; 
}

void AArea::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Area Begins To Load."));

	m_CurrentLoadingStep = eLoadingStep::Unloaded;
	auto* pGroundTruthComponent	{Cast<ILoadingStep>(GetComponentByClass(UGroundTruthLoadingStep::StaticClass()))};
	auto* pMatchComponent		{Cast<ILoadingStep>(GetComponentByClass(UMatchesLoadingStep::StaticClass()))};
	auto* pQueriesComponent		{Cast<ILoadingStep>(GetComponentByClass(UQueriesLoadingStep::StaticClass()))};
	auto* pBitMapComponent		{Cast<ILoadingStep>(GetComponentByClass(UBitMapLoadingStep::StaticClass()))};

	m_ComponentsVector[eLoadingComponents::Tracks]		= nullptr; 
	m_ComponentsVector[eLoadingComponents::GroundTruth]	= pGroundTruthComponent;
	m_ComponentsVector[eLoadingComponents::HeightMap]	= nullptr; 
	m_ComponentsVector[eLoadingComponents::Match]		= pMatchComponent;
	m_ComponentsVector[eLoadingComponents::Query]		= pQueriesComponent;
	m_ComponentsVector[eLoadingComponents::BitMap]		= pBitMapComponent;
	m_ComponentsVector[eLoadingComponents::Terrain]		= nullptr; 

	FString ProjectContentDirectory = FPaths::ProjectContentDir();
	m_DataDirectoyPath = ProjectContentDirectory + "EyeHike/Data/" + u_AreaName + "/";

	// TODO: CHANGE THIS SO IT IS CALLED FROM AREA MANAGER. 
	LoadFrom(eLoadingStep::Match); 
}

void AArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_CurrentLoadingStep != eLoadingStep::Loaded) 
	{
		const bool FinishedCurrentLoadingStep{
			std::all_of(m_StepsToComponentsMap[m_CurrentLoadingStep].begin(), m_StepsToComponentsMap[m_CurrentLoadingStep].end(),
				[&m_ComponentsVector = m_ComponentsVector](const eLoadingComponents aLoadingComponentType)
				{
					const auto* pLoadingComponent {m_ComponentsVector.at(aLoadingComponentType)};
					return pLoadingComponent == nullptr ? true : pLoadingComponent->LoadingFinished();
				}
			)
		};

		if (FinishedCurrentLoadingStep)
		{
			const eLoadingStep NextLoadingStep{static_cast<eLoadingStep>(static_cast<uint32> (m_CurrentLoadingStep) + 1U)};

			if (NextLoadingStep != eLoadingStep::Loaded)
			{
				LaunchLoadingStep(NextLoadingStep);
			}
			else if (NextLoadingStep == eLoadingStep::Loaded)
			{
				m_CurrentLoadingStep = eLoadingStep::Loaded;
				UE_LOG(LogTemp, Warning, TEXT("Area has finished loading."));
				// TODO: Notify area manager that loading is finished.
			}
		}
	}
}

void AArea::LoadFrom(eLoadingStep aInitialLoadingStep) 
{
	KillLoadProcesses();


	if (aInitialLoadingStep == eLoadingStep::Download) 
	{
		LaunchLoadingStep(eLoadingStep::Download);
	}
	else 
	{
		const eLoadingStep PreviousLoadingStep{static_cast<eLoadingStep>(static_cast<uint32> (aInitialLoadingStep) - 1U)};
		LaunchLoadFromFile(PreviousLoadingStep);
	}
}

void AArea::LaunchLoadingStep(eLoadingStep aLoadingStep)
{
	UE_LOG(LogTemp, Warning, TEXT("[AREA] - Launching loading step %s"), *LoadStepToString(aLoadingStep));

	m_CurrentLoadingStep = aLoadingStep;

	for (const auto LoadingStepType : m_StepsToComponentsMap[aLoadingStep])
	{
		auto * pLoadingComponent{m_ComponentsVector.at(LoadingStepType)};
		if (pLoadingComponent != nullptr)
		{
			pLoadingComponent->LaunchLoadThread(); 
		}
	}
}

void AArea::LaunchLoadFromFile(eLoadingStep aLoadingStep)
{
	UE_LOG(LogTemp, Warning, TEXT("[AREA] - Launching loading from file step %s"), *LoadStepToString(aLoadingStep));

	m_CurrentLoadingStep = aLoadingStep;

	for (const auto LoadingStepType : m_StepsToComponentsMap[aLoadingStep])
	{
		auto* pLoadingComponent{ m_ComponentsVector.at(LoadingStepType) };
		if (pLoadingComponent != nullptr)
		{
			pLoadingComponent->LoadDataFromFiles();
		}
	}
}

void AArea::KillLoadProcesses() 
{
	for (auto [_, pComponent] : m_ComponentsVector) 
	{
		if (pComponent != nullptr && !pComponent->LoadingFinished())
		{
			pComponent->StopLoadThread();
		}
	}
}

FString AArea::LoadStepToString(eLoadingStep aLoadingStep)
{
	switch (aLoadingStep)
	{
		case eLoadingStep::Download: 
		{
			return "Download";
			break;
		}
		case eLoadingStep::Match:
		{
			return "Match";
			break;
		}
		case eLoadingStep::Query:
		{
			return "Query";
			break;
		}
		case eLoadingStep::Visualization:
		{
			return "Visualization";
			break;
		}
		case eLoadingStep::Loaded:
		{
			return "Loaded";
			break;
		}
		default:
		{
			return "";
			break;
		}
	}
}

FString AArea::GetAreaFilesPath()
{
	return m_DataDirectoyPath;
}