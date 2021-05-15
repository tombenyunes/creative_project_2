#pragma once

#include "ofxXmlSettings.h"

#include "EntityManager.h"
#include "GamemodeManager.h"

class SceneManager {
public:

		SceneManager();
		void init(Controller* game_controller, GUIManager* gui_manager, Camera* cam, FluidManager* fluid_manager, AudioManager* audio_manager, EntityManager* entity_manager, GamemodeManager* gamemode_manager);

		void update();

		void save_scene(string scene_name);
	
		void get_ready_for_new_scene() const;
	
		void load_scene(string path);
		void load_procedural_scene() const;
		void load_blank_scene();
	
		void destroy_current_scene() const;
		void reset_fluid() const;

		void key_pressed(int key);
		void key_released(int key);
		

private:

		Controller* game_controller_;
		GUIManager* gui_manager_;
		FluidManager* fluid_manager_;
		AudioManager* audio_manager_;
		EntityManager* entity_manager_;
		GamemodeManager* gamemode_manager_;

		Camera* cam_;

		ofxXmlSettings xml_;
		ofxXmlSettings xml1_;

		bool enter_pressed_{};
	
};