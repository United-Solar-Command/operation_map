// This class MUST inherit from ScriptedGameTriggerEntityClass
class SpawnOnPlayerTriggerClass : ScriptedGameTriggerEntityClass
{
}

// This class MUST inherit from ScriptedGameTriggerEntity
[EntityEditorProps(category: "Game", description: "A trigger that spawns a specified entity on the activator's position.")]
class SpawnOnPlayerTrigger : ScriptedGameTriggerEntity
{

	// This Attribute makes the variable editable in the World Editor's object properties.
	[Attribute(defvalue: "{D8A41846A7ECC31D}Prefabs/Characters/Core/Character_Base.et", uiwidget: UIWidgets.ResourceNamePicker, desc: "The entity prefab to spawn when triggered.", params: "et")]
	private ResourceName m_sPrefabToSpawn;

	//------------------------------------------------------------------------------------------------
	// This method is automatically called when an entity activates the trigger.
	// The 'ent' is the entity that entered the trigger volume (e.g., the player).
	//------------------------------------------------------------------------------------------------
	override void OnActivate(IEntity ent)
	{
		// It's good practice to call the parent class's method.
		super.OnActivate(ent);

		// Check if the activating entity and the prefab path are valid before proceeding.
		if (!ent || m_sPrefabToSpawn.IsEmpty())
		{
			Print("SpawnOnPlayerTrigger: Activating entity or Prefab is not valid!", LogLevel.WARNING);
			return;
		}

		// Get the world position (origin) of the entity that activated the trigger.
		vector activatorPos = ent.GetOrigin();
		
		// Get the orientation of the activator to spawn the new entity facing the same direction.
		vector activatorAngles = ent.GetAngles();

		// Prepare the spawn parameters for the new entity.
		EntitySpawnParams spawnParams = new EntitySpawnParams();
		spawnParams.TransformMode = ETransformMode.WORLD; // We are using world coordinates.
		
		// Set the position and rotation for the new entity.
		Math3D.AnglesToMatrix(activatorAngles, spawnParams.Transform);
		spawnParams.Transform[3] = activatorPos; // The 4th row of the matrix represents position.

		// Log the action to the console for debugging purposes.
		Print(string.Format("SpawnOnPlayerTrigger: Attempting to spawn '%1' at %2", m_sPrefabToSpawn, activatorPos), LogLevel.NORMAL);
		
		// Load the resource specified in the editor.
		Resource resource = Resource.Load(m_sPrefabToSpawn);
		if (!resource.IsValid())
		{
			Print(string.Format("SpawnOnPlayerTrigger: Failed to load resource '%1'", m_sPrefabToSpawn), LogLevel.ERROR);
			return;
		}

		// Spawn the entity into the world using the prepared parameters.
		GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), spawnParams);
	}
}