#include "Camera.h"

Camera::Camera()
{
	cam.disableMouseInput();
	cam.setVFlip(true);  // flipping the axis so it's as if we're in 2D, otherwise all the fluid/movement code will be fucked
	cam.enableOrtho();
	//cam.setScale(1);
	cam.setPosition(0 + ofGetWidth() / 2, 0 + ofGetHeight() / 2, 1305);

	//zoomDistance = 1080.0f; 

	cam.removeAllInteractions();
	//cam.addInteraction(ofEasyCam::TRANSFORM_SCALE, OF_MOUSE_BUTTON_RIGHT);
	cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);

	//cam.setNearClip(-1000000);
	//cam.setFarClip(1000000);
	
	followPlayer = true;
}

void Camera::update(int worldWidth, int worldHeight, ofVec2f _playerPos)
{
	if (followPlayer) {
		cam.setPosition(_playerPos.x, _playerPos.y, cam.getPosition().z); // camera follows player
	}
	else {
		cam.setPosition(0 + worldWidth / 2, 0 + worldHeight / 2, cam.getPosition().z); // camera follows player
	}
}

void Camera::toggleZoomMode()
{
	// reset scale/zoom
	cout << "Zoom reset" << endl;
	if ((cam.getScale().x == 1) && (cam.getScale().y == 1) && (cam.getScale().z == 1)) {
		cam.setScale(5.5, 5.5, 1);
		followPlayer = false;
	}
	else {
		cam.setScale(1);
		followPlayer = true;
	}
}

glm::mat4 Camera::getModelViewMatrix()
{
	return cam.getModelViewMatrix();	
}

glm::mat4 Camera::getLocalTransformMatrix()
{
	return cam.getLocalTransformMatrix();		
}

glm::mat4 Camera::getGlobalTransformMatrix()
{
	return cam.getGlobalTransformMatrix();
}

glm::mat4 Camera::getProjectionMatrix()
{	
	return cam.getProjectionMatrix();
}

glm::mat4 Camera::getModelViewProjectionMatrix()
{
	return cam.getModelViewProjectionMatrix();
}

ofVec3f Camera::getPosition()
{
	return cam.getPosition();
}

void Camera::begin()
{
	cam.begin();
}

void Camera::end()
{
	cam.end();
}

ofVec3f Camera::screenToWorld(ofVec3f _view)
{
	return cam.screenToWorld(_view);
}

void Camera::keyPressed(int key)
{
	if (key == 'z') {
		toggleZoomMode();
	}
	else if (key == 3682) {
		ctrlDown = true;
	}
}

void Camera::keyReleased(int key)
{
	if (key == 3682) {
		ctrlDown = false;
	}
}

void Camera::mouseDragged(int x, int y, int button)
{

}

void Camera::mousePressed(int x, int y, int button)
{

}

void Camera::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
	if (ctrlDown) {
		cam.setScale(cam.getScale().x + scrollY / 10, cam.getScale().y + scrollY / 10, 1);
		cout << "Zoom level: " << cam.getScale() << endl;
	}
}