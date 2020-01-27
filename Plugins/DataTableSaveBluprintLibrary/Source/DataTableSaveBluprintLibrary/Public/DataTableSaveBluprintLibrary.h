/* Copyright (C) 2020 Wuguyannian - All Rights Reserved
 * This plugin is downloadable from the UE4 Marketplace
 */

#pragma once

#include "Modules/ModuleManager.h"

class FDataTableSaveBluprintLibraryModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
