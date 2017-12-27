#pragma once

#include "Object.h"
#include "FlareSkirmishManager.generated.h"


class AFlareGame;
struct FFlareSpacecraftDescription;


/** Skirmish phase state */
UENUM()
namespace EFlareSkirmishPhase
{
	enum Type
	{
		Idle,
		Setup,
		Play,
		End
	};
}

/** Skirmish belligerent */
USTRUCT()
struct FFlareSkirmishPlayerData
{
	GENERATED_USTRUCT_BODY()

	// Fleet building
	uint32                                           AllowedValue;
	TArray<FFlareSkirmishSpacecraftOrder>            OrderedSpacecrafts;

	// Defaults
	FFlareSkirmishPlayerData()
		: AllowedValue(0)
	{
		OrderedSpacecrafts.Empty();
	}
};


/** Skirmish setup data */
USTRUCT()
struct FFlareSkirmishData
{
	GENERATED_USTRUCT_BODY()
	
	// Player setup
	FFlareSkirmishPlayerData                         Player;
	FFlareCompanyDescription                         PlayerCompanyData;

	// Enemy setup
	FFlareSkirmishPlayerData                         Enemy;
	FName                                            EnemyCompanyName;

	// Defaults
	FFlareSkirmishData()
		: Player()
		, Enemy()
		, EnemyCompanyName(NAME_None)
	{
	}
};


/** Skirmish result data */
USTRUCT()
struct FFlareSkirmishPlayerResult
{
	GENERATED_USTRUCT_BODY()

	// General data
	int32                                            ShipsDisabled;
	int32                                            ShipsDestroyed;
	int32                                            AmmoFired;
	int32                                            AmmoHit;

	// Defaults
	FFlareSkirmishPlayerResult()
		: ShipsDisabled(0)
		, ShipsDestroyed(0)
		, AmmoFired(0)
		, AmmoHit(0)
	{
	}
};

/** Skirmish result data */
USTRUCT()
struct FFlareSkirmishResultData
{
	GENERATED_USTRUCT_BODY()

	// General data
	bool                                             PlayerVictory;
	float                                            GameTime;

	// Player data
	FFlareSkirmishPlayerResult                       Player;
	FFlareSkirmishPlayerResult                       Enemy;

	// Defaults
	FFlareSkirmishResultData()
		: PlayerVictory(false)
		, GameTime(0)
		, Player()
		, Enemy()
	{
	}
};

/** Skirmish managing class */
UCLASS()
class HELIUMRAIN_API UFlareSkirmishManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	/*----------------------------------------------------
		Gameplay phases
	----------------------------------------------------*/

	/** Update the manager */
	void Update(float DeltaSeconds);
	
	/** Setup the setup phase */
	void StartSetup();

	/** Setup the playing phase */
	void StartPlay();

	/** Start the endgame phase */
	void EndPlay();

	/** End the game */
	void EndSkirmish();

	/** Are we playing skirmish */
	bool IsPlaying() const;


	/*----------------------------------------------------
		Setup
	----------------------------------------------------*/
	
	/** Set the combat value allowed */
	void SetAllowedValue(bool ForPlayer, uint32 Budget);

	/** Get the current combat value */
	inline uint32 GetCurrentCombatValue(bool ForPlayer) const;

	/** Add a ship */
	void AddShip(bool ForPlayer, FFlareSkirmishSpacecraftOrder Desc);


	/*----------------------------------------------------
		Scoring
	----------------------------------------------------*/

	void ShipDisabled(bool ForPlayer);

	void ShipDestroyed(bool ForPlayer);

	void AmmoFired(bool ForPlayer);

	void AmmoHit(bool ForPlayer);


protected:

	/*----------------------------------------------------
		Data
	----------------------------------------------------*/

	// Skirmish data
	TEnumAsByte<EFlareSkirmishPhase::Type>           CurrentPhase;

	// Skirmish data
	FFlareSkirmishData                               Data;
	FFlareSkirmishResultData                         Result;


public:

	/*----------------------------------------------------
		Getters
	----------------------------------------------------*/

	AFlareGame* GetGame() const;

	inline FFlareSkirmishData& GetData()
	{
		return Data;
	}

	inline const FFlareSkirmishResultData& GetResult() const
	{
		return Result;
	}

	inline uint32 GetAllowedCombatValue(bool ForPlayer) const
	{
		return ForPlayer ? Data.Player.AllowedValue : Data.Enemy.AllowedValue;
	}

};
