#ifndef _RTELUAADAPTERS_
#define _RTELUAADAPTERS_

#include "MovableMan.h"
#include "ConsoleMan.h"

namespace RTE {

#pragma region Manager Lua Adapters
	/// <summary>
	/// Adds the given MovableObject to MovableMan if it doesn't already exist in there, or prints an error if it does.
	/// </summary>
	/// <param name="movableMan">A reference to MovableMan, provided by Lua.</param>
	/// <param name="movableObject">A pointer to the MovableObject to be added.</param>
	static void AddMO(MovableMan &movableMan, MovableObject *movableObject) {
		if (movableMan.ValidMO(movableObject)) {
			g_ConsoleMan.PrintString("ERROR: Tried to add a MovableObject that already exists in the simulation! " + movableObject->GetPresetName());
		} else {
			movableMan.AddMO(movableObject);
		}
	}

	/// <summary>
	/// Adds the given Actor to MovableMan if it doesn't already exist in there, or prints an error if it does.
	/// </summary>
	/// <param name="movableMan">A reference to MovableMan, provided by Lua.</param>
	/// <param name="actor">A pointer to the Actor to be added.</param>
	static void AddActor(MovableMan &movableMan, Actor *actor) {
		if (movableMan.IsActor(actor)) {
			g_ConsoleMan.PrintString("ERROR: Tried to add an Actor that already exists in the simulation!" + actor->GetPresetName());
		} else {
			movableMan.AddActor(actor);
		}
	}

	/// <summary>
	/// Adds the given item MovableObject (generally a HeldDevice) to MovableMan if it doesn't already exist in there, or prints an error if it does.
	/// </summary>
	/// <param name="movableMan">A reference to MovableMan, provided by Lua.</param>
	/// <param name="item">A pointer to the item to be added.</param>
	static void AddItem(MovableMan &movableMan, MovableObject *item) {
		if (movableMan.ValidMO(item)) {
			g_ConsoleMan.PrintString("ERROR: Tried to add an Item that already exists in the simulation!" + item->GetPresetName());
		} else {
			movableMan.AddItem(item);
		}
	}

	/// <summary>
	/// Adds the given particle MovableObject to MovableMan if it doesn't already exist in there, or prints an error if it does.
	/// </summary>
	/// <param name="movableMan">A reference to MovableMan, provided by Lua.</param>
	/// <param name="particle">A pointer to the pctor to be added.</param>
	static void AddParticle(MovableMan &movableMan, MovableObject *particle) {
		if (movableMan.ValidMO(particle)) {
			g_ConsoleMan.PrintString("ERROR: Tried to add a Particle that already exists in the simulation!" + particle->GetPresetName());
		} else {
			movableMan.AddParticle(particle);
		}
	}

	/// <summary>
	/// Gets the number of ticks per second. Lua can't handle int64 (or long long apparently) so we'll expose this specialized function.
	/// </summary>
	/// <returns>The number of ticks per second.</returns>
	static double GetTicksPerSecond(const TimerMan &timerMan) {
		return static_cast<double>(timerMan.GetTicksPerSecond());
	}
#pragma endregion

#pragma region Misc Lua Adapters
	/// <summary>
	/// Gets a random number between -1 and 1.
	/// </summary>
	/// <returns>A random number between -1 and 1.</returns>
	static double NormalRand() {
		return RandomNormalNum<double>();
	}

	/// <summary>
	/// Gets a random number between 0 and 1.
	/// </summary>
	/// <returns>A random number between 0 and 1.</returns>
	static double PosRand() {
		return RandomNum<double>();
	}

	/// <summary>
	/// Gets a random number between 1 and the passed in upper limit number. The limits are included in the available random range.
	/// </summary>
	/// <param name="upperLimitInclusive">The upper limit for the random number.</param>
	/// <returns>A random number between 1 and the passed in number.</returns>
	static double LuaRand(double upperLimitInclusive) {
		return RandomNum<double>(1, upperLimitInclusive);
	}

	/// <summary>
	/// Explicit deletion of any Entity instance that Lua owns. It will probably be handled by the GC, but this makes it instantaneous.
	/// </summary>
	/// <param name="entityToDelete">The Entity to delete.</param>
	static void DeleteEntity(Entity *entityToDelete) {
		delete entityToDelete;
		entityToDelete = nullptr;
	}
#pragma endregion
}
#endif