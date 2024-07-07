#include "QueriesLoadingStep.h"

#include "GroundTruthLoadingStep.h"

UQueriesLoadingStep::UQueriesLoadingStep()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame. 
	// Turn these features off to improve performance if not needed.
	PrimaryComponentTick.bCanEverTick = true;
}

void UQueriesLoadingStep::BeginPlay()
{
	Super::BeginPlay();
}

void UQueriesLoadingStep::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UQueriesLoadingStep::LoadDataFromFiles()
{
	UE_LOG(LogTemp, Warning, TEXT("Loading generated queries from file."));
	// Reads queriy results from file, from a previous generated result (parse xml). 
}

void UQueriesLoadingStep::LaunchLoadThread()
{
	UE_LOG(LogTemp, Warning, TEXT("Generating queries."));

	PreProcessQueries();
	GenerateQueries(); 

	m_LoadingFinished = true;
}

void UQueriesLoadingStep::PreProcessQueries()
{
	CreateUTMConstants();
	ConvertGroundTruthToUTM(); 
}

void UQueriesLoadingStep::CreateUTMConstants()
{

}

void UQueriesLoadingStep::ConvertGroundTruthToUTM()
{
	const auto& GroundTruthComponent {GetOwner()->GetComponentByClass(UGroundTruthLoadingStep::StaticClass())};
	const auto& GroundTruth {Cast<UGroundTruthLoadingStep>(GroundTruthComponent)->GetGroundTruth()};
	

	// WARNING: We are assuming that parsed ground truth (in lon/lat) and processed ground truth (in UTM) will have same tracks ordered in the same way. 
	// Maybe we should change it to unordered map? Nope! Tracks must be traversed in order, or each point ought to have a reference to the next... 
	for (uint32 WayIndex {0U}; WayIndex < GroundTruth.size(); WayIndex++) 
	{
		const auto& CurrentWay {GroundTruth.at(WayIndex)};
		checkf(CurrentWay.size() > 0U, TEXT("FOUND EMPTY WAY WHILE CONVERTING TO UMT"));

		m_ProcessedGroundTruth.emplace_back(tUTMTrack());

		for (uint32 PointIndex {0U}; PointIndex < CurrentWay.size(); PointIndex++)
		{
			const auto& CurrentPoint {CurrentWay.at(PointIndex)};
			m_ProcessedGroundTruth.at(WayIndex).emplace_back(ConvertPointToUTM(CurrentPoint));
		}
	}
}

UQueriesLoadingStep::sUTMPoint UQueriesLoadingStep::ConvertPointToUTM(const UGroundTruthLoadingStep::sOSMPoint& aOSMPoint)
{
	double Northing	{aOSMPoint.Longitude};
	double Easting	{aOSMPoint.Latitude};

	return sUTMPoint(Easting, Northing, aOSMPoint.Id);
}


void UQueriesLoadingStep::GenerateQueries() 
{
	for (const auto& Way : m_ProcessedGroundTruth)
	{
		checkf(Way.size() > 0U, TEXT("FOUND EMPTY WAY WHILE GENERATING QUERIES"));
		const size_t LinesInWay {Way.size() - 1U};
		m_GroundTruthVisualInfo.emplace_back(tTrackVisualInfo(LinesInWay, sSegmentVisualInfo()));
	}
}

void UQueriesLoadingStep::StopLoadThread()
{
	// Kills process that is generating queries. 
}

UQueriesLoadingStep::tGroundTruthVisualInfo& UQueriesLoadingStep::GetQueriesInfo()
{
	return m_GroundTruthVisualInfo;
}