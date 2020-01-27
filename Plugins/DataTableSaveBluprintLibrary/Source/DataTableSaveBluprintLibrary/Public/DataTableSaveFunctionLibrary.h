/* Copyright (C) 2020 Wuguyannian - All Rights Reserved
 * This plugin is downloadable from the UE4 Marketplace
 */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DataTable.h"
#include "DataTableSaveFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DATATABLESAVEBLUPRINTLIBRARY_API UDataTableSaveFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/** Get a DataTable given a Path */
	UFUNCTION(BlueprintCallable, Category = "DataTable", meta = (WorldContext = "WorldContextObject"))
	static UDataTable* GetDataTable(UObject* WorldContextObject, FString TablePath);

	/** Clear the DataTable given */
	UFUNCTION(BlueprintCallable, Category = "DataTable")
	static void ClearDataTable(UDataTable* Table);

	/** Save the DataTable given */
	UFUNCTION(BlueprintCallable, Category = "DataTable")
	static void SaveDataTable(UDataTable* Table);

	/** Remove a Row from a DataTable given a RowName */
	UFUNCTION(BlueprintCallable, Category = "DataTable")
	static void RemoveDataTableRowFromName(UDataTable* Table, FName RowName);

	/** Save a Row to a DataTable given a RowName */
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "DataTable", meta = (CustomStructureParam = "Value", AutoCreateRefTerm = "Value"))
	static bool SaveDataTableRowFromName(UDataTable* Table, FName RowName, const FGenericStruct& Value);

	static bool Generic_SaveDataTableRowFromName(UDataTable* Table, FName RowName, void* ValuePtr);	
	
	/** Based on UDataTableSaveFunctionLibrary::SaveDataTableRowFromName */
	DECLARE_FUNCTION(execSaveDataTableRowFromName);
};
