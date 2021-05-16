#include "GamemodeManager.h"

GamemodeManager::GamemodeManager(const int game_mode_id)
	:	game_started_(false)
	,	gui_manager_(nullptr)
	,	current_mode_id_(game_mode_id)
	,	prev_mode_id_(game_mode_id)
	,	request_for_procedural_scene_(false)
	,	request_for_blank_scene_(false)
	,	fading_(false)
	,	transitioning_(false)
	,	fill_alpha_(0)
	,	main_mode_started_(false)
{
	log_current_mode();

	potta_one_main_.load("Fonts/PottaOne-Regular.ttf", 16, true, true);
}

void GamemodeManager::init(GUIManager* gui_manager)
{
	gui_manager_ = gui_manager;
}

void GamemodeManager::update()
{
	scene_load_fade();
}

void GamemodeManager::draw()
{
	if (fading_)
	{
		ofPushMatrix();
		ofPushStyle();
		ofSetColor(0, 0, 0, fill_alpha_);
		ofDrawRectangle(0, 0, WORLD_WIDTH, WORLD_HEIGHT);
		ofSetColor(255, 255, 255, text_alpha1_);
		potta_one_main_.drawString("Click to propell yourself", HALF_WORLD_WIDTH - potta_one_main_.stringWidth("Click to propell yourself") / 2, HALF_WORLD_HEIGHT - potta_one_main_.stringHeight("Click to propell yourself"));
		ofSetColor(255, 255, 255, text_alpha2_);
		potta_one_main_.drawString("Follow the trail", HALF_WORLD_WIDTH - potta_one_main_.stringWidth("Follow the trail") / 2, HALF_WORLD_HEIGHT - potta_one_main_.stringHeight("Follow the trail") + 56);
		ofPopStyle();
		ofPopMatrix();
	}
}

int GamemodeManager::get_current_mode_id() const
{
	return current_mode_id_;
}

string GamemodeManager::get_current_mode_string() const
{
	string mode_text;
	switch (current_mode_id_)
	{
	case 0:
		mode_text = "Sandbox";
		break;
	case 1:
		mode_text = "Procedural";
		break;
	case 2:
		mode_text = "Menu";
		break;
	case 3:
		mode_text = "Main";
		break;
	default:
		mode_text = "[MODE ID UNDEFINED]";
		break;
	}
	return mode_text;
}

void GamemodeManager::set_current_mode_id(const int game_mode_id)
{
	current_mode_id_ = game_mode_id;
	
	switch (current_mode_id_)
	{
	case 0: // sandbox mode
		// show gui
		gui_manager_->set_gui_visible(true);
		game_started_ = true;
		break;
	case 1: // procedural mode
		// disable gui
		gui_manager_->set_gui_visible(false);
		game_started_ = true;
		break;
	case 2: // menu mode
		break;
	case 3: // main mode
		// disable gui
		gui_manager_->set_gui_visible(false);
		game_started_ = true;
		main_mode_started_ = true;
		break;
	default:
		break;
	}
	
	log_current_mode();
}

void GamemodeManager::log_current_mode() const
{
	cout << "----------GamemodeManager.cpp--------" << endl;
	cout << " - Game Mode: " << get_current_mode_string() << endl;
	cout << "----------------------------------------" << endl;
}

void GamemodeManager::scene_load_fade()
{
	if (fading_)
	{
		if (transitioning_text1_)
		{
			if (ofGetFrameNum() >= frame_before_transition_ + 30)
			{
				text_alpha1_ += 1;
				if (text_alpha1_ >= 255)
				{
					transitioning_text1_ = false;
				}
			}
		}
		if (transitioning_text2_)
		{
			if (ofGetFrameNum() >= frame_before_transition_ + 100)
			{
				text_alpha2_ += 1;
				if (text_alpha2_ >= 255)
				{
					transitioning_text2_ = false;
				}
			}
		}
		else if (ofGetFrameNum() >= frame_before_transition_ + 120)
		{
			fill_alpha_ -= 0.5f;
			text_alpha1_ -= 2;
			text_alpha2_ -= 2;
			
			if (text_alpha1_ <= 0)
			{
				transitioning_ = false;
			}
			if (fill_alpha_ <= 0)
			{
				fading_ = false;
			}
		}
	}
}

void GamemodeManager::transition_scene()
{
	fading_ = true;
	transitioning_ = true;
	fill_alpha_ = 255;
	text_alpha1_ = text_alpha2_ = 0;
	transitioning_text1_ = transitioning_text2_ = true;
	frame_before_transition_ = ofGetFrameNum();
}

void GamemodeManager::key_pressed(const int key)
{
	if (key == 32) // 'space' toggles between modes
	{
		if (get_current_mode_string() == "Sandbox")
		{
			set_current_mode_id(prev_mode_id_);
		}
		else if (get_current_mode_string() == "Procedural" || get_current_mode_string() == "Main")
		{
			prev_mode_id_ = get_current_mode_id();
			set_current_mode_id(0);
		}
	}
	else if (key == 27) // 'escape'
	{
		// enter menu
		if (get_current_mode_id() != 2)
		{
			prev_mode_id_ = get_current_mode_id();
			set_current_mode_id(2);
		}
		// exit menu
		else
		{				
			if (game_started_)
			{
				set_current_mode_id(prev_mode_id_);
			}
			else
			{
				set_current_mode_id(0);
			}
		}
	}
}

void GamemodeManager::mouse_pressed(const int x, const int y, const int button)
{
	if (get_current_mode_string() == "Menu")
	{
		if (button == 0 && gui_manager_->main_mode_bounds.intersects(ofRectangle(x, y, 0, 0)))
		{
			set_current_mode_id(3);
			set_request_for_main_mode(true);
			
			//transition_scene();
		}
		else if (button == 0 && gui_manager_->procedural_mode_bounds.intersects(ofRectangle(x, y, 0, 0)))
		{
			set_current_mode_id(1);
			set_request_for_procedural_scene(true);

			//transition_scene();
		}
		else if (button == 0 && gui_manager_->sandbox_mode_bounds.intersects(ofRectangle(x, y, 0, 0)))
		{
			set_current_mode_id(0);
			set_request_for_blank_scene(true);

			//transition_scene();
		}
	}
}
