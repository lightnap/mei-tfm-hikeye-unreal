#include "GroundTruthLoadingStep.h"

#include "Area.h"
#include "XmlFile.h"
#include "XmlNode.h"

UGroundTruthLoadingStep::UGroundTruthLoadingStep()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame. 
	// Turn these features off to improve performance if not needed.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGroundTruthLoadingStep::BeginPlay()
{
	Super::BeginPlay();
}

void UGroundTruthLoadingStep::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGroundTruthLoadingStep::LoadDataFromFiles()
{
	UE_LOG(LogTemp, Warning, TEXT("Loading ground truth from file."));

	FString DataPath {Cast<AArea>(GetOwner())->GetAreaFilesPath()};
	FString GroundTruthFile {DataPath + "GroundTruth/GroundTruth.gpx"};

	FXmlFile* File = new FXmlFile(GroundTruthFile);
	FXmlNode* RootNode {File->GetRootNode()};
	TArray<FXmlNode*> WayNodes {RootNode->GetChildrenNodes()};

	// TODO: Maybe reserve memory in advance?
	// TODO: Maybe saving osm track id is important (like will be needed in matching?) 

	uint32_t WayIndex {0U};

	for (FXmlNode* Way : WayNodes)
	{
		if (Way->GetTag() == "way")
		{
			m_GroundTruth.emplace_back(tOSMTrack{});

			for (FXmlNode* Point : Way->GetChildrenNodes())
			{
				if (Point->GetTag() == "nd")
				{
					double Longitude {FCString::Atod(*(Point->GetAttribute("lon")))};
					double Latitude  {FCString::Atod(*(Point->GetAttribute("lat")))};
					uint64 Reference {FCString::Strtoui64(*(Point->GetAttribute("ref")), NULL, 10)};
					m_GroundTruth.at(WayIndex).emplace_back(Longitude, Latitude, Reference);
				}
			}

			WayIndex++;
		}
	}

	m_LoadingFinished = true;
}

UGroundTruthLoadingStep::tGroundTruth& UGroundTruthLoadingStep::GetGroundTruth()
{
	return m_GroundTruth;
}
