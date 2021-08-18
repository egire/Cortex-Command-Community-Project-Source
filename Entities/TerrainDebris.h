#ifndef _RTETERRAINDEBRIS_
#define _RTETERRAINDEBRIS_

#include "Entity.h"
#include "Material.h"

namespace RTE {

	class Box;
	class SLTerrain;

	/// <summary>
	/// Specifies a certain type of debris scattered randomly throughout a Terrain, before TerrainObjects are placed.
	/// </summary>
	class TerrainDebris : public Entity {

	public:

		EntityAllocation(TerrainDebris)
		SerializableOverrideMethods
		ClassInfoGetters

#pragma region Creation
		/// <summary>
		/// Constructor method used to instantiate a TerrainDebris object in system memory. Create() should be called before using the object.
		/// </summary>
		TerrainDebris() { Clear(); }

		/// <summary>
		/// Makes the TerrainDebris object ready for use.
		/// </summary>
		/// <returns>An error return value signaling success or any particular failure. Anything below 0 is an error signal.</returns>
		int Create() override;

		/// <summary>
		/// Creates a TerrainDebris to be identical to another, by deep copy.
		/// </summary>
		/// <param name="reference">A reference to the TerrainDebris to deep copy.</param>
		/// <returns>An error return value signaling success or any particular failure. Anything below 0 is an error signal.</returns>
		int Create(const TerrainDebris &reference);
#pragma endregion

#pragma region Destruction
		/// <summary>
		/// Destructor method used to clean up a TerrainDebris object before deletion from system memory.
		/// </summary>
		~TerrainDebris() override { Destroy(true); }

		/// <summary>
		/// Destroys and resets (through Clear()) the TerrainDebris object.
		/// </summary>
		/// <param name="notInherited">Whether to only destroy the members defined in this derived class, or to destroy all inherited members also.</param>
		/// <remarks>Don't delete bitmaps since they are owned in the CoententFile static maps.</remarks>
		void Destroy(bool notInherited = false) override { if (!notInherited) { Entity::Destroy(); } Clear(); }

		/// <summary>
		/// Resets the entire TerrainDebris, including its inherited members, to their default settings or values.
		/// </summary>
		void Reset() override { Clear(); Entity::Reset(); }
#pragma endregion

#pragma region Getters
		/// <summary>
		/// Gets all the bitmaps of this collection of debris.
		/// </summary>
		/// <returns>A vector of pointers to BITMAPs. Ownership is NOT transferred.</returns>
		std::vector<BITMAP *> GetBitmaps() const { return m_Bitmaps; }

		/// <summary>
		/// Gets the current number of BITMAPs that describe all the different pieces of debris.
		/// </summary>
		/// <returns>The number of BITMAPs this TerrainDebris has.</returns>
		int GetBitmapCount() const { return m_BitmapCount; }
#pragma endregion

#pragma region Concrete Methods
		/// <summary>
		/// Applies the debris to a SLTerrain as its read parameters specify.
		/// </summary>
		/// <param name="terrain">Pointer to the SLTerrain to place the debris on. Ownership is NOT transferred!</param>
		void ApplyDebris(SLTerrain *terrain);
#pragma endregion

	private:

		/// <summary>
		/// Enumeration for the different debris placement modes.
		/// </summary>
		enum DebrisPlacementModes {
			NoPlacementRestrictions,
			OnSurfaceOnly,
			OnCavitySurfaceOnly,
			OnSurfaceAndCavitySurface,
			OnOverhangOnly,
			OnCavityOverhangOnly,
			OnOverhangAndCavityOverhang
		};

		static Entity::ClassInfo m_sClass; //!< ClassInfo for this class.

		ContentFile m_DebrisFile; //!< ContentFile containing the path to the debris sprites.
		std::vector<BITMAP *> m_Bitmaps; //!< All the different bitmaps of this debris. Not owned.
		int m_BitmapCount; //!< How many bitmaps this debris has.

		Material m_Material; //!< The material of the debris.
		Material m_TargetMaterial; //!< The target material in which this debris should exist in.

		DebrisPlacementModes m_DebrisPlacementMode; //!< This will determine how target material checking and debris applying should behave. If set to NoPlacementRestrictions, checking will continue to penetrate down into non-air materials to try to find the target material.
		bool m_OnlyBuried; //!< Whether to only place a piece of this if we find a spot for it to fit completely buried in the terrain.

		int m_MinDepth; //!< Minimum depth into the terrain contour. This can be negative for debris placed above ground.
		int m_MaxDepth; //!< Maximum depth into the terrain contour. This can be negative for debris placed above ground.

		float m_Density; //!< Approximate Density count per meter.

#pragma region Debris Application
		/// <summary>
		/// Checks if conditions apply for a debris piece to be placed to the terrain. The actual position is returned via the passed in Box's center position.
		/// </summary>
		/// <param name="terrain">Pointer to the SLTerrain to check debris placement on. Ownership is NOT transferred!</param>
		/// <param name="buriedCheckBox">A Box that holds the debris piece's dimensions for checking. The center of the Box will be modified during checking.</param>
		/// <returns>True if a valid placement position was found, which means the passed in Box's center or corner positions are good to be used as the piece's actual position.</returns>
		bool GetPiecePlacementPosition(SLTerrain *terrain, Box &buriedCheckBox) const;

		/// <summary>
		/// Checks whether the passed in pixel color value is of target material, and if extra conditions apply for it to be valid for placement, depending on this' DebrisPlacementMode.
		/// </summary>
		/// <param name="materialCheckPixel">The pixel color value to check.</param>
		/// <param name="prevMaterialCheckPixel">The previously checked pixel color value to check extra conditions with. Does not apply when DebrisPlacementMode is NoPlacementRestrictions.</param>
		/// <returns>Whether the passed in pixel color value is valid for debris placement.</returns>
		bool MaterialPixelIsValidTarget(int materialCheckPixel, int prevMaterialCheckPixel) const;
#pragma endregion

		/// <summary>
		/// Clears all the member variables of this TerrainDebris, effectively resetting the members of this abstraction level only.
		/// </summary>
		void Clear();

		// Disallow the use of some implicit methods.
		TerrainDebris(const TerrainDebris &reference) = delete;
		void operator=(const TerrainDebris &rhs) = delete;
	};
}
#endif