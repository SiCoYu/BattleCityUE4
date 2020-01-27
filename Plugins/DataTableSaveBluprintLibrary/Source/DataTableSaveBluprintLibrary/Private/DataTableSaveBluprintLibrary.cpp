/* Copyright (C) 2020 Wuguyannian - All Rights Reserved
 * This plugin is downloadable from the UE4 Marketplace
 */

#include "DataTableSaveBluprintLibrary.h"

#define LOCTEXT_NAMESPACE "FDataTableSaveBluprintLibraryModule"

void FDataTableSaveBluprintLibraryModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FDataTableSaveBluprintLibraryModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDataTableSaveBluprintLibraryModule, DataTableSaveBluprintLibrary)