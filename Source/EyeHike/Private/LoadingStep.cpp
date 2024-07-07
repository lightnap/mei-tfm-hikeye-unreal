// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingStep.h"

// Add default functionality here for any ILoadingStep functions that are not pure virtual.
void ILoadingStep::LoadDataFromFiles() {}

void ILoadingStep::LaunchLoadThread() {}

void ILoadingStep::StopLoadThread() {}

bool ILoadingStep::LoadingFinished() const
{
	return m_LoadingFinished;
}