// This class MUST inherit from ScriptComponentClass
class SpawnOnActivateComponentClass : ScriptComponentClass
{
}

/**
 * @class SpawnOnActivateComponent
 * @brief A component that spawns a specified entity at an activator's position.
 *
 * This component should be attached to a trigger entity.
 * When another component (e.g., SCR_CallActionComponent) calls the PerformSpawn method,
 * it will spawn a prefab at the location of the entity that activated the trigger.
 */
[EntityEditorProps(category: "Game", description: "Spawns a specified entity at the activator's position when called.")]
class SpawnOnActivateComponent : ScriptComponent
{
	// This Attribute makes the variable editable in the World Editor's object properties.
	[Attribute(defvalue: "{D8A41846A7ECC31D}Prefabs/Characters/Core/Character_Base.et", uiwidget: UIWidgets.ResourceNamePicker, desc: "The entity prefab to spawn.", params: "et")]
	private ResourceName m_sPrefabToSpawn;

	//------------------------------------------------------------------------------------------------
	/**
	 * @brief Spawns the configured prefab at the activator's position and orientation.
	 * This method is intended to be called by another trigger component, like SCR_CallActionComponent.
	 * @param activator The entity that triggered the action (e.g., the player).
	 */
	void PerformSpawn(IEntity activator)
	{
		// Check if the activating entity and the prefab path are valid before proceeding.
		if (!activator || m_sPrefabToSpawn.IsEmpty())
		{
			Print("SpawnOnActivateComponent: Activator entity or Prefab is not valid!", LogLevel.WARNING);
			return;
		}

		// Get the world position (origin) of the entity that activated the trigger.
		vector activatorPos = activator.GetOrigin();
		
		// Get the orientation of the activator to spawn the new entity facing the same direction.
		vector activatorAngles = activator.GetAngles();

		// Prepare the spawn parameters for the new entity.
		EntitySpawnParams spawnParams = new EntitySpawnParams();
		spawnParams.TransformMode = ETransformMode.WORLD; // We are using world coordinates.
		
		// Set the position and rotation for the new entity based on the activator.
		Math3D.AnglesToMatrix(activatorAngles, spawnParams.Transform);
		spawnParams.Transform[3] = activatorPos; // The 4th row of the matrix represents position.

		// Log the action to the console for debugging purposes.
		Print(string.Format("SpawnOnActivateComponent: Attempting to spawn '%1' at %2", m_sPrefabToSpawn, activatorPos), LogLevel.NORMAL);
		
		// Load the resource specified in the editor.
		Resource resource = Resource.Load(m_sPrefabToSpawn);
		if (!resource.IsValid())
		{
			Print(string.Format("SpawnOnActivateComponent: Failed to load resource '%1'", m_sPrefabToSpawn), LogLevel.ERROR);
			return;
		}

		// Spawn the entity into the world using the prepared parameters.
		GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), spawnParams);
	}
}