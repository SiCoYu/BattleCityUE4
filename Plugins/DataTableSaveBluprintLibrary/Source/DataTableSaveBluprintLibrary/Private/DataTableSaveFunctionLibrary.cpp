// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTableSaveFunctionLibrary.h"
#include "Engine/Engine.h"
#include "Misc/MessageDialog.h"
#include "Misc/Paths.h"
#include "Misc/PackageName.h"

UDataTable* UDataTableSaveFunctionLibrary::GetDataTable(UObject* WorldContextObject, FString TablePath)
{
	return LoadObject<UDataTable>(nullptr, *TablePath, nullptr, LOAD_None, nullptr);
}

void UDataTableSaveFunctionLibrary::StartFilling(UDataTable* Table, bool ClearAll)
{
	if (ClearAll)
	{
		Table->EmptyTable();
	}
}

void UDataTableSaveFunctionLibrary::EndFilling(UDataTable* Table)
{
	Table->Modify(true);
	Table->OnDataTableImported().Broadcast();
	Table->OnDataTableChanged().Broadcast();
	// Place were we should save the file, including the filename
	UPackage* PackageToSave = Table->GetOutermost();
	// The name of the package
	const FString PackageName = PackageToSave->GetName();
	FString FinalPackageSavePath;
	FString FinalPackageFilename;
	FString BaseFilename, Extension, Directory;
	FString ExistingFilename;
	FPackageName::DoesPackageExist(PackageName, NULL, &ExistingFilename);
	// Split the path to get the filename without the directory structure
	FPaths::NormalizeFilename(ExistingFilename);
	FPaths::Split(ExistingFilename, Directory, BaseFilename, Extension);
	// The final save path is whatever the existing filename is
	FinalPackageSavePath = ExistingFilename;
	FinalPackageFilename = FString::Printf(TEXT("%s.%s"), *BaseFilename, *Extension);
	
	bool bWasSuccessful = GEngine->Exec(NULL, *FString::Printf(TEXT("OBJ SAVEPACKAGE PACKAGE=\"%s\" FILE=\"%s\" SILENT=true"), *PackageName, *FinalPackageSavePath));
	FText FailureReason;
	
	if (!bWasSuccessful)
	{
		FailureReason = FText::Format(NSLOCTEXT("UnrealEd", "SaveAssetFailed", "The asset '{0}' ({1}) failed to save."), FText::FromString(PackageName), FText::FromString(FinalPackageFilename));		
	}
	else
	{
		FailureReason = FText::Format(NSLOCTEXT("UnrealEd", "SaveAssetSuccessed", "The asset '{0}' ({1}) has been saved."), FText::FromString(PackageName), FText::FromString(FinalPackageFilename));
	}
	FMessageDialog::Open(EAppMsgType::Ok, FailureReason);
}

void UDataTableSaveFunctionLibrary::RemoveDataTableRowFromName(UDataTable* Table, FName RowName)
{
	Table->RemoveRow(RowName);
}

bool UDataTableSaveFunctionLibrary::SaveDataTableRowFromName(UDataTable* Table, FName RowName, const FGenericStruct & Value)
{
	// We should never hit this!  stubs to avoid NoExport on the class.
	check(0);
	return false;
}

bool UDataTableSaveFunctionLibrary::Generic_SaveDataTableRowFromName(UDataTable* Table, FName RowName, void* ValuePtr)
{
	bool bSaved = false;

	if (ValuePtr && Table)
	{
		void* RowPtr = Table->FindRowUnchecked(RowName);

		if (RowPtr != nullptr)
		{
			const UScriptStruct* StructType = Table->GetRowStruct();

			if (StructType != nullptr)
			{
				StructType->CopyScriptStruct(RowPtr, ValuePtr);
				bSaved = true;
			}
		}
		else
		{
			FTableRowBase* RowSrc = (FTableRowBase*)ValuePtr;
			Table->AddRow(RowName, *RowSrc);
			bSaved = true;
		}
	}

	return bSaved;
}

DEFINE_FUNCTION(UDataTableSaveFunctionLibrary::execSaveDataTableRowFromName)
{
	P_GET_OBJECT(UDataTable, Table);
	P_GET_PROPERTY(UNameProperty, RowName);

	Stack.StepCompiledIn<UStructProperty>(NULL);
	UStructProperty* StructProp = Cast<UStructProperty>(Stack.MostRecentProperty);
	void* ValuePtr = Stack.MostRecentPropertyAddress;

	P_FINISH;
	bool bSuccess = false;

	if (!Table)
	{
		FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			NSLOCTEXT("SaveDataTableRowFromName", "MissingTableInput", "Failed to resolve the table input. Be sure the DataTable is valid.")
		);
		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}
	else if (StructProp && ValuePtr)
	{
		UScriptStruct* InputType = StructProp->Struct;
		const UScriptStruct* TableType = Table->GetRowStruct();

		const bool bCompatible = (InputType == TableType) ||
			(InputType->IsChildOf(TableType) && FStructUtils::TheSameLayout(InputType, TableType));
		if (bCompatible)
		{
			P_NATIVE_BEGIN;
			bSuccess = Generic_SaveDataTableRowFromName(Table, RowName, ValuePtr);
			P_NATIVE_END;
		}
		else
		{
			FBlueprintExceptionInfo ExceptionInfo(
				EBlueprintExceptionType::AccessViolation,
				NSLOCTEXT("SaveDataTableRowFromName", "IncompatibleProperty", "Incompatible output parameter; the data table's type is not the same as the input type.")
			);
			FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		}
	}
	else
	{
		FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			NSLOCTEXT("SaveDataTableRowFromName", "MissingInputProperty", "Failed to resolve the input parameter for SaveDataTableRowFromName.")
		);
		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);

	}
	*(bool*)RESULT_PARAM = bSuccess;
}